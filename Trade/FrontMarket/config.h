
#ifndef _CONFIG_HEADER_
#define _CONFIG_HEADER_

#include "UBaseLib.h"


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


    string m_sKLineDataDir;         // k线数据的根目录

    // 性能参数配置
    uint32_t m_uiWriteCahce2k;
    uint32_t m_uiWriteCahce8k;
    uint32_t m_uiWriteCahce16k;

    uint32_t m_uiReadCahce2k;
    uint32_t m_uiReadCahce8k;
    uint32_t m_uiReadCahce16k;
};

#endif
