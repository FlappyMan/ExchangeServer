#include "./Manager/DispatchManager.h"
#include "ThreadUserCenter.h"
#include "ThreadMatch.h"
#include "ThreadDB.h"

uv_loop_t g_uvUserCenterLoop;
uv_timer_t g_uvUserCenterTimer;
string CThreadUserCenter::g_sConfigFileUpdateTime;
UBBlockQueue<CBaseSession> CThreadUserCenter::g_qOrderToUCRespQueue; // 用户中心回复给UC线程的包处理后返回order线程直接发送（返回session）
UBBlockQueue<CBaseSession> CThreadUserCenter::g_qRecvUCRespQueue;	 // UC线程受到用户中心的数据包（接受到的完整数据包）
map<string, CBaseSession *> CThreadUserCenter::g_mapAuthSession;	 // 用于记录通过用户中心验证的请求数据
UBBlockQueue<CBaseSession> CThreadUserCenter::g_qOrderToUCTrans;	 //orthread传递给UCThread的数据节点

uv_stream_t *CThreadUserCenter::fdSocket = NULL; // 和用户中心连接socket
client_pkg_t *CThreadUserCenter::g_server;
uv_connect_t *CThreadUserCenter::g_connect;
void CThreadUserCenter::PraseCancelAndSelect(vector<CBaseSession *> &vecCancelAndSelect)
{
	vector<CBaseModel *> vecTrans;
	for (int i = 0; i < vecCancelAndSelect.size(); i++)
	{
		CBaseSession *pSession = vecCancelAndSelect[i];
		if (pSession->m_usType == UPCanceltrade::CMD)
		{
			// 撤单从CBasession转换成对应的CBaseodel
			CCancelTradeModel *pBaseModel = new CCancelTradeModel;
			setCancelTradeModel(pBaseModel, pSession->getCancelTradeModel());
			vecTrans.push_back(pBaseModel);
		}
		else if (pSession->m_usType == UPCanceltradebatch::CMD)
		{
			// 批量撤单从CBasession转换成对应的CBaseodel
			CCancelTradeBatchModel *pBaseModel = new CCancelTradeBatchModel;
			CCancelTradeBatchModel cancelBatModel = pSession->getCancelTradeBatchModel();
			pBaseModel->modelType = OPER_TYPE_CANCELTRADEE_BATCH;
			pBaseModel->uid = cancelBatModel.uid;
			pBaseModel->canceltradebatchid = cancelBatModel.canceltradebatchid;
			pBaseModel->orders = cancelBatModel.orders;
			vecTrans.push_back(pBaseModel);
		}
		else
		{
			// 查单数据
			CGetOrderInfoModel *pBaseModel = new CGetOrderInfoModel;
			CGetOrderInfoModel model = pSession->getGetOrderInfoModel();
			pBaseModel->modelType = OPER_TYPE_SELECT;
			pBaseModel->token = model.token;
			pBaseModel->orderid = model.orderid;
			pBaseModel->marketid = model.marketid;
			pBaseModel->uid = model.uid;
			vecTrans.push_back(pBaseModel);
		}
	}
	// 取消的订单和查单都转移到撮合线程
	CThreadMatch::m_qUCToMTQueue.put(vecTrans);
}
vector<CBaseSession *> CThreadUserCenter::TransUptradeToUC()
{
	/*每天六点重新加载配置文件*/
	if (CTool::getHour() == "06")
	{
		if (CTool::getMin() == "00")
		{
			loadRuntimeConfig();
		}
	}

	vector<CBaseSession *> vecTransSend, vecRet;
	// 获取order线程生成的发送数据包，直接发送
	int iRequestSize = CThreadUserCenter::g_qOrderToUCTrans.get(vecTransSend);
	for (int i = 0; i < iRequestSize; i++)
	{
		/*只有下单操作才会转发用户中心，撤单直接转发撮合线程*/
		CBaseSession *session = vecTransSend[i];
		if (session->m_usType == UPUptrade::CMD || session->m_usType == UPUptradebatch::CMD)
		{
			/*
			* 下单/批量撤单
			* 重新打包数据以后转发给用户中心
			*/
			UVWriteReq *write = NULL;
			if (session->m_usType == UPUptrade::CMD)
				write = session->rePackPkg(session->getTradeModel());
			else
				write = session->rePackPkg(session->getTradeBatchModel());
			uv_write((uv_write_t *)write, CThreadUserCenter::fdSocket, &write->buf, 1, CThreadUserCenter::cbWrited);
		}
		else
		{
			// 撤单,批量撤单,查询订单业务
			vecRet.push_back(session);
		}
		// 绑定所有原始请求的session到map中
		CThreadUserCenter::g_mapAuthSession.insert(pair<string, CBaseSession *>(session->getToken(), session)); 
	}
	return vecRet;
}
vector<CBaseSession *> CThreadUserCenter::getAuthPass(vector<CBaseSession *> &vecUCResp)
{
	vector<CBaseSession *> vecAuthResop; // 用户中心校验通过的数据包
	for (int i = 0; i < vecUCResp.size(); i++)
	{
		// 用户中心的数据包直接转发给order线程用于发送
		CBaseSession *session = vecUCResp[i];
		if (session->getStatus() == 1)
		{
			DEBUG_INFO("用户中心返回session成功");
			vecAuthResop.push_back(session);
			vecUCResp.erase(vecUCResp.begin() + i);
			i--;
		}
		else
		{
			DEBUG_ERROR("用户中心返回session失败");
		}
	}
	return vecAuthResop;
}
vector<CBaseSession *> CThreadUserCenter::getUCReturnPkg()
{
	vector<CBaseSession *> vecRecvResop; // 用户中心的返回数据包
	CThreadUserCenter::g_qRecvUCRespQueue.get(vecRecvResop);
	return vecRecvResop;
}
void CThreadUserCenter::respToClient(vector<CBaseSession *> &vec,bool bFreeSession)
{
	/*
	* 入口参数说明：
	* 1.operMatchResp 函数传进来的是client请求时的session重组包
	* 2.getOriginSession 函数调用后传入的参数是用户中心的返回结果包
	*/
	vector<CBaseSession *> vecProtBase;
	int iVecSize = vec.size();
	for (int i = 0; i < iVecSize; i++)
	{
		CBaseSession *session = vec[i];
		map<string, CBaseSession *>::iterator itr = CThreadUserCenter::g_mapAuthSession.find(session->getToken());
		if (itr != CThreadUserCenter::g_mapAuthSession.end())
		{
			// 这里不删除itr->second是因为成功的session还要继续传递给下个线程
			itr->second->setRespData(session->getRespData());
			itr->second->setStatus(session->getStatus());
			CThreadUserCenter::g_mapAuthSession.erase(itr);
			vecProtBase.push_back(itr->second);
		}
		else
		{
			DEBUG_ERROR("UserCenter Resp Pkg Not Found");
		}
		if (bFreeSession == true)
		{
			/*
			* 当代码的入口是1时不删除session
			* 因为此时的session是请求session，应该交给orderthread删除回收
			* 当代码入口是2时，删除session，这个session是用户中心返回的
			*/
			delete session;
			session = NULL;
		}
	}
	vec.clear();
	if (vecProtBase.size() != 0)
	{
		CThreadUserCenter::g_qOrderToUCRespQueue.put(vecProtBase);
	}
}
void CThreadUserCenter::setUpTradeModel(CTradeModel *pBaseModel, CTradeModel model, CBaseSession *session)
{
	pBaseModel->modelType = OPER_TYPE_UPTRADE;
	pBaseModel->orderid = model.orderid;
	pBaseModel->num = model.num;
	pBaseModel->uid = model.uid;
	pBaseModel->paypassword = model.paypassword;
	pBaseModel->marketid = model.marketid;
	pBaseModel->price = model.price;
	pBaseModel->totalNum = model.totalNum;
	pBaseModel->type = model.type;
	pBaseModel->tradedNum = model.tradedNum;
	pBaseModel->time = model.time;
	CResponseModel resp = session->getResponseModel();
	Json::Value root;
	UBJsonUnpack(root, resp.data);
	pBaseModel->sysUid = root["sysuid"].asUInt();
	pBaseModel->invite_uid_1 = root["recommd1"].asUInt();
	pBaseModel->invite_uid_2 = root["recommd2"].asUInt();
	pBaseModel->invite_uid_3 = root["recommd3"].asUInt();
	pBaseModel->discount = root["discount"].asString();
}
void CThreadUserCenter::setCancelTradeModel(CCancelTradeModel *pBaseModel, CCancelTradeModel model)
{
	pBaseModel->modelType = OPER_TYPE_CANCELTRADE;
	pBaseModel->canceltradeid = model.canceltradeid;
	pBaseModel->marketid = model.marketid;
	pBaseModel->uid = model.uid;
	pBaseModel->orderid = model.orderid;
	pBaseModel->price = model.price;
	pBaseModel->num = model.num;
	pBaseModel->type = model.type;
	pBaseModel->token = model.token;
}
void CThreadUserCenter::getOriginSession(vector<CBaseSession *> &vecUCAuthResp, vector<CBaseModel *> &vecRet)
{
	for (size_t i = 0; i < vecUCAuthResp.size(); i++)
	{
		// 这里不需要用户中心返回的session,根据返回session的token提取的是客户端请求的session内容，用户后续业务处理
		CBaseSession *session = vecUCAuthResp[i]; // 用户中心返回的验证通过的session
		map<string, CBaseSession *>::iterator itr = CThreadUserCenter::g_mapAuthSession.find(session->getToken());
		if (itr != CThreadUserCenter::g_mapAuthSession.end())
		{
			// 回复包找到对应的请求token
			if (itr->second->m_usType == UPUptrade::CMD)
			{
				CTradeModel *pBaseModel = new CTradeModel;
				setUpTradeModel(pBaseModel, itr->second->getTradeModel(), session);
				vecRet.push_back((CBaseModel *)pBaseModel);
				CDispatchManager::recordData(vecRet);
			}
			else if (itr->second->m_usType == UPUptradebatch::CMD)
			{
				// 批量下单，拆分成若干个下单操作
				CTradeBatchModel batchModel = itr->second->getTradeBatchModel();
				CResponseModel resp = session->getResponseModel();
				Json::Value root;
				UBJsonUnpack(root, resp.data);
				batchModel.sysUid = root["sysuid"].asUInt();
				batchModel.invite_uid_1 = root["recommd1"].asUInt();
				batchModel.invite_uid_2 = root["recommd2"].asUInt();
				batchModel.invite_uid_3 = root["recommd3"].asUInt();
				batchModel.discount = root["discount"].asString();

				for (int i = 0; i < batchModel.trades.size(); i++)
				{
					CTradeModel model;
					CTradeModel *pBaseModel = new CTradeModel;
					CTradeInfoModel infoModel = batchModel.trades[i];
					model = infoModel;
					setUpTradeModel(pBaseModel, model, session);
					pBaseModel->uid = batchModel.uid;
					pBaseModel->token = batchModel.token;
					pBaseModel->sysUid = batchModel.sysUid;
					pBaseModel->invite_uid_1 = batchModel.invite_uid_1;
					pBaseModel->invite_uid_2 = batchModel.invite_uid_2;
					pBaseModel->invite_uid_3 = batchModel.invite_uid_3;
					pBaseModel->discount = batchModel.discount;
					vecRet.push_back((CBaseModel *)pBaseModel);
				}
				CDispatchManager::recordData(vecRet);
			}
			else
			{
				DEBUG_INFO("业务类型错误");
				vecUCAuthResp.erase(vecUCAuthResp.begin() + i);
				delete session;
				session = NULL;
			}
		}
		else
		{
			DEBUG_INFO("用户中心修改撤单/批量撤单/撮合成功资产修改成功，业务在这里不与处理");
			vecUCAuthResp.erase(vecUCAuthResp.begin() + i);
			delete session;
			session = NULL;
		}
	}
}
void CThreadUserCenter::operMatchResp(vector<CBaseModel *> &vecMTResp,vector<CBaseSession *>& vecRet)
{
	// 此vecMTResp变量是撤单请求和批量撤单请求的拆分集合
	/*
	* vecMTResp包含撤单/批量撤单
	* 1：撤单成功；记录日志；重新打包为assetsupdate格式数据包转发用户中心；返回给客户端撤单成功
	* 2：撤单失败；返回给客户端撤单失败
	* 3：查询订单结果
	*/

	for (size_t i = 0; i < vecMTResp.size(); i++)
	{
		CBaseModel *pBaseModel = (CCancelTradeModel *)vecMTResp[i];
		map<string, CBaseSession *>::iterator itr = CThreadUserCenter::g_mapAuthSession.find(pBaseModel->token);
		if (itr != CThreadUserCenter::g_mapAuthSession.end())
		{
			if (pBaseModel->bResultOper == true)
			{
				if (pBaseModel->modelType == OPER_TYPE_CANCELTRADE || pBaseModel->modelType == OPER_TYPE_CANCELTRADEE_BATCH)
				{
					/*
					撤单/批量撤单
					*/
					// 记录撤单日志
					CDispatchManager::recordData((CBaseModel *)pBaseModel);
					UVWriteReq *write = NULL;
					if (pBaseModel->modelType == OPER_TYPE_CANCELTRADE)
					{
						// 撤单
						CCancelTradeModel model;
						model = (CCancelTradeModel *)pBaseModel;
						write = itr->second->rePackPkg(model);
					}
					else
					{
						// 批量撤单
						CCancelTradeBatchModel model;
						model = (CCancelTradeBatchModel *)pBaseModel;
						write = itr->second->rePackPkg(model);
					}

					/* 撤单成功赋值以后，将重新转换成assetsupdate的数据转发用户中心修改资产 */
					uv_write((uv_write_t *)write, CThreadUserCenter::fdSocket, &write->buf, 1, CThreadUserCenter::cbWrited);
				}
				else
				{
					// 查询订单结果成功
					itr->second->setRespData(pBaseModel->data);
				}
				
				// 返回状态成功给客户端
				itr->second->setStatus(1);
			}
			else
			{
				// 撤单/批量撤单失败/查询订单结果
				itr->second->setStatus(0);
				itr->second->setRespData(pBaseModel->data);
			}
			
			vecRet.push_back(itr->second);
		}
		else
		{
			DEBUG_ERROR("撤单信息没有找到请求session");
		}
		delete pBaseModel;
		pBaseModel = NULL;
	}
}
void CThreadUserCenter::cbTimer(uv_timer_t *handle)
{
	// 是否成功连接用户中心
	if (NULL != CThreadUserCenter::fdSocket)
	{
		/*
		所有业务包数据
		*/
		// 下单/批量下单转发到用户中心修改资产和验证->等待回复在进行后续业务处理
		vector<CBaseSession *> vecCancelAndSelect = TransUptradeToUC(); 
		PraseCancelAndSelect(vecCancelAndSelect); // 取消订单和查询订单的信息，直接转发撮合线程进行处理
	}

	vector<CBaseModel *> vecMTResp;
	CThreadMatch::m_qUCToMTRespQueue.get(vecMTResp);
	/*
	UCT撤单结果回复队列
	vecMTResp包含的业务
	1:撤单
	2:批量撤单
	3:查询结果
	*/
	// vecCancelReqSession变量用于存储根据CBaseModel找到的原始请求session集合
	vector<CBaseSession *> vecCancelReqSession;
	operMatchResp(vecMTResp, vecCancelReqSession);
	respToClient(vecCancelReqSession, false);

	vector<CBaseSession *> vecUCResp = getUCReturnPkg(); // 用户中心资产修改结果回复
	if (vecUCResp.size() != 0)
	{
		/*
		note：这里进来的业务场景有
		1：下单/批量下单 资产修改的结果返回
		2：撤单/批量撤单 撤单资产修改结果（不与处理即可）
		3：撮合成功 资产修改成功（不与处理即可）
		*/
		vector<CBaseSession *> vecUCAuthResp = getAuthPass(vecUCResp); // 验证通过的回复
		// 根据用户中心返回的token，反填session的请求数据到新的结构体中
		vector<CBaseModel *> vecOrginSession;
		// 资产修改验证成功的回复信息，通过其token，找到之前请求的session中的参数，并且生成数据模型CBaseModel对象
		// 删除回复成功的撤单/批量撤单/撮合成功资产修改的请求
		getOriginSession(vecUCAuthResp, vecOrginSession);
		// 回复资产修改验证失败的信息给客户端
		// 合并两个vector
		vecUCAuthResp.insert(vecUCAuthResp.end(),vecUCResp.begin(),vecUCResp.end());
		// 返回客户端操作成功（下单，撤单）
		respToClient(vecUCAuthResp);

		// 把组装好的新的结构体，放入撮合队列中
		// 到这里vecOrginSession仅包含，下单/批量下单请求
		CThreadMatch::m_qUCToMTQueue.put(vecOrginSession);
	}
	/*
	撮合逻辑部分
	*/
	vector<UVWriteReq *> vecWrite; // 获取撮合成功的数据
	CThreadMatch::m_qMTToUCQueue.get(vecWrite);
	// 这里计算手续费；重新打包成assetsupdate格式；记录日志；
	MatchDataToUserCenter(vecWrite);
}
void CThreadUserCenter::MatchDataToUserCenter(vector<UVWriteReq *> &vec)
{
	int iVecSize = vec.size();
	for (int i = 0; i < iVecSize; i++)
	{
		uv_write((uv_write_t *)vec[i], CThreadUserCenter::fdSocket, &vec[i]->buf, 1, CThreadUserCenter::cbWrited);
	}
	vec.clear();
}
void CThreadUserCenter::cbReadBuff(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
#ifdef PRINT_LOG
	cout << "[CThreadUserCenter->cbNewConnection->cbReadBuff] Malloc Buffer" << endl;
#endif
	buf->base = g_cache_ucthread_read.Get(SIZE_BUFFER_8k);
	buf->len = SIZE_BUFFER_8k;
}

