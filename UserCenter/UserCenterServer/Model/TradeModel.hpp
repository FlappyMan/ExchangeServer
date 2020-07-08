#ifndef _TRADE_MODEL_HEADER_
#define _TRADE_MODEL_HEADER_

#include "../global.h"
class CTradeModel
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
            this->num = obj.num;
            this->type = obj.type;
            this->main_coin = obj.main_coin;
        }
        return *this;
    }
    CTradeModel &operator=(UPUptrade *pkg)
    {
        this->uid = dynamic_cast<UPUptrade *>(pkg)->uid();
        this->token = dynamic_cast<UPUptrade *>(pkg)->token();
        this->paypassword = dynamic_cast<UPUptrade *>(pkg)->paypassword();
        this->marketid = dynamic_cast<UPUptrade *>(pkg)->marketid();
        this->price = dynamic_cast<UPUptrade *>(pkg)->price();
        this->num = dynamic_cast<UPUptrade *>(pkg)->num();
        this->type = dynamic_cast<UPUptrade *>(pkg)->type();
        return *this;
    }

public:
    string uid;
    string token;
    string paypassword;
    uint64_t marketid;
    string price;
    string num;
    int type;
    int main_coin;
};

#endif
