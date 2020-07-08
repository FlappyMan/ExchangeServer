#include "UBTime.h"

int GetGMTime(char* pGMTTime,int iGMTLen)
{   
    if (!pGMTTime)
    {
        return -1;
    }
    struct tm timeInfo;
    time_t cur_time=time(NULL);	
    gmtime_r(&cur_time, &timeInfo);  
    strftime(pGMTTime, iGMTLen, "%a, %d %b %Y %H:%M:%S GMT", &timeInfo);
    return 0;
}