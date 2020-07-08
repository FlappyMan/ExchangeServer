#ifndef _H_GLOBAL_HEADER_
#define _H_GLOBAL_HEADER_


#include "UProtocol.h"
#include "config.h"
#include <sys/time.h>

extern string g_dir_root;                       // 程序当前路径
extern Config g_config;
extern UBMysql g_mysql;                         // 通用源数据库
extern UBMysql g_mysql_dst;                     // 通用目标数据库
extern set<uint32_t> g_sYear;                   // 所有年份
extern map<string,uint32_t> g_mMarketMap;       // 市场id，name映射表
extern std::vector<string> g_vec_tableName_src;

extern atomic_int g_cnt;                        //计数器
extern uv_timeval_t g_timeBegin;
extern uv_timeval_t g_timeEnd;

void GetAllSrcTableName();
void GetMarketMap();
struct tm GetYearFromTimestamp(uint64_t uiTimestamp);
int64_t GetCurTime();
int64_t BuildOrderID(int64_t uiTimeStamp, uint64_t uiServerID);

typedef struct data
{
	int start;
	int end;
    string dbName;
    string tableName;
    UBMysql *ubMysql;
}Data;

typedef struct tableData
{
    uint64_t uiID;
    uint64_t uiUserID;
    uint64_t uiPeerid;
    string sMarket;
    string sPrice;
    string sNum;
    string sMum;
    string sFee_buy;
    string sFee_sell;
    uint32_t uiType;
    uint64_t uiSort;
    int64_t iAddtime;
    int64_t iEndtime;
    int32_t iStatus;
    uint64_t uiBuy_id;
    uint64_t uiSell_id;

    uint32_t uiMarketID;
}TableData;


#endif
