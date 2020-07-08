#include "ThreadBackTrade.h"
#include "ThreadClient.h"

uv_timer_t g_timer_backtrade;
uv_timer_t g_timer_resetOrderID;
uv_loop_t g_loop_backtrade;
BackTradeSrv g_srv_backtrade;

void BackTrade_cbTimer(uv_timer_t* handle)
{
    UBBlockQueue<UProtocolBase> qRes;
    g_srv_backtrade.GetResponse(qRes);
    g_srv_client.PushResponse(qRes);

    g_srv_backtrade.OnTimer();
}

void resetSelfIncID_cbTimer(uv_timer_t* handle)
{
    uv_mutex_lock(&g_mutex);
    g_uiOrderID = 1;
    uv_mutex_unlock(&g_mutex);
}

void BackTrade_cbReadBuff(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) 
{
    //cout<<"[Server] New read buffer"<<endl;
    suggested_size = SIZE_BUFFER_8k;
    buf->base = g_cache_read.Get(suggested_size);
    buf->len = suggested_size;
}

void BackTrade_cbClosed(uv_handle_t* handle) 
{
    delete (uv_tcp_t*)handle;
}

// uv__write_callbacks
void BackTrade_cbWrited(uv_write_t* req, int status)
{
    if (status < 0)
    {
        //cout<<"[Server] Write failed"<<endl;
    }
    g_cache_write_req_BackTrade.Free((UVWriteReq*)req);
}

void BackTrade_cbRead(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) 
{
    //cout<<"[Server] Server begin read"<<endl;
    if (nread < 0)
    {
        if (nread != UV_EOF)cout<<"[ThreadBackTrade] Read error :"<<uv_err_name(nread)<<endl;
        assert(nread == UV_ECONNRESET || nread == UV_EOF);
        uv_close((uv_handle_t*) client, BackTrade_cbClosed);

        g_cache_read.Free(buf->base,buf->len);
        return;
    }
    if(nread==0)
    {
        g_cache_read.Free(buf->base,buf->len);
        //cout<<"read noting"<<endl;
        return;
    }

    //cout<<"[BackTrade] readed :"<<buf->base<<endl;

    g_srv_backtrade.Read((uv_tcp_t*)client,buf->base, nread);
    g_cache_read.Free(buf->base,buf->len);
}


void BackTrade_cbNewConnection(uv_stream_t *server, int status) 
{
    //cout<<"[BackTrade] Accept new connection"<<endl;
    if (status < 0) 
    {
        //cout<<"[BackTrade] New connection error :"<<uv_strerror(status)<<endl;
        return;
    }

    uv_tcp_t *client = new uv_tcp_t;
    uv_tcp_init(&g_loop_backtrade, client);
    if (uv_accept(server, (uv_stream_t*) client) == 0)
    {
        cout<<"[BackTrade] New connection begin read : "<<endl;
        int ret=uv_read_start((uv_stream_t*) client, BackTrade_cbReadBuff, BackTrade_cbRead);
        g_srv_backtrade.NewConnection(client); 
    }
    else 
    {
        cout<<"[BackTrade] New connection accept failed"<<endl;
        uv_close((uv_handle_t*) client, BackTrade_cbClosed);
    }
}


void ThreadBacktrade(void *arg)
{
    uv_loop_init(&g_loop_backtrade);
    //g_loop_backtrade = uv_default_loop();
    uv_mutex_init(&g_mutex);

    uv_tcp_t server;
    uv_tcp_init(&g_loop_backtrade, &server);

    struct sockaddr_in addr;
    uv_ip4_addr(g_config.m_sListenIP_BackTrade.c_str(), g_config.m_uiListenPort_BackTrade, &addr);

    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    int ret = uv_listen((uv_stream_t*) &server, 1000, BackTrade_cbNewConnection);
    if (ret) 
    {
        cout<<"[Client] Listen error :"<<uv_strerror(ret)<<endl;
        return;
    }

    uv_timer_init(&g_loop_backtrade, &g_timer_backtrade);
    uv_timer_start(&g_timer_backtrade, BackTrade_cbTimer, 10, 1); 

    uv_timer_init(&g_loop_backtrade,&g_timer_resetOrderID);
    uv_timer_start(&g_timer_resetOrderID,resetSelfIncID_cbTimer,10, 1000);

    uv_run(&g_loop_backtrade, UV_RUN_DEFAULT);
}

