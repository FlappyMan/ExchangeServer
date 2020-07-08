#ifndef _BASE_REQUEST_HEADER_
#define _BASE_REQUEST_HEADER_
#include "../global.h"
#include "../Manager/DispatchManager.h"

class CBaseRequestOper
{
public:
    virtual string getRequestToken() = 0;
    virtual int requestOperation(){};
    uint16_t m_usCMD;
    uv_tcp_t *m_client;

};

#endif
