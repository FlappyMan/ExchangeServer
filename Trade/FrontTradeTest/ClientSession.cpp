#include "ClientSession.h"
#include "ThreadClient.h"

ClientSession::ClientSession(uv_tcp_t* tcp):m_tcp(tcp),m_uSessionID(0)
{
    tcp->data = this;
}

ClientSession::~ClientSession()
{

}

void ClientSession::Init()
{
    m_tLast = time(NULL);
    m_http.Init(HTTP_REQUEST);
    m_uSessionID = ++m_uClientID;
}

void ClientSession::Destroy()
{
    m_http.Destroy();
}

bool ClientSession::IsTimeout(time_t tNow)
{
    return tNow > m_tLast+50;
}

// return <0: 协议错误，=0:数据包长度不足，>0:已处理掉的数据长度
int ClientSession::Read(uv_tcp_t* client, char *pBuffer,int iDataLen)
{
    int ret = m_http.Readed(pBuffer,iDataLen);
    if(ret<0) return -1;
    if(ret>0) return ret;
    if(m_http.IsHeaderComplete()==false)
    {
        return -1;
    }
    UProtocolBase *pkg=NULL;
    string token;

    if (strcasecmp(m_http.m_sUrl.c_str(),"/api/private/upTrade") == 0)
    {
        pkg = new UPUptrade;
        m_http.GetFiled(token,"token");
        token.append(to_string(m_uSessionID));  
        dynamic_cast<UPUptrade *>(pkg)->set_token(token); 
        JsonUnpack((UPUptrade *)pkg, (char *)m_http.m_sBody.c_str(), (uint32_t)m_http.m_sBody.length());        
        g_srv_client.AddRequest(pkg);
        g_srv_client.InsertClientID(token,this);
        cout<<"m_http.m_sBody = "<<m_http.m_sBody.c_str()<<endl;
    }else if (strcasecmp(m_http.m_sUrl.c_str(),"/api/private/upTradeBatch") == 0)
    {
        pkg = new UPUptradebatch;
        m_http.GetFiled(token,"token");  
        token.append(to_string(m_uSessionID));
        dynamic_cast<UPUptradebatch *>(pkg)->set_token(token); 
        JsonUnpack((UPUptradebatch *)pkg, (char *)m_http.m_sBody.c_str(), (uint32_t)m_http.m_sBody.length());   
        g_srv_client.AddRequest(pkg);
        g_srv_client.InsertClientID(token,this);
        cout<<"m_http.m_sBody = "<<m_http.m_sBody.c_str()<<endl;
    }else if (strcasecmp(m_http.m_sUrl.c_str(),"/api/private/cancelTrade") == 0)
    {
        pkg = new UPCanceltrade;
        m_http.GetFiled(token,"token"); 
        token.append(to_string(m_uSessionID));
        dynamic_cast<UPCanceltrade *>(pkg)->set_token(token); 
        JsonUnpack((UPCanceltrade *)pkg, (char *)m_http.m_sBody.c_str(), (uint32_t)m_http.m_sBody.length());   
        g_srv_client.AddRequest(pkg);
        g_srv_client.InsertClientID(token,this);
        cout<<"m_http.m_sBody = "<<m_http.m_sBody.c_str()<<endl;
    }else if (strcasecmp(m_http.m_sUrl.c_str(),"/api/private/cancelTradeBatch") == 0)
    {
        pkg = new UPCanceltradebatch;
        m_http.GetFiled(token,"token");
        token.append(to_string(m_uSessionID));
        dynamic_cast<UPCanceltradebatch *>(pkg)->set_token(token); 
        JsonUnpack((UPCanceltradebatch *)pkg, (char *)m_http.m_sBody.c_str(), (uint32_t)m_http.m_sBody.length());   
        g_srv_client.AddRequest(pkg);
        g_srv_client.InsertClientID(token,this);
        cout<<"m_http.m_sBody = "<<m_http.m_sBody.c_str()<<endl;
    }else
    {    
        return -1;
    }
    cout<<"m_uSessionID = "<<m_uSessionID<<endl;
    return iDataLen;
}