void CThreadUserCenter::cbClosed(uv_handle_t *handle)
{
	cout << "[CThreadUserCenter::cbClosed] UserCenter Tcp disconnect" << endl;
	mapBaseSession::iterator itrSession = CDispatchManager::g_mapUserCenterSession.find((client_pkg_t *)handle);
	if (itrSession != CDispatchManager::g_mapUserCenterSession.end())
	{
		if (NULL != itrSession->second)
		{
			delete itrSession->second;
			itrSession->second = NULL;
		}
		CDispatchManager::g_mapUserCenterSession.erase(itrSession);
	}
	reConnect();
}

void CThreadUserCenter::cbWrited(uv_write_t *req, int status)
{
	DEBUG_INFO("[CThreadUserCenter->cbWrited]");
	g_cache_ucthread_push.Free((UVWriteReq *)req);
}

void CThreadUserCenter::cbRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
#ifdef PRINT_LOG
	cout << "[CThreadUserCenter->cbNewConnection->cbRead] Server begin read" << endl;
#endif
	if (nread < 0)
	{
#ifdef PRINT_LOG
		cout << "[CThreadUserCenter->cbNewConnection->cbRead] socket disconnect" << endl;
#endif
		uv_close((uv_handle_t *)client, cbClosed);
	}
	else if (nread == 0)
	{
#ifdef PRINT_LOG
		cout << "[CThreadUserCenter->cbNewConnection->cbRead] read noting" << endl;
#endif
		uv_close((uv_handle_t *)client, cbClosed);
	}
	else
	{
		int iOperRet = CDispatchManager::DispatchClient(CDispatchManager::g_mapUserCenterSession, (client_pkg_t *)client, buf->base, nread, false);
		if (RET_SUCCESS == iOperRet)
		{
			mapBaseSession::iterator itr = CDispatchManager::g_mapUserCenterSession.find((client_pkg_t *)client);
			assert(itr != CDispatchManager::g_mapUserCenterSession.end());
			CThreadUserCenter::g_qRecvUCRespQueue.put(itr->second);
			CDispatchManager::g_mapUserCenterSession.erase(itr++);
#ifdef PRINT_LOG
			cout << "[CDispatchManager::DispatchHttpClient] socket recv RET_SUCCESS" << endl;
#endif
		}
		else if (RET_FAILED == iOperRet)
		{
#ifdef PRINT_LOG
			cout << "[CDispatchManager::DispatchHttpClient] socket recv RET_FAILED" << endl;
#endif
			uv_close((uv_handle_t *)client, cbClosed);
		}
		else
		{
#ifdef PRINT_LOG
			cout << "[CDispatchManager::DispatchHttpClient] socket recv RET_HOLD" << endl;
#endif
		}
	}
