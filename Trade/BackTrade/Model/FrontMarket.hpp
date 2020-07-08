/*
 * @Author: your name
 * @Date: 2020-05-11 19:44:40
 * @LastEditTime: 2020-07-02 22:35:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Trade/BackTrade/Model/depth.hpp
 */ 
#ifndef _DEPTH_MODEL_HEADER_
#define _DEPTH_MODEL_HEADER_

#include "../global.h"

class CSort
{
public:
	bool operator()(string const &A, string const &B) const
	{
		string value2 = CTool::minusLongDouble(A, B);
		return value2 == "-1"?true:false;
	}
};
class CSortBack
{
public:
    bool operator()(string const &A, string const &B) const
    {
        string value2 = CTool::minusLongDouble(A, B);
        return value2 == "-1" ? false : true;
    }
};
class CDepth
{
public:
    map<string,string,CSort> buy;
    map<string,string,CSortBack> sell;
};

class CData
{
public:
    CDepth depth;
};

class CDepthModel
{
public:
    CDepthModel(){type = "depth";}
public:
    string type;
    CData data;
};

class CMarketData
{
public:
    string newPrice;
    string buyPrice;
    string sellPrice;
    string amount;
    string number;
    uint64_t nowTime;
};
class CMarketInfoModel
{
public:
    CMarketInfoModel(){type = "markets";}
public:
    string type;
    CMarketData data;
};
class CKlineData
{
public:
    string startTime;
    string amount;
    string startPrice;
    string highPrice;
    string lowPrice;
    string endPrice;
};
class CKlineInfoModel
{
public:
    CKlineInfoModel(){type = "kline.5m";}

public:
    string getJsonPack()
    {
        Json::Value JRet;
        JRet[0] = data.startTime;
        JRet[1] = data.amount;
        JRet[2] = data.startPrice;
        JRet[3] = data.highPrice;
        JRet[4] = data.lowPrice;
        JRet[5] = data.endPrice;
        Json::FastWriter writer;
        return writer.write(JRet);
    }

public:
    string type;
    CKlineData data;
};

class CLogModel
{
public:
    int  tradeID;
    uint64_t buyOrderID;
    uint64_t sellOrderID;
    string addtime;
    string timestamp;
    int type;
    string price;
    string mum;
    string num;
};
class CTradeLog
{
public:
    int type;
    vector<CLogModel> tradelog;
};
#endif
