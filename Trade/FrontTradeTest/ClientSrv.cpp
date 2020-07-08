
#include "ClientSrv.h"
#include "ThreadClient.h"

ClientSrv::ClientSrv()
{
    // uv_mutex_init(&m_lock);
}

ClientSrv::~ClientSrv()
{

}

void ClientSrv::NewConnection(uv_tcp_t* tcp)
{
    ClientSession *p=new ClientSession(tcp);
    p->Init();
    m_mSession.insert(pair<uv_tcp_t*,ClientSession*>(tcp,p));
}

void ClientSrv::CloseConnection(uv_tcp_t *tcp)
{
    m_mSession.erase(tcp);
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
    while (req.size()>0)
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
            it->second->Destroy();
            delete it->second;
            m_mSession.erase(it);
            continue;
        }
    } 
#if 1
    while(m_qResponse.size()>0)
    {
        _DispatchPkg(m_qResponse.get());
    }  
#else
    UPResponse* resPkg = NULL;
    map<uv_tcp_t*,ClientSession*>::iterator iter = m_mSession.begin();
    for (;iter!=m_mSession.end();iter++)
    {
        resPkg = new UPResponse;
        resPkg->set_status(1);
        resPkg->set_data("交易成功!");
        Client_Write<UPResponse>((uv_stream_t*)(iter->first),resPkg,200);
    }
#endif
}

void ClientSrv::_DispatchPkg(UProtocolBase* pkg)
{
    UPResponse *res = (UPResponse *)pkg;
    map<string,ClientSession*>::iterator it = m_mClientID.begin();
    for (; it!=m_mClientID.end(); it++)
    {
        if (it->first.compare(res->token()) == 0)
        {
            Client_Write<UPResponse>((uv_tcp_t*)it->second->m_tcp,res,200);
        }  
    }  
}

void ClientSrv::InsertClientID(string &token, ClientSession* p)
{
    m_mClientID.insert(make_pair(token,p));
}
