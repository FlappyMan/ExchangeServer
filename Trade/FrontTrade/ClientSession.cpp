#include "ClientSession.h"
#include "ThreadClient.h"

ClientSession::ClientSession(uv_tcp_t* tcp,char *pBuffer,int iBufferSize):
    m_tcp(tcp),m_uSessionID(0),m_iBufferSize(iBufferSize),m_tLast(0),m_bActive(false)
{
    tcp->data = this;
}

ClientSession::~ClientSession()
{
    m_http.Destroy();
}

void ClientSession::Init()
{
    m_tLast = time(NULL);
    m_http.Init(HTTP_REQUEST);
    m_uSessionID = ++g_uiSessionID;
    m_bActive = true;
}

void ClientSession::Destroy()
{
    m_http.Destroy();
}

bool ClientSession::IsTimeout(time_t tNow)
{
    return tNow > m_tLast+30;
}

// return <0: 协议错误，=0:数据包长度不足，>0:已处理掉的数据长度
int ClientSession::Read(uv_tcp_t* client, char *pBuffer,int iDataLen)
{
    int ret = m_http.Readed(pBuffer,iDataLen);
    if(ret<0) return -1;
    if(ret>0) return ret;

    UProtocolBase *pkg=NULL;
    string token;
    string sessionID;

    if (strcasecmp(m_http.m_sUrl.data(),"/api/private/upTrade") == 0)
    {
        pkg = new UPUptrade;
        JsonUnpack((UPUptrade *)pkg, (char *)m_http.m_sBody.data(), (uint32_t)m_http.m_sBody.length());
        m_http.GetFiled(token,"token");
        sessionID = to_string(m_uSessionID);
        dynamic_cast<UPUptrade *>(pkg)->set_token(token);
        pkg->m_sTag = sessionID;

        g_srv_client.AddRequest(pkg);
        g_srv_client.SaveSession(sessionID,this);
        cout<<"m_http.m_sBody = "<<m_http.m_sBody.c_str()<<endl;
    }else if (strcasecmp(m_http.m_sUrl.data(),"/api/private/upTradeBatch") == 0)
    {
        pkg = new UPUptradebatch;
        JsonUnpack((UPUptradebatch *)pkg, (char *)m_http.m_sBody.data(), (uint32_t)m_http.m_sBody.length());
        m_http.GetFiled(token,"token"); 
        sessionID = to_string(m_uSessionID);
        dynamic_cast<UPUptradebatch *>(pkg)->set_token(token);
        pkg->m_sTag = sessionID;

        g_srv_client.AddRequest(pkg);
        g_srv_client.SaveSession(sessionID,this);
        cout<<"m_http.m_sBody = "<<m_http.m_sBody.c_str()<<endl;
    }else if (strcasecmp(m_http.m_sUrl.data(),"/api/private/cancelTrade") == 0)
    {
        pkg = new UPCanceltrade; 
        JsonUnpack((UPCanceltrade *)pkg, (char *)m_http.m_sBody.data(), (uint32_t)m_http.m_sBody.length());   
        m_http.GetFiled(token,"token");
        sessionID = to_string(m_uSessionID);
        dynamic_cast<UPCanceltrade *>(pkg)->set_token(token);
        pkg->m_sTag = sessionID;

        g_srv_client.AddRequest(pkg);
        g_srv_client.SaveSession(sessionID,this);
        cout<<"m_http.m_sBody = "<<m_http.m_sBody.c_str()<<endl;
    }else if (strcasecmp(m_http.m_sUrl.data(),"/api/private/cancelTradeBatch") == 0)
    {
        pkg = new UPCanceltradebatch;
        JsonUnpack((UPCanceltradebatch *)pkg, (char *)m_http.m_sBody.data(), (uint32_t)m_http.m_sBody.length());
        m_http.GetFiled(token,"token");
        sessionID = to_string(m_uSessionID);
        dynamic_cast<UPCanceltradebatch *>(pkg)->set_token(token);
        pkg->m_sTag = sessionID;

        g_srv_client.AddRequest(pkg);
        g_srv_client.SaveSession(sessionID,this);
        cout<<"m_http.m_sBody = "<<m_http.m_sBody.c_str()<<endl;
    }else
    {    
        return -1;
    }
    cout<<"m_uSessionID = "<<m_uSessionID<<endl;
    return iDataLen;
}