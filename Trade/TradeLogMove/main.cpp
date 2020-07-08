#include "TaskDispatch.h"
#include "config.h"
#include "thread_pool.h"

bool Init(int argc, char *argv[])
{
    if(argc<2)
    {
        //cout<<argv[0]<< "config.json"<<endl;
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
    if(g_config.Load(sConfigPath)==false)
    {
        cout<<"Load config file failed"<<endl;
        return false;
    }
    
    GetAllSrcTableName();

    return true;
}


int main(int argc, char *argv[])
{
    if(Init(argc,argv)==false)
    {
        cout<<"Init failed"<<endl;
        return -1;
    }
    int64_t tBegin,tEnd;
    tBegin = GetCurTime();
    tp::TaskExecutionMgr<tp::TaskQueue<TableData>> *tskExecMgr = new tp::TaskExecutionMgr<tp::TaskQueue<TableData>>;
    uv_thread_t tTradeLog;

    uv_thread_create(&tTradeLog, ThreadTradeLog, (void*)tskExecMgr);

    uv_thread_join(&tTradeLog);

    tskExecMgr->ThreadJoiner();
    tEnd = GetCurTime();
    cout<<"Elapsed time : "<<(tEnd-tBegin)<<endl;
    delete tskExecMgr;
    return 0;
}
