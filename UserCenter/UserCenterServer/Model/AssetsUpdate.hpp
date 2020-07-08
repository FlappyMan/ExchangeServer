/*
 * @Author: your name
 * @Date: 2020-06-08 18:55:02
 * @LastEditTime: 2020-06-17 01:19:24
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /ukexserver/UserCenter/UserCenterServer/Model/AssetsUpdate.hpp
 */ 
#ifndef _ASSETSUPDATE_MODEL_HEADER_
#define _ASSETSUPDATE_MODEL_HEADER_

#include "../global.h"
#include  "../Manager/DispatchManager.h"
class COrderInfo
{
public:
    COrderInfo &operator=(const COrderInfo &obj)
    {
        if (this != &obj)
        {
            this->uid = obj.uid;
            this->type_id = obj.type_id;
            this->type = obj.type;
            this->coinid = obj.coinid;
            this->usable = obj.usable;
            this->fronzen = obj.fronzen;
            this->lockup = obj.lockup;
        }
        return *this;
    }
    COrderInfo &operator=(UPassetsupdate::List &pkg)
    {
        this->uid = pkg.uid();
        this->type_id = pkg.type_id();
        this->type = pkg.type();
        this->coinid = pkg.coinid();
        this->usable = pkg.usable();
        if (this->usable.length() == 0)
        {
            this->usable = "0.0";
        }
        
        this->fronzen = pkg.fronzen();
        if (this->fronzen.length() == 0)
        {
            this->fronzen = "0.0";
        }
        this->lockup = pkg.lockup();
        if (this->lockup.length() == 0)
        {
            this->lockup = "0.0";
        }
        return *this;
    }

public:
    uint64_t uid;    // 资产变更用户id
    uint32_t type_id; // 业务id
    uint32_t type;   // 业务类型 1 委托交易买入// 2,委托交易卖出// 3,交易撮合（用户的可用和冻结都会变动）// 4,交易推荐奖励收益// 5,交易手续费收入（平台账户收入）// 6,UKEX visa法币充值// 7,银行汇款法币充值// 8,第三方支付法币充值// 9,UKEX visa法币提现// 10,银行汇款法币提现// 11,银行汇款法币提现手续费（平台账户收入）// 12,第三方支付法币充值手续费（平台账户收入）// 13,OTC资金冻结// 14,OTC付款// 15,OTC收款// 16,根据地址站内转出// 17,根据地址站内转入// 18,根据地址站内转出手续费（平台账户收入）// 19,平台普通站内转出// 20,平台普通站内转入// 21,平台普通站内转出手续费（平台账户收入）// 22,站外提币// 23,站外提币手续费（平台账户收入）// 24,站外充币// 25,平台赠送// 26,UKEX visa开卡收费（用户支出，平台账户收入）// 27,UKEX visa充值手续费（平台账户收入）// 28,UKEX visa提现手续费（平台账户收入）// 29,项目方归集// 30,平台系统充值// 31,冻结释放    
    uint32_t coinid; // 币种id
    string usable;   // 可用资产
    string fronzen;  // 冻结资产
    string lockup;   // 锁仓数量

    /*
    * 临时保存部分
    * */
    string gross_usable;
    string gross_frozen;
    string gross_sum;

    string remaining_usable;
    string remaining_frozen;
    string remaining_sum;
};

