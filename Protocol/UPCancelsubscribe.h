#ifndef _UPCANCELSUBSCRIBE_H_
#define _UPCANCELSUBSCRIBE_H_
#include "UBaseLib.h"
#include "UProtocolBase.h"

class UPCancelSubscribe : public UProtocolBase
{
    public:
        UPCancelSubscribe(){};
        virtual ~UPCancelSubscribe(){};

        void JsonPack(Json::Value &root){};
        bool JsonUnpack(Json::Value &root){return true;};

        bool JsonParser(string& cancelsub, Json::Value &root);
    public:
        static constexpr const char* CMD_WSAPI = "unsub";//WS
        //static constexpr const char* CMD_WEBAPI = "/api/private/subscribe";//HTTP
        //static constexpr const char* CMD_ROBOTAPI = "/api/private/subscribe";//robot http    
};


#endif
