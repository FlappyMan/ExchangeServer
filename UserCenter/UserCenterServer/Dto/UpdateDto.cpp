#include "UpdateDto.h"
#include "../Model/MsgRespDto.hpp"
#include "DispatchManager.h"
CUpdateDto::CUpdateDto()
{
    CDispatchManager::g_DBAccount.init(SIZE_BUFFER_2k);
}

CJsonObjectBase *CUpdateDto::requestOperation()
{
#ifdef PRINT_LOG
    DEBUG_INFO("[CUpdateDto::requestOperation]");
#endif
    CMsgRespDto *respDto = new CMsgRespDto;
    if (userAccount.length() == 0 || password.length() == 0)
    {
        respDto->status = 0;
        respDto->data = "参数错误";
    }
    else
    {
        CDispatchManager::g_DBAccount.Sql("UPDATE user SET password = \"", false, true);
        CDispatchManager::g_DBAccount.Sql(password, true, false);
        CDispatchManager::g_DBAccount.Sql("\" WHERE user_account = \"", false, false);
        CDispatchManager::g_DBAccount.Sql(userAccount, true, false);
        CDispatchManager::g_DBAccount.Sql("\";", false, false);
        DEBUG_INFO(CDispatchManager::g_DBAccount.getSql());
        if (CDispatchManager::g_DBAccount.Exec() < 0)
        {
            respDto->status = 0;
            respDto->data = "密码修改失败";
            
        }
        else
        {
            respDto->status = 1;
            respDto->data = "密码修改成功";
        }
    }

    respDto->m_client = m_client;
    return (CJsonObjectBase *)respDto;
}