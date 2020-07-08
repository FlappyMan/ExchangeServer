
#include "BackTradeSrv.h"
#include "ThreadClient.h"

BackTradeSrv::BackTradeSrv()
{

}

BackTradeSrv::~BackTradeSrv()
{

}

void BackTradeSrv::NewConnection(uv_tcp_t *tcp)
{
    BackTradeSession *p=new BackTradeSession(tcp,m_qResponse);
    p->Init();
    m_mSession.insert(pair<uv_tcp_t*,BackTradeSession*>(tcp,p));
}

// return <0: 协议错误，=0:数据包长度不足，>0:已处理掉的数据长度
int BackTradeSrv::Read(uv_tcp_t* tcp,char *pBuffer,int iDataLen)
{
    map<uv_tcp_t*,BackTradeSession*>::iterator it=m_mSession.find(tcp);
    if(it==m_mSession.end())return false;

    return it->second->Read(pBuffer,iDataLen,m_qResponse);
}

void BackTradeSrv::OnTimer(time_t tNow)
{
    map<uv_tcp_t*,BackTradeSession*>::iterator it;
    UProtocolBase* pkg = NULL; 
    for(it=m_mSession.begin();it!=m_mSession.end();it++)
    {
        if (it->second->IsTimeout(tNow))
        {
            delete it->second;
            m_mSession.erase(it);
            continue;
        }
    }
    while (m_qRequest.size()>0)
    {
        _DispatchReq(m_qRequest.get());
    }   
}

void BackTradeSrv::GetResponse(UBBlockQueue<UProtocolBase> &res)
{
    while (m_qResponse.size() > 0)
    {
        res.put(m_qResponse.get());
    }  
}

void BackTradeSrv::PushRequest(UBBlockQueue<UProtocolBase> &p)
{
    while (m_qRequest.size() > 0)
    {
        m_qRequest.put(p.get()); 
    }  
}

void BackTradeSrv::AddMarket(uint64_t& MarketID, BackTradeSession* bt)
{
    m_mMarketID.insert(make_pair(MarketID,bt));
}

void BackTradeSrv::_DispatchReq(UProtocolBase* pkg)
{ 
    switch(pkg->m_uiType)
    {
        case UPUptrade::CMD : 
            _DispatchReq_Uptrade((UPUptrade*)(pkg));      
            break;
        case UPUptradebatch::CMD :
            /*code*/
            break;
        case UPCanceltrade::CMD :
            _DispatchReq_CancelTrade((UPCanceltrade *)(pkg));
            break;
        case UPCanceltradebatch::CMD :
            /*code*/
            break;
        default:
            break;
    }
}

void BackTradeSrv::_DispatchReq_Uptrade(UPUptrade* pkg)
{
    map<uint64_t, BackTradeSession*>::iterator it = m_mMarketID.begin();
    for (;it!=m_mMarketID.end();it++)
    {
        if (pkg->marketid() == it->first)
        {
            BackTrade_Write<UPUptrade>((uv_stream_t*)(it->second->m_tcp),pkg,(uint32_t)SIZE_BUFFER_2k);
        }               
    }   
}

void BackTradeSrv::_DispatchReq_CancelTrade(UPCanceltrade* pkg)
{
    map<uint64_t, BackTradeSession*>::iterator it = m_mMarketID.begin();
    for (;it!=m_mMarketID.end();it++)
    {
        if (pkg->marketid() == it->first)
        {
            BackTrade_Write<UPCanceltrade>((uv_stream_t*)(it->second->m_tcp),pkg,(uint32_t)SIZE_BUFFER_2k);
        }               
    }
}

