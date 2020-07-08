#include "DispatchManager.h"
#include "HttpResp.h"
#include <sys/time.h>
#include "../ThreadDBOper.h"
mapBaseSession CDispatchManager::g_mapMapping;
mapHttpSession CDispatchManager::g_mapHttpMapping;
mapBaseTcpSession CDispatchManager::g_mapTcpBusinessSession;

UBMysql CDispatchManager::g_DBAccount;
UBMysql CDispatchManager::g_DBAdmin;
UBMysql CDispatchManager::g_DBAssets;

UBMysql CDispatchManager::g_DBBase;
UBMysql CDispatchManager::g_DBExassets;
UBMysql CDispatchManager::g_DBKyc;
UBMysql CDispatchManager::g_DBOTC;
UBMysql CDispatchManager::g_DBOther;
UBMysql CDispatchManager::g_DBSA;
UBMysql CDispatchManager::g_DBTrade;

uint64_t CDispatchManager::g_ulTCPPkgIndex = 1;
uint64_t CDispatchManager::g_ulTCPSockIndex = 1;
uint64_t CDispatchManager::g_ulHTTPPkgIndex = 1;

CDispatchManager::CDispatchManager()
{
}
void CDispatchManager::DBInit()
{
    CDispatchManager::g_DBAccount.init(4096);
    CDispatchManager::g_DBAdmin.init(4096);
    CDispatchManager::g_DBAssets.init(4096);
    CDispatchManager::g_DBBase.init(4096);
    CDispatchManager::g_DBExassets.init(4096);
    CDispatchManager::g_DBKyc.init(4096);
    CDispatchManager::g_DBOTC.init(4096);
    CDispatchManager::g_DBOther.init(4096);
    CDispatchManager::g_DBSA.init(9000);
    CDispatchManager::g_DBTrade.init(4096);

    connectToDB(CDispatchManager::g_DBAccount, Config::sAccountDB);
    connectToDB(CDispatchManager::g_DBAdmin, Config::sAdminDB);
    connectToDB(CDispatchManager::g_DBAssets, Config::sAssetsDB);
    connectToDB(CDispatchManager::g_DBBase, Config::sBaseDB);
    connectToDB(CDispatchManager::g_DBExassets, Config::sExassetsDB);
    connectToDB(CDispatchManager::g_DBKyc, Config::sKycDB);
    connectToDB(CDispatchManager::g_DBOTC, Config::sOtcDB);
    connectToDB(CDispatchManager::g_DBOther, Config::sOtherDB);
    connectToDB(CDispatchManager::g_DBSA, Config::sSaDB);
    connectToDB(CDispatchManager::g_DBTrade, Config::sTradeDB);

    string sConfigPath=g_dir_root+"BaseConfig/RuntimeConfig.json";
    if(CRuntimeConfig::Load(sConfigPath,CThreadDBOper::g_sConfigFileUpdateTime)==false)
    {
        DEBUG_ERROR("Load config file failed");
    }
    else
    {
        DEBUG_INFO("服务器配置信息加载完毕");
    }
    
}
string CDispatchManager::IntHexToString(int value, int len)
{
    stringstream ioss;    //定义字符串流
    string s_temp;        //存放转化后字符
    ioss << hex << value; //以十六制形式输出
    ioss >> s_temp;
    string s(len - s_temp.size(), '0'); //补0
    s += s_temp;                        //合并
    return s;
}
int CDispatchManager::HexStringToInt(string value)
{
    return (int)strtoll(value.c_str(), NULL, 16);
}
string CDispatchManager::md5(string strPlain)
{
    unsigned char *dst = new unsigned char[32];
    MD5(dst, (const unsigned char *)strPlain.data(), strlen((const char *)strPlain.data()));
    string md5((const char *)dst);
    delete dst;
    return md5;
}
int CDispatchManager::getNowTime()
{
    time_t nowTime = time(NULL);
    return (int)nowTime;
}
string CDispatchManager::getRandomToken()
{
    string retToken;
    stringstream ss;
    struct timeval tv;
    gettimeofday(&tv, NULL); //该函数在sys/time.h头文件中
    ss << (uint64_t)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
    ss >> retToken;
    return retToken;
}
void CDispatchManager::connectToDB(UBMysql &dbConn, string &dbName)
{
    bool bConnected = dbConn.Connect(Config::sHost, Config::uiPort, Config::sUser, Config::sPwd, dbName);
    if (bConnected == false)
    {
        string sError;
        dbConn.Error(sError);
    }
}
int CDispatchManager::DispatchClient(client_sokt_t *client, char *pBuffer, int iDataLen)
{
    mapBaseSession::iterator itr = CDispatchManager::g_mapMapping.find(client);
    if (itr != CDispatchManager::g_mapMapping.end())
    {
        return itr->second->OnRecv(client, pBuffer, iDataLen);
        
    }
    return RET_FAILED;
}

