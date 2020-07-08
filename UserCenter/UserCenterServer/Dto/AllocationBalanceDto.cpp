#include "AllocationBalanceDto.h"
#include "../Model/MsgRespDto.hpp"
#include "DispatchManager.h"
CAllocationBalanceDto::CAllocationBalanceDto()
{
    CDispatchManager::g_DBAssets.init(SIZE_BUFFER_2k);
}

CJsonObjectBase *CAllocationBalanceDto::requestOperation()
{
#ifdef PRINT_LOG
    DEBUG_INFO("[CAllocationBalanceDto::requestOperation]");
#endif
    CMsgRespDto *respDto = new CMsgRespDto;

    if (uid.length() == 0 || coinId.length() == 0 || assetsNum.length() == 0)
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
        CDispatchManager::g_DBAssets.Sql("select usable,freeze from user_assets_", false, true);
        CDispatchManager::g_DBAssets.Sql(coinId, true, false);
        CDispatchManager::g_DBAssets.Sql(" where status = 1 and user_id = \"", false, false);
        CDispatchManager::g_DBAssets.Sql(uid, true, false);
        CDispatchManager::g_DBAssets.Sql("\";", false, false);
        DEBUG_INFO(CDispatchManager::g_DBAssets.getSql());
        if (CDispatchManager::g_DBAssets.Select())
        {
            if (CDispatchManager::g_DBAssets.NextRow())
            {
                string usable, freeze;
                CDispatchManager::g_DBAssets.Value(0, usable);
                CDispatchManager::g_DBAssets.Value(1, freeze);
                if (method == 1)
                {
                    string sUsable = CDispatchManager::minusLongDouble(usable, assetsNum);
                    if (sUsable == "-1")
                    {
                        respDto->status = 0;
                        respDto->data = "可用资产不足";
                    }
                    else
                    {
                        string sFreeze = CDispatchManager::addLongDouble(freeze, assetsNum);
                        CDispatchManager::g_DBAssets.Sql("UPDATE user_assets_", false, true);
                        CDispatchManager::g_DBAssets.Sql(coinId, true, false);
                        CDispatchManager::g_DBAssets.Sql(" SET usable = \"", false, false);
                        CDispatchManager::g_DBAssets.Sql(sUsable, true, false);
                        CDispatchManager::g_DBAssets.Sql("\", freeze = \"", false, false);
                        CDispatchManager::g_DBAssets.Sql(sFreeze, true, false);
                        CDispatchManager::g_DBAssets.Sql("\" where status = 1 and user_id = \"", false, false);
                        CDispatchManager::g_DBAssets.Sql(uid, true, false);
                        CDispatchManager::g_DBAssets.Sql("\";", false, false);
                        DEBUG_INFO(CDispatchManager::g_DBAssets.getSql());

                        if (CDispatchManager::g_DBAssets.Exec() >= 0)
                        {
                            respDto->status = 1;
                            respDto->data = "操作成功";
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
                    string sUsable = CDispatchManager::addLongDouble(usable, assetsNum);
                    string sFreeze = CDispatchManager::minusLongDouble(freeze, assetsNum);
                    if (sFreeze == "-1")
                    {
                        respDto->status = 0;
                        respDto->data = "冻结资产不足";
                    }
                    else
                    {
                        CDispatchManager::g_DBAssets.Sql("UPDATE user_assets_", false, true);
                        CDispatchManager::g_DBAssets.Sql(coinId, true, false);
                        CDispatchManager::g_DBAssets.Sql(" SET usable = \"", false, false);
                        CDispatchManager::g_DBAssets.Sql(sUsable, true, false);
                        CDispatchManager::g_DBAssets.Sql("\", freeze = \"", false, false);
                        CDispatchManager::g_DBAssets.Sql(sFreeze, true, false);
                        CDispatchManager::g_DBAssets.Sql("\" where status = 1 and user_id = \"", false, false);
                        CDispatchManager::g_DBAssets.Sql(uid, true, false);
                        CDispatchManager::g_DBAssets.Sql("\";", false, false);
                        DEBUG_INFO(CDispatchManager::g_DBAssets.getSql());

                        if (CDispatchManager::g_DBAssets.Exec() >= 0)
                        {
                            respDto->status = 1;
                            respDto->data = "操作成功";
                        }
                        else
                        {
                            respDto->status = 0;
                            respDto->data = "操作失败";
                        }
                    }
                }
            }
            else
            {
                respDto->status = 0;
                respDto->data = "无查询数据";
            }
        }
        else
        {
            respDto->status = 0;
            respDto->data = "信息错误";
        }
    }

    respDto->m_client = m_client;
    return (CJsonObjectBase *)respDto;
}