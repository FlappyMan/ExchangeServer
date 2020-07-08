/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-28 23:28:02
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/Model/CancelTradeBatchModel.hpp
 */
#ifndef _CANCELTRADEBAT_MODEL_HEADER_
#define _CANCELTRADEBAT_MODEL_HEADER_

#include "../global.h"
#include "BaseModel.hpp"
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
    bool bIsSucc;   // 撤单是否成功
    uint64_t orderid;
    uint64_t marketid;
    string price;
    string num;
};

class CCancelTradeBatchModel : public CBaseModel
{
public:
    CCancelTradeBatchModel &operator=(const CCancelTradeBatchModel &obj)
    {
        if (this != &obj)
        {
            this->orders = obj.orders;
            this->uid = obj.uid;
            this->token = obj.token;
        }
        return *this;
    }
    CCancelTradeBatchModel &operator=(CCancelTradeBatchModel *Obj)
    {
        if (this != Obj)
        {
            this->orders = Obj->orders;
            this->uid = Obj->uid;
            this->token = Obj->token;
        }
        return *this;
    }
    CCancelTradeBatchModel &operator=(UPCanceltradebatch *pkg)
    {
        this->uid = dynamic_cast<UPCanceltradebatch *>(pkg)->uid();
        this->token = dynamic_cast<UPCanceltradebatch *>(pkg)->token();
        this->canceltradebatchid = dynamic_cast<UPCanceltradebatch *>(pkg)->canceltradebatchid();
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
    
    void recordToDB(UBMysql& dbMysql,int iNowTime)
    {
        string sqlBase = "DELETE FROM trading WHERE order_id in(";
        dbMysql.Sql(sqlBase, false, true);
        for (int i = 0; i < orders.size() - 1; i++)
        {
            string oid = std::to_string(orders[i].orderid);
            dbMysql.Sql(oid, false, false);
            dbMysql.Sql(",", false, false);
        }
        string oid = std::to_string(orders[orders.size() - 1].orderid);
        dbMysql.Sql(oid, false, false);
        dbMysql.Sql(");", false, false);
        DEBUG_INFO(dbMysql.getSql());
        dbMysql.Exec();
    }

public:
    uint64_t uid;
    uint64_t canceltradebatchid;
    vector<CCancelTradeInfoModel> orders;
};
#endif
