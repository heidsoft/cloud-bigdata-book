/*  
   消息处理程序
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


#include "error.h"
#include "ltmsg.h"

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

static int ltMsgFPrint(char *pHead,char *pVar,short nType,long lLen,char *pValue);
long   ltMsgValidCode=0;

/*=====================================*/


void ltMsgSetValidCode(long lMsgCode)
{
    ltMsgValidCode = lMsgCode;
}


int ltMsgIsValidCode(long lMsgCode)
{
    int iTemp;
    if(ltMsgValidCode != 0) {
       iTemp = lMsgCode ;
       if(iTemp != ltMsgValidCode)
          return 0;
       else
          return 1;
    }
    else
        return 1;
}

long  ltMsgGetCode()
{
    return ltMsgValidCode;
}



ltMsgHead *ltMsgInit(unsigned long  lCode,
                     unsigned long  lFunCode,
                     unsigned long  lMsgId,
                     unsigned long  lMaxBytes)
{
    ltMsgHead   *psMsgHead;
 
    if( (psMsgHead = (ltMsgHead *)malloc(sizeof(ltMsgHead) + lMaxBytes)) 
            == NULL) {
        return (ltMsgHead *)NULL;
    }
    memset((char *)psMsgHead,0,sizeof(ltMsgHead));
    psMsgHead->lCode =  lCode;
    psMsgHead->lMsgId = lMsgId;    /* if the Value is 0, It will be set in send */
    psMsgHead->nVersion = LT_MSG_VERSION;
    psMsgHead->cCrypt =   LT_MSG_NOCRYPT;
    psMsgHead->lMaxBytes = lMaxBytes+sizeof(ltMsgHead);
    psMsgHead->lBytes = sizeof(ltMsgHead);
    psMsgHead->lFunCode = lFunCode;
    psMsgHead->msgpktype = 0;  /*0 一般的msg包，1 html content包    2 数据流包*/ 
    psMsgHead->sid = 0;     /*系统的sessionId*/
    psMsgHead->rid = 0;
    return (ltMsgHead *)psMsgHead;
}


int ltMsgAdd_s(ltMsgHead **psMsgHead0,char *pVarName,char *pVarValue)
{
    uint32 lLen;
    uint16 vLen;
    char *p;
    ltMsgVar sMsgVar;
    ltMsgHead *psMsgHead;
    psMsgHead = (*psMsgHead0);
    lLen = strlen(pVarValue) + 1;
    vLen = min(128,strlen(pVarName) + 1 + 2 * sizeof(uint16) + sizeof(uint32));
    if(psMsgHead->lMaxBytes - psMsgHead->lBytes < lLen + vLen) {
        psMsgHead = (ltMsgHead *)realloc(psMsgHead, 
                psMsgHead->lBytes + lLen + vLen + LT_MSG_ALLOCSTEP);
        if(psMsgHead == (ltMsgHead *)NULL) {
            return (-1);
        }
        else {
           *psMsgHead0 = psMsgHead;
           psMsgHead->lMaxBytes = psMsgHead->lBytes + vLen + lLen + LT_MSG_ALLOCSTEP;
        }
    }
    p = (char *)psMsgHead + psMsgHead->lBytes;
    sMsgVar.nType = LT_TYPE_STRING;
    sMsgVar.lBytes = lLen;
    sMsgVar.nVarLen = vLen;
    strncpy(sMsgVar.caVar,pVarName,LT_MSG_MAXVARLEN);
    memcpy(p,&sMsgVar,vLen);  
    strcpy(p+vLen,pVarValue);
    psMsgHead->lBytes = psMsgHead->lBytes + lLen + vLen;
    return 0;
}


