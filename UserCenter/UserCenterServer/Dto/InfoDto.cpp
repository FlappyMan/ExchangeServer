#include "InfoDto.h"
#include "../Model/MsgRespDto.hpp"
#include "DispatchManager.h"
CInfoDto::CInfoDto()
{
    CDispatchManager::g_DBAccount.init(SIZE_BUFFER_4k);
}

CJsonObjectBase *CInfoDto::requestOperation()
{
#ifdef PRINT_LOG
    DEBUG_INFO("[CInfoDto::requestOperation]");
#endif
    string sError;
    CMsgRespDto *respDto = new CMsgRespDto;
    int iColumnSize = column.size();
    int iValueSize = value.size();
    if (method != 1 && method != 2)
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
            if (uid.length() == 0 || iColumnSize == 0 || iValueSize == 0)
            {
                respDto->status = 0;
                respDto->data = "参数错误";
            }
            else
            {
                // 1-修改用户权限信息
                if (iColumnSize == iValueSize)
                {
                    CDispatchManager::g_DBAccount.Sql("UPDATE user_info SET ", false, true);

                    for (int i = 0; i < iColumnSize; i++)
                    {
                        CDispatchManager::g_DBAccount.Sql(column[i], true, false);
                        CDispatchManager::g_DBAccount.Sql("=\"", false, false);
                        CDispatchManager::g_DBAccount.Sql(value[i], true, false);
                        if (i != (iColumnSize - 1))
                            CDispatchManager::g_DBAccount.Sql("\",", false, false);
                        else
                            CDispatchManager::g_DBAccount.Sql("\"", false, false);
                    }

                    CDispatchManager::g_DBAccount.Sql(" WHERE user_id = \"", false, false);
                    CDispatchManager::g_DBAccount.Sql(uid, true, false);
                    CDispatchManager::g_DBAccount.Sql("\";", false, false);
                    DEBUG_INFO(CDispatchManager::g_DBAccount.getSql());

                    if (CDispatchManager::g_DBAccount.Exec() < 0)
                    {
                        respDto->status = 0;
                        respDto->data = "修改失败";
                    }
                    else
                    {
                        respDto->status = 1;
                        respDto->data = "修改成功";
                    }
                }
                else
                {
                    respDto->status = 0;
                    respDto->data = "参数错误";
                }
            }
        }
        break;
        case 2:
        {
            if (uid.length() == 0 || iColumnSize == 0)
            {
                respDto->status = 0;
                respDto->data = "参数错误";
            }
            else
            {
                // 2-验证用户权限信息
                CDispatchManager::g_DBAccount.Sql("SELECT ", false, true);
                for (int i = 0; i < iColumnSize; i++)
                {
                    CDispatchManager::g_DBAccount.Sql(column[i], true, false);
                    if (i != (iColumnSize - 1))
                        CDispatchManager::g_DBAccount.Sql(",", false, false);
                }
                CDispatchManager::g_DBAccount.Sql(" FROM user_info WHERE user_id = \"", false, false);
                CDispatchManager::g_DBAccount.Sql(uid, true, false);
                CDispatchManager::g_DBAccount.Sql("\";", false, false);
                DEBUG_INFO(CDispatchManager::g_DBAccount.getSql());
                if (CDispatchManager::g_DBAccount.Select())
                {
                    if (CDispatchManager::g_DBAccount.NextRow())
                    {
                        Json::Value sJson;
                        for (int i = 0; i < iColumnSize; i++)
                        {
                            string value;
                            CDispatchManager::g_DBAccount.Value(i, value);
                            sJson[column[i]] = value;
                        }

                        Json::FastWriter writer;
                        std::string dataString = writer.write(sJson);

                        respDto->status = 1;
                        respDto->data = dataString;
                    }
                    else
                    {
                        respDto->status = 0;
                        respDto->data = "查询错误";
                    }
                }
                else
                {
                    respDto->status = 0;
                    respDto->data = "查询错误";
                }
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