#include "./Manager/DispatchManager.h"
#include "ThreadOrder.h"
#include "ThreadUserCenter.h"

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

uv_loop_t g_uvOrderLoop;
uv_timer_t g_uvOrderTimer;

#define THIS_LOOP g_uvOrderLoop
#define THIS_TIMER g_uvOrderTimer

uv_mutex_t CThreadOrder::g_mutex;

map<client_pkg_t *, uv_thread_t *> CThreadOrder::g_mapThread;
map<client_pkg_t *,CServerInfo> CThreadOrder::g_mapConnectInfo;
UBBlockQueue<CBaseSession> CThreadOrder::g_NodeSession;

CThreadOrder::CThreadOrder()
{
	uv_mutex_init(&CThreadOrder::g_mutex);
}
CThreadOrder::~CThreadOrder()
{
	uv_mutex_destroy(&CThreadOrder::g_mutex);
}

void CThreadOrder::cbTimer(uv_timer_t *handle)
{
	vector<CBaseSession*> vecNodes;
	CThreadOrder::g_NodeSession.get(vecNodes);
	CThreadUserCenter::g_qOrderToUCTrans.put(vecNodes);

	vector<CBaseSession*> tempVec;
	int iRequestSize = CThreadUserCenter::g_qOrderToUCRespQueue.get(tempVec);
	for (int i = 0; i < iRequestSize; i++)
	{
		CBaseSession *session = tempVec[i];
		UVWriteReq *write = session->rePackPkg();
		uv_write((uv_write_t *)write, (uv_stream_t *)session->getClientFd(), &write->buf, 1, CThreadOrder::cbWrited);
		delete session;
	}

}

void CThreadOrder::cbReadBuff(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
#ifdef PRINT_LOG
	cout << "[CThreadOrder->cbNewConnection->cbReadBuff] Malloc Buffer" << endl;
#endif
	buf->base = g_cache_orderthread_read.Get(SIZE_BUFFER_8k);
	buf->len = SIZE_BUFFER_8k;
}

void CThreadOrder::cbClosed(uv_handle_t *handle)
{
	for (mapBaseSession::iterator itr = CDispatchManager::g_mapTCPSession.begin(); itr != CDispatchManager::g_mapTCPSession.end();)
	{
		delete itr->second;
		itr->second = NULL;
		CDispatchManager::g_mapTCPSession.erase(itr++);
	}
	// 重新链接此socket
	uv_thread_t *p_connThread = new uv_thread_t;
	CThreadOrder::g_mapThread.insert(pair<client_pkg_t *, uv_thread_t *>((client_pkg_t *)handle, p_connThread));
	uv_thread_create(p_connThread, CThreadOrder::reConnect, handle);
}

void CThreadOrder::cbWrited(uv_write_t *req, int status)
{
	g_cache_orderthread_resp.Free((UVWriteReq *)req);
}

void CThreadOrder::cbRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
#ifdef PRINT_LOG
	cout << "[CThreadOrder->cbNewConnection->cbRead] Server begin read" << endl;
#endif
	if (nread < 0)
	{
#ifdef PRINT_LOG
		cout << "[CThreadOrder->cbNewConnection->cbRead] socket disconnect" << endl;
#endif
		uv_close((uv_handle_t *)client, cbClosed);
	}
	else if (nread == 0)
	{
#ifdef PRINT_LOG
		cout << "[CThreadOrder->cbNewConnection->cbRead] read noting" << endl;
#endif
		uv_close((uv_handle_t *)client, cbClosed);
	}
	else
	{
		int iOperRet = CDispatchManager::DispatchClient(CDispatchManager::g_mapTCPSession, (client_pkg_t *)client, buf->base, nread, true);
		if (RET_SUCCESS == iOperRet)
		{
			mapBaseSession::iterator itr = CDispatchManager::g_mapTCPSession.find((client_pkg_t *)client);
			assert(itr != CDispatchManager::g_mapTCPSession.end());
			CDispatchManager::g_mapTCPSession.erase(itr++);
			
#ifdef PRINT_LOG
			cout << "[CDispatchManager::DispatchClient] socket recv RET_SUCCESS session size = " << CDispatchManager::g_mapTCPSession.size() << endl;
#endif
		}
		else if (RET_FAILED == iOperRet)
		{
#ifdef PRINT_LOG
			cout << "[CDispatchManager::DispatchClient] socket recv RET_FAILED" << endl;
#endif
			uv_close((uv_handle_t *)client, cbClosed);
		}
		else
		{
#ifdef PRINT_LOG
			cout << "[CDispatchManager::DispatchClient] socket recv RET_HOLD" << endl;
#endif
		}
	}
#ifdef PRINT_LOG
	cout << "[CThreadOrder::cbRead] delete[] buf->base" << endl;
#endif
	g_cache_orderthread_read.Free(buf->base,buf->len);
}