int ltMsgAdd_v(ltMsgHead **psMsgHead0,char *pVarName,char *pVarValue, 
                unsigned long lLen)
{
    char *p;
    ltMsgHead *psMsgHead;
    ltMsgVar   sMsgVar;
    uint16      vLen;
    
    psMsgHead = (*psMsgHead0);
    vLen = min(128,strlen(pVarName) + 1 + 2 * sizeof(uint16) + sizeof(uint32));
    if(psMsgHead->lMaxBytes -psMsgHead->lBytes < lLen + vLen) {
        psMsgHead = (ltMsgHead *)realloc(psMsgHead, 
                psMsgHead->lBytes + lLen + vLen + LT_MSG_ALLOCSTEP);
        if(psMsgHead == (ltMsgHead *)NULL) {
            
            return (-1);
        }
        else {
            *psMsgHead0 = psMsgHead;
            psMsgHead->lMaxBytes = psMsgHead->lBytes + vLen + lLen + LT_MSG_ALLOCSTEP;
        }
    }
    p = (char *)psMsgHead + psMsgHead->lBytes;
    strncpy(sMsgVar.caVar,pVarName,LT_MSG_MAXVARLEN);
    sMsgVar.nType = LT_TYPE_STRUCT;
    sMsgVar.lBytes = lLen;
    sMsgVar.nVarLen = vLen;
    memcpy(p,&sMsgVar,vLen);
    memcpy(p+vLen,pVarValue,lLen);
    psMsgHead->lBytes = psMsgHead->lBytes + lLen + vLen;
    return 0;
}

/* Data put into bufe with network long */
int ltMsgAdd_l(ltMsgHead **psMsgHead0,char *pVarName,long lVarValue)
{

    char *p;
    ltMsgHead *psMsgHead;
    ltMsgVar   sMsgVar;
    uint16      vLen;
    
    psMsgHead = (*psMsgHead0);
    vLen = min(128,strlen(pVarName) + 1 + 2 * sizeof(uint16) + sizeof(uint32));
    if(psMsgHead->lMaxBytes -psMsgHead->lBytes < sizeof(uint32) + vLen) {
        psMsgHead = (ltMsgHead *)realloc(psMsgHead, 
                psMsgHead->lBytes + sizeof(uint32) + vLen + LT_MSG_ALLOCSTEP);
        if(psMsgHead == (ltMsgHead *)NULL) {
           
            return (-1);
        }
        else {
           *psMsgHead0 = psMsgHead;
           psMsgHead->lMaxBytes = psMsgHead->lBytes + vLen + LT_MSG_ALLOCSTEP;
                         + sizeof(uint32);
        }
    }
    p = (char *)psMsgHead + psMsgHead->lBytes;
    strncpy(sMsgVar.caVar,pVarName,LT_MSG_MAXVARLEN);
    sMsgVar.nType = LT_TYPE_LONG;
    sMsgVar.lBytes = sizeof(long);
    sMsgVar.nVarLen = vLen;
    memcpy(p,&sMsgVar,vLen);
    memcpy(p+vLen,&lVarValue,sizeof(long));
    psMsgHead->lBytes = psMsgHead->lBytes + sizeof(long) + vLen;
    return 0;
}
    

/* Data plt into bufe with network short */
int ltMsgAdd_n(ltMsgHead **psMsgHead0,char *pVarName,short nVarValue)
{
    ltMsgHead *psMsgHead;
    char *p;
    ltMsgVar  sMsgVar;
    uint16     vLen;
   
    vLen = min(128,strlen(pVarName) + 1 + 2 * sizeof(uint16) + sizeof(uint32));
    psMsgHead = (*psMsgHead0);
    if(psMsgHead->lMaxBytes -psMsgHead->lBytes < sizeof(uint16) + vLen) {
        psMsgHead = (ltMsgHead *)realloc(psMsgHead, 
                psMsgHead->lBytes + sizeof(uint16) + vLen + LT_MSG_ALLOCSTEP);
        if(psMsgHead == (ltMsgHead *)NULL) {
            
            return (-1);
        }
        else {
           *psMsgHead0 = psMsgHead;
           psMsgHead->lMaxBytes = psMsgHead->lBytes + vLen 
                           + LT_MSG_ALLOCSTEP+LT_MSG_VARLEN + sizeof(uint16);
       }
    }
    p = (char *)psMsgHead + psMsgHead->lBytes;
    strncpy(sMsgVar.caVar,pVarName,LT_MSG_MAXVARLEN);
    sMsgVar.nType = LT_TYPE_SHORT;
    sMsgVar.lBytes = sizeof(short);
    sMsgVar.nVarLen = vLen;
    memcpy(p,&sMsgVar,sizeof(ltMsgVar));
    memcpy(p+vLen,&nVarValue,sizeof(short));
    psMsgHead->lBytes = psMsgHead->lBytes + sizeof(short) + vLen;
    return 0;
}
    

