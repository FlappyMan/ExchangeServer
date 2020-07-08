#include "ThreadTCPOper.h"
#include "DispatchManager.h"
#include "BaseRequestOper.h"
#include "ThreadDBOper.h"
uv_loop_t g_uvLoop;
uv_timer_t g_uvTimer;

void CThreadTCPOper::cbTimer(uv_timer_t *handle)
{
	while (0 != CThreadDBOper::m_qDBResult.size())
	{
		CDBResult *result = CThreadDBOper::m_qDBResult.get();
		if(RET_FAILED == CDispatchManager::DispatchOnSend(result->m_cProt, (uv_tcp_t *)result->m_cOper->m_client))
		{
			DEBUG_ERROR("cache error at CDispatchManager::DispatchOnSend");
		}

		if (NULL != result->m_cOper)
		{
			delete result->m_cOper;
			result->m_cOper = NULL;
		}
		if (NULL != result)
		{
			delete result;
			result = NULL;
		}
	}
}

void CThreadTCPOper::cbReadBuff(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
#ifdef PRINT_LOG
	DEBUG_INFO("[CThreadTCPOper->cbNewConnection->cbReadBuff] New read buffer");
#endif
	buf->base = g_cache_tcp_read.Get(SIZE_BUFFER_8k);
	buf->len = SIZE_BUFFER_8k;
}

void CThreadTCPOper::cbClosed(uv_handle_t *handle)
{
	mapBaseSession::iterator itr = CDispatchManager::g_mapMapping.find((client_sokt_t *)handle);
	if (itr != CDispatchManager::g_mapMapping.end())
	{
		if (NULL != itr->second)
		{
			delete itr->second;
			itr->second = NULL;
		}
		CDispatchManager::g_mapMapping.erase(itr);
	}
	delete (client_sokt_t *)handle;
}

void CThreadTCPOper::cbWrited(uv_write_t *req, int status)
{
#ifdef PRINT_LOG
	DEBUG_INFO("[CThreadTCPOper::cbWrited]");
#endif
	if (status < 0)
	{
#ifdef PRINT_LOG
		DEBUG_ERROR("[CThreadTCPOper->cbWrited] Write failed");
#endif
	}
	g_cache_write_req.Free((UVWriteReq *)req);
}

void CThreadTCPOper::cbRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
	DEBUG_INFO("[CThreadTCPOper->cbNewConnection->cbRead] Server begin read");
	if (nread < 0)
	{
#ifdef PRINT_LOG
		DEBUG_INFO("[CThreadTCPOper->cbNewConnection->cbRead] Read error ");// << uv_err_name(nread) << endl;
#endif
		uv_close((uv_handle_t *)client, cbClosed);
	}
	else if (nread == 0)
	{
#ifdef PRINT_LOG
		DEBUG_INFO("[CThreadTCPOper->cbNewConnection->cbRead] read noting");
#endif
		uv_close((uv_handle_t *)client, cbClosed);
	}
	else
	{
#ifdef PRINT_LOG
		DEBUG_INFO("[CThreadTCPOper->cbNewConnection->cbRead] readed");
#endif
		int ret = CDispatchManager::DispatchClient((client_sokt_t *)client, buf->base, nread);
		if (RET_FAILED == ret)
		{
			uv_close((uv_handle_t *)client, cbClosed);
		}
		else if (RET_SUCCESS == ret)
		{
#ifdef PRINT_LOG
			DEBUG_INFO("[CDispatchManager::DispatchClient] socket recv RET_SUCCESS");
#endif
		}
		else
		{
#ifdef PRINT_LOG
			DEBUG_INFO("[CDispatchManager::DispatchClient] socket recv RET_HOLD");
#endif
		}
	}
	g_cache_tcp_read.Free(buf->base,buf->len);
}

void CThreadTCPOper::cbNewConnection(uv_stream_t *server, int status)
{
#ifdef PRINT_LOG
	DEBUG_INFO("[CThreadTCPOper->cbNewConnection] Accept new connection");
#endif
	if (status < 0)
	{
#ifdef PRINT_LOG
		DEBUG_ERROR("[CThreadTCPOper->cbNewConnection] New connection error"); // uv_strerror(status)
#endif
	}
	else
	{
		client_sokt_t *client = new client_sokt_t;
		client->ulPkgIndex = CDispatchManager::g_ulTCPSockIndex++;
		uv_tcp_init(&g_uvLoop, (uv_tcp_t *)client);
		if (uv_accept(server, (uv_stream_t *)client) == 0)
		{
			CBaseSession *session = new CBaseSession;
			session->m_client = client;
        	CDispatchManager::g_mapMapping.insert(pair<client_sokt_t *, CBaseSession *>(client, session));

			int ret = uv_read_start((uv_stream_t *)client, cbReadBuff, cbRead);
#ifdef PRINT_LOG
			DEBUG_INFO("[CThreadTCPOper->cbNewConnection] New connection begin read");
#endif
		}
		else
		{
#ifdef PRINT_LOG
			DEBUG_INFO("[CThreadTCPOper->cbNewConnection] New connection accept failed");
#endif
			uv_close((uv_handle_t *)client, cbClosed);
		}
	}
}

void CThreadTCPOper::ThreadTCPOper(void *arg)
{
	DEBUG_INFO("[CThreadTCPOper Init]");
	g_cache_write_req.Init(100,0,0);
	g_cache_tcp_read.Init(0,100,0);
	uv_loop_init(&g_uvLoop);

	uv_tcp_t server;
	uv_tcp_init(&g_uvLoop, &server);

	struct sockaddr_in addr;
	uv_ip4_addr(Config::g_sTcpIP.c_str(), Config::g_usTcpPort, &addr);

	uv_tcp_bind(&server, (const struct sockaddr *)&addr, 0);
	int ret = uv_listen((uv_stream_t *)&server, 1000, cbNewConnection);

	uv_timer_init(&g_uvLoop, &g_uvTimer);
	uv_timer_start(&g_uvTimer, cbTimer, 10, 1);
	uv_run(&g_uvLoop, UV_RUN_DEFAULT);
}
