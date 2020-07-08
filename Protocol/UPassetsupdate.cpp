#include "UPassetsupdate.h"

void UPassetsupdate::JsonPack(Json::Value &root)
{
    root["uid"] = Json::Value(uid());
    root["paypassword"] = Json::Value(paypassword());
    root["opertype"] = Json::Value(opertype());
    root["operid"] = Json::Value(operid());
    Json::Value& array = root["list"];
    Json::Value data;
    for (int i = 0; i < list_size(); ++i)
    {
        const List& lst = list(i);
        data["uid"] = lst.uid();
        data["typeid"] = lst.type_id();
        data["type"] = lst.coinid();
        data["usable"] = lst.usable();
        data["frozen"] = lst.fronzen();
        data["lockup"] = lst.lockup();
        array.append(data);
        data.clear();
    }
}

bool UPassetsupdate::JsonUnpack(Json::Value &root)
{
    set_uid(root["uid"].asUInt64());
    set_paypassword(root["paypassword"].asString());
    set_opertype(root["opertype"].asUInt());
    set_operid(root["operid"].asUInt());
    if(root["list"].isArray() == false)return false;
    List* pList = NULL;
    for (uint32_t i = 0; i < root["list"].size(); ++i)
    {
        Json::Value& item = root["list"][i];
        pList = add_list();
        pList->set_uid(item["uid"].asUInt64());
        pList->set_type_id(item["type_id"].asUInt());
        pList->set_type(item["type"].asUInt());
        pList->set_coinid(item["coinid"].asUInt());
        pList->set_usable(item["usable"].asString());
        pList->set_fronzen(item["fronzen"].asString());
        pList->set_lockup(item["lockup"].asString());
    }
    return true;
}