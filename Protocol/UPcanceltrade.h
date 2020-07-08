#ifndef _UPCANCELTRADE_H_
#define _UPCANCELTRADE_H_
#include "UBaseLib.h"
#include "proto/ukex.canceltrade.pb.h"
#include "UProtocolBase.h"

class UPCanceltrade : public UProtocolBase, public ukex::canceltrade
{
    public:
        UPCanceltrade(){};
        virtual ~UPCanceltrade(){};

        void JsonPack(Json::Value &root);
        bool JsonUnpack(Json::Value &root);
    public:
        //static constexpr const char* CMD_WSAPI = "sub";//WS
        static constexpr const char* CMD_WEBAPI = "/api/private/cancelTrade";//HTTP
        static constexpr const char* CMD_ROBOTAPI = "/api/private/cancelTrade";//robot http
};


#endif
