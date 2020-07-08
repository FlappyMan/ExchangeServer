
#ifndef _UBCRYPTO_HEADER_
#define _UBCRYPTO_HEADER_

#include "UBHeader.h"
#include "openssl/evp.h"
#include "openssl/bio.h"
#include "openssl/buffer.h"
#include "openssl/sha.h"
#include "openssl/rsa.h"
#include <openssl/pem.h>
#include <openssl/bn.h>
#include <openssl/blowfish.h>
#include <openssl/aes.h>
#include <openssl/md5.h>

bool Base64Encode(string &strEncoded,const char *pData, int iDataLen, bool bNewLine=false);
bool Base64Decode(string &strDecoded,const char *pData, int iDataLen, bool bNewLine=false);

enum{RSA_KEY_BITS=2048,RSA_KEY_BYTES=256};
bool RsaKeyGen(string &sPrivateKey,string &sPublicKey);

// return <0: 输出长度不够(所需长度负值), =0: 未执行，>0:加密后长度
int RSA_Private_Encrypt(uint8_t *pOut,int iOutLen,string &key,uint8_t *pIn,int iInLen);
int RSA_Public_Decrypt(uint8_t *pOut,int iOutLen,string &key,uint8_t *pIn,int iInLen);
// return <0: 输出长度不够(所需长度负值), =0: 未执行，>0:加密后长度
int RSA_Private_Decrypt(uint8_t *pOut,int iOutLen,string &key,uint8_t *pIn,int iInLen);
// return <0: 输出长度不够(所需长度负值), =0: 未执行，>0:加密后长度
int RSA_Public_Encrypt(uint8_t *pOut,int iOutLen,string &key,uint8_t *pIn,int iInLen);

/*pOut 加密后数据; iOutLen 加密后数据长度; pIn 待加密数据; iInLen 待加密数据长度*/
int BlowfishEncrypt(unsigned char *pOut,int iOutLen, const unsigned char *pIn, int iInLen);
/*pOut 解密后数据; iOutLen 解密后数据长度; pIn 待解密数据; iInLen 待解密数据长度*/
int BlowfishDecrypt(unsigned char *pOut,int iOutLen, const unsigned char *pIn, int iInLen);

/*pOut 加密后数据; iOutLen 加密后数据长度; pIn 待加密数据; iInLen 待加密数据长度*/
int AesEncrypt(unsigned char *pOut,int iOutLen, const unsigned char *pIn, int iInLen);
/*pOut 解密后数据; iOutLen 解密后数据长度; pIn 待解密数据; iInLen 待解密数据长度 */
int AesDecrypt(unsigned char *pOut,int iOutLen, const unsigned char *pIn, int iInLen);

int MD5(unsigned char *pDst, const unsigned char *pSrc, int iSrcLen);

void Sha256(string &hash,const uint8_t *pData,uint32_t uiLen);

#endif
