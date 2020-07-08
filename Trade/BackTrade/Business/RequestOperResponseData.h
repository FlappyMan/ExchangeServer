#ifndef _RESPONSE_REQUESTOPER_HEADER_
#define _RESPONSE_REQUESTOPER_HEADER_
#include "BaseRequestOper.h"
#include "ResponseModel.hpp"

class CRequestOperResponseData : public CBaseRequestOper
{
public:
    CRequestOperResponseData(CResponseModel model);
    string getRequestToken();
private:
    CResponseModel m_mResponseModel;
};

#endif
