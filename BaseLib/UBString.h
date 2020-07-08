
#ifndef _UBSTRING_HEADER_
#define _UBSTRING_HEADER_

#include "UBHeader.h"

class UBString
{

};



void UBStringSplit(vector< std::string > &vRet,const string& str, char d);
void UBStringSplit(vector< std::string > &vRet,const string& str, string& delim);

void UBStringToUpper(string &s);
void UBStringToLower(string &s);

#endif

