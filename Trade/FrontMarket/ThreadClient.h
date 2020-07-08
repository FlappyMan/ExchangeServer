

#ifndef _THREAD_CLIENT_HEADER_
#define _THREAD_CLIENT_HEADER_

#include "ClientSrv.h"

extern uv_loop_t g_loop_client;
extern ClientSrv g_srv_client;
extern UBWsParser g_ws;

void ThreadClient(void *arg);
void Client_cbWrited(uv_write_t* req, int status);

template<class T>
int32_t Client_Write(uv_tcp_t* client,T* pkg,uint32_t uiPkgMaxSize=SIZE_BUFFER_2k) 
{	
	UVWriteReq* req=g_cache_write_req_Client.Get(uiPkgMaxSize);
	if(req==NULL)return -1;

	int ret=JsonPack<T>(pkg,req->buf.base,req->len);
	if(ret<=0)
	{
		g_cache_write_req_Client.Free(req);
		return ret;
	}

	char g_sReplyFrame[SIZE_BUFFER_2k] = {0};
	int retWS = g_ws.WSSetFrame(TEXT_FRAME,(uint8_t*)req->buf.base,ret,(uint8_t*)g_sReplyFrame,SIZE_BUFFER_2k);
	g_cache_write_req_Client.Free(req);

	UVWriteReq* reqReply=g_cache_write_req_Client_Frame.Get(uiPkgMaxSize);
	if(reqReply==NULL)return -1;
	memcpy(reqReply->buf.base,(const char *)g_sReplyFrame,retWS);
	reqReply->buf.len = retWS;
	reqReply->pkg = pkg;

	uv_write((uv_write_t *)reqReply, (uv_stream_t*)client, &reqReply->buf, 1, Client_cbWrited);
};

#endif

