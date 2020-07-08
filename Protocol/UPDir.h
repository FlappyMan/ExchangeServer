
#ifndef _UPDIR_HEADER_
#define _UPDIR_HEADER_

#include "UBaseLib.h"

/*
常用文件路径计算类


*/

const char UPDIR_KLINE_DIR_ROOT[]="Kline/";


class UPDir
{
public:
	UPDir();
	~UPDir();

	static bool KLineFileName(string &sPath,int iTimeGap,time_t tNow=0);
	static bool KLineFilePath(string &sPath,string &sRoot,int iTimeGap,time_t tNow=0);



};



#endif
