
#include "ClientSrv.h"

ClientSrv::ClientSrv(int iBufferSize)
{
	uv_mutex_init(&m_lock);

	m_iBufferSize=iBufferSize;
	m_pBuffer=new char[m_iBufferSize];
}

ClientSrv::~ClientSrv()
{
	for(map<uv_tcp_t*,ClientSession*>::iterator it=m_mSession.begin();it!=m_mSession.end();it++)
	{
		delete it->second;
	}
	m_mSession.clear();
}

void ClientSrv::NewConnection(uv_tcp_t* tcp)
{
	ClientSession *p=new ClientSession(tcp,m_pBuffer,m_iBufferSize);
	p->Init();
	m_mSession.insert(make_pair(tcp,p));
}

void ClientSrv::CloseConnection(uv_tcp_t *tcp)
{
	map<uv_tcp_t*,ClientSession*>::iterator it=m_mSession.find(tcp);
	if(it!=m_mSession.end())
	{
		delete it->second;
		m_mSession.erase(it);
	}
}

// return <0: 协议错误，=0:数据包长度不足，>0:已处理掉的数据长度
int ClientSrv::Read(uv_tcp_t* tcp,char *pBuffer,int iDataLen)
{
	map<uv_tcp_t*,ClientSession*>::iterator it=m_mSession.find(tcp);
	if(it==m_mSession.end())
	{
		return -10;
	}

	return it->second->Read(pBuffer,iDataLen);
}

void ClientSrv::Subscribe(ClientSession* p,SUBSCRIBE_TYPE st, string &sub)
{
	Market *pMarket=g_market_mgr.Get(p->m_subs.m_uiMarketID);
	if(pMarket==NULL)return;

	if(st==SUBSCRIBE_TYPE_DEPTH)
	{
		pMarket->DepthAdd(p);
	}
	else if(st==SUBSCRIBE_TYPE_TRADELOG)
	{
		pMarket->TraceLogAdd(p);
	}
	else if(st==SUBSCRIBE_TYPE_KLINE)
	{
		pMarket->KlineAdd(p,(KLINE)KLineFromString(sub));
	}

}

void ClientSrv::Unsubscribe(ClientSession* p,SUBSCRIBE_TYPE st,string &s)
{
	Market *pMarket=g_market_mgr.Get(p->m_subs.m_uiMarketID);
	if(pMarket==NULL)return;

	if(st==SUBSCRIBE_TYPE_DEPTH)
	{
		pMarket->DepthRemove(p);
	}
	else if(st==SUBSCRIBE_TYPE_TRADELOG)
	{
		pMarket->TraceLogRemove(p);
	}
	else if(st==SUBSCRIBE_TYPE_KLINE)
	{
		pMarket->KlineRemove(p,(KLINE)KLineFromString(s));
	}
}

void ClientSrv::PushRequest(queue<UProtocolBase*> &q)
{
	uv_mutex_lock(&m_lock);
	while(q.size()>0)
	{
		m_qReqest.push(q.front());
		q.pop();
	}
	uv_mutex_unlock(&m_lock);
}

void ClientSrv::OnTimer(time_t tNow)
{
	queue<UProtocolBase*> qReq;
	uv_mutex_lock(&m_lock);
	qReq.swap(m_qReqest);
	uv_mutex_unlock(&m_lock);

	map<uv_tcp_t*,ClientSession*>::iterator it;
	for(it=m_mSession.begin();it!=m_mSession.end();)
	{
		if(it->second->IsTimeout(tNow))
		{
		   it->second->Destroy();
		   
		   delete it->second;
		   it=m_mSession.erase(it);
		   continue;
		}
		it++;
	}

	while(qReq.size()>0)
	{
		_DispatchPkg(qReq.front());
		qReq.pop();
	}
}


