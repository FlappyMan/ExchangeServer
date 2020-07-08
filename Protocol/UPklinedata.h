#ifndef _UPKLINEDATA_H_
#define _UPKLINEDATA_H_
#include "UBaseLib.h"
#include "proto/ukex.klinedata.pb.h"
#include "UProtocolBase.h"

class UPKlinedata : public UProtocolBase, public ukex::klinedata
{
    public:
        UPKlinedata(){};
        virtual ~UPKlinedata(){};

        void JsonPack(Json::Value &root);
        bool JsonUnpack(Json::Value &root);
    public:
        static constexpr const char* CMD_WSAPI = "kline";//WS
        static constexpr const char* CMD_WEBAPI = "/api/private/kline";//HTTP
        static constexpr const char* CMD_ROBOTAPI = "/api/private/upTrklineade";//robot http


        enum{KLINE_FILE_RECORD_LENGTH=200,};        // K线数据每条记录在记录文件中最大长度
    public:
        string m_sMarketID;
};


#endif
