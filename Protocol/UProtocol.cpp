#include "UProtocol.h"

// return <0: 协议错误，=0:数据包长度不足，>0:第一个包长度
int ProtocolHead(uint16_t &uiType,uint32_t &uiPackLen,char *pBuff,int uiLen)
{
    if(uiLen<PROTOCOL_LEN_HEADER)
    {
        return 0;
    }

    // type
    memcpy(&uiType,pBuff,sizeof(uint16_t));
    uiType=ntohs(uiType);
    char *pos=pBuff+sizeof(uint16_t);

    // value length
    memcpy(&uiPackLen,pos,sizeof(uint32_t));
    uiPackLen=ntohl(uiPackLen)+PROTOCOL_LEN_HEADER;
    pos+=sizeof(uint32_t);
    if(uiPackLen>PROTOCOL_LEN_MAX)
    {
        return -1;
    }
    
    return uiPackLen;
}


