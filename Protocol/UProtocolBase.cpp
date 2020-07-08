
#include "UProtocolBase.h"

int MARKETFromString(string &s)
{
    return 0;
}

int CurrencyFromString(string &s)
{
    for(int i=1;i<CURRENCY_MAX;i++)
    {
        if(s.compare(CURRENCY_STR[i])==0)return i;
    }
    return 0;
}

int KLineFromString(string &s)
{
    for(int i=1;i<KLINE_MAX;i++)
    {
        if(s.compare(KLINE_STR[i])==0)return i;
    }
    return 0;
}

