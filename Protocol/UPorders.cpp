#include "UPorders.h"

void UPOrders::JsonPack(Json::Value &root)
{
    root["orders"] = orders();
    Data tmpData = data();
    Json::Value& jdata = root["data"];
    jdata["orderNumber"] = tmpData.ordernumber();
    jdata["type"] = tmpData.type();
    jdata["amount"] = tmpData.amount();
    jdata["total"] = tmpData.total();
    jdata["initialRate"] = tmpData.initialrate();
    jdata["initialAmount"] = tmpData.initialamount();
    jdata["filledAmount"] = tmpData.filledamount();
    jdata["currencyPair"] = tmpData.currencypair();
    jdata["timestamp"] = tmpData.timestamp();
    jdata["status"] = tmpData.status();
}

bool UPOrders::JsonUnpack(Json::Value &root)
{
    set_orders(root["orders"].asString());
    Data* data = mutable_data();
    Json::Value& item = root["data"];
    data->set_ordernumber(item["orderNumber"].asUInt64());
    data->set_type(item["type"].asString());
    data->set_amount(item["amount"].asString());
    data->set_total(item["total"].asString());
    data->set_initialrate(item["initialRate"].asString());
    data->set_initialamount(item["initialAmount"].asString());
    data->set_filledamount(item["initialAmount"].asString());
    data->set_currencypair(item["currencyPair"].asString());
    data->set_timestamp(item["timestamp"].asUInt64());
    data->set_status(item["status"].asString());
    return true;
}