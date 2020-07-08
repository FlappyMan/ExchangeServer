#include "UProtocol.h"
// #include <string.h>
// #include <iostream>
// using namespace std;

#define DEFAULT_PORT 8000
#define MAXLINE 4096

void addBeforeNum(string &num, int idx)
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
void mutipBeforeNum(string &num, int iData)
{
	char cData = iData + '0';
	num.insert(0, string(&cData, 1));
}
void subBeforeNum(string &num, int idx)
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
string sumIntValue(string &s1, string &s2, int iLen, int iLevel)
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
string subIntValue(string &s1, string &s2, int iLen, int iLevel)
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
void subString(string &_s1, string &_s2)
{
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

	assert(s1_int_part.length() <= 16);
	assert(s1_frac_part.length() <= 16);

	assert(s2_int_part.length() <= 16);
	assert(s2_frac_part.length() <= 16);

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
string addLongDouble(string _s1, string _s2)
{
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

	if (sRet.data()[0] == '.')
	{
		char cData = '0';
		sRet.insert(0, string(&cData, 1));
	}

	return sRet;
}
string minusLongDouble(string _s1, string _s2)
{
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

		return sRet;
	}
}

string mutipLongDouble(string _s1, string _s2)
{
	// 获取s1小数点后有几位
	// 获取s2小数点后有几位
	// 循环开始-循环次数由是s2不包含小数点长度决定的
	// 从s2的最后一位开始乘以s1
	// 乘出来的结果乘与下标决定【新数字】的小数点所在位置
	// 放入数组对应行数中
	// 循环结束
	// 调用addLongDouble，把多维数组中的所有小数加起来
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
class ESort
{
public:
	bool operator()(string const &A, string const &B) const
	{
		string value2 = minusLongDouble(A, B);
		return value2 == "-1"?true:false;
	}
};

#define SIZE (sizeof(class UPMatchFile))
#define SIZE_TEST (sizeof(class CBuff))

class CBuff
{
public:
	char szBuff[128];
};
int main(int argc, char **argv)
{
	int iCtrol = 2;
	FILE *fp;
	if ((fp = fopen("./testrecord.dat", "a+")) == NULL)
	{
		exit(1);
	}
	// for (int iLoop = 0; iLoop < iCtrol; iLoop++)
	// {
	// 	int64_t nowTime = (int64_t)time(NULL);
	// 	UPMatchFile *pRecord = new UPMatchFile;
	// 	pRecord->type = 1;
	// 	// pRecord->order.user_id = nowTime;
	// 	// pRecord->uid = nowTime;
	// 	// pRecord->invite_uid_1 = nowTime + 1;
	// 	// pRecord->invite_uid_2 = nowTime + 2;
	// 	// pRecord->invite_uid_3 = nowTime + 3;
	// 	// pRecord->type = 1;
	// 	// pRecord->time = nowTime;
	// 	// for (size_t i = 0; i < iLoop+1; i++)
	// 	// {
	// 	// 	pRecord->price[i] = (i+1) + '0';
	// 	// }
	// 	// pRecord->totalNum[0] = '2';
	// 	// pRecord->num[0] = '3';
		
	// 	fwrite(pRecord, SIZE, 1, fp);
	// 	fflush(fp);
	// 	delete pRecord;
	// }
	
	// {
	// 	CBuff *pRecord;
	// 	fread(pRecord, SIZE_TEST, iCtrol, fp);
	// 	cout << "value = " << pRecord->szBuff << endl;
	// 	for (int i = 0; i < iCtrol; i++)
	// 	{
	// 		cout << "CBuff = " << pRecord->szBuff << endl;
	// 		cout << "-------------------------------------------------------" << endl;
	// 		pRecord++;
	// 	}
	// }

	// {
	// 	char *pRecord = new char[128];
	// 	Json::Value JRet;
	// 	JRet[0] = 2520406000;
	// 	JRet[1] = 2847.87525200;
	// 	JRet[2] = 321.59183000;
	// 	JRet[3] = 329.61898800;
	// 	JRet[4] = 303.85212300;
	// 	JRet[5] = 314.92304300;
	// 	Json::FastWriter writer;
	// 	string ret = writer.write(JRet);
	// 	memset(pRecord,0,128);
	// 	strncpy(pRecord,ret.c_str(),ret.length());
	// 	fwrite(pRecord, 128, 1, fp);
	// 	fflush(fp);
	// 	delete pRecord;
	// }

	// {
	// 	char *pRecord = new char[128*iCtrol];
	// 	int iRet = fread(pRecord, SIZE_TEST, 128*iCtrol, fp);
		
	// 	for (int i = 0; i < iCtrol; i++)
	// 	{
	// 		cout << "pRecord = " << pRecord << endl;
	// 		pRecord += 128;
	// 	}
	// }
	fclose(fp);
	return 0;

	// stringstream sTime;
    // srand((unsigned)time(NULL));
    // cout<<"nowTime = "<<(int64_t)nowTime<<endl;
	// // string _s1 = "19283108.451296";
	// // string _s2 = "42353462.128314";
	// // string value1 = mutipLongDouble(_s1, _s2);
	// // cout<<"value1 = "<<value1<<endl;
	// for (int i = 0; i < 1000000; i++)
	// {
	// 	string _s1 = "19283108.451296";
	// 	string _s2 = "42353462.128314";
	// 	string value1 = mutipLongDouble(_s1, _s2);
	// }
	// nowTime = time(NULL);
    // cout<<"nowTime = "<<(int64_t)nowTime<<endl;
	// string _s1 = "1928310892766430.7223455423451296";
	// string _s2 = "4235346234234441.8761345345128314";
	// // string _s1 = "42412344.1";
	// // string _s2 = "14533345.2";
	// string value1 = addLongDouble(_s1, _s2);
	// // string value2 = minusLongDouble(_s1, _s2);
	// // string value3 = mutipLongDouble(_s1, _s2);
	// // double dUsable = atof(value2.c_str());
	// cout << "s1 + s2 = " << value1 << endl;
	// cout << "s1 - s2 = " << value2 << endl;
	// cout << "s1 x s2 = " << value3 << endl;
	// cout << "dUsable = " << dUsable << endl;
	// map<string,int,ESort> mapCmp;
	// mapCmp.insert(pair<string,int>("0.7223455423451296",1));
	// mapCmp.insert(pair<string,int>("10.7223455423451296",1));
	// mapCmp.insert(pair<string,int>("0.8223455423451296",1));
	// mapCmp.insert(pair<string,int>("0.3223455423451296",1));
	// mapCmp.insert(pair<string,int>("7.6223455423451296",1));
	// mapCmp.insert(pair<string,int>("7.62234554234543",1));
	// mapCmp.insert(pair<string,int>("7.6223455423455665",1));
	// mapCmp.insert(pair<string,int>("7.6223455423423426",1));
	// for (map<string,int>::iterator itr = mapCmp.begin(); itr != mapCmp.end(); itr++)
	// {
	// 	cout<<itr->first<<endl;
	// }
	
}
