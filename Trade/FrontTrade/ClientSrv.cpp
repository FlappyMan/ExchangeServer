#include "ClientSrv.h"
#include "ThreadClient.h"

ClientSrv::ClientSrv(int iBufferSize)
{
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
    m_mSession.insert(pair<uv_tcp_t*,ClientSession*>(tcp,p));
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
    if(it==m_mSession.end())return false;

    return it->second->Read(tcp, pBuffer, iDataLen);
}

void ClientSrv::PushResponse(UBBlockQueue<UProtocolBase> &res)
{
    while (res.size()>0)
    {
        m_qResponse.put(res.get());
    }
}

void ClientSrv::GetRequest(UBBlockQueue<UProtocolBase> &req)
{
    while (m_qRequest.size()>0)
    {
        req.put(m_qRequest.get());
    }
}

void ClientSrv::AddRequest(UProtocolBase* pkg)
{
    m_qRequest.put(pkg);
}

void ClientSrv::OnTimer(time_t tNow)
{
    map<uv_tcp_t*,ClientSession*>::iterator it;
    for(it=m_mSession.begin();it!=m_mSession.end();it++)
    {
        if (it->second->IsTimeout(tNow))
        {
            it->second->m_bActive = false;
            uv_close((uv_handle_t*)it->second->m_tcp,Client_cbClosed);
        }
        while(m_qResponse.size()>0 && it->second->m_bActive)
        {
            _DispatchPkg(m_qResponse.get());
        }  
    }
}

void ClientSrv::_DispatchPkg(UProtocolBase* pkg)
{
	switch (pkg->m_uiType)
    {
        case UPResponse::CMD :
            _DispatchPkg_Response((UPResponse*)pkg);
            break;
        default:
            break;
    }
}
        
void ClientSrv::_DispatchPkg_Response(UPResponse* pkg)
{
    map<string,ClientSession*>::iterator it;
    it = m_mSessionRecord.find(pkg->m_sTag);
    if(it != m_mSessionRecord.end())
    {
        Client_Write<UPResponse>(it->second->m_tcp,pkg,200);
    }
    // else
    // {
    //     m_mSessionRecord.erase(it);
    // }
}

void ClientSrv::SaveSession(string &SessionID, ClientSession* p)
{
    m_mSessionRecord.insert(make_pair(SessionID,p));
}
