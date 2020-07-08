#include "ThreadControl.h"
#include "DispatchManager.h"
#include "ThreadDB.h"
#include "TradeModel.hpp"
uv_loop_t g_uvControlLoop;
uv_timer_t g_uvControlTimer;

#define THIS_LOOP g_uvControlLoop
#define THIS_TIMER g_uvControlTimer
map<client_pkg_t *, uv_thread_t *> CThreadControl::g_mapThread;
map<client_pkg_t *, CServerInfo> CThreadControl::g_mapConnectInfo;
list<CMarketInfoModel> CThreadControl::g_listMarketInfo;
UBBlockQueue<CBaseModel> CThreadControl::m_qContrlQueue;
UBBlockObjQueue<CMatchedDataModel> CThreadControl::m_qControlMatchQueue;
CDepthModel CThreadControl::g_depth;
map<int, CKlineInfoModel> CThreadControl::g_mapKline;
map<int, CKlineInfoModel> CThreadControl::g_mapKline5min;
map<int, CKlineInfoModel> CThreadControl::g_mapKline15min;
map<int, CKlineInfoModel> CThreadControl::g_mapKline30min;
map<int, CKlineInfoModel> CThreadControl::g_mapKline1h;
map<int, CKlineInfoModel> CThreadControl::g_mapKline6h;
map<int, CKlineInfoModel> CThreadControl::g_mapKline24h;

vector<CBaseModel *> CThreadControl::g_vecSessionRealse;
UBBlockQueue<CBaseSession> CThreadControl::g_qRecvReqQueue;
// UBBlockObjQueue<CKlineInfoModel> CThreadControl::m_qKLineQueue;

