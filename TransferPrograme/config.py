#!/usr/bin/env python
#!-*-coding:utf-8 -*-
#!@Author:FriendWu
import ConfigParser

class Config(object):
    # Ukex 数据库变量信息
    Ukex_dbAddress = ''
    Ukex_dbPort = ''
    Ukex_userName = ''
    Ukex_password = ''
    Ukex_dbName = ''

    # Ukex_Pay 数据库变量信息
    UkexPay_dbAddress = ''
    UkexPay_dbPort = ''
    UkexPay_userName = ''
    UkexPay_password = ''
    UkexPay_dbName = ''

    # acccount 数据库变量信息
    accountDB_dbAddress = ''
    accountDB_dbPort = ''
    accountDB_userName = ''
    accountDB_password = ''
    accountDB_dbName = ''

    # assets 数据库变量信息
    assetsDB_dbAddress = ''
    assetsDB_dbPort = ''
    assetsDB_userName = ''
    assetsDB_password = ''
    assetsDB_dbName = ''

    # SA 数据库变量信息
    SADB_dbAddress = ''
    SADB_dbPort = ''
    SADB_userName = ''
    SADB_password = ''
    SADB_dbName = ''

    def __init__(self):
        self.getConfigInfo()

    def getConfigInfo(self):
        config = ConfigParser.ConfigParser()
        config.read("./config.ini")

        Ukexdatabase = config.get('UKEX Database iniPath', 'filePath')
        accountDBdatabase = config.get('account Database iniPath', 'filePath')
        assetsDBdatabase = config.get('assets Database iniPath', 'filePath')
        SADBdatabase = config.get('SA Database iniPath', 'filePath')

        # 读取Ukex的数据库
        UkexConfigDB = ConfigParser.ConfigParser()
        UkexConfigDB.read(Ukexdatabase)
        self.Ukex_dbAddress = UkexConfigDB.get('DBADDRESS', 'DBADDRESS')
        self.Ukex_dbPort = UkexConfigDB.get('PORT', 'PORT')
        self.Ukex_userName = UkexConfigDB.get('USERNAME', 'USERNAME')
        self.Ukex_password = UkexConfigDB.get('PASSWORD', 'PASSWORD')
        self.Ukex_dbName = UkexConfigDB.get('DBNAME', 'DBNAME')

        # 读取new的数据库
        accountDBConfigDB = ConfigParser.ConfigParser()
        accountDBConfigDB.read(accountDBdatabase)
        self.accountDB_dbAddress = accountDBConfigDB.get('DBADDRESS', 'DBADDRESS')
        self.accountDB_dbPort = accountDBConfigDB.get('PORT', 'PORT')
        self.accountDB_userName = accountDBConfigDB.get('USERNAME', 'USERNAME')
        self.accountDB_password = accountDBConfigDB.get('PASSWORD', 'PASSWORD')
        self.accountDB_dbName = accountDBConfigDB.get('DBNAME', 'DBNAME')

        # 读取new的数据库
        assetsDBConfigDB = ConfigParser.ConfigParser()
        assetsDBConfigDB.read(assetsDBdatabase)
        self.assetsDB_dbAddress = assetsDBConfigDB.get('DBADDRESS', 'DBADDRESS')
        self.assetsDB_dbPort = assetsDBConfigDB.get('PORT', 'PORT')
        self.assetsDB_userName = assetsDBConfigDB.get('USERNAME', 'USERNAME')
        self.assetsDB_password = assetsDBConfigDB.get('PASSWORD', 'PASSWORD')
        self.assetsDB_dbName = assetsDBConfigDB.get('DBNAME', 'DBNAME')

        # 读取new的数据库
        SADBConfigDB = ConfigParser.ConfigParser()
        SADBConfigDB.read(SADBdatabase)
        self.SADB_dbAddress = SADBConfigDB.get('DBADDRESS', 'DBADDRESS')
        self.SADB_dbPort = SADBConfigDB.get('PORT', 'PORT')
        self.SADB_userName = SADBConfigDB.get('USERNAME', 'USERNAME')
        self.SADB_password = SADBConfigDB.get('PASSWORD', 'PASSWORD')
        self.SADB_dbName = SADBConfigDB.get('DBNAME', 'DBNAME')
