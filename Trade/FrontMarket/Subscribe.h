
#ifndef _SUBSCRIBE_HEADER_
#define _SUBSCRIBE_HEADER_


#include "UProtocol.h"
#include "global.h"


class ClientSession;
class Subscribe
{
public:

public:
	string m_uiMarketID;            // market id

	string m_currency;		// 当前货币种类 (markets)
	string m_depth;			// 深度
	string m_tradelog;		// tradelog
	string m_kline;		// kline 的时间间隔
public:
};

class Market
{
public:
	Market(){};
	Market(uint64_t id,string &sid):m_sid(sid),m_id(id){};
	~Market(){};
	void Init(uint64_t id,string &sid){m_id=id;m_sid=sid;};

	void DepthAdd(ClientSession *p);
	void DepthRemove(ClientSession *p);

	void TraceLogAdd(ClientSession *p);
	void TraceLogRemove(ClientSession *p);

	void KlineAdd(ClientSession *p,KLINE kl);
	void KlineRemove(ClientSession *p,KLINE kl);
public:
	string m_sid;
	uint64_t m_id;

	set<ClientSession*> m_depth;
	set<ClientSession*> m_tracelog;
	set<ClientSession*> m_order;

	set<ClientSession*> m_60s;
	set<ClientSession*> m_5m;
	set<ClientSession*> m_15m;
	set<ClientSession*> m_30m;
	set<ClientSession*> m_1h;
	set<ClientSession*> m_6h;
	set<ClientSession*> m_1d;
	set<ClientSession*> m_1w;
	set<ClientSession*> m_1m;
};


class MarketMgr
{
public:
    MarketMgr();
    ~MarketMgr();

	void Destroy();
	void Init();

	Market* Get(string &id);
	Market* Get(uint64_t uiMarketID);

	void AddMarket(UPMarketAdd *pMarket);
public:
    map<uint64_t,Market*> m_mMarket;
	map<string,uint64_t> m_mStr2ID;
};
extern MarketMgr g_market_mgr;



#endif


