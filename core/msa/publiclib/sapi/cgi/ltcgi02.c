/*  Cgi 操作有关的实用程序 
    For Upload
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "publiclib.h"
#include "ltmsg.h"
#include "ltcomm.h"
#include "ltcgi.h"
#include "libxml/tree.h"
#include "libxml/parser.h"
#include "ltplt.h"

#define LF 10
#define CR 13


/* 文件上载 


 */

ltCgiEntryU *ltCgiUpLoad(FILE *fp,char *pTempPath,int *iSumVar,int *err)
{
    FILE *fpo;
    ltCgiEntryU *psCgiEntry;
    int iReturn,j;
    int iBytes,iLen,iReadLen,iFile,iUploadFlags,iMaxVar;
    char *pBuffer;
    char caBound[256];
    char caBuffer[256];
    char caVarName[64];
    char caFileName[256];
    char caVarValue[1024];
    char caContentType[64];
    char caSaveName[256];
    char caHead[512];
    char caUpLoadPath[255];
    iUploadFlags = 0;
    j=0;
    *err=0;
    iMaxVar = 32;
    psCgiEntry = (ltCgiEntryU *)malloc(iMaxVar * sizeof(ltCgiEntryU));
    if(psCgiEntry == NULL) {
        return NULL;
    }    
    strcpy(caUpLoadPath,pTempPath);

    iBytes = ltCgiFgets(caBuffer,255,&iLen,fp);
    if(iBytes != 0) {
        ltCgiFreeEntriesU(psCgiEntry,iMaxVar);         
        return NULL;
    }
    /* 查找边界字符串  */
    if(strncmp(caBuffer,"--",2)!=0) {
        free(psCgiEntry);
        return NULL;
    }
    caBound[0] = '\r';
    caBound[1] = '\n';
    strcpy(caBound+2,caBuffer);
    iReturn = 0;
    while(iReturn!=EOF) {
            /* Read Head */
        iReturn = ltCgiFReadHead(fp,caHead);
        if(iReturn == 0) {
            iReturn = ltCgiAnalyLine(caHead,caVarName,caFileName,caContentType);
            if(iReturn == 1 || iUploadFlags) { /* 普通变量  */
                pBuffer = ltCgiReadUntilSP(fp,&iReadLen,caBound,strlen(caBound));
                if(pBuffer) {

                    /* 变量 uploadpath为保留变量 */
                    if(strncmp(caVarName,"Upload",6) == 0) {  /* 上载目录  */
                        strcpy(caUpLoadPath,pBuffer);
                        if(strncmp(pBuffer,"String",6) == 0) { /* 将文件内容放在字符串中  */
                           iUploadFlags = 1;
                        }
                        else if(strncmp(pBuffer,"Void",4) == 0) { /* 将文件内容放在一个指针中 */
                            iUploadFlags = 2;
                        }
                        free(pBuffer);
                    }
                    else {  /* 一般变量  */
                        if(j>= iMaxVar) {
                            psCgiEntry = 
                                realloc(psCgiEntry,sizeof(ltCgiEntryU) * (iMaxVar + 32));
                            iMaxVar = iMaxVar + 32;
                            if(psCgiEntry == NULL) {
                            	*err=-100;
                                return NULL;
                            }
                        }
                        psCgiEntry[j].name = malloc(strlen(caVarName) + 1);
                        strcpy(psCgiEntry[j].name,caVarName);
                        psCgiEntry[j].iLen = iReadLen;
                        psCgiEntry[j].val = pBuffer;
                        if(iReturn == 7) {
                            psCgiEntry[j].iType = iUploadFlags;
                        }
                        else {
                            psCgiEntry[j].iType = 0;
                        }
                        j++;
                    }
                }
                else {
                    free(psCgiEntry);
                    return NULL;
                }
            }
            else if(iReturn == 7) { /* 文件  */
                if(j>= iMaxVar) {
                    psCgiEntry = realloc(psCgiEntry,sizeof(ltCgiEntryU) * (iMaxVar + 32));
                    iMaxVar = iMaxVar + 32;
                    if(psCgiEntry == NULL) {
                        return NULL;
                    }
                }
                psCgiEntry[j].name = malloc(strlen(caVarName) + 1);
                strcpy(psCgiEntry[j].name,caVarName);
                ltCgiCvtSaveFile(caFileName,caUpLoadPath,caSaveName);
               
                if(!ltStrIsSpaces(caSaveName)) {
                    fpo = fopen(caSaveName,"w");
                }
                else fpo = NULL;
                iReturn = ltCgiReadUntilF(fp,fpo,caBound,strlen(caBound));
                if(fpo)
                   fclose(fpo);
                else {
                    strcpy(caSaveName,"\0");
                }
                if(iReturn!=1){
                	*err=-100;
                	ltCgiFreeEntriesU(psCgiEntry,j);
                	return NULL;
                }
                sprintf(caVarValue,"%s;%s;%s",caFileName,caSaveName,caContentType);
                ltCgiPlusToSpace(caVarValue);
                ltCgiUnEscapeUrl(caVarValue);
                psCgiEntry[j].val = malloc(strlen(caVarValue) + 1);
                strcpy(psCgiEntry[j].val,caVarValue);
                j++;
                iFile++;

            }
            else {
              
                ltCgiFreeEntriesU(psCgiEntry,j);
                return NULL;
            }
        }
    }
    *iSumVar = j;
    return psCgiEntry;
}

