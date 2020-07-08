
#ifndef _UPROTOCOL_BASE_HEADER_
#define _UPROTOCOL_BASE_HEADER_

#include "UBaseLib.h"

class UProtocolBase
{
    public:
        UProtocolBase():m_uiType(0),m_uiPkgLength(0){};
        virtual ~UProtocolBase(){};

        virtual void JsonPack(Json::Value &root)=0;
        virtual bool JsonUnpack(Json::Value &root)=0;
    public:
        uint16_t m_uiType;      // 包类型
        uint32_t m_uiPkgLength; // 打包后长度，使用时注意场景
        string m_sTag;
        enum {MAXCMD=200};
};

class UVWriteReq
{
    public:
        UVWriteReq():len(0),pkg(NULL){};
        UVWriteReq(UProtocolBase *p):len(0),pkg(p){};
        virtual ~UVWriteReq()
        {
            if(pkg!=NULL)delete pkg;
        };

        void Reset(){len=0;if(pkg){delete pkg;pkg=NULL;};};
    public:
        uv_write_t req;
        uv_buf_t buf;
        uint32_t len;           // buf 中实际开辟空间长度
        UProtocolBase *pkg;
};

enum MARKET
{
    MARKET_UNKNOWN=0,

    // 法币交易
    MARKET_BTC_GBP, MARKET_ETH_GBP, MARKET_BCH_GBP,MARKET_LTC_GBP,
    MARKET_BTC_EUR, MARKET_ETH_EUR, MARKET_LTC_EUR, MARKET_BCH_EUR,

    // 币币交易
    MARKET_LTC_BTC, MARKET_XMR_BTC, MARKET_BCH_BTC, MARKET_ETH_BTC, MARKET_BSV_BTC,
    MARKET_LTC_ETH, MARKET_XMR_ETH, MARKET_AE_ETH, MARKET_BTM_ETH, 
    MARKET_BTC_USDT, MARKET_ETH_USDT, MARKET_LTC_USDT, MARKET_UPB_USDT, MARKET_NCD_USDT, MARKET_DIC_USDT,

    MARKET_CNT,
};

const char MARKET_STR[][10]=
{
    "UNKNOWN",
    "btc_gbp","eth_gbp","bch_gbp","ltc_gbp",
    "btc_eur","eth_eur","ltc_eur","bch_eur",
    "ltc_btc","xmr_btc","bch_btc","eth_btc","bsv_btc",
    "ltc_eth","xmr_eth","ae_eth","btm_eth",
    "btc_usdt","eth_usdt","ltc_usdt","upb_usdt","ncd_usdt","dic_usdt"
};
int MARKETFromString(string &s);

enum CURRENCY
{
    CURRENCY_UNKNOWN=0,
    CURRENCY_USD=1,      // 美元
    CURRENCY_GBP,      // 英镑
    CURRENCY_EUR,      // 欧元
    CURRENCY_CNY,      // 人民币
    CURRENCY_MAX,
};
const char CURRENCY_STR[][8]={"UNKNOWN","USD","GBP","EUR","CNY"};
int CurrencyFromString(string &s);

enum KLINE
{
    KLINE_UNKNOWN=0,
    KLINE_60S,
    KLINE_5M,
    KLINE_15M,
    KLINE_30M,
    KLINE_1H,
    KLINE_6H,
    KLINE_1D,
    KLINE_1W,
    KLINE_1M,
    KLINE_MAX,
};
const char KLINE_STR[][8]={"UNKNOWN","1m","5m","15m","30m","1h","6h","1d","1w","1M"};
int KLineFromString(string &s);



























#endif
