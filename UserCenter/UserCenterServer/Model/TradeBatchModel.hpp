/*
 * @Author: your name
 * @Date: 2020-04-03 01:45:55
 * @LastEditTime: 2020-05-26 00:53:39
 * @LastEditors: your name
 * @Description: In User Settings Edittoken
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Model/TradeBatchModel.hpp
 */ 
#ifndef _TRADEBATCH_MODEL_HEADER_
#define _TRADEBATCH_MODEL_HEADER_

#include "../global.h"

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
        }

        return *this;
    }
    CTradeInfoModel &operator=(UPUptradebatch::Trades& pkg)
    {
        this->price = pkg.price();
        this->marketid = pkg.marketid();
        this->num = pkg.num();
        this->type = pkg.type();
        return *this;
    }

public:
    string marketid;
    string price;
    string num;
    int type;
};

class CTradeBatchModel
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

public:
    string uid;
    string token;
    string paypassword;
    vector<CTradeInfoModel> trades;
};

#endif
