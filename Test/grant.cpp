#include <stdlib.h>
#include <stdio.h>

#include "UProtocol.h"

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

enum
{
	SQL_BUFF_SIZE = 4096
};
void TestMysql()
{
	string sError;
	string sHost = "127.0.0.1";
	uint16_t uiPort = 3306;
	string sUser = "grant";
	string sPwd = "123456";
	string sDB = "grant_test";

	// 创建mysql 对象，并连接数据库
	UBMysql mysql(SQL_BUFF_SIZE);
	bool bConnected = mysql.Connect(sHost, uiPort, sUser, sPwd, sDB);
	if (bConnected == false)
	{
		mysql.Error(sError);
		cout << "connect to database failed:" << mysql.Error() << ":" << sError << endl;
		return;
	}
	cout << "connect to database:" << sHost << ":" << uiPort << endl;

	// 执行一条sql语句，删除已经存在的表
	string sql = "DROP TABLE A;";
	int iRet = mysql.Exec(sql);
	cout << sql << endl;
	if (iRet < 0)
	{
		mysql.Error(sError);
		cout << "exec sql failed:" << mysql.Error() << ":" << sError << endl;
	}

	// 创建一张新表A
	sql = "create table A(id int not null default 0);";
	cout << sql << endl;
	iRet = mysql.Exec(sql);
	if (iRet < 0)
	{
		mysql.Error(sError);
		cout << "exec sql failed:" << mysql.Error() << ":" << sError << endl;
	}

	// 向表A中插入100条数据
	int iSqlLen = 0;
	char sSql[256];
	for (int i = 0; i < 100; i++)
	{
		iSqlLen = snprintf(sSql, 256, "insert into A(id) values(%u)", i);
		cout << sSql << endl;
		mysql.Exec(sSql, iSqlLen);
	}

	// 从表A中查询全部数据，并打印
	sql = "select id from A";
	cout << sql << endl;
	bool bHasValue = mysql.Select(sql);
	if (bHasValue == false)
	{
		cout << sql << " has no record" << endl;
	}

	uint32_t uiResult = 0;
	while (mysql.NextRow())
	{
		mysql.Value(0, uiResult);
		cout << uiResult << endl;
	}

	// 断开连接
	mysql.Disconnect();
}

void TestJson()
{
	string str = "";

	Json::Value root;
	root["key"] = Json::Value("value");
	UBJsonPack(str, root, true);
	cout << "json:" << str << endl;

	root.clear();
	UBJsonUnpack(root, str);

	cout << root["key"] << endl;
}

void TestJson2()
{
	string str = "{}";

	Json::Value root;
	UBJsonPack(str, root, true);

	Json::Value &v = root["key"];
	if (root["key"].isNull())
	{
		cout << "json has no key" << endl;
		return;
	}

	if (root["key"]["key2"].isNull())
	{
		cout << "json has no key.key2" << endl;
		return;
	}

	int x = root["key"]["key2"].asUInt();

	cout << "All Ok:" << x << endl;
	return;
}

void TestUBBuffer()
{
	string a = "aaaaaaaaaa";
	string b = "bbbbbbbbbb";
	string c = "cccccccccc";

	UBBuffer buffer(2048);
	buffer.AppendData(a);

	string str(buffer.Data(), buffer.DataLength());
	cout << "a only:\t" << str << endl;

	buffer.AppendData(b);
	str.assign(buffer.Data(), buffer.DataLength());
	cout << "a & b:\t" << str << endl;

	buffer.RemoveData(a.length());
	str.assign(buffer.Data(), buffer.DataLength());
	cout << "b only:\t" << str << endl;

	buffer.AppendData(c);
	str.assign(buffer.Data(), buffer.DataLength());
	cout << "b & c:\t" << str << endl;

	buffer.RemoveData(b.length());
	str.assign(buffer.Data(), buffer.DataLength());
	cout << "c only:\t" << str << endl;

	buffer.RemoveData(c.length());
	str.assign(buffer.Data(), buffer.DataLength());
	cout << "nothing left:\t" << str << endl;

	UBBuffer buffer2(2048);
	buffer2.AppendData(a);
	buffer2.AppendData(b);
	buffer2.AppendData(c);
	buffer2.Swap(&buffer);
	str.assign(buffer.Data(), buffer.DataLength());
	cout << "after swap(a & b & c):" << str << endl;

	buffer.Clear();
	str.assign(buffer.Data(), buffer.DataLength());
	cout << "after clear():" << str << endl;
}

