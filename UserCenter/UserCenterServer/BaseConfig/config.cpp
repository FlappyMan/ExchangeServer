
#include "config.h"

string Config::g_sHttpIP = "0.0.0.0";
uint16_t Config::g_uiHttpPort = 0;

string Config::g_sTcpIP = "0.0.0.0";
uint16_t Config::g_usTcpPort = 0;

string Config::sHost = "192.168.1.20";
uint16_t Config::uiPort = 3306;
string Config::sUser = "wuyoupeng";
string Config::sPwd = "123456";
string Config::sAccountDB = "account";
string Config::sAdminDB = "admin";
string Config::sAssetsDB = "assets";
string Config::sBaseDB = "base";
string Config::sExassetsDB = "exassets";
string Config::sKycDB = "kyc";
string Config::sOtcDB = "otc";
string Config::sOtherDB = "other";
string Config::sSaDB = "sa";
string Config::sTradeDB = "trade";
string Config::sRSAPrivateKey = "";
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
    Config::g_usTcpPort = root["accept_tcp_port"].asUInt64();
    Config::g_sTcpIP = root["accept_tcp_ip"].asString();

    Config::g_uiHttpPort = root["accept_http_port"].asUInt64();
    Config::g_sHttpIP = root["accept_http_ip"].asString();

    Config::sHost = root["host"].asString();
    Config::uiPort = root["port"].asUInt64();
    Config::sUser = root["user"].asString();
    Config::sPwd = root["password"].asString();
    Config::sAccountDB = root["account"].asString();
    Config::sAdminDB = root["admin"].asString();
    Config::sAssetsDB = root["assets"].asString();
    Config::sBaseDB = root["base"].asString();
    Config::sExassetsDB = root["exassets"].asString();
    Config::sKycDB = root["kyc"].asString();
    Config::sOtcDB = root["otc"].asString();
    Config::sOtherDB = root["other"].asString();
    Config::sSaDB = root["sa"].asString();
    Config::sTradeDB = root["trade"].asString();
    getRSAPrivateKey();
    return true;
}

int Config::getRSAPrivateKey()
{
    int fd = -1;
    int ret = -1;
    char buf[6200] = {0};
    char szPath[128] = {0};
    char szFullPath[256] = {0};
    getcwd(szPath, 256);
    // sprintf(szFullPath, "%s%s", szPath, "/BaseConfig/1111/rsa_public_key_blowfish.pem");
    // sprintf(szFullPath, "%s%s", szPath, "/UserCenter/UserCenterServer/BaseConfig/rsa_private_key_blowfish.pem");
    // sprintf(szFullPath, "%s%s", szPath, "/BaseConfig/rsa_private_key_blowfish.pem");

    enum{BUFF_SIZE=512};
    char sBuffer[BUFF_SIZE]={0};
    int count = readlink( "/proc/self/exe", sBuffer, BUFF_SIZE );
    if ( count < 0 || count >= BUFF_SIZE )
    {
        cout<<"get program full path failed"<<endl;
        return false;
    }
    string dir_root;
    dir_root.assign(sBuffer,count);
    string::size_type pos=dir_root.find_last_of('/');
    if(pos==dir_root.npos)return false;
    dir_root.erase(pos+1);

    string sConfigPath=dir_root+"/BaseConfig/rsa_private_key_blowfish.pem";

    cout<<sConfigPath<<endl;
    fd = open(sConfigPath.data(), O_RDWR);
    if (-1 == fd)
    {
        cout << "open rsa file failure" << endl;
    }
    else
    {
        cout << "open rsa file success" << endl;
    }
    ret = read(fd, buf, 6200);
    if (ret < -1)
    {
        cout << "read rsa file failure" << endl;
    }
    else
    {
        string iStr = buf;
        unsigned char *recBuf = new unsigned char[ret];
        BlowfishDecrypt(recBuf, ret, (const unsigned char *)buf, ret);
        Config::sRSAPrivateKey = (const char *)recBuf;
        cout << "Config::sRSAPrivateKey = " << Config::sRSAPrivateKey << endl;
        /********************************************************************************************/
        // string iStr = buf;

        // enum{BUFF_LEN=2048};
        // char sEncode[BUFF_LEN]={0};
        // int iOut=BlowfishEncrypt((uint8_t*)sEncode,BUFF_LEN,(uint8_t*)iStr.data(),iStr.length());
        // cout<<"encode len:"<<iOut<<endl;

        // volatile int len = 0;
        // char szFull[256] = {0};
        // sprintf(szFull, "%s%s", szPath, "/BaseConfig/rsa_private_key_blowfish.pem");
        // int fp = open(szFull, O_RDWR | O_CREAT);
        // int wrflag = write(fp, (const char *)sEncode, iOut);
    }
    close(fd);
}
