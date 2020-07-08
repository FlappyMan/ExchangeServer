/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-11 01:40:17
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Manager/HttpSession.h
 */ 
#ifndef _HTTP_SESSION_HEADER_
#define _HTTP_SESSION_HEADER_
#include "../global.h"
class CHttpSession
{
public:
    CHttpSession();
    ~CHttpSession();
public:
    virtual int OnRecv(client_sokt_t *client,char *pBuffer, int iDataLen);
    bool isRecvFinish(){return m_bRecvFinish;}
    
public:
    CJsonObjectBase *m_pObject;

private:
    void praseAssetsUpdate(string sHttpBody);
    void praseLogin(string sHttpBody);
    void praseRegist(string sHttpBody);
    void praseUpdate(string sHttpBody);
    void prasePay(string sHttpBody);
    void praseAddBindDevice(string sHttpBody,string token,int deadLine);
    void praseRemoveBindDevice(string sHttpBody);
    void praseCheckBindDevice(string sHttpBody);
    void praseAuth(string sHttpBody);
    void praseInfo(string sHttpBody);
    void praseRealInfo(string sHttpBody);
    void praseAddBalance(string sHttpBody);
    void praseReduceBalance(string sHttpBody);
    void praseAllocationBalance(string sHttpBody);
    void praseSetMaster(string sHttpBody);
    void praseCoinAdd(string sHttpBody);
    
private:
    bool m_bRecvFinish;
    UBHttpParser m_httpParser;
};

#endif
