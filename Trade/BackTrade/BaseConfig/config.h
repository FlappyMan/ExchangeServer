/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-11 00:46:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/BaseConfig/config.h
 */

#ifndef _CONFIG_HEADER_
#define _CONFIG_HEADER_

#include "UBaseLib.h"
#include <fstream>

class ConnectServer
{
public:
    string m_sConnectIP;
    uint16_t m_usConnectPort;
    string m_sConnectKey;
};
typedef vector<ConnectServer> vecConnectServer;

class Config
{
public:
    Config();
    ~Config();
    static bool Load(string &sPath);
    static void getRSAKey(string sFilePath, string &key,int iLen);
public:
    static vecConnectServer g_vecConnectServers;
    static vecConnectServer g_vecConnectMarkets;

    static string g_sHttpIP;
    static uint16_t g_uiHttpPort;

    static string g_sUserCenterIP;
    static uint16_t g_uiUserCenterPort;

    static string sMarket;
    static uint32_t uiMarketID;

    static string sHost;
    static uint16_t uiPort;
    static string sUser;
    static string sPwd;
    static string sAccountDB;
    static string sAdminDB;
    static string sAssetsDB;
    static string sAssetsLogDB;
    static string sBaseDB;
    static string sExassetsDB;
    static string sKycDB;
    static string sOtcDB;
    static string sOtherDB;
    static string sSaDB;
    static string sTradeDB;
    static string sCoinA;   // btc 商品
    static string sCoinB;   // usdt 货币
    static string currencyName;
    static string webIcon;
    static string appIcon;
    static string currency;
    static string recordDir;
    static string sRSAUserCenterPubKey;
    static string sRSAFrontTradePriKey;
};

#endif
