/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-07-03 01:26:13
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/ThreadMatch.h
 */
#ifndef _THREAD_MATCH_HEADER_
#define _THREAD_MATCH_HEADER_

#include "global.h"
#include "DispatchManager.h"
#include "./Dto/UPJsonReflex.hpp"
#include "CancelTradeBatchModel.hpp"
#include "CancelTradeModel.hpp"
#include "TradeBatchModel.hpp"
#include "TradeModel.hpp"

extern uv_loop_t g_uvMatchLoop;
extern uv_timer_t g_uvMatchTimer;

class StringCMP
{
public:
	bool operator()(string const &A, string const &B) const
	{
		string value2 = CTool::minusLongDouble(A, B);
		return value2 == "-1"?true:false;
	}
};

typedef vector<CTradeModel> vecMatchData;   // 相同时间的数据队列
typedef map<int32_t,vecMatchData> mapSortForTime; // 以时间先后排序的队列
typedef map<string,mapSortForTime,StringCMP> mapSortForPrice; // 以价格先后排序的队列
class CThreadMatch
{
public:
    CThreadMatch();
    ~CThreadMatch();
    static void ThreadMatch(void *arg);
    static void cbTimer(uv_timer_t *handle);
    static int RecordTrade(CTradeModel tradeData,mapSortForPrice& mapInsert);
    static int RemoveTrade(CCancelTradeModel& tradeData,mapSortForPrice& mapRemove);
    static int BatRecordTrade(CTradeBatchModel model);
    static int BatRemoveTrade(CCancelTradeBatchModel& model,vector<uint64_t>& vecCancelFailed);
    static int getOrderInfo(CGetOrderInfoModel& model);
    static int dispatchOperNodes(CBaseModel *pModel);
    static void matchBuySellQue(vector<CMatchedDataModel>& vecRet);
    static void recordToFile(vector<CMatchedDataModel> &vecModel,vector<UVWriteReq *>& vecWrite);
    static bool ClearOrderMap(vecMatchData& vecMatch);
    static void assetsListSet(UPassetsupdate::List *pList, uint64_t uid, uint64_t type_id, ASSETS_UPDATE oper_type, string coinid, string usable, string fronzen);
public:
    static UBBlockQueue<CBaseModel> m_qUCToMTQueue;
    static UBBlockQueue<CBaseModel> m_qUCToMTRespQueue;
    static UBBlockObjQueue<CMatchedDataModel> m_qMTToDBQueue;
    static UBBlockQueue<UVWriteReq> m_qMTToUCQueue;
private:
    static map<int,CTradeModel> m_mapOrderInfo;
    static vector<CTradeModel> g_vecOrder;
    static mapSortForPrice m_mapMatchBuy;
    static mapSortForPrice m_mapMatchSell;
};

#endif
