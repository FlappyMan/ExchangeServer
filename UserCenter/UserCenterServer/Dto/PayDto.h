#ifndef _PAY_DTO_HEADER_
#define _PAY_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"

class CPayDto : public CJsonObjectBase
{
public:
    CPayDto();
    virtual ~CPayDto(){};
    CJsonObjectBase * requestOperation();
    void respDtoSerialize(){};
public:
    string uid;
    string paypassword;
    int method;
};

#endif