#ifdef PRINT_LOG
	cout << "[CThreadUserCenter::cbRead] delete[] buf->base" << endl;
#endif
	g_cache_ucthread_read.Free(buf->base, buf->len);
}

void CThreadUserCenter::reConnect()
{
	/*
	* 用户中心服务器连接不存在，backtrade每隔iSleepTime秒后重连服务器
	*/
	CThreadUserCenter::fdSocket = NULL;
	int iSleepTime = 5;
	cout << "[CThreadUserCenter::cbConnect] connect UserCenter after " << iSleepTime << " seconds" << endl;
	sleep(iSleepTime);

	client_pkg_t *server = new client_pkg_t;
	server->ulPkgIndex = CDispatchManager::g_ulUCIndex++;
	uv_tcp_init(&g_uvUserCenterLoop, (uv_tcp_t *)server);
	uv_connect_t *connect = new uv_connect_t;
	struct sockaddr_in addr;
	uv_ip4_addr(Config::g_sUserCenterIP.c_str(), Config::g_uiUserCenterPort, &addr);
	uv_tcp_connect(connect, server, (const sockaddr *)&addr, cbConnect);

	if (NULL != CThreadUserCenter::g_connect)
	{
		delete CThreadUserCenter::g_connect;
		CThreadUserCenter::g_connect = NULL;
	}
	CThreadUserCenter::g_connect = connect;

	if (NULL != CThreadUserCenter::g_server)
	{
		delete CThreadUserCenter::g_server;
		CThreadUserCenter::g_server = NULL;
	}
	CThreadUserCenter::g_server = server;
}

