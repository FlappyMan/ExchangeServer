#ifndef _HTTP_RESP_HEADER_
#define _HTTP_RESP_HEADER_
#include "../global.h"

class CHttpResp : public UProtocolBase
{
public:
    CHttpResp(){};
    virtual ~CHttpResp(){};

    void JsonPack(Json::Value &root);
    bool JsonUnpack(Json::Value &root);
};

#endif