int ltMsgHton(ltMsgHead *psMsgHead)
{
    
    if((psMsgHead->cOrder&LT_MSG_CONTENTNORDER) == 0 && 
         psMsgHead->lBytes > sizeof(ltMsgHead)) {
               
		ltMsgHton1( (char *)psMsgHead + sizeof(ltMsgHead),
                    psMsgHead->lBytes - sizeof(ltMsgHead));
                psMsgHead->cOrder=psMsgHead->cOrder|LT_MSG_CONTENTNORDER;

    }
    ltMsgHton0(psMsgHead);
    return 0;
}

/* Cvt Host Order to Network Order Only Head */
int ltMsgHton0(ltMsgHead *psMsgHead)
{
    register int i;
    /* cvt the Content */
    
    if((psMsgHead->cOrder&LT_MSG_HEADNORDER) == 0) {
        if(psMsgHead->lBytes >= sizeof(ltMsgHead)) {
            psMsgHead->lFunCode = htonl(psMsgHead->lFunCode);
        }
        psMsgHead->lCode  = htonl(psMsgHead->lCode);
        psMsgHead->lBytes = htonl(psMsgHead->lBytes);
        psMsgHead->nVersion = htons(psMsgHead->nVersion);
        psMsgHead->lMsgId = htonl(psMsgHead->lMsgId);
        psMsgHead->lMaxBytes = htonl(psMsgHead->lMaxBytes);
        if(psMsgHead->lSendTime == 0){
            psMsgHead->lSendTime = htonl(time(0));
        }else{
            psMsgHead->lSendTime = htonl(psMsgHead->lSendTime);
	}
        psMsgHead->lCheckSum = htonl(psMsgHead->lCheckSum);
	psMsgHead->comFlag =   htonl(psMsgHead->comFlag);
        psMsgHead->cOrder= LT_MSG_HEADNORDER |psMsgHead->cOrder;

	//psMsgHead->msgpktype = htonl(psMsgHead->msgpktype);
	psMsgHead->sid =   htonl(psMsgHead->sid);
        psMsgHead->rid= htonl(psMsgHead->rid);
    }
    return 0;
}

/* Cvf the Content Only  */
int ltMsgHton1(char *pContent, unsigned long lBytes)
{
    uint32 lValue;
    uint16 nValue,nType,nVarLen;
    uint32 lVBytes;
    register unsigned int i;
    ltMsgVar sMsgVar;
    char *p;
    /* cvt the Content */
    
   
    p= (char *)pContent;
    for(i=0;i<lBytes;) {
        memcpy(&sMsgVar,p,sizeof(ltMsgVar)-LT_MSG_MAXVARLEN);
        nType = sMsgVar.nType;
        lVBytes = sMsgVar.lBytes;
        sMsgVar.nType = htons(nType);
        sMsgVar.lBytes = htonl(lVBytes);
        nVarLen = sMsgVar.nVarLen;
        sMsgVar.nVarLen = htons(sMsgVar.nVarLen);
        memcpy(p,&sMsgVar,sizeof(ltMsgVar)-LT_MSG_MAXVARLEN);
        switch(nType) {
            case LT_TYPE_LONG:
                memcpy(&lValue,p+nVarLen,sizeof(unsigned long));
                lValue = htonl(lValue);
                memcpy(p+nVarLen,&lValue,sizeof(unsigned long));
                break;
            case LT_TYPE_SHORT:
                memcpy(&nValue,p+nVarLen,sizeof(unsigned short));
                nValue = htons(nValue);
                memcpy(p+nVarLen,&nValue,sizeof(unsigned short));
                break;
        }
        p=p+lVBytes + nVarLen;
        i=i+lVBytes + nVarLen;
    }
    return 0;
}


int ltMsgNtoh(ltMsgHead *psMsgHead)
{
   
    ltMsgNtoh0(psMsgHead);
    if((psMsgHead->cOrder&LT_MSG_CONTENTNORDER) == LT_MSG_CONTENTNORDER &&
        psMsgHead->lBytes >= sizeof(ltMsgHead)) {
        
            ltMsgNtoh1( (char *)psMsgHead + sizeof(ltMsgHead),
                psMsgHead->lBytes - sizeof(ltMsgHead));
        
           psMsgHead->cOrder = psMsgHead->cOrder &1;
    }
    return 0;
}

