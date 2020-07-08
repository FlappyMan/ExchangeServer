/*
 * @Author: your name
 * @Date: 2020-05-07 23:04:37
 * @LastEditTime: 2020-06-24 02:06:22
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/main.cpp
 */
#include "UProtocol.h"
#include "./BaseConfig/config.h"
#include "global.h"
#include "ThreadDB.h"
#include "ThreadOrder.h"
#include "ThreadUserCenter.h"
#include "ThreadMatch.h"
#include "ThreadControl.h"
bool Init(int argc, char *argv[])
{
    if(argc<2)
    {
        return false;
    }

    enum{BUFF_SIZE=512};
    char sBuffer[BUFF_SIZE]={0};
    int count = readlink( "/proc/self/exe", sBuffer, BUFF_SIZE );
    if ( count < 0 || count >= BUFF_SIZE )
    {
        cout<<"get program full path failed"<<endl;
        return false;
    }
    g_dir_root.assign(sBuffer,count);
    string::size_type pos=g_dir_root.find_last_of('/');
    if(pos==g_dir_root.npos)return false;
    g_dir_root.erase(pos+1);

    string sConfigPath=g_dir_root+argv[1];
    cout<<"sConfigPath = "<<sConfigPath<<endl;
    if(Config::Load(sConfigPath)==false)
    {
        cout<<"Load config file failed"<<endl;
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    if(Init(argc,argv)==false)
    {
        cout<<"[BackTrade Init Failed]"<<endl;
        return -1;
    }
    cout<<"[BackTrade running]"<<endl;
    uv_thread_t tOrderThread,tUCThread,tDBThread,tMatchThread,tControllerThread;
    uv_thread_create(&tOrderThread, CThreadOrder::ThreadOrder, NULL);
    uv_thread_create(&tUCThread, CThreadUserCenter::ThreadUserCenter, NULL);
    uv_thread_create(&tDBThread, CThreadDB::ThreadDB, NULL);
    uv_thread_create(&tMatchThread, CThreadMatch::ThreadMatch, NULL);
    uv_thread_create(&tControllerThread, CThreadControl::ThreadContrl, NULL);

    uv_thread_join(&tOrderThread);
    uv_thread_join(&tUCThread);
    uv_thread_join(&tDBThread);
    uv_thread_join(&tMatchThread);
    uv_thread_join(&tControllerThread);
    return 0;
}
