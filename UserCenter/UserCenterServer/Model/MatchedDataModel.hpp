/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-05-19 22:29:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Model/MatchedDataModel.hpp
 */ 
#ifndef _MATCHDATA_MODEL_HEADER_
#define _MATCHDATA_MODEL_HEADER_

#include "../global.h"

class CSellUserModel
{
public:
    CSellUserModel &operator=(const CSellUserModel &obj)
    {
        if (this != &obj)
        {
            this->userid = obj.userid;
            this->orderid = obj.orderid;
        }
        return *this;
    }
    CSellUserModel &operator=(UPMatchedData::SellUser& pkg)
    {
        this->userid = pkg.userid();
        this->orderid = pkg.orderid();
        return *this;
    }

public:
    string userid;
    uint64_t orderid;
};



class CBuyUserModel
{
public:
    CBuyUserModel &operator=(const CBuyUserModel &obj)
    {
        if (this != &obj)
        {
            this->userid = obj.userid;
            this->orderid = obj.orderid;
            this->buyPrice = obj.buyPrice;
        }
        return *this;
    }
    CBuyUserModel &operator=(UPMatchedData::BuyUser& pkg)
    {
        this->userid = pkg.userid();
        this->orderid = pkg.orderid();
        this->buyPrice = pkg.buyprice();
        return *this;
    }

public:
    string userid;
    uint64_t orderid;
    string buyPrice;
};

class CMatchedDataModel
{
public:
    CMatchedDataModel &operator=(const CMatchedDataModel &obj)
    {
        if (this != &obj)
        {
            this->number = obj.number;
            this->price = obj.price;
            this->token = obj.token;
            this->buyuser = obj.buyuser;
            this->selluser = obj.selluser;
        }
        return *this;
    }
    CMatchedDataModel &operator=(UPMatchedData *pkg)
    {
        this->number = dynamic_cast<UPMatchedData *>(pkg)->number();
        this->price = dynamic_cast<UPMatchedData *>(pkg)->price();
        this->token = dynamic_cast<UPMatchedData *>(pkg)->token();
        UPMatchedData::BuyUser buy = dynamic_cast<UPMatchedData *>(pkg)->buyuser();
        this->buyuser = buy;
        UPMatchedData::SellUser sell = dynamic_cast<UPMatchedData *>(pkg)->selluser();
        this->selluser = sell;
        return *this;
    }

public:
    string price;   // 商品价格 如btc
    string number;  // 商品数量 btc数量
    CBuyUserModel buyuser;
    CSellUserModel selluser;
    string token;
};

#endif
