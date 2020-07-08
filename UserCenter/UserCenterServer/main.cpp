/*
 * @Author: your name
 * @Date: 2020-05-07 03:04:59
 * @LastEditTime: 2020-06-11 01:50:29
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/main.cpp
 */ 
#include "UProtocol.h"
#include "./BaseConfig/config.h"
#include "global.h"
#include "ThreadTCPOper.h"
#include "ThreadHttpOper.h"
#include "ThreadDBOper.h"
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
        DEBUG_ERROR("Load config file failed");
        return false;
    }
    return true;
}


int main(int argc, char *argv[])
{
    if(Init(argc,argv)==false)
    {
        DEBUG_INFO("[UserCenter Init Failed]");
        return -1;
    }
    DEBUG_INFO("[UserCenter running]");
    uv_thread_t tTcp,tHttp,tDB;
    uv_thread_create(&tTcp, CThreadTCPOper::ThreadTCPOper, NULL);
    uv_thread_create(&tHttp, CThreadHttpOper::ThreadHttpOper, NULL);
    uv_thread_create(&tDB, CThreadDBOper::ThreadDBOper, NULL);

    uv_thread_join(&tTcp);
    uv_thread_join(&tHttp);
    uv_thread_join(&tDB);
    return 0;
}
