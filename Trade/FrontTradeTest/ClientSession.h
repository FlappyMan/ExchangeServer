

#ifndef _CLIENT_SESSION_HEADER_
#define _CLIENT_SESSION_HEADER_

#include "global.h"

class ClientSession
{
    public:
        ClientSession(uv_tcp_t* tcp);
        ~ClientSession();

        void Init();
        void Destroy();
        bool IsTimeout(time_t tNow);

        int Read(uv_tcp_t* client, char *pBuffer,int iDataLen);

    public:
        uv_tcp_t * m_tcp;
        UBBuffer m_buffer;
        UBHttpParser m_http;
        time_t m_tLast;
        uint64_t m_uSessionID;
};

#endif
