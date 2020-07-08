#ifndef _FILEREQUEST_H_
#define _FILEREQUEST_H_

#include "UBaseLib.h"
#include "proto/ukex.filerequest.pb.h"
#include "UProtocolBase.h"

class UPFileRequest : public UProtocolBase, public ukex::filerequest
{
public:
	UPFileRequest(){};
	virtual ~UPFileRequest(){};

	void JsonPack(Json::Value &root){};
	bool JsonUnpack(Json::Value &root){return true;};
};

#endif

