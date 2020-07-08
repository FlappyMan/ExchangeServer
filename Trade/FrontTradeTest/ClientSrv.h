
#ifndef _CLIENT_SRV_HEADER_
#define _CLIENT_SRV_HEADER_

#include "ClientSession.h"

class ClientSrv
{
    public:
        ClientSrv();
        ~ClientSrv();

        void NewConnection(uv_tcp_t* tcp);
        void CloseConnection(uv_tcp_t *tcp);
        int Read(uv_tcp_t *tcp,char *pBuffer,int iDataLen); 
        void PushResponse(UBBlockQueue<UProtocolBase> &res);
        void GetRequest(UBBlockQueue<UProtocolBase> &req);
        void OnTimer(time_t tNow);
    public:
        void _DispatchPkg(UProtocolBase* pkg);
        void InsertClientID(string &token, ClientSession*);
        void AddRequest(UProtocolBase* pkg);
    public:
        map<uv_tcp_t*,ClientSession*> m_mSession;
        map<string,ClientSession*> m_mClientID;
        UBBlockQueue<UProtocolBase> m_qRequest,m_qResponse;
};


#endif
