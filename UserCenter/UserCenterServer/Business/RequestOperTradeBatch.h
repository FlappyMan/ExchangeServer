/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-05-14 00:19:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Business/RequestOperTradeBatch.h
 */
#ifndef _TRADEBATCH_REQUESTOPER_HEADER_
#define _TRADEBATCH_REQUESTOPER_HEADER_
#include "BaseRequestOper.h"
#include "TradeBatchModel.hpp"
class CRequestOperTradeBatch : public CBaseRequestOper
{
public:
    CRequestOperTradeBatch(CTradeBatchModel model);
    UProtocolBase * requestOperation();
    string getOperationToken();
    int operDB(string coinId,CTradeInfoModel model,string uid,bool bIsReset);
public:
    CTradeBatchModel m_mTradeBatch;
};

#endif
