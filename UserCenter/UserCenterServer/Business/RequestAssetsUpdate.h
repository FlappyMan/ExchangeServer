/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-11 00:36:20
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Business/RequestOperTrade.h
 */
#ifndef _ASSETSUPDATE_REQUESTOPER_HEADER_
#define _ASSETSUPDATE_REQUESTOPER_HEADER_
#include "BaseRequestOper.h"
#include "AssetsUpdate.hpp"

class CRequestAssetsUpdate : public CBaseRequestOper
{
public:
    CRequestAssetsUpdate(CAssetsUpdate model);
    UProtocolBase * requestOperation();
    string getOperationToken();
private:
    vector<COrderInfo> AssetsUpdate(vector<COrderInfo>& vecOrder, bool bIsReset = false);
    void recordToFinancialAssetsLog(vector<COrderInfo>& vec,string userID);
    string getTypeName(int type);
    string getRespString(string level);
private:
    CAssetsUpdate m_mAssetsModel;
};

#endif
