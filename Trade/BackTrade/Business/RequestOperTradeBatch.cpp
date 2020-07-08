/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-21 23:27:31
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/Business/RequestOperTradeBatch.cpp
 */
#include "RequestOperTradeBatch.h"

CRequestOperTradeBatch::CRequestOperTradeBatch(CTradeBatchModel model)
{
    m_mTradeBatch = model;
}
string CRequestOperTradeBatch::getRequestToken()
{
    return m_mTradeBatch.token;
}

// int CRequestOperTradeBatch::operDB(CTradeInfoModel model, string uid)
// {
    // if (model.type == 1 || model.type == 2)
    // {
    //     string marketid = std::to_string(model.marketid);
    //     string strBase = "SELECT fee_buy,fee_sell from coin_market where id = \"" + marketid + "\";";
    //     CDispatchManager::g_DBTrade.Sql(strBase, false, true);
    //     if (CDispatchManager::g_DBTrade.Select())
    //     {
    //         if (CDispatchManager::g_DBTrade.NextRow())
    //         {
    //             string fee_buy, fee_sell, total, estimated_fee;
    //             int iNowTime = CDispatchManager::getNowTime();
    //             CDispatchManager::g_DBTrade.Value(0, fee_buy);
    //             CDispatchManager::g_DBTrade.Value(1, fee_sell);
    //             fee_buy = CDispatchManager::mutipLongDouble(fee_buy, "0.01");
    //             fee_sell = CDispatchManager::mutipLongDouble(fee_sell, "0.01");
    //             total = CDispatchManager::mutipLongDouble(model.price, model.num);
    //             if (model.type == 1)
    //             {
    //                 estimated_fee = CDispatchManager::mutipLongDouble(total, fee_buy);
    //             }
    //             else
    //             {
    //                 estimated_fee = CDispatchManager::mutipLongDouble(total, fee_sell);
    //             }

    //             string tableName = " wait_" + marketid + " ";
    //             string sqlBase = "INSERT INTO " + tableName + "(order_id,user_id,market_id,price,trading_volume,traded_volume,trading_amount,estimated_fee,actual_fee,oper_type,status,endtime,create_time,update_time) VALUES (\"";
    //             string oid = std::to_string(model.orderid);
    //             CDispatchManager::g_DBTrade.Sql(sqlBase, false, true);
    //             CDispatchManager::g_DBTrade.Sql(oid, false, false);
    //             CDispatchManager::g_DBTrade.Sql("\",\"", false, false);
    //             CDispatchManager::g_DBTrade.Sql(uid, false, false);
    //             CDispatchManager::g_DBTrade.Sql("\",\"", false, false);
    //             CDispatchManager::g_DBTrade.Sql(marketid, true, false);
    //             CDispatchManager::g_DBTrade.Sql("\",\"", false, false);
    //             CDispatchManager::g_DBTrade.Sql(model.price, true, false);
    //             CDispatchManager::g_DBTrade.Sql("\",\"", false, false);
    //             CDispatchManager::g_DBTrade.Sql(model.num, true, false);
    //             CDispatchManager::g_DBTrade.Sql("\",0,0,\"", false, false);
    //             CDispatchManager::g_DBTrade.Sql(estimated_fee, true, false);
    //             CDispatchManager::g_DBTrade.Sql("\",0,", false, false);
    //             string strType = std::to_string(model.type);
    //             CDispatchManager::g_DBTrade.Sql(strType, true, false);
    //             CDispatchManager::g_DBTrade.Sql(",0,", false, false);
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
    //     }
    // }
// }

int CRequestOperTradeBatch::requestOperation()
{
    int iRet = RET_SUCCESS;
    // if (m_mTradeBatch.uid.length() == 0)
    // {
    //     iRet = RET_FAILED;
    // }
    // else
    // {
    //     for (size_t i = 0; i < m_mTradeBatch.trades.size(); i++)
    //     {
    //         CTradeInfoModel info = m_mTradeBatch.trades[i];
    //         // operDB(info, m_mTradeBatch.uid);
    //     }
    // }
    return iRet;
}