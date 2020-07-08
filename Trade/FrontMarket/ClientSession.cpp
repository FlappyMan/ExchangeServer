#include "ClientSession.h"
#include "ThreadClient.h"

ClientSession::ClientSession(uv_tcp_t* tcp,char *pBuffer,int iBufferSize):
    m_tcp(tcp),m_pBuffer(pBuffer),m_iBufferSize(iBufferSize),m_tLast(0),m_bUpgrade(false),m_iFrameFinFlag(0),m_iFrameHeaderLen(0)
{

}

ClientSession::~ClientSession()
{

}

void ClientSession::Init()
{
    m_tLast=time(NULL);

    m_ubBuffer.Init(SIZE_BUFFER_8k);
    m_ws.Init(HTTP_REQUEST);
}

void ClientSession::Destroy()
{
    m_ws.Destroy();
}

bool ClientSession::IsTimeout(time_t tNow)
{
    return tNow > m_tLast+30;
}

void ClientSession::SendPkg(UProtocolBase *pkg)
{
    m_tLast=time(NULL);

    switch(pkg->m_uiType)
    {
        case UPKlinedata::CMD :
            _SendPkg_KLine((UPKlinedata*)pkg);
            break;
        case UPAllmarketinfo::CMD :
            _SendPkg_AllMarketInfo((UPAllmarketinfo*)pkg);
            break;
        case UPDepthdata::CMD : 
            _SendPkg_DepthData((UPDepthdata*)pkg);
            break;
        case UPMatchedData::CMD :
            _SendPkg_MatchedDat((UPMatchedData*)pkg);
            break;
        default:
            break;
    }
}


void ClientSession::_SendPkg_KLine(UPKlinedata *pkg)
{
    Client_Write<UPKlinedata>(m_tcp,pkg);
}

void ClientSession::_SendPkg_AllMarketInfo(UPAllmarketinfo *pkg)
{
    Client_Write<UPAllmarketinfo>(m_tcp,pkg);
}

void ClientSession::_SendPkg_DepthData(UPDepthdata *pkg)
{
    Client_Write<UPDepthdata>(m_tcp,pkg);

}

void ClientSession::_SendPkg_MatchedDat(UPMatchedData *pkg)
{
    Client_Write<UPMatchedData>(m_tcp,pkg);
}

void cbWrited(uv_write_t *write_req, int status)
{
    if (status != 0)return;else cout<<"send success"<<endl;
    g_cache_write_req.Free((UVWriteReq*)write_req);
}

// return <0: 协议错误，=0:数据包长度不足，>0:已处理掉的数据长度
int ClientSession::Read(char *pBuffer,int iDataLen)
{
    int ret=m_ws.Readed(pBuffer,iDataLen);
    if(ret<0)return -1;
    if(ret>0)return ret;
    m_tLast =time(NULL)+30;
    if(m_ws.IsWebSocket() && !m_bUpgrade)
    {
        m_bUpgrade = true;
        string sOutHeader;
        m_ws.WSReplyHeader(sOutHeader);

        UVWriteReq* req= g_cache_write_req.Get(SIZE_BUFFER_2k);
        memcpy(req->buf.base,(const char *)sOutHeader.data(),sOutHeader.length());
        req->buf.len = sOutHeader.length();
        uv_write((uv_write_t*)req, (uv_stream_t*)m_tcp, (uv_buf_t*)&req->buf, 1, cbWrited);
    }else{
        int iTotalDataLen=m_ubBuffer.AppendData(pBuffer,iDataLen);
        ret=_FrameParse();
        if(ret<=0)return ret;
        else return iDataLen;
    }
}

void cbWritedReply(uv_write_t *write_req, int status)
{
    if (status != 0)return;else cout<<"send server replyFrame success"<<endl;
    free(write_req);
}

