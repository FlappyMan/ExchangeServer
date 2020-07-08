#ifndef _HEARTBEAT_H_
#define _HEARTBEAT_H_

#include "UBaseLib.h"
#include "UProtocolBase.h"
/*
* 心跳
*/
class UPHeartBeat : public UProtocolBase
{
public:
    UPHeartBeat(){};
    ~UPHeartBeat(){};

    void JsonPack(Json::Value &root){};
    bool JsonUnpack(Json::Value &root){return true;};     
public:
    uint64_t m_uTimeStamp;
};

#endif