#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ltmsg.h"

#define FUN_KEY         "LT"


/* 将 psMsgHead加入校验数据  */
int ltMsgSetCheckSum(ltMsgHead *psMsgHead,char *pKey)
{
    
    psMsgHead->lCheckSum = ltMd5Code( (char *)psMsgHead + sizeof(ltMsgHead),
                        psMsgHead->lBytes - sizeof(ltMsgHead),pKey);
    /*ltSysLog("Set Msg pKey==[%s] lCheck==[%d]\n",pKey,psMsgHead->lCheckSum);*/
    return 0;
}



ltMsgHead *ltMsgInitN(unsigned long  lCode,
                     char *pFunName,
                     unsigned long  lMsgId,
                     unsigned long  lMaxBytes)
{
    unsigned long lFunCode;
    
    lFunCode =  ltMd5Code(pFunName,strlen(pFunName),FUN_KEY);

    return ltMsgInit(lCode,lFunCode,lMsgId,lMaxBytes);
}

                     
/* 检查数据包的CheckSum
    返回: 0---不正确
          1---正确
 */
int ltMsgCheckSum(ltMsgHead *psMsgHead,char *pKey)
{
    unsigned long lCheckSum;
    lCheckSum = ltMd5Code( (char *)psMsgHead + sizeof(ltMsgHead),
                        psMsgHead->lBytes - sizeof(ltMsgHead),pKey);

    if(lCheckSum == psMsgHead->lCheckSum) {
	//printf(" OK \n");
        return 1;
    }
    else {
	//printf(" Error \n");
        return 0;
    }                        
    return 0;
}
    

/* 加密数据包

 */

int ltMsgDesEncrypt(ltMsgHead **psMsgHead0,char *pKey)
{
    char *p;
    unsigned long lBytes;
    ltMsgHead *psMsgHead;
    psMsgHead = *psMsgHead0;
    p = malloc(psMsgHead->lBytes + 8);
    if(p == NULL) {
        return (-1);
    }
    lBytes =  ltDesEncrypt( (char *)psMsgHead + sizeof(ltMsgHead),
                   psMsgHead->lBytes, p, pKey);
    if(lBytes > psMsgHead->lMaxBytes - sizeof(ltMsgHead)) {
        psMsgHead = (ltMsgHead *)realloc(psMsgHead,lBytes + sizeof(ltMsgHead));
        if(psMsgHead == NULL) {
            return (-1);
        }
        else {
            psMsgHead->lMaxBytes = lBytes + sizeof(ltMsgHead);
        }
    }
    memcpy((char *)psMsgHead+sizeof(ltMsgHead),p,lBytes);
    psMsgHead->lBytes = lBytes;
    psMsgHead->cCrypt = LT_CRYPT_DES;
    *psMsgHead0 = psMsgHead;
    free(p);
    return 0;
}


/* 解密  */
int ltMsgDesDecrypt(ltMsgHead **psMsgHead0,char *pKey)
{
    char *p;
    unsigned lBytes;
    ltMsgHead *psMsgHead;
    psMsgHead = *psMsgHead0;
    if(psMsgHead->cCrypt > 0) {
        p = malloc(psMsgHead->lBytes);
        if(p == NULL) {
            return (-1);
        }
        lBytes =  ltDesDecrypt( (char *)psMsgHead + sizeof(ltMsgHead),
                   psMsgHead->lBytes, p, pKey);
        if(lBytes > psMsgHead->lMaxBytes - sizeof(ltMsgHead)) {
            psMsgHead = (ltMsgHead *)realloc(psMsgHead,lBytes + sizeof(ltMsgHead));
            if(psMsgHead == NULL) {
                return (-1);
            }
            else {
                psMsgHead->lMaxBytes = lBytes + sizeof(ltMsgHead);
            }
        }
        memcpy((char *)psMsgHead+sizeof(ltMsgHead),p,lBytes);
        psMsgHead->lBytes = lBytes;
        psMsgHead->cCrypt = 0;
        *psMsgHead0 = psMsgHead;
        free(p);
    }
    return 0;
}


int ltMsgSEncrypt(ltMsgHead *psMsgHead)
{
    char *p;
    int  i;
    p=  (char *)psMsgHead;
    p = p+sizeof(ltMsgHead);
    for(i=0;i<(psMsgHead->lBytes-sizeof(ltMsgHead));i++){
    	p[i]=0xff-p[i];
    }
    psMsgHead->cCrypt = LT_CRYPT_S;
    return 0;
}


/* 解密  */
int ltMsgSDecrypt(ltMsgHead *psMsgHead)
{
    char *p;
    int  i;
    if(psMsgHead->cCrypt > 0) {
    	  p=  (char *)psMsgHead;
		    p = p+sizeof(ltMsgHead);
		    for(i=0;i<(psMsgHead->lBytes-sizeof(ltMsgHead));i++){
		    	p[i]=0xff-p[i];
		    }
		    psMsgHead->cCrypt = 0;
    }
    return 0;
}
