/*
 * @Author: your name
 * @Date: 2020-05-07 03:04:58
 * @LastEditTime: 2020-05-12 20:31:13
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Model/CancelTradeModel.hpp
 */
#ifndef _CANCELTRADE_MODEL_HEADER_
#define _CANCELTRADE_MODEL_HEADER_

#include "../global.h"
class CCancelTradeModel
{
public:
    CCancelTradeModel &operator=(const CCancelTradeModel &obj)
    {
        if (this != &obj)
        {
            this->marketid = obj.marketid;
            this->orderid = obj.orderid;
            this->uid = obj.uid;
            this->token = obj.token;
            this->canceltradeid = obj.canceltradeid;
            this->type = obj.type;
            this->price = obj.price;
            this->num = obj.num;
        }
        return *this;
    }
    CCancelTradeModel &operator=(UPCanceltrade *pkg)
    {
        this->marketid = dynamic_cast<UPCanceltrade *>(pkg)->marketid();
        this->token = dynamic_cast<UPCanceltrade *>(pkg)->token();
        this->uid = dynamic_cast<UPCanceltrade *>(pkg)->uid();
        this->orderid = dynamic_cast<UPCanceltrade *>(pkg)->orderid();
        this->canceltradeid = dynamic_cast<UPCanceltrade *>(pkg)->canceltradeid();
        this->type = dynamic_cast<UPCanceltrade *>(pkg)->type();
        this->price = dynamic_cast<UPCanceltrade *>(pkg)->price();
        this->num = dynamic_cast<UPCanceltrade *>(pkg)->number();
        return *this;
    }

public:
    uint64_t marketid;
    string token;
    string uid;
    int type;
    uint64_t orderid;
    uint64_t canceltradeid;
    string price;
    string num;
};


#endif
