#include "CheckBindDeviceDto.h"
#include "../Model/CheckBindDeviceRespDto.hpp"
#include "DispatchManager.h"
CCheckBindDeviceDto::CCheckBindDeviceDto()
{
    CDispatchManager::g_DBOther.init(SIZE_BUFFER_4k);
}

CJsonObjectBase *CCheckBindDeviceDto::requestOperation()
{
#ifdef PRINT_LOG
    DEBUG_INFO("[CCheckBindDeviceDto::requestOperation]");
#endif
    CCheckBindDeviceRespDto *respDto = new CCheckBindDeviceRespDto;
    if (uid.length() == 0 || deviceID.length() == 0)
    {
        respDto->status = 0;
        respDto->data = "参数错误";
    }
    else
    {
        string sError;
        time_t tNowTime = time(NULL);

        CDispatchManager::g_DBOther.Sql("SELECT status FROM user_device WHERE uid = \"", false, true);
        CDispatchManager::g_DBOther.Sql(uid, true, false);
        CDispatchManager::g_DBOther.Sql("\" AND device_id = \"", false, false);
        CDispatchManager::g_DBOther.Sql(deviceID, true, false);
        CDispatchManager::g_DBOther.Sql("\";", false, false);
        DEBUG_INFO(CDispatchManager::g_DBOther.getSql());
        if (CDispatchManager::g_DBOther.Select())
        {
            if (CDispatchManager::g_DBOther.NextRow())
            {
                int uiResult = 0;
                CDispatchManager::g_DBOther.Value(0, uiResult);

                Json::Value sJson;
                sJson["groupid"] = uiResult;
                Json::FastWriter writer;
                string dataString = writer.write(sJson);

                respDto->data = dataString;
                respDto->status = 1;
            }
            else
            {
                respDto->status = 0;
                respDto->data = "错误信息";
            }
        }
        else
        {
            respDto->status = 0;
            respDto->data = "查询失败";
        }
    }

    respDto->m_client = m_client;
    return (CJsonObjectBase *)respDto;
}