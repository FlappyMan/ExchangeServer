/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-11 19:04:12
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Dto/LoginDto.h
 */ 
#ifndef _LOGIN_DTO_HEADER_
#define _LOGIN_DTO_HEADER_

#include "../global.h"
#include "UPJsonReflex.hpp"


class CLoginDto : public CJsonObjectBase
{
public:
    CLoginDto();
    virtual ~CLoginDto(){};
    CJsonObjectBase * requestOperation();
    void respDtoSerialize(){};
public:
    string userAccount;
    string password;
    int sourceType;
};

#endif