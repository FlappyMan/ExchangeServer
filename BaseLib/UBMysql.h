
#ifndef _UBMYSQL_HEADER_
#define _UBMYSQL_HEADER_

#include "UBHeader.h"

class UBMysql
{
public:
	UBMysql();
	UBMysql(uint32_t uiSqlSize);
	UBMysql(char *pSqlBuff,uint32_t uiSqlSize);
	virtual ~UBMysql(); 

	int Error(){return m_iError;};
	void Error(string &str){str=m_sError;};
	void init(uint32_t uiSqlSize);

	void Disconnect();
	bool Connect(string &host,int port,string &user,string &pwd,string &db);
	bool Connect(string &unixsocket,string &user,string &pwd,string &db);
	

	// 在内部sql缓冲中构造sql
	bool Sql(string &sSql,bool bNeedEscape,bool bCleanBuffer=false){return Sql(sSql.data(),sSql.length(),bNeedEscape,bCleanBuffer);};
	bool Sql(const char *pSql,bool bNeedEscape, bool bCleanBuffer=false){return Sql(pSql,strlen(pSql),bNeedEscape,bCleanBuffer);};
	bool Sql(const char *pSql,int iSqlLen,bool bNeedEscape = false,bool bCleanBuffer=false);

	// Insert & Update 操作，返回受影响记录数
	int Exec(){return Exec(m_pSql,m_iSqlLength);};			//执行内部buffer中的sql
	int Exec(string &sql){return Exec(sql.data(),sql.length());}	//执行指定sql
	int Exec(const char *sql,int len);								//执行指定sql

	// Select 操作
	void SelectClear();
	bool Select(){return Select(m_pSql,m_iSqlLength);};
	bool Select(string &sql){return Select(sql.data(),sql.length());};
	bool Select(const char *sql,int len);

	// select 取结果相关函数
	bool NextRow();
	void Value(uint32_t iIdx,int16_t &iValue,int16_t iDefault=0);
	void Value(uint32_t iIdx,uint16_t &iValue,uint16_t iDefault=0);
	void Value(uint32_t iIdx,int32_t &iValue,int32_t iDefault=0);
	void Value(uint32_t iIdx,uint32_t &uiValue,uint32_t uiDefault=0);
	void Value(uint32_t iIdx,int64_t &iValue,int64_t iDefault=0);
	void Value(uint32_t iIdx,uint64_t &uiValue,uint64_t uiDefault=0);
	void Value(uint32_t iIdx,double &dValue,double dDefault=0);
	void Value(uint32_t iIdx,float &fValue,float fDefault=0.0);
	void Value(uint32_t iIdx,string &sValue);
	uint32_t Value(uint32_t iIdx,uint8_t *pBuff,uint32_t iBuffSize);		// 返回实际存入的字节数

	uint64_t LastID(){return m_pMysql==NULL?0:mysql_insert_id(m_pMysql);};
	char* getSql(){if(m_iSqlLength<m_iSqlSize)m_pSql[m_iSqlLength]=0;return m_pSql;};
protected:
	
protected:
	MYSQL *m_pMysql;
	MYSQL_RES *m_pMysqlResult;
	string m_sError;
	int m_iError;

	char *m_pSql;
	const bool m_bFreeBuffer;
	uint32_t m_iSqlSize;
	uint32_t m_iSqlLength;

private:
	MYSQL_ROW m_row;
	uint32_t m_uiColumnCnt;
};



#endif