int CDispatchManager::DispatchHttpClient(client_sokt_t *client, char *pBuffer, int iDataLen)
{
    mapHttpSession::iterator itr = CDispatchManager::g_mapHttpMapping.find(client);
    if (itr != CDispatchManager::g_mapHttpMapping.end())
    {
        return itr->second->OnRecv(client, pBuffer, iDataLen);
    }
    return RET_FAILED;
}
string CDispatchManager::getToken(string uid)
{
    stringstream sTime, sDeadLine; // 当前时间，token截止时间
    srand((unsigned)time(NULL));   // 随机种子

    time_t nowTime = time(NULL);
    int64_t deadLine = (int64_t)nowTime + 604800;
    sTime << nowTime;
    sDeadLine << deadLine;
    return sTime.str() + "-" + uid + "-" + sDeadLine.str(); // 原始token
}
int CDispatchManager::DispatchHttpResponse(string sResp, uv_tcp_t *client, bool success)
{
#ifdef PRINT_LOG
    DEBUG_INFO("[CDispatchManager::DispatchHttpResponse] = " + sResp);
#endif
    string content = "Content-Length: ";
    content += to_string(sResp.length());
    content += "\r\n";

    char s_time[60] = {0};
    GetGMTime(s_time);

    int iTotalLen = success == true ? 20 : 27;
    iTotalLen += 126;
    iTotalLen += content.length();
    iTotalLen += strlen(s_time);
    iTotalLen += sResp.length();

    UVWriteReq *write = g_cache_http_req.Get(iTotalLen);
    if (write == NULL || write->buf.base == NULL)
    {
        return RET_FAILED;
    }

    write->buf.len = iTotalLen;
    int iCopyLen = 0;
    int iDataLen = 0;
    if (success == true)
    {
        iDataLen = 20;
        memcpy(write->buf.base, "HTTP/1.1 200 OK \r\n", iDataLen);
    }
    else
    {
        iDataLen = 27;
        memcpy(write->buf.base, "HTTP/1.1 404 Not Found \r\n", iDataLen);
    }
    iCopyLen = (iDataLen - 2);
    iDataLen = 42;
    memcpy(write->buf.base + iCopyLen, "Content-Type: text/html; charset=UTF-8\r\n", iDataLen);
    iCopyLen += (iDataLen - 2);

    iDataLen = content.length();
    memcpy(write->buf.base + iCopyLen, content.data(), iDataLen);
    iCopyLen += (iDataLen - 1);

    iDataLen = 27;
    memcpy(write->buf.base + iCopyLen, "Content-Language: zh-CN\r\n", iDataLen);
    iCopyLen += (iDataLen - 2);

    iDataLen = 21;
    memcpy(write->buf.base + iCopyLen, "Connection: Close\r\n", iDataLen);
    iCopyLen += (iDataLen - 2);

    iDataLen = 6;
    memcpy(write->buf.base + iCopyLen, "Date: ", iDataLen);
    iCopyLen += (iDataLen - 1);

    iDataLen = strlen(s_time);
    memcpy(write->buf.base + iCopyLen, s_time, iDataLen);
    iCopyLen += (iDataLen);

    iDataLen = 4;
    memcpy(write->buf.base + iCopyLen, "\r\n", iDataLen);
    iCopyLen += (iDataLen - 2);

    iDataLen = 22;
    memcpy(write->buf.base + iCopyLen, "Server: UkexServer\r\n", iDataLen);
    iCopyLen += (iDataLen - 2);

    iDataLen = 4;
    memcpy(write->buf.base + iCopyLen, "\r\n", iDataLen);
    iCopyLen += (iDataLen - 2);

    iDataLen = sResp.length();
    memcpy(write->buf.base + iCopyLen, sResp.c_str(), iDataLen);
    iCopyLen += iDataLen;

    uv_write((uv_write_t *)write, (uv_stream_t *)client, &write->buf, 1, CThreadHttpOper::cbWrited);
}

