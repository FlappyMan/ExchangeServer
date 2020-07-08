#include "HttpSession.h"
#include "DispatchManager.h"
#include "LoginDto.h"
#include "RegistDto.h"
#include "AddBalanceDto.h"
#include "UpdateDto.h"
#include "RemoveBindDeviceDto.h"
#include "ReduceBalanceDto.h"
#include "RealInfoDto.h"
#include "PayDto.h"
#include "InfoDto.h"
#include "CheckBindDeviceDto.h"
#include "AuthDto.h"
#include "AllocationBalanceDto.h"
#include "AddBindDeviceDto.h"
#include "AddBalanceDto.h"
#include "SetMasterDto.h"
#include "CoinAddDto.h"
#include "AssetsUpdateDto.h"

#define ASSETS_UPDATE "/Api/Balance/Change"
#define PASSWORD_UPDATE "/Api/Password/Update"
#define PASSWORD_PAY "/Api/Password/Pay"
#define DEVICE_ADDBINDDEVICE "/Api/Device/AddBindDevice"
#define DEVICE_REMOVEBIND "/Api/Device/RemoveBind"
#define DEVICE_CHECKBIND "/Api/Device/CheckBind"
#define USER_AUTH "/Api/User/Auth"
#define USER_INFO "/Api/User/Info"
#define USER_REALINFO "/Api/User/RealInfo"
#define BALANCE_ADD "/Api/Balance/Add"
#define BALANCE_REDUCE "/Api/Balance/Reduce"
#define BALANCE_ALLOCATION "/Api/Balance/Allocation"
// #define BALANCE_TRANS "/Api/Balance/Trans"
// #define BALANCE_FRONZENTRANS "/Api/Balance/FrozenTrans"
#define DEVICE_SETMASTER "/Api/Device/SetMaster"
#define USERCENTER_REGIST "/Api/UserCenter/Regist"
#define USERCENTER_LOGIN "/Api/UserCenter/Login"
#define COIN_ADD "/Api/Coin/Add"
CHttpSession::CHttpSession()
{
    m_httpParser.Init(HTTP_REQUEST);
    m_bRecvFinish = false;
    m_pObject = NULL;
}
CHttpSession::~CHttpSession()
{
    if (NULL != m_pObject)
    {
        delete m_pObject;
        m_pObject = NULL;
    }
    m_httpParser.Destroy();
}