int ltMsgNtoh1(char *pContent,unsigned long lBytes)
{
    uint32 lValue;
    uint16 nValue;
    register unsigned int i;
    ltMsgVar sMsgVar;
    char *p;
    /* cvt the Content */
    p= (char *)pContent;

    for(i=0;i<lBytes;) {
        memcpy(&sMsgVar,p,sizeof(ltMsgVar)-LT_MSG_MAXVARLEN);
        sMsgVar.nType = ntohs(sMsgVar.nType);
        sMsgVar.nVarLen = ntohs(sMsgVar.nVarLen);
        sMsgVar.lBytes = ntohl(sMsgVar.lBytes);
        memcpy(p,&sMsgVar,sizeof(ltMsgVar)-LT_MSG_MAXVARLEN);
        switch(sMsgVar.nType) {
            case LT_TYPE_LONG:
                memcpy(&lValue,p+sMsgVar.nVarLen,sizeof(unsigned long));
                lValue = ntohl(lValue);
                memcpy(p+sMsgVar.nVarLen,&lValue,sizeof(unsigned long));
                break;
            case LT_TYPE_SHORT:
                memcpy(&nValue,p+sMsgVar.nVarLen,sizeof(unsigned short));
                nValue = ntohs(nValue);
                memcpy(p+sMsgVar.nVarLen,&nValue,sizeof(unsigned short));
                break;
        }
        p=p+sMsgVar.lBytes + sMsgVar.nVarLen;
        i=i+sMsgVar.lBytes + sMsgVar.nVarLen;
    }
    return 0;
}


int ltMsgNtoh0(ltMsgHead *psMsgHead)
{
    register int i;
    /* cvt the Content */
    if( (psMsgHead->cOrder & LT_MSG_HEADNORDER) == 1) {
        psMsgHead->lBytes = ntohl(psMsgHead->lBytes);
        if(psMsgHead->lBytes >= sizeof(ltMsgHead)) {
            psMsgHead->lFunCode = ntohl(psMsgHead->lFunCode);
        }
        psMsgHead->lCode  = ntohl(psMsgHead->lCode);
        psMsgHead->lMsgId = ntohl(psMsgHead->lMsgId);
        psMsgHead->lSendTime = ntohl(psMsgHead->lSendTime);
        psMsgHead->lCheckSum = ntohl(psMsgHead->lCheckSum);
        psMsgHead->lMaxBytes = ntohl(psMsgHead->lMaxBytes);
        psMsgHead->nVersion = ntohs(psMsgHead->nVersion);
	psMsgHead->comFlag  =ntohl(psMsgHead->comFlag);
        psMsgHead->cOrder = psMsgHead->cOrder & 2;

	//psMsgHead->msgpktype = htonl(psMsgHead->msgpktype);
	psMsgHead->sid =   ntohl(psMsgHead->sid);
        psMsgHead->rid= ntohl(psMsgHead->rid);
    }
    return 0;
}


char *ltMsgGetVar_s(ltMsgHead *psMsgHead,char *pVarName)
{
    char *p;
    ltMsgVar    sMsgVar;
    register unsigned int i;
   
   
    p= (char *)psMsgHead + sizeof(ltMsgHead);
    for(i=sizeof(ltMsgHead);i<psMsgHead->lBytes;) {
        memcpy(&sMsgVar,p,sizeof(ltMsgVar)-LT_MSG_MAXVARLEN);
        memcpy(sMsgVar.caVar,p+sizeof(ltMsgVar)-LT_MSG_MAXVARLEN,
                   sMsgVar.nVarLen - sizeof(ltMsgVar)+ LT_MSG_MAXVARLEN);
        if(strcmp(sMsgVar.caVar,pVarName)==0) {
            return (char *)(p + sMsgVar.nVarLen);
        }
        else {
            p=p+sMsgVar.lBytes + sMsgVar.nVarLen;
            i=i+sMsgVar.lBytes + sMsgVar.nVarLen;
        }
    }
    return (char *)NULL;
}




