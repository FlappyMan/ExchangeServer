
#include "UPlogin.h"

// return <0: failed, 0: success
int UPLogin::ClientRequest(string &sPrivateKey,Json::Value &root,bool bIsPriKey)
{
	string sKey=to_string(random());
	set_key(sKey);

	enum{JSON_LEN=RSA_KEY_BYTES};
	string sJson;
	root["key"]=Json::Value(sKey);
	if(UBJsonPack(sJson,root,false)==false)
	{
		root.removeMember("key");
		return -2;
	}
	root.removeMember("key");
 	if(sJson.length()>JSON_LEN)return -3;

    BIO *bio = BIO_new_mem_buf(sPrivateKey.data(), sPrivateKey.length());
	if (bio==NULL)return -2;

	RSA *rsa= NULL;
	if (true == bIsPriKey)
	{
		rsa=PEM_read_bio_RSAPrivateKey(bio, &rsa,NULL, NULL);
		
	}
	else
	{
		rsa=PEM_read_bio_RSAPublicKey(bio, &rsa,NULL, NULL);
	}
	
	if(rsa==NULL)
	{
		BIO_free(bio);
		return -4;
	}

    uint8_t to[JSON_LEN]={0};
    uint8_t from[JSON_LEN]={0};
    memcpy(from,sJson.data(),sJson.length());
    int ret = -1;
	if (true == bIsPriKey)
	{
		ret = RSA_private_encrypt(JSON_LEN,from,to,rsa,RSA_NO_PADDING);
	}
	else
	{
		ret = RSA_public_encrypt(JSON_LEN,from,to,rsa,RSA_NO_PADDING);
	}
	
    if(ret<0)return -5;
	BIO_free(bio);
	RSA_free(rsa);

    set_hash((char*)to,JSON_LEN);
    return 0;
}

// return <0: failed, 0: success
int UPLogin::ServerCheck(Json::Value &root,string &sPublicKey,bool bIsPubKey)
{
    enum{JSON_LEN=RSA_KEY_BYTES};
    if(hash().length()!=JSON_LEN)return -1;

    BIO *bio = BIO_new_mem_buf(sPublicKey.data(), sPublicKey.length());
	if (bio==NULL)return -2;

	RSA *rsa= NULL;
	if (true == bIsPubKey)
	{
		rsa=PEM_read_bio_RSAPublicKey(bio, &rsa,NULL, NULL);
	}
	else
	{
		rsa=PEM_read_bio_RSAPrivateKey(bio, &rsa,NULL, NULL);
	}
	
	
	if(rsa==NULL)
	{
		BIO_free(bio);
		return -3;
	}

	uint8_t from[JSON_LEN]={0};
	uint8_t to[JSON_LEN+1]={0};
    memcpy(from,hash().data(),hash().length());
	
	if (true == bIsPubKey)
	{
		RSA_public_decrypt(JSON_LEN,from,to,rsa,RSA_NO_PADDING);
	}
    else
	{
		RSA_private_decrypt(JSON_LEN,from,to,rsa,RSA_NO_PADDING);	
	}
	
	BIO_free(bio);
	RSA_free(rsa);

    string sJson=(char*)to;
	UBJsonUnpack(root,sJson);
	
	string sKey=root["key"].asString();
	root.removeMember("key");
	if(sKey.compare(key())!=0)return -4;
    return 0;
}