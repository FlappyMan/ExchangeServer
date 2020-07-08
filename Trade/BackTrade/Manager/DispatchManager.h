/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-07-01 23:26:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/Manager/DispatchManager.h
 */ 


#ifndef _DISPATCH_MANAGER_HEADER_
#define _DISPATCH_MANAGER_HEADER_
#include "../global.h"
#include "UBaseSession.h"

class CDispatchManager
{
public:
    CDispatchManager();
    ~CDispatchManager();
public:
    static int DispatchClient(mapBaseSession &mapSession, client_pkg_t *client, char *pBuffer, int iDataLen, bool bTokenSession);
    static void recordData(CBaseModel* pModel);
    static void recordData(vector<CBaseModel*>& vecRet);
public:
    static void connectToDB(UBMysql &dbConn, string &dbName);
    static void DBInit();
public:

    static mapBaseSession g_mapTCPSession;  // orderThread线程统一接受socket对象
    static mapBaseSession g_mapUserCenterSession; // UCThread线程indx-业务处理对象
    static mapBaseSession g_mapFrontMarketSession; // UCThread线程indx-业务处理对象

    static UBMysql g_DBAccount;
    static UBMysql g_DBAdmin;
    static UBMysql g_DBAssets;
    static UBMysql g_DBAssetsLog;
    static UBMysql g_DBBase;
    static UBMysql g_DBExassets;
    static UBMysql g_DBKyc;
    static UBMysql g_DBOTC;
    static UBMysql g_DBOther;
    static UBMysql g_DBSA;
    static UBMysql g_DBTrade;
    
    static uint64_t g_ulOrderIndex;
    static uint64_t g_ulUCIndex;
    static uint64_t g_ulControlIndex;
    
};

#endif
