
#include "UPCache.h"

WriteReqCache::WriteReqCache() : m_ui2kCnt(0), m_ui8kCnt(0), m_ui16kCnt(0)
{
}

WriteReqCache::~WriteReqCache()
{
	Destroy();
}

void WriteReqCache::Destroy()
{
	while (m_q2k.size() > 0)
	{
		delete m_q2k.front();
		m_q2k.pop();
	}
	while (m_q8k.size() > 0)
	{
		delete m_q8k.front();
		m_q8k.pop();
	}
	while (m_q16k.size() > 0)
	{
		delete m_q16k.front();
		m_q16k.pop();
	}

	m_ui2kCnt = 0;
	m_ui8kCnt = 0;
	m_ui16kCnt = 0;
}

void WriteReqCache::Init(int i2kCnt, int i8kCnt, int i16kCnt)
{
	Destroy();
	
	m_ui2kCnt = i2kCnt;
	m_ui8kCnt = i8kCnt;
	m_ui16kCnt = i16kCnt;

	UVWriteReq *p = NULL;
	for (uint32_t i = 0; i < m_ui2kCnt; i++)
	{
		p = new UVWriteReq;
		p->buf.base = new char[SIZE_BUFFER_2k];
		p->buf.len = 0;
		p->len = SIZE_BUFFER_2k;
		m_q2k.push(p);
	}

	for (uint32_t i = 0; i < m_ui8kCnt; i++)
	{
		p = new UVWriteReq;
		p->buf.base = new char[SIZE_BUFFER_8k];
		p->buf.len = 0;
		p->len = SIZE_BUFFER_8k;
		m_q8k.push(p);
	}

	for (uint32_t i = 0; i < m_ui16kCnt; i++)
	{
		p = new UVWriteReq;
		p->buf.base = new char[SIZE_BUFFER_16k];
		p->buf.len = 0;
		p->len = SIZE_BUFFER_16k;
		m_q16k.push(p);
	}
}

UVWriteReq *WriteReqCache::Get(int iBuffSize)
{
	UVWriteReq *p = NULL;
	if (iBuffSize <= SIZE_BUFFER_2k)
	{
		if (m_q2k.size() <= 0)
		{
			p = new UVWriteReq;
			p->buf.base = new char[SIZE_BUFFER_2k];
			p->buf.len = 0;
            p->len=SIZE_BUFFER_2k;
			m_q2k.push(p);
		}
		else
		{
			p = m_q2k.front();
			m_q2k.pop();
		}
	}
	else if (iBuffSize <= SIZE_BUFFER_8k)
	{
		if (m_q8k.size() <= 0)
		{
			p = new UVWriteReq;
			p->buf.base = new char[SIZE_BUFFER_8k];
			p->buf.len = 0;
            p->len=SIZE_BUFFER_8k;
			m_q8k.push(p);
		}
		else
		{
			p = m_q8k.front();
			m_q8k.pop();
		}
	}
	else if (iBuffSize <= SIZE_BUFFER_16k)
	{
		if (m_q16k.size() <= 0)
		{
			p = new UVWriteReq;
			p->buf.base = new char[SIZE_BUFFER_16k];
			p->buf.len = 0;
            p->len=SIZE_BUFFER_16k;
			m_q16k.push(p);
		}
		else
		{
			p = m_q16k.front();
			m_q16k.pop();
		}
	}
	else
	{
		p = new UVWriteReq;
		p->buf.base = new char[iBuffSize];
		p->buf.len = iBuffSize;
        p->len=iBuffSize;
	}

	return p;
}

