#ifndef _UPROTOCOL_H_
#define _UPROTOCOL_H_

#include "UBaseLib.h"
#include "UProtocol_Header.h"


using namespace std;
enum
{
	PROTOCOL_LEN_HEADER=6,			// type(2byte)+pkg_length(4byte)

	PROTOCOL_LEN_MAX=2*1024*1024,	// 最大包长度2M Byte
	PROTOCOL_LEN_MIN=10,			// type(2byte)+pkg_length(4byte)+tag_length(4byte)+tag(0-n bytes)	
};

// return <0: 协议错误，=0:数据包长度不足，>0:第一个包长度
int ProtocolHead(uint16_t &uiType,uint32_t &uiPackLen,char *pBuff,int iLen);

template<class UProtocol>
int ProtoPackSize(UProtocol &up)
{
	string str;
	if(up.SerializeToString(&str)==false)
	{
		return 0;
	}
	return PROTOCOL_LEN_MIN+up.m_sTag.length()+str.length();
};

template<class UProtocol>
int ProtoPack(char *pBuff,uint32_t uiLen, UProtocol &up)
{
	string str;
	if(up.SerializeToString(&str)==false)
	{
		return 0;
	}

	up.m_uiPkgLength=sizeof(uint32_t)+up.m_sTag.length()+str.length();
	const uint32_t iPackSize=PROTOCOL_LEN_HEADER+up.m_uiPkgLength;
	if(uiLen<iPackSize)
	{
		return -iPackSize;
	}
	uint16_t uiType=htons(up.CMD);
	uint32_t uiPkgLen=htonl(up.m_uiPkgLength);
	uint32_t uiTagLen=htonl(up.m_sTag.length());

	char *pos=pBuff;
	memcpy(pos,&uiType,sizeof(uint16_t));
	pos+=sizeof(uint16_t);
	memcpy(pos,&uiPkgLen,sizeof(uint32_t));
	pos+=sizeof(uint32_t);
	memcpy(pos,&uiTagLen,sizeof(uint32_t));
	pos+=sizeof(uint32_t);
	memcpy(pos,up.m_sTag.data(),up.m_sTag.length());
	pos+=up.m_sTag.length();
	memcpy(pos,str.data(),str.length());
	return iPackSize;
};

// pBuff: 包含包头6个字节
template<class UProtocol>
bool ProtoUnpack(UProtocol &up,char *pBuff,uint32_t uiLen)
{
	char *pos=pBuff;

	// type
	uint16_t *p16=(uint16_t*)pos;
	up.m_uiType=ntohs(*p16);
	pos+=sizeof(uint16_t);
	
	// pkg length
	uint32_t *p32=(uint32_t*)pos;
	up.m_uiPkgLength=ntohl(*p32);
	if(up.m_uiPkgLength+PROTOCOL_LEN_HEADER > uiLen)return false;
	pos+=sizeof(uint32_t);
	
	// tag length
	uint32_t uiTagLength=0;
	memcpy(&uiTagLength,pos,sizeof(uint32_t));
	uiTagLength=ntohl(uiTagLength);
	pos+=sizeof(uint32_t);

	// tag
	if(uiTagLength>0)
	{
		up.m_sTag.assign(pos,uiTagLength);
		pos+=uiTagLength;
	}

	// pkg
	string str(pos,up.m_uiPkgLength-sizeof(uint32_t)-uiTagLength);
	stringstream ss(str) ;
	if(up.ParseFromString(str))return true;
	else return false;		
};

template<class UProtocol>
int JsonPack(UProtocol *p,char *pBuff,uint32_t uiLen)
{
	Json::Value root;
	p->JsonPack(root);
	return UBJsonPack(pBuff,uiLen,root);
};

template<class UProtocol>
bool JsonUnpack(UProtocol *p,char *pBuff,uint32_t uiLen)
{
	p->m_uiType = UProtocol::CMD;
	Json::Value root;
	if(UBJsonUnpack(root,pBuff,uiLen)==false)return false;
	return p->JsonUnpack(root);
};

#endif







