
#include "config.h"

Config::Config() : m_uiListenPort_BackTrade(6000),m_uiListenPort_Client(443),
    m_uiWriteCahce2k(1000),m_uiWriteCahce8k(1000),m_uiWriteCahce16k(1000),
    m_uiReadCahce2k(1000),m_uiReadCahce8k(1000),m_uiReadCahce16k(1000)
{

}


Config::~Config()
{

}

bool Config::Load(string &sPath)
{
    struct stat stFile;
    if(stat(sPath.c_str(),&stFile)!=0)
    {
        return false;
    }

    ifstream ifile(sPath.c_str());
    if(ifile.is_open()==false)return false;

    char *pBuffer=new char[stFile.st_size];
    ifile.read(pBuffer,stFile.st_size);

    Json::Value root;
    if(UBJsonUnpack(root,pBuffer,stFile.st_size)==false)
    {
        delete []pBuffer;
        return false;
    }
    delete []pBuffer;
    if(!root["BackTrade"].isNull())
    {
        Json::Value &bt=root["BackTrade"];
        if(!bt["ip"].isNull())
        {
            m_sListenIP_BackTrade=bt["ip"].asString();
        }
        if(!bt["port"].isNull())
        {
            m_uiListenPort_BackTrade=bt["port"].asUInt();
        }
    }

    if(!root["Client"].isNull())
    {
        Json::Value &client=root["Client"];
        if(!client["ip"].isNull())
        {
            m_sListenIP_Client=client["ip"].asString();
        }
        if(!client["port"].isNull())
        {
            m_uiListenPort_Client=client["port"].asUInt();
        }
    }

    if(!root["WriteCache"].isNull())
    {
        Json::Value &wc=root["WriteCache"];
        if(!wc["2k"].isNull())
        {
            m_uiWriteCahce2k=wc["2k"].asUInt();
        }
        if(!wc["8k"].isNull())
        {
            m_uiWriteCahce8k=wc["8k"].asUInt();
        }
        if(!wc["16k"].isNull())
        {
            m_uiWriteCahce16k=wc["16k"].asUInt();
        }
    }

    if(!root["ReadCache"].isNull())
    {
        Json::Value &rc=root["ReadCache"];
        if(!rc["2k"].isNull())
        {
            m_uiReadCahce2k=rc["2k"].asUInt();
        }
        if(!rc["8k"].isNull())
        {
            m_uiReadCahce8k=rc["8k"].asUInt();
        }
        if(!rc["16k"].isNull())
        {
            m_uiReadCahce16k=rc["16k"].asUInt();
        }
    }
    if(!root["KLineDataDir"].isNull())
    {
        m_sKLineDataDir=root["KLineDataDir"].asString();
    }
    return true;
}

