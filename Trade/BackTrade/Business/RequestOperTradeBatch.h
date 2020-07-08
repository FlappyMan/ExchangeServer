/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-21 23:27:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/Business/RequestOperTradeBatch.h
 */
#ifndef _TRADEBATCH_REQUESTOPER_HEADER_
#define _TRADEBATCH_REQUESTOPER_HEADER_
#include "BaseRequestOper.h"
#include "TradeBatchModel.hpp"
class CRequestOperTradeBatch : public CBaseRequestOper
{
public:
    CRequestOperTradeBatch(CTradeBatchModel model);
    string getRequestToken();
    int requestOperation();
    // int operDB(CTradeInfoModel model, string uid);
public:
    CTradeBatchModel m_mTradeBatch;
};

#endif
