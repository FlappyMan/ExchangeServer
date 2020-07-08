
#include "UBBuffer.h"

void UBBuffer::Init(char *p,int s)
{
	m_pBuffer=p;
	m_uiBufferSize=s;
	m_bAutoFree=false;
}
void UBBuffer::Init(int sz)
{
	m_pBuffer=new char[sz];
	m_uiBufferSize=sz;
	m_bAutoFree=true;
}

uint32_t UBBuffer::AppendData(const char *pData,uint32_t uiLen)
{
	uint32_t uiSpace=m_uiBufferSize-m_uiDataPos+m_uiDataLen;
	uint32_t uiAppended=uiLen>uiSpace?uiSpace:uiLen;
	if(uiAppended>0)
	{
		memcpy(m_pBuffer+m_uiDataPos+m_uiDataLen,pData,uiAppended);
		m_uiDataLen+=uiAppended;
	}
	return uiAppended;
}


uint32_t UBBuffer::RemoveData(uint32_t uiLength)
{
	if(uiLength>=m_uiDataLen || uiLength>=m_uiBufferSize)
	{
		m_uiDataPos=0;
		m_uiDataLen=0;
		return 0;
	}

	uint32_t uiMove=m_uiDataLen-uiLength;
	memmove(m_pBuffer,m_pBuffer+m_uiDataPos+uiLength,uiMove);
	
	m_uiDataPos=0;
	m_uiDataLen-=uiMove;
	return m_uiDataLen;
}

void UBBuffer::Clear()
{
	m_uiDataPos=0;
	m_uiDataLen=0;
}


void UBBuffer::Swap(UBBuffer *p)
{
	char *pBuffer=p->m_pBuffer;
	p->m_pBuffer=m_pBuffer;
	m_pBuffer=pBuffer;

	uint32_t tmp=p->m_uiBufferSize;
	p->m_uiBufferSize=m_uiBufferSize;
	m_uiBufferSize=tmp;

	tmp=p->m_uiDataLen;
	p->m_uiDataLen=m_uiDataLen;
	m_uiDataLen=tmp;

	tmp=p->m_uiDataPos;
	p->m_uiDataPos=m_uiDataPos;
	m_uiDataPos=tmp;

	bool b=p->m_bAutoFree;
	p->m_bAutoFree=m_bAutoFree;
	m_bAutoFree=b;     
}
