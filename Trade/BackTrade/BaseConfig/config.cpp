
#include "config.h"

string Config::g_sHttpIP = "0.0.0.0";
uint16_t Config::g_uiHttpPort = 0;

string Config::g_sUserCenterIP = "0.0.0.0";
uint16_t Config::g_uiUserCenterPort = 0;

uint32_t Config::uiMarketID = 0;
string Config::sMarket = "btc_usdt";

string Config::sHost = "192.168.1.20";
uint16_t Config::uiPort = 3306;
string Config::sUser = "wuyoupeng";
string Config::sPwd = "123456";
string Config::sAccountDB = "account";
string Config::sAdminDB = "admin";
string Config::sAssetsDB = "assets";
string Config::sAssetsLogDB = "assetslog";
string Config::sBaseDB = "base";
string Config::sExassetsDB = "exassets";
string Config::sKycDB = "kyc";
string Config::sOtcDB = "otc";
string Config::sOtherDB = "other";
string Config::sSaDB = "sa";
string Config::sTradeDB = "trade";
string Config::sCoinA;
string Config::sCoinB;
string Config::sRSAUserCenterPubKey = "";
string Config::sRSAFrontTradePriKey = "";
string Config::currencyName;
string Config::webIcon;
string Config::appIcon;
string Config::currency;
string Config::recordDir;

vecConnectServer Config::g_vecConnectServers;
vecConnectServer Config::g_vecConnectMarkets;

Config::Config()
{
}

Config::~Config()
{
}

bool Config::Load(string &sPath)
{
    Json::Value root;
    fstream fs(sPath);
    if (!fs)
        return false;
    stringstream ss;
    ss << fs.rdbuf();
    string str(ss.str());
    bool ret = UBJsonUnpack(root, str);
    if (!ret)
        return false;

    int iServerSize = root["connect_server"].size();
    for (int i = 0; i < iServerSize; i++)
    {
        ConnectServer server;
        server.m_sConnectIP = root["connect_server"][i]["ip"].asString();
        server.m_usConnectPort = root["connect_server"][i]["port"].asUInt64();
        server.m_sConnectKey = root["connect_server"][i]["key"].asString();
        Config::g_vecConnectServers.push_back(server);
    }
    int iMarketSize = root["connect_market"].size();
    for (int i = 0; i < iMarketSize; i++)
    {
        ConnectServer server;
        server.m_sConnectIP = root["connect_market"][i]["ip"].asString();
        server.m_usConnectPort = root["connect_market"][i]["port"].asUInt64();
        server.m_sConnectKey = root["connect_market"][i]["key"].asString();
        Config::g_vecConnectMarkets.push_back(server);
    }

    Config::g_uiHttpPort = root["accept_http_port"].asUInt64();
    Config::g_sHttpIP = root["accept_http_ip"].asString();

    Config::g_sUserCenterIP = root["usercenter_ip"].asString();
    Config::g_uiUserCenterPort = root["usercenter_port"].asUInt64();

    Config::sMarket = root["market"].asString();
    Config::uiMarketID = root["marketID"].asUInt64();

    Config::sHost = root["host"].asString();
    Config::uiPort = root["port"].asUInt64();
    Config::sUser = root["user"].asString();
    Config::sPwd = root["password"].asString();
    Config::sAccountDB = root["account"].asString();
    Config::sAdminDB = root["admin"].asString();
    Config::sAssetsDB = root["assets"].asString();
    Config::sAssetsLogDB = root["assetslog"].asString();
    Config::sBaseDB = root["base"].asString();
    Config::sExassetsDB = root["exassets"].asString();
    Config::sKycDB = root["kyc"].asString();
    Config::sOtcDB = root["otc"].asString();
    Config::sOtherDB = root["other"].asString();
    Config::sSaDB = root["sa"].asString();
    Config::sTradeDB = root["trade"].asString();
    Config::sCoinA = root["coinA_ID"].asString();
    Config::sCoinB = root["coinB_ID"].asString();
    Config::currencyName = root["currency_name"].asString();
    Config::webIcon = root["web_icon"].asString();
    Config::appIcon = root["app_icon"].asString();
    Config::currency = root["currency"].asString();

    char szPath[128] = {0};
    char szFullPath[256] = {0};
    getcwd(szPath, 256);
    sprintf(szFullPath, "%s%s", szPath, "/Trade/BackTrade/Record/");
    Config::recordDir = szFullPath;

    // getRSAKey("Trade/BackTrade/BaseConfig/rsa_public_key_blowfish.pem", Config::sRSAUserCenterPubKey, 0);
    // getRSAKey("Trade/BackTrade/BaseConfig/rsa_private_key_blowfish.pem", Config::sRSAFrontTradePriKey, 0);
    getRSAKey("/BaseConfig/rsa_public_key_blowfish.pem", Config::sRSAUserCenterPubKey, 0);
    getRSAKey("/BaseConfig/rsa_private_key_blowfish.pem", Config::sRSAFrontTradePriKey, 0);
    cout << "Config::sRSAUserCenterPubKey = " << Config::sRSAUserCenterPubKey << endl;
    cout << "Config::sRSAFrontTradePriKey = " << Config::sRSAFrontTradePriKey << endl;
    return true;
}
void Config::getRSAKey(string sFilePath, string &key, int iLen)
{
    char buf[6200] = {0};
    enum{BUFF_SIZE=512};
    char sBuffer[BUFF_SIZE]={0};
    int count = readlink( "/proc/self/exe", sBuffer, BUFF_SIZE );
    if ( count < 0 || count >= BUFF_SIZE )
    {
        cout<<"get program full path failed"<<endl;
        return;
    }
    string rootDir;
    rootDir.assign(sBuffer,count);
    string::size_type pos=rootDir.find_last_of('/');
    if(pos==rootDir.npos)return;
    rootDir.erase(pos+1);

    string szFullPath=rootDir+sFilePath;

    int fd = open(szFullPath.data(), O_RDWR);
    if (-1 == fd)
    {
        cout << "open rsa file failure path = " << szFullPath << endl;
    }
    else
    {
        cout << "open rsa file successful" << endl;
    }
    int ret = read(fd, buf, 6200);
    if (ret < -1)
    {
        cout << "read rsa file failure" << endl;
    }
    else
    {
        unsigned char *recBuf = new unsigned char[ret + iLen + 1];
        memset(recBuf,0,ret + iLen + 1);
        BlowfishDecrypt(recBuf, ret + iLen, (const unsigned char *)buf, ret + iLen);
        key = (const char *)recBuf;
    }
    close(fd);
}
