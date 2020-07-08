#include "RegistDto.h"
#include "../Model/RegistRespDto.hpp"
#include "DispatchManager.h"
CRegistDto::CRegistDto()
{
    CDispatchManager::g_DBAccount.init(SIZE_BUFFER_4k);
    CDispatchManager::g_DBAssets.init(SIZE_BUFFER_4k);
    CDispatchManager::g_DBBase.init(SIZE_BUFFER_2k);
}

CJsonObjectBase *CRegistDto::requestOperation()
{
#ifdef PRINT_LOG
    DEBUG_INFO("[CRegistDto][requestOperation]");
#endif
    CRegistRespDto *respDto = new CRegistRespDto;
    if (userAccount.length() == 0 || password.length() == 0 || deviceID.length() == 0)
    {
        respDto->data = "参数错误";
        respDto->status = 0;
    }
    else if (sourceType != 1 && sourceType != 2)
    {
        respDto->data = "参数错误";
        respDto->status = 0;
    }
    else
    {
        CDispatchManager::g_DBAccount.Sql("select id from user where user_account = \"", false, true);
        CDispatchManager::g_DBAccount.Sql(userAccount, true, false);
        CDispatchManager::g_DBAccount.Sql("\";", false, false);
        if (CDispatchManager::g_DBAccount.Select())
        {
            if (CDispatchManager::g_DBAccount.NextRow() == false)
            {
                int iSqlRet = 0;

                stringstream sTime;
                srand((unsigned)time(NULL));
                time_t nowTime = time(NULL);
                sTime << (int64_t)nowTime;
                string sTimeTemp = sTime.str();

                // 插入表user
                CDispatchManager::g_DBAccount.Sql("INSERT INTO user(user_account,password,source_type,login_attempts,login_time,create_time,update_time) VALUES(\"", false, true);
                CDispatchManager::g_DBAccount.Sql(userAccount, true, false);
                CDispatchManager::g_DBAccount.Sql("\",\"", false, false);
                CDispatchManager::g_DBAccount.Sql(password, true, false);
                CDispatchManager::g_DBAccount.Sql("\",\"", false, false);
                string st = std::to_string(sourceType);
                CDispatchManager::g_DBAccount.Sql(st, true, false);
                CDispatchManager::g_DBAccount.Sql("\",1,", false, false);
                CDispatchManager::g_DBAccount.Sql(sTimeTemp, true, false);
                CDispatchManager::g_DBAccount.Sql(",", false, false);
                CDispatchManager::g_DBAccount.Sql(sTimeTemp, true, false);
                CDispatchManager::g_DBAccount.Sql(",", false, false);
                CDispatchManager::g_DBAccount.Sql(sTimeTemp, true, false);
                CDispatchManager::g_DBAccount.Sql(");", false, false);

                if (CDispatchManager::g_DBAccount.Exec() >= 0)
                {
                    string userId = to_string(CDispatchManager::g_DBAccount.LastID());

                    // 插入user_account表
                    CDispatchManager::g_DBAccount.Sql("INSERT INTO user_account(account,account_type,source_type,user_id,ukex_uid,ukexpay_uid,create_time,update_time) VALUES (\"", false, true);
                    CDispatchManager::g_DBAccount.Sql(userAccount, true, false);
                    CDispatchManager::g_DBAccount.Sql("\",1,\"", false, false);
                    CDispatchManager::g_DBAccount.Sql(st, true, false);
                    CDispatchManager::g_DBAccount.Sql("\",", false, false);
                    CDispatchManager::g_DBAccount.Sql(userId, true, false);
                    CDispatchManager::g_DBAccount.Sql(",0,0,", false, false);
                    CDispatchManager::g_DBAccount.Sql(sTimeTemp, true, false);
                    CDispatchManager::g_DBAccount.Sql(",", false, false);
                    CDispatchManager::g_DBAccount.Sql(sTimeTemp, true, false);
                    CDispatchManager::g_DBAccount.Sql(");", false, false);

                    // 插入user_auth表
                    CDispatchManager::g_DBAccount.Sql("INSERT INTO user_auth(user_id,is_allow_login,paypassword_status,user_status,auth_status,kyc_auth,email_auth,allow_openapi_withdrawal,is_reauth,is_pass_aml,register_type,verify_type,global_kyc_status,last_operate_time,account_status,os_verify,pin_verify,two_factor,hide_balance,create_time,update_time) VALUES (\"", false, false);
                    CDispatchManager::g_DBAccount.Sql(userId, true, false);
                    CDispatchManager::g_DBAccount.Sql("\",1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,", false, false);
                    CDispatchManager::g_DBAccount.Sql(sTimeTemp, true, false);
                    CDispatchManager::g_DBAccount.Sql(",", false, false);
                    CDispatchManager::g_DBAccount.Sql(sTimeTemp, true, false);
                    CDispatchManager::g_DBAccount.Sql(");", false, false);
                    

                    // 插入user_facts表
                    CDispatchManager::g_DBAccount.Sql("INSERT INTO user_facts(user_id,first_name,last_name,birthday,country,residence_country,province,city,town,district,street_address,building_number,building_name,sex,zip_code,apartment,first_residence_certificate,second_residence_certificate,street_code,unit,house_code,passport,has_deducted_gbp,has_deducted_eur,has_deducted_usd,passport_ocr,passport_expire,passport_picture,passport_picture_hold,driving_number,driving_picture,driving_picture_hold,is_europe,nationality,create_time,update_time) VALUES (\"", false, false);
                    CDispatchManager::g_DBAccount.Sql(userId, true, false);
                    CDispatchManager::g_DBAccount.Sql("\",'','',0,'','','','','','','','','',0,'','','','','','','','',0,0,0,'','','','','','','',0,'',\"", false, false);
                    CDispatchManager::g_DBAccount.Sql(sTimeTemp, true, false);
                    CDispatchManager::g_DBAccount.Sql(",", false, false);
                    CDispatchManager::g_DBAccount.Sql(sTimeTemp, true, false);
                    CDispatchManager::g_DBAccount.Sql(");", false, false);

                    // 插入user_info表
                    CDispatchManager::g_DBAccount.Sql("INSERT INTO user_info(user_id,mobile,id_type,identity_number,identity_expiry_date,identity_info,paypassword,recommender_level1,recommender_level2,recommender_level3,realname,country,c2c_info_ext,ext,invit_time,source_of_funds,mobile_binding_time,id_number,ip_address,address_desc,email,wechat,wechat_img,wechat_openid,alipay,alipay_img,invite_code,custom_cate_id,master_account,source_proj_uid,uuid,uuid_salt,user_image,country_code,user_type,account_active_time,secret_key,refer_currency,kyc_refuse_num,lucky_count,c2c_refuse_num,rescue_code,create_time,update_time) VALUES (", false, false);
                    CDispatchManager::g_DBAccount.Sql(userId, true, false);
                    CDispatchManager::g_DBAccount.Sql(",'',0,'','',NULL,'','','','','','',NULL,NULL,0,NULL,0,'','','','','','','','','','',0,0,0,'','','','',0,0,'',0,0,0,0,'',", false, false);
                    CDispatchManager::g_DBAccount.Sql(sTimeTemp, true, false);
                    CDispatchManager::g_DBAccount.Sql(",", false, false);
                    CDispatchManager::g_DBAccount.Sql(sTimeTemp, true, false);
                    CDispatchManager::g_DBAccount.Sql(");", false, false);
                    
                    DEBUG_INFO(CDispatchManager::g_DBAccount.getSql());
                    if (CDispatchManager::g_DBAccount.Exec() >= 0)
                    {
                        CDispatchManager::g_DBBase.Sql("SELECT id FROM coin_coin;", false, true);
                        if (CDispatchManager::g_DBBase.Select())
                        {
                            CDispatchManager::g_DBAssets.Sql(" ", false, true);
                            while (CDispatchManager::g_DBBase.NextRow())
                            {
                                string coinID;
                                CDispatchManager::g_DBBase.Value(0, coinID);
                                string sqlTemp = "INSERT INTO user_assets_" + coinID + " (source_type,user_id,usable,freeze,lock_up,status,create_time,update_time) VALUES(\"";
                                CDispatchManager::g_DBAssets.Sql(sqlTemp, false, false);
                                CDispatchManager::g_DBAssets.Sql(st, true, false);
                                CDispatchManager::g_DBAssets.Sql("\",\"", false, false);
                                CDispatchManager::g_DBAssets.Sql(userId, true, false);
                                CDispatchManager::g_DBAssets.Sql("\",0,0,0,1,", false, false);
                                CDispatchManager::g_DBAssets.Sql(sTimeTemp, true, false);
                                CDispatchManager::g_DBAssets.Sql(",", false, false);
                                CDispatchManager::g_DBAssets.Sql(sTimeTemp, true, false);
                                CDispatchManager::g_DBAssets.Sql(");", false, false);
                            }
                            // 用户资产表插入完成
                            stringstream sDeadLine;      // 当前时间，token截止时间
                            srand((unsigned)time(NULL)); // 随机种子
                            int64_t deadLine = (int64_t)nowTime + 604800;
                            sDeadLine << deadLine;

                            string sToken = CDispatchManager::IntHexToString(deadLine, 8);
                            sToken += CDispatchManager::IntHexToString(std::stoi(userId), 8);

                            string md5Value = sToken + TOKEN_MD5_KEY;
                            sToken += CDispatchManager::md5(md5Value);

                            char cData = sToken.data()[sToken.length() - 1];
                            sToken.insert(0, string(&cData, 1));

                            string sDeadLineTemp = sDeadLine.str();

                            CDispatchManager::g_DBAssets.Sql("INSERT INTO other.user_device (device_id,uid,name,token,token_time,public_key,equipment,comment,source_type,status,create_time,update_time) VALUES (\"", false, false);
                            CDispatchManager::g_DBAssets.Sql(deviceID, true, false);
                            CDispatchManager::g_DBAssets.Sql("\",", false, false);
                            CDispatchManager::g_DBAssets.Sql(userId, true, false);
                            CDispatchManager::g_DBAssets.Sql(",", false, false);
                            CDispatchManager::g_DBAssets.Sql("'',\"", false, false);
                            CDispatchManager::g_DBAssets.Sql(sToken, true, false);
                            CDispatchManager::g_DBAssets.Sql("\",\"", false, false);
                            CDispatchManager::g_DBAssets.Sql(sDeadLineTemp, false, false);
                            CDispatchManager::g_DBAssets.Sql("\",'',", false, false);
                            CDispatchManager::g_DBAssets.Sql("'',", false, false);
                            CDispatchManager::g_DBAssets.Sql("'',", false, false);
                            CDispatchManager::g_DBAssets.Sql(st, true, false);
                            CDispatchManager::g_DBAssets.Sql(",0,", false, false);
                            CDispatchManager::g_DBAssets.Sql(sTimeTemp, true, false);
                            CDispatchManager::g_DBAssets.Sql(",", false, false);
                            CDispatchManager::g_DBAssets.Sql(sTimeTemp, true, false);
                            CDispatchManager::g_DBAssets.Sql(");", false, false);

                            if (CDispatchManager::g_DBAssets.Exec() >= 0)
                            {
                                string dataString;
                                Json::Value sJson;
                                sJson["token"] = Json::Value(sToken);
                                sJson["deadLine"] = Json::Value(sDeadLine.str());
                                sJson["uid"] = Json::Value(userId);
                                UBJsonPack(dataString, sJson, true);

                                respDto->status = 1;
                                respDto->data = dataString;
                            }
                            else
                            {
                                respDto->data = "操作失败";
                                respDto->status = 0;
                            }
                        }
                        else
                        {
                            respDto->data = "操作失败";
                            respDto->status = 0;
                        }
                    }
                    else
                    {
                        respDto->data = "操作失败";
                        respDto->status = 0;
                    }
                }
                else
                {
                    respDto->data = "操作失败";
                    respDto->status = 0;
                }
            }
            else
            {
                respDto->data = "账号重复";
                respDto->status = 0;
            }
        }
        else
        {
            respDto->data = "操作失败";
            respDto->status = 0;
        }
    }

    respDto->m_client = m_client;
    return (CJsonObjectBase *)respDto;
}