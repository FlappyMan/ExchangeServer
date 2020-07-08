
#include "UBaseLib.h"

enum{STR_LEN=100};
char str[STR_LEN];

void TestRead(int round,UBHttpParser &hp,int fd,uint32_t uiSize)
{
	lseek(fd,0,SEEK_SET);
	hp.Destroy();

	int ret=0;
	int iReaded=0;
	int iReadLen=0;
	while(iReaded<uiSize)
	{
		iReadLen=((uiSize-iReaded) > STR_LEN) ? STR_LEN : (uiSize-iReaded);
		read(fd,str,iReadLen);
		iReaded+=iReadLen;

		ret=hp.Readed(str,iReadLen);
		if(ret<=0)break;
	}

	if(hp.GetParser().type==HTTP_RESPONSE)
	{
		cout<<round<<":"<<"status:"<<hp.IsGet()<<hp.GetStatusCode()<<endl;
	}
	else
	{
		cout<<round<<":["<<(hp.IsPost()?"POST":"GET")<<"] url:"<<hp.m_sUrl<<endl;
	}
	return;

	cout<<"IsGet:"<<hp.IsGet()<<endl;
	for(map<string,string>::iterator it=hp.m_mFiled.begin();it!=hp.m_mFiled.end();it++)
	{
		cout<<it->first<<" :"<<it->second<<endl;
	}
	cout<<hp.m_sBody<<endl;
}


void TestFile(UBHttpParser &hp,string sFileName)
{
	getcwd(str,STR_LEN);
	string sFilePath=str;
	sFilePath.append(sFileName);
	int fd=open(sFilePath.c_str(),O_RDONLY);
	if(fd==-1)
	{
		cout<<"Open file failed:"<<sFilePath<<endl;
		return ;
	}

	struct stat _stat;
	fstat(fd,&_stat);
	for(int i=0;i<1000;i++)
	{
		TestRead(i,hp,fd,_stat.st_size);
	}
	close(fd);
}

int main()
{
	UBHttpParser hp;
//	hp.Init(HTTP_RESPONSE);
//	TestFile(hp,"/json-en.html");
//	hp.Destroy();

	hp.Init(HTTP_REQUEST);		// http_parser 不能切换方式，所以需要注释掉上面的
	TestFile(hp,"/http_test.html");
	hp.Destroy();

	return 0;
}