void CThreadOrder::reConnect(void *arg)
{
	//用户中心服务器连接不存在，backtrade每隔iSleepTime秒后重连服务器
	client_pkg_t *client = (client_pkg_t *)arg;

	map<client_pkg_t *, uv_thread_t *>::iterator itrThread = CThreadOrder::g_mapThread.find(client);
	assert(itrThread != CThreadOrder::g_mapThread.end());
	if (NULL != itrThread->second)
	{
		delete itrThread->second;
		itrThread->second = NULL;
	}
	CThreadOrder::g_mapThread.erase(itrThread);

	int iSleepTime = 5;

	sleep(iSleepTime);
	struct sockaddr_in addr;
	CServerInfo server_info;
	client_pkg_t *server = new client_pkg_t;

	uv_mutex_lock(&CThreadOrder::g_mutex);
	server->ulPkgIndex = CDispatchManager::g_ulOrderIndex++;
	uv_mutex_unlock(&CThreadOrder::g_mutex);

	server_info.conn = new uv_connect_t;
	map<client_pkg_t *,CServerInfo>::iterator itr = CThreadOrder::g_mapConnectInfo.find(client);
	assert(itr != CThreadOrder::g_mapConnectInfo.end());
	cout << "[CThreadOrder::cbConnect] after " << iSleepTime << " seconds reconnect sock = " << itr->first->ulPkgIndex << " server port = " << itr->second.info.m_usConnectPort << endl;

	server_info.info = itr->second.info;

	uv_tcp_init(&THIS_LOOP, (uv_tcp_t *)server);
	uv_ip4_addr(server_info.info.m_sConnectIP.c_str(), server_info.info.m_usConnectPort, &addr);
	uv_tcp_connect(server_info.conn, server, (const sockaddr *)&addr, cbConnect);

	if (NULL != itr->second.conn)
	{
		delete itr->second.conn;
		itr->second.conn = NULL;
	}

	if (NULL != itr->first)
	{
		delete itr->first;
	}

	CThreadOrder::g_mapConnectInfo.erase(itr);
	CThreadOrder::g_mapConnectInfo.insert(pair<client_pkg_t *, CServerInfo>(server, server_info));
}

void CThreadOrder::cbConnect(uv_connect_t *req, int status)
{
	if (status == 0)
	{
		client_pkg_t *client = (client_pkg_t *)req->handle;
		client->ulPkgIndex = CDispatchManager::g_ulOrderIndex++;
		int ret = uv_read_start((uv_stream_t *)client, cbReadBuff, cbRead);
		cout << "[CThreadOrder::cbConnect] connect FrontTrade success index = " << client->ulPkgIndex << " ret code = " << ret << endl;
		rsaLoginServer(client);
	}
	else
	{
		uv_thread_t *p_connThread = new uv_thread_t;
		CThreadOrder::g_mapThread.insert(pair<client_pkg_t *, uv_thread_t *>((client_pkg_t *)req->handle, p_connThread));
		uv_thread_create(p_connThread, CThreadOrder::reConnect, req->handle);
	}
}
void CThreadOrder::rsaLoginServer(client_pkg_t *client)
{
	Json::Value sJson;
	sJson["mid"] = Config::uiMarketID;
	sJson["mn"] = Config::sMarket.c_str();

	UPLogin *lg = new UPLogin;
	lg->ClientRequest(Config::sRSAFrontTradePriKey,sJson);

	const int BUFFSIZE = ProtoPackSize<UPLogin>(*lg);
	UVWriteReq *write = g_cache_orderthread_login.Get(BUFFSIZE);
	assert(write != NULL);
	assert(write->buf.base != NULL);
	write->pkg = lg;
	write->buf.len = ProtoPack<UPLogin>(write->buf.base, BUFFSIZE, *lg);

	assert(write->buf.len > 0);
	uv_write((uv_write_t *)write, (uv_stream_t *)client, &write->buf, 1, CThreadOrder::cbLogin);
}
void CThreadOrder::cbLogin(uv_write_t *req, int status)
{
	g_cache_orderthread_login.Free((UVWriteReq *)req);
}
void CThreadOrder::ThreadOrder(void *arg)
{
	cout << "[CThreadOrder Init]" << endl;
	uv_loop_init(&THIS_LOOP);
	g_cache_orderthread_resp.Init(100,0,0);
	g_cache_orderthread_login.Init(100, 0, 0);
	g_cache_orderthread_read.Init(0,100,0);
	for (int iServerCount = 0; iServerCount < Config::g_vecConnectServers.size(); iServerCount++)
	{
		CServerInfo server_info;
		struct sockaddr_in addr;
		client_pkg_t *server = new client_pkg_t;
		server_info.conn = new uv_connect_t;
		server_info.info = Config::g_vecConnectServers[iServerCount];

		uv_mutex_lock(&CThreadOrder::g_mutex);
		server->ulPkgIndex = CDispatchManager::g_ulOrderIndex++;
		uv_mutex_unlock(&CThreadOrder::g_mutex);

		uv_tcp_init(&THIS_LOOP, (uv_tcp_t *)server);
		uv_ip4_addr(server_info.info.m_sConnectIP.c_str(), server_info.info.m_usConnectPort, &addr);
		int ret = uv_tcp_connect(server_info.conn, server, (const sockaddr *)&addr, cbConnect);

		cout << "[CThreadOrder Client connect code]:" << uv_strerror(ret) << endl;
		cout << "[CThreadOrder Client connect] IP = " << server_info.info.m_sConnectIP << endl;
		cout << "[CThreadOrder Client connect] PORT = " << server_info.info.m_usConnectPort << endl;
		cout << "[CThreadOrder Client connect] connect index = " << server->ulPkgIndex << endl;
		CThreadOrder::g_mapConnectInfo.insert(pair<client_pkg_t *, CServerInfo>(server, server_info));
		cout << "[CThreadOrder] Connect code :" << uv_strerror(ret) << endl;
	}

	uv_timer_init(&THIS_LOOP, &THIS_TIMER);
	uv_timer_start(&THIS_TIMER, cbTimer, 10, 1);
	uv_run(&THIS_LOOP, UV_RUN_DEFAULT);
}
