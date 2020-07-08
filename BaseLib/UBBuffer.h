
#ifndef _UBRFFER_HEADER_
#define _UBUFFER_HEADER_

#include "UBHeader.h"


// 目前buffer内数据是先进先出
class UBBuffer
{
public:
	UBBuffer():m_pBuffer(NULL),m_uiBufferSize(0),m_uiDataLen(0),m_uiDataPos(0),m_bAutoFree(true){};
	UBBuffer(char *p,int s,bool bAutoFree=false):m_pBuffer(p),m_uiBufferSize(s),m_uiDataLen(s),m_uiDataPos(0),m_bAutoFree(bAutoFree)
	{
		m_pBuffer = new char[s];
		memcpy(m_pBuffer,p,s);
		m_uiDataLen = s;
		m_uiBufferSize = s;
	};
	UBBuffer(int sz):m_pBuffer(NULL),m_uiBufferSize(0),m_uiDataLen(0),m_uiDataPos(0),m_bAutoFree(true)
	{
		m_pBuffer=new char[sz];
		m_uiBufferSize=sz;
	};
	virtual ~UBBuffer(){
		if(NULL != m_pBuffer && m_bAutoFree)
		{
			delete []m_pBuffer;
			m_pBuffer = NULL;
		}
		};

	// 无参数构造函数后的初始化 
	void Init(char *p,int s);
	void Init(int sz);

	// 返回现有数据的地址和长度
	char *Data(){return m_pBuffer+m_uiDataPos;};
	uint32_t DataLength(){return m_uiDataLen;};

	// 在当前已有数据的末尾添加数据, 返回实际添加进去的大小
	uint32_t AppendData(string &sData){return AppendData(sData.data(),sData.length());};
	uint32_t AppendData(const char *pData,uint32_t uiLen);

	// 从数据头部开始删除，并删除指定长度的数据，返回实际删掉的数据
	uint32_t RemoveData(uint32_t uiLength);

	void Swap(UBBuffer *p);
	void Clear();

public:
	char *m_pBuffer;        		// 
	uint32_t m_uiBufferSize;         // buffer size
	uint32_t m_uiDataLen;            // data length in buffer
	uint32_t m_uiDataPos;
	bool m_bAutoFree;       
};




#endif
