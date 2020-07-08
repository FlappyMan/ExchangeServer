#include "UPRequest.h"

bool UPRequest::JsonParser(string& req, Json::Value &root)
{
    req = root["req"].asString();
    return true;
}