// return <0: 协议错误，=0:数据包长度不足，>0:已处理掉的数据长度
int ClientSession::_FrameParse()
{
    UBWsParserFrameType wsft;
    int ret=m_ws.WSGetFrame(&wsft,(uint8_t*)m_pBuffer,m_iBufferSize,(uint8_t*)m_ubBuffer.Data(),m_ubBuffer.DataLength(), m_iFrameFinFlag, m_iFrameHeaderLen);
    if(ret<=0)return ret;

    m_ubBuffer.RemoveData(ret+m_iFrameHeaderLen);
    if(m_iFrameFinFlag == 1)
    {
        m_ws.m_ubPayloadBuf.RemoveData(ret);
        
        if(wsft==UBWsParserFrameType::PING_FRAME || wsft==UBWsParserFrameType::PONG_FRAME)return ret;

        cout<<"Frame data : "<<m_pBuffer<<endl;
        
        enum{FRAME_SIZE=1024};
        char g_sReplyFrame[FRAME_SIZE] = {0};
        size_t rt = m_ws.WSSetFrame(TEXT_FRAME,(uint8_t*)m_pBuffer,ret,(uint8_t*)g_sReplyFrame,FRAME_SIZE);

        uv_write_t* req = new uv_write_t;
        req->data = (void*)g_sReplyFrame;
        uv_buf_t send_buf = {
            .base = (char*)g_sReplyFrame,
            .len = rt
        };
        uv_write((uv_write_t*)req, (uv_stream_t*)m_tcp, (uv_buf_t*)&send_buf, 1, cbWritedReply);

        Json::Value root;
        if(UBJsonUnpack(root,m_pBuffer,ret)==false){return -3;}
        if(root.isNull())return -2;

        if(!root["req"].isNull())
        {
            string sReq = root["req"].asString();
            return _Requst(sReq);
        }
        else if(!root["sub"].isNull())
        {
            string sSub = root["sub"].asString();
            return _Subscribe(sSub);
        }
        else if(!root["unsub"].isNull())
        {
            string sUnsub = root["unsub"].asString();
            return _Unsubscribe(sUnsub);
        }
        else if(!root["opt"].isNull())
        {
            return _Opt(root);
        }
        else
        {
            return -3;
        }
    }
    return 0; 
}

// 订阅请求
int ClientSession::_Subscribe(string &str)
{
    vector<string> v;
    UBStringSplit(v,str,'.');
    if(v.size()<=1)return 0;

    if(v[1].compare("markets")==0)
    {
        m_subs.m_currency=CurrencyFromString(v[2]);
    }
    else if(v[1].compare("depth")==0)
    {
        if(m_subs.m_depth.length()>0)g_srv_client.Unsubscribe(this,SUBSCRIBE_TYPE_DEPTH,m_subs.m_depth);
        m_subs.m_depth=v[2];
        g_srv_client.Subscribe(this,SUBSCRIBE_TYPE_DEPTH,m_subs.m_depth);
    }
    else if(v[1].compare("tradelog")==0)
    {
        if(m_subs.m_tradelog.length()>0)g_srv_client.Unsubscribe(this,SUBSCRIBE_TYPE_TRADELOG,m_subs.m_tradelog);
        m_subs.m_tradelog=v[2];
        g_srv_client.Subscribe(this,SUBSCRIBE_TYPE_TRADELOG,m_subs.m_tradelog);
    }
    else if(v[1].compare("kline")==0)
    {
        if(m_subs.m_kline.length()>0)g_srv_client.Unsubscribe(this,SUBSCRIBE_TYPE_KLINE,m_subs.m_kline);
        m_subs.m_kline=v[2];
        g_srv_client.Subscribe(this,SUBSCRIBE_TYPE_KLINE,m_subs.m_kline);
    }
    return 0;
}

int ClientSession::_Unsubscribe(string &str)
{
    vector<string> v;
    UBStringSplit(v,str,'.');
    if(v.size()<=0)return 0;
    if(v.size()<1)return 0;

    if(v[1].compare("markets")==0)
    {
        m_subs.m_currency=CURRENCY_UNKNOWN;
    }
    else if(v[1].compare("depth")==0)
    {
        g_srv_client.Unsubscribe(this,SUBSCRIBE_TYPE_DEPTH,m_subs.m_depth);
        m_subs.m_depth=MARKET_UNKNOWN;
    }
    else if(v[1].compare("tradelog")==0)
    {
        g_srv_client.Unsubscribe(this,SUBSCRIBE_TYPE_TRADELOG,m_subs.m_tradelog);
        m_subs.m_tradelog=MARKET_UNKNOWN;
    }
    else if(v[1].compare("kline")==0)
    {
        g_srv_client.Unsubscribe(this,SUBSCRIBE_TYPE_KLINE,m_subs.m_kline);
        m_subs.m_kline=KLINE_UNKNOWN;
    }
    return 0;
}

