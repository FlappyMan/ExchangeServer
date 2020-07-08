#include "RequestOperMatchData.h"
CRequestOperMatchData::CRequestOperMatchData(CMatchedDataModel model)
{
    m_mMatchData = model;
}
void CRequestOperMatchData::addAmount(string amount, string uid, string coinID)
{
    string tableName = " user_assets_" + coinID + " ";
    string sqlBase = "select usable from " + tableName + " where user_id = \"";
    CDispatchManager::g_DBAssets.Sql(sqlBase, false, true);
    CDispatchManager::g_DBAssets.Sql(uid, true, false);
    CDispatchManager::g_DBAssets.Sql("\";", false, false);
    if (CDispatchManager::g_DBAssets.Select())
    {
        if (CDispatchManager::g_DBAssets.NextRow())
        {
            string usable;
            CDispatchManager::g_DBAssets.Value(0, usable);

            string total = CDispatchManager::addLongDouble(usable, amount);
            CDispatchManager::g_DBAssets.Sql("UPDATE ", false, true);
            CDispatchManager::g_DBAssets.Sql(tableName, true, false);
            CDispatchManager::g_DBAssets.Sql(" SET usable = \"", false, false);
            CDispatchManager::g_DBAssets.Sql(total, true, false);
            CDispatchManager::g_DBAssets.Sql("\" where status = 1 and user_id = \"", false, false);
            CDispatchManager::g_DBAssets.Sql(uid, true, false);
            CDispatchManager::g_DBAssets.Sql("\";", false, false);
            DEBUG_INFO(CDispatchManager::g_DBAssets.getSql());
            CDispatchManager::g_DBAssets.Exec();
        }
    }
}
void CRequestOperMatchData::minusAmount(string amount, string uid, string coinID)
{
    string tableName = " user_assets_" + coinID + " ";
    string sqlBase = "select freeze from " + tableName + " where user_id = \"";
    CDispatchManager::g_DBAssets.Sql(sqlBase, false, true);
    CDispatchManager::g_DBAssets.Sql(uid, true, false);
    CDispatchManager::g_DBAssets.Sql("\";", false, false);
    if (CDispatchManager::g_DBAssets.Select())
    {
        if (CDispatchManager::g_DBAssets.NextRow())
        {
            string freeze;
            CDispatchManager::g_DBAssets.Value(0, freeze);

            string total = CDispatchManager::minusLongDouble(freeze, amount);
            CDispatchManager::g_DBAssets.Sql("UPDATE ", false, true);
            CDispatchManager::g_DBAssets.Sql(tableName, true, false);
            CDispatchManager::g_DBAssets.Sql(" SET freeze = \"", false, false);
            CDispatchManager::g_DBAssets.Sql(total, true, false);
            CDispatchManager::g_DBAssets.Sql("\" where status = 1 and user_id = \"", false, false);
            CDispatchManager::g_DBAssets.Sql(uid, true, false);
            CDispatchManager::g_DBAssets.Sql("\";", false, false);
            DEBUG_INFO(CDispatchManager::g_DBAssets.getSql());
            CDispatchManager::g_DBAssets.Exec();
        }
    }
}
void CRequestOperMatchData::FeeChargeCalculation(string uid, string &totalFee)
{
    // 买方账户计算
    string sqlBase = "SELECT recommender_level1,recommender_level2,recommender_level3 FROM user_info WHERE user_id = \"";
    CDispatchManager::g_DBAccount.Sql(sqlBase, false, true);
    CDispatchManager::g_DBAccount.Sql(uid, true, false);
    CDispatchManager::g_DBAccount.Sql("\";", false, false);
    if (CDispatchManager::g_DBAccount.Select())
    {
        if (CDispatchManager::g_DBAccount.NextRow())
        {
            string InviteFee_1 = CDispatchManager::mutipLongDouble(totalFee, m_sInvitUID_1); // 买方手续费*平台配置此货币1级邀请人手续费比例
            string InviteFee_2 = CDispatchManager::mutipLongDouble(totalFee, m_sInvitUID_2); // 买方手续费*平台配置此货币1级邀请人手续费比例
            string InviteFee_3 = CDispatchManager::mutipLongDouble(totalFee, m_sInvitUID_3); // 买方手续费*平台配置此货币1级邀请人手续费比例

            string commender_1, commender_2, commender_3;
            CDispatchManager::g_DBAccount.Value(0, commender_1);
            CDispatchManager::g_DBAccount.Value(1, commender_2);
            CDispatchManager::g_DBAccount.Value(2, commender_3);
            string coinB = m_coinB;
            if (commender_1.length() != 0 && commender_1 != "0")
            {
                addAmount(InviteFee_1, commender_1, coinB);
                totalFee = CDispatchManager::minusLongDouble(totalFee, InviteFee_1);
            }
            if (commender_2.length() != 0 && commender_2 != "0")
            {
                addAmount(InviteFee_2, commender_2, coinB);
                totalFee = CDispatchManager::minusLongDouble(totalFee, InviteFee_2);
            }
            if (commender_3.length() != 0 && commender_3 != "0")
            {
                addAmount(InviteFee_3, commender_3, coinB);
                totalFee = CDispatchManager::minusLongDouble(totalFee, InviteFee_3);
            }
            addAmount(totalFee, m_sFeeUID, coinB); // 平台手续费
        }
    }
}
UProtocolBase *CRequestOperMatchData::requestOperation()
{
    string configTable = "SELECT fee_buy,fee_sell,fee_user_id,invit_buy,invit_sell,invit_1,invit_2,invit_3 FROM coin_market WHERE id = " + std::to_string(m_marketID);
    CDispatchManager::g_DBTrade.Sql(configTable, false, true);
    CDispatchManager::g_DBTrade.Sql(";", false, false);
    DEBUG_INFO(CDispatchManager::g_DBTrade.getSql());
    if (CDispatchManager::g_DBTrade.Select())
    {
        if (CDispatchManager::g_DBTrade.NextRow())
        {
            CDispatchManager::g_DBTrade.Value(0, m_sFeeBuy);
            CDispatchManager::g_DBTrade.Value(1, m_sFeeSell);
            CDispatchManager::g_DBTrade.Value(2, m_sFeeUID);
            CDispatchManager::g_DBTrade.Value(3, m_sInvitBuyFee);
            CDispatchManager::g_DBTrade.Value(4, m_sInvitSellFee);
            CDispatchManager::g_DBTrade.Value(5, m_sInvitUID_1);
            CDispatchManager::g_DBTrade.Value(6, m_sInvitUID_2);
            CDispatchManager::g_DBTrade.Value(7, m_sInvitUID_3);
        }
        else
        {
            DEBUG_ERROR("can't find config market");
        }
    }
    else
    {
        DEBUG_ERROR("database not connected");
    }
    
    // m_coinA 为商品 如 btc
    // m_coinB 为货币 如 usdt
    if (m_mMatchData.price.length() != 0 && m_mMatchData.number.length() != 0)
    {
        // 本次撮合成功获得的总金额
        // 商品成交价格 m_mMatchData.price
        // 商品成交个数 m_mMatchData.number
        if (CDispatchManager::minusLongDouble(m_mMatchData.price,m_mMatchData.buyuser.buyPrice) == "-1")
        {
            // 买方价格大于成交价，退还手续费归还部分金额
            string dealPrice = CDispatchManager::minusLongDouble(m_mMatchData.buyuser.buyPrice,m_mMatchData.price);
            string returnFee = CDispatchManager::mutipLongDouble(dealPrice, m_mMatchData.number);
            addAmount(returnFee, m_mMatchData.buyuser.userid, m_coinB);
        }
        
        string totalAmount = CDispatchManager::mutipLongDouble(m_mMatchData.price, m_mMatchData.number); // 需要花费的货币总金额
        // 根据不同的费率算到各自的手续费提成
        string totalBuyFee = CDispatchManager::mutipLongDouble(totalAmount, m_sFeeBuy);   // 平台配置买方手续费总额（货币）
        string totalSellFee = CDispatchManager::mutipLongDouble(totalAmount, m_sFeeSell); // 平台配置卖方手续费总额（货币）

        FeeChargeCalculation(m_mMatchData.buyuser.userid, totalBuyFee);
        string buyAmount = CDispatchManager::minusLongDouble(totalAmount, totalBuyFee);     // 用户实际能够使用购买的货币总量（已经减去所有手续费）
        string commodity = CDispatchManager::diviLongDouble(buyAmount, m_mMatchData.price); // 总金额（扣除手续费）/单价 = 商品数量
        addAmount(commodity, m_mMatchData.buyuser.userid, m_coinA);                      // 增加用户的商品数量amount个
        minusAmount(totalAmount, m_mMatchData.buyuser.userid, m_coinB);                  // 用户的货币冻结金额减少totalAmount个

        FeeChargeCalculation(m_mMatchData.selluser.userid, totalSellFee);             // 平台手续费
        string sellAmount = CDispatchManager::minusLongDouble(totalAmount, totalSellFee); // 卖方能收到的货币总数（已经扣除手续费）
        addAmount(sellAmount, m_mMatchData.selluser.userid, m_coinB);                     // 卖方获得货币总数amount
        minusAmount(m_mMatchData.number, m_mMatchData.selluser.userid, m_coinA);       // 卖方商品冻结数减少 m_mMatchData.number 个
    }

    return NULL;
}
string CRequestOperMatchData::getOperationToken()
{
    return m_mMatchData.token;
}