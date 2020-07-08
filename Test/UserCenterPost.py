import requests,json

if __name__ == '__main__':
    headers = {
        # "token": "e5e95824800002b908ad7a32094fd70e28743c877ab4e5ffe"
        }
        

    #################regist
    # url_json = 'http://127.0.0.1:9503/Api/UserCenter/Regist'
    # data_json = json.dumps(
    #     {
    #         'deviceID':'FriendWu-iphone',
    #         'userAccount':'Fiona2039@qq.com',
    #         'password':"123456",
    #         'sourceType':1
    #     })
    # ##############login
    url_json = 'http://127.0.0.1:9503/Api/UserCenter/Login'
    data_json = json.dumps(
        {
            'deviceID':'FriendWu-iphone',
            'userAccount':'Fiona2038@qq.com',
            'password':"123456",
            'sourceType':1
        })

    # ###########add banlance
    # url_json = 'http://127.0.0.1:9503/Api/Balance/Add'
    # data_json = json.dumps(
    #     {
    #         'uid':'11152',
    #         'type':2,
    #         'coinId':"10000",
    #         'quantityNum':'1.00000000100001'
    #     })

    # ##########/Api/Device/AddBindDevice
    # url_json = 'http://127.0.0.1:9503/Api/Device/AddBindDevice'
    # data_json = json.dumps(
    #     {
    #         'uid':'11152',
    #         'deviceID':"wuyoupeng_iphone_id",
    #         'deviceName':"wuyoupeng's iphone",
    #         'deviceOS':'iphone7s',
    #         'sourceType':1
    #     })
    # ##########/Api/Balance/Allocation
    # url_json = 'http://127.0.0.1:9503/Api/Balance/Allocation'
    # data_json = json.dumps(
    #     {
    #         'uid':'11152',
    #         'coinId':"98",
    #         'assetsNum':"0.5",
    #         'deviceOS':'iphone7s',
    #         'method':2
    #     })
    ############/Api/User/Auth
    # url_json = 'http://127.0.0.1:9503/Api/User/Auth'
    # data_json = json.dumps(
    #     {
    #         'uid':'11152',
    #         'column':['paypassword_status','user_status'],
    #         'value':['1','1'],
    #         'method':1
    # })

    # ###########/Api/Device/CheckBind
    # url_json = 'http://127.0.0.1:9503/Api/Device/CheckBind'
    # data_json = json.dumps({
    #         'uid':'11152',
    #         'deviceID':'wuyoupeng_iphone_id',
    #     })


    ############/Api/Device/CheckBind
    # url_json = 'http://127.0.0.1:9503/Api/Device/SetMaster'
    # data_json = json.dumps({
    #         'status':1,
    #         'uid':'11152',
    #         'deviceID':'wuyoupeng_iphone_id'
    #     })

    ###### /Api/User/Info
    # url_json = 'http://127.0.0.1:9503/Api/User/Info'
    # data_json = json.dumps(
    #     {
    #         'uid':'11100',
    #         'column':['mobile','id_type'],
    #         'value':['13628379881','1'],
    #         'method':1
    # })

    # url_json = 'http://127.0.0.1:9503/Api/Password/Pay'
    # data_json = json.dumps(
    #     {
    #         'uid':'11100',
    #         'paypassword':'1234567',
    #         'method':1
    # })

    # url_json = 'http://127.0.0.1:9503/Api/User/RealInfo'
    # data_json = json.dumps(
    #     {
    #         'uid':'11100',
    #         'column':['first_name','last_name'],
    #         'value':['wu','youpeng'],
    #         'method':1
    # })

    # url_json = 'http://127.0.0.1:9503/Api/Balance/Reduce'
    # data_json = json.dumps(
    #     {
    #         'uid':'11152',
    #         'coinId':'98',
    #         'quantityNum':'0.1112',
    #         'type':2
    # })

    # url_json = 'http://127.0.0.1:9503/Api/Device/RemoveBind'
    # data_json = json.dumps(
    #     {
    #         'uid':'11152',
    #         'deviceID':'wuyoupeng_iphone_id'
    # })

    # url_json = 'http://127.0.0.1:9503/Api/Password/Update'
    # data_json = json.dumps(
    #     {
    #         'userAccount':'Fiona2009@qq.com',
    #         'password':'1234567'
    # })
    
    # url_json = 'http://127.0.0.1:9503/Api/Coin/Add'
    # data_json = json.dumps(
    #     {
    #         'name':'wuyoupeng1',
    #         'type':'rgb',
    #         'title':'TestCoin'
    # })

    r_json = requests.post(url_json,data_json,headers=headers)
    print(r_json)
