#ifndef _UPASSETSUPDATE_H_
#define _UPASSETSUPDATE_H_
#include "UBaseLib.h"
#include "proto/ukex.assetsupdate.pb.h"
#include "UProtocolBase.h"

class UPassetsupdate : public UProtocolBase, public ukex::assetsupdate
{
    public:
        UPassetsupdate(){};
        virtual ~UPassetsupdate(){};

        void JsonPack(Json::Value &root);
        bool JsonUnpack(Json::Value &root);
    public:
        static constexpr const char* CMD_WSAPI = "assetsupdate";//WS
        static constexpr const char* CMD_WEBAPI = "/api/private/assetsupdate";//HTTP
        static constexpr const char* CMD_ROBOTAPI = "/api/private/assetsupdate";//robot http
};


#endif
