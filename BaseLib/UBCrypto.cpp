
#include "UBCrypto.h"

bool Base64Encode(string &strEncoded, const char *pData, int iDataLen, bool bNewLine)
{
    BIO *b64 = BIO_new(BIO_f_base64());
    if (!bNewLine)
    {
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    }
    BIO *bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, pData, iDataLen);
    BIO_flush(b64);

    BUF_MEM *bptr;
    BIO_get_mem_ptr(b64, &bptr);
    BIO_set_close(b64, BIO_NOCLOSE);

    strEncoded.assign(bptr->data, bptr->length);
    BIO_free_all(b64);
    return true;
}

// base64 解码
bool Base64Decode(string &strDecoded, const char *pData, int iDataLen, bool bNewLine)
{
    strDecoded.clear();

    char *buffer = new char[iDataLen];
    BIO *b64 = BIO_new(BIO_f_base64());
    if (!bNewLine)
    {
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    }
    BIO *bmem = BIO_new_mem_buf(pData, iDataLen);
    bmem = BIO_push(b64, bmem);
    iDataLen = BIO_read(bmem, buffer, iDataLen);
    BIO_free_all(bmem);

    if (iDataLen > 0)
    {
        strDecoded.assign(buffer, iDataLen);
        delete[] buffer;
        return true;
    }
    delete[] buffer;
    return false;
}

bool RsaKeyGen(string &sPrivateKey, string &sPublicKey)
{
    sPrivateKey.clear();
    sPublicKey.clear();

    RSA *rsa = RSA_new();
    BIGNUM *bne = BN_new();
    int ret = BN_set_word(bne, RSA_F4);
    if (ret == 0)
        return false;
    ret = RSA_generate_key_ex(rsa, RSA_KEY_BITS, bne, NULL);
    if (ret == 0)
        return false;

    BIO *pPrivateKey = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPrivateKey(pPrivateKey, rsa, NULL, NULL, 0, NULL, NULL);
    size_t uiPrivate = BIO_pending(pPrivateKey);
    char *buffer = new char[uiPrivate];
    BIO_read(pPrivateKey, buffer, uiPrivate);
    sPrivateKey.assign(buffer, uiPrivate);
    delete[] buffer;

    BIO *pPublicKey = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pPublicKey, rsa);
    size_t uiPublic = BIO_pending(pPublicKey);
    buffer = new char[uiPublic];
    BIO_read(pPublicKey, buffer, uiPublic);
    sPublicKey.assign(buffer, uiPublic);
    delete[] buffer;

    RSA_free(rsa);
}

// return <0: 输出长度不够(所需长度负值), =0: 未执行，>0:加密后长度
int RSA_Private_Encrypt(uint8_t *pOut, int iOutLen, string &key, uint8_t *pIn, int iInLen)
{
    int iEncodeLen = RSA_KEY_BYTES - (iInLen % RSA_KEY_BYTES) + iInLen;
    if (iOutLen < iEncodeLen)
        return -iEncodeLen;

    BIO *bio = BIO_new_mem_buf(key.data(), key.length());
    if (bio == NULL)
    {
        return 0;
    }

    RSA *rsa = NULL;
    rsa = PEM_read_bio_RSAPrivateKey(bio, &rsa, NULL, NULL);
    if (rsa == NULL)
    {
        BIO_free(bio);
        return 0;
    }

    uint8_t from[RSA_KEY_BYTES] = {0};
    uint8_t to[RSA_KEY_BYTES] = {0};

    int len = 0;
    uint8_t *o = pOut;
    for (uint8_t *i = pIn; i < pIn + iInLen;)
    {
        len = (pIn + iInLen - i) > RSA_KEY_BYTES ? RSA_KEY_BYTES : (pIn + iInLen - i);
        if (len == RSA_KEY_BYTES)
        {
            RSA_private_encrypt(len, i, o, rsa, RSA_NO_PADDING);
        }
        else
        {
            memcpy(from, i, len);
            RSA_private_encrypt(len, from, o, rsa, RSA_NO_PADDING);
        }
        i += len;
        o += len;
    }

    BIO_free(bio);
    RSA_free(rsa);

    return iEncodeLen;
}
// return <0: 输出长度不够(所需长度负值), =0: 未执行，>0:加密后长度
int RSA_Public_Decrypt(uint8_t *pOut, int iOutLen, string &key, uint8_t *pIn, int iInLen)
{
    if (iInLen % RSA_KEY_BYTES != 0)
        return 0;
    if (iOutLen < iInLen)
        return -iInLen;

    BIO *bio = BIO_new_mem_buf(key.data(), key.length());
    if (bio == NULL)
        return 0;

    RSA *rsa = NULL;
    rsa = PEM_read_bio_RSAPublicKey(bio, &rsa, NULL, NULL);
    if (rsa == NULL)
    {
        BIO_free(bio);
        cout << "PEM_read_bio_RSAPublicKey failed" << endl;
        return 0;
    }

    uint8_t from[RSA_KEY_BYTES] = {0};
    uint8_t to[RSA_KEY_BYTES] = {0};

    int len = 0;
    uint8_t *o = pOut;
    for (uint8_t *i = pIn; i < pIn + iInLen;)
    {
        len = (pIn + iInLen - i) > RSA_KEY_BYTES ? RSA_KEY_BYTES : (pIn + iInLen - i);
        if (len == RSA_KEY_BYTES)
        {
            RSA_public_decrypt(len, i, o, rsa, RSA_NO_PADDING);
        }
        else
        {
            memcpy(from, i, len);
            RSA_public_decrypt(len, from, o, rsa, RSA_NO_PADDING);
        }
        i += len;
        o += len;
    }

    BIO_free(bio);
    RSA_free(rsa);

    return iInLen;
}