char *ltMsgGetVar_v(ltMsgHead *psMsgHead,char *pVarName,
        unsigned long *lBytes)
{
    char *p;
    ltMsgVar    sMsgVar;
    register unsigned int i;
   
    p= (char *)psMsgHead + sizeof(ltMsgHead);
    for(i=sizeof(ltMsgHead);i<psMsgHead->lBytes;) {
        memcpy(&sMsgVar,p,sizeof(ltMsgVar)-LT_MSG_MAXVARLEN);
        memcpy(sMsgVar.caVar,p+sizeof(ltMsgVar)-LT_MSG_MAXVARLEN,
                   sMsgVar.nVarLen - sizeof(ltMsgVar)+ LT_MSG_MAXVARLEN);
        if(strcmp(sMsgVar.caVar,pVarName)==0) {
            *lBytes = sMsgVar.lBytes;
            return (char *)(p + sMsgVar.nVarLen);
        }
        else {
            p=p+sMsgVar.lBytes + sMsgVar.nVarLen;
            i=i+sMsgVar.lBytes + sMsgVar.nVarLen;
        }
    }
    *lBytes = 0;

    return (char *)NULL;
}




long ltMsgGetVar_l(ltMsgHead *psMsgHead,char *pVarName)
{
    char *p;
    long lValue;
    ltMsgVar    sMsgVar;
    register unsigned int i;
   

    p= (char *)psMsgHead + sizeof(ltMsgHead);
    for(i=sizeof(ltMsgHead);i<psMsgHead->lBytes;) {
        memcpy(&sMsgVar,p,sizeof(ltMsgVar)-LT_MSG_MAXVARLEN);
        memcpy(sMsgVar.caVar,p+sizeof(ltMsgVar)-LT_MSG_MAXVARLEN,
                   sMsgVar.nVarLen - sizeof(ltMsgVar)+ LT_MSG_MAXVARLEN);
        if(strcmp(sMsgVar.caVar,pVarName)==0) {
            memcpy(&lValue,p+sMsgVar.nVarLen,sizeof(long));
            return (lValue);
        }
        else {
            p=p+sMsgVar.lBytes + sMsgVar.nVarLen;
            i=i+sMsgVar.lBytes + sMsgVar.nVarLen;
        }
    }

    return (-1);
}



short ltMsgGetVar_n(ltMsgHead *psMsgHead,char *pVarName)
{
    char *p;
    short nValue;
    ltMsgVar    sMsgVar;
    register unsigned int i;

       
    p= (char *)psMsgHead + sizeof(ltMsgHead);
    for(i=sizeof(ltMsgHead);i<psMsgHead->lBytes;) {
        memcpy(&sMsgVar,p,sizeof(ltMsgVar)-LT_MSG_MAXVARLEN);
        memcpy(sMsgVar.caVar,p+sizeof(ltMsgVar)-LT_MSG_MAXVARLEN,
                   sMsgVar.nVarLen - sizeof(ltMsgVar)+ LT_MSG_MAXVARLEN);
        if(strcmp(sMsgVar.caVar,pVarName)==0) {
            memcpy(&nValue,p + sMsgVar.nVarLen,sizeof(short));

            return (nValue);
        }
        else {
            p=p+sMsgVar.lBytes + sMsgVar.nVarLen;
            i=i+sMsgVar.lBytes + sMsgVar.nVarLen;
        }
    }

    return (-1);
}



/*  iNum---The No of Var, start with 1  */

int ltMsgGetThatVar(ltMsgHead *psMsgHead,
            ltMsgVar *psMsgVar,char *pValue,short iNum)
{
    char *p;
    ltMsgVar  sMsgVar;
    register unsigned int i;
    register int j;
    j=1;

    
    p= (char *)psMsgHead + sizeof(ltMsgHead);
    for(i=sizeof(ltMsgHead);i<psMsgHead->lBytes;) {
        memcpy(&sMsgVar,p,sizeof(ltMsgVar)-LT_MSG_MAXVARLEN);
        memcpy(sMsgVar.caVar,p+sizeof(ltMsgVar)-LT_MSG_MAXVARLEN,
                   sMsgVar.nVarLen - sizeof(ltMsgVar)+ LT_MSG_MAXVARLEN);
        if(j==iNum){
            *psMsgVar = sMsgVar;
            memcpy(pValue, p+sMsgVar.nVarLen,sMsgVar.lBytes);
            return 1;
        }
        else {
            p=p+sMsgVar.lBytes + sMsgVar.nVarLen;
            i=i+sMsgVar.lBytes + sMsgVar.nVarLen;
            j++;
        }
    }
    return 0;
}




/*  iNum---The No of Var, start with 1  */