class A
{
public:
	A(){};
	virtual ~A() { cout << "~A()" << endl; };

public:
	int a1 = 1;
	int a2 = 2;
};

class B
{
public:
	B(){};
	virtual ~B() { cout << "~B()" << endl; };

public:
	int b1 = 3;
	int b2 = 4;
};

class C : public A, public B
{
public:
	C(){};
	~C() { cout << "~C()" << endl; };

public:
	int c1 = 5;
	int c2 = 6;
};

void TestInherit()
{
	{
		C c;
		A *a = &c;
		cout << a->a1 << endl;
	}
	{
		C *c = new C;
		A *a = c;
		cout << "begin delete a" << endl;
		delete a;
		cout << "end delete a" << endl;
	}
}
/*
// return <0: 输出长度不够(所需长度负值), =0: 未执行，>0:加密后长度
int RSA_Private_Encrypt(uint8_t *pOut,int iOutLen,string &key,uint8_t *pIn,int iInLen)
{
	int iEncodeLen=RSA_KEY_BYTES - (iInLen % RSA_KEY_BYTES) + iInLen;
	if(iOutLen<iEncodeLen)return -iEncodeLen;

	BIO *bio = BIO_new_mem_buf(key.data(), key.length());
	if (bio==NULL)return 0;

	RSA *rsa= NULL;
	rsa=PEM_read_bio_RSAPrivateKey(bio, &rsa,NULL, NULL);
	if(rsa==NULL)
	{
		BIO_free(bio);
		return 0;
	}

	uint8_t from[RSA_KEY_BYTES]={0};
	uint8_t to[RSA_KEY_BYTES]={0};

	int len=0;
	uint8_t *o=pOut;
	for(uint8_t *i=pIn;i<pIn+iInLen;)
	{
		len=(pIn+iInLen-i)>RSA_KEY_BYTES ? RSA_KEY_BYTES : (pIn+iInLen-i);
		if(len==RSA_KEY_BYTES)
		{
			RSA_private_encrypt(len,i,o,rsa,RSA_NO_PADDING);
		}
		else
		{
			memcpy(from,i,len);
			RSA_private_encrypt(len,from,o,rsa,RSA_NO_PADDING);
		}
		i+=len;
		o+=len;
	}

	BIO_free(bio);
	RSA_free(rsa);

	return iEncodeLen;
}


// return <0: 输出长度不够(所需长度负值), =0: 未执行，>0:加密后长度
int RSA_Public_Decrypt(uint8_t *pOut,int iOutLen,string &key,uint8_t *pIn,int iInLen)
{
	if(iInLen%RSA_KEY_BYTES != 0)return 0;
	if(iOutLen<iInLen)return -iInLen;

	BIO *bio = BIO_new_mem_buf(key.data(), key.length());
	if (bio==NULL)return 0;

	RSA *rsa= NULL;
	rsa=PEM_read_bio_RSAPublicKey(bio, &rsa,NULL, NULL);
	if(rsa==NULL)
	{
		BIO_free(bio);
		cout<<"PEM_read_bio_RSAPublicKey failed"<<endl;
		return 0;
	}

	uint8_t from[RSA_KEY_BYTES]={0};
	uint8_t to[RSA_KEY_BYTES]={0};

	int len=0;
	uint8_t *o=pOut;
	for(uint8_t *i=pIn;i<pIn+iInLen;)
	{
		len=(pIn+iInLen-i)>RSA_KEY_BYTES ? RSA_KEY_BYTES : (pIn+iInLen-i);
//		cout<<"encode round:"<<(i-pIn)/RSA_KEY_BYTES<<","<<(i-pIn)<<"+"<<len<<"="<<(i-pIn)+len<<endl;
		if(len==RSA_KEY_BYTES)
		{
			RSA_public_decrypt(len,i,o,rsa,RSA_NO_PADDING);
		}
		else
		{
			memcpy(from,i,len);
			RSA_public_decrypt(len,from,o,rsa,RSA_NO_PADDING);
		}
		i+=len;
		o+=len;
	}

	BIO_free(bio);
	RSA_free(rsa);

	return iInLen;
}
*/

