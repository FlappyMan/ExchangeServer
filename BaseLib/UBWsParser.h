
#ifndef _UBWSPARSER_HEADER_
#define _UBWSPARSER_HEADER_

#include "UBHttpParser.h"
#include "UBBuffer.h"



enum UBWsParserFrameType 
{
	ERROR_FRAME=0xFF00,
	INCOMPLETE_FRAME=0xFE00,

	OPENING_FRAME=0x3300,
	CLOSING_FRAME=0x3400,

	CONTINUATION_TEXT_FRAME=0x01,
	CONTINUATION_BINARY_FRAME=0x02,
	CONTINUATION_FRAME=0X03,

	TEXT_FRAME=0x81,
	BINARY_FRAME=0x82,

	PING_FRAME=0x19,
	PONG_FRAME=0x1A
};



class UBWsParser : public UBHttpParser
{
public:
    UBWsParser(){m_ubPayloadBuf.Init(SIZE_BUFFER_8k);};
    ~UBWsParser(){};

	bool WSReplyHeader(string &sOutHeader);		// 生成 WebSocket的返回头
	int64_t WSSetFrame(UBWsParserFrameType ft, uint8_t *pData, uint32_t iDataLen, uint8_t *pBuffer, uint32_t iBufferSize);
	int64_t WSGetFrame(UBWsParserFrameType *pFrameType, uint8_t *pOutBuffer, int iOutBufferSize, uint8_t *pInBuffer, int iInBufferLen, int &iFrameFinFlag, int &iFrameHeaderLen);
public:
	UBBuffer m_ubPayloadBuf;
};

#endif


