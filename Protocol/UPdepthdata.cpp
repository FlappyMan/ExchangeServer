#include "UPdepthdata.h"

void UPDepthdata::JsonPack(Json::Value &root)
{
    Json::Value& array_b = root["data"]["depth"]["buy"];
    Json::Value& array_s = root["data"]["depth"]["sell"];
    Json::Value bdata,sdata;
    root["type"] = type();
    Data tmpData = data();
    const Data::Depth& tmpDepth = tmpData.depth();
    for(int i = 0; i < tmpDepth.buy_size(); ++i)
    {
        const Data::Depth::buyAction& tmpBuy = tmpDepth.buy(i);
        bdata["b0"] = tmpBuy.b0();
        bdata["b1"] = tmpBuy.b1();
        array_b.append(bdata);
        bdata.clear();
    }
    for(int j = 0; j < tmpDepth.sell_size(); ++j)
    {
        const Data::Depth::sellAction& tmpSell = tmpDepth.sell(j);
        sdata["s0"] = tmpSell.s0();
        sdata["s1"] = tmpSell.s1();
        array_s.append(sdata);
        sdata.clear();
    }
}

bool UPDepthdata::JsonUnpack(Json::Value &root)
{
    Json::Value buyItem,sellItem;
    buyItem = root["data"]["depth"]["buy"];
    sellItem = root["data"]["depth"]["sell"];
    if(buyItem.isArray() == false || sellItem.isArray() == false)return false;
    set_type(root["type"].asString());
    Data* pData = mutable_data();
    Data::Depth* pDepth = pData->mutable_depth();
    Data::Depth::buyAction* pBuy = NULL;
    Data::Depth::sellAction* pSell = NULL;
    for(uint32_t i = 0; i < buyItem.size(); ++i)
    {
        pBuy = pDepth->add_buy();
        Json::Value& item = buyItem[i];      
        pBuy->set_b0(item["b0"].asString());
        pBuy->set_b1(item["b1"].asString());
    }
    for(uint32_t j = 0; j < sellItem.size(); ++j)
    {
        pSell = pDepth->add_sell();
        Json::Value& item = sellItem[j];  
        pSell->set_s0(item["s0"].asString());
        pSell->set_s1(item["s1"].asString());
    }
    return true;
}
