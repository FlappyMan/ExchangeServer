/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-05-21 23:36:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Manager/UBaseSession.h
 */
#ifndef _BASE_SESSION_HEADER_
#define _BASE_SESSION_HEADER_
#include "../global.h"
// #include "BaseRequestOper.h"
class CBaseRequestOper;

class CBaseSession
{
public:
    CBaseSession();
    ~CBaseSession();

public:
    virtual int OnRecv(client_sokt_t *client, char *pBuffer, int iDataLen);
    string getToken(){return m_sToken;}

public:
    client_sokt_t *m_client;
    
private:
    CBaseRequestOper *PraseOperation();

private:
    UBBuffer m_buffer;
    uint16_t m_uiType;
    uint32_t m_uiValueLen;
    bool m_bIsLogin;
    string m_sToken;
    string m_coinA;
    string m_coinB;
    int m_marketID;
};

#endif
