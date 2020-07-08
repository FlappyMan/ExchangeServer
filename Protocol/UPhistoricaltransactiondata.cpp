#include "UPhistoricaltransactiondata.h"

void UPHistoricalTransactionData::JsonPack(Json::Value &root)
{
    Json::Value& array = root["data"]["tradelog"];
    Json::Value tdata;
    root["type"] = type();
    const Data& tmpData = data();
    for(int i = 0; i < tmpData.tradelog_size(); ++i)
    {
        const Data::TradeLog& tmpTradeLog = tmpData.tradelog(i);
        tdata["tradeid"] = tmpTradeLog.tradeid();
        tdata["addtime"] = tmpTradeLog.addtime();
        tdata["timestamp"] = tmpTradeLog.timestamp();
        tdata["type"] = tmpTradeLog.type();
        tdata["price"] = tmpTradeLog.price();
        tdata["mum"] = tmpTradeLog.mum();
        tdata["num"] = tmpTradeLog.num(); 
        array.append(tdata);
        tdata.clear();      
    }
}

bool UPHistoricalTransactionData::JsonUnpack(Json::Value &root)
{
    Json::Value item = root["data"]["tradelog"];
    if(item.isArray() == false)return false;
    set_type(root["type"].asString());
    Data* pData = mutable_data();
    Data::TradeLog *pTradeLog = NULL;
    for(uint32_t i = 0; i < root["data"]["tradelog"].size(); ++i)
    {
        Json::Value& tradeItem = item[i];
        pTradeLog = pData->add_tradelog();
        pTradeLog->set_tradeid(tradeItem["tradeid"].asUInt64());
        pTradeLog->set_addtime(tradeItem["addtime"].asString());
        pTradeLog->set_timestamp(tradeItem["timestamp"].asString());
        pTradeLog->set_type(tradeItem["type"].asUInt64());
        pTradeLog->set_price(tradeItem["price"].asString());
        pTradeLog->set_mum(tradeItem["mum"].asString());
        pTradeLog->set_num(tradeItem["num"].asString());      
    }
    return true;
}
