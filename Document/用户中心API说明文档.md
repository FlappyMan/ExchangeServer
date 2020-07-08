[TOC]

# 历史记录
日期		 | 作者		| 内容
-------------|----------|------------------------------
2019-12-30   | WYP     | 文档创建


日期		 | 作者		| API名称                            |描述
-------------|---------|------------------------------------|---------------------------------|---------------------------------------------------------------
2019-12-30   | WYP     | [Api/UserCenter/Regist](#Regist)      | 创建
2020-04-02   | WYP     | [api/UserCenter/Login](#Login)      | 创建
2020-04-02   | WYP     | [Api/Password/Update](#Update)      | 创建
2020-04-02   | WYP     | [Api/Password/Pay](#Pay)      | 创建
2020-04-02   | WYP     | [Api/Device/AddBindDevice](#AddBindDevice)      | 创建
2020-04-02   | WYP     | [Api/Device/RemoveBind](#RemoveBind)      | 创建
2020-04-02   | WYP     | [Api/Device/CheckBind](#CheckBind)      | 创建
2020-04-02   | WYP     | [Api/User/Auth](#Auth)      | 创建
2020-04-02   | WYP     | [Api/User/Info](#Info)      | 创建
2020-04-02   | WYP     | [Api/User/RealInfo](#RealInfo)      | 创建
2020-04-02   | WYP     | [Api/Device/SetMaster](#SetMaster)      | 创建
2020-06-02   | WYP     | ~~[Api/Balance/Add](#Add)~~      | 创建
2020-06-02   | WYP     | ~~[Api/Balance/Reduce](#Reduce)~~      | 创建
2020-06-02   | WYP     | ~~[Api/Balance/Allocation](#Allocation)~~      | 创建
2020-04-02   | WYP     | [Api/Coin/Add](#AddCoin)      | 创建
2020-06-01   | WYP     | [Api/Balance/Change](#BalanceChange)      | 创建

# API说明
- 用户中心server与ukex和ukexpay服务端之间采用Http通信
- 接口相对路径：/api/{0}/{1}，其中0和1为不同接口动态变化的值
- 带删除线的接口为废弃接口，但仍在使用。不再使用的接口会直接从文档删除
- 接口请求所带参数以及返回参数均使用rsa非对称加密，客户端server保存public key，用户中心保存privatekey

## header说明
- 除登陆（Login）和注册（Regist）接口外 token为必填项

### User-Agent
- 无


## json格式说明
服务端返回的json将包含固定的status和数据字段data。

## 状态码Code
状态码将分不同的号段供不同的接口使用，以达到根据编号即可查出是哪个接口的哪个错误。


## 返回值格式
格式一：返回单个值

    {"status": 1, "data":31276}

格式二：返回对象

    {"status": 1,"data":{"Name":"兑换商品1", "Desc":"兑换商品描述"}}

格式三：返回列表

    {"status": 1,"data":[
        {"Name":"兑换商品1", "Desc":"兑换商品描述"},
        {"Name":"兑换商品1", "Desc":"兑换商品描述"}
    ]}

# 接口及数据报文
## http 客户端API请求

### 新增用户账号
> <a id="Regist">Api/UserCenter/Regist</a>
> 作者 ：吴友鹏

是否携带token：否

接口调用说明：客户端用户注册后，需要添加新用户的时候调用此接口，并且
用户中心server会在表：user,user_account,user_auth,user_facts,user_info中个插入一条初始化数据
用户中心会在所有资产表（user_assets_[coin_ID]）插入一条置空数据


请求方式：POST

请求数据：

字段                    | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
deviceID              | string            |  是  | 设备唯一码
userAccount              | string            |  是  | 用户账号
password               | string            |  是  | 用户密码
sourceType            | int               |  是  | 来源类型 1-ukex；2-ukexpay；

响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data

字段                   | 数据类型           | 必填 | 备注
-----------------------|-------------------|------|--------------------------
token                  | string            |  是  | server生成的token
deadLine               | int               |  是  | token截至日期
uid                    | string            |  是  | userid



### 登录
> <a id="Login">Api/UserCenter/Login</a>
> 作者 ：吴友鹏

接口调用说明：需要验证用户账号和密码是否正确时调用，调用成功后返回时效性token以及userid

是否携带token：否

请求方式：POST

请求数据：

字段                    | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
deviceID              | string            |  是  | 设备唯一码
userAccount              | string            |  是  | 用户账号
password               | string            |  是  | 用户密码
sourceType            | int               |  是  | 来源类型：1-ukex；2-ukexpay

响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data

字段                   | 数据类型           | 必填 | 备注
-----------------------|-------------------|------|--------------------------
token                  | string            |  是  | server生成的token
deadLine               | int               |  是  | token截至日期
uid                    | string            |  是  | userid

### 更新登录密码
> <a id="Update">Api/Password/Update</a>
> 作者 ：吴友鹏

接口调用说明：用户修改用户密码的接口


是否携带token：是

请求方式：POST

请求数据：

字段                    | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
userAccount              | string            |  是  | 用户账号
password               | string            |  是  | 用户密码

响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data = "密码修改成功"/"密码修改失败"


### 交易密码
> <a id="Pay">Api/Password/Pay</a>
> 作者 ：吴友鹏

接口调用说明：用户修改或验证用户交易密码的接口

是否携带token：是

请求方式：POST

请求数据：

字段                    | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
uid                    | string            |  是  | 用户id
paypassword            | string            |  是  | 支付密码
method              | int               |  是  | 操作类型：1-验证交易密码；2-修改交易密码

响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data = "验证成功"/"验证失败" or "修改成功"/"修改失败"


### 设备绑定
> <a id="AddBindDevice">Api/Device/AddBindDevice</a>
> 作者 ：吴友鹏

接口调用说明：用户绑定用户和设备的接口


是否携带token：是

请求方式：POST

请求数据：

字段                    | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
uid                    | string            |  是  | 用户id
deviceID              | string            |  是  | 设备唯一码
deviceName            | string            |  是  | 设备名称
deviceOS              | string            |  是  | 设备系统
sourceType            | int               |  是  | 来源类型：1-ukex；2-ukexpay

响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data = "绑定成功"/"绑定失败"

### 删除设备绑定
> <a id="RemoveBind">Api/Device/RemoveBind</a>
> 作者 ：吴友鹏

接口调用说明：删除（标记位）用户绑定用户和设备的接口

是否携带token：是

请求方式：POST

请求数据：

字段                    | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
uid                    | string            |  是  | 用户id
deviceID              | string            |  是  | 设备唯一码

响应status:
- 0 ：status 失败
- 1 ：status 成功

响应数据：data = "移除成功"/"移除失败"

### 检测设备绑定
> <a id="CheckBind">Api/Device/CheckBind</a>
> 作者 ：吴友鹏

接口调用说明：检测对应设备是否绑定此用户

是否携带token：是

请求方式：POST

请求数据：

字段                    | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
uid                    | string            |  是  | 用户id
deviceID              | string            |  是  | 设备唯一码

响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data 
字段                   | 数据类型           | 必填 | 备注
-----------------------|-------------------|------|--------------------------
status                | string            |  是  | 用户组id，1：主账户；2：普通用户

### 用户权限信息
> <a id="Auth">Api/User/Auth</a>
> 作者 ：吴友鹏

接口调用说明：修改，验证用户信息

是否携带token：是

请求方式：POST

修改示例json：
{
    "column":["is_reauth","is_pass_aml"],
    "value":[1,1],
    "method":1
}
验证示例json：
{
    "column":["is_reauth","is_pass_aml"],
    "method":2
}

请求数据：

字段                    | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
uid                    | string            |  是  | 用户id
column                 | List<T>            |  是  | 字段名
value                  | List<T>            |  否  | 对应字段的值
method              | int                |  是  | 1-修改权限数据；2-查找验证权限数据

T -> column:
字段                   | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
"is_allow_login"       | string            |  是  | 对应权限字段名

T -> value:
值                     | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
1                      | int               |  是  | column对应字段值

修改响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data = "操作成功"/"操作失败"

验证响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data = {"is_reauth":"1","is_pass_aml":"0"}

### 用户信息
> <a id="Info">Api/User/Info</a>
> 作者 ：吴友鹏

接口调用说明：修改，验证用户信息


是否携带token：是

修改示例json：
{
    "column":["realname","country"],
    "value":["friendwu","CN"],
    "method":1
}
验证示例json：
{
    "column":["realname","country"],
    "method":2
}

请求方式：POST

请求数据：

字段                    | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
uid                    | string            |  是  | 用户id
column                 | List<T>            |  是  | 新增字段名
value                  | List<T>            |  否  | 对应字段的值
method                 | int                |  是  | 1-修改数据；2-查找数据

T -> column:
字段                   | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
"mobile"               | string            |  是  | 对应权限字段名

T -> value:
值                     | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
"13628379881"          | string            |  是  | column对应字段值

修改响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data = "操作成功"/"操作失败"

验证响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data = {"realname":"friendwu","country":"CN"}


### 用户真实信息
> <a id="RealInfo">Api/User/RealInfo</a>
> 作者 ：吴友鹏

接口调用说明：修改，验证用户信息


是否携带token：是

请求方式：POST


修改示例json：
{
    "column":["first_name","last_name"],
    "value":["wu","youpeng"],
    "method":1
}
验证示例json：
{
    "column":["first_name","last_name"],
    "method":2
}

请求数据：

字段                    | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
uid                    | string            |  是  | 用户id
column                 | List<T>            |  是  | 新增字段名
value                  | List<T>            |  是  | 对应字段的值
method              | int                |  是  | 1-修改数据；2-查找数据

T -> column:
字段                   | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
"first_name"           | string            |  是  | 对应权限字段名

T -> value:
值                     | 数据类型           | 必填 | 描述
-----------------------|-------------------|------|--------------------------
"wu"                   | string            |  是  | column对应字段值

响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data = "操作成功"/"操作失败"

验证响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data = {"first_name":"wu","last_name":"youpeng"}

### 设置主设备
> <a id="SetMaster">Api/Device/SetMaster</a>
> 作者 ：吴友鹏

接口调用说明：设置用户设备为主设备

是否携带token：是

请求方式：POST

请求数据：

字段                    | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
status                 | int               |  是  | 1.主设备 0.普通设备
uid                    | string            |  是  | 用户id
deviceID               | string            |  是  | 被设置的设备id


响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data = "设置成功"/"设置失败"

### 资产新增
> <a id="Add">~~Api/Balance/Add~~</a>
> 作者 ：吴友鹏

接口调用说明：用户资产增加包括（充值，转入）

是否携带token：是

请求方式：POST

请求数据：

字段                    | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
uid                    | string            |  是  | 用户id
type                   | int               |  是  | 1-可用资产 2-冻结资产
coinId                 | string            |  是  | 充值货币id
quantityNum            | string            |  是  | 充值到账数量


响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data = "充值成功"/"充值失败"


### 资产减少
> <a id="Reduce">~~Api/Balance/Reduce~~</a>
> 作者 ：吴友鹏

接口调用说明：用户资产减少包括（提现，转出）

是否携带token：是

请求方式：POST

请求数据：

字段                    | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
uid                    | string            |  是  | 用户id
type                   | int               |  是  | 1-可用资产 2-冻结资产
coinId                 | string            |  是  | 充值货币id
quantityNum            | string            |  是  | 充值减少数量


响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data = "操作成功"/"操作失败"


### 资产配置
> <a id="Allocation">~~Api/Balance/Allocation~~</a>
> 作者 ：吴友鹏

接口调用说明：用户自己的资产配置转移，可用->冻结；冻结->可用

是否携带token：是

请求方式：POST

请求数据：

字段                    | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
uid                    | string            |  是  | 用户id
coinId                 | string            |  是  | 被配置的货币id
assetsNum              | string            |  是  | 被配置资产数量 
method              | int               |  是  | 1-冻结资产；2-解冻资产

响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data = "操作成功"/"操作失败"

### 资产变更
> <a id="BalanceChange">Api/Balance/Change</a>
> 作者 ：吴友鹏

接口调用说明：资产变更

是否携带token：是

请求方式：POST

请求数据：
字段                    | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
list                    | List<T>          | 是 
note                    | string           | 否 | 变动描述

T -> list:
字段                    | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
uid                    | int               |  是  | 用户id
typeID                 | int               |  是  | 业务关联id
type                   | int               |  是  | 业务类型 // 1,	// 委托交易买入// 2,	// 委托交易卖出// 3,	// 交易撮合（用户的可用和冻结都会变动）// 4,	// 交易推荐奖励收益// 5,	// 交易手续费收入（平台账户收入）// 6,	// UKEX visa法币充值// 7,	// 银行汇款法币充值// 8,	// 第三方支付法币充值// 9,	// UKEX visa法币提现// 10,	// 银行汇款法币提现// 11,	// 银行汇款法币提现手续费（平台账户收入）// 12,	// 第三方支付法币充值手续费（平台账户收入）// 13,	// OTC资金冻结// 14,	// OTC付款// 15,	// OTC收款// 16,	// 根据地址站内转出// 17,	// 根据地址站内转入// 18,	// 根据地址站内转出手续费（平台账户收入）// 19,	// 平台普通站内转出// 20,	// 平台普通站内转入// 21,	// 平台普通站内转出手续费（平台账户收入）// 22,	// 站外提币// 23,	// 站外提币手续费（平台账户收入）// 24,	// 站外充币// 25,	// 平台赠送// 26,	// UKEX visa开卡收费（用户支出，平台账户收入）// 27,	// UKEX visa充值手续费（平台账户收入）// 28,	// UKEX visa提现手续费（平台账户收入）// 29,	// 项目方归集// 30,	// 平台系统充值// 31,	// 冻结释放
coinid                 | int               |  是  | 变更货币id
usable                 | string            |  否  | 可用数量
fronzen                | string            |  否  | 冻结数量
lockup                 | string            |  否  | 锁仓数量

响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data = "成功"/"失败"

### 资产配置
> <a id="AddCoin">Api/Coin/Add</a>
> 作者 ：吴友鹏

接口调用说明：新增币种配置

是否携带token：是

请求方式：POST

请求数据：

字段                    | 数据类型          | 必填 | 备注
-----------------------|-------------------|------|--------------------------
name                    | string            |  是  | 币种名称
type                 | string            |  是  | 币种类型： rgb (认购币) qbb(钱包币) cxb(创新币) 
title              | string            |  是  | 币种中文名称  

响应status:

- 0 ：status 失败
- 1 ：status 成功

响应数据：data = "操作成功"/"操作失败"

