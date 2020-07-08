#include "ThreadHttpOper.h"
#include "./Manager/DispatchManager.h"
#include "ThreadDBOper.h"
#include <vector>
uv_loop_t g_uvHttpLoop;
uv_timer_t g_uvHttpTimer;

void CThreadHttpOper::cbTimer(uv_timer_t *handle)
{
	std::vector<CJsonObjectBase *> vecArray;
	for (mapHttpSession::iterator itr = CDispatchManager::g_mapHttpMapping.begin(); itr != CDispatchManager::g_mapHttpMapping.end();)
	{
		if (true == itr->second->isRecvFinish())
		{
			CJsonObjectBase *pRespOper = itr->second->m_pObject;
			vecArray.push_back(pRespOper);
			itr->second->m_pObject = NULL;
			delete itr->second;
			CDispatchManager::g_mapHttpMapping.erase(itr++);
		}
		else
		{
			itr++;
		}
	}
	if (vecArray.size() != 0)
	{
		CThreadDBOper::m_qDBJsonObjOper.put(vecArray);
	}

	std::vector<CJsonObjectBase *> vecResp;
	int iResultSize = CThreadDBOper::m_qDBResultJsonOper.get(vecResp);
	for (int i = 0; i < iResultSize; i++)
	{
		CJsonObjectBase *pResult = vecResp[i];
		if(RET_FAILED == CDispatchManager::DispatchHttpResponse(pResult->getResponse(), (uv_tcp_t *)pResult->m_client))
		{
			DEBUG_ERROR("resp error at CDispatchManager::DispatchHttpResponse");
		}
		if (NULL != pResult)
		{
			delete pResult;
			pResult = NULL;
		}
	}
}

void CThreadHttpOper::cbReadBuff(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
#ifdef PRINT_LOG
	DEBUG_INFO("[CThreadHttpOper->cbNewConnection->cbReadBuff] Malloc Buffer");
#endif
	buf->base = g_cache_http_read.Get(SIZE_BUFFER_8k);
	buf->len = SIZE_BUFFER_8k;
}

void CThreadHttpOper::cbClosed(uv_handle_t *handle)
{
	mapHttpSession::iterator itr = CDispatchManager::g_mapHttpMapping.find((client_sokt_t *)handle);
	if (itr != CDispatchManager::g_mapHttpMapping.end())
	{
		if (NULL != itr->second->m_pObject)
		{
			delete itr->second->m_pObject;
			itr->second->m_pObject = NULL;
		}
		
		if (NULL != itr->second)
		{
			delete itr->second;
			itr->second = NULL;
		}
		CDispatchManager::g_mapHttpMapping.erase(itr);
	}
	delete (client_sokt_t *)handle; // conn
}

void CThreadHttpOper::cbWrited(uv_write_t *req, int status)
{
#ifdef PRINT_LOG
	DEBUG_INFO("[CThreadHttpOper->cbWrited]");
#endif
	if (status < 0)
	{
#ifdef PRINT_LOG
		DEBUG_ERROR("[CThreadHttpOper->cbWrited] Write failed");
#endif
	}
	uv_close((uv_handle_t *)req->handle, cbClosed);
	g_cache_http_req.Free((UVWriteReq *)req);
}

void CThreadHttpOper::cbRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
#ifdef PRINT_LOG
	DEBUG_INFO("[CThreadHttpOper->cbNewConnection->cbRead] Server begin read");
#endif
	if (nread < 0)
	{
#ifdef PRINT_LOG
		DEBUG_INFO("[CThreadHttpOper->cbNewConnection->cbRead] socket disconnect");
#endif
		uv_close((uv_handle_t *)client, cbClosed);
	}
	else if (nread == 0)
	{
#ifdef PRINT_LOG
		DEBUG_INFO("[CThreadHttpOper->cbNewConnection->cbRead] read noting");
#endif
		uv_close((uv_handle_t *)client, cbClosed);
	}
	else
	{
		int iOperRet = CDispatchManager::DispatchHttpClient((client_sokt_t *)client, buf->base, nread);
		if (RET_SUCCESS == iOperRet)
		{
#ifdef PRINT_LOG
			DEBUG_INFO("[CDispatchManager::DispatchHttpClient] socket recv RET_SUCCESS");
#endif
		}
		else if (RET_FAILED == iOperRet)
		{
#ifdef PRINT_LOG
			DEBUG_ERROR("[CDispatchManager::DispatchHttpClient] socket recv RET_FAILED");
#endif
			uv_close((uv_handle_t *)client, cbClosed);
		}
		else
		{
#ifdef PRINT_LOG
			DEBUG_INFO("[CDispatchManager::DispatchHttpClient] socket recv RET_HOLD");
#endif
		}
	}
	g_cache_http_read.Free(buf->base,buf->len);
}

void CThreadHttpOper::cbNewConnection(uv_stream_t *server, int status)
{
#ifdef PRINT_LOG
	DEBUG_INFO("[CThreadHttpOper->cbNewConnection] Accept new connection");
#endif
	if (status < 0)
	{
#ifdef PRINT_LOG
		DEBUG_INFO("[CThreadHttpOper->cbNewConnection] New connection error"); // uv_strerror(status)
#endif
	}
	else
	{
		client_sokt_t *client = new client_sokt_t;
		client->ulPkgIndex = CDispatchManager::g_ulHTTPPkgIndex++;
		uv_tcp_init(&g_uvHttpLoop, (uv_tcp_t *)client);
		if (uv_accept(server, (uv_stream_t *)client) == 0)
		{
			CHttpSession *session = new CHttpSession;
        	CDispatchManager::g_mapHttpMapping.insert(pair<client_sokt_t *, CHttpSession *>(client, session));
			int ret = uv_read_start((uv_stream_t *)client, cbReadBuff, cbRead);
#ifdef PRINT_LOG
			DEBUG_INFO("[CThreadHttpOper->cbNewConnection] New connection begin read ");
#endif
		}
		else
		{
#ifdef PRINT_LOG
			DEBUG_ERROR("[CThreadHttpOper->cbNewConnection] New connection accept failed");
#endif
			uv_close((uv_handle_t *)client, cbClosed);
		}
	}
}

void CThreadHttpOper::ThreadHttpOper(void *arg)
{
	DEBUG_INFO("[CThreadHttpOper Init]");
	g_cache_http_req.Init(100,0,0);
	g_cache_http_read.Init(0,100,0);
	uv_loop_init(&g_uvHttpLoop);
	uv_tcp_t server;
	uv_tcp_init(&g_uvHttpLoop, &server);

	struct sockaddr_in addr;
	uv_ip4_addr(Config::g_sHttpIP.c_str(), Config::g_uiHttpPort, &addr);

	uv_tcp_bind(&server, (const struct sockaddr *)&addr, 0);
	int ret = uv_listen((uv_stream_t *)&server, 1000, cbNewConnection);
	uv_timer_init(&g_uvHttpLoop, &g_uvHttpTimer);
	uv_timer_start(&g_uvHttpTimer, cbTimer, 10, 1);
	uv_run(&g_uvHttpLoop, UV_RUN_DEFAULT);
}
