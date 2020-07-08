#include "ThreadMatch.h"
#include "ThreadDB.h"
uv_loop_t g_uvMatchLoop;
uv_timer_t g_uvMatchTimer;

#define THIS_LOOP g_uvMatchLoop
#define THIS_TIMER g_uvMatchTimer

UBBlockQueue<CBaseModel> CThreadMatch::m_qUCToMTQueue;
UBBlockQueue<CBaseModel> CThreadMatch::m_qUCToMTRespQueue;
UBBlockObjQueue<CMatchedDataModel> CThreadMatch::m_qMTToDBQueue;
UBBlockQueue<UVWriteReq> CThreadMatch::m_qMTToUCQueue;

vector<CTradeModel> CThreadMatch::g_vecOrder;
map<int, CTradeModel> CThreadMatch::m_mapOrderInfo;
mapSortForPrice CThreadMatch::m_mapMatchBuy;
mapSortForPrice CThreadMatch::m_mapMatchSell;

int CThreadMatch::RecordTrade(CTradeModel model, mapSortForPrice &mapInsert)
{
	int iRet = RET_SUCCESS;
	int32_t nowTime = (int32_t)time(NULL);
	model.time = nowTime;
	// 记录订单id与请求数据结构的关系

	pair<map<int, CTradeModel>::iterator, bool> pairID = CThreadMatch::m_mapOrderInfo.insert(pair<int, CTradeModel>(model.orderid, model));
	if (pairID.second == false)
	{
		DEBUG_ERROR("map插入失败 orderid");
		iRet = RET_FAILED;
	}
	else
	{
		if (model.type == 1 || model.type == 2)
		{
			mapSortForPrice::iterator itr = mapInsert.find(model.price); // 查找相同价格是否存在
			if (itr != mapInsert.end())
			{
				// 此价格map存在
				mapSortForTime::iterator itrTime = itr->second.find(nowTime); // 查找是否存在时间为key的map节点
				if (itrTime != itr->second.end())
				{
					// 存在此map节点，累加到此节点的vector后面
					itrTime->second.push_back(model);
				}
				else
				{
					// 不存在以时间为节点的map，创建此节点
					vecMatchData vec;
					vec.push_back(model);
					pair<mapSortForTime::iterator, bool> pairTime = itr->second.insert(pair<int32_t, vecMatchData>(nowTime, vec));
					if (pairTime.second == false)
					{
						DEBUG_ERROR("map插入失败 nowTime");
						iRet = RET_FAILED;
					}
				}
			}
			else
			{
				// 此价格不存在
				vecMatchData vec;
				vec.push_back(model); // 存入队列中

				mapSortForTime mapTime;
				pair<mapSortForTime::iterator, bool> pairTime = mapTime.insert(pair<int32_t, vecMatchData>(nowTime, vec)); // 队列存入以时间为key的map中
				if (pairTime.second == false)
				{
					DEBUG_ERROR("map插入失败 nowTime");
					iRet = RET_FAILED;
				}

				pair<mapSortForPrice::iterator, bool> pairPrice = mapInsert.insert(pair<string, mapSortForTime>(model.price, mapTime)); // 时间为key的map存入以价格为key的map中
				if (pairPrice.second == false)
				{
					DEBUG_ERROR("map插入失败 mapTime");
					iRet = RET_FAILED;
				}
			}
		}
		else
		{
			DEBUG_ERROR("trade type error");
			iRet = RET_FAILED;
		}
	}

	return iRet;
}
int CThreadMatch::RemoveTrade(CCancelTradeModel &model, mapSortForPrice &mapRemove)
{
	int iRet = RET_FAILED;
	// 根据订单id查找订单信息
	map<int, CTradeModel>::iterator itrResp = CThreadMatch::m_mapOrderInfo.find(model.canceltradeid);
	if (itrResp != CThreadMatch::m_mapOrderInfo.end())
	{
		if (model.type == 1 || model.type == 2)
		{
			// 查找是否存在对应价格的节点
			mapSortForPrice::iterator itrPrice = mapRemove.find(itrResp->second.price);
			if (itrPrice != mapRemove.end())
			{
				// 查找是否存在对应时间的节点
				mapSortForTime::iterator itrTime = itrPrice->second.find(itrResp->second.time);
				if (itrTime != itrPrice->second.end())
				{
					bool bFindNode = false;
					// 便利时间节点中的队列
					for (size_t i = 0; i < itrTime->second.size(); i++)
					{
						CTradeModel obj = itrTime->second[i];
						// 找到队列中所有订单号符合的节点，删除掉
						if (obj.orderid == model.canceltradeid)
						{
							model.price = obj.price;
							model.num = obj.num;
							bFindNode = true;
							itrTime->second.erase(itrTime->second.begin());
							i--;
						}
					}
					// 如果所有节点被删除，这依次删除上级map节点
					if (0 == itrTime->second.size())
					{
						itrPrice->second.erase(itrTime);
						mapRemove.erase(itrPrice);
					}
					CThreadMatch::m_mapOrderInfo.erase(itrResp);
					iRet = bFindNode == true ? RET_SUCCESS : RET_FAILED;
				}
				else
				{
					DEBUG_ERROR("can't find Time node");
				}
			}
			else
			{
				DEBUG_ERROR("can't find Price node");
			}
		}
		else
		{
			DEBUG_ERROR("cancel trade error");
		}
	}
	return iRet;
}
int CThreadMatch::BatRecordTrade(CTradeBatchModel model)
{
	int iTradeCount = model.trades.size();
	for (int i = 0; i < iTradeCount; i++)
	{
		CTradeInfoModel infoModel = model.trades[i];
		CTradeModel insrtModel;
		insrtModel = infoModel;
		insrtModel.uid = model.uid;
		if (RecordTrade(insrtModel, insrtModel.type == 1 ? CThreadMatch::m_mapMatchBuy : CThreadMatch::m_mapMatchSell) == RET_FAILED)
		{
			return RET_FAILED;
		}
	}
	return RET_SUCCESS;
}
int CThreadMatch::BatRemoveTrade(CCancelTradeBatchModel &model, vector<uint64_t> &vecCancelFailed)
{
	bool bIsFailed = false;
	int iOrderCount = model.orders.size();
	for (int i = 0; i < iOrderCount; i++)
	{
		CCancelTradeModel insrtModel;
		insrtModel = model.orders[i];
		insrtModel.uid = model.uid;

		if (RemoveTrade(insrtModel, insrtModel.type == 1 ? CThreadMatch::m_mapMatchBuy : CThreadMatch::m_mapMatchSell) == RET_FAILED)
		{
			vecCancelFailed.push_back(insrtModel.orderid);
			bIsFailed = true;
			model.orders[i].bIsSucc = false;
		}
		else
		{
			model.orders[i].bIsSucc = true;
		}

		model.orders[i].price = insrtModel.price;
		model.orders[i].num = insrtModel.num;
	}
	return bIsFailed == false ? RET_SUCCESS : RET_FAILED;
}

