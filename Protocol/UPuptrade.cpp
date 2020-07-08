/*
 * @Author: your name
 * @Date: 2020-05-06 02:03:02
 * @LastEditTime: 2020-06-27 19:14:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Protocol/UPuptrade.cpp
 */ 
#include "UPuptrade.h"

void UPUptrade::JsonPack(Json::Value &root)
{
    root["uid"] = Json::Value(uid());
    root["paypassword"] = Json::Value(paypassword());
    root["marketid"] = Json::Value(marketid());
    root["price"] = Json::Value(price());
    root["num"] = Json::Value(num());
    root["type"] = Json::Value(type());
    if(has_token())
    {
        root["token"] = Json::Value(token());
    }
    root["id"] = Json::Value(orderid());
}

bool UPUptrade::JsonUnpack(Json::Value &root)
{
    set_uid(root["uid"].asUInt64());
    set_paypassword(root["paypassword"].asString());
    set_marketid(root["marketid"].asInt64());
    set_price(root["price"].asString());
    set_num(root["num"].asString());
    set_type(root["type"].asInt());
    set_token(root["token"].asString());
    set_orderid(root["id"].asUInt64());
    return true;
}
