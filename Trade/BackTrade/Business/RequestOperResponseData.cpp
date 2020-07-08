#include "RequestOperResponseData.h"

CRequestOperResponseData::CRequestOperResponseData(CResponseModel model)
{
    m_mResponseModel = model;
}
string CRequestOperResponseData::getRequestToken()
{
    return m_mResponseModel.token;
}