int CHttpSession::OnRecv(client_sokt_t *client, char *pBuffer, int iDataLen)
{
#ifdef PRINT_LOG
    DEBUG_INFO("[CHttpSession OnRecv]");
#endif
    // 类成员变量

    int ret = m_httpParser.Readed(pBuffer, iDataLen);
    if (ret == RET_SUCCESS)
    {
#ifdef PRINT_LOG
        DEBUG_INFO("[CHttpSession OnRecv HTTP API]" + m_httpParser.m_sUrl);
        DEBUG_INFO("[CHttpSession OnRecv Body] = " + m_httpParser.m_sBody);
#endif
        m_bRecvFinish = true;

        map<string, string>::iterator it = m_httpParser.m_mFiled.find("token");
        if (it != m_httpParser.m_mFiled.end())
        {
            time_t nowTime = time(NULL);
            string token = it->second.substr(1, 16);
            string strDeadLine = token.substr(0, 8);

            int deadLine = CDispatchManager::HexStringToInt(strDeadLine);
            if ((int64_t)nowTime > deadLine)
            {
                CDispatchManager::DispatchHttpResponse("token out of time", (uv_tcp_t *)client, false);
                return RET_SUCCESS;
            }
            else
            {
                string md5Dest = it->second.substr(17, 32);
                string userId = token.substr(8, 16);
                string md5Value = strDeadLine + userId + TOKEN_MD5_KEY;
                if (CDispatchManager::md5(md5Value) != md5Dest)
                {
                    // 未知接口
                    CDispatchManager::DispatchHttpResponse("token error", (uv_tcp_t *)client, false);
                    return RET_SUCCESS;
                }
            }

            
            if (m_httpParser.m_sUrl == PASSWORD_UPDATE)
            {
                praseUpdate(m_httpParser.m_sBody);
            }
            else if (m_httpParser.m_sUrl == PASSWORD_PAY)
            {
                prasePay(m_httpParser.m_sBody);
            }
            else if (m_httpParser.m_sUrl == DEVICE_ADDBINDDEVICE)
            {
                praseAddBindDevice(m_httpParser.m_sBody,it->second,deadLine);
            }
            else if (m_httpParser.m_sUrl == DEVICE_REMOVEBIND)
            {
                praseRemoveBindDevice(m_httpParser.m_sBody);
            }
            else if (m_httpParser.m_sUrl == DEVICE_CHECKBIND)
            {
                praseCheckBindDevice(m_httpParser.m_sBody);
            }
            else if (m_httpParser.m_sUrl == USER_AUTH)
            {
                praseAuth(m_httpParser.m_sBody);
            }
            else if (m_httpParser.m_sUrl == USER_INFO)
            {
                praseInfo(m_httpParser.m_sBody);
            }
            else if (m_httpParser.m_sUrl == USER_REALINFO)
            {
                praseRealInfo(m_httpParser.m_sBody);
            }
            else if (m_httpParser.m_sUrl == BALANCE_ADD)
            {
                praseAddBalance(m_httpParser.m_sBody);
            }
            else if (m_httpParser.m_sUrl == BALANCE_REDUCE)
            {
                praseReduceBalance(m_httpParser.m_sBody);
            }
            else if (m_httpParser.m_sUrl == BALANCE_ALLOCATION)
            {
                praseAllocationBalance(m_httpParser.m_sBody);
            }
            else if (m_httpParser.m_sUrl == DEVICE_SETMASTER)
            {
                praseSetMaster(m_httpParser.m_sBody);
            }
            else if (m_httpParser.m_sUrl == COIN_ADD)
            {
                praseCoinAdd(m_httpParser.m_sBody);
            }
            else
            {
                // 未知接口
                CDispatchManager::DispatchHttpResponse("", (uv_tcp_t *)client, false);
                return RET_SUCCESS;
            }
            m_pObject->m_client = client;
            return RET_SUCCESS;
        }
        else
        {
            if (m_httpParser.m_sUrl == USERCENTER_REGIST)
            {
                praseRegist(m_httpParser.m_sBody);
            }
            else if (m_httpParser.m_sUrl == USERCENTER_LOGIN)
            {
                praseLogin(m_httpParser.m_sBody);
            }
            else if (m_httpParser.m_sUrl == ASSETS_UPDATE)
            {
                praseAssetsUpdate(m_httpParser.m_sBody);
            }
            else
            {
                // 未知接口
                CDispatchManager::DispatchHttpResponse("", (uv_tcp_t *)client, false);
                return RET_SUCCESS;
            }
            m_pObject->m_client = client;
            return RET_SUCCESS;
        }
    }
    else if (ret > 0)
    {
        DEBUG_INFO("[CHttpSession RET_HOLD]");
        pBuffer[iDataLen] = 0;
        return RET_HOLD;
    }
    else
    {
        DEBUG_INFO("[CHttpSession RET_FAILED]");
        CDispatchManager::DispatchHttpResponse("", (uv_tcp_t *)client, false);
        return RET_SUCCESS;
    }
}

void CHttpSession::praseLogin(string sHttpBody)
{
    Json::Value root;
    CLoginDto *pObj = new CLoginDto;
    if (UBJsonUnpack(root, sHttpBody) != false)
    {
        pObj->userAccount = root["userAccount"].asString();
        pObj->password = root["password"].asString();
        pObj->sourceType = root["sourceType"].asUInt64();
    }
    m_pObject = (CJsonObjectBase *)pObj;
}
void CHttpSession::praseRegist(string sHttpBody)
{
    Json::Value root;
    CRegistDto *pObj = new CRegistDto;
    if (UBJsonUnpack(root, sHttpBody) != false)
    {
        pObj->userAccount = root["userAccount"].asString();
        pObj->password = root["password"].asString();
        pObj->deviceID = root["deviceID"].asString();
        pObj->sourceType = root["sourceType"].asUInt64();
    }
    m_pObject = (CJsonObjectBase *)pObj;
}

