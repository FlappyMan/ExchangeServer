
#ifndef _UBDATE_HEADER_
#define _UBDATE_HEADER_

#include "UBHeader.h"

class UBDate
{
    //重载输出运算符
    friend ostream &operator<<(ostream &_cout, const UBDate d)
    {
        _cout << d._year;
        if(d._month<10)_cout<<"0";
        _cout<< d._month;
        if(d._day<10)_cout<<"0";
        _cout<< d._day;
        return _cout;
    }

public:
    //输入日期的合法性判断
    UBDate(int year = 1900, int month = 1, int day = 1) : _year(year), _month(month), _day(day)
    {
        if (!((year > 0) && (month > 0 && month < 13) && (day > 0 && day <= GetMonthDays(_month))))
        {
            //不合法日期自动变成1990-1-1
            _year = 1900;
            _month = 1;
            _day = 1;
        }
    };

    //拷贝构造
    UBDate(const UBDate &d) : _year(d._year), _month(d._month), _day(d._day)
    {
    };

    void Set(int year, int month, int day)
    {
        _year=year;
        _month=month;
        _day=day;
        if (!((year > 0) && (month > 0 && month < 13) && (day > 0 && day <= GetMonthDays(_month))))
        {
            _year = 1900;
            _month = 1;
            _day = 1;
        }
    };

    //判断是否为闰年
    bool IsLeapYear()
    {
        if (((_year % 4 == 0) && (_year % 100 != 0)) || (_year % 400 == 0))
        {
            return true;
        }
        return false;
    };

    void str(string &str)
    {
        str=to_string(_year);
        if(_month<10)str.append(1,'0');
        str.append(to_string(_month));
        if(_day<10)str.append(1,'0');
        str.append(to_string(_day));
    };

    string str()
    {
        string str=to_string(_year);
        if(_month<10)str.append(1,'0');
        str.append(to_string(_month));
        if(_day<10)str.append(1,'0');
        str.append(to_string(_day));
        return str;
    };

    int Year(){return _year;};
    int Month(){return _month;};
    int Day(){return _day;};

    //赋值运算符重载
    UBDate &operator=(const UBDate &d)
    {
        if (this != &d)
        {
            _year = d._year;
            _month = d._month;
            _day = d._day;
        }
        return *this;
    }
    
    //一个日期加上一个天数
    UBDate operator+(int day)
    {
        if (day < 0) //天数若为负数变为减
        {
            return *this - (0 - day);
        }
        UBDate temp(*this);
        temp._day += day;
        int daysInMonth = temp.GetMonthDays(temp._month);
        while (temp._day > daysInMonth)
        {
            temp._day -= daysInMonth;
            temp._month += 1;
            if (temp._month > 12)
            {
                temp._year += 1;
                temp._month = 1;
            }
        }
        return temp;
    }

    //一个日期减去一个天数
    UBDate operator-(int day)
    {
        if (day < 0) //天数若为负数变为加
        {
            return *this + (0 - day);
        }
        UBDate temp(*this);
        temp._day -= day;
        while (temp._day <= 0)
        {
            temp._month--;
            if (temp._month <= 0)
            {
                temp._year--;
                temp._month = 12;
            }
            temp._day += temp.GetMonthDays(temp._month);
        }
        return temp;
    }
    //前置++
    UBDate& operator++()
    {
        *this = *this + 1; //_day += 1;
        return *this;
    }
    //后置++
    const UBDate operator++(int)
    {
        UBDate temp(*this);
        ++*this;
        return temp;
    }
    //前置--
    UBDate& operator--()
    {
        *this = *this - 1;
        return *this;
    }
    //后置--
    const UBDate operator--(int)
    {
        UBDate temp(*this);
        --*this;
        return temp;
    }
    //两个日期之间间隔的天数
    int operator-(const UBDate &d)
    {
        UBDate maxdate(*this);
        UBDate mindate(d);
        if (maxdate < mindate)
        {
            maxdate = mindate;
            mindate = *this;
        }
        int count = 0;
        while (mindate < maxdate)
        {
            mindate = mindate + 1;
            ++count;
        }
        return count;
    }
    //判断两个日期相等
    bool operator==(const UBDate &d)
    {
        if ((_year == d._year) && (_month == d._month) && (_day == d._day))
        {
            return true;
        }
        return false;
    }
    //判断两个日期不相等
    bool operator!=(const UBDate &d)
    {
        return !(*this == d);
    }
    //比较大小
    bool operator>(const UBDate &d)
    {
        if ((_year > d._year) || ((_year == d._year) && (_month > d._month)) || ((_year == d._year) && (_month == d._month) && (_day > d._day)))
        {
            return true;
        }
        return false;
    }

    //比较大小
    bool operator<(const UBDate &d)
    {
        return !(*this > d || *this == d);
    }

private:
    //获取每个月的天数
    int GetMonthDays(int month)
    {
        int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if (IsLeapYear() && (2 == month)) //在这里改变2月的天数
        {
            days[month] += 1;
        }
        return days[month];
    }
    int _year;
    int _month;
    int _day;
};

#endif
