#ifndef __INCLUDE_DATE_TOOLS__
#define __INCLUDE_DATE_TOOLS__
#pragma once

//���ڼ���
static string CaculateWeekDay(int year, int month, int day)
{
    string szWeek[7] = { "��","һ","��", "��", "��", "��", "��" };

    //��ķ����ɭ���㹫ʽ
    if (month == 1 || month == 2)
    {
        year--;
        month += 12;
    }
    int iIndex = (day + 2 * month + 3 * (month + 1) / 5 + year + year / 4 - year / 100 + year / 400 + 1) % 7;
    return szWeek[iIndex];
}

//���ڸ�ʽ��
static string DateFormate(long lDate,char split)
{
    char szDate[40] = { 0 };
    long year  = 0;
    long month = 0;
    long day   = 0;

    year = lDate / 10000;
    month = (lDate - year * 10000) / 100;
    day = lDate - year * 10000 - month * 100;

    string szWeek = CaculateWeekDay(year, month, day);
    sprintf(szDate, "%d%c%02d%c%02d %s", year, split, month, split, day, szWeek.c_str());
    
    return szDate;
}


#endif

