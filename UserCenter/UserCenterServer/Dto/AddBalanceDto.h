#ifndef _ADDBALANCE_DTO_HEADER_
#define _ADDBALANCE_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"


class CAddBalanceDto : public CJsonObjectBase
{
public:
    CAddBalanceDto();
    virtual ~CAddBalanceDto(){};
    CJsonObjectBase * requestOperation();
    void respDtoSerialize(){};
public:
    string uid;
    int type;
    string coinId;
    string quantityNum;
};

#endif