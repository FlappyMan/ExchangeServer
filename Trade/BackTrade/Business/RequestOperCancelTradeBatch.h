/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-05-14 02:52:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/Business/RequestOperCancelTradeBatch.h
 */
#ifndef _TRADECANCELBAT_REQUESTOPER_HEADER_
#define _TRADECANCELBAT_REQUESTOPER_HEADER_
#include "BaseRequestOper.h"
#include "CancelTradeBatchModel.hpp"
class CRequestOperCancelTradeBatch : public CBaseRequestOper
{
public:
    CRequestOperCancelTradeBatch(CCancelTradeBatchModel model);
    string getRequestToken();
    int requestOperation();
// private:
//     int operDB(CCancelTradeInfoModel model, string uid);
public:
    CCancelTradeBatchModel m_mCancelTradeBatch;
};

#endif
