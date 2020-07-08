#include "UPklinedata.h"

void UPKlinedata::JsonPack(Json::Value &root)
{
    Json::Value& array = root["data"];
    Json::Value kdata;
    root["type"] = type();
    for(int i = 0; i < data_size(); ++i)
    {
        const Data& tmpData = data(i);
        array[i].append(tmpData.k0());
        array[i].append(tmpData.k1());
        array[i].append(tmpData.k2());
        array[i].append(tmpData.k3());
        array[i].append(tmpData.k4());
        array[i].append(tmpData.k5());
        kdata.clear();
    }
}

bool UPKlinedata::JsonUnpack(Json::Value &root)
{
    Json::Value kdata = root["data"];
    if(kdata.isArray() == false)return false;
    set_type(root["type"].asString());
    Data* pData = NULL;
    for(uint32_t i = 0; i < root["data"].size(); ++i)
    {
        Json::Value& tradeItem = kdata[i];
        pData = add_data();
        pData->set_k0(tradeItem[0].asString());
        pData->set_k1(tradeItem[1].asString());
        pData->set_k2(tradeItem[2].asString());
        pData->set_k3(tradeItem[3].asString());
        pData->set_k4(tradeItem[4].asString());
        pData->set_k5(tradeItem[5].asString());     
    }
    return true;
}
