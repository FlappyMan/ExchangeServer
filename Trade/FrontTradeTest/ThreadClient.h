#ifndef _THREAD_CLIENT_HEADER_
#define _THREAD_CLIENT_HEADER_

#include "ClientSrv.h"
#include "ThreadBackTrade.h"

void ThreadClient(void *arg);

extern uv_loop_t g_loop_client;
extern ClientSrv g_srv_client;
extern uint64_t m_uClientID;

void ThreadClient(void *arg);
int GetGMTime(char* szGmtTime);

void Client_cbWrited(uv_write_t* req, int status);
template<class T>
int32_t Client_Write(uv_tcp_t* client,T* resPkg,int status) 
{	
    UVWriteReq* req= g_cache_write_req.Get(SIZE_BUFFER_2k);
    if(req==NULL)return -1;
    char s_time[60] = {0};
    GetGMTime(s_time);
    string sHttpReq;
    if(resPkg && status == 200)
    {
        char buf[SIZE_BUFFER_2k] = {0};
        int ret = JsonPack<T>(resPkg,buf,SIZE_BUFFER_2k);
        if(ret<=0)
        {
            g_cache_write_req.Free(req);
            return ret;
        }
        string sBody=buf;
        sHttpReq="POST /api/private/upTrade HTTP/1.1\r\n";
        sHttpReq.append("Content-Type: application/json;charset=utf-8\r\n");
        char contentLen[20] = {0};	
        sprintf(contentLen,"Content-Length: %lu\r\n",sBody.length());
        sHttpReq.append(contentLen);
        sHttpReq.append("Connection: Keep-Alive\r\n");		
        sHttpReq.append("\r\n");
        sHttpReq.append(sBody);
    }else
    {
    }
    req->pkg = resPkg;
    req->buf.len = sHttpReq.length();
    strncpy(req->buf.base,(const char *)sHttpReq.c_str(),sHttpReq.length());
    uv_write((uv_write_t *)req, (uv_stream_t*)client, &req->buf, 1, Client_cbWrited);
};


#endif