/* 释放变量链表  */
int ltCgiFreeEntriesU(ltCgiEntryU *psEntries,int iSum) 
{
    register int i;
    for(i=0;i<iSum;i++) {
        free(psEntries[i].name);
        free(psEntries[i].val);
    }
    free(psEntries);
    return 0;
}



/*
  Return: 0--正常读,未结束
          EOF--文件结束
*/

int ltCgiFgets(char *pBuffer,int iMaxLen,int *iReadLen,FILE *fp)
{
    register int i;
    int  iChar;
    i = 0;
    while(i<iMaxLen) {
        if( (iChar  = fgetc(fp)) == EOF) {  /* 文件结束  */
             *iReadLen = i;
             return EOF;
        }
        pBuffer[i] = iChar;
        if(pBuffer[i] == '\n') {
            *iReadLen = i;
            pBuffer[i] = 0;
            return 0;
        }
        else if(pBuffer[i]!= '\r') {
            i++;
        }
    }
    *iReadLen = i;
    pBuffer[i] = 0;
    return EOF;
}



/*
   Return: 0--正常读,未结束
           1--终止符
          -1--文件结束
*/

int ltCgiReadUntilF(FILE *fpi,FILE *fpo,char *pTerm,int iLen)
{
    char caBuffer[256];
    int  allsize;
    int  iChar;
    register int j,j0,k,j1;
    j0 = 0;
    k = 0;
    j = 0;
    j1 = 0;
    allsize=0;
    while(1) {
        if(j>=j1) {
            iChar = fgetc(fpi);
            allsize++;
            if(allsize>LT_CGI_MAX_FILE){
            		return -100;
            }
            if( iChar == EOF) {
                j1++;
                if(fpo)
                    fwrite(caBuffer+j0,j-j0,1,fpo);
                return EOF;
            }
            else {
                caBuffer[j1] = iChar;
                j1++;
                if(j1==250) {
                    memcpy(caBuffer,caBuffer+j0,j1-j0);
                    j1 = j1 - j0;
                    j = j - j0;
                    j0 = 0;
                }
            }
        }
        if(caBuffer[j] == pTerm[k]) {
            j++;
            k++;
            if(k==iLen) { /* 碰到结束符  */
                return 1;
            }
        }
        else {
            if(fpo){
                fputc(caBuffer[j0],fpo);
             }
             j0 ++;
            j = j0;
            k = 0;
        }
    }
}


