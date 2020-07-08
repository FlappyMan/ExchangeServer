/*
 * @Author: your name
 * @Date: 2020-05-06 02:03:02
 * @LastEditTime: 2020-06-27 19:24:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Protocol/UPuptradebatch.cpp
 */ 
#include "UPuptradebatch.h"

void UPUptradebatch::JsonPack(Json::Value &root)
{
    root["uid"] = Json::Value(uid());
    root["paypassword"] = Json::Value(paypassword());
    Json::Value& array=root["trades"];  
    Json::Value trade;
    for(int i = 0; i < trades_size(); ++i)
    {
        const Trades& tmpTrade = trades(i);       
        trade["marketid"] = tmpTrade.marketid();
        trade["price"] = tmpTrade.price();
        trade["num"] = tmpTrade.num();
        trade["type"] = tmpTrade.type();
        trade["id"] = tmpTrade.orderid();
        array.append(trade);
        trade.clear();
    }
}

bool UPUptradebatch::JsonUnpack(Json::Value &root)
{
    set_uid(root["uid"].asUInt64());
    set_paypassword(root["paypassword"].asString());
    if(root["trades"].isArray() == false)return false;
    Trades* pTrades = NULL;
    for(uint32_t i=0; i < root["trades"].size(); ++i)
    {
        Json::Value& item = root["trades"][i];      
        pTrades = add_trades();
        pTrades->set_marketid(item["marketid"].asUInt64());
        pTrades->set_price(item["price"].asString());
        pTrades->set_num(item["num"].asString());
        pTrades->set_type(item["type"].asUInt());
        pTrades->set_orderid(item["id"].asUInt64());
    }
    return true;
}