void ClientSrv::_DispatchPkg(UProtocolBase *pkg)
{
	UPAllmarketinfo *pAllMarketInfo=NULL;
	UPKlinedata *pKline=NULL;
	UPDepthdata *pDept=NULL;
	UPHistoricalTransactionData *pHistory=NULL;
	UPMatchedData *pMatchData=NULL;

	switch (pkg->m_uiType)
		{
		case UPDepthdata::CMD :
			_DispatchPkg_DepthData((UPDepthdata*)pkg);
			break;
		case UPKlinedata::CMD :
			_DispatchPkg_KLine((UPKlinedata*)pkg);
			break;
		case UPAllmarketinfo::CMD :
			_DispatchPkg_AllMarketInfo((UPAllmarketinfo*)pkg);
			break;
		case UPMatchedData::CMD :
			_DispatchPkg_MatchedData((UPMatchedData*)pkg);
			break;
		case UPHistoricalTransactionData::CMD :
			_DispatchPkg_TransactionData((UPHistoricalTransactionData*)pkg);
			break;
		case UPMarketAdd::CMD:
			_AddMarket((UPMarketAdd*)pkg);
		default:
			break;
		}
}

void ClientSrv::_AddMarket(UPMarketAdd *pkg)
{
	g_market_mgr.AddMarket(pkg);
}


void ClientSrv::_DispatchPkg_AllMarketInfo(UPAllmarketinfo *pkg)
{
	for(map<uv_tcp_t*,ClientSession*>::iterator it=m_mSession.begin();it!=m_mSession.end();it++)
	{
		it->second->SendPkg(pkg);
	}
}


void ClientSrv::_DispatchPkg_KLine(UPKlinedata *pkg)
{
	Market *pMarket=g_market_mgr.Get(pkg->m_sMarketID);
	if(pMarket==NULL)return;

	vector<string> vType;
	UBStringSplit(vType,pkg->type(),'.');
	if(vType.size()!=2)return;

	if(vType[1].compare("1m"))
	{
		_DispatchPkg(pkg,pMarket->m_60s);
	}
	else if(vType[1].compare("5m"))
	{
		_DispatchPkg(pkg,pMarket->m_5m);
	}
	else if(vType[1].compare("15m"))
	{
		_DispatchPkg(pkg,pMarket->m_15m);
	}
	else if(vType[1].compare("30m"))
	{
		_DispatchPkg(pkg,pMarket->m_30m);
	}
	else if(vType[1].compare("1h"))
	{
		_DispatchPkg(pkg,pMarket->m_1h);
	}
	else if(vType[1].compare("6h"))
	{
		_DispatchPkg(pkg,pMarket->m_6h);
	}	
	else if(vType[1].compare("1d"))
	{
		_DispatchPkg(pkg,pMarket->m_1d);
	}
	else if(vType[1].compare("1w"))
	{
		_DispatchPkg(pkg,pMarket->m_1w);
	}
	else if(vType[1].compare("1m"))
	{
		_DispatchPkg(pkg,pMarket->m_1m);
	}

}



void ClientSrv::_DispatchPkg_DepthData(UPDepthdata *pkg)
{
	Market *pMarket=g_market_mgr.Get(pkg->m_sMarketID);
	if(pMarket==NULL)return;

	_DispatchPkg(pkg,pMarket->m_depth);
}

void ClientSrv::_DispatchPkg_MatchedData(UPMatchedData *pkg)
{
	Market *pMarket=g_market_mgr.Get(pkg->m_sMarketID);
	if(pMarket==NULL)return;

	_DispatchPkg(pkg,pMarket->m_order);
}

void ClientSrv::_DispatchPkg_TransactionData(UPHistoricalTransactionData *pkg)
{
	Market *pMarket=g_market_mgr.Get(pkg->m_sMarketID);
	if(pMarket==NULL)return;

	_DispatchPkg(pkg,pMarket->m_tracelog);
}


void ClientSrv::_DispatchPkg(UProtocolBase *pkg,set<ClientSession*> &client)
{
	for(set<ClientSession*>::iterator it=client.begin();it!=client.end();it++)
	{
		(*it)->SendPkg(pkg);
	}
}
