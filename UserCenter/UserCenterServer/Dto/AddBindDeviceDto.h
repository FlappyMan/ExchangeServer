#ifndef _ADDBINDDEVICE_DTO_HEADER_
#define _ADDBINDDEVICE_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"


class CAddBindDeviceDto : public CJsonObjectBase
{
public:
    CAddBindDeviceDto();
    virtual ~CAddBindDeviceDto(){};
    CJsonObjectBase * requestOperation();
    void respDtoSerialize(){};
public:
    string uid;
    string deviceID;
    string deviceName;
    string deviceOS;
    string token;
    int tokenTime;
    int sourceType;
};

#endif