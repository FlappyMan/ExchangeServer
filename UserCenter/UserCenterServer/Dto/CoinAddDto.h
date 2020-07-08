#ifndef _COINADD_DTO_HEADER_
#define _COINADD_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"


class CCoinAddDto : public CJsonObjectBase
{
public:
    CCoinAddDto();
    virtual ~CCoinAddDto(){};
    CJsonObjectBase * requestOperation();
    void respDtoSerialize(){};
public:
    string name;
    string type;
    string title;
};

#endif