void CThreadControl::savePrice(int type, string price, string num, bool bIsAdd)
{
	if (type == 1)
	{
		map<string, string, CSort>::iterator itr = CThreadControl::g_depth.data.depth.buy.find(price);
		if (itr == CThreadControl::g_depth.data.depth.buy.end())
		{
			CThreadControl::g_depth.data.depth.buy.insert(pair<string, string>(price, num));
		}
		else
		{
			if (bIsAdd == true)
			{
				itr->second = CTool::addLongDouble(itr->second, num);
			}
			else
			{
				itr->second = CTool::minusLongDouble(itr->second, num);
			}
		}
	}
	else
	{
		map<string, string, CSortBack>::iterator itr = CThreadControl::g_depth.data.depth.sell.find(price);
		if (itr == CThreadControl::g_depth.data.depth.sell.end())
		{
			CThreadControl::g_depth.data.depth.sell.insert(pair<string, string>(price, num));
		}
		else
		{
			if (bIsAdd == true)
			{
				itr->second = CTool::addLongDouble(itr->second, num);
			}
			else
			{
				itr->second = CTool::minusLongDouble(itr->second, num);
			}
		}
	}
}
UPDepthdata *CThreadControl::packDepth()
{
	UPDepthdata *data = new UPDepthdata;
	data->set_type(CThreadControl::g_depth.type);
	UPDepthdata::Data *pData = data->mutable_data();
	UPDepthdata::Data::Depth *pDepth = pData->mutable_depth();

	for (map<string, string, CSortBack>::iterator itr = CThreadControl::g_depth.data.depth.buy.begin(); itr != CThreadControl::g_depth.data.depth.buy.end(); itr++)
	{
		UPDepthdata::Data::Depth::buyAction *pBuy = pDepth->add_buy();
		pBuy->set_b0(itr->first);
		pBuy->set_b1(itr->second);
	}

	for (map<string, string, CSortBack>::iterator itr = CThreadControl::g_depth.data.depth.sell.begin(); itr != CThreadControl::g_depth.data.depth.sell.end(); itr++)
	{
		UPDepthdata::Data::Depth::sellAction *pSell = pDepth->add_sell();
		pSell->set_s0(itr->first);
		pSell->set_s1(itr->second);
	}
	return data;
}
void CThreadControl::savePriceAndPush(CBaseModel *session, client_pkg_t *client, bool bIsAdd)
{
	if (bIsAdd == true)
	{
		CTradeModel model;
		model = (CTradeModel *)session;
		savePrice(model.type, model.price, model.num, bIsAdd);
	}
	else
	{
		CCancelTradeModel model;
		// model = (CTradeModel*)session;
		savePrice(model.type, model.price, model.num, bIsAdd);
	}

	UPDepthdata *data = packDepth();

	const int BUFFSIZE = ProtoPackSize<UPDepthdata>(*data);
	UVWriteReq *write = g_cache_ctrlthread_push.Get(BUFFSIZE);
	assert(write != NULL);
	assert(write->buf.base != NULL);
	write->buf.len = ProtoPack<UPDepthdata>(write->buf.base, BUFFSIZE, *data);
	assert(write->buf.len > 0);
	uv_write((uv_write_t *)write, (uv_stream_t *)client, &write->buf, 1, CThreadControl::cbWrited);
	delete data;
	data = NULL;
}
void CThreadControl::batSavePriceAndPush(CBaseSession *session, client_pkg_t *client, bool bIsAdd)
{

	if (bIsAdd == true)
	{
		CTradeBatchModel model = session->getTradeBatchModel();
		int iTradeSize = model.trades.size();
		for (size_t i = 0; i < iTradeSize; i++)
		{
			CTradeInfoModel info = model.trades[i];
			savePrice(info.type, info.price, info.num, bIsAdd);
		}
	}
	else
	{
		CCancelTradeBatchModel model = session->getCancelTradeBatchModel();
		int iOrderSize = model.orders.size();
		for (size_t i = 0; i < iOrderSize; i++)
		{
			CCancelTradeInfoModel info = model.orders[i];
			savePrice(info.type, info.price, info.num, bIsAdd);
		}
	}

	UPDepthdata *data = packDepth();

	const int BUFFSIZE = ProtoPackSize<UPDepthdata>(*data);
	UVWriteReq *write = g_cache_ctrlthread_push.Get(BUFFSIZE);
	assert(write != NULL);
	assert(write->buf.base != NULL);
	write->buf.len = ProtoPack<UPDepthdata>(write->buf.base, BUFFSIZE, *data);
	assert(write->buf.len > 0);
	uv_write((uv_write_t *)write, (uv_stream_t *)client, &write->buf, 1, CThreadControl::cbWrited);
	delete data;
	data = NULL;
}
UPAllmarketinfo *CThreadControl::packMarketInfo()
{
	int nowtime = CTool::getNowTime();
	for (list<CMarketInfoModel>::iterator itr = CThreadControl::g_listMarketInfo.begin(); itr != CThreadControl::g_listMarketInfo.end(); itr++)
	{
		if (nowtime - itr->data.nowTime > 86400)
		{
			// 超过24小时的数据删除
			CThreadControl::g_listMarketInfo.erase(itr);
		}
	}
	if (CThreadControl::g_listMarketInfo.size() == 0)
	{
		return NULL;
	}

	UPAllmarketinfo *data = new UPAllmarketinfo;
	data->set_type(CThreadControl::g_listMarketInfo.begin()->type);
	UPAllmarketinfo::Data *pData = data->add_data();
	string amount_24 = "0.0";
	string lowPirce = CThreadControl::g_listMarketInfo.begin()->data.newPrice;
	string number_24 = "0.0";
	string rate_24 = "0.0";
	string highPrice = lowPirce;

	list<CMarketInfoModel>::iterator itr = --CThreadControl::g_listMarketInfo.end();
	pData->set_m0(Config::sMarket);
	pData->set_m1(itr->data.newPrice);
	pData->set_m2(itr->data.buyPrice);
	pData->set_m3(itr->data.sellPrice);
	pData->set_m8(Config::sMarket);
	pData->set_m9(Config::webIcon);
	pData->set_m11(Config::currencyName);
	pData->set_m12(Config::appIcon);
	pData->set_m13(Config::currency);
	for (list<CMarketInfoModel>::iterator itr = CThreadControl::g_listMarketInfo.begin(); itr != CThreadControl::g_listMarketInfo.end(); itr++)
	{
		amount_24 = CTool::addLongDouble(amount_24, itr->data.amount);
		if (CTool::minusLongDouble(itr->data.newPrice, lowPirce) == "-1")
		{
			lowPirce = itr->data.newPrice;
		}
		if (CTool::minusLongDouble(highPrice, itr->data.newPrice) == "-1")
		{
			highPrice = itr->data.newPrice;
		}

		number_24 = CTool::addLongDouble(number_24, itr->data.number);
	}
	pData->set_m4(amount_24);
	pData->set_m5(lowPirce);
	pData->set_m6(number_24);
	pData->set_m7(rate_24);
	pData->set_m10(highPrice);

	return data;
}
void CThreadControl::saveMarketInfo(CMatchedDataModel model)
{
	CMarketInfoModel info;
	info.data.newPrice = model.price;
	info.data.buyPrice = model.buyuser.price;
	info.data.sellPrice = model.price;
	info.data.amount = CTool::mutipLongDouble(model.price, model.number);
	info.data.number = model.number;
	info.data.nowTime = CTool::getNowTime();
	CThreadControl::g_listMarketInfo.push_back(info);
}
void CThreadControl::praseMarketInfo(vector<CMatchedDataModel> vecMatchModel)
{
	int iVecSize = vecMatchModel.size();
	for (size_t i = 0; i < iVecSize; i++)
	{
		CMatchedDataModel model = vecMatchModel[i];
		saveMarketInfo(model);
	}
	if (0 != iVecSize)
	{
		for (map<client_pkg_t *, CServerInfo>::iterator itr = CThreadControl::g_mapConnectInfo.begin(); itr != CThreadControl::g_mapConnectInfo.end(); itr++)
		{
			if (true == itr->second.bIsValid)
			{
				UPAllmarketinfo *data = packMarketInfo();

				const int BUFFSIZE = ProtoPackSize<UPAllmarketinfo>(*data);
				UVWriteReq *write = g_cache_ctrlthread_push.Get(BUFFSIZE);
				assert(write != NULL);
				assert(write->buf.base != NULL);
				write->buf.len = ProtoPack<UPAllmarketinfo>(write->buf.base, BUFFSIZE, *data);
				assert(write->buf.len > 0);
				uv_write((uv_write_t *)write, (uv_stream_t *)itr->first, &write->buf, 1, CThreadControl::cbWrited);
				delete data;
				data = NULL;
			}
		}
	}
}
void CThreadControl::RecordEveryTimeKline(CKlineInfoModel klineModel, map<int, CKlineInfoModel> &mapKline, int iRecordTime, int iRecordType)
{
	int startTime = stoi(klineModel.data.startTime);
	if (startTime - mapKline.rbegin()->first <= iRecordTime)
	{
		map<int, CKlineInfoModel>::iterator itr = --mapKline.end();
		itr->second.data.amount = CTool::addLongDouble(itr->second.data.amount, klineModel.data.amount);
		itr->second.data.endPrice = klineModel.data.endPrice;
		if (CTool::minusLongDouble(itr->second.data.highPrice, klineModel.data.highPrice) == "-1")
		{
			itr->second.data.highPrice = klineModel.data.highPrice;
		}
		if (CTool::minusLongDouble(klineModel.data.lowPrice, itr->second.data.lowPrice) == "-1")
		{
			itr->second.data.lowPrice = klineModel.data.lowPrice;
		}
	}
	else
	{
		mapKline.insert(pair<int, CKlineInfoModel>(startTime, klineModel));
		map<int, CKlineInfoModel>::iterator itr = --mapKline.end();

		CKlineInfoModel kline;
		kline.data.startTime = itr->second.data.startTime;
		kline.data.amount = itr->second.data.amount;
		kline.data.startPrice = itr->second.data.startPrice;
		kline.data.highPrice = itr->second.data.highPrice;
		kline.data.lowPrice = itr->second.data.lowPrice;
		kline.data.endPrice = itr->second.data.endPrice;
		recordToFile(kline.getJsonPack(), iRecordType);
	}
}
void CThreadControl::saveKlineData(CKlineInfoModel klineModel)
{
	int startTime = stoi(klineModel.data.startTime);
	if (CThreadControl::g_mapKline.size() == 0)
	{
		CThreadControl::g_mapKline.insert(pair<int, CKlineInfoModel>(startTime, klineModel));
		CThreadControl::g_mapKline5min.insert(pair<int, CKlineInfoModel>(startTime, klineModel));
		CThreadControl::g_mapKline15min.insert(pair<int, CKlineInfoModel>(startTime, klineModel));
		CThreadControl::g_mapKline30min.insert(pair<int, CKlineInfoModel>(startTime, klineModel));
		CThreadControl::g_mapKline1h.insert(pair<int, CKlineInfoModel>(startTime, klineModel));
		CThreadControl::g_mapKline6h.insert(pair<int, CKlineInfoModel>(startTime, klineModel));
		CThreadControl::g_mapKline24h.insert(pair<int, CKlineInfoModel>(startTime, klineModel));
	}
	else
	{
		RecordEveryTimeKline(klineModel, CThreadControl::g_mapKline, 60, TIME_GAP_1_MINUTE);
		RecordEveryTimeKline(klineModel, CThreadControl::g_mapKline5min, 300, TIME_GAP_5_MINUTE);
		RecordEveryTimeKline(klineModel, CThreadControl::g_mapKline15min, 1800, TIME_GAP_15_MINUTE);
		RecordEveryTimeKline(klineModel, CThreadControl::g_mapKline30min, 3600, TIME_GAP_30_MINUTE);
		RecordEveryTimeKline(klineModel, CThreadControl::g_mapKline1h, 7200, TIME_GAP_1_HOUR);
		RecordEveryTimeKline(klineModel, CThreadControl::g_mapKline6h, 7200 * 6, TIME_GAP_6_HOUR);
		RecordEveryTimeKline(klineModel, CThreadControl::g_mapKline24h, 7200 * 24, TIME_GAP_24_HOUR);
	}
}
void CThreadControl::getKlineFromMin(UPKlinedata *data, int count5min)
{
	if (count5min > CThreadControl::g_mapKline5min.size())
	{
		return;
	}

	map<int, CKlineInfoModel>::iterator itr = --CThreadControl::g_mapKline5min.end();
	string startTime, amount, startPrice, highPrice, lowPrice, endPrice;
	endPrice = itr->second.data.endPrice;
	for (size_t i = 0; i < count5min; i++)
	{
		if (i == (count5min - 1))
		{
			startPrice = itr->second.data.startPrice;
			startTime = itr->second.data.startTime;
		}
		amount = CTool::addLongDouble(amount, itr->second.data.amount);
		if (CTool::minusLongDouble(highPrice, itr->second.data.highPrice) == "-1")
		{
			highPrice = itr->second.data.highPrice;
		}
		if (CTool::minusLongDouble(itr->second.data.lowPrice, lowPrice) == "-1")
		{
			lowPrice = itr->second.data.lowPrice;
		}
		itr--;
	}

	// 15分钟k线数据
	UPKlinedata::Data *pData = data->add_data();
	pData->set_k0(startTime);
	pData->set_k1(amount);
	pData->set_k2(startPrice);
	pData->set_k3(highPrice);
	pData->set_k4(lowPrice);
	pData->set_k5(endPrice);
}
UPKlinedata *CThreadControl::packKline()
{
	UPKlinedata *data = new UPKlinedata;
	data->set_type("kline.5m");
	map<int, CKlineInfoModel>::iterator itr = --CThreadControl::g_mapKline.end();
	// 1分钟k线数据
	UPKlinedata::Data *pData = data->add_data();
	pData->set_k0(itr->second.data.startTime);
	pData->set_k1(itr->second.data.amount);
	pData->set_k2(itr->second.data.startPrice);
	pData->set_k3(itr->second.data.highPrice);
	pData->set_k4(itr->second.data.lowPrice);
	pData->set_k5(itr->second.data.endPrice);
	getKlineFromMin(data, 1);
	getKlineFromMin(data, 3);
	getKlineFromMin(data, 6);
	getKlineFromMin(data, 12);
	getKlineFromMin(data, 72);
	getKlineFromMin(data, 288);
	return data;
}
void CThreadControl::recordToFile(string value, const int type)
{
	time_t tCur = CTool::getNowTime();
	string sRelativePath;
	UPDir::KLineFilePath(sRelativePath, Config::recordDir, type, tCur);
	FILE *fp = NULL;
	fp = fopen(sRelativePath.c_str(), "a+");
	if (fp == NULL)
	{
		printf("Open Failure!!!\n");
		return;
	}
	char *pWrite = new char[KLINE_DATA_LEN];
	memset(pWrite,0,KLINE_DATA_LEN);
	strncpy(pWrite,value.c_str(),value.length());
	fwrite(pWrite, KLINE_DATA_LEN, 1, fp);
	fflush(fp);
	fclose(fp);
	delete pWrite;
}
void CThreadControl::praseKlineInfo(vector<CMatchedDataModel> vecMatchModel)
{
	int iVecSize = vecMatchModel.size();
	if (0 != iVecSize)
	{
		int nowTime = CTool::getNowTime();

		CKlineInfoModel klineModel;
		klineModel.data.startTime = std::to_string(nowTime);
		klineModel.data.startPrice = vecMatchModel[0].price;
		klineModel.data.endPrice = vecMatchModel[iVecSize - 1].price;

		for (size_t i = 0; i < iVecSize; i++)
		{
			CMatchedDataModel model = vecMatchModel[i];
			string total = CTool::mutipLongDouble(model.price, model.number);
			klineModel.data.amount = CTool::addLongDouble(klineModel.data.amount, total);
			if (CTool::minusLongDouble(klineModel.data.highPrice, model.price) == "-1")
			{
				klineModel.data.highPrice = model.price;
			}
			if (CTool::minusLongDouble(model.price, klineModel.data.lowPrice) == "-1")
			{
				klineModel.data.lowPrice = model.price;
			}
		}

		saveKlineData(klineModel);

		for (map<client_pkg_t *, CServerInfo>::iterator itr = CThreadControl::g_mapConnectInfo.begin(); itr != CThreadControl::g_mapConnectInfo.end();)
		{
			if (true == itr->second.bIsValid)
			{
				UPKlinedata *data = new UPKlinedata;
				data->set_type("kline.5m");
				// 时时K线数据
				UPKlinedata::Data *pData = data->add_data();
				pData->set_k0(klineModel.data.startTime);
				pData->set_k1(klineModel.data.amount);
				pData->set_k2(klineModel.data.startPrice);
				pData->set_k3(klineModel.data.highPrice);
				pData->set_k4(klineModel.data.lowPrice);
				pData->set_k5(klineModel.data.endPrice);

				const int BUFFSIZE = ProtoPackSize<UPKlinedata>(*data);
				UVWriteReq *write = g_cache_ctrlthread_push.Get(BUFFSIZE);
				write->buf.len = ProtoPack<UPKlinedata>(write->buf.base, BUFFSIZE, *data);
				write->pkg = data;
				uv_write((uv_write_t *)write, (uv_stream_t *)itr->first, &write->buf, 1, CThreadControl::cbWrited);
			}
		}
	}
}
void CThreadControl::praseTradeLog(vector<CMatchedDataModel> vecMatchModel)
{
	if (vecMatchModel.size() != 0)
	{
		int nowTime = CTool::getNowTime();
		for (map<client_pkg_t *, CServerInfo>::iterator itr = CThreadControl::g_mapConnectInfo.begin(); itr != CThreadControl::g_mapConnectInfo.end();)
		{
			if (true == itr->second.bIsValid)
			{
				UPHistoricalTransactionData *data = new UPHistoricalTransactionData;
				data->set_type("tradelog");
				int iVecSize = vecMatchModel.size();
				for (size_t i = 0; i < iVecSize; i++)
				{
					CMatchedDataModel model = vecMatchModel[i];
					UPHistoricalTransactionData::Data *pTrade = data->mutable_data();
					UPHistoricalTransactionData::Data::TradeLog *pData = pTrade->add_tradelog();
					pData->set_tradeid(model.buyuser.orderid + model.selluser.orderid);
					pData->set_buyorderid(model.buyuser.orderid);
					pData->set_sellorderid(model.selluser.orderid);
					pData->set_addtime(std::to_string(nowTime));
					pData->set_timestamp(std::to_string(nowTime));
					pData->set_type(1);
					pData->set_price(model.price);
					pData->set_mum(CTool::mutipLongDouble(model.price, model.number));
					pData->set_num(model.number);
				}

				const int BUFFSIZE = ProtoPackSize<UPHistoricalTransactionData>(*data);
				UVWriteReq *write = g_cache_ctrlthread_push.Get(BUFFSIZE);
				assert(write != NULL);
				assert(write->buf.base != NULL);
				write->buf.len = ProtoPack<UPHistoricalTransactionData>(write->buf.base, BUFFSIZE, *data);
				assert(write->buf.len > 0);
				uv_write((uv_write_t *)write, (uv_stream_t *)itr->first, &write->buf, 1, CThreadControl::cbWrited);
				delete data;
				data = NULL;
			}
		}
	}
}
void CThreadControl::praseMatchSucc(vector<CMatchedDataModel> vecMatchModel)
{
	if (vecMatchModel.size() != 0)
	{
		for (map<client_pkg_t *, CServerInfo>::iterator itr = CThreadControl::g_mapConnectInfo.begin(); itr != CThreadControl::g_mapConnectInfo.end();)
		{
			if (true == itr->second.bIsValid)
			{
				int iVecSize = vecMatchModel.size();
				for (size_t i = 0; i < iVecSize; i++)
				{
					CMatchedDataModel model = vecMatchModel[i];
					UPMatchedData *pkg = new UPMatchedData;
					pkg->set_price(model.price);
					pkg->set_number(model.number);
					pkg->set_token(model.token);
					UPMatchedData::BuyUser *BuyUser = pkg->mutable_buyuser();
					BuyUser->set_userid(model.buyuser.userid);
					BuyUser->set_orderid(model.buyuser.orderid);
					BuyUser->set_buyprice(model.buyuser.price);
					UPMatchedData::SellUser *SellUser = pkg->mutable_selluser();
					SellUser->set_userid(model.selluser.userid);
					SellUser->set_orderid(model.selluser.orderid);
					SellUser->set_sellprice(model.selluser.price);
					const int BUFFSIZE = ProtoPackSize<UPMatchedData>(*pkg);
					UVWriteReq *write = g_cache_ctrlthread_push.Get(BUFFSIZE);
					assert(write != NULL);
					assert(write->buf.base != NULL);
					write->buf.len = ProtoPack<UPMatchedData>(write->buf.base, BUFFSIZE, *pkg);
					assert(write->buf.len > 0);
					uv_write((uv_write_t *)write, (uv_stream_t *)itr->first, &write->buf, 1, CThreadControl::cbWrited);
					delete pkg;
					pkg = NULL;
				}
			}
		}
	}
}
void CThreadControl::cbTimer(uv_timer_t *handle)
{
	/*
	* frontmarket请求k线历史数据，返回文件内容
	*/
	vector<CBaseSession *> vecResp;
	CThreadControl::g_qRecvReqQueue.get(vecResp);
	int iVecSize = vecResp.size();
	for (size_t i = 0; i < iVecSize; i++)
	{
		CBaseSession *session = vecResp[i];
		UPFileData *data = session->rePackFilePkg();
		const int BUFFSIZE = ProtoPackSize<UPFileData>(*data);
		UVWriteReq *write = g_cache_ctrlthread_push.Get(BUFFSIZE);
		write->buf.len = ProtoPack<UPFileData>(write->buf.base, BUFFSIZE, *data);
		write->pkg = data;
		uv_write((uv_write_t *)write, (uv_stream_t *)session->getClientFd(), &write->buf, 1, CThreadControl::cbWrited);
		delete session;
	}

	/*
	* 下单/撤单引起的数据修改和广播操作
	*/
	CThreadControl::m_qContrlQueue.get(CThreadControl::g_vecSessionRealse);
	businessPushOper(CThreadControl::g_vecSessionRealse);
	for (size_t i = 0; i < CThreadControl::g_vecSessionRealse.size(); i++)
	{
		CBaseModel *session = CThreadControl::g_vecSessionRealse[i];
		CThreadControl::g_vecSessionRealse.erase(CThreadControl::g_vecSessionRealse.begin() + i);
		delete session;
		session = NULL;
	}

	/*
	* 撮合成功，更新广播市场信息，k线信息，交易日至信息，和撮合成功信息
	*/
	vector<CMatchedDataModel> vecMatchModel;
	CThreadControl::m_qControlMatchQueue.get(vecMatchModel);
	praseMarketInfo(vecMatchModel);
	praseKlineInfo(vecMatchModel);
	praseTradeLog(vecMatchModel);
	praseMatchSucc(vecMatchModel);
}

