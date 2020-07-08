#ifndef _TRADE_MODEL_HEADER_
#define _TRADE_MODEL_HEADER_

#include "../global.h"
#include "TradeBatchModel.hpp"
#include "BaseModel.hpp"

class CTradeModel : public CBaseModel
{
public:
    CTradeModel &operator=(const CTradeModel &obj)
    {
        if (this != &obj)
        {
            this->uid = obj.uid;
            this->token = obj.token;
            this->paypassword = obj.paypassword;
            this->marketid = obj.marketid;
            this->price = obj.price;
            this->tradedNum = obj.tradedNum;
            this->num = obj.num;
            this->type = obj.type;
            this->totalNum = obj.totalNum;
            this->orderid = obj.orderid;
        }
        return *this;
    }
    CTradeModel &operator=(CTradeModel *pObj)
    {
        if (this != pObj)
        {
            this->orderid = pObj->orderid;
            this->num = pObj->num;
            this->uid = pObj->uid;
            this->paypassword = pObj->paypassword;
            this->marketid = pObj->marketid;
            this->price = pObj->price;
            this->totalNum = pObj->totalNum;
            this->type = pObj->type;
            this->tradedNum = pObj->tradedNum;
            this->time = pObj->time;
            this->sysUid = pObj->sysUid;
            this->invite_uid_1 = pObj->invite_uid_1;
            this->invite_uid_2 = pObj->invite_uid_2;
            this->invite_uid_3 = pObj->invite_uid_3;
            this->discount = pObj->discount;
        }
        return *this;
    }
    CTradeModel &operator=(const CTradeInfoModel &obj)
    {
        this->time = obj.time;
        this->marketid = obj.marketid;
        this->price = obj.price;
        this->num = obj.num;
        this->type = obj.type;
        this->orderid = obj.orderid;
        return *this;
    }
    CTradeModel &operator=(UPUptrade *pkg)
    {
        this->uid = dynamic_cast<UPUptrade *>(pkg)->uid();
        this->token = dynamic_cast<UPUptrade *>(pkg)->token();
        this->paypassword = dynamic_cast<UPUptrade *>(pkg)->paypassword();
        this->marketid = dynamic_cast<UPUptrade *>(pkg)->marketid();
        this->price = dynamic_cast<UPUptrade *>(pkg)->price();
        this->tradedNum = dynamic_cast<UPUptrade *>(pkg)->num();
        this->totalNum = dynamic_cast<UPUptrade *>(pkg)->num();
        this->num = dynamic_cast<UPUptrade *>(pkg)->num();
        this->type = dynamic_cast<UPUptrade *>(pkg)->type();
        this->orderid = dynamic_cast<UPUptrade *>(pkg)->orderid();
        return *this;
    }
    void recordToDB(UBMysql& dbMysql,int iNowTime)
    {
        string sqlBase = "INSERT INTO trading (order_id,user_id,invite_uid_1,invite_uid_2,invite_uid_3,oper_type,price,deal_time,trading_volume,traded_volume,create_time,update_time) VALUES (\"";
        string oid = std::to_string(orderid);
        dbMysql.Sql(sqlBase, false, true);
        dbMysql.Sql(oid, false, false);
        dbMysql.Sql("\",\"", false, false);
        string sUid = std::to_string(uid);
        dbMysql.Sql(sUid, false, false);
        dbMysql.Sql("\",\"", false, false);
        string sIUid = std::to_string(invite_uid_1);
        dbMysql.Sql(sIUid, true, false);
        dbMysql.Sql("\",\"", false, false);
        sIUid = std::to_string(invite_uid_2);
        dbMysql.Sql(sIUid, true, false);
        dbMysql.Sql("\",\"", false, false);
        sIUid = std::to_string(invite_uid_3);
        dbMysql.Sql(sIUid, true, false);
        dbMysql.Sql("\",\"", false, false);
        string ot = std::to_string(type);
        dbMysql.Sql(ot, true, false);
        dbMysql.Sql("\",\"", false, false);
        dbMysql.Sql(price, true, false);
        dbMysql.Sql("\",\"", false, false);
        string strTime = std::to_string(iNowTime);
        dbMysql.Sql(strTime, true, false);
        dbMysql.Sql("\",\"", false, false);
        dbMysql.Sql(num, true, false);
        dbMysql.Sql("\",\"", false, false);
        dbMysql.Sql(tradedNum, true, false);
        dbMysql.Sql("\",\"", false, false);
        dbMysql.Sql(strTime, true, false);
        dbMysql.Sql(",", false, false);
        dbMysql.Sql(strTime, true, false);
        dbMysql.Sql(");", false, false);
        DEBUG_INFO(dbMysql.getSql());
        dbMysql.Exec();
    }
public:
    /*撮合系统使用*/
    uint64_t orderid;   // fronttrade创建
    string num;  // 本次交易数量
    
    /*接口协议内容*/
    uint64_t uid;
    string paypassword;
    uint64_t marketid;
    string price;
    string totalNum;    //总交易数量
    int type;   // 1 买入,2 卖出

    /*用户中心返回*/
    uint64_t invite_uid_1;
    uint64_t invite_uid_2;
    uint64_t invite_uid_3;
    uint64_t sysUid;
    string   discount;
    int time;   // 成交时间
    string tradedNum;   //已交易

};

#endif
