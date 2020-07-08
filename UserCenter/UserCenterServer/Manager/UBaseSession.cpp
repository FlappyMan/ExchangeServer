#include "UBaseSession.h"
#include "RequestOperTrade.h"
#include "RequestOperTradeBatch.h"
#include "RequestOperCancelTrade.h"
#include "RequestOperCancelTradeBatch.h"
#include "RequestOperMatchData.h"
#include "RequestAssetsUpdate.h"
#include "TradeModel.hpp"
#include "TradeBatchModel.hpp"
#include "CancelTradeModel.hpp"
#include "CancelTradeBatchModel.hpp"
#include "MatchedDataModel.hpp"
#include "AssetsUpdate.hpp"
#include "../ThreadDBOper.h"

#define DEC_OUT_LEN 6200
#define SRC_IN_LEN 6144
#define BUFF_SIZE 2048
CBaseSession::CBaseSession()
{
    m_buffer.Init(BUFF_SIZE);
    m_uiType = 0;
    m_uiValueLen = 0;
}
CBaseSession::~CBaseSession()
{
}
CBaseRequestOper *CBaseSession::PraseOperation()
{
    UProtocolBase *pkg = NULL;
    CBaseRequestOper *oper = NULL;
    if (m_bIsLogin == false && m_uiType != UPLogin::CMD)
    {
        uv_close((uv_handle_t *)m_client, CThreadTCPOper::cbClosed);
        return NULL;
    }
    
    switch (m_uiType)
    {
    case UPassetsupdate::CMD:
    {
#ifdef PRINT_LOG
        DEBUG_INFO("[UPassetsupdate]:[CMD]");
#endif
        pkg = new UPassetsupdate;
        ProtoUnpack(*(UPassetsupdate *)pkg, m_buffer.Data(), m_uiValueLen);
        CAssetsUpdate model;
        model = (UPassetsupdate *)pkg;
        oper = new CRequestAssetsUpdate(model);
    }
    break;
    case UPLogin::CMD:
    {
#ifdef PRINT_LOG
        DEBUG_INFO("[UPLogin]:[CMD]");
#endif
        UPLogin* pPkg = new UPLogin;
        ProtoUnpack(*(UPLogin *)pPkg, m_buffer.Data(), m_uiValueLen);

        Json::Value root;
        if (pPkg->ServerCheck(root, Config::sRSAPrivateKey, false) == RET_SUCCESS)
        {
            DEBUG_INFO("[CBaseSession::UPLogin::CMD] check success");
            m_bIsLogin = true;
            m_coinA = root["coinA"].asString();
            m_coinB = root["coinB"].asString();
            m_marketID = root["mid"].asUInt64();
            
            Json::FastWriter writer;
            string ret = writer.write(root);
            cout<<"UPLogin Info = "<<ret<<endl;
        }
        else
        {
            DEBUG_ERROR("[CBaseSession::UPLogin::CMD] token error ");
            uv_close((uv_handle_t *)m_client, CThreadTCPOper::cbClosed);
        }
    }
    break;
    default:
    {
        return NULL;
    }
    break;
    }
    if (NULL != oper)
    {
        oper->m_coinA = m_coinA;
        oper->m_coinB = m_coinB;
        oper->m_marketID = m_marketID;
        oper->m_client = m_client;
        oper->m_usCMD = (uint16_t)m_uiType;
        oper->m_sTag = pkg->m_sTag;
        m_sToken = oper->getOperationToken();
    }

    if (NULL != pkg)
    {
        delete pkg;
        pkg = NULL;
    }
    return oper;
}
int CBaseSession::OnRecv(client_sokt_t *client, char *pBuffer, int iDataLen)
{
#ifdef PRINT_LOG
    DEBUG_INFO("[CBaseSession]:[OnRecv]");
#endif
    int iTotalDataLen = m_buffer.AppendData(pBuffer, iDataLen);
   
    int ret = ProtocolHead(m_uiType, m_uiValueLen, m_buffer.Data(), m_buffer.DataLength());
    if (ret == RET_HOLD)
    {
#ifdef PRINT_LOG
        DEBUG_INFO("[CBaseSession]:[RET_HOLD]");
#endif
        return ret;
    }
    else if (ret == RET_FAILED)
    {
#ifdef PRINT_LOG
        DEBUG_INFO("[CBaseSession]:[RET_FAILED]");
#endif
        return ret;
    }
    else
    {
#ifdef PRINT_LOG
        DEBUG_INFO("[CBaseSession]:[RET_SUCCESS]");
#endif
        CBaseRequestOper *pOper = PraseOperation();
        if (NULL != pOper)
        {
            CThreadDBOper::m_qDBOper.put(pOper);
        }
        m_buffer.RemoveData(m_uiValueLen);
        return RET_SUCCESS;
    }
}
