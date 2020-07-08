/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-05-21 23:40:19
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Business/BaseRequestOper.h
 */ 
#ifndef _BASE_REQUEST_HEADER_
#define _BASE_REQUEST_HEADER_
#include "../global.h"
#include "../Manager/DispatchManager.h"
typedef struct 
{
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;


class CBaseRequestOper
{
public:
    virtual UProtocolBase* requestOperation() = 0;
    virtual string getOperationToken() = 0;
    uint16_t m_usCMD;
    client_sokt_t *m_client;
    string m_sTag;
    string m_coinA;
    string m_coinB;
    int m_marketID;
};

#endif
