#ifndef _TASKWRAPPER_
#define _TASKWRAPPER_
#include "global.h"

template<class T>
class Task
{
    public:
        Task(){};
        ~Task(){};

        static void DoTask(T* tskQueueData);
    private:
        
};
template<class T>
void Task<T>::DoTask(T* tskQueueData)
{
	string sError;
	char sql[1024] = {0};
	int iSqlLen = 0;
	uint32_t uiResult = 0;

	//每个线程创建自己的mysql实例连接目标数据库
	UBMysql* ubDstMysql = new UBMysql;
	string sDB = "";
	if (!ubDstMysql->Connect(g_config.m_new_DB_ip,g_config.m_new_DB_port,g_config.m_new_DB_user,g_config.m_new_DB_pwd,sDB))
	{
		cout<<"Connect new database failed!"<<endl;
		return;
	}

	TableData* tbData = NULL;
	while (tskQueueData->Size()>0)
	{
		g_cnt++;//数据计数器
		tskQueueData->Pop(tbData);

		//获取数据tm结构
		struct tm _tm= GetYearFromTimestamp(tbData->iAddtime);
		int iYear = _tm.tm_year+1900;
		int iMon = _tm.tm_mon+1;
		int iDay = _tm.tm_mday;
		string sYear = std::to_string(iYear);
		char sMon[3] = {0};
		snprintf(sMon,3,"%02d",iMon);
		char sYearMon[7] = {0};
		snprintf(sYearMon,7,"%04d%02d",iYear,iMon);

		//高频交易用户库 trade_user_business
		if(tbData->uiUserID==29 || tbData->uiUserID==30 || tbData->uiUserID==10780 || tbData->uiUserID==10272 || 
			tbData->uiUserID==10228 || tbData->uiUserID==11422 || tbData->uiUserID==10637)
		{
			iSqlLen = snprintf(sql,1024,"Create Database If Not Exists trade_user_business Character Set UTF8;");
			if(ubDstMysql->Exec(sql,iSqlLen) < 0)
			{
				string sErr;
				ubDstMysql->Error(sErr);
				cout << "create trade_user_business DB failed:" << sErr << endl;
				//return;
			}
		reInsert:
			iSqlLen = snprintf(sql,1024,"REPLACE INTO `trade_user_business`.`%lu`("
					"`id`,`peerid`,`price`,`num`,`mum`,"
					"`fee_buy`,`fee_sell`,`type`,`sort`,`addtime`,`endtime`,`status`,`buy_id`,`sell_id`) values("
					"%lu,%lu,"
					"%s,%s,%s,%s,%s,"
					"%d,%lu,%ld,%ld,%d,%ld,%lu);",
					tbData->uiUserID,
					tbData->uiID,tbData->uiPeerid,
					tbData->sPrice.c_str(),tbData->sNum.c_str(),tbData->sMum.c_str(),tbData->sFee_buy.c_str(),tbData->sFee_sell.c_str(),
					tbData->uiType,tbData->uiSort,tbData->iAddtime,tbData->iEndtime,tbData->iStatus,tbData->uiBuy_id,tbData->uiSell_id);
			int ret = ubDstMysql->Exec(sql,iSqlLen);
			if(ret<0)
			{
			reCreate:
				iSqlLen = snprintf(sql,2048,"CREATE TABLE if not exists `trade_user_business`.`%lu` ("
						"`id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,"
						//"`userid` int(11) unsigned NOT NULL,"
						"`peerid` int(11) unsigned NOT NULL,"
						//"`market` varchar(50) NOT NULL,"
						"`price` char(33) NOT NULL,"
						"`num` char(33) NOT NULL,"
						"`mum` char(33) NOT NULL,"
						"`fee_buy` char(33) NOT NULL,"
						"`fee_sell` char(33) NOT NULL,"
						"`type` tinyint(2) unsigned NOT NULL COMMENT '类型:1买入,2卖出',"
						"`sort` int(11) unsigned NOT NULL,"
						"`addtime` int(11) unsigned NOT NULL,"
						"`endtime` int(11) unsigned NOT NULL,"
						"`status` int(4) NOT NULL,"
						"`buy_id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '买方挂单id',"
						"`sell_id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '卖方挂单id',"
						"PRIMARY KEY (`id`),"
						//"KEY `userid` (`userid`),"
						"KEY `peerid` (`peerid`),"
						"KEY `main` (`addtime`) USING BTREE"
					") ENGINE=myisam DEFAULT CHARSET=utf8;",tbData->uiUserID);
				ret = ubDstMysql->Exec(sql,iSqlLen);
				if (ret<0)
				{
					int iLastError = ubDstMysql->Error();
					cout << "create user table error num:" << iLastError << endl;

					string sLastError;
					ubDstMysql->Error(sLastError);
					cout<<"create user table error:"<<sLastError<<endl;
					goto reCreate;
				}
				goto reInsert;		
			}
			continue;
		}

		iSqlLen = snprintf(sql,1024,"Create Database If Not Exists trade_user_%s Character Set UTF8;",sYear.c_str());
		if(ubDstMysql->Exec(sql,iSqlLen) < 0)
		{
			string sErr;
			ubDstMysql->Error(sErr);
			cout << "create trade_user DB failed:" << sErr << endl;
			//return;
		}
		iSqlLen = snprintf(sql,1024,"Create Database If Not Exists trade_market_%s%s Character Set UTF8;",sYear.c_str(),sMon);
		if(ubDstMysql->Exec(sql,iSqlLen) < 0)
		{
			string sErr;
			ubDstMysql->Error(sErr);
			cout << "create trade_market DB failed:" << sErr << endl;
			//return;
		}
	
		//user库
		{
		reInsertUserTable:
			int iUserTable = tbData->uiUserID%g_config.m_UserTableCount;
			iSqlLen = snprintf(sql,1024,"REPLACE INTO `trade_user_%d`.`%03d`("
					"`id`,`userid`,`peerid`,`price`,`num`,`mum`,"
					"`fee_buy`,`fee_sell`,`type`,`sort`,`addtime`,`endtime`,`status`,`buy_id`,`sell_id`) values("
					"%lu,%lu,%lu,"
					"%s,%s,%s,%s,%s,"
					"%d,%lu,%ld,%ld,%d,%lu,%lu);",
					iYear,iUserTable,
					tbData->uiID,tbData->uiUserID,tbData->uiPeerid,
					tbData->sPrice.c_str(),tbData->sNum.c_str(),tbData->sMum.c_str(),tbData->sFee_buy.c_str(),tbData->sFee_sell.c_str(),
					tbData->uiType,tbData->uiSort,tbData->iAddtime,tbData->iEndtime,tbData->iStatus,tbData->uiBuy_id,tbData->uiSell_id);		
			int ret = ubDstMysql->Exec(sql,iSqlLen);				
			if(ret<0)
			{
			reCreateUTable:
				iSqlLen = snprintf(sql,2048,"CREATE TABLE if not exists `trade_user_%d`.`%03d`("
						"`id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,"
						"`userid` int(11) unsigned NOT NULL,"
						"`peerid` int(11) unsigned NOT NULL,"
						// "`market` varchar(50) NOT NULL,"
						"`price` char(33) NOT NULL,"
						"`num` char(33) NOT NULL,"
						"`mum` char(33) NOT NULL,"
						"`fee_buy` char(33) NOT NULL,"
						"`fee_sell` char(33) NOT NULL,"
						"`type` tinyint(2) unsigned NOT NULL COMMENT '类型:1买入,2卖出',"
						"`sort` int(11) unsigned NOT NULL,"
						"`addtime` int(11) unsigned NOT NULL,"
						"`endtime` int(11) unsigned NOT NULL,"
						"`status` int(4) NOT NULL,"
						"`buy_id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '买方挂单id',"
						"`sell_id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '卖方挂单id',"
						"PRIMARY KEY (`id`),"
						"KEY `userid` (`userid`),"
						"KEY `peerid` (`peerid`)"
						//"KEY `main` (`addtime`) USING BTREE"
					") ENGINE=myisam DEFAULT CHARSET=utf8;",iYear,iUserTable);
				int ret = ubDstMysql->Exec(sql,iSqlLen);
				if(ret < 0)
				{
					int iLastError = ubDstMysql->Error();
					cout << "create userid%1000 table error num:" << iLastError << endl;

					string sLastError;
					ubDstMysql->Error(sLastError);
					cout<<"create userid%1000 table error:"<<sLastError<<endl;
					goto reCreateUTable;
				}
				//再次插入数据
				goto reInsertUserTable;
			}	
		}
		//market库
		{
		reInsertMarketTable:
			iSqlLen = snprintf(sql,1024,"REPLACE INTO `trade_market_%04d%02d`.`%d_%02d`("
					"`id`,`price`,`num`,`mum`,"
					"`fee_buy`,`fee_sell`,`type`,`sort`,`endtime`,`status`,`buy_id`,`sell_id`) values("
					"%lu,"
					"%s,%s,%s,%s,%s,"
					"%d,%lu,%ld,%d,%lu,%lu);",
					iYear,iMon,tbData->uiMarketID,iDay,
					tbData->uiID,
					tbData->sPrice.c_str(),tbData->sNum.c_str(),tbData->sMum.c_str(),tbData->sFee_buy.c_str(),tbData->sFee_sell.c_str(),
					tbData->uiType,tbData->uiSort,tbData->iEndtime,tbData->iStatus,tbData->uiBuy_id,tbData->uiSell_id);
			int ret = ubDstMysql->Exec(sql,iSqlLen);				
			if(ret<0)
			{
			reCreateTMarket:
				iSqlLen = snprintf(sql,2048,"CREATE TABLE if not exists `trade_market_%04d%02d`.`%d_%02d`("
						"`id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,"
						// "`userid` int(11) unsigned NOT NULL,"
						// "`peerid` int(11) unsigned NOT NULL,"
						"`price` char(33) NOT NULL,"
						"`num` char(33) NOT NULL,"
						"`mum` char(33) NOT NULL,"
						"`fee_buy` char(33) NOT NULL,"
						"`fee_sell` char(33) NOT NULL,"
						"`type` tinyint(2) unsigned NOT NULL COMMENT '类型:1买入,2卖出',"
						"`sort` int(11) unsigned NOT NULL,"
						//"`addtime` int(11) unsigned NOT NULL,"
						"`endtime` int(11) unsigned NOT NULL,"
						"`status` int(4) NOT NULL,"
						"`buy_id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '买方挂单id',"
						"`sell_id` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '卖方挂单id',"
						"PRIMARY KEY (`id`)"
						// "KEY `userid` (`userid`),"
						// "KEY `peerid` (`peerid`)"
						//"KEY `main` (`addtime`) USING BTREE"
						") ENGINE=myisam DEFAULT CHARSET=utf8;",
						iYear,iMon,tbData->uiMarketID,iDay);
				int ret = ubDstMysql->Exec(sql,iSqlLen);
				if(ret < 0)
				{
					string sLastError;
					ubDstMysql->Error(sLastError);
					cout<<"create marketid_day table error:"<<sLastError<<endl;
					goto reCreateTMarket;
				}
				//再次插入数据
				goto reInsertMarketTable;		
			}
		}
	}
	cout<<"g_cnt : "<<g_cnt<<endl;
	delete ubDstMysql;   
}
#endif