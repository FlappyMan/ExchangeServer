/*
 * @Author: your name
 * @Date: 2020-06-11 02:05:54
 * @LastEditTime: 2020-06-16 01:17:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Dto/AssetsUpdateDto.h
 */ 
#ifndef _ASSETSUPDATE_DTO_HEADER_
#define _ASSETSUPDATE_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"
#include "AssetsUpdate.hpp"

class CAssetsUpdateDto : public CJsonObjectBase
{
public:
    CAssetsUpdateDto();
    virtual ~CAssetsUpdateDto(){};
    CJsonObjectBase * requestOperation();
    void respDtoSerialize(){};
    
public:
    vector<COrderInfo*> m_vecAssets;
    string note;
};

#endif