void CThreadControl::businessPushOper(vector<CBaseModel *> &vecPushFrontMarket)
{
	for (size_t i = 0; i < vecPushFrontMarket.size(); i++)
	{
		CBaseModel *session = vecPushFrontMarket[i];
		for (map<client_pkg_t *, CServerInfo>::iterator itr = CThreadControl::g_mapConnectInfo.begin(); itr != CThreadControl::g_mapConnectInfo.end();)
		{
			if (true == itr->second.bIsValid)
			{
				switch (session->modelType)
				{
				case OPER_TYPE_UPTRADE:
				{
					savePriceAndPush(session, itr->first, true);
				}
				break;
				case UPCanceltrade::CMD:
				{
					savePriceAndPush(session, itr->first, false);
				}
				break;
				default:
					break;
				}
			}
			itr++;
		}
	}
}
void CThreadControl::cbClosed(uv_handle_t *handle)
{
	// 重新链接此socket
	uv_thread_t *p_connThread = new uv_thread_t;
	CThreadControl::g_mapThread.insert(pair<client_pkg_t *, uv_thread_t *>((client_pkg_t *)handle, p_connThread));
	uv_thread_create(p_connThread, CThreadControl::reConnect, handle);
}
void CThreadControl::cbWrited(uv_write_t *req, int status)
{
	g_cache_ctrlthread_push.Free((UVWriteReq *)req);
}
void CThreadControl::reConnect(void *arg)
{
	//用户中心服务器连接不存在，backtrade每隔iSleepTime秒后重连服务器
	client_pkg_t *client = (client_pkg_t *)arg;

	map<client_pkg_t *, uv_thread_t *>::iterator itrThread = CThreadControl::g_mapThread.find(client);
	assert(itrThread != CThreadControl::g_mapThread.end());
	if (NULL != itrThread->second)
	{
		delete itrThread->second;
		itrThread->second = NULL;
	}
	CThreadControl::g_mapThread.erase(itrThread);

	int iSleepTime = 5;

	sleep(iSleepTime);
	struct sockaddr_in addr;
	CServerInfo server_info;
	client_pkg_t *server = new client_pkg_t;
	server->ulPkgIndex = CDispatchManager::g_ulControlIndex++;
	server_info.conn = new uv_connect_t;

	map<client_pkg_t *, CServerInfo>::iterator itr = CThreadControl::g_mapConnectInfo.find(client);
	assert(itr != CThreadControl::g_mapConnectInfo.end());

	server_info.info = itr->second.info;

	uv_tcp_init(&THIS_LOOP, (uv_tcp_t *)server);
	uv_ip4_addr(server_info.info.m_sConnectIP.c_str(), server_info.info.m_usConnectPort, &addr);
	uv_tcp_connect(server_info.conn, server, (const sockaddr *)&addr, cbConnect);
	cout << "[CThreadControll::cbConnect] after " << iSleepTime << " seconds reconnect sock = " << CDispatchManager::g_ulControlIndex << " server port = " << server_info.info.m_usConnectPort << endl;
	if (NULL != itr->second.conn)
	{
		delete itr->second.conn;
		itr->second.conn = NULL;
	}

	if (NULL != itr->first)
	{
		delete itr->first;
	}

	CThreadControl::g_mapConnectInfo.erase(itr);
	CThreadControl::g_mapConnectInfo.insert(pair<client_pkg_t *, CServerInfo>(server, server_info));
}

