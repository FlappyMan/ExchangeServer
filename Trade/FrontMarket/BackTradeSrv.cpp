
#include "BackTradeSrv.h"

BackTradeSrv::BackTradeSrv()
{
	uv_mutex_init(&m_lock);
}

BackTradeSrv::~BackTradeSrv()
{

}


void BackTradeSrv::NewConnection(uv_tcp_t *tcp)
{
	BackTradeSession *p=new BackTradeSession(tcp,this);
	p->Init();
	m_mSession.insert(make_pair(tcp,p));
}

// return <0: 协议错误，=0:数据包长度不足，>0:已处理掉的数据长度
int BackTradeSrv::Read(uv_tcp_t* tcp,char *pBuffer,int iDataLen)
{
	map<uv_tcp_t*,BackTradeSession*>::iterator it=m_mSession.find(tcp);
	if(it==m_mSession.end())return -10;

	return it->second->Read(pBuffer,iDataLen);
}


void BackTradeSrv::AddRequest(UProtocolBase *p)
{
	uv_mutex_lock(&m_lock);
	m_qReqest.push(p);
	uv_mutex_unlock(&m_lock);
}

void BackTradeSrv::GetRequest(queue<UProtocolBase*> &q)
{
	uv_mutex_lock(&m_lock);
	q.swap(m_qReqest);
	uv_mutex_unlock(&m_lock);
}
