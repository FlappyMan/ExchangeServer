

#ifndef _H_GLOBAL_HEADER_
#define _H_GLOBAL_HEADER_


#include "UProtocol.h"
#include "config.h"

class UPMarketAdd : public UProtocolBase
{
public:
	enum {CMD=MAXCMD+1};

	UPMarketAdd(){};
	~UPMarketAdd(){};

    void JsonPack(Json::Value &root){};
    bool JsonUnpack(Json::Value &root){};
public:
	string m_sMarketID;
    uint64_t m_uiMarketID;
};





extern string g_dir_root;       // 程序当前路径
extern Config g_config;
extern WriteReqCache g_cache_write_req;
extern WriteReqCache g_cache_write_req_Client;
extern WriteReqCache g_cache_write_req_Client_Frame;
extern WriteReqCache g_cache_write_req_BackTrade;
extern ReadCache g_cache_read;


#endif
