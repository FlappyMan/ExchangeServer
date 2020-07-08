#include "DispatchManager.h"
#include "HttpResp.h"
#include "../ThreadOrder.h"


mapBaseSession CDispatchManager::g_mapTCPSession;
mapBaseSession CDispatchManager::g_mapUserCenterSession;
mapBaseSession CDispatchManager::g_mapFrontMarketSession;

uint64_t CDispatchManager::g_ulControlIndex = 1;
uint64_t CDispatchManager::g_ulOrderIndex = 1;
uint64_t CDispatchManager::g_ulUCIndex = 1;

UBMysql CDispatchManager::g_DBAccount;
UBMysql CDispatchManager::g_DBAdmin;
UBMysql CDispatchManager::g_DBAssets;
UBMysql CDispatchManager::g_DBAssetsLog;
UBMysql CDispatchManager::g_DBBase;
UBMysql CDispatchManager::g_DBExassets;
UBMysql CDispatchManager::g_DBKyc;
UBMysql CDispatchManager::g_DBOTC;
UBMysql CDispatchManager::g_DBOther;
UBMysql CDispatchManager::g_DBSA;
UBMysql CDispatchManager::g_DBTrade;

CDispatchManager::CDispatchManager()
{
    
}
void CDispatchManager::DBInit()
{
    CDispatchManager::g_DBAccount.init(4096);
    CDispatchManager::g_DBAdmin.init(4096);
    CDispatchManager::g_DBAssets.init(4096);
    CDispatchManager::g_DBAssetsLog.init(4096);
    CDispatchManager::g_DBBase.init(4096);
    CDispatchManager::g_DBExassets.init(4096);
    CDispatchManager::g_DBKyc.init(4096);
    CDispatchManager::g_DBOTC.init(4096);
    CDispatchManager::g_DBOther.init(4096);
    CDispatchManager::g_DBSA.init(4096);
    CDispatchManager::g_DBTrade.init(4096);

    connectToDB(CDispatchManager::g_DBAccount, Config::sAccountDB);
    connectToDB(CDispatchManager::g_DBAdmin, Config::sAdminDB);
    connectToDB(CDispatchManager::g_DBAssets, Config::sAssetsDB);
    connectToDB(CDispatchManager::g_DBAssetsLog, Config::sAssetsLogDB);
    connectToDB(CDispatchManager::g_DBBase, Config::sBaseDB);
    connectToDB(CDispatchManager::g_DBExassets, Config::sExassetsDB);
    connectToDB(CDispatchManager::g_DBKyc, Config::sKycDB);
    connectToDB(CDispatchManager::g_DBOTC, Config::sOtcDB);
    connectToDB(CDispatchManager::g_DBOther, Config::sOtherDB);
    connectToDB(CDispatchManager::g_DBSA, Config::sSaDB);
    connectToDB(CDispatchManager::g_DBTrade, Config::sTradeDB); 
}
void CDispatchManager::connectToDB(UBMysql &dbConn, string &dbName)
{
    bool bConnected = dbConn.Connect(Config::sHost, Config::uiPort, Config::sUser, Config::sPwd, dbName);
    if (bConnected == false)
    {
        string sError;
        dbConn.Error(sError);
    }
}
int CDispatchManager::DispatchClient(mapBaseSession &mapSession, client_pkg_t *client, char *pBuffer, int iDataLen, bool bTokenSession)
{
    mapBaseSession::iterator itr = mapSession.find(client);
    if (itr == mapSession.end())
    {
        CBaseSession *session = new CBaseSession;
        mapSession.insert(pair<client_pkg_t *, CBaseSession *>(client, session));
        return session->OnRecv(client, pBuffer, iDataLen, bTokenSession);
    }
    else
    {
        return itr->second->OnRecv(client, pBuffer, iDataLen, bTokenSession);
    }
}
void CDispatchManager::recordData(vector<CBaseModel*>& vecRet)
{
    FILE *fp;
	if ((fp = fopen("../Record/Match.dat", "a+")) == NULL)
	{
		DEBUG_ERROR("RECORD FILE ERROR");
	}
	else
	{
		// 此函数只处理下单和批量下单请求
        for (int i = 0; i < vecRet.size(); i++)
        {
            UPMatchFile *pRecord = new UPMatchFile;
            CBaseModel* pModel = vecRet[i];

            CTradeModel *pBaseModel = (CTradeModel *)vecRet[i];
            pRecord->type = MATCH_TYPE_ORDER;
            pRecord->order.id = pBaseModel->orderid;
            pRecord->order.user_id = pBaseModel->uid;
            pRecord->order.market_id = Config::uiMarketID;
            strncpy(pRecord->order.price, pBaseModel->price.data(), pBaseModel->price.length());
            strncpy(pRecord->order.trading_volume, pBaseModel->totalNum.data(), pBaseModel->totalNum.length());
            strncpy(pRecord->order.traded_volume, "0.0", 3);
            string amount = CTool::mutipLongDouble(pBaseModel->price, pBaseModel->totalNum);
            strncpy(pRecord->order.trading_amount, amount.data(), amount.length());

            if (pBaseModel->type == 1)
            {
                string fee = CTool::mutipLongDouble(amount, CRuntimeConfig::market.m_sSellFee);
                strncpy(pRecord->order.sys_fee, fee.data(), fee.length());
            }
            else
            {
                string fee = CTool::mutipLongDouble(amount, CRuntimeConfig::market.m_sBuyFee);
                strncpy(pRecord->order.sys_fee, fee.data(), fee.length());
            }
            strncpy(pRecord->order.dealFee, "0.0", 3);
            pRecord->order.oper_type = pBaseModel->type;
            pRecord->order.time = CTool::getNowTime();
            pRecord->order.invite_uid_1 = pBaseModel->invite_uid_1;
            pRecord->order.invite_uid_2 = pBaseModel->invite_uid_2;
            pRecord->order.invite_uid_3 = pBaseModel->invite_uid_3;
            pRecord->order.sys_uid = pBaseModel->sysUid;
            pRecord->order.vip_discount = stof(pBaseModel->discount);

            fwrite(pRecord, sizeof(class UPMatchFile), 1, fp);
            fflush(fp);
            delete pRecord;
        }
        fclose(fp);
	}
}
void CDispatchManager::recordData(CBaseModel* pModel)
{
	FILE *fp;
	if ((fp = fopen("../Record/Match.dat", "a+")) == NULL)
	{
		DEBUG_ERROR("RECORD FILE ERROR");
	}
	else
	{
        UPMatchFile *pRecord = new UPMatchFile;

        if(pModel->modelType == OPER_TYPE_CANCELTRADE)
        {
            // 撤单
            CCancelTradeModel *pBaseModel = (CCancelTradeModel *)pModel;
            pRecord->type = MATCH_TYPE_CANCEL;
            pRecord->cancel.id = pBaseModel->canceltradeid;
            pRecord->cancel.market_id = pBaseModel->marketid;
            pRecord->cancel.user_id = pBaseModel->uid;
            fwrite(pRecord, sizeof(class UPMatchFile), 1, fp);
            fflush(fp);
        }
        else if (pModel->modelType == OPER_TYPE_CANCELTRADEE_BATCH)
        {
            // 批量撤单
            CCancelTradeBatchModel *pBaseModel = (CCancelTradeBatchModel *)pModel;
            int iOrderSize = pBaseModel->orders.size();
            for (int i = 0; i < iOrderSize; i++)
            {
                pRecord->type = MATCH_TYPE_CANCEL;
                pRecord->cancel.id = pBaseModel->orders[i].orderid;
                pRecord->cancel.market_id = pBaseModel->orders[i].marketid;
                pRecord->cancel.user_id = pBaseModel->uid;
                fwrite(pRecord, sizeof(class UPMatchFile), 1, fp);
                fflush(fp);
            }
        }
        else
        {
            ;
        }
        
		fclose(fp);
        delete pRecord;
	}
}