int CDispatchManager::GetGMTime(char *szGmtTime)
{
    if (!szGmtTime)
    {
        return -1;
    }
    struct tm timeInfo;
    time_t cur_time = time(NULL);
    char szTemp[30] = {0};
    gmtime_r(&cur_time, &timeInfo);
    strftime(szTemp, sizeof(szTemp), "%a, %d %b %Y %H:%M:%S GMT", &timeInfo);
    strcpy(szGmtTime, szTemp);
    return 0;
}
string CDispatchManager::getUserVipDiscount(string level)
{
    return std::to_string(CRuntimeConfig::viplevel[stoi(level)]);
}
string CDispatchManager::getYYMMDD()
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y%m%d", localtime(&timep));
    return tmp;
}
string CDispatchManager::getHour()
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%H", localtime(&timep));
    string retStr = tmp;
    return retStr;
}
string CDispatchManager::getMin()
{
    time_t timep;
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%M", localtime(&timep));
    string retStr = tmp;
    return retStr;
}
int CDispatchManager::DispatchOnSend(UProtocolBase *prot, uv_tcp_t *client)
{
    switch (prot->m_uiType)
    {
    case UPassetsupdate::CMD:
    {
#ifdef PRINT_LOG
        DEBUG_INFO("[DispatchOnSend]:[UPassetsupdate]");
#endif
    }
    default:
#ifdef PRINT_LOG
        DEBUG_INFO("[DispatchOnSend]:[Unknow Protocol Type]");
#endif
        break;
    }
    UPResponse *up = reinterpret_cast<UPResponse *>(prot);
    const int BUFFSIZE = ProtoPackSize<UPResponse>(*up);
    UVWriteReq *write = g_cache_write_req.Get(BUFFSIZE);
    if (write == NULL || write->buf.base == NULL)
    {
        return RET_FAILED;
    }
    write->buf.len = ProtoPack<UPResponse>(write->buf.base, BUFFSIZE, *up);

    if (write->buf.len <= 0)
    {
        return RET_FAILED;
    }

    write->pkg = up;
    uv_write((uv_write_t *)write, (uv_stream_t *)client, &write->buf, 1, CThreadTCPOper::cbWrited);
}