/*
   Return: 0--正常读,未结束
           1--终止符
          -1--文件结束
*/

int ltCgiReadUntilS(FILE *fpi,char *pBuffer,int *iReadLen,char *pTerm,int iLen)
{
    char caBuffer[256];
    int  iChar;
    register int j,j0,j1,k,i;
    j0 = 0;
    k = 0;
    j = 0;
    i = 0;
    j1 = 0;
    while(1) {
        if(j>=j1) {
            if( (iChar = fgetc(fpi)) == EOF) {
                j1++;
                i = i + j1-j0;
                pBuffer[i] = 0;
                *iReadLen = i;
                return EOF;
            }
            else {
                caBuffer[j1] = iChar;
                j1++;
                if(j1==250) {
                    memcpy(caBuffer,caBuffer+j0,j1-j0);
                    j1 = j1 - j0;
                    j = j - j0;
                    j0 = 0;
                }
            }
        }
        if(caBuffer[j] == pTerm[k]) {
            j++;
            k++;
            if(k==iLen) { /* 碰到结束符  */
               *iReadLen = i;
               pBuffer[i] = 0;
               return 1;
            }
        }
        else {
            *(pBuffer+i) = caBuffer[j0];
            i++;
            j0 ++;
            j = j0;
            k = 0;
        }
    }
}



/*
   Return: 0--正常读,未结束
           1--终止符
          -1--文件结束
*/

char *ltCgiReadUntilSP(FILE *fpi,int *iReadLen,char *pTerm,int iLen)
{
    char caBuffer[256];
    char *pBuffer;
    int  iChar;
    int iMaxLen;
    register int j,j0,j1,k,i;
    j0 = 0;
    k = 0;
    j = 0;
    i = 0;
    j1 = 0;
    iMaxLen = 512;
    pBuffer = (char *)malloc(iMaxLen);
    if(pBuffer == NULL) {
      
        return NULL;
    }
    while(1) {
        if(j>=j1) {
            if( (iChar = fgetc(fpi)) == EOF) {
                j1++;
                i = i + j1-j0;
                pBuffer[i] = 0;
                *iReadLen = i;
                return pBuffer;
            }
            else {
                caBuffer[j1] = iChar;
                j1++;
                if(j1==250) {
                    memcpy(caBuffer,caBuffer+j0,j1-j0);
                    j1 = j1 - j0;
                    j = j - j0;
                    j0 = 0;
                }
            }
        }
        if(caBuffer[j] == pTerm[k]) {
            j++;
            k++;
            if(k==iLen) { /* 碰到结束符  */
               *iReadLen = i;
               pBuffer[i] = 0;
                //utSetErrCode(0, " Eof Of File ");
               return pBuffer;
            }
        }
        else {
            *(pBuffer+i) = caBuffer[j0];
            i++;
            if(i > iMaxLen - 1) {
                pBuffer = realloc(pBuffer,iMaxLen + 512);
                iMaxLen = iMaxLen + 512;
                if(pBuffer == NULL) {
                    //utSetErrCode(16809, " Memory Alloc Error ");
                    return NULL;
                }
            }
            j0 ++;
            j = j0;
            k = 0;
        }
    }
}




/* 读头的内容,直到一个空行,该函数在读的时候自动将最后一个;   */
int ltCgiFReadHead(FILE *fp,char *pBuffer)
{
    int iReadLen,iReturn;
    int iBytes=0;
    iReturn = ltCgiFgets(pBuffer,255,&iReadLen,fp);
    if(iReadLen == 0) {
       iReturn = ltCgiFgets(pBuffer,255,&iReadLen,fp);
    }
    while(iReadLen>0&&iReturn!=EOF) {
        iBytes = iBytes + iReadLen;
        pBuffer[iBytes] = ';';
        pBuffer[iBytes+1] = 0;
        iBytes++;
        iReturn = ltCgiFgets(pBuffer+iBytes,255,&iReadLen,fp);
    }        
    return iReturn;
}

