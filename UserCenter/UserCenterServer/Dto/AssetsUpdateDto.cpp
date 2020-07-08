/*
 * @Author: your name
 * @Date: 2020-06-11 02:05:54
 * @LastEditTime: 2020-06-16 20:21:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Dto/AssetsUpdateDto.cpp
 */
#include "AssetsUpdateDto.h"
#include "../Model/MsgRespDto.hpp"
#include "DispatchManager.h"
CAssetsUpdateDto::CAssetsUpdateDto()
{
    CDispatchManager::g_DBAssets.init(SIZE_BUFFER_2k);
}

CJsonObjectBase *CAssetsUpdateDto::requestOperation()
{
#ifdef PRINT_LOG
    DEBUG_INFO("[CAssetsUpdateDto::requestOperation]");
#endif
    int iStatus = 1;
    string retMsg = "操作成功";
    if (note.length() > 64)
    {
        iStatus = 0;
        retMsg = "备注过长";
    }
    else if (m_vecAssets.size() == 0)
    {
        iStatus = 0;
        retMsg = "参数错误";
    }
    else
    {
        CAssetsUpdate assetsModel;
        assetsModel.businessOper(m_vecAssets, iStatus, retMsg, note);
    }

    CMsgRespDto *respDto = new CMsgRespDto;
    respDto->status = iStatus;
    respDto->data = retMsg;
    respDto->m_client = m_client;
    return (CJsonObjectBase *)respDto;
}