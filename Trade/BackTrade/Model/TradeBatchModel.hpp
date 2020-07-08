/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-06-29 00:56:07
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/Model/TradeBatchModel.hpp
 */
#ifndef _TRADEBATCH_MODEL_HEADER_
#define _TRADEBATCH_MODEL_HEADER_

#include "../global.h"
#include "BaseModel.hpp"
class CTradeInfoModel
{
public:
    CTradeInfoModel &operator=(const CTradeInfoModel &obj)
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
    CTradeInfoModel &operator=(UPUptradebatch::Trades& pkg)
    {
        this->price = pkg.price();
        this->marketid = pkg.marketid();
        this->num = pkg.num();
        this->type = pkg.type();
        this->orderid = pkg.orderid();
        return *this;
    }

public:
    uint64_t orderid;
    uint64_t marketid;
    string price;
    string num;
    int time;
    int type;
};

class CTradeBatchModel : public CBaseModel
{
public:
    CTradeBatchModel &operator=(const CTradeBatchModel &obj)
    {
        if (this != &obj)
        {
            this->uid = obj.uid;
            this->token = obj.token;
            this->paypassword = obj.paypassword;
            this->trades = obj.trades;
        }

        return *this;
    }
    CTradeBatchModel &operator=(UPUptradebatch *pkg)
    {
        this->uid = dynamic_cast<UPUptradebatch *>(pkg)->uid();
        this->token = dynamic_cast<UPUptradebatch *>(pkg)->token();
        this->paypassword = dynamic_cast<UPUptradebatch *>(pkg)->paypassword();
        int iSize = dynamic_cast<UPUptradebatch *>(pkg)->trades_size();
        for (int iLoop = 0; iLoop < iSize; iLoop++)
        {
            CTradeInfoModel model;
            UPUptradebatch::Trades trade = dynamic_cast<UPUptradebatch *>(pkg)->trades(iLoop);
            model = trade;
            this->trades.push_back(model);
        }
        return *this;
    }

    void recordToDB(UBMysql& dbMysql,int iNowTime)
    {
        string sqlBase = "INSERT INTO trading (order_id,user_id,invite_uid_1,invite_uid_2,invite_uid_3,oper_type,price,deal_time,trading_volume,traded_volume,create_time,update_time) VALUES ";
        dbMysql.Sql(sqlBase, false, true);
        for (int i = 0; i < trades.size() - 1; i++)
        {
            CTradeInfoModel model = trades[i];
            string oid = std::to_string(model.orderid);
            dbMysql.Sql("(\"", false, false);
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
            string ot = std::to_string(model.type);
            dbMysql.Sql(ot, true, false);
            dbMysql.Sql("\",\"", false, false);
            dbMysql.Sql(model.price, true, false);
            dbMysql.Sql("\",\"", false, false);
            string strTime = std::to_string(iNowTime);
            dbMysql.Sql(strTime, true, false);
            dbMysql.Sql("\",\"", false, false);
            dbMysql.Sql(model.num, true, false);
            dbMysql.Sql("\",\"", false, false);
            dbMysql.Sql("0.0", true, false);    // 已交易数量
            dbMysql.Sql("\",\"", false, false);
            dbMysql.Sql(strTime, true, false);
            dbMysql.Sql(",", false, false);
            dbMysql.Sql(strTime, true, false);
            dbMysql.Sql("),", false, false);
        }
        CTradeInfoModel model = trades[trades.size() - 1];
        string oid = std::to_string(model.orderid);
        dbMysql.Sql("(\"", false, false);
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
        string ot = std::to_string(model.type);
        dbMysql.Sql(ot, true, false);
        dbMysql.Sql("\",\"", false, false);
        dbMysql.Sql(model.price, true, false);
        dbMysql.Sql("\",\"", false, false);
        string strTime = std::to_string(iNowTime);
        dbMysql.Sql(strTime, true, false);
        dbMysql.Sql("\",\"", false, false);
        dbMysql.Sql(model.num, true, false);
        dbMysql.Sql("\",\"", false, false);
        dbMysql.Sql("0.0", true, false); // 已交易数量
        dbMysql.Sql("\",\"", false, false);
        dbMysql.Sql(strTime, true, false);
        dbMysql.Sql(",", false, false);
        dbMysql.Sql(strTime, true, false);
        dbMysql.Sql(");", false, false);
        DEBUG_INFO(dbMysql.getSql());
        dbMysql.Exec();
    }

public:
    uint64_t uid;
    string paypassword;
    vector<CTradeInfoModel> trades;

    /*用户中心返回*/
    uint64_t invite_uid_1;
    uint64_t invite_uid_2;
    uint64_t invite_uid_3;
    uint64_t sysUid;
    string discount;
};

#endif
