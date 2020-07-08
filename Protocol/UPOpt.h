#ifndef _UPOPT_H_
#define _UPOPT_H_
#include "UBaseLib.h"
#include "UProtocolBase.h"

class UPOpt : public UProtocolBase
{
    public:
        UPOpt(){};
        virtual ~UPOpt(){};

        void JsonPack(Json::Value &root){};
        bool JsonUnpack(Json::Value &root){return true;};

        bool JsonParser(string& opt, Json::Value &root);
    public:
        static constexpr const char* CMD_WSAPI = "opt";//WS
        //static constexpr const char* CMD_WEBAPI = "/api/private/subscribe";//HTTP
        //static constexpr const char* CMD_ROBOTAPI = "/api/private/subscribe";//robot http    
};


#endif
