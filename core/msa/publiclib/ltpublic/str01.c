#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "lttype.h"
#include "ltstr.h"

char *ltStrDelSpaces(char *pInstr)
{
    register int i,j;
    j = strlen(pInstr) - 1;
    for(i=0;pInstr[i] == ' ' || pInstr[i] == '\t';i++);
    for(;pInstr[j] == ' ' || 
                 pInstr[j] == '\t' || pInstr[j]=='\n' || pInstr[j]=='\r';j--);
    pInstr[j + 1] = 0;
    return &pInstr[i];
}

int ltStrToUpper(char *pInstr)
{
    register int i;
    for(i=0;pInstr[i]!=0;i++)
        pInstr[i]=toupper(pInstr[i]);
    return i;
}

int ltStrToLower(char *pInstr)
{
    register int i;
    for(i=0;pInstr[i]!=0;i++)
        pInstr[i]=tolower(pInstr[i]);
    return i;
}

char *ltStrUpper(char *pInstr)
{
    char *ca;
    register int i;
    ca=(char *)malloc(255);
    memset(ca,0,255);
    for(i=0;pInstr[i]!=0;i++)
        ca[i]=toupper(pInstr[i]);
    ca[i]=0;
    return &ca[0];
}

char *ltStrGetWord(char *p,char *pOut,
                   short nMaxLen,char *pTerm)
{
    register int i=0;
    if(*p == '"') {
       p++;
       for(;i<nMaxLen&& (*p)!='"'&& (*p)!=0;i++,p++) {
           pOut[i]=(*p);
       }
       pOut[i]=0;
       if((*p) == '"') p++;
    }
    else {    
        if(*p) {
            while(!ltStrCharIsIn(pTerm,(*p))&&i<nMaxLen&&(*p)) {
                pOut[i]=(*p);
                p++;
                i++;
            }
        }
        if(i>0) {
            i--;
            for(;pOut[i]==' '&&i>0;i--);
            pOut[i+1]=0;
        }
        else
          pOut[i]=0;
    }
    return p;
}

/* 和上一个函数的主要区别是该函数可以过滤注释
 */
char *ltStrGetWord1(char *p,char *pOut,
                   short nMaxLen,char *pTerm)
{
    register int i=0;
    if(*p == '"') {
       p++;
       for(;i<nMaxLen&& (*p)!='"'&& (*p)!=0;i++,p++) {
           pOut[i]=(*p);
       }
       pOut[i]=0;
       if((*p) == '"') p++;
    }
    else {    
        if(*p) {
            while(!ltStrCharIsIn(pTerm,(*p))&&i<nMaxLen&&(*p)) {
                if(*p == '/' && *(p+1) == '*') {
                    p = p +2;
                    while(*p) {
                        if(*p == '*' && *(p+1) == '/') {
                            p = p +2;
                            break;
                        }
                        else p++;
                    }
                }
                else {
                    pOut[i]=(*p);
                    p++;
                    i++;
                }
            }
        }
        if(i>0) {
            i--;
            for(;pOut[i]==' '&&i>0;i--);
            pOut[i+1]=0;
        }
        else
          pOut[i]=0;
    }
    return p;
}

char *ltStrSkipSpaces1(char *pIn)
{
    while(1) {
        while(*pIn==' '||*pIn=='\t' ||*pIn=='\n' || *pIn=='\r') {
            pIn++;
        }
        if(*pIn) {
            if(*pIn == '/' && *(pIn+1) == '*') {
                pIn = pIn +2;
                while(*pIn) {
                    if(*pIn == '*' && *(pIn+1) == '/') {
                        pIn = pIn +2;
                        break;
                    }
                    else {
                        pIn++;
                    }
                }
            }
            else {
                return pIn;
            }
        }
        else {
            return pIn;
        }
    }
    return pIn;
}

int ltStrIsSpaces1(char *pIn)
{
    char *p;
    p=pIn;
    while(1) {
        while(*p==' '||*p==9 ||*p=='\n' ||*p == '\r')
            p++;
        if(*p==0) return 1;
        else {
            if(*p == '/' && *(p+1) == '*') {
                p = p +2;
                while(*p) {
                    if(*p == '*' && *(p+1) == '/') {
                        p = p +2;
                        break;
                    }
                    else p++;
                }
            }
            else {
                return 0;
            }
        }
    }
    return 1;
}


int ltStrCharIsIn(char *pTerm,char c)
{
    register int i;
    for(i=0;pTerm[i]!=0;i++) {
        if(pTerm[i] == c) return 1;
    }
    return 0;
}

