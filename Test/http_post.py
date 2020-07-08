import requests,json
 
if __name__ == '__main__':
    i = 0
    while i < 100:
        i = i+1
        url_json = 'http://192.168.1.20:9000/api/private/uptrade'
        data_json = json.dumps({"main_coin":5,"marketid":1,"num":3,"paypassword":"456","price":2,"token":"123","type":4})
        header={"token":"asldkfoiwefnasdlkfjqwe"}
        r_json = requests.post(url_json,data_json,headers=header)
        print(r_json)
