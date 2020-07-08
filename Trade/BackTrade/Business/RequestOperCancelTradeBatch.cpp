/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-21 23:26:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/Business/RequestOperCancelTradeBatch.cpp
 */
#include "RequestOperCancelTradeBatch.h"
CRequestOperCancelTradeBatch::CRequestOperCancelTradeBatch(CCancelTradeBatchModel model)
{
    m_mCancelTradeBatch = model;
}
string CRequestOperCancelTradeBatch::getRequestToken()
{
    return m_mCancelTradeBatch.token;
}
// int CRequestOperCancelTradeBatch::operDB(CCancelTradeInfoModel model, string uid)
// {
//     int iRet = RET_SUCCESS;
//     string tableName = " wait_" + std::to_string(Config::uiMarketID) + " ";
//     string strBase = "SELECT price,trading_volume,traded_volume,trading_amount,estimated_fee,actual_fee,oper_type from " + tableName + " where order_id = \"" + std::to_string(model.orderid) + "\" order by traded_volume desc;";
//     CDispatchManager::g_DBTrade.Sql(strBase, false, true);
//     if (CDispatchManager::g_DBTrade.Select())
//     {
//         if (CDispatchManager::g_DBTrade.NextRow())
//         {
//             string price, totalNum, tradedNum, tradeAmount, estimated_fee, actual_fee, oper_type;
//             int iNowTime = CDispatchManager::getNowTime();
//             CDispatchManager::g_DBTrade.Value(0, price);
//             CDispatchManager::g_DBTrade.Value(1, totalNum);
//             CDispatchManager::g_DBTrade.Value(2, tradedNum);
//             CDispatchManager::g_DBTrade.Value(3, tradeAmount);
//             CDispatchManager::g_DBTrade.Value(4, estimated_fee);
//             CDispatchManager::g_DBTrade.Value(5, actual_fee);
//             CDispatchManager::g_DBTrade.Value(6, oper_type);

//             string tableClose = " close_" + std::to_string(Config::uiMarketID) + " ";
//             string sqlBase = "INSERT INTO " + tableClose + "(order_id,user_id,market_id,price,trading_volume,traded_volume,trading_amount,estimated_fee,actual_fee,oper_type,status,endtime,create_time,update_time) VALUES (\"";
//             string oid = std::to_string(model.orderid);
//             CDispatchManager::g_DBTrade.Sql(oid, false, true);
//             CDispatchManager::g_DBTrade.Sql("\",\"", false, false);
//             CDispatchManager::g_DBTrade.Sql(uid, false, true);
//             CDispatchManager::g_DBTrade.Sql("\",\"", false, false);
//             string strMarket = std::to_string(Config::uiMarketID);
//             CDispatchManager::g_DBTrade.Sql(strMarket, true, false);
//             CDispatchManager::g_DBTrade.Sql("\",\"", false, false);
//             CDispatchManager::g_DBTrade.Sql(price, true, false);
//             CDispatchManager::g_DBTrade.Sql("\",\"", false, false);
//             CDispatchManager::g_DBTrade.Sql(totalNum, true, false);
//             CDispatchManager::g_DBTrade.Sql("\",\"", false, false);
//             CDispatchManager::g_DBTrade.Sql(tradedNum, true, false);
//             CDispatchManager::g_DBTrade.Sql("\",\"", false, false);
//             CDispatchManager::g_DBTrade.Sql(tradeAmount, true, false);
//             CDispatchManager::g_DBTrade.Sql("\",\"", false, false);
//             CDispatchManager::g_DBTrade.Sql(estimated_fee, true, false);
//             CDispatchManager::g_DBTrade.Sql("\",\"", false, false);
//             CDispatchManager::g_DBTrade.Sql(actual_fee, true, false);
//             CDispatchManager::g_DBTrade.Sql("\",", false, false);
//             CDispatchManager::g_DBTrade.Sql(oper_type, true, false);
//             CDispatchManager::g_DBTrade.Sql(",2,", false, false); // 2为撤单
//             string strTime = std::to_string(iNowTime);
//             CDispatchManager::g_DBTrade.Sql(strTime, true, false);
//             CDispatchManager::g_DBTrade.Sql(",", false, false);
//             CDispatchManager::g_DBTrade.Sql(strTime, true, false);
//             CDispatchManager::g_DBTrade.Sql(",", false, false);
//             CDispatchManager::g_DBTrade.Sql(strTime, true, false);
//             CDispatchManager::g_DBTrade.Sql(");", false, false);
//             DEBUG_INFO(CDispatchManager::g_DBTrade.getSql());
//             CDispatchManager::g_DBTrade.Exec();
//         }
//         else
//         {
//             iRet = RET_FAILED;
//         }
//     }
//     else
//     {
//         iRet = RET_FAILED;
//     }
//     return iRet;
// }
int CRequestOperCancelTradeBatch::requestOperation()
{
    int iRet = RET_SUCCESS;
    if (m_mCancelTradeBatch.uid == 0 || m_mCancelTradeBatch.canceltradebatchid == 0)
    {
        iRet = RET_FAILED;
    }
    else
    {
        for (size_t i = 0; i < m_mCancelTradeBatch.orders.size(); i++)
        {
            CCancelTradeInfoModel info = m_mCancelTradeBatch.orders[i];
            // operDB(info, std::to_string(m_mCancelTradeBatch.uid));
        }
    }
    return iRet;
}