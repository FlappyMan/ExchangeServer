/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-07-03 00:54:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/Manager/UBaseSession.h
 */ 
#ifndef _BASE_SESSION_HEADER_
#define _BASE_SESSION_HEADER_

#include "../global.h"
#include "CancelTradeBatchModel.hpp"
#include "CancelTradeModel.hpp"
#include "MatchedDataModel.hpp"
#include "TradeBatchModel.hpp"
#include "TradeModel.hpp"
#include "ResponseModel.hpp"
#include "FileRequest.hpp"
#include "GetOrderInfoModel.hpp"

class CBaseSession
{
public:
    CBaseSession();
    ~CBaseSession();

public:
    void requestOperation();
    string PraseOperation();
    virtual int OnRecv(client_pkg_t *client, char *pBuffer, int iDataLen, bool bTokenSession);
    bool isRecvFinish() { return m_bRecvFinish; }
    bool isTokenSession() { return m_bTokenSession; }
    bool isMoveToUC() { return m_bMoveToUC; }
    bool setMoveToUC(bool b = true) { m_bMoveToUC = b; }
    char *getBufferValue() { return m_buffer.Data(); }
    string getToken() { return m_sToken; }
    int getStatus() { return m_iStatus; }
    void setRespData(string data) {m_sRespData = data;}
    string getRespData(){return m_sRespData;}
    void setStatus(int iStatus) { m_iStatus = iStatus; }
    uint32_t getBufferLen() { return m_buffer.DataLength(); }
    client_pkg_t *getClientFd() { return m_client; }
    

public:
    /* 用于接受到客户端请求以后，重新打包成assetsupdate格式的数据包 */
    UVWriteReq *rePackPkg();
    UPFileData *rePackFilePkg();
    UVWriteReq *rePackPkg(CTradeModel model);
    UVWriteReq *rePackPkg(CTradeBatchModel model);
    UVWriteReq *rePackPkg(CCancelTradeModel model);
    UVWriteReq *rePackPkg(CCancelTradeBatchModel model);

public:
    CTradeModel getTradeModel();
    CTradeBatchModel getTradeBatchModel();
    CCancelTradeModel getCancelTradeModel();
    CCancelTradeBatchModel getCancelTradeBatchModel();
    CMatchedDataModel getMatchDataModel();
    CResponseModel getResponseModel();
    CGetOrderInfoModel getGetOrderInfoModel();
public : bool m_bClientResp;

public:
    uint32_t m_uiValueLen;
    uint16_t m_usType;

private:
    UBBuffer m_buffer;
    client_pkg_t *m_client;
    string m_sToken;
    int m_iStatus;
    string m_sRespData;
    bool m_bMoveToUC;
    bool m_bRecvFinish;
    bool m_bTokenSession;
};

#endif
