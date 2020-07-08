/*
 * @Author: your name
 * @Date: 2020-06-09 19:59:45
 * @LastEditTime: 2020-06-22 00:27:16
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/BaseConfig/RuntimeConfig.h
 */ 

#ifndef _RUNTIME_CONFIm_HEADER_
#define _RUNTIME_CONFIm_HEADER_

#include "UBaseLib.h"
#include <fstream>
class CCoinMarket
{
public:
    uint32_t m_uiMarketID;
    string m_sMarketName;
    
    uint64_t m_ulSysUid;
    string m_sBuyMin;
    string m_sBuyMax;
    string m_sSellMin;
    string m_sSellMax;
    string m_sTradeMin;
    string m_sTradeMax;
    
};

class CRuntimeConfig
{
public:
    static bool Load(string &sPath,string& updateTime);
    
public:
    static map<uint32_t,CCoinMarket> market;
    static vector<float> viplevel;
};

#endif
