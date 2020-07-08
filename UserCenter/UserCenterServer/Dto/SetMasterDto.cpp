#include "SetMasterDto.h"
#include "../Model/MsgRespDto.hpp"
#include "DispatchManager.h"
CSetMasterDto::CSetMasterDto()
{
    CDispatchManager::g_DBOther.init(SIZE_BUFFER_2k);
}

CJsonObjectBase *CSetMasterDto::requestOperation()
{
#ifdef PRINT_LOG
    DEBUG_INFO("[CSetMasterDto::requestOperation]");
#endif
    CMsgRespDto *respDto = new CMsgRespDto;
    if (deviceID.length() == 0 || uid.length() == 0)
    {
        respDto->status = 0;
        respDto->data = "参数错误";
    }
    else if (status != 1 && status != 0)
    {
        respDto->status = 0;
        respDto->data = "参数错误";
    }
    else
    {
        CDispatchManager::g_DBOther.Sql("SELECT id FROM user_device where status = 1 AND uid=", false, true);
        CDispatchManager::g_DBOther.Sql(uid, true, false);
        CDispatchManager::g_DBOther.Sql(";", false, false);

        DEBUG_INFO(CDispatchManager::g_DBOther.getSql());
        if (CDispatchManager::g_DBOther.Select())
        {
            if (CDispatchManager::g_DBOther.NextRow() == true)
            {
                string id;
                CDispatchManager::g_DBOther.Value(0, id);
                CDispatchManager::g_DBOther.Sql("UPDATE user_device SET status = 0 where id=", false, true);
                CDispatchManager::g_DBOther.Sql(id, true, false);
                CDispatchManager::g_DBOther.Sql(";", false, false);
                // 以上是讲之前的主设备充值；以下是新增现在的deviceid为主设备
                CDispatchManager::g_DBOther.Sql("UPDATE user_device SET status = 1 where uid=", false, false);
                CDispatchManager::g_DBOther.Sql(uid, true, false);
                CDispatchManager::g_DBOther.Sql(" AND device_id = \"", false, false);
                CDispatchManager::g_DBOther.Sql(deviceID, true, false);
                CDispatchManager::g_DBOther.Sql("\";", false, false);
                DEBUG_INFO(CDispatchManager::g_DBOther.getSql());
                if (CDispatchManager::g_DBOther.Exec() >= 0)
                {
                    respDto->status = 1;
                    respDto->data = "设置成功";
                }
                else
                {
                    respDto->status = 0;
                    respDto->data = "操作失败";
                }
            }
            else
            {
                CDispatchManager::g_DBOther.Sql("UPDATE user_device SET status = 1 where uid=", false, true);
                CDispatchManager::g_DBOther.Sql(uid, true, false);
                CDispatchManager::g_DBOther.Sql(" AND device_id = \"", false, false);
                CDispatchManager::g_DBOther.Sql(deviceID, true, false);
                CDispatchManager::g_DBOther.Sql("\";", false, false);
                DEBUG_INFO(CDispatchManager::g_DBOther.getSql());
                if (CDispatchManager::g_DBOther.Exec() >= 0)
                {
                    respDto->status = 1;
                    respDto->data = "设置成功";
                }
                else
                {
                    respDto->status = 0;
                    respDto->data = "操作失败";
                }
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