[TOC]

# 历史记录
日期		 | 作者		| 内容
-------------|----------|------------------------------
2019-10-17   | 吴友鹏     | 文档创建


日期		 | 作者		|  新增或修改的接口API                                                           | 描述
-----------------------------------------------------------------------------|------
2019-10-17   | 吴友鹏     | [Mapi/Trade/upTrade](#Trade_upTrade)                                | 新增接口：提交交易
2019-10-17   | 吴友鹏     | [Mapi/Trade/cancelTrade](#Trade_cancelTrade)                        | 新增接口：撤销订单
2019-10-17   | 吴友鹏     | [Mapi/Trade/upTradeBatch](#Trade_upTrade_batch)                     | 新增接口：批量提交交易
2019-10-17   | 吴友鹏     | [Mapi/Trade/cancelTradeBatch](#Trade_cancelTradeBatch)              | 新增接口：批量撤销订单
2019-10-17   | 吴友鹏     | [Mapi/Trade/getOrderInfo](#Trade_getOrderInfo)                      | 新增接口：查询订单信息



日期		 | 作者		|  FrontTrade新增或修改的自定义数据包                                             | 描述
-----------------------------------------------------------------------------|------
2019-10-17   | 吴友鹏     | [FrontTrade服务器转发数据包](#FrontTrade_trans)                               | 新增报文:转发客户端请求


日期		 | 作者		|  FrontMarket新增或修改的自定义数据包                                            | 描述
-----------------------------------------------------------------------------|------
2019-10-17   | 吴友鹏     | [FrontMarket服务器请求K线历史记录](#FrontMarket_Kline)                         | 新增报文:Frontmarket请求K线历史记录

日期		 | 作者		|  BackTrade新增或修改的自定义数据包                                            | 描述
-----------------------------------------------------------------------------|------
2019-10-17   | 吴友鹏     | [推送-交易市场总信息预览](#BackTrade_pushMarketInfo)                            | 新增报文:推送交易市场总信息
2019-10-17   | 吴友鹏     | [推送-K线数据预览](#BackTrade_Kline)                                           | 新增报文:推送K线数据
2019-10-17   | 吴友鹏     | [推送-深度数据预览](#BackTrade_Deep)                                           | 新增报文:推送深度数据
2019-10-17   | 吴友鹏     | [推送-历史交易数据](#BackTrade_historyDealInfo)                                | 新增报文:历史交易数据
2019-10-17   | 吴友鹏     | [推送-撮合成功](#BackTrade_matchup_success)                                    | 新增报文:撮合成功

# API说明
- 手机端与FrontTrade服务端采用Http通信
- FrontTrade与BackTrade服务器之间采用TCP长连接自定义协议通信
- BackTrade与FrontMarket服务器之间采用TCP长连接自定义协议通信
- FrontMarket与客户端之间采用websocket长连接通信
- http header将附上一些特定的值
- 请求与返回数据格式均为JSON字符串
- 接口相对路径：/api/{0}/{1}，其中0和1为不同接口动态变化的值
- 带删除线的接口为废弃接口，但仍在使用。不再使用的接口会直接从文档删除

## header说明
- 必填项为token
- 其余参数可参照之前设计不变动

### User-Agent
- 无


## json格式说明
服务端返回的json将包含固定的status和数据字段data。

### 状态码Code
状态码将分不同的号段供不同的接口使用，以达到根据编号即可查出是哪个接口的哪个错误。
- 0 ：status 失败
- 1 ：status 正常

### 返回值格式
格式一：返回单个值

    {"status": 1, "data":31276}

格式二：返回对象

    {"status": 1,"data":{"Name":"兑换商品1", "Desc":"兑换商品描述"}}

格式三：返回列表

    {"status": 1,"data":[
        {"Name":"兑换商品1", "Desc":"兑换商品描述"},
        {"Name":"兑换商品1", "Desc":"兑换商品描述"}
    ]}
### 其它约定
- 暂无

# 接口及数据报文
## http 客户端API请求
### 提交交易
> <a id="Trade_upTrade">Trade/upTrade</a>
> 作者 ：吴友鹏

客户端带参请求，购买币或卖出币

请求方式：POST

请求数据：

字段                   | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
uid                    | string            |  是  | 用户id
paypassword            | string            |  是  | 支付密码
marketid               | int               |  是  | 币种 id，通过之前的接口getCoinList 获取
price                  | int               |  是  | 价格
num                    | int               |  是  | 数量
type                   | int               |  是  | 交易类型，1 买入,2 卖出

响应status:

- 0 ：status 失败
- 1 ：status 正常

响应数据：

字段                   | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
status                 | int               |  是  | 0-失败，1-成功
data                   | string            |  是  | "交易成功"或"交易失败"

### 批量提交交易
> <a id="Trade_upTrade_batch">Trade/upTradeBatch</a>
> 作者 ：吴友鹏

客户端带参请求，购买币或卖出币

请求方式：POST

请求数据：

字段                   | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
uid                    | string            |  是  | 用户id
paypassword            | string            |  是  | 支付密码
trades                 | List<T>           |  是  | 订单列表信息

T -> trades:
字段                   | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
marketid               | int               |  是  | 币种 id，通过之前的接口getCoinList 获取
price                  | int               |  是  | 价格
num                    | int               |  是  | 数量
type                   | int               |  是  | 交易类型，1 买入,2 卖出


响应status:

- 0 ：status 失败
- 1 ：status 正常

响应数据：

字段                   | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
status                 | int               |  是  | 0-失败，1-成功
data                   | string            |  是  | "交易成功"或"交易失败"

### 撤销订单
> <a id="Trade_cancelTrade">Trade/cancelTrade</a>
> 作者 ：吴友鹏

客户端带参请求，撤销订单

请求方式：POST

请求数据：

字段                   | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
uid                    | string           |  是  | 用户id
marketid               | int               |  是  | 币种 id，通过之前的接口getCoinList 获取
id                     | int               |  是  | 订单 id

响应status:

- 0 ：status 失败
- 1 ：status 正常

响应数据：

字段                   | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
status                 | int               |  是  | 0-失败，1-成功
data                   | string            |  是  | "交易成功"或"交易失败"


### 批量撤销订单
> <a id="Trade_cancelTradeBatch">Trade/cancelTradeBatch</a>
> 作者 ：吴友鹏

客户端带参请求，撤销订单

请求方式：POST

请求数据：

字段                   | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
uid                    | string            |  是  | 用户id
orders                 | List<T>           |  是  | 订单列表信息

T -> orders:
字段                   | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
marketid               | int               |  是  | 币种 id，通过之前的接口getCoinList 获取
id                     | int               |  是  | 订单id

响应status:

- 0 ：status 失败
- 1 ：status 正常

响应数据：

字段                   | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
status                 | int               |  是  | 0-失败，1-成功
data                   | string            |  是  | "交易成功"或"交易失败"

### 查询交易中的订单
> <a id="Trade_getOrderInfo">Trade/getOrderInfo</a>
> 作者 ：吴友鹏

客户端带参请求，撤销订单

请求方式：POST

请求数据：

字段                   | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
uid                    | int               |  否  | 用户id
marketid               | int               |  否  | 币种 id，通过之前的接口getCoinList 获取
orderid                | int               |  否  | 订单id

请求说明：根据订单号查询
字段                   | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
orderid                | int               |  是  | 订单id

请求说明：根据市场查询
字段                   | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
uid                    | int               |  是  | 用户id
marketid               | int               |  是  | 币种 id，通过之前的接口getCoinList 获取

响应status:

- 0 ：status 失败
- 1 ：status 正常

响应数据：

字段                   | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
status                 | int               |  是  | 0-失败，1-成功
data                   | T                 |  是  | data1(status == 1) 或 data2(status == 0)

T -> data1:
字段                   | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
orderNumber            | int               |  是  | 订单id
initialRate            | string            |  是  | 下单价格
initialAmount          | string            |  是  | 下单量
amount                 | string            |  是  | 未成交数量
type                   | string            |  是  | 买卖类型 sell卖出, buy买入
lastDealTime           | int               |  是  | 成交时间
status                 | string            |  是  | cancelled已取消 done已完成
currencyPair           | string            |  是  | 交易对(示例:ltc_btc)

T -> data2:
字段                   | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
orderid                 | int               |  是  | 订单id



## FrontTrade服务端TCP数据报文
### FrontTrade-服务器转发数据包
> <a id="FrontTrade_trans">FrontTrade-服务器转发数据包</a>
> 作者 ：吴友鹏

请求方式：TCP长连接转发protocol buffer格式数据包

数据报文json格式：[{"token":"asdfasdfsadf","paypassword":"123","marketid":"111","price":100,"num":10,"type":1,"main_coin":"btc"}]

参数说明：
字段                   | 数据类型          | 必填 | 备注
-----------------------|------------------|------|--------------------------
uid                    | string           |  是  | 用户id
token                  | string           |  是  | 用户token，对应用户的userId唯一标识
paypassword            | string           |  是  | 交易密码
marketid               | int              |  是  | 币种 id，通过之前的接口getCoinList 获取
price                  | int              |  是  | 价格
num                    | int              |  是  | 数量
type                   | int              |  是  | 交易类型，1 买入,2 卖出

响应status:

- 0 ：status 失败
- 1 ：status 正常

响应报文json格式：{"status":1,"data":"交易成功"}

响应参数说明：

字段                   | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
status                 | int               |  是  | 0-失败，1-成功
data                   | string            |  是  | "交易成功"或"交易失败"

## BackTrade服务端TCP数据报文
### BackTrade-交易市场总信息
> <a id="BackTrade_pushMarketInfo">BackTrade-交易市场总信息</a>
> 作者 ：吴友鹏

请求方式：TCP长连接转发protocol buffer格式数据包

数据报文json格式：{"type":"markets","data":[{"0":"btc/usdt","1":"8400.23","2":"8400.23","3":"8400.23","4":"452","5":"8400.23","6":"4524","7":"0.64","8":"btc_usdt","9":"/upload/xxx","10":"8400.23","11":"btcoin","12":"/upload/xxx","13":""}],"exchange":{"btc":"8400.23","eth":"123.2"}}

参数说明：
字段                   | 数据类型          | 必填 | 备注
-----------------------|------------------|------|--------------------------
type                   | string           |  是  | markets市场标志
data                   | List<T>          |  是  | 市场信息内容
exchange               | T                |  是  | 价格变动信息

T -> data:
字段                   | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
"0"                    | string            |  是  | 市场交易名称
"1"                    | string            |  是  | 最新价格
"2"                    | string            |  是  | 买入价格
"3"                    | string            |  是  | 卖出价格
"4"                    | string            |  是  | 24小时交易额
"5"                    | string            |  是  | 最低价
"6"                    | string            |  是  | 24小时交易量
"7"                    | string            |  是  | 24小时变化率
"8"                    | string            |  是  | 市场交易名称
"9"                    | string            |  是  | 购买币显示图标（web端使用）
"10"                   | string            |  是  | 购买币最高价格
"11"                   | string            |  是  | 购买币名称
"12"                   | string            |  是  | 购买币显示图标（移动端使用）
"13"                   | string            |  是  | 如果是法币交易市场则为：currency 否则为空字符串

T -> exchange:
字段                   | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
"btc"                  | string            |  是  | btc价格
"eth"                  | string            |  是  | eth价格

响应报文：无

### BackTrade-K线数据
> <a id="BackTrade_Kline">BackTrade-K线数据</a>
> 作者 ：吴友鹏

请求方式：TCP长连接转发protocol buffer格式数据包

数据报文json格式：{"type":"kline.5m","data":[{"0":154221241,"1":"142","2":"8400.23","3":"8400.23","4":"8400.23","5":"8400.23"}]}

参数说明：
字段                   | 数据类型          | 必填 | 备注
-----------------------|------------------|------|--------------------------
type                   | string           |  是  | kline.5m-K线标志
data                   | List<T>          |  是  | 市场信息内容

T -> data:
字段                   | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
"0"                    | string            |  是  | k线起始时间
"1"                    | string            |  是  | 时间段内交易总量
"2"                    | string            |  是  | k线开始价格
"3"                    | string            |  是  | k线高点价格
"4"                    | string            |  是  | k线低点价格
"5"                    | string            |  是  | k线收盘价格

响应报文：无

### BackTrade-深度数据
> <a id="BackTrade_Deep">BackTrade-深度数据</a>
> 作者 ：吴友鹏

请求方式：TCP长连接转发protocol buffer格式数据包

数据报文json格式：{"type":"depth","data":{"depth":{"buy":[{"0":"8400.23","1":"20"},{"0":"8400.24","1":"20"}],"sell":[{"0":"8400.23","1":"20"},{"0":"8400.24","1":"20"}]}}}

参数说明：
字段                   | 数据类型          | 必填 | 备注
-----------------------|------------------|------|--------------------------
type                   | string           |  是  | depth标志
data                   | T                |  是  | 深度内容

T -> data:
字段                   | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
depth                  | T                 |  是  | 深度内容

T -> depth:
字段                   | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
buy                    | List<T>           |  是  | 买-深度队列
sell                   | List<T>           |  是  | 卖-深度队列

T -> buy&sell:
字段                   | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
"0"                    | string            |  是  | 价格
"1"                    | string            |  是  | 数量


响应报文：无


### BackTrade-历史交易数据
> <a id="BackTrade_historyDealInfo">BackTrade-历史交易数据</a>
> 作者 ：吴友鹏

请求方式：TCP长连接转发protocol buffer格式数据包

数据报文json格式：{"type":"tradelog","data":{"tradelog":[{"tradeID":1231241,"addtime":"18:12:45","timestamp":154245124,"type":1,"price":"8451.14","mum":"8451.14","num":"1"}]}}

参数说明：
字段                   | 数据类型          | 必填 | 备注
-----------------------|------------------|------|--------------------------
type                   | string           |  是  | tradelog标志
data                   | T                |  是  | 深度内容

T -> tradelog:
字段                   | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
tradelog               | List<T>           |  是  | 日志列表内容

T -> tradelog:
字段                   | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
tradeID                | int               |  是  | 交易id
buyOrderID             | string            |  是  | 购买订单号
sellOrderID            | string            |  是  | 卖出订单号
addtime                | string            |  是  | 交易创建时间
timestamp              | string            |  是  | 交易成交时间
type                   | int               |  是  | 1-买入，2-卖出
price                  | string            |  是  | 成交价格
mum                    | string            |  是  | 成交总额：成交总额=价格*成交量
num                    | string            |  是  | 成交量

响应报文：无


### BackTrade-撮合成功
> <a id="BackTrade_matchup_success">BackTrade-撮合成功</a>
> 作者 ：吴友鹏

请求方式：TCP长连接转发protocol buffer格式数据包

数据报文json格式：{"addtime":"07-05 19:34:48","id":321841,"price":17090,"buyuser":{"userid":"asdfasdf","num":100,"deal":50,"cny":1000,"cnyd":100},"selluser":{"userid":"asdfasdf","num":1000,"deal":50,"xnb":10000,"xnbd":1000}}

参数说明：
字段                   | 数据类型          | 必填 | 备注
-----------------------|------------------|------|--------------------------
price                  | string           |  是  | 成交价格
number                 | string           |  是  | 成交数量
buyuser                | T                |  是  | 买方信息
selluser               | T                |  是  | 卖方信息

T -> buyuser:
字段                   | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
userid                 | string           |  是  | 用户id
orderid                | int              |  是  | 订单号
buyrice                | string           | 是   | 购买原始价格

T -> selluser:
userid                 | string           |  是  | 用户id
orderid                | int              |  是  | 订单号

响应报文：无


## FrontMarket服务端TCP数据报文
### FrontMarket-服务器请求K线历史记录
> <a id="FrontMarket_Kline">FrontMarket-服务器请求K线历史记录</a>
> 作者 ：吴友鹏

请求方式：TCP长连接转发protocol buffer格式数据包

数据报文json格式：{"name":"下载文件名","path":"下载文件路径","modifytime":"修改时间","offset":1000,"length":"文件长度上限"}

参数说明：
字段                   | 数据类型          | 必填 | 备注
-----------------------|------------------|------|--------------------------
name                   | string           |  是  | 下载文件名
path                   | string           |  是  | 下载文件路径
modifytime             | string           |  是  | 修改时间
offset                 | string           |  是  | 现有内容偏移量
length                 | string           |  是  | 文件长度上限

响应报文json格式：[{"0":154221241,"1":"142","2":"8400.23","3":"8400.23","4":"8400.23","5":"8400.23"}]

响应参数说明：该服务请求接口通用，此处说明仅用于描述下载k线这个业务的数据格式内容
字段                   | 数据类型           | 必填       | 备注
-----------------------|-------------------|------------|--------------------------
0                      | datetime          | 是         | 时间戳
1                      | int               | 是         | 成交量
2                      | decimal           | 是         | 开
3                      | decimal           | 是         | 高
4                      | decimal           | 是         | 低
5                      | decimal           | 是         | 收



## FrontMarket服务端websocket返回客户端数据报文
### 参考 [服务器请求K线历史记录](#FrontMarket_Kline)
### 参考 [推送-交易市场总信息预览](#BackTrade_pushMarketInfo)
### 参考 [推送-K线数据预览](#BackTrade_Kline)
### 参考 [推送-深度数据预览](#BackTrade_Deep)
### 参考 [推送-历史交易数据](#BackTrade_historyDealInfo)
### FrontMarket-通知用户撮合成功
> <a id="BackTrade_matchup_success">FrontMarket-通知用户撮合成功</a>
> 作者 ：吴友鹏

接口说明：推送此消息后，客户端收到此消息，则代表服务器撮合信息成功，此时在调取交易接口Mapi/Ajax/getEntrustAndUsercoin获取最新交易记录

请求方式：TCP长连接json格式数据包

数据报文json格式：{"type":"trade_success","data":1}

参数说明：
字段                   | 数据类型          | 必填 | 备注
-----------------------|------------------|------|--------------------------
type                   | string           |  是  | 通知类型trade_success，交易撮合成功
data                   | int              |  是  | 默认初始值为1

