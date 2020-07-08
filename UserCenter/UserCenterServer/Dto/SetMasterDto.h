#ifndef _SETMASTER_DTO_HEADER_
#define _SETMASTER_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"


class CSetMasterDto : public CJsonObjectBase
{
public:
    CSetMasterDto();
    virtual ~CSetMasterDto(){};
    CJsonObjectBase * requestOperation();
    void respDtoSerialize(){};
public:
    int status;
    string uid;
    string deviceID;
};

#endif