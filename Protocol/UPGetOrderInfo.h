/*
 * @Author: your name
 * @Date: 2020-07-02 23:54:24
 * @LastEditTime: 2020-07-05 18:31:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Protocol/UPGetOrderInfo.h
 */ 
#ifndef _GETORDERINFO_H_
#define _GETORDERINFO_H_

#include "UBaseLib.h"
#include "proto/ukex.getorderinfo.pb.h"
#include "UProtocolBase.h"

class UPGetOrderInfo : public UProtocolBase, public ukex::getorderinfo
{
public:
	UPGetOrderInfo(){};
	virtual ~UPGetOrderInfo(){};

	void JsonPack(Json::Value &root);
	bool JsonUnpack(Json::Value &root);
};

#endif