// return <0: 输出长度不够(所需长度负值), =0: 未执行，>0:加密后长度
int RSA_Public_Encrypt(uint8_t *pOut,int iOutLen,string &key,uint8_t *pIn,int iInLen)
{
	int iEncodeLen=RSA_KEY_BYTES - (iInLen % RSA_KEY_BYTES) + iInLen;
	if(iOutLen<iEncodeLen)return -iEncodeLen;
	cout<<"EncodeLen:"<<iEncodeLen<<endl;

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
			RSA_public_encrypt(len,i,o,rsa,RSA_NO_PADDING);
		}
		else
		{
			memcpy(from,i,len);
			RSA_public_encrypt(len,from,o,rsa,RSA_NO_PADDING);
		}
		i+=len;
		o+=len;
	}

	BIO_free(bio);
	RSA_free(rsa);

	return iEncodeLen;
}


// return <0: 输出长度不够(所需长度负值), =0: 未执行，>0:加密后长度
int RSA_Private_Decrypt(uint8_t *pOut,int iOutLen,string &key,uint8_t *pIn,int iInLen)
{
	if(iInLen%RSA_KEY_BYTES != 0)return 0;
	if(iOutLen<iInLen)return -iInLen;

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
			RSA_private_decrypt(len,i,o,rsa,RSA_NO_PADDING);
		}
		else
		{
			memcpy(from,i,len);
			RSA_private_decrypt(len,from,o,rsa,RSA_NO_PADDING);
		}
		i+=len;
		o+=len;
	}

	BIO_free(bio);
	RSA_free(rsa);

	return iInLen;
}

void Sha256(string &hash, const uint8_t *pData, uint32_t uiLen)
{
    uint8_t out[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, pData, uiLen);
    SHA256_Final(out, &sha256);

    hash.assign((char *)out, SHA256_DIGEST_LENGTH);
}
/*
 *  1、Blowfish默认分组大小BF_BLOCK=8Byte;
 *	2、in_size为待加密字符串的实际长度(不包含字符串结束符'\0'),out_size为加密后字符串的实际长度(不包含字符串结束符'\0');
 * 	   BF_BLOCK分组加密大小为8Byte,加密后的out_size为8的整数倍;
 *	3、如果待加密的字符串长度(不包含字符串结束符'\0')不是8的整数倍,补齐整数倍并以'0'填充;
 * 	4、例:in_size = 7Byte,则in补齐到8Byte，out_size = 8Byte;如in_size = 8Byte,则out_size = 8Byte
 */
const char g_blowfish_key[BF_BLOCK + BF_BLOCK] = "UkexServerKey";
int BlowfishEncrypt(unsigned char *pOut, int iOut, const unsigned char *pIn, int iIn)
{
    bool bLastBlock=false;
    int iBlockCnt = iIn / BF_BLOCK;
    if(iIn%BF_BLOCK!=0)
    {
        iBlockCnt++;
        bLastBlock=true;
    }
    
    int iOutLen = iBlockCnt * BF_BLOCK;
    if (iOut < iOutLen)
    {
        cout << "BF_ENCRYPT Arg 3 need a larger size: " << -iOutLen << endl;
        return -iOutLen;
    }

    BF_KEY bf_key;
    BF_set_key(&bf_key, BF_BLOCK + BF_BLOCK, (uint8_t*)g_blowfish_key);

    uint8_t tmp[BF_BLOCK];
    for(int pos=0;pos<iOutLen;pos+=BF_BLOCK)
    {
        if(bLastBlock && (pos+BF_BLOCK == iOutLen) )
        {
            memset(tmp,0,BF_BLOCK);
            memcpy(tmp,pIn,iIn-pos);
            BF_ecb_encrypt(tmp, pOut+pos, &bf_key, BF_ENCRYPT);
            break;
        }
        BF_ecb_encrypt(pIn+pos, pOut+pos, &bf_key, BF_ENCRYPT);
    }
    return iOutLen;
}

