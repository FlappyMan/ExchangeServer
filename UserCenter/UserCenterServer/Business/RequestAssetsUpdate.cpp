/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-15 20:37:39
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Business/RequestOperTrade.cpp
 */
#include "RequestAssetsUpdate.h"

CRequestAssetsUpdate::CRequestAssetsUpdate(CAssetsUpdate model)
{
    m_mAssetsModel = model;
}

UProtocolBase *CRequestAssetsUpdate::requestOperation()
{
    int iStatus = 0;
    string retMsg = "";
    if (m_mAssetsModel.paypassword.length() == 0 || m_mAssetsModel.uid == 0 || m_mAssetsModel.opertype == 0)
    {
        iStatus = 0;
        retMsg = "参数缺失";
    }
    else
    {
        string vip_level;
        if ((m_mAssetsModel.opertype == 1 || m_mAssetsModel.opertype == 3) && m_mAssetsModel.operid == 1)
        {
            // 需要在和支付密码
            CDispatchManager::g_DBAccount.Sql("select vip_level from user_info where paypassword = \"", false, true);
            CDispatchManager::g_DBAccount.Sql(m_mAssetsModel.paypassword, true, false);
            CDispatchManager::g_DBAccount.Sql("\" and user_id = \"", false, false);
            string uid = std::to_string(m_mAssetsModel.uid);
            CDispatchManager::g_DBAccount.Sql(uid, true, false);
            CDispatchManager::g_DBAccount.Sql("\";", false, false);
            DEBUG_INFO(CDispatchManager::g_DBAccount.getSql());
            // 1-验证交易密码
            if (CDispatchManager::g_DBAccount.Select())
            {
                if (CDispatchManager::g_DBAccount.NextRow())
                {
                    iStatus = 1;
                    CDispatchManager::g_DBAssets.Value(0, vip_level);
                    
                }
                else
                {
                    iStatus = 0;
                    retMsg = "交易密码错误";
                }
            }
            else
            {
                iStatus = 0;
                retMsg = "执行错误";
            }
        }
        else
        {
            // 不需要支付密码
            iStatus = 1;
        }
        if (iStatus == 1)
        {
            m_mAssetsModel.businessOper(m_mAssetsModel.list,iStatus,retMsg,vip_level,m_marketID);
        }
    }
    UPResponse *rspPkg = new UPResponse;
    rspPkg->set_token(std::to_string(m_mAssetsModel.uid));
    rspPkg->set_status(iStatus);
    rspPkg->set_data(retMsg);
    UProtocolBase *prot = (UProtocolBase *)rspPkg;
    prot->m_uiType = UPassetsupdate::CMD;
    return prot;
}
string CRequestAssetsUpdate::getOperationToken()
{
    return "";
}