int ltMsgGetVar(ltMsgHead *psMsgHead,
            ltMsgVar *psMsgVar,char *pValue,char *pVarName)
{
    char *p;
    ltMsgVar  sMsgVar;
    register unsigned int i;

    
    p= (char *)psMsgHead + sizeof(ltMsgHead);
    for(i=sizeof(ltMsgHead);i<psMsgHead->lBytes;) {
        memcpy(&sMsgVar,p,sizeof(ltMsgVar)-LT_MSG_MAXVARLEN);
        memcpy(sMsgVar.caVar,p+sizeof(ltMsgVar)-LT_MSG_MAXVARLEN,
                   sMsgVar.nVarLen - sizeof(ltMsgVar)+ LT_MSG_MAXVARLEN);
        if(strcmp(sMsgVar.caVar,pVarName)==0) {
            *psMsgVar = sMsgVar;
            memcpy(pValue, p+sMsgVar.nVarLen,sMsgVar.lBytes);

            return 1;
        }
        else {
            p=p+sMsgVar.lBytes + sMsgVar.nVarLen;
            i=i+sMsgVar.lBytes + sMsgVar.nVarLen;
        }
    }
    return 0;
}


void ltMsgFree(ltMsgHead *psMsgHead)
{
    free(psMsgHead);
    return;
}

 

int ltMsgAllRecord(ltMsgHead *psMsgHead,char *pHead,
     int ltMsgDoRecord(char *pHead,char *pVar,short nType,long lLen,char *pValue))
{
    char *p;
    ltMsgVar    sMsgVar;
    register int j,iReturn;
	register unsigned int i;
    j=0;
   
    p= (char *)psMsgHead + sizeof(ltMsgHead);
    for(i=sizeof(ltMsgHead);i<psMsgHead->lBytes;) {
        memcpy(&sMsgVar,p,sizeof(ltMsgVar)-LT_MSG_MAXVARLEN);
        memcpy(sMsgVar.caVar,p+sizeof(ltMsgVar)-LT_MSG_MAXVARLEN,
                   sMsgVar.nVarLen - sizeof(ltMsgVar)+ LT_MSG_MAXVARLEN);
        iReturn = ltMsgDoRecord(pHead,sMsgVar.caVar,
                        sMsgVar.nType,sMsgVar.lBytes,
                        p+ sMsgVar.nVarLen);
        if(iReturn!=0) return iReturn;
        p= p+ sMsgVar.lBytes +  sMsgVar.nVarLen;
        i=i+sMsgVar.lBytes +  sMsgVar.nVarLen;
        j++;
    }
    return 0;
}



int ltMsgPrintMsg(ltMsgHead *psMsgHead)
{
    printf(" Message Head Information \n");
    printf("======================================================\n");
    printf(" lCode:[%u]  lBytes:[%u]  lMsgId:[%u] lSendTime:[%u]\n",
        psMsgHead->lCode,psMsgHead->lBytes,psMsgHead->lMsgId,psMsgHead->lSendTime);
    printf(" lCheckSum:[%u] nVersion:[%u] cCrypt:[%d] cOrder:[%d] \n",
        psMsgHead->lCheckSum,psMsgHead->nVersion,psMsgHead->cCrypt,psMsgHead->cOrder);
    if(psMsgHead->lBytes >= sizeof(ltMsgHead)) {
        printf("lFunction:[%u] cComFlag:[%u] \n",
            psMsgHead->lFunCode,psMsgHead->comFlag);
       printf("msgpktype:[%d] sid:[%u]  rid:[%u]\n",
            psMsgHead->msgpktype,psMsgHead->sid,psMsgHead->rid);
        printf("========================================================\n");
        printf(" VarName   Bytes  Type      Content                     \n");
        ltMsgAllRecord(psMsgHead,(char *)NULL,ltMsgPrint);
    }
    return 0;
}

int ltMsgPrint(char *pHead,char *pVar,short nType,long lLen,char *pValue)
{
    
    short    nValue;
    long     lValue;
    printf("%-10s%4d ",pVar,lLen);
    switch(nType) {
        case LT_TYPE_STRING:
            printf("STRING  [%s]\n",pValue);
           break;
        case LT_TYPE_SHORT:
            memcpy(&nValue,pValue,sizeof(short));
            printf(" SHORT  [%d]\n",nValue);
            break;
        case LT_TYPE_LONG:
            memcpy(&lValue,pValue,sizeof(long));
            printf(" LONG   [%ld]\n",lValue);
            break;
        default:
            printf("UNKNOW  nType==%d \n",nType);
            printf("\n");
            break;
    }
    return 0;
}


