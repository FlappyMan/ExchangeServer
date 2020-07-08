/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-05-07 01:08:07
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/Business/RequestOperTrade.h
 */
#ifndef _TRADE_REQUESTOPER_HEADER_
#define _TRADE_REQUESTOPER_HEADER_
#include "BaseRequestOper.h"
#include "TradeModel.hpp"

class CRequestOperTrade : public CBaseRequestOper
{
public:
    CRequestOperTrade(CTradeModel model);
    string getRequestToken();
    int requestOperation();
    
private:
    CTradeModel m_mTradeModel;
};

#endif
