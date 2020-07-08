#include "AddBalanceDto.h"
#include "../Model/MsgRespDto.hpp"
#include "DispatchManager.h"
CAddBalanceDto::CAddBalanceDto()
{
    CDispatchManager::g_DBAssets.init(SIZE_BUFFER_2k);
}

CJsonObjectBase *CAddBalanceDto::requestOperation()
{
#ifdef PRINT_LOG
    DEBUG_INFO("[CAddBalanceDto::requestOperation]");
#endif
    CMsgRespDto *respDto = new CMsgRespDto;
    if (uid.length() == 0 || coinId.length() == 0)
    {
        respDto->status = 0;
        respDto->data = "参数错误";
    }
    else if (CDispatchManager::minusLongDouble(quantityNum, string("0.0000000000000001")) == "-1")
    {
        respDto->status = 0;
        respDto->data = "参数错误";
    }
    else if(type != 1 && type != 2)
    {
        respDto->status = 0;
        respDto->data = "参数错误";
    }
    else
    {
        string balanceType = type == 1?"usable":"freeze";
        string sqlTemp = "select "+ balanceType +" from user_assets_";
        CDispatchManager::g_DBAssets.Sql(sqlTemp,false,true);
        CDispatchManager::g_DBAssets.Sql(coinId,true,false);
        CDispatchManager::g_DBAssets.Sql(" where status = 1 and user_id =",false,false);
        CDispatchManager::g_DBAssets.Sql(uid,true,false);
        CDispatchManager::g_DBAssets.Sql(";",false,false);
        if (CDispatchManager::g_DBAssets.Select() == true)
        {
            DEBUG_INFO(CDispatchManager::g_DBAssets.getSql());
            if (CDispatchManager::g_DBAssets.NextRow())
            {
                string balance;
                CDispatchManager::g_DBAssets.Value(0, balance);
                string value = CDispatchManager::addLongDouble(balance, quantityNum);
                CDispatchManager::g_DBAssets.Sql("UPDATE user_assets_",false,true);
                CDispatchManager::g_DBAssets.Sql(coinId,true,false);
                string sqlSetTemp = " SET  " + balanceType + "  = \"";
                CDispatchManager::g_DBAssets.Sql(sqlSetTemp,false,false);
                CDispatchManager::g_DBAssets.Sql(value,true,false);
                CDispatchManager::g_DBAssets.Sql("\" where status = 1 and user_id = \"",false,false);
                CDispatchManager::g_DBAssets.Sql(uid,true,false);
                CDispatchManager::g_DBAssets.Sql("\";",false,false);
                DEBUG_INFO(CDispatchManager::g_DBAssets.getSql());
                if (CDispatchManager::g_DBAssets.Exec() >= 0)
                {
                    respDto->status = 1;
                    respDto->data = "新增资产成功";
                }
                else
                {
                    respDto->status = 0;
                    respDto->data = "新增资产失败";
                }
            }
            else
            {
                respDto->status = 0;
                respDto->data = "新增资产失败";
            }
        }
        else
        {
            respDto->status = 0;
            respDto->data = "新增资产失败";
        }
    }

    respDto->m_client = m_client;
    return (CJsonObjectBase *)respDto;
}