#ifndef _UP_MATCH_FILE_H_
#define _UP_MATCH_FILE_H_
#include "UBaseLib.h"
#include "UProtocolBase.h"

class UPMatchFile
{
public:
    UPMatchFile(){};
    ~UPMatchFile(){};

	class Order
	{
	public:
		/* 数据日志&入库字段 */
		uint64_t id;			 // order id
		uint32_t user_id;		 // 用户id
		uint32_t market_id;		 // 市场id
		char price[34];			 // 价格
		char trading_volume[34]; // 交易总数
		char traded_volume[34];	 // 已成交数
		char trading_amount[34]; // 总额
		char sys_fee[34];		 // 平台手续费
		char dealFee[34];		 // 真实手续费
		uint8_t oper_type;		 // 买卖类型
		int time;				 // 成交时间
		/* 重启日志需要字段 */
		uint32_t invite_uid_1; // 用户邀请人1级
		uint32_t invite_uid_2; // 用户邀请人2级
		uint32_t invite_uid_3; // 用户邀请人3级
		uint32_t sys_uid;	   // 平台用户id
		float vip_discount;	   // 根据用户vip等级计算出的打折率
	};

	class Cancel
	{
	public:
		/* 数据日志&入库字段 */
		uint64_t id;					// order id
		uint64_t user_id;				// 用户id
		/* 重启日志需要字段 */
		uint64_t market_id;				// 市场id
		// int oper_type;					// 买卖类型
		// int time;						// 成交时间
		// float rate; 					// 此单手续费率
		// char price[34];	   				// 成交价格
		// char trading_volume[34]; 		// 交易总数
		// char trading_volume[34];		// 已交易总数
	};

	class Match
	{
	public:
		/* 数据日志&入库字段 */
		uint64_t id;						// match id
		uint64_t buyer_user_id;				// 买方用户id
		uint64_t seller_user_id;			// 卖方用户id
		uint64_t sysUid;					// 平台uid，用于收取手续费
		uint32_t market_id;					// 市场id
		char price[34];						// 价格
		char dealnum[34];					// 本次撮合成交数量
		char trading_amount[34];			// 交易总额
		char buyer_sys_fee[34];				// 买平台手续费
		char seller_sys_fee[34];			// 卖平台手续费
		int time;							// 撮合成功时间
		// uint8_t oper_type;					// 1:buy,2:sell 撮合成功不区分买卖
		uint64_t buyer_order_id;			// 买方订单号
		uint64_t seller_order_id;			// 卖方订单号

		/* 重启日志需要字段 */
		char buyer_price[34];				// 买方下单价格
		char seller_price[34];				// 卖方下单价格
		float buyer_discount;				// 买方vip折扣率
		float seller_discount;				// 卖方vip折扣率
		// char trading_volume[34];			// 交易总数 和 dealnum 重复
		char buyer_traded_volume[34];				// 已交易总数
		char seller_traded_volume[34];				// 已交易总数
		uint64_t buyer_invite_uid_1;		// 买方邀请人1的uid
		uint64_t buyer_invite_uid_2;		// 买方邀请人2的uid
		uint64_t buyer_invite_uid_3;		// 买方邀请人3的uid
		char buyer_invite_fee_1[34];		// 买方邀请人1获得手续费
		char buyer_invite_fee_2[34];		// 买方邀请人2获得手续费
		char buyer_invite_fee_3[34];		// 买方邀请人3获得手续费
		uint64_t seller_invite_uid_1;		// 卖方邀请人1的uid
		uint64_t seller_invite_uid_2;		// 卖方邀请人1的uid
		uint64_t seller_invite_uid_3;		// 卖方邀请人1的uid
		char seller_invite_fee_1[34];		// 卖方邀请人1获得手续费
		char seller_invite_fee_2[34];		// 卖方邀请人2获得手续费
		char seller_invite_fee_3[34];		// 卖方邀请人3获得手续费
	};

public:
	uint8_t type; // 0: not valid,1:order,2:cancel,3:match
	union 
	{
		Order order;
		Cancel cancel;
		Match match;
	};
};



#endif

