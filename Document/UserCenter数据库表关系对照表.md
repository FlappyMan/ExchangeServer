[TOC]

# 历史记录
日期		 | 作者		| 内容
-------------|----------|------------------------------
2019-11-11   | WYP     | 文档创建


日期		 | 作者		|  数据库名               | 新表名                           | ukex表                  | ukexpay表           |描述
-------------|------------|----------------------|---------------------------------|---------------------------------------------------------------
2019-11-11   | WYP     | Account              | [user_account](#user_account)    | coin_user              | cnuk_user_main                        | 创建
2019-11-11   | WYP     | Account              | [user](#user)                    | coin_user              | cnuk_user_main                        | 创建
2019-11-11   | WYP     | Account              | [user_info](#user_info)          | coin_user              | cnuk_user_main & cnuk_user_info       | 创建
2019-11-11   | WYP     | Account              | [user_auth](#user_auth)          | coin_user              | cnuk_user_main                        | 创建
2019-11-11   | WYP     | Account              | [user_facts](#user_facts)        | coin_user_info         | cnuk_user_info                        | 创建
2019-11-11   | WYP     | Assets               | [user_assets_[coin_id]](#user_assets) | coin_user_coin_balance | cnuk_user_balance                | 创建
2019-11-11   | WYP     | Account              | [user_log](#log)                 | -                      | -                                      | 创建


## user_account
> <a id="user_account">user_account 对应 coin_user & cnuk_user_main</a>
> 作者 ：WYP
新表user字段                  | coin_user        | cnuk_user_main                    | 字段说明
-----------------------------|------------------|-----------------------------------|---------
account                      | username&email&mobile         | email                | 新增
account_type                 | -                | -                                | 新增
source_type                  | -                | -                                | 新增
ukex_id                      | id               | -                                | 修改&新增
ukexpay_uid                  | -                | uid                                | 新增&修改
user_id                      | id               | uid                               | 新增
create_time                  | -                | -                                | 新增
update_time                  | -                | -                                | 新增


## user
> <a id="user">user 对应 coin_user & cnuk_user_main</a>
> 作者 ：WYP
新表user字段             | coin_user         | cnuk_user_main            |  coin_user 说明   | cnuk_user_main 说明
------------------------|-------------------|---------------------------|----------------------|---------------------
id                      | id                | uid                       | -                    | 修改      
user_account            | username          | email                     | 修改                  | 修改      
password                | password          | password                  | -                    | -
login_attempts          | logins            | -                         | 修改                  | - 
source_type             |-                  | -                         |-                     | -
无                      | ~~token~~         | -                         | -                    | -
login_time              | login_time        | -                         | -                    | -
无                      | ~~sort~~          | -                         | 新表无此字段          | -
无                      | ~~endtime~~       | -                         | 新表无此字段          | -
create_time             | -                 | -                         | 新增                 | -
update_time             | -                 | -                         | 新增                 | -

## user_info
> <a id="user_info">user_info 对应 coin_user & cnuk_user_main & cnuk_user_info</a>
> 作者 ：WYP
新表user字段             | coin_user         | cnuk_user_main       | cnuk_user_info         | coin_user说明  | cnuk_user_main说明  | cnuk_user_info说明             
------------------------|-------------------|----------------------|------------------------|-----------------|----------------------|-------------------
user_id                 | id                | uid                  | uid                    | 修改             | 修改                  | 修改      
mobile                  | moble             | phone                | -                      | 修改             | 修改                  | 无
id_type                 | id_type           | -                    | certificate_type       | -               | -                    | 修改      
identity_number         | -                 | -                    | identity_number        | -               | -                    | 新增
identity_expiry_date    | -                 | -                    | identity_expiry_date   | -               | -                    | 新增
identity_info           | -                 | -                    | identity_info          | -               | -                    | 新增
paypassword             | paypassword       | pin                  | -                      | -               | 修改                  | -
-                       | -                 | ~~password_second~~  | -                      | -               | 移除                  | -
-                       | ~~regist_time~~   | -                    | -                      | 移除             | -                    | -
recommender_level1      | invit_1           | -                    | invit_1                | 修改             | -                    | 修改      
recommender_level2      | invit_2           | -                    | invit_2                | 修改             | -                    | 修改      
recommender_level3      | invit_3           | -                    | invit_3                | 修改             | -                    | 修改      
realname                | truename          | -                    | cn_name                | 修改             | -                    | 修改      
country                 | country           | country              | -                      | -               | -                    | -
c2c_info_ext            | -                 | -                    | c2c_info_ext           | -               | -                    | 新增
ext                     | -                 | -                    | ext                    | -               | -                    | 新增
invit_time              | -                 | -                    | invit_time             | -               | -                    | 新增
source_of_funds         | -                 | -                    | source_of_funds        | -               | -                    | 新增
mobile_binding_time     | mobletime         | -                    | -                      | 修改             | -                    | -
id_number               | idcard            | -                    | -                      | 修改             | -                    | -
ip_address              | addip             | -                    | -                      | 修改             | -                    | -
address_desc            | addr              | -                    | -                      | 修改             | -                    | -
email                   | email             | -                    | -                      | -               | -                    | -
wechat                  | wechat            | -                    | -                      | -               | -                    | -
wechat_img              | wechat_img        | -                    | -                      | -               | -                    | -
wechat_openid           | openid            | -                    | -                      | 修改             | -                    | -
alipay                  | alipay            | -                    | -                      | -               | -                    | -
alipay_img              | alipay_img        | -                    | -                      | -               | -                    | -
invite_code             | invit             | -                    | invit_code             | 修改             | -                    | -
custom_cate_id          | custom_cate_id    | -                    | -                      | -               | -                    | -
master_account          | parent_id         | -                    | -                      | 修改             | -                    | -
source_proj_uid         | source_proj_uid   | -                    | -                      | -               | -                    | -
uuid                    | uuid              | -                    | -                      | -               | -                    | -
uuid_salt               | uuid_salt         | -                    | -                      | -               | -                    | -
user_image              | -                 | img_url              | -                      | -               | 新增                 | -
country_code            | -                 | country_code         | -                      | -               | 新增                 | -
user_type               | -                 | type                 | -                      | -               | 新增                 | -
account_active_time     | -                 | active_time          | -                      | -               | 新增                 | -
secret_key              | ga                | secret_key           | -                      | 修改            | 新增                 | -
refer_currency          | -                 | refer_currency       | -                      | -               | 新增                 | -
kyc_refuse_num          | -                 | kyc_refuse_num       | -                      | -               | 新增                 | -
lucky_count             | -                 | lucky_count          | -                      | -               | 新增                 | -
c2c_refuse_num          | -                 | c2c_refuse_num       | -                      | -               | 新增                 | -
rescue_code             | -                 | rescue_code          | -                      | -               | 新增                 | -
create_time             | -                 | -                    | -                      | 新增            | -                    | -
update_time             | -                 | -                    | -                      | 新增            | -                    | -

## user_auth
> <a id="user_auth">user_auth 对应 coin_user & cnuk_user_main</a>
> 作者 ：WYP
新表user字段             | coin_user         | cnuk_user_main        | coin_user说明        | cnuk_user_main说明
------------------------|-------------------|-----------------------|----------------------|-----------------------
user_id                 | id                | uid                   | 修改                  | 修改      
is_allow_login          | is_allow_login    | -                     | -                    | -
paypassword_status      | tpwdsetting       | -                     | 修改                  | -
user_status             | status            | -                     | 修改                  | -
auth_status             | isauth            | c2c_status            | 修改                  | -
kyc_auth                | kycauth           | auth_status           | 修改                  | -
email_auth              | is_email          | -                     | 修改                  | -
allow_openapi_withdrawal    | allow_api_withdrawal  | -                     | 修改                  | -
is_reauth               | is_reauth         | -                     | -                    | -
is_pass_aml             | is_pass_aml       | -                     | -                    | -
register_type           | register_by       | -                     | 修改                  | -
verify_type             | verify_type       | -                     | -                    | -
global_kyc_status       | global_kyc_status | -                     | -                    | -
last_operate_time       | last_operate_time | -                     | -                    | -
account_status          | -                 | status                | -                    | 新增
-                       | -                 | ~~has_balance~~       | -                    | 移除
os_verify               | -                 | os_verify             | -                    | 新增
pin_verify              | -                 | pin_verify            | -                    | 新增
two_factor              | -                 | two_factor            | -                    | 新增
hide_balance            | -                 | hide_balance          | -                    | 新增
create_time             | -                 | -                     | 新增                 | -
update_time             | -                 | -                     | 新增                 | -

## user_facts
> <a id="user_facts">user_facts 对应 coin_user_info & cnuk_user_info</a>
> 作者 ：WYP
新表user字段             | coin_user_info    | cnuk_user_info           | coin_user_info说明 | cnuk_user_info说明
------------------------|-------------------|--------------------------|--------------------|---------------------
user_id                 | user_id           | uid                       | -                 | 修改      
first_name              | first_name        | first_name                | -                 | -
last_name               | last_name         | last_name                 | -                 | -
birthday                | birthday          | birth                     | -                 | 修改      
country                 | country           | cnuk_user_main.country    | -                 | 修改
residence_country       | -                 | residence_country         | -                 | -      
province                | province          | state                     | -                 | 修改      
city                    | city              | city                      | -                 | -
town                    | town              | town                      | -                 | -
district                | district          | district                  | -                 | -
street_address          | street_address    | street                    | -                 | 修改      
building_number         | building_number   | building                  | -                 | 修改      
building_name           | building_name     | building_name             | -                 | -
sex                     | sex               | sex                       | -                 | -
zip_code                | zip_code          | postcode                  | -                 | 修改      
apartment               | -                 | apartment                 | -                 | 新增
first_residence_certificate                     | living_proof_1 | living_proof_1      | 修改               | 修改      
second_residence_certificate                    | living_proof_2 | living_proof_2      | 修改               | 修改      
street_code             | street_code       | -                         | -                 | -
unit                    | unit              | -                         | -                 | -
house_code              | house_code        | -                         | -                 | -
passport                | passport          | -                         | -                 | -
passport_ocr            | passport_ocr      | -                         | -                 | -
passport_expire         | passport_expire   | -                         | -                 | -
passport_picture        | passport_1        | -                         | 修改               | -
passport_picture_hold   | passport_2        | -                         | 修改               | -
driving_number          | driving           | -                         | 修改               | -
driving_picture         | driving_1         | -                         | 修改               | -
driving_picture_hold    | driving_2         | -                         | 修改               | -
is_europe               | is_eu             | -                         | 修改               | -
has_deducted_gbp        | -                 | -                         | 新增               | -
has_deducted_eur        | -                 | -                         | 新增               | -
has_deducted_usd        | -                 | -                         | 新增               | -
-                       | ~~visa_email~~    | -                         | 移除               | -
-                       | ~~visa_mobile_code~~  | -                         | 修改               | -
-                       | ~~visa_mobile~~   | -                         | 移除               | -
nationality             | nationality       | -                         | -                  | -
create_time             | -                 | -                         | 新增               | -
update_time             | -                 | -                         | 新增               | -


## user_assets_[coin_id]
> <a id="user_assets">user_assets_[coin_id] 对应 coin_user_coin_balance & cnuk_user_balance</a>
> 作者 ：WYP
新表字段                 | coin_user_coin_balance   | cnuk_user_balance         | coin_user_coin_balance说明    | cnuk_user_balance说明
------------------------|--------------------------|---------------------------|-------------------------------|-----------------------------
id                      | id                       | id                         | -                            | -
user_id                 | user_id                  | uid                        | -                            | 修改     
source_type             | -                        | -                          | 新增                         | 新增    
-                       | ~~coin_id~~              | ~~coin_id~~                | -                            | -
-                       | ~~coin~~                 | -                          | 新表无此字段                  | -      
-                       | ~~coin_name~~            | ~~coin_name~~              | 新增无此字段                  | 新增无此字段
-                       | ~~addr_id~~              | ~~addr_id~~                | 新增无此字段                  | 新增无此字段
-                       | ~~addr~~                 | ~~addr~~                   | 新增无此字段                  | 新增无此字段
usable                  | usable                   | usable                     | 修改                         | 修改      
freeze                  | freeze                   | freeze                     | -                             | -      
lock_up                 | lockup                   | -                          | -                             | -
status                  | status                   | status                     | -                            | -
create_time             | add_time                 | -                          | 修改                          | -
update_time             | 无                       | -                          | 新增                          | -

## user_log
> <a id="log">user_log</a>
> 作者 ：WYP
> 说明 ：用于记录用户日志以及管理员操作日志
新表字段                 | 字段说明
------------------------|--------------------------
id                      | 自增int                       
user_id                 | userid                  
oper_type               | 操作类型
log                     | 操作数据内容
create_time             | 创建时间                 

