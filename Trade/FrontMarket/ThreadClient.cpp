
#include "ThreadClient.h"

uv_timer_t g_timer_client;
uv_loop_t g_loop_client;
ClientSrv g_srv_client;
UBWsParser g_ws;


void Client_cbTimer(uv_timer_t* handle)
{
	time_t tNow=time(NULL);
	g_srv_client.OnTimer(tNow);
}


void Client_cbClosed(uv_handle_t* handle) 
{
	g_srv_client.CloseConnection((uv_tcp_t*)handle);
	delete (uv_tcp_t*)handle;
}

void Client_cbReadBuff(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) 
{
	cout<<"[Client] New read buffer"<<endl;
	suggested_size=SIZE_BUFFER_8k;
	buf->base = g_cache_read.Get(suggested_size);
	buf->len = suggested_size;
}

void Client_cbRead(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) 
{
	cout<<"[Client] begin read"<<endl;
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
		g_cache_read.Free(buf->base,buf->len);
		return;
	}

	//cout<<"[Client] readed :"<<buf->base<<endl;
	if(g_srv_client.Read((uv_tcp_t*)client,buf->base, nread)<0)
	{
		uv_close((uv_handle_t*) client, Client_cbClosed);
	}

	g_cache_read.Free(buf->base,buf->len);
}


void Client_cbWrited(uv_write_t* req, int status)
{
	if (status < 0)
	{
		cout<<"[Client] Write failed"<<endl;
	}
    g_cache_write_req_Client_Frame.Free((UVWriteReq*)req);
}


void Client_cbNewConnection(uv_stream_t *server, int status) 
{
    cout<<"[Client] Accept new connection"<<endl;
	if (status < 0) 
	{
		cout<<"[Client] New connection error :"<<uv_strerror(status)<<endl;
		return;
	}

	uv_tcp_t *client = new uv_tcp_t;
	uv_tcp_init(&g_loop_client, client);
	if (uv_accept(server, (uv_stream_t*) client) == 0)
	{
		int ret=uv_read_start((uv_stream_t*) client, Client_cbReadBuff, Client_cbRead);
		cout<<"[Client] New connection begin read : "<<ret<<endl;

		g_srv_client.NewConnection(client);
	}
	else
	{
		cout<<"[Client] New connection accept failed"<<endl;
		uv_close((uv_handle_t*) client, Client_cbClosed);
	}
}

void ThreadClient(void *arg)
{
    uv_loop_init(&g_loop_client);

	uv_tcp_t server;
	uv_tcp_init(&g_loop_client, &server);

	struct sockaddr_in addr;
	uv_ip4_addr(g_config.m_sListenIP_Client.c_str(), g_config.m_uiListenPort_Client, &addr);

	uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
	int ret = uv_listen((uv_stream_t*) &server, 1000, Client_cbNewConnection);
	if (ret) 
	{
		cout<<"[Client] Listen error :"<<uv_strerror(ret)<<endl;
		return;
	}

	uv_timer_init(&g_loop_client, &g_timer_client);
	uv_timer_start(&g_timer_client, Client_cbTimer, 10, 1); 

	uv_run(&g_loop_client, UV_RUN_DEFAULT);
}

