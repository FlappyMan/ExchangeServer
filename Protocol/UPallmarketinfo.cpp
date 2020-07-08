#include "UPallmarketinfo.h"

void UPAllmarketinfo::JsonPack(Json::Value &root)
{
    root["type"] = type();
    Json::Value marketinfo;
    Json::Value& array = root["data"];
    Json::Value jdata;
    for(int i = 0; i < data_size(); ++i)
    {
        const Data &tmpData = data(i);
        jdata["0"] = tmpData.m0();
        jdata["1"] = tmpData.m1();
        jdata["2"] = tmpData.m2();
        jdata["3"] = tmpData.m3();
        jdata["4"] = tmpData.m4();
        jdata["5"] = tmpData.m5();
        jdata["6"] = tmpData.m6();
        jdata["7"] = tmpData.m7();
        jdata["8"] = tmpData.m8();
        jdata["9"] = tmpData.m9();
        jdata["10"] = tmpData.m10();
        jdata["11"] = tmpData.m11();
        jdata["12"] = tmpData.m12();
        jdata["13"] = tmpData.m13();
        array.append(jdata);
        jdata.clear();
    }
    root["marketid"] = marketid();
    Exchange tmpExchange = exchange();
    root["exchange"]["btc"] = tmpExchange.btc();
    root["exchange"]["eth"] = tmpExchange.eth();	
}

bool UPAllmarketinfo::JsonUnpack(Json::Value &root)
{
    if(root["data"].isArray() == false)
        return false;
    set_type(root["type"].asString());
    Data *pData = NULL;
    for(uint32_t i = 0; i < root["data"].size(); ++i)
    {
        Json::Value& itemData = root["data"][i];
        pData = add_data();
        pData->set_m0(itemData["0"].asString());
        pData->set_m1(itemData["1"].asString());
        pData->set_m2(itemData["2"].asString());
        pData->set_m3(itemData["3"].asString());
        pData->set_m4(itemData["4"].asString());
        pData->set_m5(itemData["5"].asString());
        pData->set_m6(itemData["6"].asString());
        pData->set_m7(itemData["7"].asString());
        pData->set_m8(itemData["8"].asString());
        pData->set_m9(itemData["9"].asString());
        pData->set_m10(itemData["10"].asString());
        pData->set_m11(itemData["11"].asString());
        pData->set_m12(itemData["12"].asString());
        pData->set_m13(itemData["13"].asString());
    }
    set_marketid(root["marketid"].asUInt64());
    Exchange* pExchange = NULL;
    pExchange = mutable_exchange();
    pExchange->set_btc(root["exchange"]["btc"].asString());
    pExchange->set_eth(root["exchange"]["eth"].asString());
    return true;
}
