#ifndef _REDUCEBALANCE_DTO_HEADER_
#define _REDUCEBALANCE_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"

class CReduceBalanceDto : public CJsonObjectBase
{
public:
    CReduceBalanceDto();
    virtual ~CReduceBalanceDto(){};
    CJsonObjectBase *requestOperation();
    void respDtoSerialize(){};

public:
    string uid;
    int type;
    string coinId;
    string quantityNum;
};

#endif