class CAssetsUpdate
{
public:
    CAssetsUpdate &operator=(const CAssetsUpdate &obj)
    {
        if (this != &obj)
        {
            this->uid = obj.uid;
            this->paypassword = obj.paypassword;
            this->opertype = obj.opertype;
            this->operid = obj.operid;
            this->list = obj.list;
        }
        return *this;
    }
    CAssetsUpdate &operator=(UPassetsupdate *pkg)
    {
        this->uid = dynamic_cast<UPassetsupdate *>(pkg)->uid();
        this->paypassword = dynamic_cast<UPassetsupdate *>(pkg)->paypassword();
        this->opertype = dynamic_cast<UPassetsupdate *>(pkg)->opertype();
        this->operid = dynamic_cast<UPassetsupdate *>(pkg)->operid();
        int iSize = dynamic_cast<UPassetsupdate *>(pkg)->list_size();
        for (int iLoop = 0; iLoop < iSize; iLoop++)
        {
            COrderInfo* pModel = new COrderInfo;
            UPassetsupdate::List obj = dynamic_cast<UPassetsupdate *>(pkg)->list(iLoop);
            pModel->uid = obj.uid();
            pModel->type_id = obj.type_id();
            pModel->type = obj.type();
            pModel->coinid = obj.coinid();
            pModel->usable = obj.usable();
            pModel->fronzen = obj.fronzen();
            pModel->lockup = obj.lockup();
            if (pModel->usable.length() == 0)
            {
                pModel->usable = "0.0";
            }

            if (pModel->fronzen.length() == 0)
            {
                pModel->fronzen = "0.0";
            }
            
            if (pModel->lockup.length() == 0)
            {
                pModel->lockup = "0.0";
            }

            this->list.push_back(pModel);
        }

        return *this;
    }
public:
    void businessOper(vector<COrderInfo*> &vecOrder,int& iStatus,string& retMsg,string& vip_level,int marketID)
    {
        vector<COrderInfo*> vec;
        AssetsUpdate(vecOrder,vec,false);
        if (vec.size() != vecOrder.size())
        {
            vector<COrderInfo*> vec2;
            AssetsUpdate(vec,vec2, true);
            iStatus = 0;
            retMsg = "资产修改失败";
        }
        else
        {
            // 资产修改执行成功
            recordToFinancialAssetsLog(vec, marketID);
            retMsg = getRespString(vip_level,marketID);
            iStatus = 1;
        }
        int iVecSize = vecOrder.size();
        for (size_t i = 0; i < iVecSize; i++)
        {
            COrderInfo* p = vecOrder[i];
            delete p;
        }
    }
    void businessOper(vector<COrderInfo*> &vecOrder,int& iStatus,string& retMsg,string note)
    {
        vector<COrderInfo*> vec;
        AssetsUpdate(vecOrder,vec,false);
        if (vec.size() != vecOrder.size())
        {
            vector<COrderInfo*> vec2;
            AssetsUpdate(vec,vec2, true);
            iStatus = 0;
            retMsg = "资产修改失败";
        }
        else
        {
            // 资产修改执行成功
            recordToFinancialAssetsLog(vec,note);
            retMsg = "资产修改成功";
            iStatus = 1;
        }
        int iVecSize = vecOrder.size();
        for (size_t i = 0; i < iVecSize; i++)
        {
            COrderInfo* p = vecOrder[i];
            delete p;
        }
    }
private:
    void AssetsUpdate(vector<COrderInfo*> &vecOrder,vector<COrderInfo*> &vec, bool bIsReset)
    {
        int iVecSize = vecOrder.size();
        for (size_t i = 0; i < iVecSize; i++)
        {
            COrderInfo* pModel = vecOrder[i];
            string tableName = " user_assets_" + std::to_string(pModel->coinid) + " ";
            string sqlBase = "select usable,freeze,lock_up from " + tableName + " where user_id = \"";
            CDispatchManager::g_DBAssets.Sql(sqlBase, false, true);
            string uid = std::to_string(pModel->uid);
            CDispatchManager::g_DBAssets.Sql(uid, true, false);
            CDispatchManager::g_DBAssets.Sql("\";", false, false);
            DEBUG_INFO(CDispatchManager::g_DBAssets.getSql());
            if (CDispatchManager::g_DBAssets.Select())
            {
                if (CDispatchManager::g_DBAssets.NextRow())
                {
                    string usable, freeze,lockup, total;
                    CDispatchManager::g_DBAssets.Value(0, usable);
                    CDispatchManager::g_DBAssets.Value(1, freeze);
                    CDispatchManager::g_DBAssets.Value(1, lockup);
                    if (usable.length() == 0)
                        usable = "0.0";
                    if (freeze.length() == 0)
                        freeze = "0.0";
                    if (lockup.length() == 0)
                        lockup = "0.0";
                    pModel->gross_usable = usable;
                    pModel->gross_frozen = freeze;
                    pModel->gross_sum = CDispatchManager::addLongDouble(usable, freeze);
                    string strBalanceUsable = "0.0";
                    if (CDispatchManager::IsBigThanZero(pModel->usable) == true)
                    {
                        if (bIsReset == false)
                            strBalanceUsable = CDispatchManager::addLongDouble(pModel->usable, usable);
                        else
                            strBalanceUsable = CDispatchManager::minusLongDouble(usable, pModel->usable);
                    }
                    else
                    {
                        while (pModel->usable.data()[0] == '-')
                        {
                            pModel->usable.erase(0, 1);
                        }
                        if (bIsReset == false)
                            strBalanceUsable = CDispatchManager::minusLongDouble(usable, pModel->usable);
                        else
                            strBalanceUsable = CDispatchManager::addLongDouble(usable, pModel->usable);

                        if ("-1" == strBalanceUsable)
                        {
                            // 金额错误
                            break;
                        }
                    }

                    string strBalanceFreeze = "0.0";
                    if (CDispatchManager::IsBigThanZero(pModel->fronzen) == true)
                    {
                        if (bIsReset == false)
                            strBalanceFreeze = CDispatchManager::addLongDouble(pModel->fronzen, freeze);
                        else
                            strBalanceFreeze = CDispatchManager::minusLongDouble(freeze, pModel->fronzen);
                    }
                    else
                    {
                        while (pModel->fronzen.data()[0] == '-')
                        {
                            pModel->fronzen.erase(0, 1);
                        }
                        if (bIsReset == false)
                            strBalanceFreeze = CDispatchManager::minusLongDouble(freeze, pModel->fronzen);
                        else
                            strBalanceFreeze = CDispatchManager::addLongDouble(freeze, pModel->fronzen);
                        if ("-1" == strBalanceFreeze)
                        {
                            // 金额错误
                            break;
                        }
                    }
                    
                    string strLockUp = "0.0";
                    if (CDispatchManager::IsBigThanZero(pModel->lockup) == true)
                    {
                        if (bIsReset == false)
                            strLockUp = CDispatchManager::addLongDouble(pModel->lockup, lockup);
                        else
                            strLockUp = CDispatchManager::minusLongDouble(lockup, pModel->lockup);
                    }
                    else
                    {
                        while (pModel->lockup.data()[0] == '-')
                        {
                            pModel->lockup.erase(0, 1);
                        }
                        if (bIsReset == false)
                            strLockUp = CDispatchManager::minusLongDouble(lockup, pModel->lockup);
                        else
                            strLockUp = CDispatchManager::addLongDouble(lockup, pModel->lockup);
                        if ("-1" == strLockUp)
                        {
                            // 金额错误
                            break;
                        }
                    }
                    pModel->remaining_usable = strBalanceUsable;
                    pModel->remaining_frozen = strBalanceFreeze;
                    pModel->remaining_sum = CDispatchManager::addLongDouble(strBalanceUsable, strBalanceFreeze);
                    CDispatchManager::g_DBAssets.Sql("UPDATE ", false, true);
                    CDispatchManager::g_DBAssets.Sql(tableName, true, false);
                    CDispatchManager::g_DBAssets.Sql(" SET usable = \"", false, false);
                    CDispatchManager::g_DBAssets.Sql(strBalanceUsable, true, false);
                    CDispatchManager::g_DBAssets.Sql("\", freeze = \"", false, false);
                    CDispatchManager::g_DBAssets.Sql(strBalanceFreeze, true, false);
                    CDispatchManager::g_DBAssets.Sql("\", lock_up = \"", false, false);
                    CDispatchManager::g_DBAssets.Sql(strLockUp, true, false);
                    CDispatchManager::g_DBAssets.Sql("\" where status = 1 and user_id = \"", false, false);
                    CDispatchManager::g_DBAssets.Sql(uid, true, false);
                    CDispatchManager::g_DBAssets.Sql("\";", false, false);
                    DEBUG_INFO(CDispatchManager::g_DBAssets.getSql());
                    if (CDispatchManager::g_DBAssets.Exec() >= 0)
                    {
                        vec.push_back(pModel);
                    }
                    else
                    {
                        // 资产修改的sql错误
                        break;
                    }
                }
                else
                {
                    // 没有查询到用户信息
                    break;
                }
            }
            else
            {
                // sql执行错误
                break;
            }
        }
    }
    string getRespString(string level,int marketID)
    {
        Json::Value retData;
        retData["recommd1"] = "0";
        retData["recommd2"] = "0";
        retData["recommd3"] = "0";
        string sqlBase = "SELECT recommender_level1,recommender_level2,recommender_level3 FROM user_info WHERE user_id = \"";
        CDispatchManager::g_DBAccount.Sql(sqlBase, false, true);
        string sUID = std::to_string(uid);
        CDispatchManager::g_DBAccount.Sql(sUID, true, false);
        CDispatchManager::g_DBAccount.Sql("\";", false, false);
        if (CDispatchManager::g_DBAccount.Select())
        {
            if (CDispatchManager::g_DBAccount.NextRow())
            {
                string commender_1, commender_2, commender_3;
                CDispatchManager::g_DBAccount.Value(0, commender_1);
                CDispatchManager::g_DBAccount.Value(1, commender_2);
                CDispatchManager::g_DBAccount.Value(2, commender_3);
                if (commender_1 != "0" && commender_1.length() != 0)
                    retData["recommd1"] = commender_1;
                if (commender_2 != "0" && commender_2.length() != 0)
                    retData["recommd2"] = commender_2;
                if (commender_3 != "0" && commender_3.length() != 0)
                    retData["recommd3"] = commender_3;
            }
        }
        map<uint32_t, CCoinMarket>::iterator itr = CRuntimeConfig::market.find(marketID);
        if (itr != CRuntimeConfig::market.end())
        {
            retData["sysuid"] = itr->second.m_ulSysUid;
        }
        else
        {
            retData["sysuid"] = 0;
        }

        retData["discount"] = CDispatchManager::getUserVipDiscount(level);

        Json::FastWriter writer;
        return writer.write(retData);
    }
    void recordToFinancialAssetsLog(vector<COrderInfo*> &vec, string note)
    {
        string nowTime = std::to_string(CDispatchManager::getNowTime());
        int iSize = vec.size();
        for (size_t i = 0; i < iSize; i++)
        {
            operModel(vec[i],note,nowTime);
        }
    }
    void recordToFinancialAssetsLog(vector<COrderInfo*> &vec, int marketID)
    {
        string nowTime = std::to_string(CDispatchManager::getNowTime());
        string sBaseNote = "";
        map<uint32_t, CCoinMarket>::iterator itr = CRuntimeConfig::market.find(marketID);
        if (itr != CRuntimeConfig::market.end())
        {
            sBaseNote = itr->second.m_sMarketName;
        }
        else
        {
            sBaseNote = std::to_string(marketID);
        }
        
        int iSize = vec.size();
        for (size_t i = 0; i < iSize; i++)
        {
            COrderInfo* pModel = vec[i];
            string typeName = getTypeName(pModel->type);
            string note = sBaseNote + typeName + std::to_string(pModel->coinid);
            operModel(pModel, note, nowTime);
        }
    }
    void operModel(COrderInfo* pModel, string note,string nowTime)
    {
        string fee = pModel->usable;
        string fee_frozen = "-" + pModel->fronzen;
        string type = std::to_string(pModel->type);
        string oper_id = nowTime;
        string coinId = std::to_string(pModel->coinid);

        string sDataTag = CDispatchManager::getYYMMDD();
        string insertFinancial = "INSERT INTO financial_records_" + sDataTag + "(user_id,coin_id,gross_usable,gross_frozen,gross_sum,fee,fee_frozen,oper_type,oper_id,oper_desc,remaining_usable,remaining_frozen,remaining_sum,record_hash,status,create_time,update_time) VALUES (\"";
        CDispatchManager::g_DBSA.Sql(insertFinancial, false, true);
        string sUID = std::to_string(uid);
        CDispatchManager::g_DBSA.Sql(sUID, false, false);
        CDispatchManager::g_DBSA.Sql("\",\"", false, false);
        CDispatchManager::g_DBSA.Sql(coinId, true, false);
        CDispatchManager::g_DBSA.Sql("\",\"", false, false);
        CDispatchManager::g_DBSA.Sql(pModel->usable, true, false);
        CDispatchManager::g_DBSA.Sql("\",\"", false, false);
        CDispatchManager::g_DBSA.Sql(pModel->fronzen, true, false);
        string beTotal = CDispatchManager::addLongDouble(pModel->usable, pModel->fronzen);
        CDispatchManager::g_DBSA.Sql("\",\"", false, false);
        CDispatchManager::g_DBSA.Sql(beTotal, true, false);
        CDispatchManager::g_DBSA.Sql("\",\"", false, false);
        CDispatchManager::g_DBSA.Sql(fee, true, false);
        CDispatchManager::g_DBSA.Sql("\",\"", false, false);
        CDispatchManager::g_DBSA.Sql(fee_frozen, true, false);
        CDispatchManager::g_DBSA.Sql("\",\"", false, false);
        CDispatchManager::g_DBSA.Sql(type, true, false);
        CDispatchManager::g_DBSA.Sql("\",\"", false, false);
        CDispatchManager::g_DBSA.Sql(oper_id, true, false);
        CDispatchManager::g_DBSA.Sql("\",\"", false, false);
        CDispatchManager::g_DBSA.Sql(note, true, false);
        CDispatchManager::g_DBSA.Sql("\",\"", false, false);
        CDispatchManager::g_DBSA.Sql(pModel->remaining_usable, true, false);
        CDispatchManager::g_DBSA.Sql("\",\"", false, false);
        CDispatchManager::g_DBSA.Sql(pModel->remaining_frozen, true, false);
        CDispatchManager::g_DBSA.Sql("\",\"", false, false);
        CDispatchManager::g_DBSA.Sql(pModel->remaining_sum, true, false);
        CDispatchManager::g_DBSA.Sql("\",\"hash\",1,\"", false, false);
        CDispatchManager::g_DBSA.Sql(nowTime, false, false);
        CDispatchManager::g_DBSA.Sql("\",\"", false, false);
        CDispatchManager::g_DBSA.Sql(nowTime, false, false);
        CDispatchManager::g_DBSA.Sql("\");", false, false);
        DEBUG_INFO(CDispatchManager::g_DBSA.getSql());
        if (CDispatchManager::g_DBSA.Exec() < 0)
        {
            string createTableSql = "CREATE TABLE financial_records_" + sDataTag + "(\
                        id BIGINT NOT NULL AUTO_INCREMENT  COMMENT \'id 无符号64长整型=[时间戳秒+自增int]\' ,\
                        user_id BIGINT NOT NULL COMMENT \'用户id\' ,\
                        coin_id int(11) unsigned NOT NULL COMMENT \'币种 id\' ,\
                        gross_usable DECIMAL(32,16) unsigned NOT NULL COMMENT \'交易前资产总额\' ,\
                        gross_frozen DECIMAL(32,16) unsigned NOT NULL COMMENT \'交易前冻结资产\' ,\
                        gross_sum DECIMAL(32,16) unsigned NOT NULL COMMENT \'交易前资产总计\' ,\
                        fee DECIMAL(32,16) NOT NULL COMMENT \'可用变动\' ,\
                        fee_frozen DECIMAL(32,16) NOT NULL COMMENT \'冻结变动\' ,\
                        oper_type tinyint(4) unsigned NOT NULL COMMENT \'操作类型 1.交易买入 2.交易卖出 3.法币充值 4.法币提现 5.交易奖励 6.交易手续费 7.c2c打款 8.c2c收款 9.交易转出 10.交易转入 11.提现手续费 12.转出手续费 13.系统充值 14.冻结释放 15.挖矿（暂无） 16.服务费 17.项目方归集 18.三方otoc交易\' ,\
                        oper_id int(11) unsigned NOT NULL COMMENT \'操作标识\' ,\
                        oper_desc varchar(64) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT \'操作备注\' ,\
                        remaining_usable DECIMAL(32,16) unsigned NOT NULL COMMENT \'剩余正常\' ,\
                        remaining_frozen DECIMAL(32,16) unsigned NOT NULL COMMENT \'剩余冻结\' ,\
                        remaining_sum DECIMAL(32,16) unsigned NOT NULL COMMENT \'剩余总计\' ,\
                        record_hash  varchar(64) NOT NULL COMMENT \'防篡改唯一标识\' ,\
                        status  tinyint(4) unsigned NOT NULL COMMENT \'状态 0 未校验 1 正常 2 异常 3 已对账 4：最后一条数据标记(分表)\' ,\
                        create_time int(11) DEFAULT 0 COMMENT \'创建时间\' ,\
                        update_time int(11) DEFAULT 0 COMMENT \'更新时间\' ,\
                        PRIMARY KEY (id)\
                    ) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT = \'财务记录表\';";
            CDispatchManager::g_DBSA.Sql(createTableSql, false, true);
            DEBUG_INFO(CDispatchManager::g_DBSA.getSql());
            if (CDispatchManager::g_DBSA.Exec() >= 0)
            {
                CDispatchManager::g_DBSA.Sql(insertFinancial, false, true);
                CDispatchManager::g_DBSA.Sql(sUID, false, false);
                CDispatchManager::g_DBSA.Sql("\",\"", false, false);
                CDispatchManager::g_DBSA.Sql(coinId, true, false);
                CDispatchManager::g_DBSA.Sql("\",\"", false, false);
                CDispatchManager::g_DBSA.Sql(pModel->usable, true, false);
                CDispatchManager::g_DBSA.Sql("\",\"", false, false);
                CDispatchManager::g_DBSA.Sql(pModel->fronzen, true, false);
                CDispatchManager::g_DBSA.Sql("\",\"", false, false);
                CDispatchManager::g_DBSA.Sql(beTotal, true, false);
                CDispatchManager::g_DBSA.Sql("\",\"", false, false);
                CDispatchManager::g_DBSA.Sql(fee, true, false);
                CDispatchManager::g_DBSA.Sql("\",\"", false, false);
                CDispatchManager::g_DBSA.Sql(fee_frozen, true, false);
                CDispatchManager::g_DBSA.Sql("\",\"", false, false);
                CDispatchManager::g_DBSA.Sql(type, true, false);
                CDispatchManager::g_DBSA.Sql("\",\"", false, false);
                CDispatchManager::g_DBSA.Sql(oper_id, true, false);
                CDispatchManager::g_DBSA.Sql("\",\"", false, false);
                CDispatchManager::g_DBSA.Sql(note, true, false);
                CDispatchManager::g_DBSA.Sql("\",\"", false, false);
                CDispatchManager::g_DBSA.Sql(pModel->remaining_usable, true, false);
                CDispatchManager::g_DBSA.Sql("\",\"", false, false);
                CDispatchManager::g_DBSA.Sql(pModel->remaining_frozen, true, false);
                CDispatchManager::g_DBSA.Sql("\",\"", false, false);
                CDispatchManager::g_DBSA.Sql(pModel->remaining_sum, true, false);
                CDispatchManager::g_DBSA.Sql("\",\"hash\",1,\"", false, false);
                CDispatchManager::g_DBSA.Sql(nowTime, false, false);
                CDispatchManager::g_DBSA.Sql("\",\"", false, false);
                CDispatchManager::g_DBSA.Sql(nowTime, false, false);
                CDispatchManager::g_DBSA.Sql("\");", false, false);
                DEBUG_INFO(CDispatchManager::g_DBSA.getSql());
                CDispatchManager::g_DBSA.Exec();
            }
            else
            {
                DEBUG_ERROR("financial_records create failed");
                string error;
                CDispatchManager::g_DBSA.Error(error);
                cout<<"error = "<<error<<endl;
            }
        }
        else
        {
            // DEBUG_INFO("数据插入成功");
        }
    }
    string getTypeName(int type)
    {
        string typeName = "";
        switch (type)
        {
        case OPER_TYPE_COMMISION_BUY:
            typeName = "委托交易买入";
            break;
        case OPER_TYPE_COMMISION_SELL:
            typeName = "委托交易卖出";
            break;
        case OPER_TYPE_TRADE_MATCH:
            typeName = "交易撮合";
            break;
        case OPER_TYPE_TRADE_RECOMMENDATION_REWARDS:
            typeName = "交易推荐奖励收益";
            break;
        case OPER_TYPE_TRANSACTION_FEE:
            typeName = "交易手续费收入";
            break;
        case OPER_TYPE_VISA_FIAT_TOPUP:
            typeName = "UKEX visa法币充值";
            break;
        case OPER_TYPE_BANK_REMITTANCE_FIAT_RECHARGE:
            typeName = "银行汇款法币充值";
            break;
        case OPER_TYPE_THIRD_PARTY_PAYMENT_FIAT_RECHARGE:
            typeName = "第三方支付法币充值";
            break;
        case OPER_TYPE_VISA_FIAT_WITHDRAW:
            typeName = "UKEX visa法币提现";
            break;
        case OPER_TYPE_BANK_TRANSFER_FIAT_WITHDRAWAL:
            typeName = "银行汇款法币提现";
            break;
        case OPER_TYPE_BANK_TRANSFER_FIAT_WITHDRAWAL_FEE:
            typeName = "银行汇款法币提现手续费";
            break;
        case OPER_TYPE_THIRD_PARTY_PAYMENT_FIAT_RECHARGE_FEE:
            typeName = "第三方支付法币充值手续费";
            break;
        case OPER_TYPE_OTC_FUND_FREEZE:
            typeName = "OTC资金冻结";
            break;
        case OPER_TYPE_OTC_PAYMENT:
            typeName = "OTC付款";
            break;
        case OPER_TYPE_OTC_COLLECT_MONEY:
            typeName = "OTC收款";
            break;
        case OPER_TYPE_TRANSFER_OUT_ACCORDING_TO_ADDRESS:
            typeName = "根据地址站内转出";
            break;
        case OPER_TYPE_TRANSFER_IN_ACCORDING_TO_ADDRESS:
            typeName = "根据地址站内转入";
            break;
        case OPER_TYPE_TRANSFER_OUT_ACCORDING_TO_ADDRESS_FEE:
            typeName = "根据地址站内转出手续费";
            break;
        case OPER_TYPE_TRANSFER_OUT_OF_PLATFORM_NORMAL_STATION:
            typeName = "平台普通站内转出";
            break;
        case OPER_TYPE_TRANSFER_IN_OF_PLATFORM_NORMAL_STATION:
            typeName = "平台普通站内转入";
            break;
        case OPER_TYPE_TRANSFER_OUT_OF_PLATFORM_NORMAL_FEE:
            typeName = "平台普通站内转出手续费";
            break;
        case OPER_TYPE_WITHDRAWAL_OUTSIDE_STATION:
            typeName = "站外提币";
            break;
        case OPER_TYPE_WITHDRAWAL_OUTSIDE_STATION_FEE:
            typeName = "站外提币手续费";
            break;
        case OPER_TYPE_RECHARGE_OUTSIDE_STATION:
            typeName = "站外充币";
            break;
        case OPER_TYPE_PLATFORM_GIFT:
            typeName = "平台赠送";
            break;
        case OPER_TYPE_VISA_CARD_HANDLING_FEE:
            typeName = "UKEX visa开卡收费";
            break;
        case OPER_TYPE_VISA_RECHARGE_FEE:
            typeName = "UKEX visa充值手续费";
            break;
        case OPER_TYPE_VISA_WITHDRAWAL_FEE:
            typeName = "UKEX visa提现手续费";
            break;
        case OPER_TYPE_PROJECT_SIDE_COLLECTION:
            typeName = "项目方归集";
            break;
        case OPER_TYPE_PLATFORM_SYSTEM_RECHARGE:
            typeName = "平台系统充值";
            break;
        case OPER_TYPE_FREEZE_RELEASE:
            typeName = "冻结释放";
            break;
        default:
            typeName = "未知类型";
            break;
        }
        return typeName;
    }

public :
    /*
    * 协议部分
    * */
    uint64_t uid;
    string paypassword; // 支付密码
    uint32_t opertype;  // 业务类型：1-下单；2-撤单；3-批量下单；4-批量撤单 5-撮合成功 ORDER_OPER_TYPE
    uint32_t operid;    // 支付密码是否生效 1-需要验证；0-不需要验证
    vector<COrderInfo*> list;
};

#endif
