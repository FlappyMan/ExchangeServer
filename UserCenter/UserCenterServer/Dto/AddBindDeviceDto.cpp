#include "AddBindDeviceDto.h"
#include "../Model/MsgRespDto.hpp"
#include "DispatchManager.h"
CAddBindDeviceDto::CAddBindDeviceDto()
{
    CDispatchManager::g_DBOther.init(SIZE_BUFFER_2k);  
}

CJsonObjectBase *CAddBindDeviceDto::requestOperation()
{
#ifdef PRINT_LOG
    DEBUG_INFO("[CAddBindDeviceDto::requestOperation]");
#endif
    CMsgRespDto *respDto = new CMsgRespDto;

    if (uid.length() == 0 || deviceID.length() == 0 || deviceOS.length() == 0 || deviceName.length() == 0)
    {
        respDto->status = 0;
        respDto->data = "参数错误";
    }
    else if (sourceType != 1 && sourceType != 2)
    {
        respDto->status = 0;
        respDto->data = "参数错误";
    }
    else
    {
        string sError;
        string name = deviceOS.length() == 0 ? deviceName : deviceOS;
        time_t tNowTime = time(NULL);

        CDispatchManager::g_DBOther.Sql("INSERT INTO user_device(device_id,uid,name,token,token_time,public_key,equipment,comment,source_type,status,create_time,update_time) VALUES (\"", false, true);
        CDispatchManager::g_DBOther.Sql(deviceID, true, false);
        CDispatchManager::g_DBOther.Sql("\",", false, false);
        CDispatchManager::g_DBOther.Sql(uid, true, false);
        CDispatchManager::g_DBOther.Sql(",\"", false, false);
        CDispatchManager::g_DBOther.Sql(name, true, false);
        CDispatchManager::g_DBOther.Sql("\",\"", false, false);
        CDispatchManager::g_DBOther.Sql(token, true, false);
        CDispatchManager::g_DBOther.Sql("\",", false, false);
        string sDeadLineTemp = to_string(tokenTime);
        CDispatchManager::g_DBOther.Sql(sDeadLineTemp, false, false);
        CDispatchManager::g_DBOther.Sql(",", false, false);
        CDispatchManager::g_DBOther.Sql("'',", false, false);
        CDispatchManager::g_DBOther.Sql("'',", false, false);
        CDispatchManager::g_DBOther.Sql("'',", false, false);
        string st = to_string(sourceType);
        CDispatchManager::g_DBOther.Sql(st, true, false);
        CDispatchManager::g_DBOther.Sql(",0,", false, false);
        string nt = to_string(tNowTime);
        CDispatchManager::g_DBOther.Sql(nt, true, false);
        CDispatchManager::g_DBOther.Sql(",", false, false);
        CDispatchManager::g_DBOther.Sql(nt, true, false);
        CDispatchManager::g_DBOther.Sql(");", false, false);
        DEBUG_INFO(CDispatchManager::g_DBOther.getSql());
        if (CDispatchManager::g_DBOther.Exec() < 0)
        {
            respDto->status = 0;
            respDto->data = "设备绑定失败";
        }
        else
        {
            respDto->status = 1;
            respDto->data = "设备绑定成功";
        }
    }
    respDto->m_client = m_client;
    return (CJsonObjectBase *)respDto;
}