// return <0: 输出长度不够(所需长度负值), =0: 未执行，>0:加密后长度
// int RSA_Public_Encrypt(uint8_t *pOut, int iOutLen, string &key, uint8_t *pIn, int iInLen)
// {
// 	int iEncodeLen = RSA_KEY_BYTES - (iInLen % RSA_KEY_BYTES) + iInLen;
// 	if (iOutLen < iEncodeLen)
// 		return -iEncodeLen;
// 	cout << "EncodeLen:" << iEncodeLen << endl;

// 	BIO *bio = BIO_new_mem_buf(key.data(), key.length());
// 	if (bio == NULL)
// 		return 0;

// 	RSA *rsa = NULL;
// 	rsa = PEM_read_bio_RSAPublicKey(bio, &rsa, NULL, NULL);
// 	if (rsa == NULL)
// 	{
// 		BIO_free(bio);
// 		cout << "PEM_read_bio_RSAPublicKey failed" << endl;
// 		return 0;
// 	}

// 	uint8_t from[RSA_KEY_BYTES] = {0};
// 	uint8_t to[RSA_KEY_BYTES] = {0};

// 	int len = 0;
// 	uint8_t *o = pOut;
// 	for (uint8_t *i = pIn; i < pIn + iInLen;)
// 	{
// 		len = (pIn + iInLen - i) > RSA_KEY_BYTES ? RSA_KEY_BYTES : (pIn + iInLen - i);
// 		//		cout<<"encode round:"<<(i-pIn)/RSA_KEY_BYTES<<","<<(i-pIn)<<"+"<<len<<"="<<(i-pIn)+len<<endl;
// 		if (len == RSA_KEY_BYTES)
// 		{
// 			RSA_public_encrypt(len, i, o, rsa, RSA_NO_PADDING);
// 		}
// 		else
// 		{
// 			memcpy(from, i, len);
// 			RSA_public_encrypt(len, from, o, rsa, RSA_NO_PADDING);
// 		}
// 		i += len;
// 		o += len;
// 	}

// 	BIO_free(bio);
// 	RSA_free(rsa);

// 	return iEncodeLen;
// }

// // return <0: 输出长度不够(所需长度负值), =0: 未执行，>0:加密后长度
// int RSA_Private_Decrypt(uint8_t *pOut, int iOutLen, string &key, uint8_t *pIn, int iInLen)
// {
// 	if (iInLen % RSA_KEY_BYTES != 0)
// 		return 0;
// 	if (iOutLen < iInLen)
// 		return -iInLen;

// 	BIO *bio = BIO_new_mem_buf(key.data(), key.length());
// 	if (bio == NULL)
// 		return 0;

// 	RSA *rsa = NULL;
// 	rsa = PEM_read_bio_RSAPrivateKey(bio, &rsa, NULL, NULL);
// 	if (rsa == NULL)
// 	{
// 		BIO_free(bio);
// 		return 0;
// 	}

// 	uint8_t from[RSA_KEY_BYTES] = {0};
// 	uint8_t to[RSA_KEY_BYTES] = {0};

