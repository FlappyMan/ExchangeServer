/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-07-01 23:30:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/ThreadDB.cpp
 */
#include "ThreadDB.h"
#include "DispatchManager.h"
#include "ThreadControl.h"
#include "ThreadMatch.h"
uv_loop_t g_uvDBLoop;
uv_timer_t g_uvDBTimer;

#define THIS_LOOP g_uvDBLoop
#define THIS_TIMER g_uvDBTimer

UBBlockQueue<CBaseModel> CThreadDB::m_qDBQueue;

void CThreadDB::cbTimer(uv_timer_t *handle)
{
	// 从UC线程获取过来的数据，进行数据库操作如（冻结资产，解冻资产）
	vector<CBaseModel *> vecToMatchQueue;
	CThreadDB::m_qDBQueue.get(vecToMatchQueue);
	for (int i = 0; i < vecToMatchQueue.size(); i++)
	{
		CBaseModel *pSession = vecToMatchQueue[i];
		if (pSession->modelType != OPER_TYPE_MATCH_SUC)
		{
			pSession->recordToDB(CDispatchManager::g_DBTrade,CTool::getNowTime());
		}
		else
		{
			pSession->recordToDB(CDispatchManager::g_DBTrade,CDispatchManager::g_DBAssetsLog,CTool::getNowTime());
		}
		
		
	}
	CThreadControl::m_qContrlQueue.put(vecToMatchQueue);
}
void CThreadDB::ThreadDB(void *arg)
{
	spdlog::info("[CThreadDB Init]");
	CDispatchManager::DBInit();
	uv_loop_init(&THIS_LOOP);
	uv_timer_init(&THIS_LOOP, &THIS_TIMER);
	uv_timer_start(&THIS_TIMER, cbTimer, 10, 1);
	uv_run(&THIS_LOOP, UV_RUN_DEFAULT);
}
