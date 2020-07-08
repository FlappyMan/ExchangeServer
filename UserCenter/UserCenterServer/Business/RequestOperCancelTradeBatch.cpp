/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-05-26 02:03:52
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Business/RequestOperCancelTradeBatch.cpp
 */
#include "RequestOperCancelTradeBatch.h"

CRequestOperCancelTradeBatch::CRequestOperCancelTradeBatch(CCancelTradeBatchModel model)
{
    m_mCancelTradeBatch = model;
}
int CRequestOperCancelTradeBatch::operDB(string coinID,CCancelTradeInfoModel model,string uid,bool bIsReset)
{
    int iRet = RET_SUCCESS;
    if (model.price.length() == 0 && model.num.length() == 0)
    {
        return iRet;
    }
    else
    {
        string tableName = " user_assets_" + coinID + " ";
        string sqlBase = "select usable,freeze from " + tableName + " where user_id = \"";
        CDispatchManager::g_DBAssets.Sql(sqlBase, false, true);
        CDispatchManager::g_DBAssets.Sql(uid, true, false);
        CDispatchManager::g_DBAssets.Sql("\";", false, false);
        DEBUG_INFO(CDispatchManager::g_DBAssets.getSql());
        if (CDispatchManager::g_DBAssets.Select())
        {
            if (CDispatchManager::g_DBAssets.NextRow())
            {
                string usable, freeze;
                CDispatchManager::g_DBAssets.Value(0, usable);
                CDispatchManager::g_DBAssets.Value(1, freeze);

                string total = CDispatchManager::mutipLongDouble(model.price, model.num);
                string strBalanceUsable = bIsReset == false ? CDispatchManager::addLongDouble(usable, total) : CDispatchManager::minusLongDouble(usable, total);
                string strBalanceFreeze = bIsReset == false ? CDispatchManager::minusLongDouble(freeze, total) : CDispatchManager::addLongDouble(freeze, total);
                CDispatchManager::g_DBAssets.Sql("UPDATE ", false, true);
                CDispatchManager::g_DBAssets.Sql(tableName, true, false);
                CDispatchManager::g_DBAssets.Sql(" SET usable = \"", false, false);
                CDispatchManager::g_DBAssets.Sql(strBalanceUsable, true, false);
                CDispatchManager::g_DBAssets.Sql("\" , freeze = \"", false, false);
                CDispatchManager::g_DBAssets.Sql(strBalanceFreeze, true, false);
                CDispatchManager::g_DBAssets.Sql("\" where status = 1 and user_id = \"", false, false);
                CDispatchManager::g_DBAssets.Sql(uid, true, false);
                CDispatchManager::g_DBAssets.Sql("\";", false, false);
                DEBUG_INFO(CDispatchManager::g_DBAssets.getSql());
                if (CDispatchManager::g_DBAssets.Exec() >= 0)
                {
                    iRet = RET_HOLD;
                }
            }
            else
            {
                iRet = RET_FAILED;
            }
        }
        else
        {
            iRet = RET_FAILED;
        }
    }
    return iRet;
}
UProtocolBase *CRequestOperCancelTradeBatch::requestOperation()
{
    int iStatus = 1;
    string retMsg = "请求成功";
    bool bIsRetNULL = false;
    if (m_mCancelTradeBatch.uid.length() == 0)
    {
        iStatus = 0;
        retMsg = "参数错误";
    }
    else
    {
        vector<CCancelTradeInfoModel> vecReset;
        int iOrderSize = m_mCancelTradeBatch.orders.size();
        for (size_t i = 0; i < iOrderSize; i++)
        {
            CCancelTradeInfoModel model = m_mCancelTradeBatch.orders[i];
            int iRet = operDB(model.type == 1?m_coinA:m_coinB,model,m_mCancelTradeBatch.uid,false);
            if (iRet == RET_FAILED)
            {
                iStatus = 0;
                retMsg = "参数错误";
                break;
            }
            else if(iRet == RET_HOLD)
            {
                bIsRetNULL = true;
            }
            else
            {
                // 成功
                vecReset.push_back(model);
            }
        }
        if (iStatus == 0)
        {
            for (size_t i = 0; i < vecReset.size(); i++)
            {
                CCancelTradeInfoModel model = vecReset[i];
                operDB(model.type == 1?m_coinA:m_coinB,model,m_mCancelTradeBatch.uid,true);
            }
        }
    }
    if (bIsRetNULL == true)
    {
        return NULL;
    }
    
    UPResponse *rspPkg = new UPResponse;
    rspPkg->set_token(m_mCancelTradeBatch.token);
    rspPkg->set_status(iStatus);
    rspPkg->set_data(retMsg);
    UProtocolBase *prot = (UProtocolBase *)rspPkg;
    prot->m_uiType = UPCanceltradebatch::CMD;
    return prot;
}
string CRequestOperCancelTradeBatch::getOperationToken()
{
    return m_mCancelTradeBatch.token;
}