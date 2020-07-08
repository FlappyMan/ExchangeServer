

#include "UPDir.h"


// 根据时间获取对应时间间隔的KLine文件名
// tNow==0 : 表示缺省使用当天
bool UPDir::KLineFileName(string &sPath,int iTimeGap,time_t tNow)
{
	if(tNow==0)tNow=time(NULL);

	struct tm _tm;
	localtime_r(&tNow,&_tm);

	enum{TIME_LEN=32};
	char sTemp[TIME_LEN];
	int iLen=0;
	if(iTimeGap==TIME_GAP_1_MINUTE)
	{
		iLen=snprintf(sTemp,TIME_LEN,"%04d%02d%02d",_tm.tm_year+1900,_tm.tm_mon+1,_tm.tm_mday);
	}
	else if(iTimeGap==TIME_GAP_5_MINUTE || iTimeGap==TIME_GAP_15_MINUTE || iTimeGap==TIME_GAP_30_MINUTE ||
		iTimeGap==TIME_GAP_1_HOUR || iTimeGap==TIME_GAP_6_HOUR || iTimeGap==TIME_GAP_24_HOUR)
	{
		iLen=snprintf(sTemp,TIME_LEN,"%04d",_tm.tm_year+1900);
	}
	if(iLen<=0)return false;
	sPath.assign(sTemp,iLen);
	return true;
}

bool UPDir::KLineFilePath(string &sPath,string &sRoot,int iTimeGap,time_t tNow)
{
	string sFilName;
	if(KLineFileName(sFilName,iTimeGap,tNow)==false)return false;

	sPath=sRoot;
    if(iTimeGap==TIME_GAP_1_MINUTE)sPath.append("1m/");
    else if(iTimeGap==TIME_GAP_5_MINUTE)sPath.append("5m/");
    else if(iTimeGap==TIME_GAP_15_MINUTE)sPath.append("15m/");
    else if(iTimeGap==TIME_GAP_30_MINUTE)sPath.append("30m/");
    else if(iTimeGap==TIME_GAP_1_HOUR)sPath.append("1h/");
    else if(iTimeGap==TIME_GAP_6_HOUR)sPath.append("6h/");
    else if(iTimeGap==TIME_GAP_24_HOUR)sPath.append("24h/");
    else return false;

    sPath.append(sFilName);
	return true;
}

