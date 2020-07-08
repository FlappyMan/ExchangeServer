#ifndef _CONFIG_HEADER_
#define _CONFIG_HEADER_

#include "UBaseLib.h"
#include <fstream>


class Config
{
public:
    Config();
    ~Config();
    bool Load(string &sPath);

public:
    string m_tradeLog_path;

    string m_DB_ip;
    uint16_t m_DB_port;
    string m_DB_user;
    string m_DB_pwd;
    string m_DB_name;

    string m_new_DB_ip;
    uint16_t m_new_DB_port;
    string m_new_DB_user;
    string m_new_DB_pwd;
    string m_new_DB_name;

    uint32_t m_totalPerTime;
    uint32_t m_threadCnt;
    uint32_t m_UserTableCount;

    int m_dbBeginYear;
    int m_dbEndYear;
    int m_dbEndMonth;

};

#endif
