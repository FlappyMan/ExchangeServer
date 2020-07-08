#include "CoinAddDto.h"
#include "../Model/MsgRespDto.hpp"
#include "DispatchManager.h"
CCoinAddDto::CCoinAddDto()
{
    CDispatchManager::g_DBAssets.init(SIZE_BUFFER_64k);
    CDispatchManager::g_DBAccount.init(SIZE_BUFFER_2k);
}

CJsonObjectBase *CCoinAddDto::requestOperation()
{
#ifdef PRINT_LOG
    DEBUG_INFO("[CCoinAddDto::requestOperation]");
#endif
    CMsgRespDto *respDto = new CMsgRespDto;
    if (name.length() == 0 || type.length() == 0 || title.length() == 0)
    {
        respDto->status = 0;
        respDto->data = "参数错误";
    }
    else
    {
        // 插入新币种id到基础表配置种
        CDispatchManager::g_DBAssets.Sql("insert into base.coin_coin(name,type,title,img,app_img,fee_bili,fee_meitian,dj_zj,dj_dk,dj_yh,dj_mm,zr_zs,zr_jz,zr_dz,zc_min,zc_max,zc_jz,js_yw,js_sm,js_qb,js_ym,js_gw,js_lt,js_wk,cs_yf,cs_sf,cs_fb,cs_qk,cs_zl,cs_cl,cs_zm,cs_nd,cs_jl,cs_ts,cs_bz) VALUES (\"", false, true);
        CDispatchManager::g_DBAssets.Sql(name, true, false);
        CDispatchManager::g_DBAssets.Sql("\",\"", false, false);
        CDispatchManager::g_DBAssets.Sql(type, true, false);
        CDispatchManager::g_DBAssets.Sql("\",\"", false, false);
        CDispatchManager::g_DBAssets.Sql(title, true, false);
        CDispatchManager::g_DBAssets.Sql("\",'','',0,'','','','','',0,'','',0,0,'','','','','','','','','','','','',0,0,'','',0,'','');", false, false);
        if (CDispatchManager::g_DBAssets.Exec() >= 0)
        {
            // 创建新币种的资产信息表
            string coinID = to_string(CDispatchManager::g_DBAssets.LastID());
            string strCreateSql = "CREATE TABLE user_assets_" + coinID + "(id BIGINT NOT NULL AUTO_INCREMENT  COMMENT \'id 无符号64长整型=[时间戳秒+自增int]\' ,\
		                    source_type tinyint(1) NOT NULL COMMENT \'资产状态 0:未知; 1-ukex用户 ;2-ukexpay用户;\' ,\
                            user_id BIGINT NOT NULL COMMENT \'userid\' ,\
                            usable DECIMAL(32,16) unsigned NOT NULL COMMENT \'可用金额\',\
                            freeze DECIMAL(32,16) unsigned NOT NULL  COMMENT \'冻结金额\' ,\
                            lock_up DECIMAL(32,16) unsigned NOT NULL  COMMENT \'锁仓\' ,\
                            status tinyint(1) NOT NULL COMMENT \'资产状态 -3:未激活; -2:隐藏;-1:销户;0-禁用;1-正常;\' ,\
                            create_time int(11) unsigned NOT NULL COMMENT \'创建时间\' ,\
                            update_time int(11) DEFAULT 0  COMMENT \'更新时间\' ,\
                            PRIMARY KEY (id)\
                            ) COMMENT = \'用户资产表\';;";
            CDispatchManager::g_DBAssets.Sql(strCreateSql, false, true);
            if (CDispatchManager::g_DBAssets.Exec() >= 0)
            {
                CDispatchManager::g_DBAccount.Sql("SELECT user_id,source_type FROM user_account;", false, true);
                if (CDispatchManager::g_DBAccount.Select())
                {
                    stringstream sTime;
                    srand((unsigned)time(NULL));
                    time_t nowTime = time(NULL);
                    sTime << (int64_t)nowTime;
                    string sTimeTemp = sTime.str();
                    CDispatchManager::g_DBAssets.Sql(" ", false, true);
                    string sqlInsert = "INSERT INTO user_assets_" + coinID + " (source_type,user_id,usable,freeze,lock_up,status,create_time,update_time) VALUES";
                    while (CDispatchManager::g_DBAccount.NextRow())
                    {
                        // 向新表种插入所有之前记录的所有用户数据信息
                        string userId;
                        string st;
                        CDispatchManager::g_DBAccount.Value(0, userId);
                        CDispatchManager::g_DBAccount.Value(1, st);

                        sqlInsert += "(" + st + "," + userId  + ",0,0,0,1," + sTimeTemp + "," + sTimeTemp + "),";
                    }
                    sqlInsert.pop_back();
                    if (CDispatchManager::g_DBAssets.Exec(sqlInsert) >= 0)
                    {
                        respDto->status = 1;
                        respDto->data = "操作成功";
                    }
                    else
                    {
                        respDto->status = 0;
                        respDto->data = "操作失败";
                    }
                }
                else
                {
                    respDto->status = 0;
                    respDto->data = "操作失败";
                }
            }
            else
            {
                respDto->status = 0;
                respDto->data = "操作失败";
            }
        }
        else
        {
            respDto->status = 0;
            respDto->data = "操作失败";
        }
    }

    respDto->m_client = m_client;
    return (CJsonObjectBase *)respDto;
}