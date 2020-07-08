#include "UProtocol.h"


enum{FRAME_SIZE=1024};
string g_sHttpRequest;
string g_sHttpReply;

char g_sRequestFrame[FRAME_SIZE];
int g_iRequestFrameLen=0;
string g_strReqquest="hello";

char g_sReplyFrame[FRAME_SIZE];
int g_iReplyFrameLen=0;
string g_strReply;


UBWsParser g_ws_server;
UBWsParser g_ws_client;

void InitWetSocket()
{
	g_sHttpRequest="GET / HTTP/1.1\r\n";
	g_sHttpRequest.append("Host: localhost:8080\r\n");
	g_sHttpRequest.append("Connection: Upgrade\r\n");
	g_sHttpRequest.append("Upgrade: websocket\r\n");
	g_sHttpRequest.append("Origin: http://127.0.0.1:3000\r\n");
	g_sHttpRequest.append("Sec-WebSocket-Version: 13\r\n");
	g_sHttpRequest.append("Sec-WebSocket-Key: w4v7O6xFTi36lq3RNcgctw==\r\n");
	g_sHttpRequest.append("\r\n");

	g_sHttpReply.append("HTTP/1.1 101 Switching Protocols\r\n");
	g_sHttpReply.append("Connection:Upgrade\r\n");
	g_sHttpReply.append("Upgrade: websocket\r\n");
	g_sHttpReply.append("Sec-WebSocket-Accept: Oy4NRAQ13jhfONC7bP8dTKb4PTU=\r\n");


	UBWsParser wp;
	g_iRequestFrameLen=wp.WSSetFrame(TEXT_FRAME,(uint8_t*)g_strReqquest.data(),g_strReqquest.length(),(uint8_t*)g_sRequestFrame,FRAME_SIZE);
	g_iReplyFrameLen=wp.WSSetFrame(TEXT_FRAME,(uint8_t*)g_strReply.data(),g_strReply.length(),(uint8_t*)g_sReplyFrame,FRAME_SIZE);
}

void ThreadTest1(void *arg)
{
	int iSleep = *((int *) arg);
	while (iSleep>0) 
	{
		iSleep--;
		sleep(1);
		cout<<"Thread1"<<endl;
	}
	cout<<"Thread1 finished"<<endl;
}

void ThreadTest2(void *arg)
{
	int iSleep = *((int *) arg);
	while (iSleep>0) 
	{
		iSleep--;
		sleep(1);
		cout<<"Thread2"<<endl;
	}
	cout<<"Thread2 finished"<<endl;
}

void ThreadTest() 
{
	int arg = 10;
	uv_thread_t t1,t2;
	uv_thread_create(&t1, ThreadTest1, &arg);
	uv_thread_create(&t2, ThreadTest2, &arg);

	uv_thread_join(&t1);
	uv_thread_join(&t2);
}

void EmptyLoop()
{
	uv_loop_t *loop = new uv_loop_t;
	uv_loop_init(loop);

	cout<<"Empty loop begin"<<endl;
	uv_run(loop, UV_RUN_DEFAULT);
	cout<<"Empty loop end"<<endl;

	uv_loop_close(loop);
	delete loop;
}


// 以下是 server / client 相关测试
uv_loop_t loop_server;
uv_loop_t loop_client;
enum
{
	DEFAULT_PORT=9000,
	DEFAULT_BACKLOG=1000,
};