int CThreadMatch::getOrderInfo(CGetOrderInfoModel& model)
{
	int iRet = RET_FAILED;
	// 根据订单id查找订单信息
	map<int, CTradeModel>::iterator itrResp = CThreadMatch::m_mapOrderInfo.find(model.orderid);
	if (itrResp != CThreadMatch::m_mapOrderInfo.end())
	{
		Json::Value retData;
		retData["orderNumber"] = model.orderid;
		retData["initialRate"] = itrResp->second.price;
		retData["initialAmount"] = itrResp->second.totalNum;
		retData["amount"] = CTool::minusLongDouble(itrResp->second.totalNum,itrResp->second.tradedNum);
		retData["type"] = itrResp->second.type;
		retData["lastDealTime"] = itrResp->second.time;
		retData["status"] = "open";
		retData["currencyPair"] = Config::sMarket;
		Json::FastWriter writer;
		model.data = writer.write(retData);
		iRet = RET_SUCCESS;
	}
	return iRet;
}
int CThreadMatch::dispatchOperNodes(CBaseModel *pModel)
{
	int iRet = RET_FAILED;
	switch (pModel->modelType)
	{
	case OPER_TYPE_UPTRADE:
	{
		CTradeModel model;
		model = (CTradeModel *)pModel;
		iRet = RecordTrade(model, model.type == 1 ? CThreadMatch::m_mapMatchBuy : CThreadMatch::m_mapMatchSell);
		if (iRet == RET_SUCCESS)
		{
			DEBUG_DEBUG("下单成功");
			pModel->bResultOper = true;
		}
		else
		{
			DEBUG_ERROR("下单失败");
			pModel->bResultOper = false;
		}
	}
	break;
	case OPER_TYPE_CANCELTRADEE_BATCH:
	{
		CCancelTradeBatchModel model;
		vector<uint64_t> vecCancelFailed;
		CCancelTradeBatchModel *pTemp = (CCancelTradeBatchModel *)pModel;
		model = pTemp;
		iRet = BatRemoveTrade(model, vecCancelFailed);
		if (iRet == RET_SUCCESS)
		{
			DEBUG_DEBUG("撤单成功");
			pModel->bResultOper = true;
		}
		else
		{
			DEBUG_ERROR("撤单失败");
			Json::Value retData;
			for (int i = 0; i < vecCancelFailed.size(); i++)
			{
				retData[i] = vecCancelFailed[i];
			}
			Json::FastWriter writer;
			pModel->data = writer.write(retData);
			pModel->bResultOper = false;
		}
		for (int i = 0; i < model.orders.size(); i++)
		{
			for (int j = 0; j < pTemp->orders.size(); j++)
			{
				if (pTemp->orders[j].orderid == model.orders[i].orderid)
				{
					pTemp->orders[j].price = model.orders[i].price;
					pTemp->orders[j].num = model.orders[i].num;
					pTemp->orders[j].bIsSucc = model.orders[i].bIsSucc;
				}
			}
		}
	}
	break;
	case OPER_TYPE_CANCELTRADE:
	{
		CCancelTradeModel model;
		CCancelTradeModel *pTemp = (CCancelTradeModel *)pModel;
		model = pTemp;
		iRet = RemoveTrade(model, model.type == 1 ? CThreadMatch::m_mapMatchBuy : CThreadMatch::m_mapMatchSell);
		if (iRet == RET_SUCCESS)
		{
			DEBUG_DEBUG("撤单成功");
			pModel->bResultOper = true;
		}
		else
		{
			DEBUG_ERROR("撤单失败");
			pModel->bResultOper = false;
			Json::Value retData;
			retData[0] = model.orderid;
			Json::FastWriter writer;
			pModel->data = writer.write(retData);
		}
		pTemp->price = model.price;
		pTemp->num = model.num;
	}
	break;
	case OPER_TYPE_SELECT:
	{
		// 查询订单
		CGetOrderInfoModel model;
		model = (CGetOrderInfoModel *)pModel;
		iRet = getOrderInfo(model);
		if (iRet == RET_SUCCESS)
		{
			DEBUG_DEBUG("查询成功");
			pModel->bResultOper = true;
			pModel->data = model.data;
		}
		else
		{
			DEBUG_ERROR("查询失败");
			pModel->bResultOper = false;
			Json::Value retData;
			retData["orderid"] = model.orderid;
			Json::FastWriter writer;
			pModel->data = writer.write(retData);
		}
	}
	break;
	default:
		break;
	}

	return iRet;
}
void CThreadMatch::cbTimer(uv_timer_t *handle)
{
	vector<CBaseModel *> vecToMatchQueue, vecTransDB, vecCancelTrade;
	/* 
	变量vecToMatchQueue 包含3种业务场景的数据包 
	1：下单/批量下单
	2：撤单/批量撤单
	3：查询订单信息
	*/

	CThreadMatch::m_qUCToMTQueue.get(vecToMatchQueue);
	int iVecSize = vecToMatchQueue.size();
	for (size_t i = 0; i < iVecSize; i++)
	{
		CBaseModel *pModel = vecToMatchQueue[i];
		dispatchOperNodes(pModel); // 对所有数据包进行买卖队列操作，重新组包的操作
		if (pModel->modelType == OPER_TYPE_CANCELTRADE || pModel->modelType == OPER_TYPE_CANCELTRADEE_BATCH || pModel->modelType == OPER_TYPE_SELECT)
		{
			// 撤单请求/查询请求 放入用于返回给UCT的队列
			vecCancelTrade.push_back(pModel);
		}
		// 所有操作对象放入DBT对象用作Frontmarket推送操作
		if (pModel->bResultOper == true)
		{
			vecTransDB.push_back(pModel);
		}
		else
		{
			/*
			* 撤单失败/下单失败/查询失败
			*/
			if (pModel->modelType == OPER_TYPE_CANCELTRADEE_BATCH)
			{
				CCancelTradeBatchModel *pTemp = (CCancelTradeBatchModel *)pModel;
				for (int i = 0; i < pTemp->orders.size(); i++)
				{
					// 遍历撤销订单列表，把成功的订单放到vecTransDB中
					// 撤单失败的订单，这里不处理，因为这里的都是成功的，会影响指标
					CCancelTradeInfoModel infoModel = pTemp->orders[i];
					if (infoModel.bIsSucc == true)
					{
						CCancelTradeModel *pCancelModel = new CCancelTradeModel;
						pCancelModel->marketid = infoModel.marketid;
						pCancelModel->orderid = infoModel.orderid;
						pCancelModel->uid = pTemp->uid;
						pCancelModel->token = pTemp->token;
						pCancelModel->canceltradeid = infoModel.orderid;
						pCancelModel->type = infoModel.type;
						pCancelModel->price = infoModel.price;
						pCancelModel->num = infoModel.num;
						vecTransDB.push_back(pCancelModel);
					}
				}
			}
			else if (pModel->modelType == OPER_TYPE_UPTRADE_BATCH)
			{
				DEBUG_ERROR("批量下单不会出现此情况");
			}
			else if (pModel->modelType == OPER_TYPE_UPTRADE)
			{
				DEBUG_ERROR("下单不会出现此情况");
			}
			else if (pModel->modelType == OPER_TYPE_CANCELTRADE)
			{
				DEBUG_ERROR("撤单失败这里不操作");
			}
			else
			{
				DEBUG_ERROR("查询失败这里不操作");
			}
		}
	}
	// 撮合程序下单撤单的操作结果（内存指针）返回给用户中心线程（成功只读，delete失败）
	// vecCancelTrade 包含批量撤单/撤单/查询结果
	CThreadMatch::m_qUCToMTRespQueue.put(vecCancelTrade);
	// 撮合程序下单撤单的操作结果（内存指针）传递给DB线程（失败不处理，delete成功）
	CThreadDB::m_qDBQueue.put(vecTransDB);
	vector<CMatchedDataModel> vecModel;
	matchBuySellQue(vecModel); // 买卖队列撮合逻辑
	vector<UVWriteReq *> vecWrite;
	recordToFile(vecModel,vecWrite);
	CThreadMatch::m_qMTToUCQueue.put(vecWrite); // 先测试数据库手续费入库这些的功能
	// 转发数据对象到数据库线程
	CThreadMatch::m_qMTToDBQueue.put(vecModel);
}
void CThreadMatch::recordToFile(vector<CMatchedDataModel> &vecModel,vector<UVWriteReq *>& vecWrite)
{
	FILE *fp;
	if ((fp = fopen("../Record/Match.dat", "a+")) == NULL)
	{
		DEBUG_ERROR("RECORD FILE ERROR");
	}
	else
	{
		int time = CTool::getNowTime();
		int iVecSize = vecModel.size();
		for (int i = 0; i < iVecSize; i++)
		{
			CMatchedDataModel model = vecModel[i];
			model.modelType = OPER_TYPE_MATCH_SUC;
			// 本次交易总额
			string amount = CTool::mutipLongDouble(model.price, model.number);
			// 买方预期总额
			string buyerExpectAmount = CTool::mutipLongDouble(model.buyuser.price, model.number);
			// 卖方预期总额
			string sellerExpectAmount = CTool::mutipLongDouble(model.selluser.price, model.number);
			string buyerRealDiffAmount = "0.0";	 // 买方真实差额
			string sellerRealDiffAmount = "0.0"; // 卖方真实差额
			if (CTool::minusLongDouble(model.price, model.buyuser.price) == "-1")
			{
				// 成交价格是小值
				buyerRealDiffAmount = CTool::minusLongDouble(buyerExpectAmount, amount);
				sellerRealDiffAmount = CTool::minusLongDouble(sellerExpectAmount, amount);
			}
			else
			{
				// 成交价格是大值
				buyerRealDiffAmount = CTool::minusLongDouble(amount, buyerExpectAmount);
				sellerRealDiffAmount = CTool::minusLongDouble(amount, sellerExpectAmount);
			}
			// 买方预期手续费
			string buyerExpectFee = CTool::mutipLongDouble(buyerExpectAmount, CRuntimeConfig::market.m_sBuyFee);
			// 买方实际差额手续费
			string buyerRealDiffFee = CTool::mutipLongDouble(buyerRealDiffAmount, CRuntimeConfig::market.m_sBuyFee);
			// 买方真实手续费
			string buyerRealFee = CTool::mutipLongDouble(amount, CRuntimeConfig::market.m_sBuyFee);
			buyerRealFee = CTool::mutipLongDouble(buyerRealFee, model.buyuser.discount);
			model.buyuser.invite_fee_1 = CTool::mutipLongDouble(buyerRealFee, CRuntimeConfig::market.m_sInvite_fee_1);
			model.buyuser.invite_fee_2 = CTool::mutipLongDouble(buyerRealFee, CRuntimeConfig::market.m_sInvite_fee_2);
			model.buyuser.invite_fee_3 = CTool::mutipLongDouble(buyerRealFee, CRuntimeConfig::market.m_sInvite_fee_3);

			buyerRealFee = CTool::minusLongDouble(buyerRealFee, model.buyuser.invite_fee_1);
			buyerRealFee = CTool::minusLongDouble(buyerRealFee, model.buyuser.invite_fee_2);
			buyerRealFee = CTool::minusLongDouble(buyerRealFee, model.buyuser.invite_fee_3);

			// 卖方真实手续费
			string sellerRealFee = CTool::mutipLongDouble(amount, CRuntimeConfig::market.m_sSellFee);
			sellerRealFee = CTool::mutipLongDouble(sellerRealFee, model.selluser.discount);
			model.selluser.invite_fee_1 = CTool::mutipLongDouble(sellerRealFee, CRuntimeConfig::market.m_sInvite_fee_1);
			model.selluser.invite_fee_2 = CTool::mutipLongDouble(sellerRealFee, CRuntimeConfig::market.m_sInvite_fee_2);
			model.selluser.invite_fee_3 = CTool::mutipLongDouble(sellerRealFee, CRuntimeConfig::market.m_sInvite_fee_3);

			UPMatchFile *pRecord = new UPMatchFile;
			pRecord->type = OPER_TYPE_MATCH_SUC;
			pRecord->match.id = model.id;
			pRecord->match.buyer_user_id = model.buyuser.userid;
			pRecord->match.seller_user_id = model.selluser.userid;
			pRecord->match.sysUid = model.sysUid;
			pRecord->match.market_id = Config::uiMarketID;
			strncpy(pRecord->match.price, model.price.c_str(), model.price.length());
			strncpy(pRecord->match.dealnum, model.number.c_str(), model.number.length());

			if (amount != "-1")
				strncpy(pRecord->match.trading_amount, amount.c_str(), amount.length());
			else
				strncpy(pRecord->match.trading_amount, "0.0", 3);

			pRecord->match.time = time;
			pRecord->match.buyer_order_id = model.buyuser.orderid;
			pRecord->match.seller_order_id = model.selluser.orderid;
			strncpy(pRecord->match.buyer_price, model.buyuser.price.c_str(), model.buyuser.price.length());
			strncpy(pRecord->match.seller_price, model.selluser.price.c_str(), model.selluser.price.length());
			pRecord->match.buyer_discount = stof(model.buyuser.discount);
			pRecord->match.seller_discount = stof(model.selluser.discount);
			strncpy(pRecord->match.buyer_traded_volume, model.buyuser.tradedNum.c_str(), model.buyuser.tradedNum.length());
			strncpy(pRecord->match.seller_traded_volume, model.selluser.tradedNum.c_str(), model.selluser.tradedNum.length());
			pRecord->match.buyer_invite_uid_1 = model.buyuser.invite_uid_1;
			pRecord->match.buyer_invite_uid_2 = model.buyuser.invite_uid_2;
			pRecord->match.buyer_invite_uid_3 = model.buyuser.invite_uid_3;
			pRecord->match.seller_invite_uid_1 = model.selluser.invite_uid_1;
			pRecord->match.seller_invite_uid_2 = model.selluser.invite_uid_2;
			pRecord->match.seller_invite_uid_3 = model.selluser.invite_uid_3;

			strncpy(pRecord->match.buyer_sys_fee, buyerRealFee.c_str(), buyerRealFee.length());
			strncpy(pRecord->match.seller_sys_fee, sellerRealFee.c_str(), sellerRealFee.length());

			strncpy(pRecord->match.buyer_invite_fee_1, model.buyuser.invite_fee_1.c_str(), model.buyuser.invite_fee_1.length());
			strncpy(pRecord->match.buyer_invite_fee_2, model.buyuser.invite_fee_2.c_str(), model.buyuser.invite_fee_2.length());
			strncpy(pRecord->match.buyer_invite_fee_3, model.buyuser.invite_fee_3.c_str(), model.buyuser.invite_fee_3.length());
			strncpy(pRecord->match.seller_invite_fee_1, model.selluser.invite_fee_1.c_str(), model.selluser.invite_fee_1.length());
			strncpy(pRecord->match.seller_invite_fee_2, model.selluser.invite_fee_2.c_str(), model.selluser.invite_fee_2.length());
			strncpy(pRecord->match.seller_invite_fee_3, model.selluser.invite_fee_3.c_str(), model.selluser.invite_fee_3.length());

			fwrite(pRecord, sizeof(class UPMatchFile), 1, fp);
			fflush(fp);
			delete pRecord;

			// 资产情况组包
			UPassetsupdate *pkg = new UPassetsupdate;
			pkg->set_uid(model.sysUid);
			pkg->set_paypassword("");
			pkg->set_opertype(OPER_TYPE_MATCH_SUC);
			pkg->set_operid(0);

			// 买方货币变动
			string frozenMinus = CTool::addLongDouble(buyerExpectAmount, buyerExpectFee);
			string usableAdd = CTool::addLongDouble(buyerRealDiffFee, buyerRealDiffAmount);
			assetsListSet(pkg->add_list(), model.buyuser.userid, model.id, OPER_TYPE_TRADE_MATCH, Config::sCoinB, usableAdd, "-" + frozenMinus);
			// 买方商品增加
			assetsListSet(pkg->add_list(), model.buyuser.userid, model.id, OPER_TYPE_TRADE_MATCH, Config::sCoinA, model.number, "0.0");
			// 卖方商品减少
			assetsListSet(pkg->add_list(), model.selluser.userid, model.id, OPER_TYPE_TRADE_MATCH, Config::sCoinA, "0.0", "-" + model.number);
			// 卖方货币变动
			string usableSellAdd = CTool::minusLongDouble(amount, sellerRealFee);
			assetsListSet(pkg->add_list(), model.selluser.userid, model.id, OPER_TYPE_TRADE_MATCH, Config::sCoinB, usableAdd, "0.0");

			// 邀请人可用增加
			assetsListSet(pkg->add_list(), model.buyuser.invite_uid_1, model.id, OPER_TYPE_TRADE_RECOMMENDATION_REWARDS, Config::sCoinB, model.buyuser.invite_fee_1, "0.0");
			assetsListSet(pkg->add_list(), model.buyuser.invite_uid_2, model.id, OPER_TYPE_TRADE_RECOMMENDATION_REWARDS, Config::sCoinB, model.buyuser.invite_fee_2, "0.0");
			assetsListSet(pkg->add_list(), model.buyuser.invite_uid_3, model.id, OPER_TYPE_TRADE_RECOMMENDATION_REWARDS, Config::sCoinB, model.buyuser.invite_fee_3, "0.0");
			assetsListSet(pkg->add_list(), model.selluser.invite_uid_1, model.id, OPER_TYPE_TRADE_RECOMMENDATION_REWARDS, Config::sCoinB, model.selluser.invite_fee_1, "0.0");
			assetsListSet(pkg->add_list(), model.selluser.invite_uid_2, model.id, OPER_TYPE_TRADE_RECOMMENDATION_REWARDS, Config::sCoinB, model.selluser.invite_fee_2, "0.0");
			assetsListSet(pkg->add_list(), model.selluser.invite_uid_3, model.id, OPER_TYPE_TRADE_RECOMMENDATION_REWARDS, Config::sCoinB, model.selluser.invite_fee_3, "0.0");

			// 平台资产可用增加
			string usable = CTool::addLongDouble(buyerRealFee, sellerRealFee);
			assetsListSet(pkg->add_list(), model.sysUid, model.id, OPER_TYPE_TRANSACTION_FEE, Config::sCoinB, usable, "0.0");

			const int BUFFSIZE = ProtoPackSize<UPassetsupdate>(*pkg);
			UVWriteReq *write = g_cache_orderthread_resp.Get(BUFFSIZE);
			write->buf.len = ProtoPack<UPassetsupdate>(write->buf.base, BUFFSIZE, *pkg);
			write->pkg = pkg;
			vecWrite.push_back(write);
		}
		fclose(fp);
	}
}
void CThreadMatch::assetsListSet(UPassetsupdate::List *pList, uint64_t uid, uint64_t type_id, ASSETS_UPDATE oper_type, string coinid, string usable, string fronzen)
{
	pList->set_uid(uid);
	pList->set_type_id(type_id);
	pList->set_type(oper_type);
	pList->set_coinid(stoi(coinid));
	pList->set_usable(usable);
	pList->set_fronzen(fronzen);
	pList->set_lockup("0.0");
}
bool CThreadMatch::ClearOrderMap(vecMatchData &vecMatch)
{
	bool bNeedClear = false;
	map<int, CTradeModel>::iterator itr = CThreadMatch::m_mapOrderInfo.find(vecMatch.begin()->orderid);
	if (itr != CThreadMatch::m_mapOrderInfo.end())
	{
		CThreadMatch::m_mapOrderInfo.erase(itr);
		vecMatch.erase(vecMatch.begin());
		if (vecMatch.size() == 0)
		{
			bNeedClear = true;
		}
	}
	else
	{
		DEBUG_ERROR("撮合成功没有找到对应的订单信息");
	}
	return bNeedClear;
}
void CThreadMatch::matchBuySellQue(vector<CMatchedDataModel> &vecRet)
{
	while (CThreadMatch::m_mapMatchBuy.size() != 0 && CThreadMatch::m_mapMatchSell.size() != 0)
	{
		if (CThreadMatch::m_mapMatchBuy.rbegin()->first >= CThreadMatch::m_mapMatchSell.begin()->first)
		{
			// 买队列中买价最高的 和 卖队列中卖家最低的开始撮合
			int iNowTime = CTool::getNowTime();
			mapSortForTime mapTimeBuy = CThreadMatch::m_mapMatchBuy.rbegin()->second;
			mapSortForTime mapTimeSell = CThreadMatch::m_mapMatchSell.begin()->second;
			vecMatchData matchBuy = mapTimeBuy.begin()->second;
			vecMatchData matchSell = mapTimeSell.begin()->second;
			CTradeModel buy = matchBuy[0];
			CTradeModel sell = matchSell[0];
			CMatchedDataModel model;
			// 成交价格以买卖双方谁线下单，则以谁的价格为准
			model.price = mapTimeBuy.begin()->first < mapTimeSell.begin()->first ? buy.price : sell.price;
			model.sysUid = buy.sysUid; // 平台用户id，买卖双方是一样的
			model.id = iNowTime;
			model.time = iNowTime;
			model.selluser.price = sell.price;
			model.token = std::to_string(buy.orderid) + std::to_string(sell.orderid);
			// 卖方本次撮合可被交易的数量
			sell.num = CTool::minusLongDouble(sell.totalNum, sell.tradedNum);
			// 买方本次撮合可被交易的数量
			buy.num = CTool::minusLongDouble(buy.totalNum, buy.tradedNum);

			// 买方用户信息
			model.buyuser.userid = buy.uid;
			model.buyuser.orderid = buy.orderid;
			model.buyuser.price = buy.price;
			model.buyuser.discount = buy.discount;
			buy.tradedNum = CTool::addLongDouble(buy.tradedNum, model.number);
			model.buyuser.tradedNum = buy.tradedNum;
			model.buyuser.tradeTotal = buy.totalNum;
			model.buyuser.invite_uid_1 = buy.invite_uid_1;
			model.buyuser.invite_uid_2 = buy.invite_uid_2;
			model.buyuser.invite_uid_3 = buy.invite_uid_3;

			// 卖方用户信息
			model.selluser.userid = sell.uid;
			model.selluser.orderid = sell.orderid;
			model.selluser.price = sell.price;
			model.selluser.discount = buy.discount;
			sell.tradedNum = CTool::addLongDouble(sell.tradedNum, model.number);
			model.selluser.tradedNum = sell.tradedNum;
			model.selluser.tradeTotal = sell.totalNum;
			model.selluser.invite_uid_1 = sell.invite_uid_1;
			model.selluser.invite_uid_2 = sell.invite_uid_2;
			model.selluser.invite_uid_3 = sell.invite_uid_3;

			if (CTool::minusLongDouble(buy.num, sell.num) != "-1")
			{
				// 买单的数量大于等于卖单的数量
				// 本次撮合成交数量，是以数量小的一个为准
				model.number = sell.num;
				if (ClearOrderMap(matchSell))
				{
					mapTimeSell.erase(mapTimeSell.begin());
					CThreadMatch::m_mapMatchSell.erase(CThreadMatch::m_mapMatchSell.begin());

					if (buy.num == sell.num)
					{
						if (ClearOrderMap(matchBuy))
						{
							mapTimeBuy.erase(mapTimeBuy.begin());
							CThreadMatch::m_mapMatchBuy.erase(--CThreadMatch::m_mapMatchBuy.end());
						}
					}
					else
					{
						; //买卖数量不相等，则不存在两边节点都需要处理的情况
					}
				}
			}
			else
			{
				// 卖单的数量大于买单的数量
				// 本次撮合成交数量，是以数量小的一个为准
				model.number = buy.num;
				if (ClearOrderMap(matchBuy))
				{
					mapTimeBuy.erase(mapTimeBuy.begin());
					CThreadMatch::m_mapMatchBuy.erase(--CThreadMatch::m_mapMatchBuy.end());
				}
			}
			vecRet.push_back(model);
		}
		else
		{
			break;
		}
	}
}
void CThreadMatch::ThreadMatch(void *arg)
{
	spdlog::info("[CThreadMatch Init]");
	uv_loop_init(&THIS_LOOP);
	uv_timer_init(&THIS_LOOP, &THIS_TIMER);
	uv_timer_start(&THIS_TIMER, cbTimer, 10, 1);
	uv_run(&THIS_LOOP, UV_RUN_DEFAULT);
}