void WriteReqCache::Free(UVWriteReq *p)
{
	if (p->pkg)
	{
		delete p->pkg;
		p->pkg = NULL;
	}
	if (p->len == SIZE_BUFFER_2k)
	{
		if (m_q2k.size() > m_ui2kCnt)
		{
			if (p->buf.base)
			{
				delete[] p->buf.base;
			}
			delete p;
			p = NULL;
		}
		else
		{
            p->buf.len=0;
			m_q2k.push(p);
		}
	}
	else if (p->len == SIZE_BUFFER_8k)
	{
		if (m_q8k.size() > m_ui8kCnt)
		{
			if (p->buf.base)
			{
				delete[] p->buf.base;
			}
			delete p;
			p = NULL;
		}
		else
		{
            p->buf.len=0;
			m_q8k.push(p);
		}
	}
	else if (p->len == SIZE_BUFFER_16k)
	{
		if (m_q16k.size() > m_ui16kCnt)
		{
			if (p->buf.base)
			{
				delete[] p->buf.base;
			}
			delete p;
			p = NULL;
		}
		else
		{
            p->buf.len=0;
			m_q16k.push(p);
		}
	}
	else
	{
		if (p->buf.base)
		{
			delete[] p->buf.base;
		}
		delete p;
		p = NULL;
	}
}

ReadCache::ReadCache() : m_ui2kCnt(0), m_ui8kCnt(0), m_ui16kCnt(0)
{
}

ReadCache::~ReadCache()
{
	Destroy();
}

void ReadCache::Destroy()
{
	while (m_q2k.size() > 0)
	{
		delete m_q2k.front();
		m_q2k.pop();
	}
	while (m_q8k.size() > 0)
	{
		delete m_q8k.front();
		m_q8k.pop();
	}
	while (m_q16k.size() > 0)
	{
		delete m_q16k.front();
		m_q16k.pop();
	}

	m_ui2kCnt = 0;
	m_ui8kCnt = 0;
	m_ui16kCnt = 0;
}

void ReadCache::Init(int i2kCnt, int i8kCnt, int i16kCnt)
{
	m_ui2kCnt = i2kCnt;
	m_ui8kCnt = i8kCnt;
	m_ui16kCnt = i16kCnt;

	char *p = NULL;
	for (uint32_t i = 0; i < m_ui2kCnt; i++)
	{
		p = new char[SIZE_BUFFER_2k];
		m_q2k.push(p);
	}

	for (uint32_t i = 0; i < m_ui8kCnt; i++)
	{
		p = new char[SIZE_BUFFER_8k];
		m_q8k.push(p);
	}

	for (uint32_t i = 0; i < m_ui16kCnt; i++)
	{
		p = new char[SIZE_BUFFER_16k];
		m_q16k.push(p);
	}
}

char *ReadCache::Get(int iBuffSize)
{
	char *p = NULL;
	if (iBuffSize <= SIZE_BUFFER_2k)
	{
		if (m_q2k.size() <= 0)
		{
			p = new char[SIZE_BUFFER_2k];
		}
		else
		{
			p = m_q2k.front();
			m_q2k.pop();
		}
	}
	else if (iBuffSize <= SIZE_BUFFER_8k)
	{
		if (m_q8k.size() <= 0)
		{
			p = new char[SIZE_BUFFER_8k];
		}
		else
		{
			p = m_q8k.front();
			m_q8k.pop();
		}
	}
	else if (iBuffSize <= SIZE_BUFFER_16k)
	{
		if (m_q16k.size() <= 0)
		{
			p = new char[SIZE_BUFFER_16k];
		}
		else
		{
			p = m_q16k.front();
			m_q16k.pop();
		}
	}
	else
	{
		p = new char[iBuffSize];
	}
	return p;
}

void ReadCache::Free(char *p, int len)
{
	if(p==NULL)return;
	
	if (len <= SIZE_BUFFER_2k)
	{
		if (m_q2k.size() > m_ui2kCnt)
			delete[] p;
		else
			m_q2k.push(p);
	}
	else if (len <= SIZE_BUFFER_8k)
	{
		if (m_q8k.size() > m_ui8kCnt)
			delete[] p;
		else
			m_q8k.push(p);
	}
	else if (len <= SIZE_BUFFER_16k)
	{
		if (m_q16k.size() > m_ui16kCnt)
			delete[] p;
		else
			m_q16k.push(p);
	}
	else
	{
		delete[] p;
	}
	
}
