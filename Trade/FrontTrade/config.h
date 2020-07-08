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
    string m_sListenIP_BackTrade;
    uint16_t m_uiListenPort_BackTrade;

    string m_sListenIP_Client;
    uint16_t m_uiListenPort_Client;

    uint8_t m_serverID;

    string m_connectKey_BackTrade;
};

#endif
