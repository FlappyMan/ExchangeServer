/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-16 03:03:16
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Manager/DispatchManager.h
 */
#ifndef _DISPATCH_MANAGER_HEADER_
#define _DISPATCH_MANAGER_HEADER_
#include "../global.h"
#include "UBaseSession.h"
#include "HttpSession.h"
#include "../ThreadTCPOper.h"
#include "../ThreadHttpOper.h"
typedef map<client_sokt_t *, CBaseSession *> mapBaseSession;
typedef map<string, CBaseRequestOper *> mapBaseTcpSession; // token-业务处理对象 适用于tcp
typedef map<client_sokt_t *, CHttpSession *> mapHttpSession;

class CDispatchManager
{
public:
    CDispatchManager();
    ~CDispatchManager();

public:
    
    static int DispatchClient(client_sokt_t *client, char *pBuffer, int iDataLen);
    static void CloseClient(uv_tcp_t *client);
    static int DispatchOnSend(UProtocolBase *prot, uv_tcp_t *client);

    static int DispatchHttpClient(client_sokt_t *client, char *pBuffer, int iDataLen);
    static void CloseHttpClient(uv_tcp_t *client);
    static int DispatchHttpResponse(string sResp, uv_tcp_t *client, bool success = true);

public:
    static void DBInit();
    static void connectToDB(UBMysql &dbConn, string &dbName);

public:
    // 功用方法部分
    static string getYYMMDD();
    static string getHour();
    static string getMin();
    static int getNowTime();
    static string getRandomToken();
    static int HexStringToInt(string value);
    static string IntHexToString(int value, int len);
    static string md5(string strPlain);
    static string getToken(string uid);
    static int GetGMTime(char *szGmtTime);
    static bool IsBigThanZero(string value);
    static string getUserVipDiscount(string level);

public:
    // 高精度字符串加减乘除部分
    static string addLongDouble(string _s1, string _s2);
    static string minusLongDouble(string _s1, string _s2);
    static string mutipLongDouble(string _s1, string _s2);
    static string diviLongDouble(string _s1, string _s2);
    static void addBeforeNum(string &num, int idx);
    static void subBeforeNum(string &num, int idx);
    static string sumIntValue(string &s1, string &s2, int iLen, int iLevel);
    static string subIntValue(string &s1, string &s2, int iLen, int iLevel);
    static void subString(string &_s1, string &_s2);
    static void mutipBeforeNum(string &num, int iData);
    
public:
    static mapBaseSession g_mapMapping;
    static mapHttpSession g_mapHttpMapping;
    static mapBaseTcpSession g_mapTcpBusinessSession; // orderThread线程，每个token业务请求处理对象

    static UBMysql g_DBAccount;
    static UBMysql g_DBAdmin;
    static UBMysql g_DBAssets;
    static UBMysql g_DBBase;
    static UBMysql g_DBExassets;
    static UBMysql g_DBKyc;
    static UBMysql g_DBOTC;
    static UBMysql g_DBOther;
    static UBMysql g_DBSA;
    static UBMysql g_DBTrade;

    static uint64_t g_ulTCPPkgIndex;
    static uint64_t g_ulTCPSockIndex;
    static uint64_t g_ulHTTPPkgIndex;
};

#endif