void CHttpSession::praseAssetsUpdate(string sHttpBody)
{
    Json::Value root;
    CAssetsUpdateDto *pObj = new CAssetsUpdateDto;
    if (UBJsonUnpack(root, sHttpBody) != false)
    {
        Json::Value &list = root["list"];
        pObj->note = root["note"].asString();
        int iListSize = list.size();
        for (int i = 0; i < iListSize; i++)
        {
            Json::Value& obj = list[i];
            COrderInfo* dto = new COrderInfo;
            dto->uid = obj["uid"].asUInt64();
            dto->type_id = obj["typeID"].asUInt();
            dto->type = obj["type"].asUInt();
            dto->coinid = obj["coinid"].asUInt();
            dto->usable = obj["usable"].asString();
            dto->fronzen = obj["fronzen"].asString();
            dto->lockup = obj["lockup"].asString();
            if (dto->usable.length() == 0)
            {
                dto->usable = "0.0";
            }
            if (dto->fronzen.length() == 0)
            {
                dto->fronzen = "0.0";
            }
            if (dto->lockup.length() == 0)
            {
                dto->lockup = "0.0";
            }
            pObj->m_vecAssets.push_back(dto);
        }
    }
    
    m_pObject = (CJsonObjectBase *)pObj;
}
void CHttpSession::praseUpdate(string sHttpBody)
{
    Json::Value root;
    CUpdateDto *pObj = new CUpdateDto;
    if (UBJsonUnpack(root, sHttpBody) != false)
    {
        pObj->userAccount = root["userAccount"].asString();
        pObj->password = root["password"].asString();
    }
    m_pObject = (CJsonObjectBase *)pObj;
}
void CHttpSession::prasePay(string sHttpBody)
{
    Json::Value root;
    CPayDto *pObj = new CPayDto;
    if (UBJsonUnpack(root, sHttpBody) != false)
    {
        pObj->uid = root["uid"].asString();
        pObj->paypassword = root["paypassword"].asString();
        pObj->method = root["method"].asUInt64();
    }
    m_pObject = (CJsonObjectBase *)pObj;
}
void CHttpSession::praseAddBindDevice(string sHttpBody,string token,int deadLine)
{
    Json::Value root;
    CAddBindDeviceDto *pObj = new CAddBindDeviceDto;
    if (UBJsonUnpack(root, sHttpBody) != false)
    {
        pObj->uid = root["uid"].asString();
        pObj->deviceID = root["deviceID"].asString();
        pObj->deviceName = root["deviceName"].asString();
        pObj->deviceOS = root["deviceOS"].asString();
        pObj->sourceType = root["sourceType"].asUInt64();
        pObj->token = token;
        pObj->tokenTime = deadLine;
    }
    m_pObject = (CJsonObjectBase *)pObj;
}
void CHttpSession::praseRemoveBindDevice(string sHttpBody)
{
    Json::Value root;
    CRemoveBindDeviceDto *pObj = new CRemoveBindDeviceDto;
    if (UBJsonUnpack(root, sHttpBody) != false)
    {
        pObj->uid = root["uid"].asString();
        pObj->deviceID = root["deviceID"].asString();
    }
    m_pObject = (CJsonObjectBase *)pObj;
}
void CHttpSession::praseCheckBindDevice(string sHttpBody)
{
    Json::Value root;
    CCheckBindDeviceDto *pObj = new CCheckBindDeviceDto;
    if (UBJsonUnpack(root, sHttpBody) != false)
    {
        pObj->uid = root["uid"].asString();
        pObj->deviceID = root["deviceID"].asString();
    }

    m_pObject = (CJsonObjectBase *)pObj;
}
void CHttpSession::praseAuth(string sHttpBody)
{
    Json::Value root;
    CAuthDto *pObj = new CAuthDto;
    if (UBJsonUnpack(root, sHttpBody) != false)
    {
        Json::Value &column = root["column"];
        Json::Value &value = root["value"];
        pObj->uid = root["uid"].asString();
        int iColumnSize = column.size();
        int iValueSize = value.size();

        for (int i = 0; i < iColumnSize; i++)
        {
            pObj->column.push_back(column[i].asString());
        }
        for (int i = 0; i < iValueSize; i++)
        {
            pObj->value.push_back(value[i].asString());
        }
        pObj->method = root["method"].asUInt64();
    }

    m_pObject = (CJsonObjectBase *)pObj;
}
void CHttpSession::praseInfo(string sHttpBody)
{
    Json::Value root;
    CInfoDto *pObj = new CInfoDto;
    if (UBJsonUnpack(root, sHttpBody) != false)
    {
        Json::Value &column = root["column"];
        Json::Value &value = root["value"];
        pObj->uid = root["uid"].asString();
        int iColumnSize = column.size();
        int iValueSize = value.size();

        for (int i = 0; i < iColumnSize; i++)
        {
            pObj->column.push_back(column[i].asString());
        }
        for (int i = 0; i < iValueSize; i++)
        {
            pObj->value.push_back(value[i].asString());
        }

        pObj->method = root["method"].asUInt64();
    }

    m_pObject = (CJsonObjectBase *)pObj;
}
void CHttpSession::praseRealInfo(string sHttpBody)
{
    Json::Value root;
    CRealInfoDto *pObj = new CRealInfoDto;
    if (UBJsonUnpack(root, sHttpBody) != false)
    {
        Json::Value &column = root["column"];
        Json::Value &value = root["value"];
        pObj->uid = root["uid"].asString();
        int iColumnSize = column.size();
        int iValueSize = value.size();

        for (int i = 0; i < iColumnSize; i++)
        {
            pObj->column.push_back(column[i].asString());
        }
        for (int i = 0; i < iValueSize; i++)
        {
            pObj->value.push_back(value[i].asString());
        }

        pObj->method = root["method"].asUInt64();
    }

    m_pObject = (CJsonObjectBase *)pObj;
}
void CHttpSession::praseAddBalance(string sHttpBody)
{
    Json::Value root;
    CAddBalanceDto *pObj = new CAddBalanceDto;
    if (UBJsonUnpack(root, sHttpBody) != false)
    {
        pObj->uid = root["uid"].asString();
        pObj->type = root["type"].asUInt64();
        pObj->coinId = root["coinId"].asString();
        pObj->quantityNum = root["quantityNum"].asString();
    }

    m_pObject = (CJsonObjectBase *)pObj;
}
void CHttpSession::praseReduceBalance(string sHttpBody)
{
    Json::Value root;
    CReduceBalanceDto *pObj = new CReduceBalanceDto;
    if (UBJsonUnpack(root, sHttpBody) != false)
    {
        pObj->uid = root["uid"].asString();
        pObj->type = root["type"].asUInt64();
        pObj->coinId = root["coinId"].asString();
        pObj->quantityNum = root["quantityNum"].asString();
    }

    m_pObject = (CJsonObjectBase *)pObj;
}
void CHttpSession::praseAllocationBalance(string sHttpBody)
{
    Json::Value root;
    CAllocationBalanceDto *pObj = new CAllocationBalanceDto;
    if (UBJsonUnpack(root, sHttpBody) != false)
    {
        pObj->uid = root["uid"].asString();
        pObj->coinId = root["coinId"].asString();
        pObj->assetsNum = root["assetsNum"].asString();
        pObj->method = root["method"].asUInt64();
    }

    m_pObject = (CJsonObjectBase *)pObj;
}
void CHttpSession::praseSetMaster(string sHttpBody)
{
    Json::Value root;
    CSetMasterDto *pObj = new CSetMasterDto;
    if (UBJsonUnpack(root, sHttpBody) != false)
    {
        pObj->deviceID = root["deviceID"].asString();
        pObj->status = root["status"].asUInt64();
        pObj->uid = root["uid"].asString();
    }

    m_pObject = (CJsonObjectBase *)pObj;
}

void CHttpSession::praseCoinAdd(string sHttpBody)
{
    Json::Value root;
    CCoinAddDto *pObj = new CCoinAddDto;
    if (UBJsonUnpack(root, sHttpBody) != false)
    {
        pObj->name = root["name"].asString();
        pObj->type = root["type"].asString();
        pObj->title = root["title"].asString();
    }
    m_pObject = (CJsonObjectBase *)pObj;
}