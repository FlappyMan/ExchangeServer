/*
 * @Author: your name
 * @Date: 2020-06-09 19:59:45
 * @LastEditTime: 2020-06-22 00:27:26
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/BaseConfig/RuntimeConfig.cpp
 */ 
#include "RuntimeConfig.h"

map<uint32_t,CCoinMarket> CRuntimeConfig::market;
vector<float> CRuntimeConfig::viplevel;
bool CRuntimeConfig::Load(string &sPath,string& updateTime)
{
    Json::Value root;
    fstream fs(sPath);
    if (!fs)
        return false;
    stringstream ss;
    ss << fs.rdbuf();
    string str(ss.str());
    bool ret = UBJsonUnpack(root, str);
    if (!ret)
    {
        return false;
    }
    else
    {
        struct stat buf;
        int result = stat((char *)sPath.c_str(), &buf);
        if (result != 0)
        {
            printf("Problem getting information");
            switch (errno)
            {
            case ENOENT:
                printf("File not found.");
                break;
            case EINVAL:
                printf("Invalid parameter to _stat.");
                break;
            default:
                /* Should never be reached. */
                printf("Unexpected error in _stat.");
            }
        }
        else
        {
            string timebuf = std::to_string(buf.st_ctime);
            if (updateTime != timebuf)
            {
                CRuntimeConfig::market.clear();
                CRuntimeConfig::viplevel.clear();
                updateTime = timebuf;
            }
            else
            {
                return true;
            }
        }
    }

    Json::Value &marketList = root["market"];
    for (int i = 0; i < marketList.size(); i++)
    {
        Json::Value& market = marketList[i];
        CCoinMarket obj;
        obj.m_uiMarketID = market["id"].asUInt();
        obj.m_sMarketName = market["name"].asString();
        
        obj.m_ulSysUid = market["fee_user_id"].asUInt64();
        obj.m_sBuyMin = market["buy_min"].asString();
        obj.m_sBuyMax = market["buy_max"].asString();
        obj.m_sSellMin = market["sell_min"].asString();
        obj.m_sSellMax = market["sell_max"].asString();
        obj.m_sTradeMin = market["trade_min"].asString();
        obj.m_sTradeMax = market["trade_max"].asString();
        
        CRuntimeConfig::market.insert(pair<uint32_t, CCoinMarket>(obj.m_uiMarketID, obj));
    }
    

    Json::Value &level = root["viplevel"];
    for(int i = 0; i < level.size(); i++)
    {
        CRuntimeConfig::viplevel.push_back(level[i].asFloat());
    }

    return true;
}