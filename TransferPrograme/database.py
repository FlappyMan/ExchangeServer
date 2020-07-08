#!/usr/bin/env python
#!-*-coding:utf-8 -*-
#!@Author:FriendWu
import MySQLdb.cursors
from config import Config
import os
class Database(object):
    dbConfid = None
    # ukex
    ukex_conn = None
    ukex_cursor = None
    # account
    accountDB_conn = None
    accountDB_cursor = None
    # assets
    assetsDB_conn = None
    assetsDB_cursor = None
    # SA
    SADB_conn = None
    SADB_cursor = None

    def __init__(self):
        self.dbConfid = Config()
        self.getUkexDBConnect()
        self.getAccountDBConnect()
        self.getAssetsDBConnect()
        self.getSADBConnect()

    def getUkexDBConnect(self):
        try:
            if self.ukex_conn is None:
                self.ukex_conn   = MySQLdb.connect(host=self.dbConfid.Ukex_dbAddress,\
                                              user=self.dbConfid.Ukex_userName,\
                                              passwd=self.dbConfid.Ukex_password,\
                                              db=self.dbConfid.Ukex_dbName, \
                                              cursorclass=MySQLdb.cursors.DictCursor)
                self.ukex_cursor = self.ukex_conn.cursor()
                self.ukex_conn.set_character_set('utf8')
                self.ukex_cursor.execute('SET NAMES utf8;')
                self.ukex_cursor.execute('SET CHARACTER SET utf8;')
                self.ukex_cursor.execute('SET character_set_connection=utf8;')
                print('connect to database =' + self.dbConfid.Ukex_dbAddress)
        except:
            import traceback
            traceback.print_exc(file=open('traceback_INFO.txt', 'a+'))
            traceback.print_exc()

    def getAccountDBConnect(self):
        try:
            if self.accountDB_conn is None:
                self.accountDB_conn   = MySQLdb.connect(host=self.dbConfid.accountDB_dbAddress,\
                                              user=self.dbConfid.accountDB_userName,\
                                              passwd=self.dbConfid.accountDB_password,\
                                              db=self.dbConfid.accountDB_dbName, \
                                              cursorclass=MySQLdb.cursors.DictCursor)
                self.accountDB_cursor = self.accountDB_conn.cursor()
                self.accountDB_conn.set_character_set('utf8')
                self.accountDB_cursor.execute('SET NAMES utf8;')
                self.accountDB_cursor.execute('SET CHARACTER SET utf8;')
                self.accountDB_cursor.execute('SET character_set_connection=utf8;')
                print('connect to database =' + self.dbConfid.accountDB_dbAddress)
        except:
            import traceback
            traceback.print_exc(file=open('traceback_INFO.txt', 'a+'))
            traceback.print_exc()
    def getAssetsDBConnect(self):
        try:
            if self.assetsDB_conn is None:
                self.assetsDB_conn   = MySQLdb.connect(host=self.dbConfid.assetsDB_dbAddress,\
                                              user=self.dbConfid.assetsDB_userName,\
                                              passwd=self.dbConfid.assetsDB_password,\
                                              db=self.dbConfid.assetsDB_dbName, \
                                              cursorclass=MySQLdb.cursors.DictCursor)
                self.assetsDB_cursor = self.assetsDB_conn.cursor()
                self.assetsDB_conn.set_character_set('utf8')
                self.assetsDB_cursor.execute('SET NAMES utf8;')
                self.assetsDB_cursor.execute('SET CHARACTER SET utf8;')
                self.assetsDB_cursor.execute('SET character_set_connection=utf8;')
                print('connect to database =' + self.dbConfid.assetsDB_dbAddress)
        except:
            import traceback
            traceback.print_exc(file=open('traceback_INFO.txt', 'a+'))
            traceback.print_exc()

    def getSADBConnect(self):
        try:
            if self.SADB_conn is None:
                self.SADB_conn   = MySQLdb.connect(host=self.dbConfid.SADB_dbAddress,\
                                              user=self.dbConfid.SADB_userName,\
                                              passwd=self.dbConfid.SADB_password,\
                                              db=self.dbConfid.SADB_dbName, \
                                              cursorclass=MySQLdb.cursors.DictCursor)
                self.SADB_cursor = self.SADB_conn.cursor()
                self.SADB_conn.set_character_set('utf8')
                self.SADB_cursor.execute('SET NAMES utf8;')
                self.SADB_cursor.execute('SET CHARACTER SET utf8;')
                self.SADB_cursor.execute('SET character_set_connection=utf8;')
                print('connect to database =' + self.dbConfid.SADB_dbAddress)
        except:
            import traceback
            traceback.print_exc(file=open('traceback_INFO.txt', 'a+'))
            traceback.print_exc()

    def resetConnect(self,cursor,conn):
        cursor.close()
        conn.close()
        conn = None
        cursor = None

    def selectSQL(self,cursor, columns , tableName):
        baseSql = 'select ' + columns + ' from ' + tableName
        print (baseSql)
        try:
            cursor.execute(baseSql)
            data = cursor.fetchall()
            return data
        except:
            import traceback
            file = open('traceback_INFO.txt', 'a+')
            file.write(baseSql)
            file.close()
            traceback.print_exc(file=open('traceback_INFO.txt','a+'))
            traceback.print_exc()

    def excuteSQL(self,cursor, sql):
        baseSql = sql
        print(baseSql)
        try:
            cursor.execute(baseSql)
            data = cursor.fetchall()
            if cursor == self.ukex_cursor:
                self.ukex_conn.commit()
            # elif cursor == self.ukexpay_cursor:
            #     self.ukexpay_conn.commit()
            elif cursor == self.assetsDB_cursor:
                self.assetsDB_conn.commit()
            elif cursor == self.SADB_cursor:
                self.SADB_conn.commit()
            else:
                self.accountDB_conn.commit()
            return data
        except:
            import traceback
            file = open('traceback_INFO.txt', 'a+')
            file.write(baseSql)
            file.close()
            traceback.print_exc(file=open('traceback_INFO.txt', 'a+'))
            traceback.print_exc()

    def inserSQL(self,cursor, tableName):
        baseSql = 'INSERT INTO ' + tableName
        # print(baseSql)
        try:
            cursor.execute(baseSql)
            if cursor == self.ukex_cursor:
                self.ukex_conn.commit()
            # elif cursor == self.ukexpay_cursor:
            #     self.ukexpay_conn.commit()
            elif cursor == self.assetsDB_cursor:
                self.assetsDB_conn.commit()
            elif cursor == self.SADB_cursor:
                self.SADB_conn.commit()
            else:
                self.accountDB_conn.commit()
        except:
            import traceback
            file = open('traceback_INFO.txt', 'a+')
            file.write(baseSql)
            file.close()
            traceback.print_exc(file=open('traceback_INFO.txt', 'a+'))
            traceback.print_exc()

    def update(self,cursor,sql):
        baseSql = 'UPDATE ' + sql
        print(baseSql)
        try:
            cursor.execute(baseSql)
            if cursor == self.ukex_cursor:
                self.ukex_conn.commit()
            # elif cursor == self.ukexpay_cursor:
            #     self.ukexpay_conn.commit()
            elif cursor == self.assetsDB_cursor:
                self.assetsDB_conn.commit()
            elif cursor == self.SADB_cursor:
                self.SADB_conn.commit()
            else:
                self.accountDB_conn.commit()
        except:
            import traceback
            file = open('traceback_INFO.txt', 'a+')
            file.write(baseSql)
            file.close()
            traceback.print_exc(file=open('traceback_INFO.txt', 'a+'))
            traceback.print_exc()