/* 分析头的内容  */  
int ltCgiAnalyLine(char *pBuffer,char *caVarName,char *caFileName,char *caContentType)
{
	
	char *p;
	char caTemp1[256];
	char caTemp2[256];
	int iNameFlags = 0;
	int iFileFlags = 0;
	int iContentFlags = 0;
	
	/* 取 Content-Disposition: form-data */
	p = ltStrGetWord(pBuffer,caTemp1,255,";");
	if(*p != ';') {
	    //ltSetErrCode(16801," Statement Not Coplate [%s]",pBuffer);
	    return (-1);
	}
	ltStrToLower(caTemp1);

	if(strcmp(caTemp1,"content-disposition: form-data") == 0) {
	    p++;
        while(*p) {
            p = ltStrSkipSpaces(p);
	        p = ltStrGetWord(p,caTemp1,255,"=:");
	        if(*p == '=' || *p == ':') {
                ltStrToLower(caTemp1);
                p++;
                p = ltStrSkipSpaces(p);
                p = ltStrGetWord(p,caTemp2,255,";");
                if(strcmp(caTemp1,"name")==0) {
                    ltStrncpy(caVarName,caTemp2,9);
                    iNameFlags = 1;
                }
                else if(strcmp(caTemp1,"filename")==0) {
                    ltStrncpy(caFileName,caTemp2,255);
                    iFileFlags = 2;
                }
                else if(strcmp(caTemp1,"content-type")==0) {
                    ltStrncpy(caContentType,caTemp2,255);
                    iContentFlags = 4;
                }
                if(*p == '"') p++;
                if(*p == ';') p++;
            }
            else {
                //ltSetErrCode(16802, "Synax Error [%s]",pBuffer);
                return (-1);
            }
                
         }
         return (iNameFlags + iFileFlags + iContentFlags);
     }
    else {
        //ltSetErrCode(16803, "Synax Error [%s]",pBuffer);
        return (-1);
    }
}


int ltCgiCvtSaveFile(char *caFileName,char *pTempPath,char *caSaveName)
{
    register int i;
    int l,j;
    char caTemp[256];
    if(ltStrIsSpaces(caFileName)) {
        strcpy(caSaveName,"\0");
    }
    else {
        l = strlen(caFileName);
        for(i=l;i>0&&caFileName[i-1]!=92&&caFileName[i-1]!=47;i--);
        for(j=0;i<l;i++,j++) {
            if(caFileName[i] == ' ') {caTemp[j] = '.'; }
            else { caTemp[j] = caFileName[i]; }
        }
        caTemp[j] = 0;
        sprintf(caSaveName,"%s/%s",pTempPath,caTemp);
    }
    return 0;
}


/* Do Upload  */