int ltMsgGetSomeVar(ltMsgHead *psMsgHead,int iNumVar,...)
{
    register int i,iReadCount;
    short nType;
    short *pnValue;
    long  *plValue,*lLen;
    char  *pVar,*pValue,*p;
    unsigned long  lBytes;
    va_list ap;
    va_start(ap,iNumVar);
    iReadCount=0;

    for(i=0;i<iNumVar;i++) {
       pVar = va_arg(ap,char *);
        nType = va_arg(ap,int);
         switch (nType) {
            case LT_TYPE_SHORT:
                pnValue = va_arg(ap, short *);
                *pnValue = ltMsgGetVar_n(psMsgHead,pVar);
                iReadCount++;
                break;
            case LT_TYPE_LONG:
                plValue = va_arg(ap, long *);
                *plValue = ltMsgGetVar_l(psMsgHead,pVar);
                iReadCount++;
                break;
            case LT_TYPE_STRING:
                pValue = va_arg(ap, char *);
                p = ltMsgGetVar_s(psMsgHead,pVar);
                if(p) {
                    strcpy(pValue,p);
                    iReadCount++;
                }
                else {
                    strcpy(pValue,"\0");
                }
                break;
            case LT_TYPE_STRUCT:
               pValue = va_arg(ap, char *);
               lLen = va_arg(ap, long *);
               p = ltMsgGetVar_v(psMsgHead,pVar,&lBytes);
               if(p) {
                 memcpy(pValue,p,lBytes);
                 *lLen = lBytes;
                 iReadCount++;
               }
               else {
                 *lLen = 0;
               }
               break;            
            default:

                return (-1);
        }
    }
    va_end(ap);

    return iReadCount;
}
                    

int ltMsgGetSomeNVar(ltMsgHead *psMsgHead,int iNumVar,...)
{
    register int i,iReadCount;
    short nType;
    short *pnValue;
    long  *plValue;
    long  lBytes;
    int iLen;
    char  *pVar,*pValue,*p;
    va_list ap;
    va_start(ap,iNumVar);
    iReadCount=0;

    for(i=0;i<iNumVar;i++) {
       pVar = va_arg(ap,char *);
       nType = va_arg(ap,int);
       iLen = va_arg(ap,int);
         switch (nType) {
            case LT_TYPE_SHORT:
                pnValue = va_arg(ap, short *);
                *pnValue = ltMsgGetVar_n(psMsgHead,pVar);
                iReadCount++;
                break;
            case LT_TYPE_LONG:
                plValue = va_arg(ap, long *);
                *plValue = ltMsgGetVar_l(psMsgHead,pVar);
                
                    iReadCount++;
                break;
            case LT_TYPE_STRING:
                pValue = va_arg(ap, char *);
                p = ltMsgGetVar_s(psMsgHead,pVar);
                if(p) {
                    if(iLen > 0) {
                        strncpy(pValue,p,iLen);
                        pValue[iLen]=0;
                    }
                    else
                        strcpy(pValue,p);
                    iReadCount++;
                }
                else {
                    strcpy(pValue,"\0");
                }
                break;
            case LT_TYPE_STRUCT:
               pValue = va_arg(ap, char *);
               p = ltMsgGetVar_v(psMsgHead,pVar,(unsigned long *)&lBytes);
               if(p) {
                 if(iLen < lBytes) lBytes = iLen;
                 memcpy(pValue,p,lBytes);
                 iReadCount++;
               }
               else
                 memset(pValue,0,iLen);
               break;            
            default:
               return (-1);
        }
    }
    va_end(ap);
    return iReadCount;
}

/* 从消息数据包中取指定的变量
           psMsgHead --- 消息句柄
           iNumVar   --- 变量的数量
              变量列表:
              char *pVar   --- 变量名
              int  iType  --- 变量类型
              long *lBytes --- 值的字节数 Inplt==要求输出的最大字节数,
                                 对于数字型,输入的值不起作用
                                          Oltplt==实际输出的字节数, 
                                          -1 表示变量不存在
                    value  --- 变量的值,根据具体的变量类型确定
 */
