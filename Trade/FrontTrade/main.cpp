#include "UProtocol.h"
#include "ThreadClient.h"
#include "ThreadBackTrade.h"
#include "config.h"
#include "global.h"
#include <malloc.h>

bool Init(int argc, char *argv[])
{
    g_cache_write_req_Client.Init(100,100,100);
    g_cache_write_req_BackTrade.Init(100,100,100);
    if(argc<2)
    {
        //cout<<argv[0]<< "config.json"<<endl;
        return false;
    }
    g_cache_read.Init(10,10,10);
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
    if(g_config.Load(sConfigPath)==false)
    {
        cout<<"Load config file failed"<<endl;
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
#if 0 
    if(-1 == daemon(1,0))
    {
        return -1;
    }
#endif
    if(Init(argc,argv)==false)
    {
        cout<<"Init failed"<<endl;
        return -1;
    }
    uv_thread_t tClient,tBackTrade;
    uv_thread_create(&tBackTrade, ThreadBacktrade, NULL);
    uv_thread_create(&tClient, ThreadClient, NULL);
    malloc_trim(0);

    uv_thread_join(&tClient);
    uv_thread_join(&tBackTrade);
    return 0;
}
