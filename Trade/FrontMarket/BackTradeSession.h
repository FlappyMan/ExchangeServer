

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
	BackTradeSession(uv_tcp_t *tcp,BackTradeSrv *pSrv);
	~BackTradeSession();

	void Destroy();
	void Init();
	bool IsTimeout(time_t tNow);

	int Read(char *pBuffer,int iDataLen);
protected:
	bool LoginCheck(UPLogin *pLogin);
	void BuildLoginSyncReq();
	void _Sync1Minute(time_t tCur);
	void _SyncXMinute(time_t tCur);
	void _SyncFileInfo(UPFileRequest *pReq);

	void SendNextSyncReq();
	bool RecvFileRequest(UPFileRequest *pkg);
	bool RecvFileData(UPFileData *pkg);

	void GetRsaPublicKey();
protected:
	const uv_tcp_t* m_tcp;
	BackTradeSrv *m_pSrv;

	UBBuffer m_buffer;
	
	queue<UPFileRequest *> m_qSyncFile;
public:
	bool m_bLogined;
	bool m_bSyncFinished;           // 是否已经完成初始化工作（本地缓存k线数据）
	string m_sMarketID;
	uint64_t m_uiMarketID;
    string m_sPublicKey;
protected:
	time_t m_tLast;					// 最近一次操作时间 
};

#endif
