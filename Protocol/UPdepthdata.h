#ifndef _UPDEPTHDATA_H_
#define _UPDEPTHDATA_H_
#include "UBaseLib.h"
#include "proto/ukex.depthdata.pb.h"
#include "UProtocolBase.h"

class UPDepthdata : public UProtocolBase, public ukex::depthdata
{
    public:
        UPDepthdata(){};
        virtual ~UPDepthdata(){};

        void JsonPack(Json::Value &root);
        bool JsonUnpack(Json::Value &root);
    public:
        static constexpr const char* CMD_WSAPI = "depth";//WS
        static constexpr const char* CMD_WEBAPI = "/api/private/depth";//HTTP
        static constexpr const char* CMD_ROBOTAPI = "/api/private/depth";//robot http
    public:
        string m_sMarketID;
};


#endif