typedef struct 
{
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

void free_write_req(uv_write_t *req) 
{
    write_req_t *wr = (write_req_t*) req;
    delete []wr->buf.base;
    delete wr;
}

void ServerTest_Closed(uv_handle_t* handle) 
{
   delete (uv_tcp_t*)handle;
}

void ServerTest_Writed(uv_write_t* req, int status)
{
	if (status < 0)
	{
		cout<<"\t [Server] Write failed"<<endl;
	}
    free_write_req(req);
}

void ServerTest_Read(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf) 
{
	cout<<"\t [Server] Server begin read:"<<nread<<endl;
	if (nread < 0)
	{
		delete []buf->base;

		if (nread != UV_EOF)cout<<"Read error :"<<uv_err_name(nread)<<endl;
		uv_close((uv_handle_t*) client, ServerTest_Closed);
		uv_stop(&loop_server);
		return;
	}
	if(nread==0)
	{
		delete []buf->base;
		cout<<"read noting"<<endl;
		return;
	}

//	cout<<"\t [Server] readed :"<<buf->base<<endl;
	if(g_ws_server.IsWebSocket())
	{
		g_strReply.append(buf->base,nread);
		cout<<"\t [Server] socket already is websocket, Frame total size:"<<g_strReply.length()<<endl;
		cout<<"\t [Server] frame raw:"<<g_strReply<<endl;

		char sFrame[FRAME_SIZE];
		UBWsParserFrameType wsft;
		int iFrameLen=g_ws_server.WSGetFrame(&wsft,(uint8_t*)sFrame,FRAME_SIZE,(uint8_t*)g_strReply.data(),g_strReply.length());
		if(iFrameLen>0)
		{
			g_strReply.assign(sFrame,iFrameLen);
			cout<<"\t [Server] frame content:"<<g_strReply<<endl;
		}
		else
		{
			cout<<"\t [Server] Frame decode failed:"<<iFrameLen<<endl;
		}
		return;
	}

	int ret=g_ws_server.Readed(buf->base, nread);
	if(ret<0)
	{
		if (nread != UV_EOF)cout<<"\t [Server] Read WebSocket error :"<<uv_err_name(nread)<<endl;
		uv_close((uv_handle_t*) client, ServerTest_Closed);
		uv_stop(&loop_server);
		return;
	}

	cout<<"\t [Server] WebSocket parse:"<<ret<<",IsWebSocket?"<<g_ws_server.IsWebSocket()<<",IsHeader?"<<g_ws_server.IsHeaderComplete()<<endl;
	if(g_ws_server.IsWebSocket() && g_ws_server.IsHeaderComplete())
	{
		g_strReply=g_ws_server.m_sBody;

		write_req_t *req = new write_req_t;
		char *pReply=new char[g_iReplyFrameLen];
		memcpy(pReply,g_sReplyFrame,g_iReplyFrameLen);
	    req->buf = uv_buf_init(pReply,g_iReplyFrameLen);
   	 	uv_write((uv_write_t*) req, client, &req->buf, 1, ServerTest_Writed);
	}
}

void ServerTest_ReadBuff(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) 
{
	cout<<"\t [Server] New read buffer"<<endl;
	buf->base = new char[suggested_size];
	buf->len = suggested_size;
}


void ServerTest_NewConnection(uv_stream_t *server, int status) 
{
	cout<<"\t [Server] Accept new connection"<<endl;
	if (status < 0) 
	{
		cout<<"\t [Server] New connection error :"<<uv_strerror(status)<<endl;
		return;
	}

	uv_tcp_t *client = new uv_tcp_t;
	uv_tcp_init(&loop_server, client);
	if (uv_accept(server, (uv_stream_t*) client) == 0)
	{
		int ret=uv_read_start((uv_stream_t*) client, ServerTest_ReadBuff, ServerTest_Read);
		cout<<"\t [Server] New connection begin read : "<<ret<<endl;
	}
	else 
	{
		cout<<"\t [Server] New connection accept failed"<<endl;
		uv_close((uv_handle_t*) client, ServerTest_Closed);
	}
}

void ThreadServerTest(void *arg)
{
	g_ws_server.Init(HTTP_REQUEST);

   uv_loop_init(&loop_server);

	uv_tcp_t server;
	uv_tcp_init(&loop_server, &server);

	struct sockaddr_in addr;
	uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);

	uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
	int ret = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, ServerTest_NewConnection);
	if (ret) 
	{
		cout<<"[Client] Listen error :"<<uv_strerror(ret)<<endl;
		return;
	}
	uv_run(&loop_server, UV_RUN_DEFAULT);
}

void ClientTest_Closed(uv_handle_t* handle) 
{
//	delete (uv_tcp_t*)handle;
	uv_stop(&loop_client);
}

bool g_bSendFinished=false;
void ClientTest_Writed(uv_write_t* req, int status)
{
	cout<<"[Client] write status:"<<status<<endl;
	if (status < 0)
	{
		cout<<"[Client] Write failed"<<endl;
	}

	if(g_bSendFinished)
	{
		sleep(1);
		cout<<"[Client] close socket"<<endl;
		uv_close((uv_handle_t*) req->handle, ClientTest_Closed);
		free_write_req(req);
		return;
	}

	sleep(1);

	g_bSendFinished=true;

	char *pRequest=new char[g_iRequestFrameLen];
	memcpy(pRequest,g_sRequestFrame,g_iRequestFrameLen);

	write_req_t *reqW = new write_req_t;
	reqW->buf = uv_buf_init(pRequest,g_iRequestFrameLen);
	uv_write((uv_write_t*)reqW, (uv_stream_t*)(req->handle),&reqW->buf, 1, ClientTest_Writed);
}

void ClientTest_Connected(uv_connect_t* req, int status)
{
	cout<<"[Client] connect success, begin send..."<<endl;

	char *pRequest=new char[g_sHttpRequest.length()];
	memcpy(pRequest,g_sHttpRequest.data(),g_sHttpRequest.length());

	write_req_t *reqW = new write_req_t;
	reqW->buf = uv_buf_init(pRequest,g_sHttpRequest.length());
	uv_write((uv_write_t*)reqW, (uv_stream_t*)(req->handle),&reqW->buf, 1, ClientTest_Writed);
}


void ThreadClientTest(void *arg)
{
	g_ws_client.Init(HTTP_RESPONSE);

   uv_loop_init(&loop_client);

	uv_tcp_t client;
	uv_tcp_init(&loop_client, &client);

	struct sockaddr_in addr;
	uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);

	uv_connect_t *req=new uv_connect_t;
	int ret=uv_tcp_connect(req,&client,(sockaddr*)&addr,ClientTest_Connected);
	if(ret)
	{
		cout<<"[Client] Connect error :"<<uv_strerror(ret)<<endl;
		return;
	}

	uv_run(&loop_client, UV_RUN_DEFAULT);
}


void ServerClientTest()
{
	InitWetSocket();

	uv_thread_t server,client;
	uv_thread_create(&server, ThreadServerTest, NULL);
	uv_thread_create(&client, ThreadClientTest, NULL);

	uv_thread_join(&client);
	uv_thread_join(&server);

	cout<<"UV test finished"<<endl;
}

int main()
{
//	EmptyLoop();

//	ThreadTest();

	ServerClientTest();

	return 0;
}