int ltStrIsSpaces(char *pIn)
{
    char *p;
    p=pIn;
    while(*p==' '||*p==9 ||*p=='\n' || *p=='\r')
        p++;
    if(*p!=0) return 0;
    else return 1;
}

char *ltStrSkipSpaces(char *pIn)
{
   while(*pIn==' '||*pIn=='\t' ||*pIn=='\n' || *pIn=='\r') pIn++;
   return pIn;
}


/* Return
   0--Left  1-- Right   2--ASCII  */
int ltStrHanZi(char *pInstr,int iPos)
{
	register int i;
	int iLeft;
        iLeft= 2;
	for(i=iPos;i>=0 && ( (int)pInstr[i]<0 || pInstr[i]> 126);i--) {
		if(iLeft) iLeft =0;
		else iLeft = 1;
	}
	return iLeft;
}

int ltStrRand(long iNumber,ltStrRandNumber *psStrRand)
{
    register int i;
    for(i=0;i<iNumber;i++) {
      psStrRand[i].lRand=rand();
    }
    qsort((void *)psStrRand,(size_t)iNumber,(size_t)sizeof(ltStrRandNumber),ltStrCompare_001);
    return 0;
}

int ltStrCompare_001(const void *p1,const void *p2)
{
    return memcmp(p1,p2,sizeof(long));
}



char *ltStrAddOneStr(char *pOutStr,char *pInstr,int iLen)
{
    register int l,i;
    for(i=0,l=strlen(pOutStr);i<iLen&&pInstr[i];i++,l++) 
        pOutStr[l]=pInstr[i];
    pOutStr[l]=0;
    return pOutStr;
}

 
char *ltStrncpy(char *pOutstr,char *pInstr,int iLen)
{
    register int i;
    for(i=0;i<iLen && pInstr[i];i++) 
        pOutstr[i]=pInstr[i];   
    pOutstr[i]=0;
    return pOutstr;
}


long ltStrGetId()
{
    long lId;
    static long  lSumId;
    lId = getpid() * 10000 + lSumId;
    lSumId ++;
    return lId;
}

int ltStrIsDigital(char *InStr)
{
   register int i;
   for(i=0;InStr[i]!=0;i++) {
     if(InStr[i]>'9'||InStr[i]<'0') {
         return 0;
     }
   }
   return 1;
}


int ltStrGet_l(char *pPrompt,long *lReturn)
{
    char caTemp[64];
    printf("%s",pPrompt);
    fgets(caTemp,64,stdin);
    caTemp[strlen(caTemp)-1] = 0;
    if(ltStrIsDigital(caTemp)) {
        *lReturn = atol(caTemp);
        return 0;
    }
    else {
        return (-1);
    }
}


int ltStrGet_s(char *pPrompt,char *pOut)
{
    printf("%s",pPrompt);
    fgets(pOut,256,stdin);
    pOut[strlen(pOut)-1] = 0;
    return strlen(pOut);
}


int ltStrGetd_l(char *pPrompt, long *lReturn)
{
    char caTemp[64];
    printf("%s[%d]:",pPrompt, *lReturn);
    fgets(caTemp,64,stdin);
    if(strlen(caTemp) == 1) {
        return 0;
    }
    caTemp[strlen(caTemp)-1] = 0;
    if(ltStrIsDigital(caTemp)) {
        *lReturn = atol(caTemp);
        return 0;
    }
    else {
        return (-1);
    }
}


int ltStrGetd_s(char *pPrompt,char *pOut)
{
    char caTemp[256];
    printf("%s[%s]:",pPrompt,pOut);
    fgets(caTemp,256,stdin);
    caTemp[strlen(caTemp)-1] = 0;
    if(strlen(caTemp) > 0)
       strcpy(pOut,caTemp);
    return strlen(pOut);
}

int ltStrGetd_d(char *pPrompt,char *pFormat,long *lOut)
{
    char caTemp[256];
    int iReturn;    
    printf("%s[%s]:",pPrompt,
         ltTimeFormat("%y-%m-%d %H:%M:%S",*lOut));
    fgets(caTemp,256,stdin);
    caTemp[strlen(caTemp)-1] = 0;
    if(strlen(caTemp) > 0) {
       iReturn = ltTimeStrToLong(pFormat,caTemp);
       if(iReturn < 0) return (-1);
       *lOut = iReturn;
    }
    return 0;
}

/* 统计 c在字符串中出现的次数  */
int ltStrCountChar(char *pIn,char c)
{
    char *p;
    int iCount;
    p = pIn;
    while(*p) {
        if(*p == c) iCount++;
        p++;
    }
    return iCount;
}




