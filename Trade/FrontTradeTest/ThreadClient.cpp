#include "ThreadClient.h"

uv_timer_t g_timer_client;
uv_loop_t g_loop_client;
ClientSrv g_srv_client;
uint64_t m_uClientID = 0;

void Client_cbTimer(uv_timer_t* handle)
{
    UBBlockQueue<UProtocolBase> qReq;
    g_srv_client.GetRequest(qReq);
    g_srv_backtrade.PushRequest(qReq);

    time_t tNow = time(NULL);
    g_srv_client.OnTimer(tNow);
}

void Client_cbClosed(uv_handle_t* handle)
{
    map<uv_tcp_t*,ClientSession*>::iterator it=g_srv_client.m_mSession.find((uv_tcp_t*)handle);
    if (it != g_srv_client.m_mSession.end())
    {
        it->second->Destroy();
        delete it->second;
        g_srv_client.m_mSession.erase(it);
    }
    //delete (uv_tcp_t*)handle;
}

void Client_cbReadBuff(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) 
{
    cout<<"[Client] New read buffer"<<endl;
    suggested_size = SIZE_BUFFER_8k;
    buf->base = g_cache_read.Get(suggested_size);
    buf->len = suggested_size;
}

void Client_cbWrited(uv_write_t* req, int status)
{
    if (status < 0)
    {
        cout<<"[Client] Write failed"<<endl;
    }
    uv_close((uv_handle_t*)req->handle, Client_cbClosed);
    g_cache_write_req.Free((UVWriteReq*)req);
}

void Client_cbRead(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) 
{
    cout<<"[Client] Client begin read"<<endl;
    if (nread < 0)
    {
        if (nread != UV_EOF)cout<<"Read error :"<<uv_err_name(nread)<<endl;
        uv_close((uv_handle_t*) client, Client_cbClosed);
        g_cache_read.Free(buf->base,buf->len);
        return;
    }
    if(nread==0)
    {
        cout<<"read noting"<<endl;
        uv_close((uv_handle_t*) client, Client_cbClosed);
        g_cache_read.Free(buf->base,buf->len);
        return;
    }

    cout<<"[Client] readed :"<<buf->base<<endl;
    int ret = g_srv_client.Read((uv_tcp_t*)client, buf->base, nread);
    if(ret < 0)
    {
        uv_close((uv_handle_t*) client, Client_cbClosed);
    }
    g_cache_read.Free(buf->base,buf->len);
}


void Client_cbNewConnection(uv_stream_t *server, int status) 
{
    //cout<<"[Client] Accept new connection"<<endl;
    if (status < 0) 
    {
        //cout<<"[Client] New connection error :"<<uv_strerror(status)<<endl;
        return;
    }
    uv_tcp_t *client = new uv_tcp_t;
    uv_tcp_init(&g_loop_client, client);

    if (uv_accept(server, (uv_stream_t*) client) == 0)
    {
        g_srv_client.NewConnection(client);
        int ret=uv_read_start((uv_stream_t*) client, Client_cbReadBuff, Client_cbRead);
        //cout<<"[Client] New connection begin read : "<<ret<<endl;
    }
    else
    {
        cout<<"[Client] New connection accept failed"<<endl;
        uv_close((uv_handle_t*) client, Client_cbClosed);
    }
}

void on_connect(uv_connect_t* req,int status)
{
    if(status)
    {
        cout<<"Connect  error"<<endl;
        uv_close((uv_handle_t*)req->handle,NULL);
        cout<<"close socket!"<<endl;
    }
    // UPUptrade* res = NULL;
    // res = new UPUptrade;
    // res->set_main_coin(5);
    // res->set_marketid(1);
    // res->set_num(66);
    // res->set_paypassword("123456");
    // res->set_price(123);
    // res->set_token("999999");
    // res->set_type(555);
    // Client_Write<UPUptrade>((uv_tcp_t*)req->handle,res,200);

    // int ret=uv_read_start((uv_stream_t*)req->handle, Client_cbReadBuff, Client_cbRead);
    // if(ret)
    // {
    //     cout<<"[Client] uv_read_start error"<<endl;
    // }else{
    //     cout<<"[Client] uv_read finished"<<endl;
    // }
}

void ThreadClient(void *arg)
{
    uv_loop_init(&g_loop_client);

    uv_tcp_t *server = NULL;
    uv_connect_t connect;

    server = new uv_tcp_t;
    uv_tcp_init(&g_loop_client, server);

    struct sockaddr_in addr;
    uv_ip4_addr(g_config.m_sListenIP_Client.c_str(), g_config.m_uiListenPort_Client, &addr);

    uv_tcp_connect(&connect,server,(const struct sockaddr*)&addr,on_connect);

    uv_timer_init(&g_loop_client, &g_timer_client);
    uv_timer_start(&g_timer_client, Client_cbTimer, 10, 1); 

    uv_run(&g_loop_client, UV_RUN_DEFAULT);
}


int GetGMTime(char* szGmtTime)
{   
    if (!szGmtTime)
    {
        return -1;
    }
    struct tm timeInfo;
    time_t cur_time=time(NULL);	
    char szTemp[30] = {0};
    gmtime_r(&cur_time, &timeInfo);  
    strftime(szTemp, sizeof(szTemp), "%a, %d %b %Y %H:%M:%S GMT", &timeInfo);
    strcpy(szGmtTime, szTemp); 
    return 0;
}
