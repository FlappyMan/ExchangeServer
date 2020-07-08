/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-05-25 19:17:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Business/RequestOperCancelTrade.cpp
 */
#include "RequestOperCancelTrade.h"
CRequestOperCancelTrade::CRequestOperCancelTrade(CCancelTradeModel model)
{
    m_mCancelTrade = model;
}
UProtocolBase *CRequestOperCancelTrade::requestOperation()
{
    int iStatus = 0;
    string retMsg = "";
    if (m_mCancelTrade.marketid == 0 || m_mCancelTrade.orderid == 0 || m_mCancelTrade.canceltradeid == 0)
    {
        iStatus = 0;
        retMsg = "参数错误";
    }
    else
    {
        if (m_mCancelTrade.type != 1 && m_mCancelTrade.type != 2)
        {
            iStatus = 0;
            retMsg = "参数错误";
        }
        else if(m_mCancelTrade.price.length() == 0 || m_mCancelTrade.num.length() == 0)
        {
            iStatus = 1;
            retMsg = "验证通过";
        }
        else
        {
            string coinid = m_mCancelTrade.type == 1?m_coinB:m_coinA;
            string tableName = " user_assets_" + coinid + " ";
            string sqlBase = "select usable,freeze from " + tableName + " where user_id = ";
            CDispatchManager::g_DBAssets.Sql(sqlBase, false, true);
            CDispatchManager::g_DBAssets.Sql(m_mCancelTrade.uid, true, false);
            CDispatchManager::g_DBAssets.Sql(";", false, false);
            DEBUG_INFO(CDispatchManager::g_DBAssets.getSql());
            if (CDispatchManager::g_DBAssets.Select())
            {
                if (CDispatchManager::g_DBAssets.NextRow())
                {
                    string usable, freeze;
                    CDispatchManager::g_DBAssets.Value(0, usable);
                    CDispatchManager::g_DBAssets.Value(1, freeze);

                    string total = CDispatchManager::mutipLongDouble(m_mCancelTrade.price, m_mCancelTrade.num);
                    string strBalanceUsable = CDispatchManager::addLongDouble(usable, total);
                    string strBalanceFreeze = CDispatchManager::minusLongDouble(freeze, total);
                    CDispatchManager::g_DBAssets.Sql("UPDATE ", false, true);
                    CDispatchManager::g_DBAssets.Sql(tableName, true, false);
                    CDispatchManager::g_DBAssets.Sql(" SET usable = \"", false, false);
                    CDispatchManager::g_DBAssets.Sql(strBalanceUsable, true, false);
                    CDispatchManager::g_DBAssets.Sql("\" , freeze = \"", false, false);
                    CDispatchManager::g_DBAssets.Sql(strBalanceFreeze, true, false);
                    CDispatchManager::g_DBAssets.Sql("\" where status = 1 and user_id = \"", false, false);
                    CDispatchManager::g_DBAssets.Sql(m_mCancelTrade.uid, true, false);
                    CDispatchManager::g_DBAssets.Sql("\";", false, false);
                    DEBUG_INFO(CDispatchManager::g_DBAssets.getSql());
                    if (CDispatchManager::g_DBAssets.Exec() < 0)
                    {
                        iStatus = 0;
                        retMsg = "操作失败";
                    }
                    else
                    {
                        iStatus = 1;
                        retMsg = "操作成功";
                    }
                    return NULL;
                }
                else
                {
                    iStatus = 0;
                    retMsg = "操作失败";
                }
            }
            else
            {
                iStatus = 0;
                retMsg = "操作失败";
            }
        }
    }

    UPResponse *rspPkg = new UPResponse;
    rspPkg->set_token(m_mCancelTrade.token);
    rspPkg->set_status(iStatus);
    rspPkg->set_data(retMsg);
    UProtocolBase *prot = (UProtocolBase *)rspPkg;
    prot->m_uiType = UPCanceltrade::CMD;
    return prot;
}
string CRequestOperCancelTrade::getOperationToken()
{
    return m_mCancelTrade.token;
}