#ifndef _UPDATE_DTO_HEADER_
#define _UPDATE_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"


class CUpdateDto : public CJsonObjectBase
{
public:
    CUpdateDto();
    virtual ~CUpdateDto(){};
    CJsonObjectBase * requestOperation();
    void respDtoSerialize(){};
public:
    string userAccount;
    string password;
};

#endif