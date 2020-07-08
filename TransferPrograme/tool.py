#!/usr/bin/env python
#!-*-coding:utf-8 -*-
#!@Author:FriendWu

class Tool(object):
    def __init__(self):
        print ('aaa')
    def stringToJsonObj(self, strData):
        if len(strData) == 0:
            return {}
        return json.loads(strData)