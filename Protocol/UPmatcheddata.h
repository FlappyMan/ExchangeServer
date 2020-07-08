#ifndef _UPMATCHEDDATA_H_
#define _UPMATCHEDDATA_H_
#include "UBaseLib.h"
#include "proto/ukex.matcheddata.pb.h"
#include "UProtocolBase.h"

class UPMatchedData : public UProtocolBase, public ukex::matcheddata
{
    public:
        UPMatchedData(){};
        virtual ~UPMatchedData(){};

        void JsonPack(Json::Value &root);
        bool JsonUnpack(Json::Value &root);
    public:
        static constexpr const char* CMD_WSAPI = "trade";//WS
        static constexpr const char* CMD_WEBAPI = "/api/private/upTrade";//HTTP
        static constexpr const char* CMD_ROBOTAPI = "/api/private/upTrade";//robot http
    public:
        string m_sMarketID;
};


#endif
