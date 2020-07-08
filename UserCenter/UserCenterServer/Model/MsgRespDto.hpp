#ifndef _MSG_RESP_DTO_HEADER_
#define _MSG_RESP_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"

class CMsgRespDto : public CJsonObjectBase
{
public:
    string getResponse()
    {
        Json::Value jsonObj;
        jsonObj["data"] = data;
        jsonObj["status"] = status;
        Json::FastWriter writer;
        string ret = writer.write(jsonObj);
        return ret;
    }
    CJsonObjectBase * requestOperation(){};
public:
    string data;
    int status;
};

#endif