void CDispatchManager::addBeforeNum(string &num, int idx)
{
    if (idx == 0)
        return;

    int value = (num.data()[idx - 1] - '0') + 1;
    if (value == 10)
    {
        char cTemp = '0';
        num.replace(idx - 1, 1, string(&cTemp, 1));
        addBeforeNum(num, idx - 1);
    }
    else
    {
        char cTemp = value + '0';
        num.replace(idx - 1, 1, string(&cTemp, 1));
    }
}
void CDispatchManager::subBeforeNum(string &num, int idx)
{
    int value = (num.data()[idx] - '0') - 1;
    if (idx == 0)
    {
        char cTemp = value + '0';
        num.replace(idx, 1, string(&cTemp, 1));
    }
    else
    {
        if (value < 0)
        {
            char cTemp = '9';
            num.replace(idx, 1, string(&cTemp, 1));
            subBeforeNum(num, idx - 1);
        }
        else
        {
            char cTemp = value + '0';
            num.replace(idx, 1, string(&cTemp, 1));
        }
    }
}
string CDispatchManager::sumIntValue(string &s1, string &s2, int iLen, int iLevel)
{
    string iRet;
    int iValue1 = s1.data()[iLen] - '0';
    int iValue2 = s2.data()[iLen] - '0';

    int sum = iValue1 + iValue2;
    if (sum >= 10)
    {
        char cData = (sum - 10) + '0';
        iRet.insert(0, string(&cData, 1));
        if (iLen > 0)
        {
            string iEx = sumIntValue(s1, s2, iLen - 1, iLevel + 1);
            int value = (iEx.data()[iLen - 1] - '0') + 1;
            if (value == 10)
            {
                value = 0;
                addBeforeNum(iEx, iLen - 1);
            }

            char cTemp = value + '0';
            iEx.replace(iLen - 1, 1, string(&cTemp, 1));
            iRet.insert(0, iEx.data());
        }
    }
    else
    {

        char cData = sum + '0';
        iRet.insert(0, string(&cData, 1));
        if (iLen > 0)
        {
            string iEx = sumIntValue(s1, s2, iLen - 1, iLevel + 1);
            iRet.insert(0, iEx.data());
        }
    }

    return iRet;
}
string CDispatchManager::subIntValue(string &s1, string &s2, int iLen, int iLevel)
{
    string iRet;
    int iValue1 = s1.data()[iLen] - '0';
    int iValue2 = s2.data()[iLen] - '0';

    int result = iValue1 - iValue2;
    if (result >= 0)
    {
        char cData = result + '0';
        iRet.insert(0, string(&cData, 1));
        if (iLen > 0)
        {
            string iEx = subIntValue(s1, s2, iLen - 1, iLevel + 1);
            iRet.insert(0, iEx.data());
        }
    }
    else
    {
        if (iLen > 0)
        {
            char cData = (result + 10) + '0';
            iRet.insert(0, string(&cData, 1));
            subBeforeNum(s1, iLen - 1);
            string iEx = subIntValue(s1, s2, iLen - 1, iLevel + 1);
            int value = (iEx.data()[iLen - 1] - '0');
            char cTemp = value + '0';
            iEx.replace(iLen - 1, 1, string(&cTemp, 1));
            iRet.insert(0, iEx.data());
        }
        else if (iLen == 0)
        {
            iRet.insert(0, "-");
        }
    }
    return iRet;
}
void CDispatchManager::subString(string &_s1, string &_s2)
{
    int iS1Len = _s1.length();
    int iS2Len = _s2.length();
    bool hasPoint = false;
    for (int i = 0; i < iS1Len; i++)
    {
        if (_s1.data()[i] == '.')
        {
            hasPoint = true;
        }
    }
    if (hasPoint == false)
    {
        _s1 += ".00";
    }
    else
    {
        hasPoint = false;
    }

    for (int i = 0; i < iS2Len; i++)
    {
        if (_s2.data()[i] == '.')
        {
            hasPoint = true;
        }
    }
    if (hasPoint == false)
    {
        _s2 += ".00";
    }
    else
    {
        hasPoint = false;
    }

    string s1_int_part, s1_frac_part;
    string s2_int_part, s2_frac_part;

    int s1_len = _s1.length();
    int s2_len = _s2.length();

    for (int i = 0; i < s1_len; i++)
    {
        if (_s1.data()[i] == '.')
        {
            s1_int_part = _s1.substr(0, i);
            s1_frac_part = _s1.substr(i + 1, _s1.length());
        }
    }

    for (int i = 0; i < s2_len; i++)
    {
        if (_s2.data()[i] == '.')
        {
            s2_int_part = _s2.substr(0, i);
            s2_frac_part = _s2.substr(i + 1, _s2.length());
        }
    }

    if (s1_int_part.length() > 16)
    {
        s1_int_part = s1_int_part.substr(0, 16);
    }
    if (s1_frac_part.length() > 16)
    {
        s1_frac_part = s1_frac_part.substr(0, 16);
    }
    if (s2_int_part.length() > 16)
    {
        s2_int_part = s2_int_part.substr(0, 16);
    }
    if (s2_frac_part.length() > 16)
    {
        s2_frac_part = s2_frac_part.substr(0, 16);
    }

    int s1_int_part_Len = 16 - s1_int_part.length();
    for (size_t i = 0; i < s1_int_part_Len; i++)
    {
        s1_int_part.insert(0, "0");
    }

    int s2_int_part_Len = 16 - s2_int_part.length();
    for (size_t i = 0; i < s2_int_part_Len; i++)
    {
        s2_int_part.insert(0, "0");
    }

    int s1_frac_part_Len = 16 - s1_frac_part.length();
    for (int i = 0; i < s1_frac_part_Len; i++)
    {
        s1_frac_part += "0";
    }

    int s2_frac_part_Len = 16 - s2_frac_part.length();
    for (int i = 0; i < s2_frac_part_Len; i++)
    {
        s2_frac_part += "0";
    }

    _s1 = s1_int_part + s1_frac_part;
    _s2 = s2_int_part + s2_frac_part;
}
bool CDispatchManager::IsBigThanZero(string value)
{
    if (value.data()[0] == '-')
    {
        return false;
    }
    else
    {
        return true;
    }
}
string CDispatchManager::addLongDouble(string _s1, string _s2)
{
    if(_s1.length() == 0)
        _s1 = "0.0";
    if (_s2.length() == 0)
        _s2 = "0.0";
    
    subString(_s1, _s2);
    string result = sumIntValue(_s1, _s2, _s1.length() - 1, 0);
    string sRet = result.insert(result.length() - 16, ".");

    while (sRet.data()[0] == '0')
    {
        sRet.erase(0, 1);
    }

    while (sRet.data()[sRet.length() - 1] == '0')
    {
        sRet.pop_back();
    }

    if (sRet.data()[sRet.length() - 1] == '.')
    {
        sRet += "0";
    }
    if (sRet.data()[0] == '.')
    {
        char cData = '0';
        sRet.insert(0, string(&cData, 1));
    }
    return sRet;
}

