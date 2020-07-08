#ifndef _UPTRADEBATCH_H_
#define _UPTRADEBATCH_H_
#include "UBaseLib.h"
#include "proto/ukex.uptradebatch.pb.h"
#include "UProtocolBase.h"

class UPUptradebatch : public UProtocolBase, public ukex::uptradebatch
{
    public:
        UPUptradebatch(){};
        virtual ~UPUptradebatch(){};

        void JsonPack(Json::Value &root);
        bool JsonUnpack(Json::Value &root);
    public:
        //static constexpr const char* CMD_WSAPI = "sub";//WS
        static constexpr const char* CMD_WEBAPI = "/api/private/upTradeBatch";//HTTP
        static constexpr const char* CMD_ROBOTAPI = "/api/private/upTradeBatch";//robot http    
};


#endif
