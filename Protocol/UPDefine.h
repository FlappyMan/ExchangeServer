/*
 * @Author: your name
 * @Date: 2020-06-08 01:30:47
 * @LastEditTime: 2020-06-27 22:51:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/Protocol/UPDefine.h
 */ 
#ifndef _UPDEFINE_HEADER_
#define _UPDEFINE_HEADER_

enum ORDER_OPER_TYPE
{
    ORDER_OPER_TYPE_BUY=1,
    ORDER_OPER_TYPE_SELL=2,
};

enum UPTRADE_OPER_TYPE
{
    UPTRADE_OPER_TYPE_BUY=1,
    UPTRADE_OPER_TYPE_SELL=2,
};

enum CANCEL_TRADE_OPER_TYPE
{
    CANCEL_TRADE_OPER_TYPE_BUY=1,
    CANCEL_TRADE_OPER_TYPE_SELL=2,
};

enum FILE_REQUEST_OPER_TYPE
{
    FILE_REQUEST_OPER_TYPE_UPLOAD=1,
    FILE_REQUEST_OPER_TYPE_DOWNLOAD=2,

    FILE_REQUEST_OPER_TYPE_CANCEL_UPLOAD,
    FILE_REQUEST_OPER_TYPE_CANCEL_DOWNLOAD,
};

enum MATCH_FILE_TYPE
{
    MATCH_TYPE_UNKNOW=0,
	MATCH_TYPE_ORDER=1,
	MATCH_TYPE_CANCEL=2,
	MATCH_TYPE_MATCH=3,
};
enum ASSETS_UPDATE
{
    OPER_TYPE_UPTRADE = 1,
    OPER_TYPE_CANCELTRADE = 2,
    OPER_TYPE_UPTRADE_BATCH,
    OPER_TYPE_CANCELTRADEE_BATCH,
    OPER_TYPE_MATCH_SUC,
    OPER_TYPE_SELECT,
    
    OPER_ID_VALIDATION = 0,
    OPER_ID_NO_VALIDATION,

    OPER_TYPE_COMMISION_BUY = 1,                       // 1、委托交易买入
    OPER_TYPE_COMMISION_SELL,                          // 2、委托交易卖出
    OPER_TYPE_TRADE_MATCH,                             // 3、交易撮合(用户的可用和冻结都会变动)
    OPER_TYPE_TRADE_RECOMMENDATION_REWARDS,            // 4、交易推荐奖励
    OPER_TYPE_TRANSACTION_FEE,                         // 5、交易手续费收入(平台账户收入)
    OPER_TYPE_VISA_FIAT_TOPUP,                         // 6、UKEX visa法币充值
    OPER_TYPE_BANK_REMITTANCE_FIAT_RECHARGE,           // 7、银行汇款法币充值
    OPER_TYPE_THIRD_PARTY_PAYMENT_FIAT_RECHARGE,       // 8、第三方支付法币充值
    OPER_TYPE_VISA_FIAT_WITHDRAW,                      // 9、UKEX visa法币提现
    OPER_TYPE_BANK_TRANSFER_FIAT_WITHDRAWAL,           // 10、银行汇款法币体现
    OPER_TYPE_BANK_TRANSFER_FIAT_WITHDRAWAL_FEE,       // 11、银行汇款法币提现手续费(平台账户收入)
    OPER_TYPE_THIRD_PARTY_PAYMENT_FIAT_RECHARGE_FEE,   // 12、第三方支付法币充值手续费(平台账户收入)
    OPER_TYPE_OTC_FUND_FREEZE,                         // 13、OTC资金冻结
    OPER_TYPE_OTC_PAYMENT,                             // 14、OTC付款
    OPER_TYPE_OTC_COLLECT_MONEY,                       // 15、OTC收款
    OPER_TYPE_TRANSFER_OUT_ACCORDING_TO_ADDRESS,       // 16、根据地址站内转出
    OPER_TYPE_TRANSFER_IN_ACCORDING_TO_ADDRESS,        // 17、根据地址站内转入
    OPER_TYPE_TRANSFER_OUT_ACCORDING_TO_ADDRESS_FEE,   // 18、根据地址站内转出手续费(平台账户收入)
    OPER_TYPE_TRANSFER_OUT_OF_PLATFORM_NORMAL_STATION, // 19、平台普通站内转出
    OPER_TYPE_TRANSFER_IN_OF_PLATFORM_NORMAL_STATION,  // 20、平台普通站内转入
    OPER_TYPE_TRANSFER_OUT_OF_PLATFORM_NORMAL_FEE,     // 21、平台普通站内转出手续费(平台账户收入)
    OPER_TYPE_WITHDRAWAL_OUTSIDE_STATION,              // 22、站外提币
    OPER_TYPE_WITHDRAWAL_OUTSIDE_STATION_FEE,          // 23、站外提币手续费(平台账户收入)
    OPER_TYPE_RECHARGE_OUTSIDE_STATION,                // 24、站外充币
    OPER_TYPE_PLATFORM_GIFT,                           // 25、平台赠送
    OPER_TYPE_VISA_CARD_HANDLING_FEE,                  // 26、UKEX visa开卡收费（用户支出，平台账户收入）
    OPER_TYPE_VISA_RECHARGE_FEE,                       // 27、UKEX visa充值手续费(平台账户收入)
    OPER_TYPE_VISA_WITHDRAWAL_FEE,                     // 28、UKEX visa提现手续费(平台账户收入)
    OPER_TYPE_PROJECT_SIDE_COLLECTION,                 // 29、项目方归集
    OPER_TYPE_PLATFORM_SYSTEM_RECHARGE,                // 30、平台系统充值
    OPER_TYPE_FREEZE_RELEASE,                          // 31、冻结释放
};

#endif