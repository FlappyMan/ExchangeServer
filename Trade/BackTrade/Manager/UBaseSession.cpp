#include "UBaseSession.h"
#include "../ThreadOrder.h"
#include "BaseRequestOper.h"
#include "RequestOperTrade.h"
#include "RequestOperTradeBatch.h"
#include "RequestOperCancelTrade.h"
#include "RequestOperCancelTradeBatch.h"
#include "RequestOperResponseData.h"
#include "RequestGetOrderInfo.h"


#include "../ThreadUserCenter.h"

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

CBaseSession::CBaseSession()
{
    m_buffer.Init(2048);
    m_uiValueLen = 0;
    m_usType = 0;
    m_bRecvFinish = false;
    m_bMoveToUC = false;
    m_bClientResp = false;
}
CBaseSession::~CBaseSession()
{
}

int CBaseSession::OnRecv(client_pkg_t *client, char *pBuffer, int iDataLen, bool bTokenSession)
{
#ifdef PRINT_LOG
    cout << "[CBaseSession]:[OnRecv]" << endl;
#endif
    int iTotalDataLen = m_buffer.AppendData(pBuffer, iDataLen);
   
    int ret = ProtocolHead(m_usType, m_uiValueLen, m_buffer.Data(), m_buffer.DataLength());
    if (ret == RET_HOLD)
    {
#ifdef PRINT_LOG
        cout << "[CBaseSession]:[RET_HOLD]" << endl;
#endif
        m_bRecvFinish = true;
        return ret;
    }
    else if (ret == RET_FAILED)
    {
#ifdef PRINT_LOG
        cout << "[CBaseSession]:[RET_FAILED]" << endl;
#endif
        return ret;
    }
    else
    {
#ifdef PRINT_LOG
        cout << "[CBaseSession]:[RET_SUCCESS]" << endl;
#endif
        m_bRecvFinish = true;
        m_client = client;
        m_sToken = PraseOperation();
        if (true == bTokenSession)
        {
            CThreadOrder::g_NodeSession.put(this);
        }

        return RET_SUCCESS;
    }
}
void CBaseSession::requestOperation()
{
    UProtocolBase *pkg = NULL;
    CBaseRequestOper *oper = NULL;
    switch (m_usType)
    {
    case UPUptrade::CMD:
    {
        pkg = new UPUptrade;
        ProtoUnpack(*(UPUptrade *)pkg, m_buffer.Data(), m_uiValueLen);
        CTradeModel model;
        model = (UPUptrade *)pkg;
        oper = new CRequestOperTrade(model);
    }
    break;
    case UPUptradebatch::CMD:
    {
        pkg = new UPUptradebatch;
        ProtoUnpack(*(UPUptradebatch *)pkg, m_buffer.Data(), m_uiValueLen);
        CTradeBatchModel model;
        model = (UPUptradebatch *)pkg;
        oper = new CRequestOperTradeBatch(model);
    }
    break;
    case UPCanceltrade::CMD:
    {
        pkg = new UPCanceltrade;
        ProtoUnpack(*(UPCanceltrade *)pkg, m_buffer.Data(), m_uiValueLen);
        CCancelTradeModel model;
        model = (UPCanceltrade *)pkg;
        oper = new CRequestOperCancelTrade(model);
    }

    break;
    case UPCanceltradebatch::CMD:
    {
        pkg = new UPCanceltradebatch;
        ProtoUnpack(*(UPCanceltradebatch *)pkg, m_buffer.Data(), m_uiValueLen);
        CCancelTradeBatchModel model;
        model = (UPCanceltradebatch *)pkg;
        oper = new CRequestOperCancelTradeBatch(model);
    }
    break;
    case UPResponse::CMD:
    {
        pkg = new UPResponse;
        ProtoUnpack(*(UPResponse *)pkg, m_buffer.Data(), m_uiValueLen);
        CResponseModel model;
        model = (UPResponse *)pkg;
        oper = new CRequestOperResponseData(model);
        m_iStatus = model.status;
    }
    break;
    default:break;
    }
    oper->requestOperation();
    if (NULL != pkg)
    {
        delete pkg;
        pkg = NULL;
    }

    if (NULL != oper)
    {
        delete[] oper;
        oper = NULL;
    }
}
UVWriteReq * CBaseSession::rePackPkg()
{
    UPResponse *rspPkg = new UPResponse;
    rspPkg->set_token(m_sToken);
    rspPkg->set_status(m_iStatus);
    if (m_sRespData.length() != 0)
    {
        rspPkg->set_data(m_sRespData);
    }
    else
    {
        rspPkg->set_data(m_iStatus == 0 ? "failed" : "success");
    }

    const int BUFFSIZE = ProtoPackSize<UPResponse>(*rspPkg);
    UVWriteReq *write = g_cache_orderthread_resp.Get(BUFFSIZE);
    write->buf.len = ProtoPack<UPResponse>(write->buf.base, BUFFSIZE, *rspPkg);
    write->pkg = rspPkg;
    return write;
}
UPFileData* CBaseSession::rePackFilePkg()
{
    UPFileRequest* req = new UPFileRequest;
    ProtoUnpack(*(UPFileRequest *)req, m_buffer.Data(), m_uiValueLen);
    CFileRequest model;
    model = (UPFileRequest *)req;

    delete req;
    req = NULL;

    FILE *fp;
    string filepath = model.path+model.name;
    if ((fp = fopen(filepath.c_str(), "a+")) == NULL)
    {
        return NULL;
    }

    int iTotalSize = model.offset + model.length;
    char *pRecord = new char[KLINE_DATA_LEN*iTotalSize];
	memset(pRecord,0,KLINE_DATA_LEN*iTotalSize);
    int iRet = fread(pRecord, 1, KLINE_DATA_LEN*iTotalSize, fp);
    pRecord += (KLINE_DATA_LEN*model.offset);
	
    UPFileData* pkg = new UPFileData;
    pkg->set_name(model.name);
    pkg->set_data(model.path);
    pkg->set_modifytime(model.modifytime);
    pkg->set_offset(model.offset);
    pkg->set_length(model.length);
    pkg->set_token(model.token);
    pkg->set_data(pRecord);

    fclose(fp);
    delete pRecord;

    return pkg;
}
UVWriteReq * CBaseSession::rePackPkg(CCancelTradeBatchModel model)
{
    UPassetsupdate* pkg = new UPassetsupdate;
    pkg->set_uid(model.uid);
    pkg->set_paypassword("");
    pkg->set_opertype(OPER_TYPE_CANCELTRADEE_BATCH);
    pkg->set_operid(0);// 支付密码生效
    for (int i = 0; i < model.orders.size(); i++)
    {
        CCancelTradeInfoModel infoModel = model.orders[i];
        UPassetsupdate::List *pList = pkg->add_list();
        pList->set_uid(model.uid);
        pList->set_type_id(infoModel.orderid);
        pList->set_type(infoModel.type == 1 ? OPER_TYPE_COMMISION_BUY : OPER_TYPE_COMMISION_SELL);
        pList->set_coinid(stoi(infoModel.type == 1 ? Config::sCoinB : Config::sCoinA));
        string sAmount = CTool::mutipLongDouble(infoModel.price, infoModel.num);
        string fee = infoModel.type == 1 ? CRuntimeConfig::market.m_sBuyFee : CRuntimeConfig::market.m_sSellFee;
        string feeValue = CTool::mutipLongDouble(sAmount, fee);
        string sTotal = CTool::addLongDouble(sAmount, feeValue);
        pList->set_usable(sTotal);
        pList->set_fronzen("-" + sTotal);
        pList->set_lockup("0.0");
    }

    const int BUFFSIZE = ProtoPackSize<UPassetsupdate>(*pkg);
    UVWriteReq *write = g_cache_orderthread_resp.Get(BUFFSIZE);
    write->buf.len = ProtoPack<UPassetsupdate>(write->buf.base, BUFFSIZE, *pkg);
    write->pkg = pkg;
    return write;
}
UVWriteReq * CBaseSession::rePackPkg(CCancelTradeModel model)
{
    UPassetsupdate* pkg = new UPassetsupdate;
    pkg->set_uid(model.uid);
    pkg->set_paypassword("");
    pkg->set_opertype(OPER_TYPE_CANCELTRADE);
    pkg->set_operid(0);// 支付密码生效
    UPassetsupdate::List* pList = pkg->add_list();
    pList->set_uid(model.uid);
    pList->set_type_id(model.canceltradeid);
    pList->set_type(model.type == 1?OPER_TYPE_COMMISION_BUY:OPER_TYPE_COMMISION_SELL);
    pList->set_coinid(stoi(model.type == 1?Config::sCoinB:Config::sCoinA));
    string sAmount = CTool::mutipLongDouble(model.price,model.num);
    string fee = model.type == 1?CRuntimeConfig::market.m_sBuyFee:CRuntimeConfig::market.m_sSellFee;
    string feeValue = CTool::mutipLongDouble(sAmount,fee);
    string sTotal = CTool::addLongDouble(sAmount,feeValue);
    pList->set_usable(sTotal);
    pList->set_fronzen("-"+sTotal);
    pList->set_lockup("0.0");

    const int BUFFSIZE = ProtoPackSize<UPassetsupdate>(*pkg);
    UVWriteReq *write = g_cache_orderthread_resp.Get(BUFFSIZE);
    write->buf.len = ProtoPack<UPassetsupdate>(write->buf.base, BUFFSIZE, *pkg);
    write->pkg = pkg;
    return write;
}
UVWriteReq * CBaseSession::rePackPkg(CTradeModel model)
{
    UPassetsupdate* pkg = new UPassetsupdate;
    pkg->set_uid(model.uid);
    pkg->set_paypassword(model.paypassword);
    pkg->set_opertype(OPER_TYPE_UPTRADE);
    pkg->set_operid(1);// 支付密码生效
    UPassetsupdate::List* pList = pkg->add_list();
    pList->set_uid(model.uid);
    pList->set_type_id(model.orderid);
    pList->set_type(model.type);
    pList->set_coinid(stoi(model.type == 1?Config::sCoinB:Config::sCoinA));
    string sAmount = CTool::mutipLongDouble(model.price,model.totalNum);
    string fee = model.type == 1?CRuntimeConfig::market.m_sBuyFee:CRuntimeConfig::market.m_sSellFee;
    string feeValue = CTool::mutipLongDouble(sAmount,fee);
    string sTotal = CTool::addLongDouble(sAmount,feeValue);
    pList->set_usable("-"+sTotal);
    pList->set_fronzen(sTotal);
    pList->set_lockup("0.0");

    const int BUFFSIZE = ProtoPackSize<UPassetsupdate>(*pkg);
    UVWriteReq *write = g_cache_orderthread_resp.Get(BUFFSIZE);
    write->buf.len = ProtoPack<UPassetsupdate>(write->buf.base, BUFFSIZE, *pkg);
    write->pkg = pkg;
    return write;
}
UVWriteReq * CBaseSession::rePackPkg(CTradeBatchModel model)
{
    UPassetsupdate* pkg = new UPassetsupdate;
    pkg->set_uid(model.uid);
    pkg->set_paypassword(model.paypassword);
    pkg->set_opertype(OPER_TYPE_UPTRADE_BATCH);
    pkg->set_operid(1);// 支付密码生效
    for (int i = 0; i < model.trades.size(); i++)
    {
        CTradeInfoModel infoModel = model.trades[i];
        UPassetsupdate::List *pList = pkg->add_list();
        pList->set_uid(model.uid);
        pList->set_type_id(infoModel.orderid);
        pList->set_type(infoModel.type == 1 ? OPER_TYPE_COMMISION_BUY : OPER_TYPE_COMMISION_SELL);
        pList->set_coinid(stoi(infoModel.type == 1 ? Config::sCoinB : Config::sCoinA));
        string sAmount = CTool::mutipLongDouble(infoModel.price, infoModel.num);
        string fee = infoModel.type == 1 ? CRuntimeConfig::market.m_sBuyFee : CRuntimeConfig::market.m_sSellFee;
        string feeValue = CTool::mutipLongDouble(sAmount, fee);
        string sTotal = CTool::addLongDouble(sAmount, feeValue);
        pList->set_usable("-" + sTotal);
        pList->set_fronzen(sTotal);
        pList->set_lockup("0.0");
    }

    const int BUFFSIZE = ProtoPackSize<UPassetsupdate>(*pkg);
    UVWriteReq *write = g_cache_orderthread_resp.Get(BUFFSIZE);
    write->buf.len = ProtoPack<UPassetsupdate>(write->buf.base, BUFFSIZE, *pkg);
    write->pkg = pkg;
    return write;
}

