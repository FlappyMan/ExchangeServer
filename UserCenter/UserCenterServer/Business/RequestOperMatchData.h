/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-05-18 20:47:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Business/RequestOperMatchData.h
 */ 
#ifndef _MATCHDATA_REQUESTOPER_HEADER_
#define _MATCHDATA_REQUESTOPER_HEADER_
#include "BaseRequestOper.h"
#include "MatchedDataModel.hpp"
class CRequestOperMatchData : public CBaseRequestOper
{
public:
    CRequestOperMatchData(CMatchedDataModel model);
    UProtocolBase * requestOperation();
    string getOperationToken();
private:
    void FeeChargeCalculation(string uid, string &totalFee);
    void addAmount(string amount, string uid, string coinID);
    void minusAmount(string amount, string uid, string coinID);
public:
    CMatchedDataModel m_mMatchData;
private:
    string m_sFeeBuy;
    string m_sFeeSell;
    string m_sFeeUID;
    string m_sInvitBuyFee;
    string m_sInvitSellFee;
    string m_sInvitUID_1;
    string m_sInvitUID_2;
    string m_sInvitUID_3;
};

#endif
