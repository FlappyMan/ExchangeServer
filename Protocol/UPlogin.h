#ifndef _UPLOGIN_H_
#define _UPLOGIN_H_

#include "UBaseLib.h"
#include "proto/ukex.login.pb.h"
#include "UProtocolBase.h"

/*
FrontTrade|FrontMarket / BackTrade 之间预先第一个认证协议(json格式, json编码长度不超过 256 个字节)
BackTrade 将 Private key 写在代码中
FrontTrade/FrontMarket 将 Public key 写在配置文件中 
Private / Public Key 由 RSA（256byte） 生成, 可以调用 UBCrypto 中的 RsaKeyGen() 生成

BackTrade 在建立好连接后，发送的第一个包必须是 UPLogin 包, 包内是一个 json，上层可加入少量业务信息(比如 market)
并在使用 pb 打包前必须调用 ClientRequest() 生成包内容以及传入业务需要的少量数据

FrontTrade/FrontMarket 收到第一个包必须是 UPLogin 包, 收到第一个包后先用 pb 解包，
然后调用 ServerCheck() 来验证登陆包，并获得业务层信息（比如market）

*/

/*
BackTrade -> FrontMarket 完整json: {mid:"1",mn:"btc_usdt",key:"123"}
BackTrade -> FrontTrade 完整json：{id:"1",mn:"btc_usdt",key:"123"}
*/


class UPLogin : public UProtocolBase, public ukex::login
{
public:
	UPLogin(){};
	virtual ~UPLogin(){};

	// return <0: failed, 0: success
	int ClientRequest(string &sPrivateKey,Json::Value &root,bool bIsPriKey = true);

	// return <0: failed, 0: success
	int ServerCheck(Json::Value &root,string &sPublicKey,bool bIsPubKey = true);

	void JsonPack(Json::Value &root){};
    bool JsonUnpack(Json::Value &root){return true;};
};

#endif