int ltCgiDoUpload(ltCgiConfInfo *psCgiConf,char *curPath)
{
    ltCgiEntryU *psEntries;
    ltCgiEntry  *psCookies;
    char  *pcSetCookie,*p;
    int   totalVar,totalCookie,iServer;
    long  lFunCode;
    ltMsgHead *MsgHead,*getMsgHead;
    int i,iResult;
    int iSocket;
    int errint;
    

	 
	psEntries = ltCgiUpLoad(stdin,psCgiConf->upLoadPath,&totalVar,&errint);
	
	if(psEntries==NULL){
		if(errint==-100){
			printf(ltCgiMsgErr("<H1><b>上传的文件超过了系统的允许值</b></H1><br><br>"));
		        return 0;
		}else{
			printf(ltCgiMsgErr("<H1><b>系统错误，没有任何参数</b></H1><br><br>"));
		        return 0;
		}
	}
	
  

	pcSetCookie = (char *)getenv("HTTP_COOKIE");
	if(pcSetCookie != NULL)
	{
	    psCookies = ltCgiRetrieveCookie(pcSetCookie,&totalCookie);
	}
	else {
        totalCookie = 0;
	    psCookies = NULL;
	}
    
    
    
    MsgHead = ltMsgInit(psCgiConf->lMsgCode, 0, ltStrGetId(),1024);
    if (MsgHead == NULL ) {
        printf(ltCgiMsgErr("<H1><b>Create Msg package error!</b></H1><br><br>Memory overload!"));
        exit(0);
    }

 

    for(i=0;i < totalVar;i++)
    {

     if(strncmp(psEntries[i].name,"Fun",3) == 0)
       {    
           lFunCode = (int)ltMd5Code(psEntries[i].val,strlen(psEntries[i].val),"LT");
           MsgHead->lFunCode = lFunCode;
       }else if(psEntries[i].name[0] != 0) {
            if(psEntries[i].iType == 2) { /* 文件存放在结构中  */
                ltMsgAdd_v(&MsgHead,psEntries[i].name,psEntries[i].val,psEntries[i].iLen);
            }
            else { 
                ltMsgAdd_s(&MsgHead,psEntries[i].name,psEntries[i].val);
            }
       }
    }
    ltCgiFreeEntriesU(psEntries,totalVar);
    if(lFunCode == 0) {
       printf(ltCgiMsgErr("<H1><b>Function set error!</b></H1><br><br>"));
       free(MsgHead);
       return 0;
    }

    for (i=0;i<totalCookie;i++) {
       ltMsgAdd_s(&MsgHead,psCookies[i].name,psCookies[i].val);
    }

    ltCgiFreeEntries(psCookies,totalCookie);
    ltCgiSendEnvTo(&MsgHead);
 /*
 printf("Content-type: text/html\n\n");
	
    printf("xxxxx\n");
   
	 return 0;
*/
    //iSocket = ltCgiConnect(psCgiConf);
    iSocket = ltCgiConnect(psCgiConf);

    if(iSocket == -1) {
        printf(ltCgiMsgErr("<H1><b>系统暂时不让连接，可能在处理关键业务，30秒钟后重试!</b></H1><br><br>"));
        free(MsgHead);
        return 0;
    }
    else {

        iResult = lt_TcpMsgSend(iSocket,MsgHead);
				free(MsgHead);
        if(iResult == -1)
        {
            ltCgiMsgErr("Message Send Error!\n");
        }
        else {
        	  int readtime;
        	  readtime=0;
        	
            getMsgHead = lt_TcpMsgRead(iSocket,&errint);
            while(getMsgHead == NULL && errint !=0)
            {

               getMsgHead = lt_TcpMsgRead(iSocket,&errint);
               readtime++;
               
               sleep(4);
               if(readtime>=4){
               	break;
               }
	    			}	
            if(getMsgHead == NULL && errint !=0)
            {
               ltCgiMsgErr("<H1><b>系统没有返回数据，可能没有数据或参数不对!</b></H1><br><br>");
	    			}	
            else {
	       if(getMsgHead->msgpktype==0){/*xml*/
	               /*if ( (p = ltMsgGetVar_s(getMsgHead,"ltdbcontent"))!=NULL)
	               {
			   char *lt_page_content;
			   char *lt_head;
	   		   char *lt_charset;
			   xmlDocPtr ltDocPtr;
			   
		 	   ltDocPtr=xmlParseMemory(p,strlen(p));
	                   lt_page_content=ltPltFileParse(curPath,ltDocPtr);
		           lt_head=lt_dbget_head(ltDocPtr);
			   lt_charset=lt_dbget_charset(ltDocPtr);
			   
	  		   printf("%s",lt_head);
			  
			   printf("%s",lt_page_content);
			   xmlFreeDoc(ltDocPtr);
			   free(lt_page_content);
	     
	               }*/
		}
		else if( getMsgHead->msgpktype==1){/*html*/
			//printf("Content-type: text/html\n\n");
			//ltMsgPrintMsg(getMsgHead);
     			//return 0;
			if( (p = ltMsgGetVar_s(getMsgHead,"lthead"))!=NULL){
				 printf("%s\n",p);
			}else{
				 printf("Content-type: text/html\n");
			}
			if( (p = ltMsgGetVar_s(getMsgHead,"lthtml"))!=NULL){
				int plen;
				char *pTemp;
				int l;
				int iRead;
				plen=strlen(p);
				pTemp = malloc(plen + 1);
    				strcpy(pTemp,p);
    				pTemp[plen] = 0;
				
				l = 0;
				    while(l < plen) {
       					iRead = printf("%s",pTemp + l);
       					if(iRead == 0) iRead = 1;
       					l = l + iRead;
    				}
    				printf("\n");
    				free(pTemp);
			}
		}
		else if(getMsgHead->msgpktype==2){
			   char caFileName[128];
	                   char buf[4096];
	                   int  lSize;
    			  
	                   p = ltMsgGetVar_s(getMsgHead,"filename");
	                   if(p) {
	                       strcpy(caFileName,p);
	                   }
	                   else {
	                       strcpy(caFileName,"unknown.dat");
	                   }
	                   printf("Content-disposition: attachment; filename=\"%s\"\n",caFileName);
	                   if( (p = ltMsgGetVar_s(getMsgHead,"contenttype"))!=NULL){
			   	printf("Content-type: %s\n",p);
			   }else{
				printf("Content-type: application/octet-stream\n");
			   }
	                   printf("Content-transfer-encoding:BINARY\n\n");

	                   lSize =lt_saferead(iSocket,buf,sizeof(buf));
	                   while(lSize > 0) {
	                       fwrite(buf,lSize,1,stdout);
	                       lSize = lt_saferead(iSocket,buf,sizeof(buf));
	                   }
		}
               free(getMsgHead);
            }
        }
       close(iSocket);
     }
	
        return 0;
}
    

