#include "PayDto.h"
#include "../Model/MsgRespDto.hpp"
#include "DispatchManager.h"
CPayDto::CPayDto()
{
    CDispatchManager::g_DBAccount.init(SIZE_BUFFER_2k);
}

CJsonObjectBase *CPayDto::requestOperation()
{
#ifdef PRINT_LOG
    DEBUG_INFO("[CPayDto::requestOperation]");
#endif
    string sError;
    CMsgRespDto *respDto = new CMsgRespDto;
    if (uid.length() == 0 || paypassword.length() == 0)
    {
        respDto->status = 0;
        respDto->data = "参数错误";
    }
    else if (method != 1 && method != 2)
    {
        respDto->status = 0;
        respDto->data = "参数错误";
    }
    else
    {
        switch (method)
        {
        case 1:
        {
            // 1-验证交易密码
            CDispatchManager::g_DBAccount.Sql("select user_id from user_info where paypassword = \"", false, true);
            CDispatchManager::g_DBAccount.Sql(paypassword, true, false);
            CDispatchManager::g_DBAccount.Sql("\" and user_id = \"", false, false);
            CDispatchManager::g_DBAccount.Sql(uid, true, false);
            CDispatchManager::g_DBAccount.Sql("\";", false, false);
            DEBUG_INFO(CDispatchManager::g_DBAccount.getSql());
            if (CDispatchManager::g_DBAccount.Select())
            {
                if (CDispatchManager::g_DBAccount.NextRow() == false)
                {
                    respDto->status = 0;
                    respDto->data = "交易密码验证不通过";
                }
                else
                {
                    respDto->status = 1;
                    respDto->data = "交易密码验证通过";
                }
            }
            else
            {
                respDto->status = 0;
                respDto->data = "操作失败";
            }
        }
        break;
        case 2:
        {
            // 2-修改交易密码
            CDispatchManager::g_DBAccount.Sql("update user_info set paypassword = \"", false, true);
            CDispatchManager::g_DBAccount.Sql(paypassword, true, false);
            CDispatchManager::g_DBAccount.Sql("\" where user_id = \"", false, false);
            CDispatchManager::g_DBAccount.Sql(uid, true, false);
            CDispatchManager::g_DBAccount.Sql("\";", false, false);
            DEBUG_INFO(CDispatchManager::g_DBAccount.getSql());
            if (CDispatchManager::g_DBAccount.Exec() < 0)
            {
                respDto->status = 0;
                respDto->data = "交易密码修改失败";
            }
            else
            {
                respDto->status = 1;
                respDto->data = "交易密码修改成功";
            }
        }
        break;
        default:
            break;
        }
    }

    respDto->m_client = m_client;
    return (CJsonObjectBase *)respDto;
}