#ifndef STR_H
#define STR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

typedef struct {
    long lRand;
    long lNumber;
} ltStrRandNumber;


/*str.c*/
extern unsigned int str_copy(char *,char *);
extern int str_diff(char *,char *);
extern int str_diffn(char *,char *,unsigned int);
extern unsigned int str_len(char *);
extern unsigned int str_chr(char *,int);
extern unsigned int str_rchr(char *,int);
extern int str_start(char *,char *);

#define str_equal(s,t) (!str_diff((s),(t)))
#define ltStrBitIs(x,y) ( (x>>(y-1))&1 )


/*str01.c*/
/*删除空格*/
char *ltStrDelSpaces(char *pInstr);
/*大写 返回长度*/
int ltStrToUpper(char *pInstr);
int ltStrToLower(char *pInstr);
/*大写 不超过255字节*/
char *ltStrUpper(char *pInstr);

char *ltStrGetWord(char *p,char *pOut,
       short nMaxLen,char *pTerm);
int ltStrCharIsIn(char *pTerm,char c);
int ltStrIsSpaces(char *pIn);
char *ltStrSkipSpaces(char *pIn);
int ltStrIsSpaces1(char *pIn);
char *ltStrSkipSpaces1(char *pIn);
char *ltStrGetWord1(char *p,char *pOut,
       short nMaxLen,char *pTerm);

int ltStrHanZi(char *pInstr,int iPos);
int ltStrRand(long iNumber,ltStrRandNumber *psStrRand);
int ltStrCompare_001(const void *p1,const void *p2);
char *ltStrAddOneStr(char *pOutStr,char *pInStr,int iLen);
char *ltStrncpy(char *pOutstr,char *pInstr,int iLen);
long ltStrGetId();




/* 判断字符串是否为有效的名字  可打印的汉字，a--z A--Z _ 0-9 . 否则转换为_*/
int ltStrCvtValidName(unsigned char *caName);
/* 判断字符串是否为有效的名字  可打印的汉字，a--z A--Z _ 0-9 */
int ltStrIsValidName(unsigned char *caName);




/* 取记录集中的内容
   iRow --- 从当前位置开始的记录序号, 从1开始
 */
char *ltDbRecordValue(char *caRecord,int iRow,int iSumField,...);

unsigned long ltFileSkipUntilStr(FILE *fp,char *pTerm,int lTerm);
/* 从文件中读一个词, 以 caVar开头, 以 pTerm结束, 最大长度为 iMaxLen

 */
int ltFileReadWord(FILE *fp,
                char *caVar,int iVlen,
                char *caWord,int iMaxLen,int *iRetLen,
                char *pTerm,int iLen);
/* 从文件中读一个词, 以 caVar开头, 以 pTerm结束, 最大长度为 iMaxLen

 */
int ltFileReadWordTermByChar(FILE *fp,
                char *caVar,int iVlen,
                char *caWord,int iMaxLen,int *iRetLen,
                char *pTerm);
char *ltFileReadAllFile(char *pFile,unsigned long *lSize);
int ltStrGetThisWordTermByStr(char *pBuffer,int iBytes,char *caVar,int iVlen,
                char *caWord,char iMaxLen,int *iRetLen,
                char *pTerm,int iLen);
/* 取指定的字符串，返回最后的指针 */
char *ltStrGetThisWord(char *pBuffer,int iBytes,char *caVar,int iVlen,
                char *caWord,int  iMaxLen,int *iRetLen,
                char *pTerm,int iLen);
int ltStrGetThisWordTermByChar(char *pBuffer,int iBytes,char *caVar,int iVlen,
                char *caWord,int iMaxLen,int *iRetLen,
                char *pTerm);




#endif
