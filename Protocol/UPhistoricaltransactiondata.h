#ifndef _UPHISTORICALTRANSACTIONDATA_H_
#define _UPHISTORICALTRANSACTIONDATA_H_
#include "UBaseLib.h"
#include "proto/ukex.historicaltransactiondata.pb.h"
#include "UProtocolBase.h"

class UPHistoricalTransactionData : public UProtocolBase, public ukex::historicaltransactiondata
{
    public:
        UPHistoricalTransactionData(){};
        virtual ~UPHistoricalTransactionData(){};

        void JsonPack(Json::Value &root);
        bool JsonUnpack(Json::Value &root); 
    public:
        static constexpr const char* CMD_WSAPI = "tradelog";//WS
        static constexpr const char* CMD_WEBAPI = "/api/private/tradelog";//HTTP
        static constexpr const char* CMD_ROBOTAPI = "/api/private/tradelog";//robot http
    public:
        string m_sMarketID;
};


#endif
