一、Jmeter测试文件说明

1)Jmeter目录下的.jmx文件为jmeter测试工具的测试文件；

2)以HTTP API开头的.jmx文件是用来测试前端发送各种http请求到FrontMarket，FrontMarket转发给Backtrade,
BackTrade再把请求发送至UserCenter,UserCenter收到请求并处理完毕后,按原路返回Response包到Jmeter.
各http请求具体功能如下，
    HTTP API_Balance_Add.jmx            资产新增
    HTTP API_Balance_Allocation.jmx     资产配置
    HTTP API_Balance_FrozenTrans.jmx    资产冻结
    HTTP API_Balance_Reduce.jmx         资产减少
    HTTP API_Device_AddBindDevice.jmx   设备绑定
    HTTP API_Device_CheckBind.jmx       检测设备绑定
    HTTP API_Device_RemoveBind.jmx      删除设备绑定
    HTTP API_Device_SetMaster.jmx       设置主设备
    HTTP API_Password_Pay.jmx           交易密码
    HTTP API_Password_Update.jmx        更新登陆密码
    HTTP API_User_Auth.jmx              用户权限信息
    HTTP API_User_Info.jmx              用户信息
    HTTP API_User_RealInfo.jmx          用户真实信息
    HTTP Api_UserCenter_Login.jmx       登陆
    HTTP Api_UserCenter_Regist.jmx      新增用户账号

(更具体的文档说明见Document目录下“用户中心API说明文档“)

3)HTTP Uptrade_test.jmx是用来测试前端发送Uptrade(交易)的http请求到FrontTrade,FrontTrade转发给Backtrade,
BackTrade再把请求发送至UserCenter,UserCenter收到请求并处理完毕后，按原路返回Response包到Jmeter测试客户端；
