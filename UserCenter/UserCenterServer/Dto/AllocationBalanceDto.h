#ifndef _ALLOCATIONBALANCE_DTO_HEADER_
#define _ALLOCATIONBALANCE_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"

class CAllocationBalanceDto : public CJsonObjectBase
{
public:
    CAllocationBalanceDto();
    virtual ~CAllocationBalanceDto(){};
    CJsonObjectBase * requestOperation();
    void respDtoSerialize(){};
public:
    string uid;
    string coinId;
    string assetsNum;
    int method;
};

#endif