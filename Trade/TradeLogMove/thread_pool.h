#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include <memory> // std::shared_ptr
#include <mutex>
#include <queue>
#include <vector>
#include <atomic>
#include <thread>
#include <assert.h>
#include <condition_variable>

#include "TaskWrapper.h"

namespace tp {
  	template<class T>
	class TaskQueue {
		private:
			std::queue<T*> queue_;
			std::mutex mutex_;
			std::condition_variable cv_;

		public:
		  explicit TaskQueue(void) {}
			~TaskQueue(void) {}

			void Push(T* msg) noexcept {
				{
					std::unique_lock<std::mutex> lock(mutex_);
					queue_.emplace(msg);
				}

				cv_.notify_one();
			} 

			void Pop(T* &v) noexcept {
				std::unique_lock<std::mutex> lock(mutex_);
				cv_.wait(lock, [this] { return !queue_.empty(); });//cv上锁前加锁mutex,cv上锁后解锁mutex,线程阻塞
				v = queue_.front();
				queue_.pop();
			}

			bool TryPop(T* &v) noexcept {
				std::unique_lock<std::mutex> lock(mutex_);
				if (queue_.empty())
					return false;

				v = queue_.front();
				queue_.pop();
				return true;
			}

			bool Empty() noexcept {
				std::unique_lock<std::mutex> lock(mutex_);
				return queue_.empty();
			}

			size_t Size() noexcept {
				std::unique_lock<std::mutex> lock(mutex_);
				return queue_.size();
			}
	};

	class ThreadsJoiner {
		private:
			std::vector<std::thread> &threads_;

		public:
			explicit ThreadsJoiner(std::vector<std::thread> &threads)
				: threads_(threads) {}

			~ThreadsJoiner() {
				for (auto& i : threads_) {
					if (i.joinable())
						i.join();
				}
			}
	};

	struct ThreadStatistics {
		int64_t total_executed_tasks = 0;
		int64_t max_elapsed_us = 0;
		int64_t min_elapsed_us = 0;
		int64_t avg_elapsed_us = 0;
		ThreadStatistics(void) {}
	};

  	template<class T>
	class TaskExecutionMgr {
		private:
			std::string executor_name_;
			uint32_t threads_count_;
			std::atomic_bool done_;
			std::vector<std::shared_ptr<TaskQueue<T>>> work_queues_;//工作队列vector<queue>
			std::vector<std::shared_ptr<ThreadStatistics>> work_statistics_;
			std::vector<std::thread> work_threads_;//线程队列vector<thread>
			ThreadsJoiner threads_joiner_;

			struct TaskWrapper { //阻塞队列中获取task队列
				T* msg = nullptr;
				explicit TaskWrapper(std::shared_ptr<TaskQueue<T>> queue) { queue->Pop(msg); }
				~TaskWrapper(void) { delete msg; }
			};

		public:
			TaskExecutionMgr()
				: executor_name_("default"), threads_count_(1), done_(false),
				threads_joiner_(work_threads_) {}//初始化列表
			~TaskExecutionMgr() 
			{ 
				done_ = true;
			}
			
			void Stop()
			{
				T* msg = NULL;
				for (auto& i : work_queues_)
				{
					msg = new T;
					i->Push(msg);//工作队列全部置空
				}
			}

			void ThreadJoiner()
			{
				for (auto& i : work_threads_) {
					if (i.joinable())
						i.join();
				}
			}

			bool Initialize(uint32_t threads_count,
					std::string &&executor_name) noexcept {
				threads_count_ = (0 == threads_count) ? std::thread::hardware_concurrency()//自定义或者运行时决定线程的数量
					: threads_count;
				assert(threads_count_ > 0);
				executor_name_ = std::move(executor_name);

				try {
					for (uint32_t i = 0; i < threads_count_; ++i) {
						work_queues_.emplace_back(std::make_shared<TaskQueue<T>>());
					}
					for (uint32_t i = 0; i < threads_count_; ++i) {
						work_threads_.emplace_back(std::thread(&TaskExecutionMgr::Run, this, i));
					}					

					cout << "executor [" << executor_name_ << "] initialized, "
						<< work_queues_.size()<<endl;
					return true;
				} catch (...) {
					done_ = true;

					return false;
				}
			}

			size_t DispatchToIdlestThread(T* msg) noexcept {
				const size_t tid = this->GetIdlestThreadId();
				assert(tid < work_threads_.size());
				work_queues_[tid]->Push(msg);
				return tid;
			}

			void DispatchToFixedThread(T* msg, size_t tid) noexcept {
				assert(tid < work_threads_.size());
				work_queues_[tid]->Push(msg);
			}

			inline size_t GetPoolSize(void) const { return threads_count_; }

			void DumpStatistics(void) {
				size_t tid = 0;
				for (const auto &s : work_statistics_) {
					cout << "executor [" << executor_name_ << "], thread #" << tid
						<< ": "
						<< "total executed tasks: " << s->total_executed_tasks
						<< "; max elapsed us: " << s->max_elapsed_us
						<< "; mix elapsed us: " << s->min_elapsed_us<<endl;;
				}
			}

		private:
			void Run(uint32_t thread_id) noexcept {
				std::shared_ptr<TaskQueue<T>> queue = work_queues_[thread_id];
				assert(queue);
				if (executor_name_ == "TASK") {
					while (!done_) {
						TaskWrapper task_wrapper(queue);
						T* request = task_wrapper.msg;
						if(request->Size()==0)break;//工作队列为空,退出线程的条件						
						Task<T>::DoTask(request);
					}
				}
			}

			size_t GetIdlestThreadId(void) noexcept {
				assert(work_queues_.size() > 0);
				size_t idlest_queue_id = 0;
				size_t idlest_queue_size = work_queues_[0]->Size();
				if (0 == idlest_queue_size) {
					return idlest_queue_id;
				}

				for (size_t qid = 1; qid < work_queues_.size(); qid++) {
					if (work_queues_[qid]->Empty()) {
						return qid;
					}

					const size_t queue_size = work_queues_[qid]->Size();
					if (queue_size < idlest_queue_size) {
						idlest_queue_size = queue_size;
						idlest_queue_id = qid;
					}
				}
				return idlest_queue_id;
			}
	};
}

#endif