string CDispatchManager::diviLongDouble(string _s1,string _s2)
{
    double value1,value2;
    value1 = stold(_s1);
    value2 = stold(_s2);
    std::ostringstream stream;
    stream << value1/value2;
    return stream.str();
}
string CDispatchManager::minusLongDouble(string _s1, string _s2)
{
    if(_s1.length() == 0)
        _s1 = "0.0";
    if (_s2.length() == 0)
        _s2 = "0.0";
    subString(_s1, _s2);
    string result = subIntValue(_s1, _s2, _s1.length() - 1, 0);
    if (result.data()[0] == '-')
    {
        return "-1";
    }
    else
    {
        string sRet = result.insert(result.length() - 16, ".");

        while (sRet.data()[0] == '0')
        {
            sRet.erase(0, 1);
        }

        while (sRet.data()[sRet.length() - 1] == '0')
        {
            sRet.pop_back();
        }
        if (sRet.data()[sRet.length() - 1] == '.')
        {
            sRet += "0";
        }
        if (sRet.data()[0] == '.')
        {
            char cData = '0';
            sRet.insert(0, string(&cData, 1));
        }
        return sRet;
    }
}

void CDispatchManager::mutipBeforeNum(string &num, int iData)
{
	char cData = iData + '0';
	num.insert(0, string(&cData, 1));
}
/*
字符串乘法
*/
string CDispatchManager::mutipLongDouble(string _s1, string _s2)
{
	// 获取s1小数点后有几位
	// 获取s2小数点后有几位
	// 循环开始-循环次数由是s2不包含小数点长度决定的
	// 从s2的最后一位开始乘以s1
	// 乘出来的结果乘与下标决定【新数字】的小数点所在位置
	// 放入数组对应行数中
	// 循环结束
	// 调用addLongDouble，把多维数组中的所有小数加起来
    if(_s1.length() == 0)
        _s1 = "0.0";
    if (_s2.length() == 0)
        _s2 = "0.0";
	int s1_len = _s1.length();
	string s1_int_part, s1_frac_part; // s1的整数部分和小数部分
	int s2_len = _s2.length();
	string s2_int_part, s2_frac_part; // s1的整数部分和小数部分
	for (int i = 0; i < s1_len; i++)
	{
		if (_s1.data()[i] == '.')
		{
			s1_int_part = _s1.substr(0, i);
			s1_frac_part = _s1.substr(i + 1, _s1.length());
		}
	}
	for (int i = 0; i < s2_len; i++)
	{
		if (_s2.data()[i] == '.')
		{
			s2_int_part = _s2.substr(0, i);
			s2_frac_part = _s2.substr(i + 1, _s2.length());
		}
	}
	_s1.erase(std::remove(_s1.begin(), _s1.end(), '.'), _s1.end());
	int iLoopCount = _s2.length();
	vector<string> vecLevelsResult;
	for (size_t i = iLoopCount; i > 0; i--)
	{
		if (_s2.data()[i - 1] == '.')
			continue;

		string result; // 最后结果字符串
		int iValue2 = _s2.data()[i - 1] - '0';
		int s1Len = _s1.length();

		for (size_t idx = s1Len; idx > 0; idx--)
		{
			int iLoopTimes = s1Len - idx;			 // 当前循环次数
			int iValue1 = _s1.data()[idx - 1] - '0'; // s1串中当前的值
			int value = iValue2 * iValue1;			 // 乘以后的结果值
			if (value >= 10)
			{
				// 大于10，进位处理
				int iData = value / 10; //获取整数位数字
				if (iLoopTimes != result.length())
				{
					// 之前的串有进位数字，这里必须加上当前数字
					int iBig = value % 10;				   //计算当前个位数字
					int iZeroIdx = result.data()[0] - '0'; //之前进位数字
					int iInsert = iZeroIdx + iBig;
					if (iInsert >= 10)
					{
						// 进位数字和当前数字之和大于10，在往前进1
						char cData = (iInsert % 10) + '0';		 //进位以后的个位数字
						result.replace(0, 1, string(&cData, 1)); //替换进位数字
						mutipBeforeNum(result, iData + 1);		 //在进1位数字1
					}
					else
					{
						char cData = iInsert + '0';
						result.replace(0, 1, string(&cData, 1));
						mutipBeforeNum(result, iData);
					}
				}
				else
				{
					// 之前没有进位的数字,插入当前数字
					char cData = (value % 10) + '0';
					result.insert(0, string(&cData, 1));
					mutipBeforeNum(result, iData);
				}
			}
			else
			{
				if (iLoopTimes != result.length())
				{
					int iZeroIdx = result.data()[0] - '0';
					int iInsert = iZeroIdx + value;
					if (iInsert >= 10)
					{
						// 进位数字和当前数字之和大于10，在往前进1
						char cData = (iInsert % 10) + '0';		 //进位以后的个位数字
						result.replace(0, 1, string(&cData, 1)); //替换进位数字
						mutipBeforeNum(result, 1);				 //在进1位数字1
					}
					else
					{
						char cData = iInsert + '0';
						result.replace(0, 1, string(&cData, 1));
					}
				}
				else
				{
					char cData = value + '0';
					result.insert(0, string(&cData, 1));
				}
			}
			// cout << "result = " << result << endl;
			// cout << "--------------------" << endl;
		}
		// cout << "push back = " << result << endl;
		vecLevelsResult.push_back(result);
	}

	int iTotalPointPos = s1_frac_part.length() + s2_frac_part.length();
	int iLevelSize = vecLevelsResult.size();
	for (int i = 0; i < iLevelSize; i++)
	{
		int index = vecLevelsResult[i].length() - iTotalPointPos + i;
		// cout << "index = " << index << endl;
		for (int iVecIdx = index; iVecIdx < 0; iVecIdx++)
		{
			char cData = '0';
			vecLevelsResult[i] = vecLevelsResult[i].insert(0, string(&cData, 1));
		}
		char cData = '.';
		int iThisLevelLen = vecLevelsResult[i].length();
		int iResultIdx = iThisLevelLen - iTotalPointPos + i;
		for (int itemp = iThisLevelLen; itemp < iResultIdx; itemp++)
		{
			char cData = '0';
			vecLevelsResult[i] = vecLevelsResult[i].insert(vecLevelsResult[i].length(), string(&cData, 1));
		}

		vecLevelsResult[i] = vecLevelsResult[i].insert(iResultIdx, string(&cData, 1));
		if (vecLevelsResult[i].data()[0] == '.')
		{
			cData = '0';
			vecLevelsResult[i] = vecLevelsResult[i].insert(0, string(&cData, 1));
		}
	}

	string sRet = "0.0";
	// cout << "**********************************" << endl;
	for (size_t i = 0; i < vecLevelsResult.size(); i++)
	{
		// cout << "* " << vecLevelsResult[i] << endl;
		sRet = addLongDouble(sRet, vecLevelsResult[i]);
	}
	// cout << "**********************************" << endl;

	return sRet;
}