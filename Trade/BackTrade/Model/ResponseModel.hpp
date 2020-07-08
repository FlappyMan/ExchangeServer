#ifndef _RESPONSE_MODEL_HEADER_
#define _RESPONSE_MODEL_HEADER_

#include "../global.h"
class CResponseModel
{
public:
    CResponseModel &operator=(const CResponseModel &obj)
    {
        if (this != &obj)
        {
            this->status = obj.status;
            this->data = obj.data;
            this->token = obj.token;
        }
        return *this;
    }
    CResponseModel &operator=(UPResponse *pkg)
    {
        this->token = dynamic_cast<UPResponse *>(pkg)->token();
        this->status = dynamic_cast<UPResponse *>(pkg)->status();
        this->data = dynamic_cast<UPResponse *>(pkg)->data();
        return *this;
    }

public:
    string token;
    int status;
    string data;
};

#endif
