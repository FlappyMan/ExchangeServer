[TOC]

# 历史记录
日期		 | 作者		| 内容
-------------|----------|------------------------------
2019-10-25   | WYP     | 文档创建


日期		 | 作者		|  数据库名               | 新表名-旧表名                                                                         |描述
-----------------------------------------------------------------------------|---------------------------------------------------------------
2019-10-25   | WYP     | Assets               | [recommended_trade_award 对应 coin_invit](#recommended_trade_award)                  | 创建
2019-10-25   | WYP     | Order                | [wait_[market_id] & close_[market_id] 对应 coin_trade](#trade)                       | 创建
2019-10-25   | WYP     | Match_Year           | [Match_[Year].[Month_Day] 对应 coin_trade_log](#trade_log)                           | 创建
2019-10-25   | WYP     | SA                   | [kline_minimum 对应 coin_trade_chart](#kline_minimum)                                | 创建
2019-10-25   | WYP     | SA                   | [kline_record 对应 coin_trade_json](#kline_record)                                   | 创建
2019-10-25   | WYP     | SA                   | [financial_records 对应 coin_finance](#financial_records)                            | 创建


## recommended_trade_award
> <a id="recommended_trade_award">recommended_trade_award 对应 coin_invit</a>
> 作者 ：WYP
>
> 新表字段                  | 旧表字段           | 字段说明
> -----------------------|-------------------|--------------------------
> id                      | id                | -
> user_id                 | userid            | 修改      
> recommender_id          | invit             | 修改      
> market_id               | type              | 根据旧表type字段解析market_id
> oper_type               | type              | 根据旧表type字段解析买卖类型      
> coin_id                 | coinname          | coinname统一通过coin_id查询
> trading_volume          | num               | 修改      
> trading_amount          | mum               | 修改      
> profit                  | fee               | 修改      
> -                       | ~~name~~          | -
> -                       | ~~sort~~          | -
> -                       | ~~endtime~~       | -
> -                       | ~~status~~        | -
> create_time             | add_time          | 修改
> update_time             | -                 | 新增    


## wait_[market_id] & close_[market_id]
> <a id="trade">wait_[market_id] & close_[market_id] 对应 coin_trade</a>
> 作者 ：WYP
>
> 新表字段                  | 旧表字段           | 字段说明
> -----------------------|-------------------|--------------------------
> id                      | id                | -
> user_id                 | userid            | 修改      
> market_id               | market            | 新表替换为market_id表示市场
> price                   | price             | -
> trading_volume          | num               | 修改      
> traded_volume           | deal              | 修改      
> trading_amount          | mum               | 修改      
> estimated_fee           | fee               | 修改      
> actual_fee              | deal_fee          | 修改      
> oper_type               | type              | 修改      
> status                  | status            | -
> endtime                 | endtime           | -
> -                       | ~~sort~~          | -
> create_time             | addtime           | 修改      
> update_time             | updatetime        | 修改      






## Match_Year.Month_Day
> <a id="trade_log">Match_Year.Month_Day 对应 coin_trade_log</a>
> 作者 ：WYP
新表字段                  | 旧表字段           | 字段说明
-----------------------|-------------------|--------------------------
id                      | id            | -
buyer_user_id           | userid        | 修改      
seller_user_id          | peerid        | 修改      
market_id               | market        | 新表替换为market_id表示市场
price                   | price         | -
trading_volume          | num           | 修改      
trading_amount          | mum           | 修改      
buyer_fee               | fee_buy       | 修改      
seller_fee              | fee_sell      | 修改      
oper_type               | type          | 修改      
buyer_order_id          | buy_id        | 修改      
seller_order_id         | sell_id       | 修改      
-                       | ~~endtime~~   | -
-                       | ~~sort~~      | -
-                       | ~~status~~    | -
create_time             | addtime       | 修改      
update_time             | updatetime    | 修改 






## kline_minimum
> <a id="kline_minimum">kline_minimum 对应 coin_trade_chart</a>
> 作者 ：WYP
新表字段                  | 旧表字段           | 字段说明
-----------------------|-------------------|--------------------------
id                      | id            | -
market_id               | market        | name为字符串，新表替换为market_id表示市场
begin_time              | addtime       | 修改      
open                    | open          | -
high                    | high          | -
low                     | low           | -
close                   | close         | -
trading_volume          | volume        | 修改      
create_time             | -             | 新增
update_time             | -             | 新增



## kline_record
> <a id="kline_record">kline_record 对应 coin_trade_json</a>
> 作者 ：WYP
新表字段                  | 旧表字段           | 字段说明
-----------------------|-------------------|--------------------------
id                      | id            | -
market_id               | market        | name为字符串，新表替换为market_id表示市场
Kline_type              | type          | 修改      
begin_time              | addtime       | 修改      
trading_volume          | -             | 新增   
open                    | -             | 新增
high                    | -             | 新增
low                     | -             | 新增
close                   | -             | 新增
-                       | ~~status~~    | -
create_time             | -             | 新增
update_time             | -             | 新增


## financial_records
> <a id="financial_records">financial_records 对应 coin_finance</a>
> 作者 ：WYP
新表字段                  | 旧表字段           | 字段说明
-----------------------|-------------------|--------------------------
id                      | id            | -
user_id                 | userid        | 修改      
coin_id                 | coinname      | coinname统一通过coin_id查询
gross_usable            | num_a         | 修改      
gross_frozen            | num_b         | 修改      
gross_sum               | num           | 修改      
fee                     | fee           | -
fee_frozen              | fee_b         | 修改      
oper_type               | type          | 修改       
oper_id                 | nameid        | 修改      
oper_desc               | remark        | 修改      
remaining_usable        | mum_a         | 修改      
remaining_frozen        | mum_b         | 修改      
remaining_sum           | mum           | 修改      
record_hash             | move          | 修改      
status                  | status        | -
create_time             | addtime       | 修改      
update_time             | -             | 新增