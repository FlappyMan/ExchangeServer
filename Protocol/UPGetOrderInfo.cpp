#include "UPGetOrderInfo.h"

void UPGetOrderInfo::JsonPack(Json::Value &root)
{
    root["uid"] = Json::Value(uid());
    root["marketid"] = Json::Value(marketid());
    root["orderid"] = Json::Value(orderid());
    root["token"] = Json::Value(token());
}

bool UPGetOrderInfo::JsonUnpack(Json::Value &root)
{
    set_uid(root["uid"].asUInt64());
    set_marketid(root["marketid"].asUInt64());
    set_orderid(root["orderid"].asUInt64());
    set_token(root["token"].asString());
    return true;
}
