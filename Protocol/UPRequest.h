#ifndef _UPREQUEST_H_
#define _UPREQUEST_H_
#include "UBaseLib.h"
#include "UProtocolBase.h"

class UPRequest : public UProtocolBase
{
    public:
        UPRequest(){};
        virtual ~UPRequest(){};

        void JsonPack(Json::Value &root){};
        bool JsonUnpack(Json::Value &root){return true;};

        bool JsonParser(string& req, Json::Value &root);
    public:
        static constexpr const char* CMD_WSAPI = "req";//WS
        //static constexpr const char* CMD_WEBAPI = "/api/private/subscribe";//HTTP
        //static constexpr const char* CMD_ROBOTAPI = "/api/private/subscribe";//robot http    
};


#endif
