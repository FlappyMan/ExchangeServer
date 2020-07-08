

#ifndef _UBJSON_HEADER_
#define _UBJSON_HEADER_



#include "UBHeader.h"

// #include "/usr/local/UkexServer/lib/jsoncpp-1.9.1/include/json/json.h"
#include "json/json.h"
/*
return <0: failed,  >=0:success
*/
int UBJsonPack(char* pBuff,int iBuffSize,Json::Value &root,bool bGoodLook=false);
int UBJsonPack(string &str,Json::Value &root,bool bGoodLook=false);


bool UBJsonUnpack(Json::Value &root,char *pBuff,int iBuffSize);
bool UBJsonUnpack(Json::Value &root,string &str);





#endif