/* 判断字符串是否为有效的名字  可打印的汉字，a--z A--Z _ 0-9 */

int ltStrIsValidName(unsigned char *caName)
{
    register int i,j;
    int l;
    /* 去掉中间的空格, 并转换为小写 */
    j = 0;
    for(i=0;caName[i]!=0;i++) {
        if(caName[i] != ' ') {
            caName[j] = tolower(caName[i]);
            j++;
        }
    }
    caName[j] = 0;
    /* 可见和不可见字符  */
    for(i=0;caName[i]!=0;i++) {
        if(caName[i] < 128) { /* ASCII */
            if( (caName[i] <= 'z' && caName[i] >= 'a') || caName[i] == '_'  || caName[i] == '.' ||
               (caName[i] <='9' && caName[i] >= '0') ) {
            }
            else {
                return 0;
            }
        }
        else {
            if(caName[i] > 175 && caName[i] < 248) {
                if(caName[i+1] > 160 && caName[i+1] < 255) {
                    i++;
                }
                else {
                    return 0;
                }
            }
            else {
                return 0;
            }
        }
    }
    return 1;
}


/* 判断字符串是否为有效的名字  可打印的汉字，a--z A--Z _  0-9 .*/

int ltStrCvtValidName(unsigned char *caName)
{
    register int i,j;
    int l;
    /* 去掉中间的空格, 并转换为小写 */
    j = 0;
    for(i=0;caName[i]!=0;i++) {
        if(caName[i] != ' ' ) {
            caName[j] = tolower(caName[i]);
            j++;
        }
    }
    caName[j] = 0;
    /* 可见和不可见字符  */
    for(i=0;caName[i]!=0;i++) {
        if(caName[i] < 128) { /* ASCII */
            if( (caName[i] <= 'z' && caName[i] >= 'a') || caName[i] == '_' || caName[i] == '.' || (caName[i] <='9' && caName[i] >= '0')) {
            }
            else {
                caName[i] = '_';
            }
        }
        else {
            if(caName[i] > 175 && caName[i] < 248) {
                if(caName[i+1] > 160 && caName[i+1] < 255) {
                    i++;
                }
                else {
                    caName[i] = '_';
                }
            }
            else {
                caName[i] = '_';
            }
        }
    }
    return 1;
}



/*   将字符串分解为n个字符, 互相之间以空格分隔
     在分解过程中自动处理引号和注释
     如:
     iReturn = ltStrSplit(pIn,"\n\0\r ",4,caVar1,32,caVar2,32,caVar3,32,caVar4,32);
     

 */
 
int ltStrSplit(char *pIn,char *pTerm,char iNum,...)
{
    va_list ap;
    int i;
    long lBytes;
    char *p,*pValue;
    va_start(ap,iNum);
    p = ltStrSkipSpaces1(pIn);
    i=0;
    while(*p && i<iNum) {
        pValue = va_arg(ap,char *);
        lBytes = va_arg(ap,int);
        p = ltStrGetWord1(p,pValue,lBytes,pTerm);
        p = ltStrSkipSpaces1(p);
        i++;
    }
    return i;
}

/* 将字符串置为空
*/
#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

int ltStrSetNull(int iSum,...)
{
    register int i;
    char *p;
    va_list ap;
    va_start(ap,iSum);
    for(i=0;i<iSum;i++) {
        p = va_arg(ap,char *);
        p[0] = 0;
    }
    va_end(ap);
}

/* 在字符串后面加一个带格式的字符串  */
int ltStrAddF(char *caOut,char *pFormat,...)
{
    int l;
    va_list ap;
    l = strlen(caOut);
    va_start(ap,pFormat);
    vsprintf(caOut+l,pFormat,ap);
    return 0;
}

/* 位操作 
     一个字符的第 iNum 位是0还是1
     iNum从0开始
 */
    

/*
int ltStrBitIs(unsigned char cChar,int iNum)
{
    return (int)(cChar & (1<<iNum))>>iNum;
}
*/

/* 将一个字符的第 iNum位设为0或1  */
unsigned char ltStrSetBit(unsigned char cChar,int iNum,int iValue)
{
    if(iValue==0) {
        return (cChar & ( 255 - (1<<iNum)));
    }
    else {
        return (cChar | (1<<iNum));
    }
}


int ltStrBitAnd(unsigned char *p1,unsigned char *p2,int iLen)
{
    register int i;
    for(i=0;i<iLen;i++) {
        p1[i] = p1[i] & p2[i];
    }
    return 0;
}


