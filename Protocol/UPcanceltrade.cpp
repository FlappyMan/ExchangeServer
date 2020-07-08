/*
 * @Author: your name
 * @Date: 2020-05-06 02:03:02
 * @LastEditTime: 2020-06-27 19:13:15
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Protocol/UPcanceltrade.cpp
 */ 
#include "UPcanceltrade.h"

void UPCanceltrade::JsonPack(Json::Value &root)
{
    root["uid"] = Json::Value(uid());
    root["id"] = Json::Value(orderid());
    root["marketid"] = Json::Value(marketid());
    if(has_token())
    {
        root["token"] = Json::Value(token());
    }
}

bool UPCanceltrade::JsonUnpack(Json::Value &root)
{
    set_uid(root["uid"].asUInt64());   
    set_orderid(root["id"].asUInt64());
    set_marketid(root["marketid"].asUInt64());
    set_token(root["token"].asString());
    return true;
}
