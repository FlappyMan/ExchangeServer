#ifndef _CHECHBINDDEVICE_DTO_HEADER_
#define _CHECHBINDDEVICE_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"


class CCheckBindDeviceDto : public CJsonObjectBase
{
public:
    CCheckBindDeviceDto();
    virtual ~CCheckBindDeviceDto(){};
    CJsonObjectBase * requestOperation();
    void respDtoSerialize(){};
public:
    string uid;
    string deviceID;
};

#endif