// 	int len = 0;
// 	uint8_t *o = pOut;
// 	for (uint8_t *i = pIn; i < pIn + iInLen;)
// 	{
// 		len = (pIn + iInLen - i) > RSA_KEY_BYTES ? RSA_KEY_BYTES : (pIn + iInLen - i);
// 		if (len == RSA_KEY_BYTES)
// 		{
// 			RSA_private_decrypt(len, i, o, rsa, RSA_NO_PADDING);
// 		}
// 		else
// 		{
// 			memcpy(from, i, len);
// 			RSA_private_decrypt(len, from, o, rsa, RSA_NO_PADDING);
// 		}
// 		i += len;
// 		o += len;
// 	}

// 	BIO_free(bio);
// 	RSA_free(rsa);

// 	return iInLen;
// }

void TestRSA()
{
	string sPrivateKey, sPublicKey;

	{
		/*
	* 读取保存privatekey的blowfish文件
	*/
		char buf[6200] = {0};
		char szPath[128] = {0};
		char szFullPath[256] = {0};
		getcwd(szPath, 256);
		sprintf(szFullPath, "%s%s", szPath, "/rsa_private_key_blowfish.pem");
		int fd = open(szFullPath, O_RDWR);
		if (-1 == fd)
		{
			cout << "open rsa file failure" << endl;
		}
		else
		{
			cout << "open rsa file success" << endl;
		}
		int ret = read(fd, buf, 6200);
		if (ret < -1)
		{
			cout << "read rsa file failure" << endl;
		}
		else
		{
			unsigned char *recBuf = new unsigned char[ret];
			int len = BlowfishDecrypt(recBuf, ret + 4, (const unsigned char *)buf, ret);
			sPrivateKey.assign((char *)recBuf, len);
			delete[] recBuf;
		}
		close(fd);
	}
	{
		/*
	* 读取保存publickey的blowfish文件
	*/
		char buf[6200] = {0};
		char szPath[128] = {0};
		char szFullPath[256] = {0};
		getcwd(szPath, 256);
		sprintf(szFullPath, "%s%s", szPath, "/rsa_public_key_blowfish.pem");
		int fd = open(szFullPath, O_RDWR);
		if (-1 == fd)
		{
			cout << "open rsa file failure" << endl;
		}
		else
		{
			cout << "open rsa file success" << endl;
		}
		int ret = read(fd, buf, 6200);
		if (ret < -1)
		{
			cout << "read rsa file failure" << endl;
		}
		else
		{
			unsigned char *recBuf = new unsigned char[ret];
			int len = BlowfishDecrypt(recBuf, ret, (const unsigned char *)buf, ret);
			sPublicKey.assign((char *)recBuf, len);
			delete[] recBuf;
		}
		close(fd);
	}

	// RsaKeyGen(sPrivateKey,sPublicKey);
	cout << "PrivateKey:" << endl;
	cout << sPrivateKey << endl;
	cout << "PublicKey:" << endl;
	cout << sPublicKey << endl;
	cout << "xxx" << endl;

	/* 模拟发送数据 */
	srand((unsigned)time(NULL));
	stringstream ss;
	string sRandom;
	ss << (uint64_t)rand();
	ss >> sRandom;

	Json::Value sJson;
	sJson["mid"] = "138";
	sJson["key"] = sRandom.c_str();

	Json::FastWriter writer;
	std::string hash = writer.write(sJson);

	string str;
	enum
	{
		DATA_LEN = 5945,
		ENC_LEN = 6200,
		DEC_LEN = 6200,
	};
	uint8_t *data = new uint8_t[DATA_LEN];
	// memset(data, 'C', DATA_LEN);
	memset(data, 0, DATA_LEN);
	memcpy(data, hash.c_str(), hash.length());
	uint8_t *enc = new uint8_t[ENC_LEN];
	uint8_t *dec = new uint8_t[DEC_LEN];

	// cout << "private key crypto" << endl;
	// int ret = RSA_Private_Encrypt(enc, ENC_LEN, sPrivateKey, data, DATA_LEN);
	// if (ret > 0)
	// {
	// 	Base64Encode(str, (char *)enc, ret);
	// 	cout << str << endl;
	// }

	// cout << "public key decrypto" << endl;
	// ret = RSA_Public_Decrypt(dec, DEC_LEN, sPublicKey, enc, ret);
	// cout << ret << endl;
	// if (ret > 0)
	// {
	// 	str.assign((char *)dec, ret);
	// 	cout << str << endl;
	// }

	cout << "public key crypto" << endl;
	int ret = RSA_Public_Encrypt(enc, ENC_LEN, sPublicKey, data, DATA_LEN);
	cout << ret << endl;
	if (ret > 0)
	{
		Base64Encode(str, (char *)enc, ret);
		cout << str << endl;
	}

	cout << "private key decrypto:" << ret << endl;
	ret = RSA_Private_Decrypt(dec, DEC_LEN, sPrivateKey, enc, ret);
	cout << ret << endl;
	if (ret > 0)
	{
		str.assign((char *)dec, hash.length());
		cout << str << endl;
	}

	delete[] data;
	delete[] enc;
	delete[] dec;
}

