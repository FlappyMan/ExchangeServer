/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-07-02 02:06:39
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/global.h
 */

#ifndef _H_GLOBAL_HEADER_
#define _H_GLOBAL_HEADER_

#include "UProtocol.h"
#include "spdlog/spdlog.h"
#include "./BaseConfig/config.h"
#include "./BaseConfig/RuntimeConfig.h"

#define PRINT_LOG
#define DEBUG

extern string g_dir_root; // 程序当前路径
extern Config g_config;
extern WriteReqCache g_cache_orderthread_resp;
extern WriteReqCache g_cache_orderthread_login;
extern WriteReqCache g_cache_ucthread_login;
extern WriteReqCache g_cache_ctrlthread_login;
extern WriteReqCache g_cache_ctrlthread_push;
extern WriteReqCache g_cache_ucthread_push;
extern ReadCache g_cache_ctrlthread_read;
extern ReadCache g_cache_orderthread_read;
extern ReadCache g_cache_ucthread_read;

#define DEBUG_DEBUG(value) spdlog::debug(value)
#define DEBUG_ERROR(value) spdlog::error(value)
#define DEBUG_INFO(value) spdlog::info(value)

#define KLINE_DATA_LEN 128
class CBaseSession;

enum
{
    DATA_LEN = 256,
    ENC_LEN = 6200,
    DEC_LEN = 6200,
};

class client_pkg_t : public uv_tcp_t
{
public:
    uint64_t ulPkgIndex;
};

class CServerInfo
{
public:
    ConnectServer info;
    uv_connect_t *conn;
    bool bIsValid;
};

typedef struct
{
    client_pkg_t *client;
    char szSession[128];
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

typedef map<client_pkg_t *, CBaseSession *> mapBaseSession; // socket编号-业务处理对象
typedef map<string, CBaseSession *> mapBaseTcpSession;      // token-业务处理对象 适用于tcp

#endif
