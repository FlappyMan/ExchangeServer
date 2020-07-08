/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-24 01:51:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/ThreadDB.h
 */ 
#ifndef _THREAD_DB_HEADER_
#define _THREAD_DB_HEADER_

#include "global.h"
#include "DispatchManager.h"
#include "./Dto/UPJsonReflex.hpp"
#include "Model/FrontMarket.hpp"
extern uv_loop_t g_uvDBLoop;
extern uv_timer_t g_uvDBTimer;

class CThreadDB
{
public:
    CThreadDB();
    ~CThreadDB();
    static void ThreadDB(void *arg);
    static void cbTimer(uv_timer_t *handle);
public:
    static UBBlockQueue<CBaseModel> m_qDBQueue;

};

#endif
