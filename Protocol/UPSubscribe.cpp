#include "UPSubscribe.h"

bool UPSubscribe::JsonParser(string& sub, Json::Value &root)
{
    sub = root["sub"].asString();
    return true;
}