

#include "UBHttpParser.h"


int UBHttpParser_cbMessageBegin(http_parser* p);
int UBHttpParser_cbHeaderComplete(http_parser* p);
int UBHttpParser_cbMessageComplete(http_parser* p);
int UBHttpParser_cbUrl(http_parser* p, const char* at, size_t length);
int UBHttpParser_cbHeaderField(http_parser* p, const char* at, size_t length);
int UBHttpParser_cbHeaderValue(http_parser* p, const char* at, size_t length);
int UBHttpParser_cbBody(http_parser* p, const char* at, size_t length);


UBHttpParser::UBHttpParser():m_bHeaderFinished(false),m_bMsgFinished(false)
{
	memset(&m_parser, 0, sizeof(http_parser));
	memset(&m_settings, 0, sizeof(http_parser_settings));
};

UBHttpParser::~UBHttpParser()
{
}


void UBHttpParser::Destroy()
{
	http_parser_type type=(http_parser_type)m_parser.type;

	memset(&m_parser, 0, sizeof(http_parser));
	memset(&m_settings, 0, sizeof(http_parser_settings));

	Init(type);
}


// enum http_parser_type { HTTP_REQUEST, HTTP_RESPONSE, HTTP_BOTH };
void UBHttpParser::Init(http_parser_type iType)
{
	if (m_parser.data != NULL)return; // 只调用一次初始化

	memset(&m_settings, 0, sizeof(http_parser_settings));
	m_settings.on_message_begin = UBHttpParser_cbMessageBegin;
	m_settings.on_url = UBHttpParser_cbUrl;
	m_settings.on_header_field = UBHttpParser_cbHeaderField;
	m_settings.on_header_value = UBHttpParser_cbHeaderValue;
	m_settings.on_headers_complete = UBHttpParser_cbHeaderComplete;
	m_settings.on_body = UBHttpParser_cbBody;
	m_settings.on_message_complete = UBHttpParser_cbMessageComplete;

	m_parser.data = this;
	http_parser_init(&m_parser, iType);

	m_sUrl.clear();
	m_sBody.clear();
	m_sCurFiled.clear();
	m_mFiled.clear();
	m_bHeaderFinished=false;
	m_bMsgFinished = false;
};

// 当读到数据后，就调用该函数
// return <0:failed; =0: finished >0:need more call
int UBHttpParser::Readed(const char *pData, uint32_t iLen)
{
	assert(m_parser.data);
	assert(m_settings.on_message_begin);
	size_t nparsed = http_parser_execute(&m_parser, &m_settings, pData, iLen);
	if (m_parser.upgrade) // websocket
	{
		return 0;
	}
	else if (nparsed != iLen)
	{
		return -1;
	}
	if(m_bMsgFinished)
	{
		http_parser_execute(&m_parser, &m_settings, pData, 0);
		return 0;
	}
	else 
	{
		return nparsed;
	}
};

bool UBHttpParser::GetFiled(string &sValue,string &sFiled)
{
	map<string, string>::iterator it = m_mFiled.find(sFiled);
	if (it == m_mFiled.end())
		return false;
	sValue = it->second;
	return true;
};

bool UBHttpParser::GetFiled(string &sValue,const char *pFiled)
{
	map<string, string>::iterator it = m_mFiled.find(pFiled);
	if (it == m_mFiled.end())
		return false;
	sValue = it->second;
	return true;
};



int UBHttpParser_cbMessageBegin(http_parser* p)
{
	return ((UBHttpParser*)p->data)->MessageBegin();
}

int UBHttpParser_cbHeaderComplete(http_parser* p)
{
	return ((UBHttpParser*)p->data)->HeaderComplete();
}

int UBHttpParser_cbMessageComplete(http_parser* p) 
{
	return ((UBHttpParser*)p->data)->MessageComplete();
}

int UBHttpParser_cbUrl(http_parser* p, const char* at, size_t length) 
{
	return ((UBHttpParser*)p->data)->Url(at,length);
}

int UBHttpParser_cbHeaderField(http_parser* p, const char* at, size_t length) 
{
	return ((UBHttpParser*)p->data)->HeaderField(at,length);
}

int UBHttpParser_cbHeaderValue(http_parser* p, const char* at, size_t length) 
{
	return ((UBHttpParser*)p->data)->HeaderValue(at,length);
}

int UBHttpParser_cbBody(http_parser* p, const char* at, size_t length)
{
	return ((UBHttpParser*)p->data)->Body(at,length);
}

