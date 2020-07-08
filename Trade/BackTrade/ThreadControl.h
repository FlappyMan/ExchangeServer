/*
 * @Author: your name
 * @Date: 2020-05-11 00:05:49
 * @LastEditTime: 2020-06-23 00:55:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/ThreadControl.h
 */
#ifndef _THREAD_CONTROL_HEADER_
#define _THREAD_CONTROL_HEADER_

#include "global.h"
#include "./Manager/DispatchManager.h"
#include "./Dto/UPJsonReflex.hpp"
#include "./Model/FrontMarket.hpp"

extern uv_loop_t g_uvControlLoop;
extern uv_timer_t g_uvControlTimer;

class CThreadControl
{
public:
    CThreadControl();
    ~CThreadControl();
    static void ThreadContrl(void *arg);
    static void cbReadBuff(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
    static void cbRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
    static void cbClosed(uv_handle_t *handle);
    static void cbTimer(uv_timer_t *handle);
    static void cbNewConnection(uv_stream_t *server, int status);
    static void cbWrited(uv_write_t *req, int status);
    static void cbLogin(uv_write_t *req, int status);
    static void cbConnect(uv_connect_t *req, int status);
    static void reConnect(void* arg);
    static void rsaLoginServer(client_pkg_t *client);

public:
    /*
    * 下单/撤单，深度指标部分
    */
    static void cancelTradeAndPush(CBaseSession *session);
    static void batSavePriceAndPush(CBaseSession *session, client_pkg_t *client, bool bIsAdd);
    static void savePriceAndPush(CBaseModel *session,client_pkg_t *client,bool bIsAdd);
    static void savePrice(int type, string price, string num,bool bIsAdd);
    static void businessPushOper(vector<CBaseModel *> &vecPushFrontMarket);
    static UPDepthdata *packDepth();

    /*
    * 撮合成功-市场信息部分
    * */
    static UPAllmarketinfo *packMarketInfo();
    static void saveMarketInfo(CMatchedDataModel model);
    static void praseMarketInfo(vector<CMatchedDataModel> vecMatchModel);

    /*
    * 撮合成功-K线信息
    * */
    static void praseKlineInfo(vector<CMatchedDataModel> vecMatchModel);
    static void saveKlineData(CKlineInfoModel klineModel);
    static void RecordEveryTimeKline(CKlineInfoModel klineModel,map<int, CKlineInfoModel> &mapKline,int iRecordTime,int iRecordType);
    static UPKlinedata *packKline();
    static void getKlineFromMin(UPKlinedata *data, int count5min);

    /*
    * 交易记录
    * */
   static void praseTradeLog(vector<CMatchedDataModel> vecMatchModel);
   /*
   * 撮合成功
   * */
   static void praseMatchSucc(vector<CMatchedDataModel> vecMatchModel);

   // 记录数据到文件
   static void recordToFile(string value,const int type);
public:
    /*
    * frontmarket 连接池
    */
    static map<client_pkg_t *,CServerInfo> g_mapConnectInfo;
    static map<client_pkg_t *,uv_thread_t*> g_mapThread;
    static UBBlockQueue<CBaseSession> g_qRecvReqQueue;  // 收到frontmarket的请求

public:
    /*
    * 下单/撤单，深度指标部分
    */
    static UBBlockQueue<CBaseModel> m_qContrlQueue;
    static UBBlockObjQueue<CMatchedDataModel> m_qControlMatchQueue;
    static CDepthModel g_depth;
    static vector<CBaseModel*> g_vecSessionRealse;

// public:
//     static UBBlockObjQueue<CKlineInfoModel> m_qKLineQueue;
    
public:
    /*
    * 市场信息部分
    */
    static list<CMarketInfoModel> g_listMarketInfo;

public:
    /*
    * 撮合成功-K线信息
    * */
    static map<int,CKlineInfoModel> g_mapKline; // 1分钟
    static map<int,CKlineInfoModel> g_mapKline5min; // 5分钟
    static map<int,CKlineInfoModel> g_mapKline15min;
    static map<int,CKlineInfoModel> g_mapKline30min;
    static map<int,CKlineInfoModel> g_mapKline1h;
    static map<int,CKlineInfoModel> g_mapKline6h;
    static map<int,CKlineInfoModel> g_mapKline24h;
};

#endif
