#ifndef _BACKTRADE_SRV_HEADER_
#define _BACKTRADE_SRV_HEADER_

#include "BackTradeSession.h"

extern uint32_t g_uiOrderID;
extern uv_mutex_t g_mutex;

class BackTradeSrv
{
    public:
        BackTradeSrv();
        ~BackTradeSrv();

        void NewConnection(uv_tcp_t *tcp);
        int Read(uv_tcp_t *tcp,char *pBuffer,int iDataLen);
        void OnTimer();
        void GetResponse(UBBlockQueue<UProtocolBase> &res);
        void PushRequest(UBBlockQueue<UProtocolBase> &req);
        void AddMarket(uint64_t& MarketID, BackTradeSession* bt);
        void _DispatchReq(UProtocolBase* req);
        void _DispatchReq_Uptrade(UPUptrade* pkg);
        void _DispatchReq_UptradeBatch(UPUptradebatch* pkg);
        void _DispatchReq_CancelTrade(UPCanceltrade* pkg);
        void _DispatchReq_CancelTradeBatch(UPCanceltradebatch* pkg);
        uint64_t CreateOrderID(uint64_t serverID);
    public:
        map<uv_tcp_t*,BackTradeSession*> m_mSession;
        map<uint64_t, BackTradeSession*> m_mMarketID;
        UBBlockQueue<UProtocolBase> m_qResponse,m_qRequest;
    public:
        string m_sPublicKey;
};

#endif
