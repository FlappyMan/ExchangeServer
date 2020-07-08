#ifndef _UPRESPONSE_H_
#define _UPRESPONSE_H_
#include "UBaseLib.h"
#include "proto/ukex.response.pb.h"
#include "UProtocolBase.h"

class UPResponse : public UProtocolBase, public ukex::response
{
    public:
        UPResponse(){};
        virtual ~UPResponse(){};

        void JsonPack(Json::Value &root);
        bool JsonUnpack(Json::Value &root);
    public:
        static constexpr const char* CMD_MAPI = "/ws/";
        static constexpr const char* CMD_WEBAPI = "/ws/";
        static constexpr const char* CMD_API = "/ws/";
};


#endif
