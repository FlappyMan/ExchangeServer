/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-07-02 02:52:11
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/Model/MatchedDataModel.hpp
 */ 
#ifndef _MATCHDATA_MODEL_HEADER_
#define _MATCHDATA_MODEL_HEADER_

#include "../global.h"
#include "BaseModel.hpp"
#include "../Manager/Tool.hpp"

class CTransUserModel
{
public:
    CTransUserModel &operator=(const CTransUserModel &obj)
    {
        if (this != &obj)
        {
            this->userid = obj.userid;
            this->orderid = obj.orderid;
            this->price = obj.price;
        }
        return *this;
    }
    CTransUserModel &operator=(UPMatchedData::BuyUser& pkg)
    {
        this->userid = pkg.userid();
        this->orderid = pkg.orderid();
        this->price = pkg.buyprice();
        return *this;
    }
    CTransUserModel &operator=(UPMatchedData::SellUser &pkg)
    {
        this->userid = pkg.userid();
        this->orderid = pkg.orderid();
        this->price = pkg.sellprice();
        return *this;
    }

public:
    uint64_t userid;
    uint64_t orderid; // 订单号
    string price; // 下单价格
    string sysFee;  // 平台手续费
    string discount; // vip折扣
    string tradedNum; // 已交易总数
    string tradeTotal; // 下单交易总数
    uint64_t invite_uid_1;
    uint64_t invite_uid_2;
    uint64_t invite_uid_3;
    string invite_fee_1;
    string invite_fee_2;
    string invite_fee_3;
};

