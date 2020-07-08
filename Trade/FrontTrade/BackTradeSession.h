#ifndef _BACKTRADE_SESSION_HEADER_
#define _BACKTRADE_SESSION_HEADER_

#include "global.h"

class BackTradeSrv;
class BackTradeSyncFile
{
    public:
        string m_sRelativePath;
        string m_sFileName;
};

class BackTradeSession
{
    public:
        BackTradeSession(uv_tcp_t *tcp,UBBlockQueue<UProtocolBase> &qRes);
        ~BackTradeSession();

        void Init();
        int Read(char *pBuffer,int iDataLen, UBBlockQueue<UProtocolBase> &m_qResponse);
        bool IsTimeout(time_t tNow);
        void GetRsaPublicKey();

    protected:
        bool LoginCheck(UPLogin *pLogin);
    public:
        uv_tcp_t* m_tcp;
        UBBuffer m_buffer;
	    BackTradeSrv *m_pSrv;  
    public:
        bool m_bLogined;
        bool m_bActive;
        time_t m_tLast;
        string m_sPublicKey;
	    uint64_t m_uiMarketID;
	    string m_sMarketID;
};

#endif