void CThreadUserCenter::cbConnect(uv_connect_t *req, int status)
{
	cout << "[CThreadOrder::cbConnect] = " << uv_strerror(status) << " status = " << status << endl;
	if (status == 0)
	{
		client_pkg_t *client = (client_pkg_t *)req->handle;
		client->ulPkgIndex = CDispatchManager::g_ulUCIndex++;

		int ret = uv_read_start((uv_stream_t *)client, cbReadBuff, cbRead);
		CThreadUserCenter::fdSocket = (uv_stream_t *)client;
		cout << "[CThreadOrder::cbConnect] connect UserCenter success index = " << client->ulPkgIndex << " ret code = " << ret << endl;
		rsaLoginServer(client);
	}
	else
	{
		reConnect();
	}
}
void CThreadUserCenter::rsaLoginServer(client_pkg_t *client)
{
	/*
	* 连接上用户中心后，向用户中心发送连接消息，publickey 加密
	*/
	Json::Value sJson;
	sJson["mid"] = Config::uiMarketID;
	sJson["coinA"] = Config::sCoinA;
	sJson["coinB"] = Config::sCoinB;
	/*
	* protocol buff 打包
	*/
	UPLogin *lg = new UPLogin;
	lg->ClientRequest(Config::sRSAUserCenterPubKey, sJson, false);
	const int BUFFSIZE = ProtoPackSize<UPLogin>(*lg);
	UVWriteReq *write = g_cache_ucthread_login.Get(BUFFSIZE);
	assert(write != NULL);
	assert(write->buf.base != NULL);
	write->buf.len = ProtoPack<UPLogin>(write->buf.base, BUFFSIZE, *lg);
	assert(write->buf.len > 0);
	write->pkg = lg;
	uv_write((uv_write_t *)write, (uv_stream_t *)client, &write->buf, 1, CThreadUserCenter::cbLogin);
}
void CThreadUserCenter::cbLogin(uv_write_t *req, int status)
{
	g_cache_ucthread_login.Free((UVWriteReq *)req);
}
void CThreadUserCenter::loadRuntimeConfig()
{
	string sConfigPath = g_dir_root + "BaseConfig/RuntimeConfig.json";
	if (CRuntimeConfig::Load(sConfigPath, CThreadUserCenter::g_sConfigFileUpdateTime) == false)
	{
		// DEBUG_ERROR("Load config file failed");
	}
	else
	{
		// DEBUG_INFO("服务器配置信息加载完毕");
	}
}
void CThreadUserCenter::ThreadUserCenter(void *arg)
{
	cout << "[CThreadUserCenter Init]" << endl;
	uv_loop_init(&g_uvUserCenterLoop);
	g_cache_ucthread_login.Init(SIZE_BUFFER_2k, 0, 0);
	g_cache_ucthread_read.Init(0, 100, 0);
	g_cache_ucthread_push.Init(0,100,0);
	loadRuntimeConfig();
	struct sockaddr_in addr;
	CThreadUserCenter::g_server = new client_pkg_t;
	CThreadUserCenter::g_connect = new uv_connect_t;
	CThreadUserCenter::g_server->ulPkgIndex = CDispatchManager::g_ulUCIndex++;

	uv_tcp_init(&g_uvUserCenterLoop, (uv_tcp_t *)CThreadUserCenter::g_server);
	uv_ip4_addr(Config::g_sUserCenterIP.c_str(), Config::g_uiUserCenterPort, &addr);
	int ret = uv_tcp_connect(CThreadUserCenter::g_connect, CThreadUserCenter::g_server, (const sockaddr *)&addr, cbConnect);
	cout << "[CThreadUserCenter] connect code = " << uv_strerror(ret) << endl;

	uv_timer_init(&g_uvUserCenterLoop, &g_uvUserCenterTimer);
	uv_timer_start(&g_uvUserCenterTimer, cbTimer, 10, 1);
	uv_run(&g_uvUserCenterLoop, UV_RUN_DEFAULT);
}
