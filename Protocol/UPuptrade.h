#ifndef _UPTRADE_H_
#define _UPTRADE_H_

#include "UBaseLib.h"
#include "proto/ukex.uptrade.pb.h"
#include "UProtocolBase.h"

class UPUptrade : public UProtocolBase, public ukex::uptrade
{
    public:
        UPUptrade(){};
        virtual ~UPUptrade(){};

        void JsonPack(Json::Value &root);
        bool JsonUnpack(Json::Value &root);
    public:
        //static constexpr const char* CMD_WSAPI = "sub";//WS
        static constexpr const char* CMD_WEBAPI = "/api/private/upTrade";//HTTP
        static constexpr const char* CMD_ROBOTAPI = "/api/private/upTrade";//robot http
};

#endif
