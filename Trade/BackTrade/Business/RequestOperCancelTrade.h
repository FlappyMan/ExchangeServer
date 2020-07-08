/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-05-06 22:42:43
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/Business/RequestOperCancelTrade.h
 */
#ifndef _TRADECANCEL_REQUESTOPER_HEADER_
#define _TRADECANCEL_REQUESTOPER_HEADER_
#include "BaseRequestOper.h"
#include "CancelTradeModel.hpp"
class CRequestOperCancelTrade : public CBaseRequestOper
{
public:
    CRequestOperCancelTrade(CCancelTradeModel model);
    string getRequestToken();
    int requestOperation();
public:
    CCancelTradeModel m_mCancelTrade;
};

#endif
