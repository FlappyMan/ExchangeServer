#ifndef _LOGINRESP_DTO_HEADER_
#define _LOGINRESP_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"

class CLoginRespDto : public CJsonObjectBase
{
public:
    CJsonObjectBase *requestOperation(){};
    string getResponse()
    {
        string dataString;
        Json::Value sJson;
        sJson["data"] = Json::Value(data);
        sJson["status"] = Json::Value(status);
        UBJsonPack(dataString, sJson, true);
        return dataString;
    }

public:
    string data;
    int status;
};

#endif