void CThreadControl::cbReadBuff(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
{
#ifdef PRINT_LOG
	cout << "[CThreadControl->cbNewConnection->cbReadBuff] Malloc Buffer" << endl;
#endif
	buf->base = g_cache_ctrlthread_read.Get(SIZE_BUFFER_8k);
	buf->len = SIZE_BUFFER_8k;
}

void CThreadControl::cbRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf)
{
	if (nread < 0)
	{
#ifdef PRINT_LOG
		cout << "[CThreadControl->cbNewConnection->cbRead] socket disconnect" << endl;
#endif
		map<client_pkg_t *, CServerInfo>::iterator itr = CThreadControl::g_mapConnectInfo.find((client_pkg_t *)client);
		itr->second.bIsValid = false;
		uv_close((uv_handle_t *)client, cbClosed);
	}
	else if (nread == 0)
	{
#ifdef PRINT_LOG
		cout << "[CThreadControl->cbNewConnection->cbRead] read noting" << endl;
#endif
		uv_close((uv_handle_t *)client, cbClosed);
	}
	else
	{
		int iOperRet = CDispatchManager::DispatchClient(CDispatchManager::g_mapFrontMarketSession, (client_pkg_t *)client, buf->base, nread, false);
		if (RET_SUCCESS == iOperRet)
		{
			mapBaseSession::iterator itr = CDispatchManager::g_mapFrontMarketSession.find((client_pkg_t *)client);
			assert(itr != CDispatchManager::g_mapFrontMarketSession.end());
			CThreadControl::g_qRecvReqQueue.put(itr->second);
			CDispatchManager::g_mapFrontMarketSession.erase(itr++);
		}
		else if (RET_FAILED == iOperRet)
		{
			uv_close((uv_handle_t *)client, cbClosed);
		}
		else
		{
		}
	}
	g_cache_ctrlthread_read.Free(buf->base, buf->len);
}
void CThreadControl::rsaLoginServer(client_pkg_t *client)
{
	Json::Value sJson;
	sJson["mid"] = Config::uiMarketID;
	sJson["mn"] = Config::sMarket.c_str();
	UPLogin *lg = new UPLogin;
	lg->ClientRequest(Config::sRSAFrontTradePriKey, sJson);

	const int BUFFSIZE = ProtoPackSize<UPLogin>(*lg);
	UVWriteReq *write = g_cache_ctrlthread_login.Get(BUFFSIZE);
	assert(write != NULL);
	assert(write->buf.base != NULL);
	write->buf.len = ProtoPack<UPLogin>(write->buf.base, BUFFSIZE, *lg);

	assert(write->buf.len > 0);
	uv_write((uv_write_t *)write, (uv_stream_t *)client, &write->buf, 1, CThreadControl::cbLogin);

	map<client_pkg_t *, CServerInfo>::iterator itr = CThreadControl::g_mapConnectInfo.find((client_pkg_t *)client);
	itr->second.bIsValid = true;
}
void CThreadControl::cbConnect(uv_connect_t *req, int status)
{
	if (status == 0)
	{
		client_pkg_t *client = (client_pkg_t *)req->handle;
		int ret = uv_read_start((uv_stream_t *)client, cbReadBuff, cbRead);
		cout << "[CThreadControl::cbConnect] connect FrontMarket success index = " << client->ulPkgIndex << " ret code = " << ret << endl;
		rsaLoginServer(client);
	}
	else
	{
		uv_thread_t *p_connThread = new uv_thread_t;
		CThreadControl::g_mapThread.insert(pair<client_pkg_t *, uv_thread_t *>((client_pkg_t *)req->handle, p_connThread));
		uv_thread_create(p_connThread, CThreadControl::reConnect, req->handle);
	}
}

