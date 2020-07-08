#include "UPklinehistoricaldata.h"

void UPKlinehistoricaldata::JsonPack(Json::Value &root)
{
    Json::Value& array = root["data"];
    Json::Value kdata;
    Data tmpData;
    for(int i = 0; i < data_size(); ++i)
    {
        const Data& tmpData = data(i);
        kdata["0"] = tmpData.k0();
        kdata["1"] = tmpData.k1();
        kdata["2"] = tmpData.k2();
        kdata["3"] = tmpData.k3();
        kdata["4"] = tmpData.k4();
        kdata["5"] = tmpData.k5();
        array.append(kdata);
        kdata.clear();
    }
}

bool UPKlinehistoricaldata::JsonUnpack(Json::Value &root)
{
    Json::Value kdata = root["data"];
    if(kdata.isArray() == false)return false;
    Data* pData = NULL;
    for(uint32_t i = 0; i < root["data"].size(); ++i)
    {
        Json::Value& tradeItem = kdata[i];
        pData = add_data();
        pData->set_k0(tradeItem["0"].asString());
        pData->set_k1(tradeItem["1"].asString());
        pData->set_k2(tradeItem["2"].asString());
        pData->set_k3(tradeItem["3"].asString());
        pData->set_k4(tradeItem["4"].asString());
        pData->set_k5(tradeItem["5"].asString());     
    }
    return true;
}
