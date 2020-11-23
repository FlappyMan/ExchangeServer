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
void Add(const int &a,const int &b){
    std::cout<<a+b<<std::endl;
}


int main(int argc, char *argv[])
{
    if(Init(argc,argv)==false)
    {
        cout<<"Init failed"<<endl;
        return -1;
    }
#if 0
    // shared_ptr<string> films[5] ={
	// shared_ptr<string> (new string("Fowl Balls")),
	// shared_ptr<string> (new string("Duck Walks")),
	// shared_ptr<string> (new string("Chicken Runs")),
	// shared_ptr<string> (new string("Turkey Errors")),
	// shared_ptr<string> (new string("Goose Eggs"))
	// };
    // shared_ptr<string> pwin;
    // pwin = films[2]; // films[2] loses ownership. 将所有权从films[2]转让给pwin，此时films[2]不再引用该字符串从而变成空指针

	// cout << "The nominees for best avian baseballl film are\n";
	// for(int i = 0; i < 5; ++i)
	// {
	// 	cout << *films[i] << endl;
	// }
	// int N, n;
	// while (cin >> N) 
    // {
    //     int a[1001] = { 0 };
	//     while (N--) 
    //     {
	// 	    cin >> n;
	// 	    a[n] = 1;
    //     }
	//     for (int i = 0; i < 1001; i++)
	//         if (a[i])
	// 		    cout << i << endl;
	// }
    // string str1,str2;
    // getline(cin,str1);
    // getline(cin,str2);
    // vector<string> vec;
    // int len1=str1.length();
    // int len2=str2.length();
    // int m = len1/8;
    // int n = len2/8;
    // if(m>0)
    // {    
    //     for(int i=0;i<m;++i)
    //     {
    //         vec.push_back(str1.substr(i*8,8));
    //     }
    //     vec.push_back(str1.substr(m*8,8));
    // }else{
    //     vec.push_back(str1);
    // }
    // if(n>0)
    // {
    //      for(int i=0;i<n;++i)
    //      {
    //          vec.push_back(str2.substr(i*8,8));
    //      }
    //      vec.push_back(str2.substr(n*8,8)); 
    // }else{
    //     vec.push_back(str2);
    // }
    // for(int i=0;i<vec.size();++i)
    // {
    //     cout<<vec[i]<<endl;
    // }


    // int S,N;
    // while(cin>>S>>N)
    // {
    //     int sum=0,tmp=-1;
    //     for(int i=1;i<N;++i)
    //         sum+=i;
    //     int tot=S-sum;
    //     if(tot%N!=0)
    //         cout<<tmp<<endl;
    //     for(int i=0;i<N;++i)
    //     {
    //         cout<<tot/N+i<<' ';
    //     }        
    // }


    // int num;
    // while(cin>>num)
    // {
    //     begin:
    //     for(int i=2;i<num;++i)
    //         for(int j=i+1;j<num;++j){
    //         if(num%i==0 && num%j==0 && i*j==num){
    //             for(int k=2;k<i;++k){
    //                 if(i%k==0)
    //                     goto begin;
    //             }
    //             for(int k=2;k<j;++k){
    //                 if(j%k==0)
    //                     goto begin;
    //             }
    //             cout<<i<<' '<<j<<endl;
    //         } 
    //         if(i==num||j==num)
    //         {
    //            cout<<-1<<-1<<endl;    
    //         }         
    //     }

    // }

    int num;
    while(cin>>num)
    {
begin:
        for(int i=2;i<num;++i)
            for(int j=i;j<num;++j){
            if(i*j==num){
                for(int k=2;k<i;++k)
                    if(i%k==0)
                        goto begin;
                for(int k=2;k<j;++k)
                    if(j%k==0)
                        goto begin;
                cout<<i<<' '<<j<<endl;
                break;
            }
            //cout<<-1<<-1<<endl;
        }

    }

#else
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
#endif
    return 0;
}