int BlowfishDecrypt(unsigned char *pOut, int iOut, const unsigned char *pIn, int iIn)
{
    if(iIn%BF_BLOCK !=0 )return 0;
    if (iOut < iIn)
    {
        cout << "BF_ENCRYPT Arg 3 need a larger size: " << iOut << endl;
        return -iOut;
    }

    BF_KEY bf_key;
    BF_set_key(&bf_key, BF_BLOCK + BF_BLOCK, (uint8_t*)g_blowfish_key);

    for(int pos=0;pos<iIn;pos+=BF_BLOCK)
    {
        BF_ecb_encrypt(pIn + pos, pOut + pos, &bf_key, BF_DECRYPT);
    }
    return iIn;
}
/*
 *  1、AES默认分组大小AES_BLOCK_SIZE=16Byte;
 *	2、in_size为待加密字符串的实际长度(不包含字符串结束符'\0'),out_size为加密后字符串的实际长度(不包含字符串结束符'\0');
 * 	   AES_BLOCK_SIZE分组加密大小为16Byte,加密后的out_size为16的整数倍;
 *	3、如果待加密的字符串长度(不包含字符串结束符'\0')不是16的整数倍,补齐整数倍并以'0'填充;
 * 	4、例:in_size = 15Byte,则in补齐到16Byte，out_size = 16Byte;如in_size = 16Byte,则out_size = 16Byte
 */
const char g_aes_key[AES_BLOCK_SIZE] = "UkexServerKey";
int AesEncrypt(unsigned char *pOut, int iOut, const unsigned char *pIn, int iIn)
{
    bool bLastBlock=false;
    int iBlockCnt = iIn / AES_BLOCK_SIZE;
    if (iIn % AES_BLOCK_SIZE!=0)
    {
        iBlockCnt++;
        bLastBlock=true;
    }
    
    int iOutLen = iBlockCnt * AES_BLOCK_SIZE;
    if (iOut < iOutLen)
    {
        cout << "AesEncrypt Arg 2 need a larger size: " << -iOutLen << endl;
        return -iOutLen;
    }
    
    AES_KEY aes_key;
    AES_set_encrypt_key((const unsigned char *)g_aes_key, AES_BLOCK_SIZE * 8, &aes_key);    

    unsigned char tmp[AES_BLOCK_SIZE];
    for (size_t pos = 0; pos < iOutLen; pos += AES_BLOCK_SIZE)
    {
        if (bLastBlock && (pos+AES_BLOCK_SIZE == iOutLen))
        {
            memset(tmp,0,AES_BLOCK_SIZE);
            memcpy(tmp,pIn,iIn-pos);
            AES_encrypt(tmp,pOut+pos,&aes_key);
        }
        AES_encrypt(pIn+pos,pOut+pos,&aes_key);
    }
    return iOutLen;
}

int AesDecrypt(unsigned char *pOut, int iOut, const unsigned char *pIn, int iIn)
{
    if(iIn % AES_BLOCK_SIZE !=0 )return 0;
    if (iOut < iIn)
    {
        cout << "AesDecrypt Arg 2 need a larger size: " << -iOut << endl;
        return -iOut;
    }

    AES_KEY aes_key;
    AES_set_decrypt_key((const unsigned char *)g_aes_key, AES_BLOCK_SIZE * 8, &aes_key);

    for(int pos=0;pos<iIn;pos+=AES_BLOCK_SIZE)
    {
        AES_decrypt(pIn + pos, pOut + pos, &aes_key);
    }
    return iIn;
}

int MD5(unsigned char *pDst,const unsigned char *pSrc, int iSrcLen)
{
    MD5_CTX md5_ctx;
    unsigned char md5[MD5_DIGEST_LENGTH] = {0};

    MD5_Init(&md5_ctx);
    MD5_Update(&md5_ctx,pSrc,iSrcLen);
    MD5_Final(md5,&md5_ctx);
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        sprintf((char*)pDst+i*2,"%02x",md5[i]);
    } 
    return 0;
}