void TestLogin()
{
	Json::Value json;
	json["key"] = Json::Value("Value");

	string sPrivateKey, sPublicKey;
	RsaKeyGen(sPrivateKey, sPublicKey);
	cout << "PrivateKey:" << endl;
	cout << sPrivateKey << endl;
	cout << "PublicKey:" << endl;
	cout << sPublicKey << endl;

	UPLogin ul;
	int ret = ul.ClientRequest(sPrivateKey, json);
	cout << "encode ret:" << ret << endl;
	{
		string str;
		Base64Encode(str, (char *)ul.key().data(), ul.key().length());
		cout << str << endl;
	}

	ret = ul.ServerCheck(json, sPublicKey);
	cout << "decode ret:" << ret << endl;
	cout << "json:" << json << endl;
}

void TestGetCwd()
{
#define MAXBUFSIZE 1024
	char buf[MAXBUFSIZE];
	int count = readlink("/proc/self/exe", buf, MAXBUFSIZE);
	if (count < 0 || count >= MAXBUFSIZE)
	{
		cout << "failed" << endl;
		return;
	}
	buf[count] = '\0';
	cout << "/proc/self/exe -> " << buf << endl;
}

void TestBlowfish(string &s1)
{
	enum
	{
		BUFF_LEN = 1024
	};
	char sEncode[BUFF_LEN] = {0};
	int iOut = BlowfishEncrypt((uint8_t *)sEncode, BUFF_LEN, (uint8_t *)s1.data(), s1.length());
	cout << "encode len:" << iOut << endl;

	char str[BUFF_LEN] = {0};
	iOut = BlowfishDecrypt((uint8_t *)str, BUFF_LEN, (uint8_t *)sEncode, iOut);
	cout << "decode len:" << iOut << endl;

	cout << str << endl;
}

void TestBlowfish()
{
	string s1 = "12345";
	TestBlowfish(s1);

	string s2;
	for (int i = 0; i < 100; i++)
		s2.append(1, 't');
	TestBlowfish(s2);
}

