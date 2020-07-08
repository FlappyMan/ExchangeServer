#include "UProtocol.h"

bool conDB()
{
	UBMysql mysql(2048);
	string userAccount = "Fiona2059@qq.com";
	string st = "1";
	string userId = "";
	string sTimeTemp = "1586245589";
	string password = "123456";

	string sUnixSock = "192.168.1.157", sUser = "zhang", sPwd = "123456", sDB = "test";
	if (mysql.Connect(sUnixSock, 3306, sUser, sPwd, sDB) == false)
	{
		int iLastError = mysql.Error();
		cout << "faield:" << iLastError << endl;

		string sLastError;
		mysql.Error(sLastError);
		cout << "faield:" << sLastError << endl;
	}
    else
        cout << "connect to mysql:" << sUnixSock << " success!" <<endl;

	string sError;
	string sql = "DROP TABLE A;";
	int iRet = mysql.Exec(sql);
	cout << sql << endl;
	if (iRet < 0)
	{
		mysql.Error(sError);
		cout << "exec sql failed:" << mysql.Error() << ":" << sError << endl;
	}
#if 1
	//Innodb
    sql = "create table A(id int not null default 0) ENGINE=innodb;";
#else
	//MyISAM
    sql = "create table A(id int not null default 0) ENGINE=myisam;";
#endif
	iRet = mysql.Exec(sql);
	if (iRet < 0)
	{
		mysql.Error(sError);
		cout << "exec sql failed:" << mysql.Error() << ":" << sError << endl;
	}
}

void ThreadWrite(void *arg)
{
	UBMysql mysql(2048);
	string userAccount = "Fiona2059@qq.com";
	string st = "1";
	string userId = "";
	string sTimeTemp = "1586245589";
	string password = "123456";

	string sUnixSock = "192.168.1.157", sUser = "zhang", sPwd = "123456", sDB = "test";
	if (mysql.Connect(sUnixSock, 3306, sUser, sPwd, sDB) == false)
	{
		int iLastError = mysql.Error();
		cout << "faield:" << iLastError << endl;

		string sLastError;
		mysql.Error(sLastError);
		cout << "faield:" << sLastError << endl;
	}
    else
        cout << "connect to mysql:" << sUnixSock << " success!" <<endl;

    //表Ａ中插入10000条数据
	int iSqlLen = 0;
	char sSql[256];
	for (int i = 0; i < 10000; i++)
	{
		iSqlLen = snprintf(sSql, 256, "insert into A(id) values(%u)", i);
		//cout << sSql << endl;
		mysql.Exec(sSql, iSqlLen);
	}
	//修改数据
	// for (size_t i = 0; i < 10000; i++)
	// {
	// 	string sql = "update A set key='666' where id>=1 and id<=10000;";
	// 	//cout << sql << endl;
	// 	bool bHasValue = mysql.Select(sql);
	// 	if (bHasValue == false)
	// 	{
	// 		cout << sql << " has no record" << endl;
	// 	}
	// }
}

void ThreadRead(void *arg)
{
	UBMysql mysql(2048);
	string userAccount = "Fiona2059@qq.com";
	string st = "1";
	string userId = "";
	string sTimeTemp = "1586245589";
	string password = "123456";

	string sUnixSock = "192.168.1.157", sUser = "zhang", sPwd = "123456", sDB = "test";
	if (mysql.Connect(sUnixSock, 3306, sUser, sPwd, sDB) == false)
	{
		int iLastError = mysql.Error();
		cout << "faield:" << iLastError << endl;

		string sLastError;
		mysql.Error(sLastError);
		cout << "faield:" << sLastError << endl;
	}
    else
        cout << "connect to mysql:" << sUnixSock << " success!" <<endl;

	for (size_t i = 0; i < 10000; i++)
	{
		string sql = "select id from A;";
		bool bHasValue = mysql.Select(sql);
		if (bHasValue == false)
		{
			cout << sql << " has no record" << endl;
		}
	}
}

int main(int argc, char* argv[])
{
    time_t tStart = time(NULL);
	uv_thread_t tWrite,tRead;
	conDB();
	uv_thread_create(&tWrite,ThreadWrite,NULL);
	uv_thread_create(&tRead,ThreadRead,NULL);

	uv_thread_join(&tWrite);
	uv_thread_join(&tRead);
	time_t tEnd = time(NULL);
	cout<<"elapsed time:"<<tEnd-tStart<<endl;
    return 0;
}