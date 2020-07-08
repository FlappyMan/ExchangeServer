
#ifndef _BACKTRADE_SRV_HEADER_
#define _BACKTRADE_SRV_HEADER_

#include "BackTradeSession.h"

class BackTradeSrv
{
public:
    BackTradeSrv();
    ~BackTradeSrv();

    void NewConnection(uv_tcp_t *tcp);
    int Read(uv_tcp_t *tcp,char *pBuffer,int iDataLen);

    void AddRequest(UProtocolBase *p);
    void GetRequest(queue<UProtocolBase*> &q);
public:
    string m_sPublicKey;                // backtrade 的 UPLogin 包中用到的 rsa 公钥

protected:
    map<uv_tcp_t*,BackTradeSession*> m_mSession;
    
    queue<UProtocolBase*> m_qReqest;
    uv_mutex_t m_lock;
};


#endif