void UBMysql_Test()
{
	UBMysql mysql(2048);
	//	UBMysql mysql(pSqlBuffer,iSqlBufferLen);
	string userAccount = "Fiona2059@qq.com";
	string st = "1";
	string userId = "";
	string sTimeTemp = "1586245589";
	string password = "123456";

	string sUnixSock = "192.168.1.157", sUser = "zhang", sPwd = "123456", sDB = "account";
	cout << "connect to mysql:" << sUnixSock << endl;
	// UBMysql::Connect(string &host, int port, string &user, string &pwd, string &db)
	if (mysql.Connect(sUnixSock, 3306, sUser, sPwd, sDB) == false)
	{
		int iLastError = mysql.Error();
		cout << "faield:" << iLastError << endl;

		string sLastError;
		mysql.Error(sLastError);
		cout << "faield:" << sLastError << endl;
		return;
	}

	// // 测试直接传入sql语句 （insert 和 update）
	// char sql[] = "insert into t1(name) values('test');";
	// int iModifiedRows = mysql.Exec(sql, strlen(sql));

	// // 测试在内部生成sql语句 （insert 和 update）
	// mysql.Sql("insert into t1(name) values('", false,true);
	// mysql.Sql("abc')",false);
	// iModifiedRows = mysql.Exec();

	// cout<<"select"<<endl;
	// 测试select 方式
	string sSqls = "select id from user where user_account = \"" + userAccount + "\";";
	bool bOK = mysql.Select(sSqls);
	if (bOK == true)
	{
		if (mysql.NextRow() == false)
		{
			mysql.Sql("INSERT INTO user(user_account,password,source_type,login_attempts,login_time,create_time,update_time) VALUES(\"", false, true);
			mysql.Sql(userAccount, true, false);
			mysql.Sql("\",\"", false, false);
			mysql.Sql(password, true, false);
			mysql.Sql("\",\"", false, false);
			// string st = std::to_string(sourceType);
			mysql.Sql(st, true, false);
			mysql.Sql("\",1,", false, false);
			mysql.Sql(sTimeTemp, true, false);
			mysql.Sql(",", false, false);
			mysql.Sql(sTimeTemp, true, false);
			mysql.Sql(",", false, false);
			mysql.Sql(sTimeTemp, true, false);
			mysql.Sql(");", false, false);

			if (mysql.Exec() >= 0)
			{
				// cout << "mysql.LastID() = " << mysql.LastID() << endl;
				userId = to_string(mysql.LastID());

				// 插入user_account表
				mysql.Sql("INSERT INTO user_account(account,account_type,source_type,user_id,ukex_uid,ukexpay_uid,create_time,update_time) VALUES (\"", false, true);
				mysql.Sql(userAccount, true, false);
				mysql.Sql("\",1,\"", false, false);
				mysql.Sql(st, true, false);
				mysql.Sql("\",", false, false);
				mysql.Sql(userId, true, false);
				mysql.Sql(",0,0,", false, false);
				mysql.Sql(sTimeTemp, true, false);
				mysql.Sql(",", false, false);
				mysql.Sql(sTimeTemp, true, false);
				mysql.Sql(");", false, false);

				// 插入user_auth表
				mysql.Sql("INSERT INTO user_auth(user_id,is_allow_login,paypassword_status,user_status,auth_status,kyc_auth,email_auth,allow_openapi_withdrawal,is_reauth,is_pass_aml,register_type,verify_type,global_kyc_status,last_operate_time,account_status,os_verify,pin_verify,two_factor,hide_balance,create_time,update_time) VALUES (\"", false, false);
				mysql.Sql(userId, true, false);
				mysql.Sql("\",1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,", false, false);
				mysql.Sql(sTimeTemp, true, false);
				mysql.Sql(",", false, false);
				mysql.Sql(sTimeTemp, true, false);
				mysql.Sql(");", false, false);

				// 插入user_facts表
				mysql.Sql("INSERT INTO user_facts(user_id,first_name,last_name,birthday,country,residence_country,province,city,town,district,street_address,building_number,building_name,sex,zip_code,apartment,first_residence_certificate,second_residence_certificate,street_code,unit,house_code,passport,has_deducted_gbp,has_deducted_eur,has_deducted_usd,passport_ocr,passport_expire,passport_picture,passport_picture_hold,driving_number,driving_picture,driving_picture_hold,is_europe,nationality,create_time,update_time) VALUES (\"", false, false);
				mysql.Sql(userId, true, false);
				mysql.Sql("\",'','',0,'','','','','','','','','',0,'','','','','','','','',0,0,0,'','','','','','','',0,'',\"", false, false);
				mysql.Sql(sTimeTemp, true, false);
				mysql.Sql(",", false, false);
				mysql.Sql(sTimeTemp, true, false);
				mysql.Sql(");", false, false);

				// 插入user_info表
				mysql.Sql("INSERT INTO user_info(user_id,mobile,id_type,identity_number,identity_expiry_date,identity_info,paypassword,recommender_level1,recommender_level2,recommender_level3,realname,country,c2c_info_ext,ext,invit_time,source_of_funds,mobile_binding_time,id_number,ip_address,address_desc,email,wechat,wechat_img,wechat_openid,alipay,alipay_img,invite_code,custom_cate_id,master_account,source_proj_uid,uuid,uuid_salt,user_image,country_code,user_type,account_active_time,secret_key,refer_currency,kyc_refuse_num,lucky_count,c2c_refuse_num,rescue_code,create_time,update_time) VALUES (", false, false);
				mysql.Sql(userId, true, false);
				mysql.Sql(",'',0,'','',NULL,'','','','','','',NULL,NULL,0,NULL,0,'','','','','','','','','','',0,0,0,'','','','',0,0,'',0,0,0,0,'',", false, false);
				mysql.Sql(sTimeTemp, true, false);
				mysql.Sql(",", false, false);
				mysql.Sql(sTimeTemp, true, false);
				mysql.Sql(");", false, false);

				// mysql.Sql("insert into t1(name) values('", false,true);
				// mysql.Sql("xxx')",false);
				int iModifiedRows = mysql.Exec();

				mysql.Sql("select user_id from user_account where account in (select user_account from user where user_account =  \"", false, true);
				mysql.Sql(userAccount, true, false);
				mysql.Sql("\" and password = \"", false, false);
				mysql.Sql(password, true, false);
				mysql.Sql("\");", false, false);
				cout << mysql.getSql() << endl;
				if (mysql.Select())
				{
					cout << "success" << endl;
				}
				else
				{
					cout << "failed" << endl;
				}
			}
		}

		// 	uint32_t uiIndex = 0;

		// 	uint32_t uiValue = 0;
		// 	string sValue;
		// 	int x=0;
		// 	while (mysql.NextRow())
		// 	{
		// 		x++;
		// 		if(x>50)break;
		// 		cout<<x<<" ";
		// 		uiIndex = 0;
		// 		mysql.Value(uiIndex++, uiValue);
		// 		mysql.Value(uiIndex++, sValue);
		// 		cout<<"id:"<<uiValue<<",name:"<<sValue<<endl;
		// 	}
	}
	// cout<<endl;

	// sSqls = "select id,name from t1;";
	// bOK = mysql.Select(sSqls);
	// if (bOK)
	// {
	// 	uint32_t uiIndex = 0;

	// 	uint32_t uiValue = 0;
	// 	string sValue;
	// 	int x=0;
	// 	while (mysql.NextRow())
	// 	{
	// 		x++;
	// 		if(x>50)break;
	// 		cout<<x<<" ";
	// 		uiIndex = 0;
	// 		mysql.Value(uiIndex++, uiValue);
	// 		mysql.Value(uiIndex++, sValue);
	// 		cout<<"id:"<<uiValue<<",name:"<<sValue<<endl;
	// 	}
	// }
	// cout<<endl;

	// mysql.Sql("insert into t1(name) values('", false, true);
	// mysql.Sql("112233')",false);
	// iModifiedRows = mysql.Exec();
	// cout<<mysql.getSql()<<endl;
}

int main()
{
	//	TestMysql();
	//	TestJson();
	//	TestJson2();
	//	TestUBBuffer();
	//  TestInherit();
	// TestGetCwd();
	// TestBlowfish();
	//	TestRSA();
	//	TestLogin();
	UBMysql_Test();

	// string hash;
	// string x="jslkdfjkljskldf ";
	// Sha256(hash,(uint8_t*)x.data(),x.length());

	// string str;
	// Base64Encode(str,hash.data(),hash.length());
	// cout<<str<<endl;

	return 0;
}
