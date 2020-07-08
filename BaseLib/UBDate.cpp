
#include "UBDate.h"


void TestDate()
{
    UBDate date(2019,1,1);
    for(int i=0;i<365;i++)
    {
        cout<<date<<endl;
        date++;
    }

    string x;
    date.str(x);
    cout<<x<<endl;
}