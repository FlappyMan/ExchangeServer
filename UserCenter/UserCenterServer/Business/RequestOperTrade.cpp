/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-18 22:56:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Business/RequestOperTrade.cpp
 */
#include "RequestOperTrade.h"

CRequestOperTrade::CRequestOperTrade(CTradeModel model)
{
    m_mTradeModel = model;
}
// int CRequestOperTrade::operDB(string coinId)
// {
//     int iRet = RET_SUCCESS;
//     string tableName = " user_assets_" + coinId + " ";
//     string sqlBase = "select usable,freeze from " + tableName + " where user_id = \"";
//     CDispatchManager::g_DBAssets.Sql(sqlBase, false, true);
//     CDispatchManager::g_DBAssets.Sql(m_mTradeModel.uid, true, false);
//     CDispatchManager::g_DBAssets.Sql("\";", false, false);
//     DEBUG_INFO(CDispatchManager::g_DBAssets.getSql());
//     if (CDispatchManager::g_DBAssets.Select())
//     {
//         if (CDispatchManager::g_DBAssets.NextRow())
//         {
//             string usable, freeze, total;
//             CDispatchManager::g_DBAssets.Value(0, usable);
//             CDispatchManager::g_DBAssets.Value(1, freeze);
//             total = CDispatchManager::mutipLongDouble(m_mTradeModel.price, m_mTradeModel.num);
//             string strBalanceUsable = CDispatchManager::minusLongDouble(usable, total);
//             if (strBalanceUsable == "-1")
//             {
//                 iRet = RET_FAILED;
//             }
//             else
//             {
//                 string strBalanceFreeze = CDispatchManager::addLongDouble(freeze, total);
//                 CDispatchManager::g_DBAssets.Sql("UPDATE ", false, true);
//                 CDispatchManager::g_DBAssets.Sql(tableName, true, false);
//                 CDispatchManager::g_DBAssets.Sql(" SET usable = \"", false, false);
//                 CDispatchManager::g_DBAssets.Sql(strBalanceUsable, true, false);
//                 CDispatchManager::g_DBAssets.Sql("\", freeze = \"", false, false);
//                 CDispatchManager::g_DBAssets.Sql(strBalanceFreeze, true, false);
//                 CDispatchManager::g_DBAssets.Sql("\" where status = 1 and user_id = \"", false, false);
//                 CDispatchManager::g_DBAssets.Sql(m_mTradeModel.uid, true, false);
//                 CDispatchManager::g_DBAssets.Sql("\";", false, false);
//                 DEBUG_INFO(CDispatchManager::g_DBAssets.getSql());
//                 if (CDispatchManager::g_DBAssets.Exec() < 0)
//                 {
//                     iRet = RET_FAILED;
//                 }
//                 else
//                 {
//                     string nowTime = std::to_string(CDispatchManager::getNowTime());
//                     string fee = total;
//                     string fee_frozen = "-"+total;
//                     string type = std::to_string(m_mTradeModel.type);
//                     string oper_id = nowTime;
//                     string oper_desc = "MarketID="+std::to_string(m_marketID) + (m_mTradeModel.type == 1?"买入冻结":"卖出冻结") + "CoinID="+coinId;
//                     string insertFinancial = "INSERT INTO financial_records(user_id,coin_id,gross_amount,gross_frozen,gross_sum,fee,fee_frozen,oper_type,oper_id,oper_desc,remaining_amount,remaining_frozen,remaining_sum,record_hash,status,create_time,update_time) VALUES (\"";
//                     CDispatchManager::g_DBSA.Sql(insertFinancial, false, true);
//                     CDispatchManager::g_DBSA.Sql(m_mTradeModel.uid, false, false);
//                     CDispatchManager::g_DBSA.Sql("\",\"", false, false);
//                     CDispatchManager::g_DBSA.Sql(coinId, true, false);
//                     CDispatchManager::g_DBSA.Sql("\",\"", false, false);
//                     CDispatchManager::g_DBSA.Sql(usable, true, false);
//                     CDispatchManager::g_DBSA.Sql("\",\"", false, false);
//                     CDispatchManager::g_DBSA.Sql(freeze, true, false);
//                     string beTotal = CDispatchManager::addLongDouble(usable,freeze);
//                     CDispatchManager::g_DBSA.Sql("\",\"", false, false);
//                     CDispatchManager::g_DBSA.Sql(beTotal, true, false);
//                     CDispatchManager::g_DBSA.Sql("\",\"", false, false);
//                     CDispatchManager::g_DBSA.Sql(fee, true, false);
//                     CDispatchManager::g_DBSA.Sql("\",\"", false, false);
//                     CDispatchManager::g_DBSA.Sql(fee_frozen, true, false);
//                     CDispatchManager::g_DBSA.Sql("\",\"", false, false);
//                     CDispatchManager::g_DBSA.Sql(type, true, false);
//                     CDispatchManager::g_DBSA.Sql("\",\"", false, false);
//                     CDispatchManager::g_DBSA.Sql(oper_id, true, false);
//                     CDispatchManager::g_DBSA.Sql("\",\"", false, false);
//                     CDispatchManager::g_DBSA.Sql(oper_desc, true, false);
//                     CDispatchManager::g_DBSA.Sql("\",\"", false, false);
//                     CDispatchManager::g_DBSA.Sql(strBalanceUsable, true, false);
//                     CDispatchManager::g_DBSA.Sql("\",\"", false, false);
//                     CDispatchManager::g_DBSA.Sql(strBalanceFreeze, true, false);
//                     CDispatchManager::g_DBSA.Sql("\",\"", false, false);
//                     CDispatchManager::g_DBSA.Sql(beTotal, true, false);
//                     CDispatchManager::g_DBSA.Sql("\",\"hash\",1,\"", false, false);
//                     CDispatchManager::g_DBSA.Sql(nowTime, false, false);
//                     CDispatchManager::g_DBSA.Sql("\",\"", false, false);
//                     CDispatchManager::g_DBSA.Sql(nowTime, false, false);
//                     CDispatchManager::g_DBSA.Sql("\");", false, false);
//                     DEBUG_INFO(CDispatchManager::g_DBSA.getSql());
//                     if (CDispatchManager::g_DBSA.Exec() < 0)
//                     {
//                         iRet = RET_FAILED;
//                     }
//                 }
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
//     return iRet;
// }
/*
* 往wait_coin表中插入一天数据记录
*/
UProtocolBase *CRequestOperTrade::requestOperation()
{
    int iStatus = 0;
    string retMsg = "";
    // if (m_mTradeModel.paypassword.length() == 0 || m_mTradeModel.marketid == 0 || m_mTradeModel.price.length() == 0 || m_mTradeModel.num.length() == 0)
    // {
    //     iStatus = 0;
    //     retMsg = "参数缺失";
    // }
    // else
    // {
    //     CDispatchManager::g_DBAccount.Sql("select user_id from user_info where paypassword = \"", false, true);
    //     CDispatchManager::g_DBAccount.Sql(m_mTradeModel.paypassword, true, false);
    //     CDispatchManager::g_DBAccount.Sql("\" and user_id = \"", false, false);
    //     CDispatchManager::g_DBAccount.Sql(m_mTradeModel.uid, true, false);
    //     CDispatchManager::g_DBAccount.Sql("\";", false, false);
    //     DEBUG_INFO(CDispatchManager::g_DBAccount.getSql());
    //     // 1-验证交易密码
    //     if (CDispatchManager::g_DBAccount.Select())
    //     {
    //         if (CDispatchManager::g_DBAccount.NextRow())
    //         {
    //             if (m_mTradeModel.type == 1)
    //             {
    //                 if (RET_SUCCESS == operDB(m_coinB))
    //                 {
    //                     iStatus = 1;
    //                     retMsg = "交易成功";
    //                 }
    //                 else
    //                 {
    //                     iStatus = 0;
    //                     retMsg = "交易失败";
    //                 }
    //             }
    //             else if (m_mTradeModel.type == 2)
    //             {
    //                 if (RET_SUCCESS == operDB(m_coinA))
    //                 {
    //                     iStatus = 1;
    //                     retMsg = "交易成功";
    //                 }
    //                 else
    //                 {
    //                     iStatus = 0;
    //                     retMsg = "交易失败";
    //                 }
    //             }
    //             else
    //             {
    //                 iStatus = 0;
    //                 retMsg = "交易失败";
    //             }
    //         }
    //         else
    //         {
    //             iStatus = 0;
    //             retMsg = "交易失败";
    //         }
    //     }
    //     else
    //     {
    //         iStatus = 0;
    //         retMsg = "执行错误";
    //     }
    // }
    UPResponse *rspPkg = new UPResponse;
    rspPkg->set_token(m_mTradeModel.token);
    rspPkg->set_status(iStatus);
    rspPkg->set_data(retMsg);
    UProtocolBase *prot = (UProtocolBase *)rspPkg;
    prot->m_uiType = UPUptrade::CMD;
    return prot;
}

string CRequestOperTrade::getOperationToken()
{
    return m_mTradeModel.token;
}