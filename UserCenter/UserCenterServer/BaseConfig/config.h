/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-11 00:46:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/BaseConfig/config.h
 */ 

#ifndef _CONFIG_HEADER_
#define _CONFIG_HEADER_

#include "UBaseLib.h"
#include <fstream>


class Config
{
public:
    Config();
    ~Config();
    static bool Load(string &sPath);
    static int getRSAPrivateKey();
    
public:
    static string g_sTcpIP;
    static uint16_t g_usTcpPort;

    static string g_sHttpIP;
    static uint16_t g_uiHttpPort;

    static string sHost;
    static uint16_t uiPort;
    static string sUser;
    static string sPwd;
    static string sAccountDB;
    static string sAdminDB;
    static string sAssetsDB;
    static string sBaseDB;
    static string sExassetsDB;
    static string sKycDB;
    static string sOtcDB;
    static string sOtherDB;
    static string sSaDB;
    static string sTradeDB;
    static string sRSAPrivateKey;
    
};

#endif