//
//
//ltCgiEntryU *ltCgiHttpFun(FILE *fp,char *pTempPath,int *iSumVar,int *err)
//{
//    FILE *fpo;
//    ltCgiEntryU *psCgiEntry;
//    int iReturn,j;
//    int iBytes,iLen,iReadLen,iFile,iUploadFlags,iMaxVar;
//    char *pBuffer;
//    char caBound[256];
//    char caBuffer[256];
//    char caVarName[64];
//    char caFileName[256];
//    char caVarValue[1024];
//    char caContentType[64];
//    char caSaveName[256];
//    char caHead[512];
//    char caUpLoadPath[255];
//    iUploadFlags = 0;
//    j=0;
//    *err=0;
//    iMaxVar = 32;
//    psCgiEntry = (ltCgiEntryU *)malloc(iMaxVar * sizeof(ltCgiEntryU));
//    if(psCgiEntry == NULL) {
//        return NULL;
//    }    
//    strcpy(caUpLoadPath,pTempPath);
//
//    iBytes = ltCgiFgets(caBuffer,255,&iLen,fp);
//    if(iBytes != 0) {
//        ltCgiFreeEntriesU(psCgiEntry,iMaxVar);         
//        return NULL;
//    }
//    /* 查找边界字符串  */
//    if(strncmp(caBuffer,"--",2)!=0) {
//        free(psCgiEntry);
//        return NULL;
//    }
//    caBound[0] = '\r';
//    caBound[1] = '\n';
//    strcpy(caBound+2,caBuffer);
//    iReturn = 0;
//    while(iReturn!=EOF) {
//            /* Read Head */
//        iReturn = ltCgiFReadHead(fp,caHead);
//        if(iReturn == 0) {
//            iReturn = ltCgiAnalyLine(caHead,caVarName,caFileName,caContentType);
//            if(iReturn == 1 || iReturn == 4 || iReturn == 7) { /* 普通变量  */
//                pBuffer = ltCgiReadUntilSP(fp,&iReadLen,caBound,strlen(caBound));
//                if(pBuffer) {
//                        if(j>= iMaxVar) {
//                            psCgiEntry = 
//                                realloc(psCgiEntry,sizeof(ltCgiEntryU) * (iMaxVar + 32));
//                            iMaxVar = iMaxVar + 32;
//                            if(psCgiEntry == NULL) {
//                            	*err=-100;
//                                return NULL;
//                            }
//                        }
//                        psCgiEntry[j].name = malloc(strlen(caVarName) + 1);
//                        strcpy(psCgiEntry[j].name,caVarName);
//                        psCgiEntry[j].iLen = iReadLen;
//                        psCgiEntry[j].val = pBuffer;
//                        if(iReturn == 7) {
//                            psCgiEntry[j].iType = 2;/*指针型的变量*/
//                        }
//                        else {
//                            psCgiEntry[j].iType = 0;
//                        }
//                        j++;
//                    
//                }
//                else {
//                    free(psCgiEntry);
//                    return NULL;
//                }
//            }
//            else {
//              
//                ltCgiFreeEntriesU(psCgiEntry,j);
//                return NULL;
//            }
//        }
//    }
//    *iSumVar = j;
//    return psCgiEntry;
//}
//
//
///* Do Upload  */
//
//int ltCgiDoHttpFun(ltCgiConfInfo *psCgiConf,char *curPath)
//{
//    ltCgiEntryU *psEntries;
//    ltCgiEntry  *psCookies;
//    char  *pcSetCookie,*p;
//    int   totalVar,totalCookie,iServer;
//    long  lFunCode;
//    ltMsgHead *MsgHead,*getMsgHead;
//    int i,iResult;
//    int iSocket;
//    int errint;
//    
//
//	  psEntries = ltCgiHttpFun(stdin,psCgiConf->upLoadPath,&totalVar,&errint);
//	 
//	  if(psEntries==NULL){
//		  if(errint==-100){
//		  	printf("Content-type: text/html\n\n");
//			  printf("上传的文件超过了系统的允许值\r\n");
//		    return 0;
//		  }else{
//		    printf("Content-type: text/html\n\n");
//			  printf("系统错误，没有任何参数\r\n");
//		    return 0;
//		  }
//	  }
//	  
////    MsgHead = ltMsgInit(psCgiConf->lMsgCode, 0, ltStrGetId(),1024);
////    if (MsgHead == NULL ) {
////        printf(ltCgiMsgErr("<H1><b>Create Msg package error!</b></H1><br><br>Memory overload!"));
////        exit(0);
////    }
//
// 
//
////    for(i=0;i < totalVar;i++)
////    {
////
////     if(strncmp(psEntries[i].name,"Fun",3) == 0)
////       {    
////           lFunCode = (int)ltMd5Code(psEntries[i].val,strlen(psEntries[i].val),"LT");
////           MsgHead->lFunCode = lFunCode;
////       }else if(psEntries[i].name[0] != 0) {
////            if(psEntries[i].iType == 2) { /* 文件存放在结构中  */
////                ltMsgAdd_v(&MsgHead,psEntries[i].name,psEntries[i].val,psEntries[i].iLen);
////            }
////            else { 
////                ltMsgAdd_s(&MsgHead,psEntries[i].name,psEntries[i].val);
////            }
////       }
////    }
//    MsgHead = NULL;
//    for(i=0;i < totalVar;i++)
//    {
//
//     if(strncmp(psEntries[i].name,"MSGBUF",6) == 0)
//     {    
//           
//            if(psEntries[i].iType == 2) { /* 文件存放在结构中  */
//                //ltMsgAdd_v(&MsgHead,psEntries[i].name,psEntries[i].val,psEntries[i].iLen);
//                MsgHead = ltMsgInit(100, 0, ltStrGetId(),psEntries[i].iLen);
//                if (MsgHead == NULL ) {
//                	  printf("Content-type: text/html\n\n");
//						        printf("Create Msg package error!Memory overload1!\r\n");
//						        exit(0);
//						    }
//						    memcpy((char *)MsgHead,psEntries[i].val,psEntries[i].iLen);
//						    break;
//            }
//       }
//    }
//    ltCgiFreeEntriesU(psEntries,totalVar);
//    if (MsgHead == NULL ) {
//    	              printf("Content-type: text/html\n\n");
//						        printf("Create Msg package error!Memory overload2!\r\n");
//						        exit(0);
//		}
//    if(MsgHead->lFunCode == (int)ltMd5Code("testreply",strlen("testreply"),"LT")){
//    	     
//
//								  char caFileName[128];
//						      char buf[4096];
//						      int  lSize;
//					    		getMsgHead=	MsgHead;  
//						      strcpy(caFileName,"msg.dat");
//						      printf("Content-disposition: attachment; filename=\"%s\"\n",caFileName); 
//									printf("Content-type: application/octet-stream\n");
//									printf("Content-transfer-encoding:BINARY\n\n");
//					        
//					        p=(char *)getMsgHead;
//					        lSize =getMsgHead->lBytes;
//						      while(lSize > 0) {
//						          if(lSize>512){
//						          	fwrite(p,512,1,stdout);
//						          	p=p+512;
//						          	lSize=lSize-512;
//						          }else{
//						            fwrite(p,lSize,1,stdout);
//						            break;
//						          }
//						      }
//							 
//                free(getMsgHead);    	
//    }
//    
//
//    iSocket = ltCgiConnect(psCgiConf);
// 
//     if(iSocket == -1) {
//     	  printf("Content-type: text/html\n\n");
//        printf("系统无法连接\r\n");
//        free(MsgHead);
//        return 0;
//    }
//    else {
//
//        iResult = lt_TcpMsgSend(iSocket,MsgHead);
//				free(MsgHead);
//        if(iResult == -1)
//        {
//            printf("Content-type: text/html\n\n");
//            printf("Message Send Error!\r\n");
//        }
//        else {
//        	  int readtime;
//        	  readtime=0;
//        	
//            getMsgHead = lt_TcpMsgRead(iSocket,&errint);
//            while(getMsgHead == NULL && errint !=0)
//            {
//
//               getMsgHead = lt_TcpMsgRead(iSocket,&errint);
//               readtime++;
//               
//               sleep(4);
//               if(readtime>=4){
//               	break;
//               }
//	    			}	
//            if(getMsgHead == NULL && errint !=0)
//            {
//               printf("Content-type: text/html\n\n");
//               printf("系统没有返回数据，可能没有数据或参数不对!\r\n");
//	    			}	
//            else {
//
//								  char caFileName[128];
//						      char buf[4096];
//						      int  lSize;
//					    			  
//						      strcpy(caFileName,"msg.dat");
//						      printf("Content-disposition: attachment; filename=\"%s\"\n",caFileName); 
//									printf("Content-type: application/octet-stream\n");
//									printf("Content-transfer-encoding:BINARY\n\n");
//					        
//					        p=(char *)getMsgHead;
//					        lSize =getMsgHead->lBytes;
//						      while(lSize > 0) {
//						          if(lSize>512){
//						          	fwrite(p,512,1,stdout);
//						          	p=p+512;
//						          	lSize=lSize-512;
//						          }else{
//						            fwrite(p,lSize,1,stdout);
//						            break;
//						          }
//						      }
//							 
//               free(getMsgHead);
//            }
//        }
//       close(iSocket);
//     }
//  	 
//        //ltMsgPrintMsg(MsgHead);
//	
//        return 0;
//}