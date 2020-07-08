/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-24 01:43:26
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/Model/CancelTradeModel.hpp
 */
#ifndef _CANCELTRADE_MODEL_HEADER_
#define _CANCELTRADE_MODEL_HEADER_

#include "../global.h"
#include "BaseModel.hpp"
#include "CancelTradeBatchModel.hpp"
class CCancelTradeModel  : public CBaseModel
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
    CCancelTradeModel &operator=(CCancelTradeModel *Obj)
    {
        if (this != Obj)
        {
            this->marketid = Obj->marketid;
            this->orderid = Obj->orderid;
            this->uid = Obj->uid;
            this->token = Obj->token;
            this->canceltradeid = Obj->canceltradeid;
            this->type = Obj->type;
            this->price = Obj->price;
            this->num = Obj->num;
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
    CCancelTradeModel &operator=(const CCancelTradeInfoModel &obj)
    {
        this->marketid = obj.marketid;
        this->orderid = obj.orderid;
        this->canceltradeid = obj.orderid;
        this->price = obj.price;
        this->num = obj.num;
        this->type = obj.type;
        return *this;
    }

    void recordToDB(UBMysql& dbMysql,int iNowTime)
    {
        string sqlBase = "DELETE FROM trading WHERE order_id = \"";
        string oid = std::to_string(canceltradeid);
        dbMysql.Sql(sqlBase, false, true);
        dbMysql.Sql(oid, false, false);
        dbMysql.Sql("\");", false, false);
        DEBUG_INFO(dbMysql.getSql());
        dbMysql.Exec();
    }

public:
    uint64_t marketid;
    uint64_t uid;
    uint64_t orderid;
    uint64_t canceltradeid;
    string price;
    string num;
    int type;
};

#endif
