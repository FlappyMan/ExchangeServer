/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-29 01:11:44
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/global.cpp
 */

#include "global.h"

string g_dir_root;
Config g_config;
ReadCache g_cache_orderthread_read; // 用于 Threadrder 线程读取tcp数据
WriteReqCache g_cache_orderthread_resp; // backtrade-frontrade业务回复包队列
WriteReqCache g_cache_orderthread_login;    // backtrade-fronttrade登录包队列

ReadCache g_cache_ctrlthread_read; // ThreadDB线程发送登陆服务器队列
WriteReqCache g_cache_ctrlthread_push; // ThreadDB线程推送数据到其他服务器队列
WriteReqCache g_cache_ctrlthread_login; // frotmarket登陆服务器队列



ReadCache g_cache_ucthread_read; // 用于 ThreadUserCenter 线程读取tcp数据
WriteReqCache g_cache_ucthread_push;  // backtrade-推送数据到用户中心服务器队列
WriteReqCache g_cache_ucthread_login;  // backtrade-usercenter发送的数据包队列

