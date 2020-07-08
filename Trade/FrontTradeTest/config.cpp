
#include "config.h"

Config::Config()
{
    m_sListenIP_BackTrade = "0.0.0.0";
    m_uiListenPort_BackTrade = 0;
    m_sListenIP_Client = "0.0.0.0";
    m_uiListenPort_Client = 0;
}


Config::~Config()
{

}

bool Config::Load(string &sPath)
{
    Json::Value root;
    fstream fs(sPath);
    if (!fs)
        return false;
    stringstream ss;
    ss << fs.rdbuf();
    string str(ss.str());
    bool ret = UBJsonUnpack(root, str);
    if(!ret)
        return false;
    this->m_sListenIP_BackTrade = root["ip_for_backtrade"].asString();
    this->m_uiListenPort_BackTrade = root["port_for_backtrade"].asUInt64();

    this->m_sListenIP_Client = root["ip_for_client"].asString();
    this->m_uiListenPort_Client = root["port_for_client"].asUInt64();
    this->m_connectKey_BackTrade = root["connect_key"].asString();
    return true;
}

