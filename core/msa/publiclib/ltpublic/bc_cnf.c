#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <asm/types.h>
#include <dirent.h>
#include <stdarg.h>
#include "publiclib.h"


int ltCnfCompareKey(char *pSource,char *pKey);

int ltCnfReadConfig(char *pHand,char *caConfigFile,
               int ltCnfSaveConf(char *p1,char *pVar,char *pVal))
{
    char *p;
    char caBuffer[256];
    char caVarName[32],caOutStr[255];
    short nLineNo;
    int iReturn;
    FILE *fp;
    fp=fopen(caConfigFile,"r");
    if(fp==NULL) {
        printf(" Error, Can't open the file %s\n",caConfigFile);
        return (-1);
    }
    p=fgets(caBuffer,255,fp);
    nLineNo=1;
    while(p) {
        if(*p!='#' && (*p)!='*' && !(ltStrIsSpaces(caBuffer))) {
            p=ltStrSkipSpaces(p);
            p=ltStrGetWord(p,caVarName,32,"=");
            p=ltStrSkipSpaces(p);
            if(*p == '=') {
                if(ltStrToUpper(caVarName)==0) {
                    printf(" Error in file %s Line %d\n",
                        caConfigFile,nLineNo);
                }
                else {
                    p++;
                    p=ltStrSkipSpaces(p);
                    p=ltStrGetWord(p,caOutStr,254," \n");
                    iReturn=ltCnfSaveConf(pHand,caVarName,caOutStr);
                    if(iReturn!=0){
                        fclose(fp);
                        return iReturn;
                    }
                }
            }
            else {
                printf(" Error in Config File %s Line %d\n",
                    caConfigFile,nLineNo);
            }
        }
        nLineNo++;
        p=fgets(caBuffer,255,fp);
    }
    fclose(fp);
    return 0;
}



int bcCnfGetValue_l(char *pFile,char *pVarName)
{
    int iReturn;
    char *p;
    p=ltStrUpper(pVarName);
    iReturn=ltCnfReadConfig(p,pFile,ltCnfLookVar);
    if(iReturn==1) 
        return atol(p);
    else
        return (-1);

}

char *bcCnfGetValue_s(char *pFile,char *pVarName,char *pValue)
{
    int iReturn;
    char *p;
    pValue[0]='\0';
    p=ltStrUpper(pVarName);
    iReturn=ltCnfReadConfig(p,pFile,ltCnfLookVar);
    if(iReturn) {
    	  sprintf(pValue,"%s",p);
        return p;
    }
    return NULL;
}



int bcCnfGetValueL(char *confdir,char *path,char *pVarName)
{
    int iReturn;
    char *p;
    char pFile[256];
    sprintf(pFile,"%s/%s/%s",confdir,path,pVarName);
    p=ltStrUpper(pVarName);
    iReturn=ltCnfReadConfig(p,pFile,ltCnfLookVar);
    if(iReturn==1)
        return atol(p);
    else
        return (-1);

}


char *bcCnfGetValueS(char *confdir,char *path,char *pVarName)
{
    int iReturn;
    char *p;
    char pFile[256];
    sprintf(pFile,"%s/%s/%s",confdir,path,pVarName);
    p=ltStrUpper(pVarName);
    iReturn=ltCnfReadConfig(p,pFile,ltCnfLookVar);
    if(iReturn) {
        return p;
    }
    return NULL;
}


int ltCnfLookVar(char *pHand,char *pVarName,char *pVarValue)
{
    if(strcmp(pVarName,pHand)==0) {
        strcpy(pHand,pVarValue);
        return 1;
    }
    else
        return 0;
}

int ltCnfCompareKey(char *pSource,char *pKey){
    return strcmp(pSource,pKey);
}
 


int bcProcGetValue_s(char *pFile,char *pVarName,char *pVarValue)
{
    int iReturn;
    char caBuffer[256];
    char *p;
    
    FILE *fp;
    fp=fopen(pFile,"r");
    if(fp==NULL) {
        printf(" Error, Can't open the file %s\n",pFile);
        return (-1);
    }
    p=fgets(caBuffer,256,fp);
    while(p) {
        if(*p!='#' && (*p)!='*' && !(ltStrIsSpaces(caBuffer))) {
            p=ltStrSkipSpaces(p);
            iReturn=strncmp(p,pVarName,strlen(pVarName));
            if(iReturn==0){
	            	p=strstr(p,":");
	            	if(p){
	            		p=p+1;
	            		p=ltStrSkipSpaces(p);
	            		sprintf(pVarValue,"%s",p);
	            		fclose(fp);
	            		return 1;
	            	}
            }
          }
          p=fgets(caBuffer,255,fp);
     }
     fclose(fp);
   
    return 0;
}

