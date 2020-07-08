
#include "ThreadBackTrade.h"
#include "ThreadClient.h"
#include <sys/time.h>
uv_timer_t g_timer_backtrade;
uv_loop_t g_loop_backtrade;
BackTradeSrv g_srv_backtrade;
int session_cnt = 0;

int64_t getCurrentTime() //直接调用这个函数就行了，返回值最好是int64_t，long long应该也可以
{
    struct timeval tv;
    gettimeofday(&tv, NULL); //该函数在sys/time.h头文件中
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void BackTrade_cbTimer(uv_timer_t *handle)
{

    UBBlockQueue<UProtocolBase> qRes;
    g_srv_backtrade.GetResponse(qRes);
    g_srv_client.PushResponse(qRes);

    time_t tNow = time(NULL);
    g_srv_backtrade.OnTimer(tNow);
}

void BackTrade_cbReadBuff(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
    //cout<<"[Server] New read buffer"<<endl;
    suggested_size = SIZE_BUFFER_8k;
    buf->base = g_cache_read.Get(suggested_size);
    buf->len = suggested_size;
}

void BackTrade_cbClosed(uv_handle_t *handle)
{
    map<uv_tcp_t *, BackTradeSession *>::iterator it = g_srv_backtrade.m_mSession.find((uv_tcp_t *)handle);
    if (it != g_srv_backtrade.m_mSession.end())
    {
        delete it->second;
        g_srv_backtrade.m_mSession.erase(it);
    }
    delete (uv_tcp_t *)handle;
}

// uv__write_callbacks
void BackTrade_cbWrited(uv_write_t *req, int status)
{
    if (status < 0)
    {
        //cout<<"[Server] Write failed"<<endl;
    }
    // uv_close((uv_handle_t *)req->handle, BackTrade_cbClosed);
    g_cache_write_req.Free((UVWriteReq *)req);
}

void BackTrade_cbRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
    cout << "[Server] Server begin read" << endl;
    if (nread < 0)
    {
        if (nread != UV_EOF)
            cout << "Read error :" << uv_err_name(nread) << endl;
        uv_close((uv_handle_t *)client, BackTrade_cbClosed);

        g_cache_read.Free(buf->base, buf->len);
        return;
    }
    if (nread == 0)
    {
        g_cache_read.Free(buf->base, buf->len);
        //cout<<"read noting"<<endl;
        return;
    }

    // cout << "[BackTrade] readed :" << buf->base << endl;

    g_srv_backtrade.Read((uv_tcp_t *)client, buf->base, nread);
    g_cache_read.Free(buf->base, buf->len);
    if (session_cnt >= 50000)
    {
        uv_close((uv_handle_t *)client, BackTrade_cbClosed);
        session_cnt = 0;
        cout<<"send msg wait"<<endl;
    }
    else
    {
        // cout<<"send message idx = "<<session_cnt<<endl;
        // session_cnt++;
        // stringstream ss;
        // string sRandom = "tokenID";
        // ss << getCurrentTime();
        // ss >> sRandom;
        // cout << "token = " << sRandom << endl;
        // UPUptrade *up = new UPUptrade;
        // up->set_token(sRandom);
        // up->set_uid("11128");
        // up->set_paypassword("123456");
        // up->set_marketid(1);
        // up->set_price(1.0);
        // up->set_num(3);
        // up->set_type(1);
        // up->set_main_coin(1);

        // const int BUFFSIZE = ProtoPackSize<UPUptrade>(*up);
        // UVWriteReq *write = g_cache_write_req.Get(BUFFSIZE);
        // assert(write != NULL);
        // assert(write->buf.base != NULL);
        // write->buf.len = ProtoPack<UPUptrade>(write->buf.base, BUFFSIZE, *up);
        // assert(write->buf.len > 0);
        // write->pkg = up;
        // uv_write((uv_write_t *)write, (uv_stream_t *)client, &write->buf, 1, BackTrade_cbWrited);
    }

    // cout << "------------------重新发送数据包------------- " << endl;
}
void cancelTradeBatch(uv_tcp_t *client)
{
    // required    string  uid = 1; 
    // required    string	token = 2;
    // message Orders
    // {
    //     required    uint64	orderid = 1;
    //     required    uint64	marketid = 2;
    //     required    uint32  type = 3;//1、买入；2、卖出
    //     optional    string  price = 4;
    //     optional    string  number = 5;
    // }
    // repeated    Orders  orders = 3;
    // required    uint64  canceltradebatchid = 4;
    srand((unsigned)time(NULL));
    stringstream ss;
    string sRandom = "tokenID";
    ss << getCurrentTime();
    ss >> sRandom;
    cout << "token = " << sRandom << endl;

    UPCanceltradebatch *up = new UPCanceltradebatch;
    up->set_uid("11109");
    up->set_token(sRandom);
    up->set_canceltradebatchid(123456);
    for (int i = 0; i < 2; ++i)
    {
        UPCanceltradebatch::Orders *pTrades = up->add_orders();
        pTrades->set_marketid(138);
        // pTrades->set_price("2.0");
        // pTrades->set_number("3.0");
        pTrades->set_type(1);
        pTrades->set_orderid(123456 + i); //stoll(sRandom)
    }
    const int BUFFSIZE = ProtoPackSize<UPCanceltradebatch>(*up);
    UVWriteReq *write = g_cache_write_req.Get(BUFFSIZE);
    assert(write != NULL);
    assert(write->buf.base != NULL);
    write->buf.len = ProtoPack<UPCanceltradebatch>(write->buf.base, BUFFSIZE, *up);
    assert(write->buf.len > 0);
    write->pkg = up;
    uv_write((uv_write_t *)write, (uv_stream_t *)client, &write->buf, 1, BackTrade_cbWrited);
}
void uptradeBatch(uv_tcp_t *client)
{
    srand((unsigned)time(NULL));
    stringstream ss;
    string sRandom = "tokenID";
    ss << getCurrentTime();
    ss >> sRandom;
    cout << "token = " << sRandom << endl;

    UPUptradebatch *up = new UPUptradebatch;
    up->set_uid("11109");
    up->set_token(sRandom);
    up->set_paypassword("123456");
    up->set_uptradebatchid(123456);
    for(int i=0; i < 2; ++i)
    {   
        UPUptradebatch::Trades* pTrades = up->add_trades();
        pTrades->set_marketid(138);
        pTrades->set_price("2.0");
        pTrades->set_num("3.0");
        pTrades->set_type(1);
        pTrades->set_orderid(123456 + i); //stoll(sRandom)
    }
    const int BUFFSIZE = ProtoPackSize<UPUptradebatch>(*up);
    UVWriteReq *write = g_cache_write_req.Get(BUFFSIZE);
    assert(write != NULL);
    assert(write->buf.base != NULL);
    write->buf.len = ProtoPack<UPUptradebatch>(write->buf.base, BUFFSIZE, *up);
    assert(write->buf.len > 0);
    write->pkg = up;
    uv_write((uv_write_t *)write, (uv_stream_t *)client, &write->buf, 1, BackTrade_cbWrited);
}
void uptrade(uv_tcp_t *client)
{
    srand((unsigned)time(NULL));
    stringstream ss;
    string sRandom = "tokenID";
    ss << getCurrentTime();
    ss >> sRandom;
    cout << "token = " << sRandom << endl;

    UPUptrade *up = new UPUptrade;
    up->set_token(sRandom);
    up->set_uid("11109");
    up->set_paypassword("123456");
    up->set_marketid(138);
    up->set_price("1.0");
    up->set_num("2");
    up->set_type(1);
    up->set_orderid(123456);
    up->set_uptradeid(123456);

    const int BUFFSIZE = ProtoPackSize<UPUptrade>(*up);
    UVWriteReq *write = g_cache_write_req.Get(BUFFSIZE);
    assert(write != NULL);
    assert(write->buf.base != NULL);
    write->buf.len = ProtoPack<UPUptrade>(write->buf.base, BUFFSIZE, *up);
    assert(write->buf.len > 0);
    write->pkg = up;
    uv_write((uv_write_t *)write, (uv_stream_t *)client, &write->buf, 1, BackTrade_cbWrited);
}
void cancelTrade(uv_tcp_t *client)
{
    srand((unsigned)time(NULL));
    stringstream ss;
    string sRandom = "tokenID";
    ss << getCurrentTime();
    ss >> sRandom;
    cout << "token = " << sRandom << endl;

    UPCanceltrade *up = new UPCanceltrade;
    up->set_uid("11109");
    up->set_token(sRandom);
    up->set_orderid(123456);
    up->set_marketid(138);
    up->set_canceltradeid(123456);
    up->set_type(1);

    const int BUFFSIZE = ProtoPackSize<UPCanceltrade>(*up);
    UVWriteReq *write = g_cache_write_req.Get(BUFFSIZE);
    assert(write != NULL);
    assert(write->buf.base != NULL);
    write->buf.len = ProtoPack<UPCanceltrade>(write->buf.base, BUFFSIZE, *up);
    assert(write->buf.len > 0);
    write->pkg = up;
    uv_write((uv_write_t *)write, (uv_stream_t *)client, &write->buf, 1, BackTrade_cbWrited);
}
void BackTrade_cbNewConnection(uv_stream_t *server, int status)
{
    cout << "[BackTrade] Accept new connection" << endl;
    if (status < 0)
    {
        //cout<<"[BackTrade] New connection error :"<<uv_strerror(status)<<endl;
        return;
    }

    uv_tcp_t *client = new uv_tcp_t;
    uv_tcp_init(&g_loop_backtrade, client);
    if (uv_accept(server, (uv_stream_t *)client) == 0)
    {
        g_srv_backtrade.NewConnection(client);
        // cout << "[BackTrade] New connection begin read : " << endl;
        uptrade(client);
        // cancelTrade(client);
        // uptradeBatch(client);
        // cancelTradeBatch(client);
    }
    else
    {
        cout << "[BackTrade] New connection accept failed" << endl;
        uv_close((uv_handle_t *)client, BackTrade_cbClosed);
    }
}

void ThreadBacktrade(void *arg)
{
    uv_loop_init(&g_loop_backtrade);

    uv_tcp_t server;
    uv_tcp_init(&g_loop_backtrade, &server);

    struct sockaddr_in addr;
    uv_ip4_addr(g_config.m_sListenIP_BackTrade.c_str(), g_config.m_uiListenPort_BackTrade, &addr);

    uv_tcp_bind(&server, (const struct sockaddr *)&addr, 0);
    int ret = uv_listen((uv_stream_t *)&server, 1000, BackTrade_cbNewConnection);
    if (ret)
    {
        cout << "[Client] Listen error :" << uv_strerror(ret) << endl;
        return;
    }

    uv_timer_init(&g_loop_backtrade, &g_timer_backtrade);
    uv_timer_start(&g_timer_backtrade, BackTrade_cbTimer, 10, 1);

    uv_run(&g_loop_backtrade, UV_RUN_DEFAULT);
}
