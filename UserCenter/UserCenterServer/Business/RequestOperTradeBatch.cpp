/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-05-25 22:39:55
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Business/RequestOperTradeBatch.cpp
 */
#include "RequestOperTradeBatch.h"

CRequestOperTradeBatch::CRequestOperTradeBatch(CTradeBatchModel model)
{
    m_mTradeBatch = model;
}
int CRequestOperTradeBatch::operDB(string coinId,CTradeInfoModel model,string uid,bool bIsReset)
{
    int iRet = RET_SUCCESS;
    string tableName = " user_assets_" + coinId + " ";
    string sqlBase = "select usable,freeze from " + tableName + " where user_id = \"";
    CDispatchManager::g_DBAssets.Sql(sqlBase, false, true);
    CDispatchManager::g_DBAssets.Sql(uid, true, false);
    CDispatchManager::g_DBAssets.Sql("\";", false, false);
    if (CDispatchManager::g_DBAssets.Select())
    {
        if (CDispatchManager::g_DBAssets.NextRow())
        {
            string usable, freeze, total;
            CDispatchManager::g_DBAssets.Value(0, usable);
            CDispatchManager::g_DBAssets.Value(1, freeze);
            total = CDispatchManager::mutipLongDouble(model.price, model.num);

            string strBalanceUsable = bIsReset == false?CDispatchManager::minusLongDouble(usable, total):CDispatchManager::addLongDouble(usable, total);
            string strBalanceFreeze = bIsReset == false?CDispatchManager::addLongDouble(freeze, total):CDispatchManager::minusLongDouble(freeze, total);
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
            if (CDispatchManager::g_DBAssets.Exec() < 0)
            {
                iRet = RET_FAILED;
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
    return iRet;
}

UProtocolBase *CRequestOperTradeBatch::requestOperation()
{
    int iStatus = 0;
    string retMsg = "";
    if (m_mTradeBatch.paypassword.length() == 0 || m_mTradeBatch.uid.length() == 0)
    {
        iStatus = 0;
        retMsg = "参数缺失";
    }
    else
    {
        CDispatchManager::g_DBAccount.Sql("select user_id from user_info where paypassword = \"", false, true);
        CDispatchManager::g_DBAccount.Sql(m_mTradeBatch.paypassword, true, false);
        CDispatchManager::g_DBAccount.Sql("\" and user_id = \"", false, false);
        CDispatchManager::g_DBAccount.Sql(m_mTradeBatch.uid, true, false);
        CDispatchManager::g_DBAccount.Sql("\";", false, false);

        // 1-验证交易密码
        if (CDispatchManager::g_DBAccount.Select())
        {
            if (CDispatchManager::g_DBAccount.NextRow())
            {
                vector<CTradeInfoModel> vecReset;
                int iTradeSize = m_mTradeBatch.trades.size();
                for (size_t i = 0; i < iTradeSize; i++)
                {
                    CTradeInfoModel info = m_mTradeBatch.trades[i];
                    if (info.type == 1)
                    {
                        if (RET_SUCCESS == operDB(m_coinB,info,m_mTradeBatch.uid,false))
                        {
                            iStatus = 1;
                            retMsg = "交易成功";
                            vecReset.push_back(info);
                        }
                        else
                        {
                            iStatus = 0;
                            retMsg = "交易失败";
                            break;
                        }
                    }
                    else if (info.type == 2)
                    {
                        if (RET_SUCCESS == operDB(m_coinA,info,m_mTradeBatch.uid,false))
                        {
                            iStatus = 1;
                            retMsg = "交易成功";
                            vecReset.push_back(info);
                        }
                        else
                        {
                            iStatus = 0;
                            retMsg = "交易失败";
                            break;
                        }
                    }
                    else
                    {
                        iStatus = 0;
                        retMsg = "交易失败";
                        break;
                    }
                }
                if (iStatus == 0)
                {
                    // 如果数据中存在失败，则恢复之前的所有数据
                    int iReset = vecReset.size();
                    for (size_t i = 0; i < iReset; i++)
                    {
                        CTradeInfoModel info = vecReset[i];
                        if (info.type == 1)
                        {
                            operDB(m_coinB,info,m_mTradeBatch.uid,true);
                        }
                        else
                        {
                            operDB(m_coinA,info,m_mTradeBatch.uid,true);
                        }
                    }
                }
                
            }
            else
            {
                iStatus = 0;
                retMsg = "交易失败";
            }
        }
        else
        {
            iStatus = 0;
            retMsg = "执行错误";
        }
    }

    UPResponse *rspPkg = new UPResponse;
    rspPkg->set_token(m_mTradeBatch.token);
    rspPkg->set_status(iStatus);
    rspPkg->set_data(retMsg);
    UProtocolBase *prot = (UProtocolBase *)rspPkg;
    prot->m_uiType = UPUptradebatch::CMD;
    return prot;
}
string CRequestOperTradeBatch::getOperationToken()
{
    return m_mTradeBatch.token;
}