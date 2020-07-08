/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:56
 * @LastEditTime: 2020-06-09 20:04:51
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/global.h
 */ 


#ifndef _H_GLOBAL_HEADER_
#define _H_GLOBAL_HEADER_


#include "UProtocol.h"
#include "./BaseConfig/config.h"
#include "./BaseConfig/RuntimeConfig.h"
#include "./Model/UPJsonReflex.hpp"
#include "spdlog/spdlog.h"
#define PRINT_LOG

extern string g_dir_root;       // 程序当前路径
extern Config g_config;
extern WriteReqCache g_cache_write_req; // 用户中心tcp数据发送缓冲
extern WriteReqCache g_cache_http_req;  // 用户中心http数据发送缓冲

extern ReadCache g_cache_http_read;
extern ReadCache g_cache_tcp_read;
#define TOKEN_MD5_KEY  "UserCenterKey"
#define DEBUG_DEBUG(value) spdlog::debug(value)
#define DEBUG_ERROR(value) spdlog::error(value)
#define DEBUG_INFO(value) spdlog::info(value)
enum
{
    DATA_LEN = 256,
    ENC_LEN = 6200,
    DEC_LEN = 6200,
};

enum{
    DB_ACCOUNT = 1,
    DB_ADMIN,
    DB_ASSETS,
    DB_Base,
    DB_EXASSETS,
    DB_KYC,
    DB_OTC,
    DB_OTHER,
    DB_SA,
    DB_TRADE,
}DBName;

#endif
