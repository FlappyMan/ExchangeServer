'''
@Author: your name
@Date: 2020-06-11 02:57:28
@LastEditTime: 2020-06-12 00:18:15
@LastEditors: Please set LastEditors
@Description: In User Settings Edit
@FilePath: /ukexserver/TransferPrograme/test.py
'''
import datetime
import json
import re
import sys
import time
import uuid
import pytz

def utc_to_local(utc_time_str, utc_format='%Y-%m-%d'):
    local_tz = pytz.timezone('UTC')
    local_format = "%Y-%m-%d"
    utc_dt = datetime.datetime.strptime(utc_time_str, utc_format)
    local_dt = utc_dt.replace(tzinfo=pytz.utc).astimezone(local_tz)
    return int(time.mktime(local_dt.timetuple()))
    
if __name__ == '__main__':
    reload(sys)
    sys.setdefaultencoding('utf-8')

    # t = datetime.datetime.strptime("1991-10-11", '%Y-%m-%d')
    # t = t + datetime.timedelta(hours=-7)
    # oTime = int(time.mktime(t.timetuple()))
    # print(oTime)
    value = utc_to_local("1991-10-11")
    value = value - 28800
    print(value)
    # print(int(time.mktime(value.timetuple())))

    # t = datetime.datetime.strptime("1974-11-07", '%Y-%m-%d')
    # t = t + datetime.timedelta(hours=-7)
    # oTime = int(time.mktime(t.timetuple()))
    # print(oTime)
    value = utc_to_local("1974-11-07")
    value = value - 28800
    print(value)
    
    value = utc_to_local("1970-01-01")
    value = value - 28800
    print(value)


