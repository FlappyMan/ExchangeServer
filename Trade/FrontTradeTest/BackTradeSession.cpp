#include "BackTradeSession.h"
#include "ThreadClient.h"

BackTradeSession::BackTradeSession(uv_tcp_t *tcp, UBBlockQueue<UProtocolBase> &qReq) : m_tcp(tcp), m_bLogined(false)
{
    tcp->data = this;
}

BackTradeSession::~BackTradeSession()
{
}

void BackTradeSession::Init()
{
    m_tLast = time(NULL);
}

bool BackTradeSession::IsTimeout(time_t tNow)
{
    return tNow > m_tLast + 50;
}

// return <0: 协议错误，=0:数据包长度不足，>0:已处理掉的数据长度
int BackTradeSession::Read(char *pBuffer, int iDataLen, UBBlockQueue<UProtocolBase> &m_qResponse)
{
    int iTotalDataLen = m_buffer.AppendData(pBuffer, iDataLen);
    for (size_t i = 0; i < iDataLen; i++)
    {
        printf("%.2x",pBuffer[i]);
    }
    cout<<endl;

    uint16_t uiType = 0;
    uint32_t uiValueLen = 0;

    int ret = 0;
    UProtocolBase *pkg = NULL;
    if (m_bLogined == false)
    {
        ret = ProtocolHead(uiType, uiValueLen, m_buffer.Data(), m_buffer.DataLength());
        if (ret <= 0)
            return ret;
        if (uiType != UPLogin::CMD)
            return -1;
        // 校验身份
        // if (LoginCheck(&pkgLogin) == false)
        //     return -1;
        m_bLogined = true;

        m_buffer.RemoveData(uiValueLen);
        return uiValueLen;
    }

    while (true)
    {
        uiType = 0;
        uiValueLen = 0;
        ret = ProtocolHead(uiType, uiValueLen, m_buffer.Data(), m_buffer.DataLength());
        if (ret < 0)
            return ret;
        else if (ret == 0)
            break;
        switch (uiType)
        {
        case UPResponse::CMD:
            pkg = new UPResponse;
            ProtoUnpack(*(UPResponse *)pkg, m_buffer.Data(), uiValueLen);
            m_qResponse.put(pkg);
            break;
        default:
            break;
        }
        m_buffer.RemoveData(uiValueLen);
    }
    return iDataLen;
}

bool BackTradeSession::LoginCheck(UPLogin *pLogin)
{
    Json::Value root;
    if (pLogin->ServerCheck(root, m_pSrv->m_sPublicKey) < 0)
        return false;
    uint64_t uMarketID = root["marketid"].asInt64();
    string sMarketName = root["marketname"].asString();
    g_srv_backtrade.m_mMarket.insert(make_pair(uMarketID, sMarketName));
    m_pSrv->AddMarket(uMarketID, this);
    return true;
}
