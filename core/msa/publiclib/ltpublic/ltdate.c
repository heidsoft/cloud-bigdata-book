#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ltdate.h"
#include "fmt.h"




#define ltTimBaseYear  1990
#define ltTimBaseTime  631036800



static char *montab[12] = {
"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"
};


void datetime_tai(dt,t)
struct datetime *dt;
datetime_sec t;
{
  int day;
  int tod;
  int year;
  int yday;
  int wday;
  int mon;
 
  tod = t % 86400;
  day = t / 86400;
  if (tod < 0) { tod += 86400; --day; }
 
  dt->hour = tod / 3600;
  tod %= 3600;
  dt->min = tod / 60;
  dt->sec = tod % 60;
 
  wday = (day + 4) % 7; if (wday < 0) wday += 7;
  dt->wday = wday;
 
  day -= 11017;
  /* day 0 is march 1, 2000 */
  year = 5 + day / 146097;
  day = day % 146097; if (day < 0) { day += 146097; --year; }
  /* from now on, day is nonnegative */
  year *= 4;
  if (day == 146096) { year += 3; day = 36524; }
  else { year += day / 36524; day %= 36524; }
  year *= 25;
  year += day / 1461;
  day %= 1461;
  year *= 4;
  yday = (day < 306);
  if (day == 1460) { year += 3; day = 365; }
  else { year += day / 365; day %= 365; }
  yday += day;
 
  day *= 10;
  mon = (day + 5) / 306;
  day = day + 5 - 306 * mon;
  day /= 10;
  if (mon >= 10) { yday -= 306; ++year; mon -= 10; }
  else { yday += 59; mon += 2; }
 
  dt->yday = yday;
  dt->year = year - 1900;
  dt->mon = mon;
  dt->mday = day + 1;
}


/* roughly 100x faster than mktime() */
datetime_sec datetime_untai(dt)
struct datetime *dt;
{
  int year;
  int day;
  int mon;
 
  year = dt->year + 1900;
 
  mon = dt->mon;
  if (mon >= 2) { mon -= 2; }
  else { mon += 10; --year; }
 
  day = (dt->mday - 1) * 10 + 5 + 306 * mon;
  day /= 10;
 
  if (day == 365) { year -= 3; day = 1460; }
  else { day += 365 * (year % 4); }
  year /= 4;
 
  day += 1461 * (year % 25);
  year /= 25;
 
  if (day == 36524) { year -= 3; day = 146096; }
  else { day += 36524 * (year % 4); }
  year /= 4;
 
  day += 146097 * (year - 5);
  day += 11017;
 
  return ((day * 24 + dt->hour) * 60 + dt->min) * 60 + dt->sec;
}




unsigned int date822fmt(s,dt)
char *s;
struct datetime *dt;
{
  unsigned int i;
  unsigned int len;
  len = 0;
  i = fmt_uint(s,dt->mday); len += i; if (s) s += i;
  i = fmt_str(s," "); len += i; if (s) s += i;
  i = fmt_str(s,montab[dt->mon]); len += i; if (s) s += i;
  i = fmt_str(s," "); len += i; if (s) s += i;
  i = fmt_uint(s,dt->year + 1900); len += i; if (s) s += i;
  i = fmt_str(s," "); len += i; if (s) s += i;
  i = fmt_uint0(s,dt->hour,2); len += i; if (s) s += i;
  i = fmt_str(s,":"); len += i; if (s) s += i;
  i = fmt_uint0(s,dt->min,2); len += i; if (s) s += i;
  i = fmt_str(s,":"); len += i; if (s) s += i;
  i = fmt_uint0(s,dt->sec,2); len += i; if (s) s += i;
  i = fmt_str(s," -0000\n"); len += i; if (s) s += i;
  return len;
}



long ltTime()
{
    return (long)time(0);
}

/* Conver the long time to format date 
     %y   ---- 2 bytes years
     %Y   ---- 4 bytes years
     %m   ----- month
     %d   ----- day
     %H   ------ Hours
     %M   ------ mintus
     %S   ------- Second
 */

char *ltTimeFormat(char *pFormat,long lTime)
{
    struct tm * sTim;
    char *p;
    char caTemp[32];
    static char pD[32];
    long lTemp;
    p=pFormat;
    sTim = localtime(&lTime);
    memset(pD,0,sizeof(pD));
    while(*p) {
        if(*p == '%') {
            p++;
            switch (*p){
                case 'Y':
                    lTemp = sTim->tm_year + 1900;
                    break;
                case 'y':
                    lTemp = sTim->tm_year;
                    if(lTemp > 100) lTemp = lTemp - 100;
                    break;
                case 'd':
                    lTemp = sTim->tm_mday;
                    break;
                case 'm':
                    lTemp = sTim->tm_mon + 1;
                    break;
                case 'M':
                    lTemp = sTim->tm_min;
                    break;
                case 'H':
                    lTemp = sTim->tm_hour;
                    break;
                case 'S':
                    lTemp = sTim->tm_sec;
                    break;
                default:
                    lTemp = (-1);
                    break;
            }
            if(lTemp >= 0) {
                if(lTemp < 10)
                    sprintf(caTemp,"0%d",lTemp);
                else
                    sprintf(caTemp,"%d",lTemp);
                p++;
                strcat(pD,caTemp);
            }

        }
        else {
            sprintf(caTemp,"%c",*p);
            strcat(pD,caTemp);
            p++;
        }
    }
    return &pD[0];
}



