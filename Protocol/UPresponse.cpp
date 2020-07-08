#include "UPresponse.h"

void UPResponse::JsonPack(Json::Value &root)
{
    root["status"] = status();
    root["data"] = data();
    if(has_token())
    {
        root["token"] = Json::Value(token());
    }
}

bool UPResponse::JsonUnpack(Json::Value &root)
{
    set_status(root["status"].asUInt());
    set_data(root["data"].asString());
    set_token(root["token"].asString());
    return true;
}
