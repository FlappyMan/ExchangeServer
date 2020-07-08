
#ifndef _UB_HTTP_PARSER_HEADER_
#define _UB_HTTP_PARSER_HEADER_

#include "UBHeader.h"
#include "http_parser.h"


class UBHttpParser
{
public:
	UBHttpParser();
	~UBHttpParser();

	void Init(http_parser_type iType);	// enum http_parser_type{ HTTP_REQUEST, HTTP_RESPONSE, HTTP_BOTH };
	void Destroy();

	// 当读到数据后，就调用该函数
	// return <0:failed; =0: finished >0:need more call
	int Readed(const char *pData,uint32_t iLen);

	int GetStatusCode(){return m_parser.status_code;};	// responses only
	bool GetFiled(string &sValue,string &sFiled);
	bool GetFiled(string &sValue,const char *pFiled);

	http_parser& GetParser(){return m_parser;};		// 调用方不应该直接修改返回值中的成员


	// callback function
	int MessageBegin(){m_bHeaderFinished=false;m_bMsgFinished=false;m_sUrl.clear();m_sBody.clear();m_sCurFiled.clear();m_mFiled.clear();return 0;};
	int MessageComplete(){m_bMsgFinished=true;return 0;};

	int Url(const char* pStr, size_t iLen){m_sUrl.append(pStr,iLen);return 0;};

	int HeaderField(const char* pStr, size_t iLen){m_sCurFiled.append(pStr,iLen);return 0;};
	int HeaderValue(const char* pStr, size_t iLen)
	{
		if(m_sCurFiled.length()>0)
		{
			m_mFiled.insert(make_pair(m_sCurFiled,string(pStr,iLen)));
			m_sCurFiled.clear();
		}
		return 0;
	};
	int HeaderComplete(){m_bHeaderFinished=true;return 0;};
	bool IsWebSocket(){return m_parser.upgrade;};
	bool IsHeaderComplete(){return m_bHeaderFinished;};
	bool IsMsgComplete(){return m_bMsgFinished;};
	bool IsGet(){if(m_bHeaderFinished==false)return false;return m_parser.method==HTTP_GET;};
	bool IsPost(){if(m_bHeaderFinished==false)return false;return m_parser.method==HTTP_POST;};



	int Body(const char* pStr, size_t iLen)
	{
		m_sBody.append(pStr,iLen);
		return 0;
	};

public:
	string m_sUrl;		// Init(HTTP_REQUEST) 时有效
	string m_sBody;
	map<string,string> m_mFiled;

protected:
	http_parser m_parser;
	http_parser_settings m_settings;

	string m_sCurFiled;
	bool m_bHeaderFinished;
	bool m_bMsgFinished;

};



#endif