int ltMsgGetRecord(ltMsgHead *psMsgHead,int iNumVar,...)
{
    register int j,i,iReadCount;
    int   nType;
    short *pnValue;
    long  *plValue;
    long  *lBytes;
    long  lLen;
    char  *pVar,*pValue,*p;
    va_list ap;
    va_start(ap,iNumVar);
    iReadCount=0;

    for(i=0;i<iNumVar;i++) {
       pVar = va_arg(ap,char *);
       nType = va_arg(ap,int);
       lBytes = va_arg(ap,long *);
       switch (nType) {
            case LT_TYPE_SHORT:
                pnValue = va_arg(ap, short *);
                *pnValue = ltMsgGetVar_n(psMsgHead,pVar);
                
                    if(lBytes) *lBytes = sizeof(short);
                    iReadCount++;
                
                break;
            case LT_TYPE_LONG:
                plValue = va_arg(ap, long *);
                *plValue = ltMsgGetVar_l(psMsgHead,pVar);
                
                    if(lBytes) *lBytes = sizeof(long);
                    iReadCount++;
                
                break;
            case LT_TYPE_STRING:
                pValue = va_arg(ap, char *);
                p = ltMsgGetVar_s(psMsgHead,pVar);
                if(p) { 
                    if(lBytes) {
                        j=((unsigned)(*lBytes)>strlen(p))?strlen(p):*lBytes;
                        strncpy(pValue,p,j);
						pValue[j]='\0';
						*lBytes=j;
                    }
                    else
                       strcpy(pValue,p);
                    iReadCount++;
                }
                else {
                    if(lBytes) *lBytes = (-1);
                }
                break;
            case LT_TYPE_STRUCT:
               pValue = va_arg(ap, char *);
               p = ltMsgGetVar_v(psMsgHead,pVar,(unsigned long *)&lLen);
               if(p) {
                 if( *lBytes < lLen) lLen = *lBytes;
                 memcpy(pValue,p,lLen);
                 iReadCount++;
               }
               else {
                 *lBytes = (-1);
               }
               break;            
            default:
                return (-1);
        }
    }
    return iReadCount;
}




int ltMsgAddSomeVar(ltMsgHead **psMsgHead,int iNumVar,...)
{
    register int i;
    int   nType;
    short nValue;
    long  lValue,lLen;
    char  *pVar,*pValue;
    int iReturn;
    va_list ap;
    va_start(ap,iNumVar);
    for(i=0;i<iNumVar;i++) {
       pVar = va_arg(ap,char *);
        nType = va_arg(ap,int);
         switch (nType) {
            case LT_TYPE_SHORT:
                 nValue = va_arg(ap, int );
                 iReturn = ltMsgAdd_n(psMsgHead,pVar,nValue);
                 if(iReturn == (-1)) return (-1);
                break;
            case LT_TYPE_LONG:
                lValue = va_arg(ap, long);
                iReturn = ltMsgAdd_l(psMsgHead,pVar,lValue);
                if(iReturn == (-1)) return (-1);
                break;
            case LT_TYPE_STRING:
                pValue = va_arg(ap, char *);
                iReturn = ltMsgAdd_s(psMsgHead,pVar,pValue);
                if(iReturn == (-1)) return (-1);
                break;
            case LT_TYPE_STRUCT:
               pValue = va_arg(ap, char *);
               lLen = va_arg(ap, long);
               iReturn = ltMsgAdd_v(psMsgHead,pVar,pValue,
                            lLen);
               if(iReturn == (-1)) return (-1);
               break;            
            default:
                   return (-1);
        }
    }
    return 0;
}
          


static int ltMsgFPrint(char *pHead,char *pVar,short nType,long lLen,char *pValue)
{
    FILE *fp;
    short    nValue;
    long     lValue;
    fp = (FILE *)pHead;
    fprintf(fp,"    %-10s%4d",pVar,lLen);
    switch(nType) {
        case LT_TYPE_STRING:
            fprintf(fp,"STRING  [%s]\n",pValue);
           break;
        case LT_TYPE_SHORT:
            memcpy(&nValue,pValue,sizeof(short));
            fprintf(fp," SHORT  [%d]\n",nValue);
            break;
        case LT_TYPE_LONG:
            memcpy(&lValue,pValue,sizeof(long));
            fprintf(fp," LONG   [%ld]\n",lValue);
            break;
        case LT_TYPE_STRUCT:
            fprintf(fp," STRUCT \n");
            break;
        default:
            fprintf(fp,"UNKNOW  nType==%d \n",nType);
            break;
    }
    return 0;
}

