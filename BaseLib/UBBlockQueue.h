#ifndef _BLOCKQUEUE_H_
#define _BLOCKQUEUE_H_
#include "uv.h"
#include <queue>

template <typename T>
class UBBlockObjQueue
{
    public:
        enum{CACHE_CNT=100000};
        UBBlockObjQueue();
        UBBlockObjQueue(int iMaxCnt);
        ~UBBlockObjQueue();
        size_t size();
        void put(std::vector<T> &vec);
        int get(std::vector<T> &vec);
    private:
        const int m_iMaxCnt;
        std::queue<T> m_queue;
        uv_mutex_t m_mutex;
};

template <typename T>
UBBlockObjQueue<T>::UBBlockObjQueue():m_iMaxCnt(CACHE_CNT)
{
    uv_mutex_init(&m_mutex);
}

template <typename T>
UBBlockObjQueue<T>::UBBlockObjQueue(int iMaxCnt):m_iMaxCnt(iMaxCnt)
{
    uv_mutex_init(&m_mutex);
}

template <typename T>
UBBlockObjQueue<T>::~UBBlockObjQueue() 
{
    uv_mutex_lock(&m_mutex);
    while(m_queue.size()>0)
    {
        m_queue.pop();
    }

    uv_mutex_unlock(&m_mutex);

    uv_mutex_destroy(&m_mutex);
}

template <typename T>
void UBBlockObjQueue<T>::put(std::vector<T> &vec)
{
    uv_mutex_lock(&m_mutex);
    if (m_queue.size() > m_iMaxCnt)
    {
        usleep(100*1000);
    }
    int iVecSize = vec.size();
    for (int iIndex = 0; iIndex < iVecSize; iIndex++)
    {
        m_queue.push(vec[iIndex]);
    }
    
    uv_mutex_unlock(&m_mutex);
}

template <typename T>
int UBBlockObjQueue<T>::get(std::vector<T> &vec)
{
    uv_mutex_lock(&m_mutex);
    int iQueSize = m_queue.size();
    if (iQueSize <= 0)
    {
        uv_mutex_unlock(&m_mutex);
        return 0;
    }
    for (int i = 0; i < iQueSize; i++)
    {
        vec.push_back(m_queue.front());
        m_queue.pop();
    }
    uv_mutex_unlock(&m_mutex);
    return vec.size();
}


template<typename T>
size_t UBBlockObjQueue<T>::size()
{
    size_t size = 0;
    uv_mutex_lock(&m_mutex);
    size = m_queue.size();
    uv_mutex_unlock(&m_mutex);
    return size;
}


template <typename T>
class UBBlockQueue
{
    public:
        enum{CACHE_CNT=100000};
        UBBlockQueue();
        UBBlockQueue(int iMaxCnt);
        ~UBBlockQueue();
        T* get();
        void put(T* data);
        size_t size();
        void put(std::vector<T*> &vec);
        int get(std::vector<T*> &vec);
    private:
        const int m_iMaxCnt;
        std::queue<T*> m_queue;
        uv_mutex_t m_mutex;
};

template <typename T>
UBBlockQueue<T>::UBBlockQueue():m_iMaxCnt(CACHE_CNT)
{
    uv_mutex_init(&m_mutex);
}

template <typename T>
UBBlockQueue<T>::UBBlockQueue(int iMaxCnt):m_iMaxCnt(iMaxCnt)
{
    uv_mutex_init(&m_mutex);
}


template <typename T>
UBBlockQueue<T>::~UBBlockQueue() 
{
    uv_mutex_lock(&m_mutex);
    while(m_queue.size()>0)
    {
        delete m_queue.front();
        m_queue.pop();
    }

    uv_mutex_unlock(&m_mutex);

    uv_mutex_destroy(&m_mutex);
}

template <typename T>
T* UBBlockQueue<T>::get()
{
    uv_mutex_lock(&m_mutex);
    if (m_queue.empty())
    {
        uv_mutex_unlock(&m_mutex);
        return NULL;
    }
    T* data = m_queue.front();
    m_queue.pop();
    uv_mutex_unlock(&m_mutex);
    return data;
}
template <typename T>
void UBBlockQueue<T>::put(T* data)
{
    if(data==NULL)return;

    uv_mutex_lock(&m_mutex);
    if (m_queue.size() > m_iMaxCnt)
    {
        usleep(100*1000);
    }   
    m_queue.push(data);
    uv_mutex_unlock(&m_mutex);
}
template <typename T>
void UBBlockQueue<T>::put(std::vector<T*> &vec)
{
    uv_mutex_lock(&m_mutex);
    if (m_queue.size() > m_iMaxCnt)
    {
        usleep(100*1000);
    }
    int iVecSize = vec.size();
    for (int iIndex = 0; iIndex < iVecSize; iIndex++)
    {
        if(vec[iIndex]==NULL)continue;
        m_queue.push(vec[iIndex]);
    }
    
    uv_mutex_unlock(&m_mutex);
}

template <typename T>
int UBBlockQueue<T>::get(std::vector<T*> &vec)
{
    uv_mutex_lock(&m_mutex);
    int iQueSize = m_queue.size();
    if (iQueSize <= 0)
    {
        uv_mutex_unlock(&m_mutex);
        return 0;
    }
    for (int i = 0; i < iQueSize; i++)
    {
        vec.push_back(m_queue.front());
        m_queue.pop();
    }
    uv_mutex_unlock(&m_mutex);
    return vec.size();
}


template<typename T>
size_t UBBlockQueue<T>::size()
{
    size_t size = 0;
    uv_mutex_lock(&m_mutex);
    size = m_queue.size();
    uv_mutex_unlock(&m_mutex);
    return size;
}
#endif
