
#ifndef _CLIENT_SRV_HEADER_
#define _CLIENT_SRV_HEADER_

#include "ClientSession.h"
#include "Subscribe.h"

class ClientSrv
{
public:
	ClientSrv(int iBufferSize=1024*8);
	~ClientSrv();

	void NewConnection(uv_tcp_t* tcp);
	void CloseConnection(uv_tcp_t *tcp);

	int Read(uv_tcp_t *tcp,char *pBuffer,int iDataLen);

	void Subscribe(ClientSession* p,SUBSCRIBE_TYPE st,string &s);
	void Unsubscribe(ClientSession* p,SUBSCRIBE_TYPE st,string &s);

	void PushRequest(queue<UProtocolBase*> &q);
	void OnTimer(time_t tNow);
public:
	void _DispatchPkg(UProtocolBase *pkg);

	void _AddMarket(UPMarketAdd *pkg);

	void _DispatchPkg_AllMarketInfo(UPAllmarketinfo *pkg);

	void _DispatchPkg_KLine(UPKlinedata *pkg);
	void _DispatchPkg_DepthData(UPDepthdata *pkg);
	void _DispatchPkg_MatchedData(UPMatchedData *pkg);
	void _DispatchPkg_TransactionData(UPHistoricalTransactionData *pkg);

	void _DispatchPkg(UProtocolBase *pkg,set<ClientSession*> &client);

protected:
	map<uv_tcp_t*,ClientSession*> m_mSession;
	
	queue<UProtocolBase*> m_qReqest;
	uv_mutex_t m_lock;

	char *m_pBuffer;
	int m_iBufferSize;

	map<uint64_t,Market*> m_mMarket;
};


#endif
