#ifndef _TRADECANCELBAT_REQUESTOPER_HEADER_
#define _TRADECANCELBAT_REQUESTOPER_HEADER_
#include "BaseRequestOper.h"
#include "CancelTradeBatchModel.hpp"
class CRequestOperCancelTradeBatch : public CBaseRequestOper
{
public:
    CRequestOperCancelTradeBatch(CCancelTradeBatchModel model);
    UProtocolBase* requestOperation();
    string getOperationToken();

private:
    int operDB(string coinID, CCancelTradeInfoModel model, string uid, bool bIsReset);

public:
    CCancelTradeBatchModel m_mCancelTradeBatch;
};

#endif
