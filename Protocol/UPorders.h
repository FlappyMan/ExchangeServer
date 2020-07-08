#ifndef _UPORDERS_H_
#define _UPORDERS_H_
#include "UBaseLib.h"
#include "proto/ukex.order.pb.h"
#include "UProtocolBase.h"

class UPOrders : public UProtocolBase, public ukex::order
{
    public:
        UPOrders(){};
        virtual ~UPOrders(){};

        void JsonPack(Json::Value &root);
        bool JsonUnpack(Json::Value &root);
    public:
        static constexpr const char* CMD_WSAPI = "order";//WS
        static constexpr const char* CMD_WEBAPI = "/api/private/orders";//HTTP
        static constexpr const char* CMD_ROBOTAPI = "/api/private/orders";//robot http
};


#endif