void CThreadControl::cbLogin(uv_write_t *req, int status)
{
	g_cache_ctrlthread_login.Free((UVWriteReq *)req);
}

void CThreadControl::ThreadContrl(void *arg)
{
	spdlog::info("[CThreadControl Init]");
	uv_loop_init(&THIS_LOOP);
	g_cache_ctrlthread_read.Init(10, 0, 0);
	g_cache_ctrlthread_login.Init(10, 0, 0);
	g_cache_ctrlthread_push.Init(SIZE_BUFFER_2k, 0, 0);
	for (int iServerCount = 0; iServerCount < Config::g_vecConnectMarkets.size(); iServerCount++)
	{
		CServerInfo server_info;
		struct sockaddr_in addr;
		client_pkg_t *server = new client_pkg_t;
		server_info.conn = new uv_connect_t;
		server_info.info = Config::g_vecConnectMarkets[iServerCount];
		server_info.bIsValid = false;
		server->ulPkgIndex = CDispatchManager::g_ulControlIndex++;

		uv_tcp_init(&THIS_LOOP, (uv_tcp_t *)server);
		uv_ip4_addr(server_info.info.m_sConnectIP.c_str(), server_info.info.m_usConnectPort, &addr);
		int ret = uv_tcp_connect(server_info.conn, server, (const sockaddr *)&addr, cbConnect);

		cout << "[CThreadControl Client connect code]:" << uv_strerror(ret) << endl;
		cout << "[CThreadControl Client connect] IP = " << server_info.info.m_sConnectIP << endl;
		cout << "[CThreadControl Client connect] PORT = " << server_info.info.m_usConnectPort << endl;
		cout << "[CThreadControl Client connect] connect index = " << server->ulPkgIndex << endl;
		CThreadControl::g_mapConnectInfo.insert(pair<client_pkg_t *, CServerInfo>(server, server_info));
		cout << "[CThreadControl] Connect code :" << uv_strerror(ret) << endl;
	}

	uv_timer_init(&THIS_LOOP, &THIS_TIMER);
	uv_timer_start(&THIS_TIMER, cbTimer, 10, 1);
	uv_run(&THIS_LOOP, UV_RUN_DEFAULT);
}