int ClientSession::_Requst(string &str)
{
    vector<string> v;
    UBStringSplit(v,str,'.');
    if(v.size()!=5)return -1;

    if(v[1].compare("kline")!=0)return -2;

    //  发送指定的market kline 数据给客户端
    string &sMarketID=v[2];
    string &sKline=v[3];
    string &sEndTime=v[4];

    struct tm _tm;
    time_t tNow=time(NULL);
    localtime_r(&tNow,&_tm);

    string sKlineFilename;				// 根据 backtrade 的文件生成逻辑
    string sFullPath=g_config.m_sKLineDataDir+sKlineFilename;

    time_t tEndTime=strtol(sEndTime.c_str(),NULL,10);
    localtime_r(&tEndTime,&_tm);
    _tm.tm_mon=1;
    _tm.tm_mday=1;
    _tm.tm_hour=0;
    _tm.tm_min=0;
    _tm.tm_sec=0;
    time_t tBeginTime=mktime(&_tm);

    enum
    {
        RECORD_LINE_LENGTH=200,			// 每条记录固定长度 200bytes，每年一个文件
    };
    uint64_t uiMaxSize=RECORD_LINE_LENGTH * 200;		// 200 条 

    uint64_t offset=0;
    if(sKline==KLINE_STR[KLINE_60S])
    {
        offset=RECORD_LINE_LENGTH * (tEndTime-tBeginTime)/60;
    }
    else if(sKline==KLINE_STR[KLINE_5M])
    {
        offset=RECORD_LINE_LENGTH * (tEndTime-tBeginTime)/(60*5);
    }
    else if(sKline==KLINE_STR[KLINE_15M])
    {
        offset=RECORD_LINE_LENGTH * (tEndTime-tBeginTime)/(60*15);
    }
    else if(sKline==KLINE_STR[KLINE_30M])
    {
        offset=RECORD_LINE_LENGTH * (tEndTime-tBeginTime)/(60*30);
    }
    else if(sKline==KLINE_STR[KLINE_1H])
    {
        offset=RECORD_LINE_LENGTH * (tEndTime-tBeginTime)/(60*60);
    }
    else if(sKline==KLINE_STR[KLINE_6H])
    {
        offset=RECORD_LINE_LENGTH * (tEndTime-tBeginTime)/(60*60*6);
    }
    else if(sKline==KLINE_STR[KLINE_1D])
    {
        offset=RECORD_LINE_LENGTH * (tEndTime-tBeginTime)/(60*60*24);
    }
    else if(sKline==KLINE_STR[KLINE_1W])
    {
        offset=RECORD_LINE_LENGTH * (tEndTime-tBeginTime)/(60*60*24*7);
    }
    else if(sKline==KLINE_STR[KLINE_1M])
    {
        offset=RECORD_LINE_LENGTH * (tEndTime-tBeginTime)/(60*60*24*30);
    }

    FILE *fp=fopen(sFullPath.c_str(),"r");
    if(fp==NULL)return -3;

    fseek(fp, 0L, SEEK_END);
    uint64_t uiSize= ftell(fp);
    fseek(fp,offset,SEEK_SET);

    if(offset+uiMaxSize>uiSize)uiMaxSize=uiSize-offset;
    uiMaxSize=uiMaxSize-(uiMaxSize%RECORD_LINE_LENGTH);
    char *pBuffer=g_cache_read.Get(uiMaxSize);
    fread(pBuffer,uiMaxSize,1,fp);
    fclose(fp);
    g_cache_read.Free(pBuffer,uiMaxSize);

    char sTemp[RECORD_LINE_LENGTH];
    UPKlinedata pkg,retPkg;
    retPkg.set_type(sMarketID+"."+sKline);
    UPKlinedata::Data* pData = NULL;	// UPKlinedata::Data =  ukex::klinedata_Data
    for(uint32_t i=0;i<uiMaxSize;i+=RECORD_LINE_LENGTH)
    {
        if(JsonUnpack(&pkg,sTemp,RECORD_LINE_LENGTH)==false)break;
        pData = pkg.add_data();
        pData->Swap((UPKlinedata::Data*)&pkg.data(0));
    }

    Client_Write<UPKlinedata>(m_tcp,&pkg);
    return 0;
}


int ClientSession::_Opt(Json::Value &root)
{
    string opt=root["opt"].asString();
    if(opt.compare("auth")!=0)return -1;

    string cid=root["cid"].asString();
    string key=root["key"].asString();
    string sign=root["sign"].asString();
    string timestamp=root["timestamp"].asString();

    return 0;
}





