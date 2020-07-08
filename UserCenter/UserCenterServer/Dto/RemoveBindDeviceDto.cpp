#include "RemoveBindDeviceDto.h"
#include "../Model/MsgRespDto.hpp"
#include "DispatchManager.h"
CRemoveBindDeviceDto::CRemoveBindDeviceDto()
{
    CDispatchManager::g_DBOther.init(SIZE_BUFFER_2k);
}

CJsonObjectBase *CRemoveBindDeviceDto::requestOperation()
{
#ifdef PRINT_LOG
    DEBUG_INFO("[CRemoveBindDeviceDto::requestOperation]");
#endif
    CMsgRespDto *respDto = new CMsgRespDto;

    if (uid.length() == 0 || deviceID.length() == 0)
    {
        respDto->status = 1;
        respDto->data = "参数错误";
    }
    else
    {
        string sError;
        time_t tNowTime = time(NULL);

        CDispatchManager::g_DBOther.Sql("select device_id from user_device where uid = \"", false, true);
        CDispatchManager::g_DBOther.Sql(uid, true, false);
        CDispatchManager::g_DBOther.Sql("\" and status = 1;", false, false);
        DEBUG_INFO(CDispatchManager::g_DBOther.getSql());
        if (CDispatchManager::g_DBOther.Select())
        {
            if (CDispatchManager::g_DBOther.NextRow())
            {
                CDispatchManager::g_DBOther.Sql("DELETE FROM user_device where uid = \"", false, true);
                CDispatchManager::g_DBOther.Sql(uid, true, false);
                CDispatchManager::g_DBOther.Sql("\" and device_id = \"", false, false);
                CDispatchManager::g_DBOther.Sql(deviceID, true, false);
                CDispatchManager::g_DBOther.Sql("\" and status = 1;", false, false);
                DEBUG_INFO(CDispatchManager::g_DBOther.getSql());
                if (CDispatchManager::g_DBOther.Exec() < 0)
                {
                    CDispatchManager::g_DBOther.Error(sError);
                    respDto->status = 0;
                    respDto->data = "设备删除失败";
                }
                else
                {
                    respDto->status = 1;
                    respDto->data = "设备删除成功";
                }
            }
            else
            {
                respDto->status = 0;
                respDto->data = "设备删除失败";
            }
        }
        else
        {
            respDto->status = 0;
            respDto->data = "操作失败";
        }
    }

    respDto->m_client = m_client;
    return (CJsonObjectBase *)respDto;
}