class CMatchedDataModel : public CBaseModel
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
    
    void recordToDB(UBMysql& dbMysql,UBMysql& dbLog,int iNowTime)
    {
        if (buyuser.tradedNum == buyuser.tradeTotal)
        {
            string sqlBase = "DELETE FROM trading where order_id = " + std::to_string(buyuser.orderid) + ";";
            dbMysql.Sql(sqlBase,false, true);
            dbMysql.Exec();
        }
        else
        {
            string sqlBase = "INSERT INTO trading (order_id,user_id,invite_uid_1,invite_uid_2,invite_uid_3,oper_type,price,deal_time,trading_volume,traded_volume,create_time,update_time) VALUES (\"";
            string oid = std::to_string(buyuser.orderid);
            dbMysql.Sql(sqlBase, false, true);
            dbMysql.Sql(oid, false, false);
            dbMysql.Sql("\",\"", false, false);
            string sUid = std::to_string(buyuser.userid);
            dbMysql.Sql(sUid, false, false);
            dbMysql.Sql("\",\"", false, false);
            string sIUid = std::to_string(buyuser.invite_uid_1);
            dbMysql.Sql(sIUid, true, false);
            dbMysql.Sql("\",\"", false, false);
            sIUid = std::to_string(buyuser.invite_uid_2);
            dbMysql.Sql(sIUid, true, false);
            dbMysql.Sql("\",\"", false, false);
            sIUid = std::to_string(buyuser.invite_uid_3);
            dbMysql.Sql(sIUid, true, false);
            dbMysql.Sql("\",\"", false, false);
            string ot = std::to_string(1);
            dbMysql.Sql(ot, true, false);
            dbMysql.Sql("\",\"", false, false);
            dbMysql.Sql(price, true, false);
            dbMysql.Sql("\",\"", false, false);
            string strTime = std::to_string(iNowTime);
            dbMysql.Sql(strTime, true, false);
            dbMysql.Sql("\",\"", false, false);
            dbMysql.Sql(buyuser.tradeTotal, true, false);
            dbMysql.Sql("\",\"", false, false);
            dbMysql.Sql(buyuser.tradedNum, true, false);
            dbMysql.Sql("\",\"", false, false);
            dbMysql.Sql(strTime, true, false);
            dbMysql.Sql(",", false, false);
            dbMysql.Sql(strTime, true, false);
            dbMysql.Sql(");", false, false);
            DEBUG_INFO(dbMysql.getSql());
            dbMysql.Exec();
        }
        if (selluser.tradedNum == selluser.tradeTotal)
        {
            string sqlBase = "DELETE FROM trading where order_id = " + std::to_string(selluser.orderid) + ";";
            dbMysql.Sql(sqlBase,false, true);
            dbMysql.Exec();
        }
        else
        {
            string sqlBase = "INSERT INTO trading (order_id,user_id,invite_uid_1,invite_uid_2,invite_uid_3,oper_type,price,deal_time,trading_volume,traded_volume,create_time,update_time) VALUES (\"";
            string oid = std::to_string(selluser.orderid);
            dbMysql.Sql(sqlBase, false, true);
            dbMysql.Sql(oid, false, false);
            dbMysql.Sql("\",\"", false, false);
            string sUid = std::to_string(selluser.userid);
            dbMysql.Sql(sUid, false, false);
            dbMysql.Sql("\",\"", false, false);
            string sIUid = std::to_string(selluser.invite_uid_1);
            dbMysql.Sql(sIUid, true, false);
            dbMysql.Sql("\",\"", false, false);
            sIUid = std::to_string(selluser.invite_uid_2);
            dbMysql.Sql(sIUid, true, false);
            dbMysql.Sql("\",\"", false, false);
            sIUid = std::to_string(selluser.invite_uid_3);
            dbMysql.Sql(sIUid, true, false);
            dbMysql.Sql("\",\"", false, false);
            string ot = std::to_string(2);
            dbMysql.Sql(ot, true, false);
            dbMysql.Sql("\",\"", false, false);
            dbMysql.Sql(price, true, false);
            dbMysql.Sql("\",\"", false, false);
            string strTime = std::to_string(iNowTime);
            dbMysql.Sql(strTime, true, false);
            dbMysql.Sql("\",\"", false, false);
            dbMysql.Sql(buyuser.tradeTotal, true, false);
            dbMysql.Sql("\",\"", false, false);
            dbMysql.Sql(buyuser.tradedNum, true, false);
            dbMysql.Sql("\",\"", false, false);
            dbMysql.Sql(strTime, true, false);
            dbMysql.Sql(",", false, false);
            dbMysql.Sql(strTime, true, false);
            dbMysql.Sql(");", false, false);
            DEBUG_INFO(dbMysql.getSql());
            dbMysql.Exec();
        }

        string userid = std::to_string(buyuser.userid);
        string strTime = std::to_string(iNowTime);
        string matchID = to_string(id);
        string buy_invite_1 = to_string(buyuser.invite_uid_1);
        string buy_invite_2 = to_string(buyuser.invite_uid_2);
        string buy_invite_3 = to_string(buyuser.invite_uid_3);

        string sell_invite_1 = to_string(selluser.invite_uid_1);
        string sell_invite_2 = to_string(selluser.invite_uid_2);
        string sell_invite_3 = to_string(selluser.invite_uid_3);
        insertToAssetsLog(dbLog,buy_invite_1,userid,string("0"),string("1"),Config::sCoinB,buyuser.invite_fee_1,matchID,strTime);
        insertToAssetsLog(dbLog,buy_invite_2,userid,string("1"),string("1"),Config::sCoinB,buyuser.invite_fee_2,matchID,strTime);
        insertToAssetsLog(dbLog,buy_invite_3,userid,string("2"),string("1"),Config::sCoinB,buyuser.invite_fee_3,matchID,strTime);

        insertToAssetsLog(dbLog,sell_invite_1,userid,string("0"),string("2"),Config::sCoinB,selluser.invite_fee_1,matchID,strTime);
        insertToAssetsLog(dbLog,sell_invite_2,userid,string("1"),string("2"),Config::sCoinB,selluser.invite_fee_2,matchID,strTime);
        insertToAssetsLog(dbLog,sell_invite_3,userid,string("2"),string("2"),Config::sCoinB,selluser.invite_fee_3,matchID,strTime);

        // string data = CTool::getYYMMDD();
        // insertToSumTable(dbLog,buy_invite_1,data,buyuser.invite_fee_1,Config::sCoinB,strTime);
        // insertToSumTable(dbLog,buy_invite_2,data,buyuser.invite_fee_2,Config::sCoinB,strTime);
        // insertToSumTable(dbLog,buy_invite_3,data,buyuser.invite_fee_3,Config::sCoinB,strTime);
        // insertToSumTable(dbLog,sell_invite_1,data,selluser.invite_fee_1,Config::sCoinB,strTime);
        // insertToSumTable(dbLog,sell_invite_2,data,selluser.invite_fee_1,Config::sCoinB,strTime);
        // insertToSumTable(dbLog,sell_invite_3,data,selluser.invite_fee_1,Config::sCoinB,strTime);
    }
    // bool insertToSumTable(UBMysql &dbLog, string uid,string data,string fee,string coinid,string strTime)
    // {
    //     if (CTool::minusLongDouble(fee,"0.0000000000000001") == "-1")
    //     {
    //         return true;
    //     }
    //     string sqlBase = "SELECT total,id FROM coin_invite_trade_income_sum where user_id = \"";
    //     dbLog.Sql(sqlBase, false, true);
    //     dbLog.Sql(uid, true, false);
    //     dbLog.Sql("\" AND coin_name = \"", false, false);
    //     dbLog.Sql(coinid, true, false);
    //     dbLog.Sql("\");", false, false);
    //     if (dbLog.Exec() > 0)
    //     {
    //         string total,id;
    //         dbLog.Value(0,total);
    //         dbLog.Value(1,id);
    //         total = CTool::addLongDouble(total,fee);
    //         string sqlUpdate = "UPDATE coin_invite_trade_income_sum SET total = " + total + ",update_time = " + strTime + " WHERE id =" + id + ";";
    //         if (dbLog.Exec(sqlUpdate) < 0)
    //         {
    //             return false;
    //         }
    //     }
    //     else
    //     {
    //         string sqlInsert = "INSERT INTO coin_invite_trade_income_sum (user_id,date,total,coin_name,create_time,update_time) VALUES (\"";
    //         dbLog.Sql(sqlInsert, false, true);
    //         dbLog.Sql(uid, false, false);
    //         dbLog.Sql("\",\"", false, false);
    //         dbLog.Sql(data, false, false);
    //         dbLog.Sql("\",\"", false, false);
    //         dbLog.Sql(fee, false, false);
    //         dbLog.Sql("\",\"", false, false);
    //         dbLog.Sql(coinid, false, false);
    //         dbLog.Sql("\",\"", false, false);
    //         dbLog.Sql(strTime, true, false);
    //         dbLog.Sql("\",\"", false, false);
    //         dbLog.Sql(strTime, true, false);
    //         dbLog.Sql(");", false, false);
    //         DEBUG_INFO(dbLog.getSql());
    //         if (dbLog.Exec() < 0)
    //         {
    //             return false;
    //         }
    //     }
    //     return true;
    // }
    bool insertToAssetsLog(UBMysql &dbLog, string uid, string inviteUid, string inviteLevel, string type, string coinid, string fee, string bid, string strTime)
    {
        if (CTool::minusLongDouble(fee,"0.0000000000000001") == "-1")
        {
            return true;
        }
        
        string sqlBase = "INSERT INTO coin_invite_trade_income (user_id,invite_user_id,income_level,trade_type,coin_name,income_amount,trade_log_id,create_time) VALUES (\"";
        dbLog.Sql(sqlBase, false, true);
        dbLog.Sql(uid, false, false);
        dbLog.Sql("\",\"", false, false);
        dbLog.Sql(inviteUid, false, false);
        dbLog.Sql("\",\"", false, false);
        dbLog.Sql(inviteLevel, false, false);
        dbLog.Sql("\",\"", false, false);
        dbLog.Sql(type, false, false);
        dbLog.Sql("\",\"", false, false);
        dbLog.Sql(coinid, false, false);
        dbLog.Sql("\",\"", false, false);
        dbLog.Sql(fee, false, false);
        dbLog.Sql("\",\"", false, false);
        dbLog.Sql(bid, false, false);
        dbLog.Sql("\",\"", false, false);
        dbLog.Sql(strTime, true, false);
        dbLog.Sql(");", false, false);
        DEBUG_INFO(dbLog.getSql());
        if (dbLog.Exec() >= 0)
        {
            return true;
        }
        return false;
    }
public:
    uint64_t id;    // match id
    string price;   // 本次撮合成交价格
    string number;  // 本次撮合成交数量
    int time;   // 撮合成功时间
    uint64_t sysUid; // 平台用户id
    CTransUserModel buyuser;
    CTransUserModel selluser;
};
#endif
