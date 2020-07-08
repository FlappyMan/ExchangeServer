
#include "Subscribe.h"

void Market::DepthAdd(ClientSession *p)
{
	m_depth.insert(p);
};
void Market::DepthRemove(ClientSession *p)
{
	m_depth.erase(p);
};
void Market::TraceLogAdd(ClientSession *p)
{
	m_tracelog.insert(p);
};
void Market::TraceLogRemove(ClientSession *p)
{
	m_tracelog.erase(p);
};


void Market::KlineAdd(ClientSession *p,KLINE kl)
{
	if(kl==KLINE_60S)m_60s.insert(p);
	else if(kl==KLINE_5M)m_5m.insert(p);
	else if(kl==KLINE_15M)m_15m.insert(p);
	else if(kl==KLINE_30M)m_30m.insert(p);
	else if(kl==KLINE_1H)m_1h.insert(p);
	else if(kl==KLINE_6H)m_6h.insert(p);
	else if(kl==KLINE_1D)m_1d.insert(p);
	else if(kl==KLINE_1W)m_1w.insert(p);
	else if(kl==KLINE_1M)m_1m.insert(p);
};

void Market::KlineRemove(ClientSession *p,KLINE kl)
{
	if(kl==KLINE_60S)m_60s.erase(p);
	else if(kl==KLINE_5M)m_5m.erase(p);
	else if(kl==KLINE_15M)m_15m.erase(p);
	else if(kl==KLINE_30M)m_30m.erase(p);
	else if(kl==KLINE_1H)m_1h.erase(p);
	else if(kl==KLINE_6H)m_6h.erase(p);
	else if(kl==KLINE_1D)m_1d.erase(p);
	else if(kl==KLINE_1W)m_1w.erase(p);
	else if(kl==KLINE_1M)m_1m.erase(p);
};



MarketMgr g_market_mgr;
MarketMgr::MarketMgr()
{

}


MarketMgr::~MarketMgr()
{
	Destroy();
}


void MarketMgr::Destroy()
{
	for(map<uint64_t,Market*>::iterator  it=m_mMarket.begin();it!=m_mMarket.end();it++)
	{
		delete it->second;
	}
	m_mMarket.clear();
	m_mStr2ID.clear();
}

void MarketMgr::Init()
{
}



Market* MarketMgr::Get(string &sMarketID)
{
	map<string,uint64_t>::iterator it=m_mStr2ID.find(sMarketID);
	if(it==m_mStr2ID.end())return NULL;

	return Get(it->second);
}

Market* MarketMgr::Get(uint64_t uiMarketID)
{
	map<uint64_t,Market*>::iterator it2=m_mMarket.find(uiMarketID);
	if(it2!=m_mMarket.end())return NULL;
	else return it2->second;
}


void MarketMgr::AddMarket(UPMarketAdd *pMarket)
{
	map<uint64_t,Market*>::iterator it=m_mMarket.find(pMarket->m_uiMarketID);
	if(it!=m_mMarket.end())return;

	Market *p=new Market(pMarket->m_uiMarketID,pMarket->m_sMarketID);
	m_mMarket.insert(make_pair(p->m_id,p));
	m_mStr2ID.insert(make_pair(p->m_sid,p->m_id));
}





