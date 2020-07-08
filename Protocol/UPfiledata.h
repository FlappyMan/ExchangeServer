#ifndef _FILEDATA_H_
#define _FILEDATA_H_

#include "UBaseLib.h"
#include "proto/ukex.filedata.pb.h"
#include "UProtocolBase.h"

class UPFileData : public UProtocolBase, public ukex::filedata
{
public:
	UPFileData(){};
	virtual ~UPFileData(){};

	void JsonPack(Json::Value &root){};
	bool JsonUnpack(Json::Value &root){return true;};
};

#endif
