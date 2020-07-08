/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-11 19:05:29
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Dto/LoginDto.cpp
 */ 
#include "LoginDto.h"
#include "../Model/LoginRespDto.hpp"
#include "DispatchManager.h"
CLoginDto::CLoginDto()
{
    CDispatchManager::g_DBAccount.init(SIZE_BUFFER_2k);
    CDispatchManager::g_DBOther.init(SIZE_BUFFER_4k);
}

CJsonObjectBase *CLoginDto::requestOperation()
{
#ifdef PRINT_LOG
    DEBUG_INFO("[CLoginDto][requestOperation]");
#endif
    CLoginRespDto *respDto = new CLoginRespDto;
    if (userAccount.length() == 0 || password.length() == 0)
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
        CDispatchManager::g_DBAccount.Sql("select user_id from user_account where account in (select user_account from user where user_account =  \"", false, true);
        CDispatchManager::g_DBAccount.Sql(userAccount, true, false);
        CDispatchManager::g_DBAccount.Sql("\" and password = \"", false, false);
        CDispatchManager::g_DBAccount.Sql(password, true, false);
        CDispatchManager::g_DBAccount.Sql("\");", false, false);
        DEBUG_INFO(CDispatchManager::g_DBAccount.getSql());
        if (CDispatchManager::g_DBAccount.Select())
        {
            if (CDispatchManager::g_DBAccount.NextRow())
            {
                stringstream sTime, sDeadLine; // 当前时间，token截止时间
                srand((unsigned)time(NULL));   // 随机种子

                time_t nowTime = time(NULL);
                int64_t deadLine = (int64_t)nowTime + 604800;

                sTime << nowTime;
                sDeadLine << deadLine;

                int userId;
                CDispatchManager::g_DBAccount.Value(0, userId);

                string sToken = CDispatchManager::IntHexToString(deadLine, 8);
                sToken += CDispatchManager::IntHexToString(userId, 8);

                string md5Value = sToken + TOKEN_MD5_KEY;
                sToken += CDispatchManager::md5(md5Value);

                char cData = sToken.data()[sToken.length() - 1];
                sToken.insert(0, string(&cData, 1));

                // 登陆次数加1
                CDispatchManager::g_DBAccount.Sql("UPDATE user set login_attempts = IFNULL(login_attempts,0)+1 WHERE user_account = \"", false, true);
                CDispatchManager::g_DBAccount.Sql(userAccount, true, false);
                CDispatchManager::g_DBAccount.Sql("\";", false, false);
                DEBUG_INFO(CDispatchManager::g_DBAccount.getSql());
                if (CDispatchManager::g_DBAccount.Exec() >= 0)
                {
                    Json::Value sJson;
                    sJson["token"] = sToken;
                    sJson["deadLine"] = sDeadLine.str();
                    sJson["uid"] = userId;

                    Json::FastWriter writer;
                    string dataString = writer.write(sJson);

                    respDto->status = 1;
                    respDto->data = dataString;
                }
                else
                {
                    respDto->status = 0;
                    respDto->data = "操作失败";
                }
            }
            else
            {
                respDto->data = "数据有误";
                respDto->status = 0;
            }
        }
        else
        {
            DEBUG_INFO(CDispatchManager::g_DBAccount.Error());
            respDto->status = 0;
            respDto->data = "process error";
        }
    }

    respDto->m_client = m_client;
    return (CJsonObjectBase *)respDto;
}