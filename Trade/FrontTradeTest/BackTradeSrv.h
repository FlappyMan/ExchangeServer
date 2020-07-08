
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
        void OnTimer(time_t tNow);
        void GetResponse(UBBlockQueue<UProtocolBase> &res);
        void PushRequest(UBBlockQueue<UProtocolBase> &p);
        void AddMarket(uint64_t& MarketID, BackTradeSession* bt);
        void _DispatchReq(UProtocolBase* req);
        void _DispatchReq_Uptrade(UPUptrade* pkg);
        void _DispatchReq_CancelTrade(UPCanceltrade* pkg);
    public:
        map<uv_tcp_t*,BackTradeSession*> m_mSession;
        map<uint64_t,BackTradeSession*> m_mMarketID;
        map<uint64_t,string> m_mMarket;
        UBBlockQueue<UProtocolBase> m_qResponse,m_qRequest;
    public:
        string m_sPublicKey; 
};

#endif