int ltStrBitOr(unsigned char *p1,unsigned char *p2,int iLen)
{
    register int i;
    for(i=0;i<iLen;i++) {
        p1[i] = p1[i] | p2[i];
    }
    return 0;
}

/* 取记录集中的内容
   iRow --- 从当前位置开始的记录序号, 从1开始
 */
char *ltDbRecordValue(char *caRecord,int iRow,int iSumField,...)
{
    char   caValue[12];
    char   *p,*pValue;
    long   *lValue;
    short  *nValue;
    double *dValue;
    char   caTemp[50];
    int i,iType,iStart,l,l1,iLen;
    va_list args;
    va_start(args,iSumField);
    p = caRecord;
    iStart = 1;
    while(iStart < iRow) {
        p = ltStrGetWord(p,caValue,11,",");
        if(*p == ',') {
            p++;
            l = atol(caValue);
            p = p + l + 1;
            if(*(p-1) == '|') {
                iStart ++;
            }
        }
        else {
            va_end(args);
            return NULL;
        }
    }
    for(i=0;i<iSumField;i++) {
        p = ltStrGetWord(p,caValue,11,",");
        if(*p != ',') {
            va_end(args);
            return NULL;
        }
        p++;
        l = atol(caValue);
        iType = va_arg(args,int);
        iLen  = va_arg(args,int);
        switch(iType) {
            case LT_TYPE_STRING:
                 pValue = va_arg(args,char *);
                 l1 = min(iLen,l);
                 memcpy(pValue,p,l1);
                 pValue[l1]=0;
                 break;
            case LT_TYPE_LONG:
                 lValue = va_arg(args,long *);
                 l1 = min(l,49);
                 memcpy(caTemp,p,l1);
                 caTemp[l1]=0;
                 *lValue = atol(caTemp);
                 break;
            case LT_TYPE_SHORT:
                 nValue = va_arg(args,short *);
                 l1 = min(l,49);
                 memcpy(caTemp,p,l1);
                 caTemp[l1]=0;
                 *nValue = (short)atol(caTemp);
                 break;
            case LT_TYPE_FLOAT:
                 dValue = va_arg(args,double *);
                 l1 = min(l,49);
                 memcpy(caTemp,p,l1);
                 caTemp[l1]=0;
                 *dValue = (double)atof(caTemp);
                 break;
            default:
                 pValue = va_arg(args,char *);
                 memcpy(pValue,p,l);
                 pValue[l]=0;
                 break;
        }
        p = p + l + 1;
    }
    va_end(args);
    return  p;
}



/*  读文件直到满足条件的字符串
    返回: >0  -- 最后一个字符在文件中的位置
          0   -- 没有满足条件的字符串

 */
unsigned long ltFileSkipUntilStr(FILE *fp,char *pTerm,int lTerm)
{
    unsigned long lPos,lOffset;
    int iRead;
    char buf[8192];
    char *p;
    
    lOffset = 0;
	while(1) {
	    lPos = ftell(fp);    /* 取当前位置  */
	    iRead = fread(buf+lOffset,1,7168,fp);
	    if(iRead > 0) {
	        if( (p = (char *)memmem(buf,iRead+lOffset,pTerm,lTerm))) {
	            lPos = lPos + p - buf + lTerm;
	            fseek(fp, lPos, SEEK_SET);
	            return lPos;
	        }
	        else {
	            if(iRead < 7168) {
	                return 0;
	            }
	            else {
	                memcpy(buf,buf+iRead-lTerm,lTerm);
	                lOffset = lTerm;
	            }
	        }
	    }
	    else {
	        return 0;
	    }
	}
}


/* 从文件中读一个词, 以 caVar开头, 以 pTerm结束, 最大长度为 iMaxLen

 */
int ltFileReadWord(FILE *fp,
                char *caVar,int iVlen,
                char *caWord,int iMaxLen,int *iRetLen,
                char *pTerm,int iLen)
{
    char *pBuf;
    char *p;
    int iReturn,iRead;
    unsigned long lPos;

    if(caVar) {
        iReturn = ltFileSkipUntilStr(fp,caVar,iVlen);
        if(iReturn <= 0) return (-1);
    }
    lPos = ftell(fp);
    pBuf = malloc(iMaxLen+1 + iLen);    
    iRead = fread(pBuf,1,iMaxLen + iLen,fp);
    if(iRead > 0) {
        if(p=(char *)memmem(pBuf,iRead,pTerm,iLen)) {
            iRead = p - pBuf;
            memcpy(caWord,pBuf,iRead);
            lPos = lPos + iRead + iLen;
            fseek(fp, lPos, SEEK_SET);
            *iRetLen = iRead;
            caWord[iRead]=0;
            free(pBuf);
            return 0;
        }
        else {
            return (-1);
        }
    }
}


