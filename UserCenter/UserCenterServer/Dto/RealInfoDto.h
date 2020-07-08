#ifndef _REALINFO_DTO_HEADER_
#define _REALINFO_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"

class CRealInfoDto : public CJsonObjectBase
{
public:
    CRealInfoDto();
    virtual ~CRealInfoDto(){};
    CJsonObjectBase *requestOperation();
    void respDtoSerialize(){};

public:
    string uid;
    vector<string> column;
    vector<string> value;
    int method;
};

#endif