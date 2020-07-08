/*
 * @Author: your name
 * @Date: 2020-05-07 03:04:58
 * @LastEditTime: 2020-05-26 00:22:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Model/CancelTradeBatchModel.hpp
 */
#ifndef _CANCELTRADEBAT_MODEL_HEADER_
#define _CANCELTRADEBAT_MODEL_HEADER_

#include "../global.h"

class CCancelTradeInfoModel
{
public:
    CCancelTradeInfoModel &operator=(const CCancelTradeInfoModel &obj)
    {
        if (this != &obj)
        {
            this->marketid = obj.marketid;
            this->price = obj.price;
            this->num = obj.num;
            this->type = obj.type;
            this->orderid = obj.orderid;
        }

        return *this;
    }
    CCancelTradeInfoModel &operator=(UPCanceltradebatch::Orders& pkg)
    {
        this->orderid = pkg.orderid();
        this->marketid = pkg.marketid();
        this->price = pkg.price();
        this->num = pkg.number();
        this->type = pkg.type();
        return *this;
    }

public:
    int type;
    uint64_t orderid;
    uint64_t marketid;
    string price;
    string num;
};


class CCancelTradeBatchModel
{
public:
    CCancelTradeBatchModel &operator=(const CCancelTradeBatchModel &obj)
    {
        if (this != &obj)
        {
            this->orders = obj.orders;
            this->token = obj.token;
            this->uid = obj.uid;
        }
        return *this;
    }
    CCancelTradeBatchModel &operator=(UPCanceltradebatch *pkg)
    {
        this->uid = dynamic_cast<UPCanceltradebatch *>(pkg)->uid();
        this->token = dynamic_cast<UPCanceltradebatch *>(pkg)->token();
        int iSize = dynamic_cast<UPCanceltradebatch *>(pkg)->orders_size();
        for (int iLoop = 0; iLoop < iSize; iLoop++)
        {
            CCancelTradeInfoModel model;
            UPCanceltradebatch::Orders orders = dynamic_cast<UPCanceltradebatch *>(pkg)->orders(iLoop);
            model = orders;
            this->orders.push_back(model);
        }

        return *this;
    }

public:
    string uid;
    string token;
    vector<CCancelTradeInfoModel> orders;
};

#endif
