/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-05-13 00:34:10
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/ThreadOrder.h
 */
#ifndef _THREAD_ORDER_HEADER_
#define _THREAD_ORDER_HEADER_

#include "global.h"

extern uv_loop_t g_uvOrderLoop;
extern uv_timer_t g_uvOrderTimer;

class CThreadOrder
{
public:
    CThreadOrder();
    ~CThreadOrder();
    static void ThreadOrder(void *arg);
    static void cbReadBuff(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
    static void cbClosed(uv_handle_t *handle);
    static void cbTimer(uv_timer_t *handle);
    static void cbNewConnection(uv_stream_t *server, int status);
    static void cbRead(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf);
    static void cbWrited(uv_write_t *req, int status);
    static void cbLogin(uv_write_t *req, int status);
    static void cbConnect(uv_connect_t *req, int status);
    static void reConnect(void *arg);
    static void rsaLoginServer(client_pkg_t *client);

public:
    // tcp连接全局变量
    static uv_mutex_t g_mutex;  // socket编号索引
    static map<client_pkg_t *,CServerInfo> g_mapConnectInfo;    // fronttrade的服务器连接信息
    static map<client_pkg_t *,uv_thread_t*> g_mapThread;    // 记录fronttrade的socket连接状态map

public:
   static UBBlockQueue<CBaseSession> g_NodeSession;
};

#endif
