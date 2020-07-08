#include "BackTradeSession.h"
#include "ThreadClient.h"

BackTradeSession::BackTradeSession(uv_tcp_t *tcp,UBBlockQueue<UProtocolBase> &qReq):m_tcp(tcp),m_bLogined(false),m_bActive(false)
{
    tcp->data = this;
    m_buffer.Init(SIZE_BUFFER_8k);
}

BackTradeSession::~BackTradeSession()
{

}

void BackTradeSession::GetRsaPublicKey()
{  
    enum{BUFF_SIZE=512};
    char buf[6200] = {0};
    char szPath[BUFF_SIZE] = {0};
    char szFullPath[BUFF_SIZE] = {0};

    char sBuffer[BUFF_SIZE]={0};
    int count = readlink( "/proc/self/exe", sBuffer, BUFF_SIZE );
    if ( count < 0 || count >= BUFF_SIZE )
    {
        cout<<"get program full path failed"<<endl;
        return;
    }
    g_dir_root.assign(sBuffer,count);
    string::size_type pos=g_dir_root.find_last_of('/');
    if(pos==g_dir_root.npos)return;
    g_dir_root.erase(pos+1);

    sprintf(szFullPath, "%s%s", g_dir_root.data(), "/fronttrade_public_key_blowfish.pem");
    int fd = open(szFullPath, O_RDWR);
    if (-1 == fd)
    {
        cout << "open rsa file failure" << endl;
    }
    else
    {
        cout << "open rsa file successful" << endl;
    }
    int ret = read(fd, buf, 6200);
    if (ret < -1)
    {
        cout << "read rsa file failed" << endl;
    }
    else
    {
        unsigned char *recBuf = new unsigned char[ret];
        BlowfishDecrypt(recBuf, ret, (const unsigned char *)buf, ret);
        m_sPublicKey = (const char *)recBuf;
    }
    close(fd);
}

void BackTradeSession::Init()
{
    m_tLast = time(NULL);
    m_bActive = true;
    GetRsaPublicKey();
}

bool BackTradeSession::IsTimeout(time_t tNow)
{
    return tNow > m_tLast+30;
}

// return <0: 协议错误，=0:数据包长度不足，>0:已处理掉的数据长度
int BackTradeSession::Read(char *pBuffer,int iDataLen,UBBlockQueue<UProtocolBase> &m_qResponse)
{
    int iTotalDataLen=m_buffer.AppendData(pBuffer,iDataLen);

    uint16_t uiType=0;
    uint32_t uiPackLen=0;

    int ret=0;
    UProtocolBase *pkg=NULL;
    if(m_bLogined==false)
    {
        ret=ProtocolHead(uiType,uiPackLen,m_buffer.Data(),m_buffer.DataLength());
        if(ret<=0)return ret;
        if(uiType!=UPLogin::CMD)return -1;

        UPLogin pkgLogin;
        ProtoUnpack(pkgLogin,m_buffer.Data(),uiPackLen);
        m_buffer.RemoveData(uiPackLen);

        // 校验身份
        if(LoginCheck(&pkgLogin)==false)return -1;
        m_bLogined=true;
    }

    while(true)
    {
        uiType=0;
        uiPackLen=0;
        ret=ProtocolHead(uiType,uiPackLen,m_buffer.Data(),m_buffer.DataLength());
        if(ret<0)return ret;
        else if(ret==0)break;
        switch (uiType)
        {
            case UPResponse::CMD :
                pkg = new UPResponse;
                ProtoUnpack(*(UPResponse*)pkg,m_buffer.Data(),uiPackLen);
                m_qResponse.put(pkg);
                break;
            default:
                break;
        }
        m_buffer.RemoveData(uiPackLen);
    }
    return iDataLen;
}

bool BackTradeSession::LoginCheck(UPLogin *pLogin)
{
    Json::Value root;
    if(pLogin->ServerCheck(root,m_sPublicKey)<0)return false;
    cout<<"BackTrade Login Success"<<endl;
    m_uiMarketID = root["mid"].asInt64();
    m_sMarketID = root["mn"].asString();

	// save market
	UPMarketAdd *pMarketAdd = new UPMarketAdd();
    pMarketAdd->m_uiMarketID = root["mid"].asUInt64();
	pMarketAdd->m_sMarketID = root["mn"].asString();

	if (pMarketAdd->m_sMarketID.length() <= 0 || pMarketAdd->m_uiMarketID == 0)
	{
		delete pMarketAdd;
		return false;
	}
    //m_pSrv->AddMarket(m_uiMarketID,this);
    return true;
}
