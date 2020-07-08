#include "global.h"


string g_dir_root; 
Config g_config;
UBMysql g_mysql(2048);
UBMysql g_mysql_dst(2048);  

set<uint32_t> g_sYear;
map<string,uint32_t> g_mMarketMap;
std::vector<string> g_vec_tableName_src;

atomic_int g_cnt(0);
uv_timeval_t g_timeBegin;
uv_timeval_t g_timeEnd;

void GetAllSrcTableName()
{
	//添加无日期的表
	g_vec_tableName_src.push_back("coin_trade_log");
	// 获取源数据库所有表名，带日期的表名默认从2019年01月01日开始
	// 设置起始时间点
	int iBeginYear = g_config.m_dbBeginYear;
	struct tm _tm;
	_tm.tm_hour = 0;
	_tm.tm_min = 0;
	_tm.tm_sec = 0;
	_tm.tm_isdst = 0;
	_tm.tm_year = iBeginYear - 1900;
	_tm.tm_mon = 0;
	_tm.tm_mday = 1;
	time_t tBegin = mktime(&_tm);

	// 当前时间
	time_t tNow = time(NULL);
	struct tm _tmNow;
	localtime_r(&tNow, &_tmNow);

	// 循环枚举所有数据库表
	int iSyncYear = 0;
	time_t tLoop = 0;
	UBDate ubDate(g_config.m_dbEndYear, g_config.m_dbEndMonth, 1);
	while (true)
	{
		_tm.tm_hour = 0;
		_tm.tm_min = 0;
		_tm.tm_sec = 0;
		_tm.tm_year = ubDate.Year()-1900;
		_tm.tm_mon = ubDate.Month()-1;
		_tm.tm_mday = ubDate.Day();
		tLoop = mktime(&_tm);
		if (tLoop < tBegin)
			break;
		
		char sTableName[128] = {0};
		snprintf(sTableName,128,"coin_trade_log_%02d%02d%02d",ubDate.Year()%100,ubDate.Month(),ubDate.Day());
		g_vec_tableName_src.push_back(sTableName);

		ubDate--;
	}
}

void GetMarketMap()
{
	string sError;
	char sql[1024] = {0};
	int iSqlLen = 0;
	uint32_t uiResult = 0;

	iSqlLen = snprintf(sql,1024,"SELECT id,name FROM %s.coin_market;",g_config.m_DB_name.c_str());
	bool bHasValue = g_mysql.Select(sql,iSqlLen);
	if (bHasValue)
	{
		uint64_t uiID = 0;
		string sName;
		while (g_mysql.NextRow())
		{
			g_mysql.Value(0,uiID);
			g_mysql.Value(1,sName);
			g_mMarketMap.insert(pair<string,uint32_t>(sName,uiID));
		}
	}
}

int64_t GetCurTime() 
{
    struct timeval tv_;
    gettimeofday(&tv_, NULL);
    return tv_.tv_sec * 1000 + tv_.tv_usec / 1000;
}

struct tm GetYearFromTimestamp(uint64_t uiTimestamp)
{
    time_t tick = (time_t)uiTimestamp;

    struct tm _tm;
    _tm = *localtime(&tick);

    return _tm;
}

int64_t g_last_timestamp = 0;
uint32_t g_auto_inc = 0;
UBLock g_auto_inc_lock;

int64_t BuildOrderID(int64_t uiTimeStamp, uint64_t uiServerID)
{
	if (g_last_timestamp != uiTimeStamp)
	{
		g_auto_inc_lock.Lock();
		g_auto_inc = 0;
		g_auto_inc_lock.Unlock();

		g_last_timestamp = uiTimeStamp;
	}

	uiTimeStamp <<= 32;

	g_auto_inc_lock.Lock();
	uint32_t tmp = g_auto_inc << 8;
	g_auto_inc++;
	g_auto_inc_lock.Unlock();

	uiTimeStamp += tmp;
	return uiTimeStamp + uiServerID;
}
