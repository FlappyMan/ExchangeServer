#include "UPCancelsubscribe.h"

bool UPCancelSubscribe::JsonParser(string& cancelsub, Json::Value &root)
{
    cancelsub = root["cancelsub"].asString();
    return true;
}