/*将一定格式的时间转换为整形日期 */
long ltTimeStrToLong(char *fmt,char *instr)
{
  time_t t;
  int i,k,j,yd,yy=1970,mm=1,dd=1,hh=0,ff=0,ss=0;
  char tmp[32];
  char tmp1[32];
  int mday[12]={31,28,31,30,31,30,31,31,30,31,30,31};
  long tt,base;
  yd=0;
  i=0;
  j=0;
  time(&t);
  base=28800;
  while(fmt[i]!=0&&instr[j]!=0)
  {
    if(fmt[i]=='%')
    {
      i++;
/*
      k=0;

      while(instr[j]>='0' && instr[j]<='9' && instr[j]!=0)
      {
        tmp[k]=instr[j];
        k++;
        j++;
      }
      if(k==0) return (-9);
      tmp[k]=0;
*/
      switch (fmt[i])
      {
         case 'Y':    
          memcpy(tmp1,&instr[j],4);
          tmp1[4] = 0;
          j = j+4;
          yy=atoi(tmp1);
          if(yy<1900) return (-1);
          break;
         case 'y':
          memcpy(tmp1,&instr[j],2);
          tmp1[2] = 0;
          j = j + 2;
          yy=1900+atoi(tmp1);
          if(yy>1000) return (-1);
          break;
         case 'm':
          memcpy(tmp1,&instr[j],2);
          tmp1[2] = 0;
          j = j + 2;
          mm=atoi(tmp1);
          if(mm>12&&mm<1) return (-2);
          break;
         case 'H':
          memcpy(tmp1,&instr[j],2);
          tmp1[2] = 0;
            j = j + 2;
          hh=atoi(tmp1);
          if(hh>23&&hh<0) return (-4);
          break;
         case 'd':
          memcpy(tmp1,&instr[j],2);
          tmp1[2] = 0;
           j = j + 2;
          dd=atoi(tmp1);
          if(dd>31&&dd<1) return (-3);
          break;
         case 'M':
          memcpy(tmp1,&instr[j],2);
          tmp1[2] = 0;
            j = j + 2;
          ff=atoi(tmp1);
          if(ff>59&&ff<0) return (-5);
          break;
         case 'S':
          memcpy(tmp1,&instr[j],2);
          tmp1[2] = 0;
          j = j + 2;
          ss=atoi(tmp1);
          if(ss>59&&ss<0) return (-6);
          break;
        }
        i++;
    }
      else
      {
        if(instr[j]!=0&&fmt[i]!=0)
        {
          i++;j++;
        }
      }
    }
    yd=0;
  for(i=1970;i<yy;i++)
  {
    if( (i % 400)==0) 
       yd+=366;
    else if((i%4)==0&&(i%100)!=0)
       yd+=366;
    else yd+=365;
  }
  for(i=1;i<mm;i++)
  {
    if(i==2)
    {
      if( (yy%400)==0)
        yd+=29;
      else if((yy%4)==0&&(yy%100)!=0)
        yd+=29;
      else
        yd+=28;
    }
    else
     yd+=mday[i-1];
  }  
  yd+=dd-1;
  tt=yd*24*60*60+hh*60*60+ff*60+ss-base;
  return tt;
}
              


/* 将字符串的时间yyyy-mm-dd 转换为 yyymmdd  */
int ltTimeStr10To8(char *pInTime,char *pFormat)
{
    long lTime;
    lTime = ltTimeStrToLong("%Y-%m-%d",pInTime);
    strcpy(pFormat,ltTimeFormat("%Y%m%d",lTime));
    return 0;
}

int ltTimeNextDay(char *caInDate,char *caOutDate)


{
    long lTime;
    char caFmt[24];
    sprintf(caFmt,"%s000000",caInDate);
    lTime = ltTimeStrToLong("%Y%m%d%H%M%S",caFmt);
    lTime = lTime + 86400;
    strcpy(caOutDate,ltTimeFormat("%Y%m%d",lTime));
    return 0;
}


int ltTimePrevDay(char *caInDate,char *caOutDate)
{
    long lTime;
    char caFmt[24];
    sprintf(caFmt,"%s000000",caInDate);
    lTime = ltTimeStrToLong("%Y%m%d",caFmt);
    lTime = lTime - 86400;
    strcpy(caOutDate,ltTimeFormat("%Y%m%d",lTime));
    return 0;
}

/* 检查日期的合法性  
    年的范围在 1900--2100之间
    >0  -- 合法
    -1 -- 年不合法
    -2 -- 月不合法
    -3 -- 日不合法
*/

int ltTimIsValidDate(int iYear,int iMonth,int iDay)
{
    if(iYear > 2100 || iYear < 1900) return (-1);
    if(iMonth < 1 || iMonth > 12)  return (-2);
    if(iMonth==1 || iMonth==3 || iMonth==5 || iMonth==7 
        || iMonth==8 || iMonth==10 || iMonth==12) {
        if(iDay < 1 || iDay > 31) return (-3);
    }
    else if(iMonth==4 || iMonth==6 || iMonth==9 || iMonth==11) {
        if(iDay < 1 || iDay > 30) return (-3);
    }
    else if( (iYear % 400)==0) {
        if(iDay < 1 || iDay > 29) return (-3);
    }
    else if( (iYear%4)==0 && (iYear%100)!=0) {
        if(iDay < 1 || iDay > 29) return (-3);
    }
    else {
        if(iDay < 1 || iDay > 28) return (-3);
    }
    return (iYear * 10000 + iMonth*100 + iDay);
}

