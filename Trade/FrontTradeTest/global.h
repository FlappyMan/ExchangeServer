

#ifndef _H_GLOBAL_HEADER_
#define _H_GLOBAL_HEADER_


#include "UProtocol.h"
#include "config.h"

class UPMarketAdd : public UProtocolBase
{
    public:
        enum {CMD=1000};

        UPMarketAdd(){};
        ~UPMarketAdd(){};

        void JsonPack(Json::Value &root){};
        bool JsonUnpack(Json::Value &root){};
    public:
        string m_sMarketID;
};

extern string g_dir_root;       // 程序当前路径
extern Config g_config;
extern WriteReqCache g_cache_write_req;
extern ReadCache g_cache_read;

#endif
