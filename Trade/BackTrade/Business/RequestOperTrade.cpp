#include "RequestOperTrade.h"

CRequestOperTrade::CRequestOperTrade(CTradeModel model)
{
    m_mTradeModel = model;
}
string CRequestOperTrade::getRequestToken()
{
    return m_mTradeModel.token;
}

int CRequestOperTrade::requestOperation()
{
    int iRet = RET_SUCCESS;
    // if (m_mTradeModel.uid.length() == 0 || m_mTradeModel.marketid == 0 || m_mTradeModel.price.length() == 0 || m_mTradeModel.num.length() == 0)
    // {
    //     iRet = RET_FAILED;
    // }
    // else
    // {
    //     if (m_mTradeModel.type == 1 || m_mTradeModel.type == 2)
    //     {
    //         string strBase = "SELECT fee_buy,fee_sell from coin_market where id = " + std::to_string(m_mTradeModel.marketid);
    //         CDispatchManager::g_DBTrade.Sql(strBase,false,true);
    //         CDispatchManager::g_DBTrade.Sql(";",false,false);
    //         DEBUG_INFO(CDispatchManager::g_DBTrade.getSql());
    //         if (CDispatchManager::g_DBTrade.Select())
    //         {
    //             if (CDispatchManager::g_DBTrade.NextRow())
    //             {
    //                 string fee_buy, fee_sell, total, estimated_fee;
    //                 int iNowTime = CDispatchManager::getNowTime();
    //                 CDispatchManager::g_DBTrade.Value(0, fee_buy);
    //                 CDispatchManager::g_DBTrade.Value(1, fee_sell);
    //                 fee_buy = CDispatchManager::mutipLongDouble(fee_buy, "0.01");
    //                 fee_sell = CDispatchManager::mutipLongDouble(fee_sell, "0.01");
    //                 total = CDispatchManager::mutipLongDouble(m_mTradeModel.price, m_mTradeModel.num);
    //                 if (m_mTradeModel.type == 1)
    //                 {
    //                     estimated_fee = CDispatchManager::mutipLongDouble(total, fee_buy);
    //                 }
    //                 else
    //                 {
    //                     estimated_fee = CDispatchManager::mutipLongDouble(total, fee_sell);
    //                 }
    //                 string marketid = std::to_string(m_mTradeModel.marketid);
    //                 string tableName = " wait_" + marketid + " ";
    //                 string sqlBase = "INSERT INTO " + tableName + "(order_id,user_id,market_id,price,trading_volume,traded_volume,trading_amount,estimated_fee,actual_fee,oper_type,status,endtime,create_time,update_time) VALUES (\"";
    //                 string oid = std::to_string(m_mTradeModel.orderid);
    //                 CDispatchManager::g_DBTrade.Sql(sqlBase, false, true);
    //                 CDispatchManager::g_DBTrade.Sql(oid, false, false);
    //                 CDispatchManager::g_DBTrade.Sql("\",\"", false, false);
    //                 CDispatchManager::g_DBTrade.Sql(m_mTradeModel.uid, false, false);
    //                 CDispatchManager::g_DBTrade.Sql("\",\"", false, false);
    //                 CDispatchManager::g_DBTrade.Sql(marketid, true, false);
    //                 CDispatchManager::g_DBTrade.Sql("\",\"", false, false);
    //                 CDispatchManager::g_DBTrade.Sql(m_mTradeModel.price, true, false);
    //                 CDispatchManager::g_DBTrade.Sql("\",\"", false, false);
    //                 CDispatchManager::g_DBTrade.Sql(m_mTradeModel.num, true, false);
    //                 CDispatchManager::g_DBTrade.Sql("\",0,\"", false, false);
    //                 string trading_amount = CDispatchManager::mutipLongDouble(m_mTradeModel.price,m_mTradeModel.num);
    //                 CDispatchManager::g_DBTrade.Sql(trading_amount, true, false);
    //                 CDispatchManager::g_DBTrade.Sql("\",\"", false, false);
    //                 CDispatchManager::g_DBTrade.Sql(estimated_fee, true, false);
    //                 CDispatchManager::g_DBTrade.Sql("\",0,", false, false);
    //                 string strType = std::to_string(m_mTradeModel.type);
    //                 CDispatchManager::g_DBTrade.Sql(strType, true, false);
    //                 CDispatchManager::g_DBTrade.Sql(",0,", false, false);
    //                 string strTime = std::to_string(iNowTime);
    //                 CDispatchManager::g_DBTrade.Sql(strTime, true, false);
    //                 CDispatchManager::g_DBTrade.Sql(",", false, false);
    //                 CDispatchManager::g_DBTrade.Sql(strTime, true, false);
    //                 CDispatchManager::g_DBTrade.Sql(",", false, false);
    //                 CDispatchManager::g_DBTrade.Sql(strTime, true, false);
    //                 CDispatchManager::g_DBTrade.Sql(");", false, false);
    //                 DEBUG_INFO(CDispatchManager::g_DBTrade.getSql());
    //                 CDispatchManager::g_DBTrade.Exec();
    //             }
    //             else
    //             {
    //                 iRet = RET_FAILED;
    //             }
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
    // }
    return iRet;
}