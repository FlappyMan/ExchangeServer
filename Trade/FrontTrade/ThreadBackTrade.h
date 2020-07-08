#ifndef _THREAD_BACKTRADE_HEADER_
#define _THREAD_BACKTRADE_HEADER_

#include "BackTradeSession.h"
#include "BackTradeSrv.h"

void BackTrade_cbWrited(uv_write_t* req, int status);

void ThreadBacktrade(void *arg);
extern uv_loop_t g_loop_backtrade;
extern BackTradeSrv g_srv_backtrade;

template<class T>
int32_t BackTrade_Write(uv_stream_t* client,T* pkg,uint32_t uiPkgMaxSize=SIZE_BUFFER_2k) 
{	
    UVWriteReq* req=g_cache_write_req_BackTrade.Get(uiPkgMaxSize);
    int ret=ProtoPack<T>(req->buf.base,req->len,*pkg);
    if(ret<=0)
    {
        req->pkg = pkg;
        g_cache_write_req_BackTrade.Free(req);
        return ret;
    }
    req->buf.len = ret;
    req->pkg=pkg;
    uv_write((uv_write_t *)req, client, &req->buf, 1, BackTrade_cbWrited);
};

#endif

