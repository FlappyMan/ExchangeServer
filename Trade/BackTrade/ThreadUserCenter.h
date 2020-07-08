/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-07-01 19:50:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/ThreadUserCenter.h
 */
#ifndef _THREAD_USERCENTER_HEADER_
#define _THREAD_USERCENTER_HEADER_

#include "global.h"
#include "Manager/DispatchManager.h"
extern uv_loop_t g_uvUserCenterLoop;
extern uv_timer_t g_uvUserCenterTimer;

class CThreadUserCenter
{
public:
    CThreadUserCenter();
    ~CThreadUserCenter();
    static void ThreadUserCenter(void *arg);
    static void cbReadBuff(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
    static void cbClosed(uv_handle_t *handle);
    static void cbTimer(uv_timer_t *handle);
    static void cbNewConnection(uv_stream_t *server, int status);
    static void cbRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
    static void cbWrited(uv_write_t *req, int status);
    static void cbConnect(uv_connect_t *req, int status);
    static void reConnect();
    static void cbLogin(uv_write_t *req, int status);
    static void rsaLoginServer(client_pkg_t *client);

private:
    static void getOriginSession(vector<CBaseSession *>& vecUCAuthResp,vector<CBaseModel*>& vecRet);
    static vector<CBaseSession *> TransUptradeToUC();
    static void PraseCancelAndSelect(vector<CBaseSession *>& vecCancel);
    static vector<CBaseSession *> getAuthPass(vector<CBaseSession *>& vecUCResp);
    static vector<CBaseSession *> getUCReturnPkg();
    static void respToClient(vector<CBaseSession *>& vec,bool bFreeSession = true);
    static void operMatchResp(vector<CBaseModel *>& vecMTResp,vector<CBaseSession *>& vecRet);
    static void MatchDataToUserCenter(vector<UVWriteReq *>& vec);
    static void setUpTradeModel(CTradeModel* pBaseModel,CTradeModel model,CBaseSession *session);
    static void setCancelTradeModel(CCancelTradeModel *pBaseModel, CCancelTradeModel model);
public:
    static void loadRuntimeConfig();
public:
    static UBBlockQueue<CBaseSession> g_qOrderToUCRespQueue;
    static UBBlockQueue<CBaseSession> g_qRecvUCRespQueue;
    static map<string,CBaseSession*> g_mapAuthSession;
    static UBBlockQueue<CBaseSession> g_qOrderToUCTrans;
public:
    static uv_stream_t *fdSocket;
    // tcp连接全局变量
    static client_pkg_t *g_server;
    static uv_connect_t *g_connect;
    static string g_sConfigFileUpdateTime;
};

#endif
