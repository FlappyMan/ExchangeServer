#ifndef _ALLMARKETINFO_H_
#define _ALLMARKETINFO_H_
#include "UBaseLib.h"
#include "proto/ukex.allmarketinfo.pb.h"
#include "UProtocolBase.h"

class UPAllmarketinfo : public UProtocolBase, public ukex::allmarketinfo
{
    public:
        UPAllmarketinfo(){};
        virtual ~UPAllmarketinfo(){};

        void JsonPack(Json::Value &root);
        bool JsonUnpack(Json::Value &root);
    public:
        static constexpr const char* CMD_WSAPI = "markets";//WS
        static constexpr const char* CMD_WEBAPI = "/api/private/upTrade";//HTTP
        static constexpr const char* CMD_ROBOTAPI = "/api/private/upTrade";//robot http
};


#endif
