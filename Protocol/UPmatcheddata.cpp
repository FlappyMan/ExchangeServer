#include "UPmatcheddata.h"

void UPMatchedData::JsonPack(Json::Value &root)
{
    root["number"] = number();
    root["price"] = price();
    BuyUser buyUser = buyuser();
    Json::Value& bitem = root["buyuser"];
    bitem["userid"] = buyUser.userid();
    bitem["orderid"] = buyUser.orderid();
    SellUser sellUser = selluser();
    Json::Value& sitem = root["SellUser"];
    sitem["userid"] = sellUser.userid();
    sitem["orderid"] = sellUser.orderid();
}

bool UPMatchedData::JsonUnpack(Json::Value &root)
{
    set_number(root["number"].asString());
    set_price(root["price"].asString());
    BuyUser* bUser = mutable_buyuser();
    Json::Value& bitem = root["buyuser"];
    bUser->set_userid(bitem["userid"].asUInt64());
    bUser->set_orderid(bitem["orderid"].asUInt64());

    SellUser* sUser = mutable_selluser();
    Json::Value& sitem = root["SellUser"];
    sUser->set_userid(sitem["userid"].asUInt64());
    sUser->set_orderid(sitem["orderid"].asUInt64());
    return true;
}
