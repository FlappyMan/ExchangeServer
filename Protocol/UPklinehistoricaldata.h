#ifndef _UPKLINEHISTORICALDATA_H_
#define _UPKLINEHISTORICALDATA_H_
#include "UBaseLib.h"
#include "proto/ukex.klinehistoricaldata.pb.h"
#include "UProtocolBase.h"

class UPKlinehistoricaldata : public UProtocolBase, public ukex::klinehistoricaldata
{
    public:
        UPKlinehistoricaldata(){};
        virtual ~UPKlinehistoricaldata(){};

        void JsonPack(Json::Value &root);
        bool JsonUnpack(Json::Value &root);
    public:
        static constexpr const char* CMD_WSAPI = "req";//WS
        static constexpr const char* CMD_WEBAPI = "/api/private/req";//HTTP
        static constexpr const char* CMD_ROBOTAPI = "/api/private/req";//robot http
};


#endif
