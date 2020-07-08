#include "config.h"

Config::Config()
{

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

    m_tradeLog_path = root["tradeLog_path"].asString();

    m_DB_ip = root["DB_ip"].asString();
    m_DB_port = root["DB_port"].asUInt();
    m_DB_user = root["DB_user"].asString();
    m_DB_pwd = root["DB_pwd"].asString();
    m_DB_name = root["DB_name"].asString();

    m_new_DB_ip = root["new_DB_ip"].asString();
    m_new_DB_port = root["new_DB_port"].asUInt();
    m_new_DB_user = root["new_DB_user"].asString();
    m_new_DB_pwd = root["new_DB_pwd"].asString();
    m_new_DB_name = root["new_DB_name"].asString();

    m_totalPerTime = root["totalPerTime"].asUInt();
    m_threadCnt = root["threadCnt"].asUInt();
    m_UserTableCount = root["UserTableCount"].asUInt();

    m_dbBeginYear = root["dbBeginYear"].asInt();
    m_dbEndYear = root["dbEndYear"].asInt();
    m_dbEndMonth = root["dbEndMonth"].asInt();
    
    return true;
}

