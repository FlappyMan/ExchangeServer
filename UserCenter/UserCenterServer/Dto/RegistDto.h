#ifndef _REGIST_DTO_HEADER_
#define _REGIST_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"


class CRegistDto : public CJsonObjectBase
{
public:
    CRegistDto();
    virtual ~CRegistDto(){};
    CJsonObjectBase * requestOperation();
    void respDtoSerialize(){};
public:
    string deviceID;
    string userAccount;
    string password;
    int sourceType;
};

#endif