#include "UPOpt.h"

bool UPOpt::JsonParser(string& opt, Json::Value &root)
{
    opt = root["opt"].asString();
    return true;
}