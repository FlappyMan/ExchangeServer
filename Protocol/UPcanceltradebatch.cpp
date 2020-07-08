#include "UPcanceltradebatch.h"

void UPCanceltradebatch::JsonPack(Json::Value &root)
{
    root["uid"] = Json::Value(uid());
    root["token"] = Json::Value(token());
    Json::Value& array=root["orders"];  
    Json::Value order;
    for(int i = 0; i < orders_size(); ++i)
    {
        const Orders& tmpOrder = orders(i);       
        order["id"] = tmpOrder.orderid();
        order["marketid"] = tmpOrder.marketid();
        array.append(order);  
        order.clear();
    }
}

bool UPCanceltradebatch::JsonUnpack(Json::Value &root)
{
    set_uid(root["uid"].asUInt64());
    set_token(root["token"].asString());
    if(root["orders"].isArray() == false)return false;
    Orders* pOrders = NULL;
    for(uint32_t i=0; i < root["orders"].size(); ++i)
    {
        Json::Value& item = root["orders"][i];      
        pOrders = add_orders();
        pOrders->set_orderid(item["id"].asUInt64());
        pOrders->set_marketid(item["marketid"].asUInt64());
    }
    return true;
}
