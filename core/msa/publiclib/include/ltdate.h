#ifndef LTDATE_H
#define LTDATE_H


struct datetime {
  int hour;
  int min;
  int sec;
  int wday;
  int mday;
  int yday;
  int mon;
  int year;
} ;

typedef long datetime_sec;

extern void datetime_tai(struct datetime *dt,datetime_sec t);
extern datetime_sec datetime_untai(struct datetime *dt);

extern unsigned int date822fmt(char *s,struct datetime *dt);
#define DATE822FMT 60

/* Conver the long time to format date 
     %y   ---- 2 bytes years
     %Y   ---- 4 bytes years
     %m   ----- month
     %d   ----- day
     %H   ------ Hours
     %M   ------ mintus
     %S   ------- Second
 */
char *ltTimeFormat(char *pFormat,long lTime);
long ltTime();

/*将一定格式的时间转换为整形日期 */
long ltTimStrToLong(char *fmt,char *instr);
/* 将字符串的时间yyyy-mm-dd 转换为 yyymmdd  */
int ltTimStr10To8(char *pInTime,char *pFormat);
/*时间格式yyyymmdd*/
int ltTimeNextDay(char *caInDate,char *caOutDate);
int ltTimePrevDay(char *caInDate,char *caOutDate);
/* 检查日期的合法性  
    年的范围在 1900--2100之间
    >0  -- 合法
    -1 -- 年不合法
    -2 -- 月不合法
    -3 -- 日不合法
*/
int ltTimIsValidDate(int iYear,int iMonth,int iDay);

#endif