CTradeModel CBaseSession::getTradeModel()
{
    UProtocolBase *pkg = new UPUptrade;
    ProtoUnpack(*(UPUptrade *)pkg, m_buffer.Data(), m_uiValueLen);
    CTradeModel model;
    model = (UPUptrade *)pkg;
    delete pkg;
    pkg = NULL;
    return model;
}
CTradeBatchModel CBaseSession::getTradeBatchModel()
{
    UProtocolBase *pkg = new UPUptradebatch;
    ProtoUnpack(*(UPUptradebatch *)pkg, m_buffer.Data(), m_uiValueLen);
    CTradeBatchModel model;
    model = (UPUptradebatch *)pkg;
    delete pkg;
    pkg = NULL;
    return model;
}
CCancelTradeModel CBaseSession::getCancelTradeModel()
{
    UProtocolBase *pkg = new UPCanceltrade;
    ProtoUnpack(*(UPCanceltrade *)pkg, m_buffer.Data(), m_uiValueLen);
    CCancelTradeModel model;
    model = (UPCanceltrade *)pkg;
    delete pkg;
    pkg = NULL;
    return model;
}
CCancelTradeBatchModel CBaseSession::getCancelTradeBatchModel()
{
    UProtocolBase *pkg = new UPCanceltradebatch;
    ProtoUnpack(*(UPCanceltradebatch *)pkg, m_buffer.Data(), m_uiValueLen);
    CCancelTradeBatchModel model;
    model = (UPCanceltradebatch *)pkg;
    delete pkg;
    pkg = NULL;
    return model;
}
CResponseModel CBaseSession::getResponseModel()
{
    UProtocolBase *pkg = new UPResponse;
    ProtoUnpack(*(UPResponse *)pkg, m_buffer.Data(), m_uiValueLen);
    CResponseModel model;
    model = (UPResponse *)pkg;
    delete pkg;
    pkg = NULL;
    return model;
}
CGetOrderInfoModel CBaseSession::getGetOrderInfoModel()
{
    UProtocolBase *pkg = new UPGetOrderInfo;
    ProtoUnpack(*(UPGetOrderInfo *)pkg, m_buffer.Data(), m_uiValueLen);
    CGetOrderInfoModel model;
    model = (UPGetOrderInfo *)pkg;
    delete pkg;
    pkg = NULL;
    return model;
}
string CBaseSession::PraseOperation()
{
    string retToken = "";
    UProtocolBase *pkg = NULL;
    CBaseRequestOper *oper = NULL;
    switch (m_usType)
    {
    case UPUptrade::CMD:
    {
#ifdef PRINT_LOG
        cout << "[CBaseSession]:[UPUptrade]:[CMD]" << endl;
#endif
        pkg = new UPUptrade;
        ProtoUnpack(*(UPUptrade *)pkg, m_buffer.Data(), m_uiValueLen);
        CTradeModel model;
        model = (UPUptrade *)pkg;
        oper = new CRequestOperTrade(model);
    }
    break;
    case UPUptradebatch::CMD:
    {
#ifdef PRINT_LOG
        cout << "[UPUptradebatch]:[CMD]" << endl;
#endif
        pkg = new UPUptradebatch;
        ProtoUnpack(*(UPUptradebatch *)pkg, m_buffer.Data(), m_uiValueLen);
        CTradeBatchModel model;
        model = (UPUptradebatch *)pkg;
        oper = new CRequestOperTradeBatch(model);
    }
    break;
    case UPCanceltrade::CMD:
    {
#ifdef PRINT_LOG
        cout << "[UPCanceltrade]:[CMD]" << endl;
#endif
        pkg = new UPCanceltrade;
        ProtoUnpack(*(UPCanceltrade *)pkg, m_buffer.Data(), m_uiValueLen);
        CCancelTradeModel model;
        model = (UPCanceltrade *)pkg;
        oper = new CRequestOperCancelTrade(model);
    }

    break;
    case UPCanceltradebatch::CMD:
    {
#ifdef PRINT_LOG
        cout << "[UPCanceltradebatch]:[CMD]" << endl;
#endif
        pkg = new UPCanceltradebatch;
        ProtoUnpack(*(UPCanceltradebatch *)pkg, m_buffer.Data(), m_uiValueLen);
        CCancelTradeBatchModel model;
        model = (UPCanceltradebatch *)pkg;
        oper = new CRequestOperCancelTradeBatch(model);
    }
    break;
    case UPResponse::CMD:
    {
#ifdef PRINT_LOG
        cout << "[UPResponse]:[CMD]" << endl;
#endif
        pkg = new UPResponse;
        ProtoUnpack(*(UPResponse *)pkg, m_buffer.Data(), m_uiValueLen);
        CResponseModel model;
        model = (UPResponse *)pkg;
        oper = new CRequestOperResponseData(model);
        m_iStatus = model.status;
    }
    break;
    case UPFileRequest::CMD:
    {
#ifdef PRINT_LOG
        cout << "[UPFileRequest]:[CMD]" << endl;
#endif
    }
    break;
    case UPGetOrderInfo::CMD:
    {
        pkg = new UPGetOrderInfo;
        ProtoUnpack(*(UPGetOrderInfo *)pkg, m_buffer.Data(), m_uiValueLen);
        CGetOrderInfoModel model;
        model = (UPGetOrderInfo *)pkg;
        oper = new CRequestGetOrderInfo(model);
    }
    break;
    default:
    {
#ifdef PRINT_LOG
        cout << "[default]:[" << m_usType << "]" << endl;
#endif
    }
    break;
    }
    if (NULL != oper)
    {
        retToken = oper->getRequestToken();
    }
    if (NULL != pkg)
    {
        delete pkg;
        pkg = NULL;
    }

    if (NULL != oper)
    {
        delete oper;
        oper = NULL;
    }
    return retToken;
}
