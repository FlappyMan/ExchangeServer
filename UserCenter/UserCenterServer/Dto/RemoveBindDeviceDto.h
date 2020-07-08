#ifndef _REMOVEBINDDEVICE_DTO_HEADER_
#define _REMOVEBINDDEVICE_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"


class CRemoveBindDeviceDto : public CJsonObjectBase
{
public:
    CRemoveBindDeviceDto();
    virtual ~CRemoveBindDeviceDto(){};
    CJsonObjectBase * requestOperation();
    void respDtoSerialize(){};
public:
    string uid;
    string deviceID;
};

#endif