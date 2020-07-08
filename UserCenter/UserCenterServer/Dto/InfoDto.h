#ifndef _INFO_DTO_HEADER_
#define _INFO_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"

class CInfoDto : public CJsonObjectBase
{
public:
    CInfoDto();
    virtual ~CInfoDto(){};
    CJsonObjectBase *requestOperation();
    void respDtoSerialize(){};

public:
    string uid;
    vector<string> column;
    vector<string> value;
    int method;
};

#endif