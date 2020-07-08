#include "TaskDispatch.h"
#include "thread_pool.h"

void ThreadTradeLog(void* args)
{
	string sError;
	char sql[1024] = {0};
	int iSqlLen = 0;
	bool bHasValue = false;
	uint32_t uiResult = 0;
	uint64_t uiNewID = 0;

	TableData* tbData = NULL;
	//login MySql
	string sDB = "";
	if (!g_mysql.Connect(g_config.m_DB_ip,g_config.m_DB_port,g_config.m_DB_user,g_config.m_DB_pwd,g_config.m_DB_name))
	{
		cout<<"Connect source database failed!"<<endl;
        cout<<"ip : "<<g_config.m_DB_ip<<endl;
        cout<<"port : "<<g_config.m_DB_port<<endl;
        cout<<"username : "<<g_config.m_DB_user<<endl;
        cout<<"dbname : "<<g_config.m_DB_name<<endl;
		return;
	}

	//获取marketName,marketID列表
	GetMarketMap();

	int32_t iThreadCnt = g_config.m_threadCnt;
	uv_thread_t tTradeLog[iThreadCnt];

	tp::TaskQueue<TableData> *tskQueueTbData;
	tp::TaskExecutionMgr<tp::TaskQueue<TableData>>* tskExecMgr = (tp::TaskExecutionMgr<tp::TaskQueue<TableData>>*)args;
	tskExecMgr->Initialize(iThreadCnt,"TASK");

	//遍历所有源数据库
	std::vector<string>::iterator it = g_vec_tableName_src.begin();
	for (; it != g_vec_tableName_src.end(); it++)
	{
		uint64_t uiBeginID = 0;
		int32_t iTotalPerTime = g_config.m_totalPerTime;
		while(1)
		{
			int iCount = 0;
			iSqlLen = snprintf(sql,1024,"SELECT * FROM %s.%s where id>%lu order by id limit %d;",g_config.m_DB_name.c_str(),(*it).c_str(),uiBeginID,iTotalPerTime);
			bool bHasValue = g_mysql.Select(sql,iSqlLen);
			if(bHasValue)
			{          
				tskQueueTbData = new tp::TaskQueue<TableData>;   
				while (g_mysql.NextRow())
				{
					++iCount;
					tbData = new TableData;
					g_mysql.Value(0, tbData->uiID);
					g_mysql.Value(1, tbData->uiUserID);
					g_mysql.Value(2, tbData->uiPeerid);
					g_mysql.Value(3, tbData->sMarket);
					g_mysql.Value(4, tbData->sPrice);
					g_mysql.Value(5, tbData->sNum);
					g_mysql.Value(6, tbData->sMum);
					g_mysql.Value(7, tbData->sFee_buy);
					g_mysql.Value(8, tbData->sFee_sell);
					g_mysql.Value(9, tbData->uiType);
					g_mysql.Value(10, tbData->uiSort);
					g_mysql.Value(11, tbData->iAddtime);
					g_mysql.Value(12, tbData->iEndtime);
					g_mysql.Value(13, tbData->iStatus);
					g_mysql.Value(14, tbData->uiBuy_id);
					g_mysql.Value(15, tbData->uiSell_id);
					//计算新表id字段
					uiNewID = BuildOrderID(tbData->iAddtime,0);
					//map中获取市场name对应的市场id
					uint32_t iMarketId = 0;
					map<string, uint32_t>::iterator iter;
					iter = g_mMarketMap.find(tbData->sMarket);
					if(iter != g_mMarketMap.end())
					{
						iMarketId = iter->second;
					}
					if(tbData->uiID > uiBeginID)
					{
						uiBeginID = tbData->uiID;
					}
					tbData->uiID = uiNewID;
					tbData->uiMarketID = iMarketId;
					tskQueueTbData->Push(tbData);
				}
				tskExecMgr->DispatchToIdlestThread(tskQueueTbData);    
			}
			if(iCount < iTotalPerTime)
			{
				tskExecMgr->Stop();
				break;
			}       
		}     
	}
}
