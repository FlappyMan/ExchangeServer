/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-16 18:35:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/ThreadDBOper.cpp
 */
#include "ThreadDBOper.h"
#include "DispatchManager.h"
#include "BaseRequestOper.h"
uv_loop_t g_uvDBLoop;
uv_timer_t g_uvDBTimer;
UBBlockQueue<CBaseRequestOper> CThreadDBOper::m_qDBOper;
UBBlockQueue<CDBResult> CThreadDBOper::m_qDBResult;
UBBlockQueue<CJsonObjectBase> CThreadDBOper::m_qDBJsonObjOper;
UBBlockQueue<CJsonObjectBase> CThreadDBOper::m_qDBResultJsonOper;
string CThreadDBOper::g_sConfigFileUpdateTime = "0";
void CThreadDBOper::cbTimer(uv_timer_t *handle)
{
	if (CDispatchManager::getHour() =="06")
	{
		if (CDispatchManager::getMin() =="00")
		{
			string sConfigPath = g_dir_root + "BaseConfig/RuntimeConfig.json";
			if (CRuntimeConfig::Load(sConfigPath, CThreadDBOper::g_sConfigFileUpdateTime) == false)
			{
				// DEBUG_ERROR("Load config file failed");
			}
			else
			{
				// DEBUG_INFO("服务器配置信息加载完毕");
			}
		}
	}

	while (CThreadDBOper::m_qDBOper.size() != 0)
	{
		CDBResult *pResult = new CDBResult;
		pResult->m_cOper = CThreadDBOper::m_qDBOper.get();
		pResult->m_cProt = pResult->m_cOper->requestOperation();
		if (NULL == pResult->m_cProt)
		{
			delete pResult->m_cOper;
			delete pResult;
			continue;
		}
		pResult->m_cProt->m_sTag = pResult->m_cOper->m_sTag;
		
		CThreadDBOper::m_qDBResult.put(pResult);
	}

	std::vector<CJsonObjectBase *> vecResp;
	std::vector<CJsonObjectBase *> vecArray;
	int iResultSize = CThreadDBOper::m_qDBJsonObjOper.get(vecResp);
	for (int i = 0; i < iResultSize; i++)
	{
		CJsonObjectBase *pObj = vecResp[i];
		CJsonObjectBase *pResponse = pObj->requestOperation();

		delete pObj;
		pObj = NULL;
		
		if (NULL == pResponse)
		{
			continue;
		}
		vecArray.push_back(pResponse);
	}

	if (vecArray.size() != 0)
	{
		CThreadDBOper::m_qDBResultJsonOper.put(vecArray);
	}
}

void CThreadDBOper::ThreadDBOper(void *arg)
{
	DEBUG_INFO("[CThreadDBOper Init]");
	CDispatchManager::DBInit();
	uv_loop_init(&g_uvDBLoop);
	uv_timer_init(&g_uvDBLoop, &g_uvDBTimer);
	uv_timer_start(&g_uvDBTimer, cbTimer, 10, 1);
	
	uv_run(&g_uvDBLoop, UV_RUN_DEFAULT);
}
