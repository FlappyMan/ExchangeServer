#ifndef _UPCANCELTRADEBATCH_H_
#define _UPCANCELTRADEBATCH_H_
#include "UBaseLib.h"
#include "proto/ukex.canceltradebatch.pb.h"
#include "UProtocolBase.h"

class UPCanceltradebatch : public UProtocolBase, public ukex::canceltradebatch
{
    public:
        UPCanceltradebatch(){};
        virtual ~UPCanceltradebatch(){};

        void JsonPack(Json::Value &root);
        bool JsonUnpack(Json::Value &root);
    public:
        //static constexpr const char* CMD_WSAPI = "sub";//WS
        static constexpr const char* CMD_WEBAPI = "/api/private/cancelTradeBatch";//HTTP
        static constexpr const char* CMD_ROBOTAPI = "/api/private/cancelTradeBatch";//robot http   
};


#endif
