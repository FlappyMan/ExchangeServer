#ifndef _AUTH_DTO_HEADER_
#define _AUTH_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"


class CAuthDto : public CJsonObjectBase
{
public:
    CAuthDto();
    virtual ~CAuthDto(){};
    CJsonObjectBase * requestOperation();
    void respDtoSerialize(){};
public:
    string uid;
    vector<string> column;
    vector<string> value;
    int method;
};

#endif