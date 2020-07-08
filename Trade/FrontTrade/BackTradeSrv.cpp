#include "BackTradeSrv.h"
#include "ThreadClient.h"

uint32_t g_uiOrderID = 1;
uv_mutex_t g_mutex;

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

void BackTradeSrv::GetResponse(UBBlockQueue<UProtocolBase> &res)
{
	while (m_qResponse.size() > 0)
	{
		res.put(m_qResponse.get());
	}  
}

void BackTradeSrv::PushRequest(UBBlockQueue<UProtocolBase> &req)
{
	while (req.size() > 0)
	{
		m_qRequest.put(req.get()); 
	}  
}

void BackTradeSrv::OnTimer()
{
	while (m_qRequest.size()>0)
	{
		uv_mutex_lock(&g_mutex);
		++g_uiOrderID;
    	uv_mutex_unlock(&g_mutex);
		_DispatchReq(m_qRequest.get());  
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
			_DispatchReq_UptradeBatch((UPUptradebatch*)pkg);
			break;
		case UPCanceltrade::CMD :
			pkg = new UPCanceltrade;
			_DispatchReq_CancelTrade((UPCanceltrade *)(pkg));
			break;
		case UPCanceltradebatch::CMD :
			_DispatchReq_CancelTradeBatch((UPCanceltradebatch*)pkg);
			break;
		default:
			break;
	}
}

uint64_t BackTradeSrv::CreateOrderID(uint64_t serverID)
{
	time_t tNow = time(NULL);
	uint64_t orderID = 0;
	orderID +=  tNow;
	orderID <<= 32;
	uint32_t incOrderID = g_uiOrderID;
	incOrderID <<=  8;
	orderID +=  incOrderID;
	orderID +=  serverID;
	return orderID;
}

void BackTradeSrv::_DispatchReq_Uptrade(UPUptrade* pkg)
{
	uint64_t orderID = 0;
	map<uv_tcp_t*,BackTradeSession*>::iterator it = m_mSession.begin();
	for (; it!=m_mSession.end(); it++)
	{
		if (pkg->marketid() == it->second->m_uiMarketID)
		{
			orderID = CreateOrderID(g_config.m_serverID);
			pkg->set_orderid(orderID);
			pkg->set_uptradeid(1);
			BackTrade_Write<UPUptrade>((uv_stream_t*)(it->second->m_tcp),pkg,(uint32_t)SIZE_BUFFER_2k);
		}      
	}
}

void BackTradeSrv::_DispatchReq_CancelTrade(UPCanceltrade* pkg)
{
	uint64_t orderID = 0;
	map<uv_tcp_t*,BackTradeSession*>::iterator it = m_mSession.begin();
	for (; it!=m_mSession.end(); it++)
	{
		if (pkg->marketid() == it->second->m_uiMarketID)
		{
			orderID = CreateOrderID(g_config.m_serverID);
			pkg->set_orderid(orderID);
			pkg->set_canceltradeid(1);
			BackTrade_Write<UPCanceltrade>((uv_stream_t*)(it->second->m_tcp),pkg,(uint32_t)SIZE_BUFFER_2k);
		}     
	}
}

void BackTradeSrv::_DispatchReq_UptradeBatch(UPUptradebatch* pkg)
{
	uint64_t orderID = 0;
	map<uv_tcp_t*,BackTradeSession*>::iterator it = m_mSession.begin();
	for (; it!=m_mSession.end(); it++)
	{
		for (size_t i = 0; i < pkg->trades_size(); i++)
		{
			UPUptradebatch::Trades item = pkg->trades(i);
			if (item.marketid() == it->second->m_uiMarketID)
			{
				orderID =CreateOrderID(g_config.m_serverID);     
				item.set_orderid(orderID);         
				BackTrade_Write<UPUptradebatch>((uv_stream_t*)(it->second->m_tcp),pkg,(uint32_t)SIZE_BUFFER_2k);
			}
			pkg->set_uptradebatchid(1);          
		}        
	}
}

void BackTradeSrv::_DispatchReq_CancelTradeBatch(UPCanceltradebatch* pkg)
{
	uint64_t orderID = 0;
	map<uv_tcp_t*,BackTradeSession*>::iterator it = m_mSession.begin();
	for (; it!=m_mSession.end(); it++)
	{
		for (size_t i = 0; i < pkg->orders_size(); i++)
		{
			UPCanceltradebatch::Orders item = pkg->orders(i);
			if (item.marketid() == it->second->m_uiMarketID)
			{
				orderID = CreateOrderID(g_config.m_serverID);
				item.set_orderid(orderID);
				BackTrade_Write<UPCanceltradebatch>((uv_stream_t*)(it->second->m_tcp),pkg,(uint32_t)SIZE_BUFFER_2k);
			}
			pkg->set_canceltradebatchid(1);         
		}
	}
}