/* 从文件中读一个词, 以 caVar开头, 以 pTerm结束, 最大长度为 iMaxLen

 */
int ltFileReadWordTermByChar(FILE *fp,
                char *caVar,int iVlen,
                char *caWord,int iMaxLen,int *iRetLen,
                char *pTerm)
{
    char *pBuf;
    char *p;
    int iReturn,iRead;
    register int i;
    unsigned long lPos;

    if(caVar) {
        iReturn = ltFileSkipUntilStr(fp,caVar,iVlen);
        if(iReturn <= 0) return (-1);
    }
    lPos = ftell(fp);
    pBuf = malloc(iMaxLen+1);    
    iRead = fread(pBuf,1,iMaxLen + 1,fp);
    if(iRead > 0) {
        iMaxLen = min(iRead,iMaxLen);
        p = pBuf;
        for(i=0;i<iMaxLen && !strchr(pTerm,(*p));i++,p++) caWord[i] = (*p);
        caWord[i]=0;
        lPos = lPos + i;
        fseek(fp, lPos, SEEK_SET);
        *iRetLen = i;
        free(pBuf);
        return 0;
    }
    else {
        return (-1);
    }
}



char *ltFileReadAllFile(char *pFile,unsigned long *lSize)
{
    
   	struct stat st;
    int iFd;
    char *pBuf;
    
    iFd=open(pFile,O_RDONLY);
    if(iFd == (-1)) {
        return NULL;
    }
    fstat(iFd,&st);
    if(pBuf = malloc(st.st_size + 1)) {
        read(iFd,pBuf,st.st_size);
        close(iFd);
        pBuf[st.st_size]=0;
    }
    *lSize = st.st_size;
    return pBuf;
}

int ltStrGetThisWordTermByStr(char *pBuffer,int iBytes,char *caVar,int iVlen,
                char *caWord,char iMaxLen,int *iRetLen,
                char *pTerm,int iLen)
{
    int lLeft,iReadLen;
    char *p1;
    char *p;
    p = (char *)memmem(pBuffer,iBytes,caVar,iVlen);
    if(p) {
        p = p + iVlen;
        lLeft =  iBytes - (p - pBuffer);
        iReadLen = min(iMaxLen + iLen,lLeft);
        p1 = p;
        p = (char *)memmem(p1,iReadLen,pTerm,iLen);
        if(p) {
            memcpy(caWord,p1,p-p1);
            *iRetLen = p - p1;
            caWord[*iRetLen]=0;
            return 0;
        }
        else {
            return (-1);
        }
    }
    return (-1);
}


/* 取指定的字符串，返回最后的指针 */
char *ltStrGetThisWord(char *pBuffer,int iBytes,char *caVar,int iVlen,
                char *caWord,int  iMaxLen,int *iRetLen,
                char *pTerm,int iLen)
{
    int lLeft,iReadLen;
    char *p1;
    char *p;
    p = (char *)memmem(pBuffer,iBytes,caVar,iVlen);
    if(p) {
        p = p + iVlen;
        lLeft =  iBytes - (p - pBuffer);
        iReadLen = min(iMaxLen + iLen,lLeft);
        p1 = p;
        p = (char *)memmem(p1,iReadLen,pTerm,iLen);
        if(p) {
            memcpy(caWord,p1,p-p1);
            *iRetLen = p - p1;
            caWord[*iRetLen]=0;
            return p+iLen;
        }
        else {
            return NULL;
        }
    }
    return NULL;
}

int ltStrGetThisWordTermByChar(char *pBuffer,int iBytes,char *caVar,int iVlen,
                char *caWord,int iMaxLen,int *iRetLen,
                char *pTerm)
{
    int lLeft,iReadLen;
    char *p1;
    char *p;
    register int i;
    p = (char *)memmem(pBuffer,iBytes,caVar,iVlen);
    if(p) {
        p = p + iVlen;
        lLeft =  iBytes - (p - pBuffer);
        lLeft = min(lLeft,iMaxLen);
        for(i=0;i<lLeft && !strchr(pTerm,(*p)) ;i++,p++) caWord[i] = *p;
        caWord[i] = 0;
        *iRetLen = i;
        if(i<lLeft) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        *iRetLen = 0;
        return EOF;
    }
}


