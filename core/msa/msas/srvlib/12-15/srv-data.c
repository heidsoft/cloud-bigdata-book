#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>
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
#include <gdfontmb.h>
#include <sys/sem.h>
#include "msas.h"

int code_convert(char *from_charset,char *to_charset,char *inbuf,int inlen,char *outbuf,int outlen)
{
iconv_t cd;
int rc;
char **pin = &inbuf;
char **pout = &outbuf;

cd = iconv_open(to_charset,from_charset);
if (cd==0) return -1;
memset(outbuf,0,outlen);
if (iconv(cd,pin,&inlen,pout,&outlen)==-1) return -1;
iconv_close(cd);
return 0;
}

int ltmodstr(char *pInstr)
{

    register int i;
    for(i=0;pInstr[i]!=0;i++){
        if(pInstr[i]=='\r'  || pInstr[i]=='\n' || pInstr[i]==' ' ||   pInstr[i]=='\'' ){ 
        	pInstr[i]='_';
        }
    }
    return 0;
}

/* 网站访问查询  */
int msasRealIdList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    char strBuf[1024];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char caTmpp[1024];
    
    long lRowNum,lStartRec,lSumRec,lCount,lNum;
    int k;

    char *caName;
    char *ctype;
    char *ccode;
    char caWhere[256];
    stralloc strTemp;
    char *exportaction;
    char  caSex[6];
    char  caCodeType[64];
    char  caCuntry[48];
    
    caName=ltMsgGetVar_s(ltMsgPk,"name");
    ctype=ltMsgGetVar_s(ltMsgPk,"ctype");
    ccode=ltMsgGetVar_s(ltMsgPk,"ccode");
    exportaction=ltMsgGetVar_s(ltMsgPk,"export");
    if(!exportaction){
    	exportaction="";
    }
		memset(caWhere,0,sizeof(caWhere));
		sprintf(caWhere,"%s"," 1=1 ");
		
    if(caName && caName[0] ){
    	sprintf(caWhere,"%s and User_name like '%c%s%c'",caWhere,'%',caName,'%');
    }
    if(ctype && ctype[0] ){
    	sprintf(caWhere,"%s and Certificate_type='%s'",caWhere,ctype);
    }
    if(ccode && ccode[0]  ){
    	sprintf(caWhere,"%s and  Certificate_code = '%s'",caWhere,ccode);
    }
   
    if(strcmp(exportaction,"1")==0){/*导出到excel*/
    	 char caFileName[128];
       sprintf(caFileName,"NetIdList-%ld.csv",time(0));
	     k=0;
	     ltMsgPk->msgpktype=2;
			 lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,caFileName,"contenttype","Application/MS-DOWNLOAD");
		   caTmpp[0]='\xEF';
		   caTmpp[1]='\xBB';
		   caTmpp[2]='\xBF';
		   write(confd,caTmpp,3);
		   
		   sprintf(strBuf,"select User_name,Sex,Certificate_type,Certificate_code,Org_name,Country from RealIDList where %s ",caWhere);
		   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		   if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
				  while(tempRow!=NULL){	
    				memset(caSex,0,sizeof(caSex));
    			  memset(caCodeType,0,sizeof(caCodeType));
    			  memset(caCuntry,0,sizeof(caCuntry));
    			  if(strcmp(tempRow[1],"1")==0){
    			  		sprintf(caSex,"%s",_ltPubInfo->pubMsgStr[13].conStr);
    			  }else{
    			      sprintf(caSex,"%s",_ltPubInfo->pubMsgStr[14].conStr);
    			  }
    			  msasGetName(tempRow[2],CTYPEINDEXSTART,CTYPEINDEXEND,caCodeType);
    			  msasGetName(tempRow[5],CUNTRYINDEXSTART,CUNTRYINDEXEND,caCuntry);
    			  
			     	sprintf(caTmpp,"%s\t%s\t%s\t%s\t%s\t%s\r\n",tempRow[0],caSex,caCodeType,tempRow[3],tempRow[4],caCuntry);	
			     	write(confd,caTmpp,strlen(caTmpp));
						//printf("caTmpp:%s\n",caTmpp);
						tempRow= ltDbFetchRow(tempCursor);
						k++;
						if(k>50000){
							break;
						}
					}
		     ltDbCloseCursor(tempCursor);
		   }
       return 0;
    }
    lRowNum=100; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0; 
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
    if(ltMsgGetVar_s(ltMsgPk,"limit")){
    		lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
    }
    if(ltMsgGetVar_s(ltMsgPk,"start")){
    		lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
    }
    sprintf(strBuf,"select count(*) from RealIDList where %s ",caWhere);

		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					lCount=atol(tempRow[0]);
				}
		}
		ltDbCloseCursor(tempCursor);
   
    strTemp.s=0;
    sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);

   k=0;
   sprintf(strBuf,"select User_name,Sex,Certificate_type,Certificate_code,Org_name,Country from RealIDList where %s limit %lu offset %lu ",caWhere,lRowNum,lStartRec);

   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
   if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
		  while(tempRow!=NULL){	
		  	 memset(caSex,0,sizeof(caSex));
    		 memset(caCodeType,0,sizeof(caCodeType));
    		 memset(caCuntry,0,sizeof(caCuntry));
    		 if(strcmp(tempRow[1],"1")==0){
    			  		sprintf(caSex,"%s",_ltPubInfo->pubMsgStr[13].conStr);
    		 }else{
    			      sprintf(caSex,"%s",_ltPubInfo->pubMsgStr[14].conStr);
    		 }
    		 msasGetName(tempRow[2],CTYPEINDEXSTART,CTYPEINDEXEND,caCodeType);
    		 msasGetName(tempRow[5],CUNTRYINDEXSTART,CUNTRYINDEXEND,caCuntry);	
		  	 if(k==0){
	     	  	sprintf(caTmpp,"{\"name\":\"%s\",\"sex\":\"%s\",\"ctype\":\"%s\",\"ccode\":\"%s\",\"oname\":\"%s\",\"Country\":\"%s\",\"ctypes\":\"%s\"}",
	     	  	 tempRow[0],caSex,caCodeType,tempRow[3],tempRow[4],caCuntry,tempRow[2]);
	     	 		stralloc_cats(&strTemp,caTmpp);	
	     	 }else{
	     	  	sprintf(caTmpp,",{\"name\":\"%s\",\"sex\":\"%s\",\"ctype\":\"%s\",\"ccode\":\"%s\",\"oname\":\"%s\",\"Country\":\"%s\",\"ctypes\":\"%s\"}",
	     	  	 tempRow[0],caSex,caCodeType,tempRow[3],tempRow[4],caCuntry,tempRow[2]);
	     	 		stralloc_cats(&strTemp,caTmpp);	
	     	 }			
				tempRow= ltDbFetchRow(tempCursor);
				k++;
			}
     ltDbCloseCursor(tempCursor);
   }

	 stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
   
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
    
	 ltMsgFree(ltMsgPk);
	 return 0;
 
}

int msasNetIdList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    char strBuf[1024];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char caTmpp[1024];
    
    long lRowNum,lStartRec,lSumRec,lCount,lNum;
    int k;

    char *caName;
    char *ctype;
    char *ccode;
    char *nettype;
    char *netcode;
    char caWhere[256];
    stralloc strTemp;
    char *exportaction;
    char  caSex[6];
    char  caCodeType[64];
    char  caCuntry[48];
    char  caNetType[48];
    
    caName=ltMsgGetVar_s(ltMsgPk,"name");
    ctype=ltMsgGetVar_s(ltMsgPk,"ctype");
    ccode=ltMsgGetVar_s(ltMsgPk,"ccode");
    nettype=ltMsgGetVar_s(ltMsgPk,"nettype");
    netcode=ltMsgGetVar_s(ltMsgPk,"netcode");
    
    exportaction=ltMsgGetVar_s(ltMsgPk,"export");
    if(!exportaction){
    	exportaction="";
    }
		memset(caWhere,0,sizeof(caWhere));
		sprintf(caWhere,"%s"," 1=1 ");
		
    if(caName && caName[0] ){
    	sprintf(caWhere,"%s and User_name like '%c%s%c'",caWhere,'%',caName,'%');
    }
    if(ctype && ctype[0] ){
    	sprintf(caWhere,"%s and Certificate_type='%s'",caWhere,ctype);
    }
    if(ccode && ccode[0]  ){
    	sprintf(caWhere,"%s and  Certificate_code = '%s'",caWhere,ccode);
    }
    if(nettype && nettype[0] ){
    	sprintf(caWhere,"%s and Dummy_type='%s'",caWhere,nettype);
    }
    if(netcode && netcode[0]  ){
    	sprintf(caWhere,"%s and  Dummy_code = '%s'",caWhere,netcode);
    }
   
    if(strcmp(exportaction,"1")==0){/*导出到excel*/
    	  char caFileName[128];
//	    char caHead[1024];
//	    sprintf(caHead,"%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n",
//	    "Pragma: public",
//	    "Expires: 0",
//	    "Cache-Control: must-revalidate, post-check=0, pre-check=0",
//	    "Content-Type: application/force-download",
//	    "Content-type: application/vnd.ms-excel",
//	    "Content-Disposition:attachment;filename=export.csv"); 
       sprintf(caFileName,"RealIdList-%ld.csv",time(0));
	     k=0;
	     ltMsgPk->msgpktype=2;
			 lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,caFileName,"contenttype","Application/MS-DOWNLOAD");
		   caTmpp[0]='\xEF';
		   caTmpp[1]='\xBB';
		   caTmpp[2]='\xBF';
		   write(confd,caTmpp,3);
		   
		   sprintf(strBuf,"select User_name,Sex,Certificate_type,Certificate_code,Org_name,Country,Dummy_type,Dummy_code from NetIDBasicList where %s ",caWhere);
		   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		   if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
				  while(tempRow!=NULL){	
    				memset(caSex,0,sizeof(caSex));
    			  memset(caCodeType,0,sizeof(caCodeType));
    			  memset(caCuntry,0,sizeof(caCuntry));
    			  memset(caNetType,0,sizeof(caNetType));
    			  
    			  if(strcmp(tempRow[1],"1")==0){
    			  		sprintf(caSex,"%s",_ltPubInfo->pubMsgStr[13].conStr);
    			  }else{
    			      sprintf(caSex,"%s",_ltPubInfo->pubMsgStr[14].conStr);
    			  }
    			  msasGetName(tempRow[2],CTYPEINDEXSTART,CTYPEINDEXEND,caCodeType);
    			  msasGetName(tempRow[5],CUNTRYINDEXSTART,CUNTRYINDEXEND,caCuntry);
    			  msasGetName(tempRow[6],NETIDINDEXSTART,NETIDINDEXEND,caNetType);
    			  
			     	sprintf(caTmpp,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\r\n",tempRow[0],caSex,caCodeType,tempRow[3],tempRow[4],caCuntry,caNetType,tempRow[7]);	
			     	write(confd,caTmpp,strlen(caTmpp));
						//printf("caTmpp:%s\n",caTmpp);
						tempRow= ltDbFetchRow(tempCursor);
						k++;
						if(k>50000){
							break;
						}
					}
		     ltDbCloseCursor(tempCursor);
		   }
       return 0;
    }
    lRowNum=100; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0; 
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
    if(ltMsgGetVar_s(ltMsgPk,"limit")){
    		lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
    }
    if(ltMsgGetVar_s(ltMsgPk,"start")){
    		lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
    }
    sprintf(strBuf,"select count(*) from NetIDBasicList where %s ",caWhere);

		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					lCount=atol(tempRow[0]);
				}
		}
		ltDbCloseCursor(tempCursor);
   
    strTemp.s=0;
    sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);

   k=0;
   sprintf(strBuf,"select User_name,Sex,Certificate_type,Certificate_code,Org_name,Country,Dummy_type,Dummy_code from NetIDBasicList where %s limit %lu offset %lu ",caWhere,lRowNum,lStartRec);
   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
   if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
		  while(tempRow!=NULL){	
		  	 memset(caSex,0,sizeof(caSex));
    		 memset(caCodeType,0,sizeof(caCodeType));
    		 memset(caCuntry,0,sizeof(caCuntry));
    		 memset(caNetType,0,sizeof(caNetType));
    		 
    		 if(strcmp(tempRow[1],"1")==0){
    			  		sprintf(caSex,"%s",_ltPubInfo->pubMsgStr[13].conStr);
    		 }else{
    			      sprintf(caSex,"%s",_ltPubInfo->pubMsgStr[14].conStr);
    		 }
    		 msasGetName(tempRow[2],CTYPEINDEXSTART,CTYPEINDEXEND,caCodeType);
    		 msasGetName(tempRow[5],CUNTRYINDEXSTART,CUNTRYINDEXEND,caCuntry);	
    		 msasGetName(tempRow[6],NETIDINDEXSTART,NETIDINDEXEND,caNetType);
    		 
		  	 if(k==0){
	     	  	sprintf(caTmpp,"{\"name\":\"%s\",\"sex\":\"%s\",\"ctype\":\"%s\",\"ccode\":\"%s\",\"oname\":\"%s\",\"Country\":\"%s\",\"nettype\":\"%s\",\"netcode\":\"%s\",\"nettypes\":\"%s\",\"ctypes\":\"%s\"}",
	     	  	 tempRow[0],caSex,caCodeType,tempRow[3],tempRow[4],caCuntry,caNetType,tempRow[7],tempRow[6],tempRow[2]);
	     	 		stralloc_cats(&strTemp,caTmpp);	
	     	 }else{
	     	  	sprintf(caTmpp,",{\"name\":\"%s\",\"sex\":\"%s\",\"ctype\":\"%s\",\"ccode\":\"%s\",\"oname\":\"%s\",\"Country\":\"%s\",\"nettype\":\"%s\",\"netcode\":\"%s\",\"nettypes\":\"%s\",\"ctypes\":\"%s\"}",
	     	  	 tempRow[0],caSex,caCodeType,tempRow[3],tempRow[4],caCuntry,caNetType,tempRow[7],tempRow[6],tempRow[2]);
	     	 		stralloc_cats(&strTemp,caTmpp);	
	     	 }			
				tempRow= ltDbFetchRow(tempCursor);
				k++;
			}
     ltDbCloseCursor(tempCursor);
   }

	 stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
   
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;
 
}



int msasNetIdTrackList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    char strBuf[1024];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char caTmpp[1024];
    
    long lRowNum,lStartRec,lSumRec,lCount,lNum;
    int k;

    char *caName;
    char *ctype;
    char *ccode;
    char *nettype;
    char *netcode;
    char *logid;
    char caWhere[256];
    stralloc strTemp;
    char *exportaction;
    char  caCodeType[64];
    char  caNetType[48];
    
    caName=ltMsgGetVar_s(ltMsgPk,"name");
    ctype=ltMsgGetVar_s(ltMsgPk,"ctype");
    ccode=ltMsgGetVar_s(ltMsgPk,"ccode");
    nettype=ltMsgGetVar_s(ltMsgPk,"nettype");
    netcode=ltMsgGetVar_s(ltMsgPk,"netcode");
    logid=ltMsgGetVar_s(ltMsgPk,"logid");
    
    exportaction=ltMsgGetVar_s(ltMsgPk,"export");
    if(!exportaction){
    	exportaction="";
    }
		memset(caWhere,0,sizeof(caWhere));
		sprintf(caWhere,"%s"," 1=1 ");
		
    if(caName && caName[0] ){
    	sprintf(caWhere,"%s and User_name like '%c%s%c'",caWhere,'%',caName,'%');
    }
    if(ctype && ctype[0] ){
    	sprintf(caWhere,"%s and Certificate_type='%s'",caWhere,ctype);
    }
    if(ccode && ccode[0]  ){
    	sprintf(caWhere,"%s and  Certificate_code = '%s'",caWhere,ccode);
    }
    if(nettype && nettype[0] ){
    	sprintf(caWhere,"%s and Dummy_type='%s'",caWhere,nettype);
    }
    if(netcode && netcode[0]  ){
    	sprintf(caWhere,"%s and  Dummy_code = '%s'",caWhere,netcode);
    }
    if(logid && logid[0]  ){
    	sprintf(caWhere,"%s and  logid = '%s'",caWhere,logid);
    }
    
    
    if(strcmp(exportaction,"1")==0){/*导出到excel*/
    	  char caFileName[128];
//	    char caHead[1024];
//	    sprintf(caHead,"%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n",
//	    "Pragma: public",
//	    "Expires: 0",
//	    "Cache-Control: must-revalidate, post-check=0, pre-check=0",
//	    "Content-Type: application/force-download",
//	    "Content-type: application/vnd.ms-excel",
//	    "Content-Disposition:attachment;filename=export.csv"); 
       sprintf(caFileName,"NetIdTrack-%ld.csv",time(0));
	     k=0;
	     ltMsgPk->msgpktype=2;
			 lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,caFileName,"contenttype","Application/MS-DOWNLOAD");
		   caTmpp[0]='\xEF';
		   caTmpp[1]='\xBB';
		   caTmpp[2]='\xBF';
		   write(confd,caTmpp,3);
		   
		   sprintf(strBuf,"select logid,User_name,Certificate_type,Certificate_code,Dummy_type,Dummy_code,Login_time,Exit_time,gnmkey,sname,Net_ending_ip from NetIDTrackList where %s ",caWhere);
		   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		   if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
				  while(tempRow!=NULL){	
    			  memset(caCodeType,0,sizeof(caCodeType));
    			  memset(caNetType,0,sizeof(caNetType));

    			  msasGetName(tempRow[2],CTYPEINDEXSTART,CTYPEINDEXEND,caCodeType);
    			  msasGetName(tempRow[4],NETIDINDEXSTART,NETIDINDEXEND,caNetType);
    			  
			     	sprintf(caTmpp,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\r\n",
			     	tempRow[0],tempRow[1],caCodeType,tempRow[3],caNetType,tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10]);	
			     	write(confd,caTmpp,strlen(caTmpp));
						//printf("caTmpp:%s\n",caTmpp);
						tempRow= ltDbFetchRow(tempCursor);
						k++;
						if(k>50000){
							break;
						}
					}
		     ltDbCloseCursor(tempCursor);
		   }
       return 0;
    }
    lRowNum=100; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0; 
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
    if(ltMsgGetVar_s(ltMsgPk,"limit")){
    		lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
    }
    if(ltMsgGetVar_s(ltMsgPk,"start")){
    		lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
    }
    sprintf(strBuf,"select count(*) from NetIDTrackList where %s ",caWhere);

		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					lCount=atol(tempRow[0]);
				}
		}
		ltDbCloseCursor(tempCursor);
   
    strTemp.s=0;
    sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);

   k=0;
   sprintf(strBuf,"select a.logid,User_name,Certificate_type,Certificate_code,Dummy_type,Dummy_code,Login_time,Exit_time,a.gnmkey,sname,Net_ending_ip,service_name\
    from NetIDTrackList a,msasclientinfo b where a.gnmkey=b.gnmkey and %s order by login_time desc limit %lu offset %lu",caWhere,lRowNum,lStartRec);
   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
   if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
		  while(tempRow!=NULL){	

    		 memset(caCodeType,0,sizeof(caCodeType));
    		 memset(caNetType,0,sizeof(caNetType));

    		 msasGetName(tempRow[2],CTYPEINDEXSTART,CTYPEINDEXEND,caCodeType);
    		 
		  	 if(k==0){
	     	  	sprintf(caTmpp,"{\"logid\":\"%s\",\"gnmkey\":\"%s\",\"name\":\"%s\",\"ctype\":\"%s\",\"ccode\":\"%s\",\"nettype\":\"%s\",\"netcode\":\"%s\",\"sname\":\"%s\",\"ont\":\"%s\",\"offt\":\"%s\",\"cip\":\"%s\",\"ctypes\":\"%s\"}",
	     	  	 tempRow[0],tempRow[8],tempRow[1],caCodeType,tempRow[3],tempRow[4],tempRow[5],tempRow[11],tempRow[6],tempRow[7],tempRow[10],tempRow[2]);
	     	 		stralloc_cats(&strTemp,caTmpp);	
	     	 }else{
	     	  	sprintf(caTmpp,",{\"logid\":\"%s\",\"gnmkey\":\"%s\",\"name\":\"%s\",\"ctype\":\"%s\",\"ccode\":\"%s\",\"nettype\":\"%s\",\"netcode\":\"%s\",\"sname\":\"%s\",\"ont\":\"%s\",\"offt\":\"%s\",\"cip\":\"%s\",\"ctypes\":\"%s\"}",
	     	  	 tempRow[0],tempRow[8],tempRow[1],caCodeType,tempRow[3],tempRow[4],tempRow[5],tempRow[11],tempRow[6],tempRow[7],tempRow[10],tempRow[2]);
	     	 		stralloc_cats(&strTemp,caTmpp);	
	     	 }			
				tempRow= ltDbFetchRow(tempCursor);
				k++;
			}
     ltDbCloseCursor(tempCursor);
   }

	 stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
   
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;
 
}





int msasClientOnOff(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    char strBuf[1024];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char caTmpp[1024];
    
    long lRowNum,lStartRec,lSumRec,lCount,lNum;
    int k;

    char *caName;
    char *ctype;
    char *ccode;
    char *ontime;
    char *offtime;
    char caWhere[256];
    stralloc strTemp;
    char *exportaction;
    char  caCodeType[64];
    
    caName=ltMsgGetVar_s(ltMsgPk,"name");
    ctype=ltMsgGetVar_s(ltMsgPk,"ctype");
    ccode=ltMsgGetVar_s(ltMsgPk,"ccode");
    ontime=ltMsgGetVar_s(ltMsgPk,"Online_time");
    offtime=ltMsgGetVar_s(ltMsgPk,"Offline_time");
    
    exportaction=ltMsgGetVar_s(ltMsgPk,"export");
    if(!exportaction){
    	exportaction="";
    }
		memset(caWhere,0,sizeof(caWhere));
		sprintf(caWhere,"%s"," 1=1 ");
		
    if(caName && caName[0] ){
    	sprintf(caWhere,"%s and User_name like '%c%s%c'",caWhere,'%',caName,'%');
    }
    if(ctype && ctype[0] ){
    	sprintf(caWhere,"%s and Certificate_type='%s'",caWhere,ctype);
    }
    if(ccode && ccode[0]  ){
    	sprintf(caWhere,"%s and  Certificate_code = '%s'",caWhere,ccode);
    }
    if(ontime && ontime[0] ){
    	sprintf(caWhere,"%s and Online_time>='%s'",caWhere,ontime);
    }
    if(offtime && offtime[0]  ){
    	sprintf(caWhere,"%s and  Offline_time <= '%s'",caWhere,offtime);
    }
    

   
    if(strcmp(exportaction,"1")==0){/*导出到excel*/
    	  char caFileName[128];
//	    char caHead[1024];
//	    sprintf(caHead,"%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n",
//	    "Pragma: public",
//	    "Expires: 0",
//	    "Cache-Control: must-revalidate, post-check=0, pre-check=0",
//	    "Content-Type: application/force-download",
//	    "Content-type: application/vnd.ms-excel",
//	    "Content-Disposition:attachment;filename=export.csv"); 
       sprintf(caFileName,"NetIdTrack-%ld.csv",time(0));
	     k=0;
	     ltMsgPk->msgpktype=2;
			 lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,caFileName,"contenttype","Application/MS-DOWNLOAD");
		   caTmpp[0]='\xEF';
		   caTmpp[1]='\xBB';
		   caTmpp[2]='\xBF';
		   write(confd,caTmpp,3);
		   
		   sprintf(strBuf,"select logid,gnmkey,sname,User_name,Certificate_type,Certificate_code,Online_time,Offline_time,Net_ending_ip,Net_ending_mac,Org_name,Country from ClientList order by online_time desc where %s ",caWhere);
		   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		   if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
				  while(tempRow!=NULL){	
    			  memset(caCodeType,0,sizeof(caCodeType));

    			  msasGetName(tempRow[4],CTYPEINDEXSTART,CTYPEINDEXEND,caCodeType);
    			  
			     	sprintf(caTmpp,"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\r\n",
			     	tempRow[0],tempRow[1],tempRow[2],tempRow[3],caCodeType,tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11]);	
			     	write(confd,caTmpp,strlen(caTmpp));
						//printf("caTmpp:%s\n",caTmpp);
						tempRow= ltDbFetchRow(tempCursor);
						k++;
						if(k>50000){
							break;
						}
					}
		     ltDbCloseCursor(tempCursor);
		   }
       return 0;
    }
    lRowNum=100; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0; 
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
    if(ltMsgGetVar_s(ltMsgPk,"limit")){
    		lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
    }
    if(ltMsgGetVar_s(ltMsgPk,"start")){
    		lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
    }
    sprintf(strBuf,"select count(*) from ClientList where %s ",caWhere);

		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					lCount=atol(tempRow[0]);
				}
		}
		ltDbCloseCursor(tempCursor);
   
    strTemp.s=0;
    sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);

   k=0;
   sprintf(strBuf,"select logid,a.gnmkey,sname,User_name,Certificate_type,Certificate_code,Online_time,Offline_time,Net_ending_ip,Net_ending_mac,Org_name,Country,service_name from ClientList a,msasclientinfo b where a.gnmkey=b.gnmkey and %s order by online_time desc limit %lu offset %lu ",caWhere,lRowNum,lStartRec);

   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
   if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
		  while(tempRow!=NULL){	
    		 memset(caCodeType,0,sizeof(caCodeType));
    		 msasGetName(tempRow[4],CTYPEINDEXSTART,CTYPEINDEXEND,caCodeType);

		  	 if(k==0){
	     	  	sprintf(caTmpp,"{\"logid\":\"%s\",\"gnmkey\":\"%s\",\"sname\":\"%s\",\"name\":\"%s\",\"ctype\":\"%s\",\"ccode\":\"%s\",\"ont\":\"%s\",\"offt\":\"%s\",\"cip\":\"%s\",\"cmac\":\"%s\",\"oname\":\"%s\",\"Country\":\"%s\",\"ctypes\":\"%s\"}",
	     	  	 tempRow[0],tempRow[1],tempRow[12],tempRow[3],caCodeType,tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[4]);
	     	 		stralloc_cats(&strTemp,caTmpp);	
	     	 }else{
	     	  	sprintf(caTmpp,",{\"logid\":\"%s\",\"gnmkey\":\"%s\",\"sname\":\"%s\",\"name\":\"%s\",\"ctype\":\"%s\",\"ccode\":\"%s\",\"ont\":\"%s\",\"offt\":\"%s\",\"cip\":\"%s\",\"cmac\":\"%s\",\"oname\":\"%s\",\"Country\":\"%s\",\"ctypes\":\"%s\"}",
	     	  	 tempRow[0],tempRow[1],tempRow[12],tempRow[3],caCodeType,tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[4]);
	     	 		stralloc_cats(&strTemp,caTmpp);	
	     	 }			
				tempRow= ltDbFetchRow(tempCursor);
				k++;
			}
     ltDbCloseCursor(tempCursor);
   }

	 stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
   
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
    
	 ltMsgFree(ltMsgPk);
	 return 0;
 
}

int msasShowAccessList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   ltDbHeadPtr dbPtr;
	 ltTablePtr  tablePtr;
	 msasDir *fnames;
	 char   *gnmkey;
   char   dfile[256];
   
	 int    ilen;
	 char   caFile1[256];
	 int    len;
	 int    iReturn;
	 
	 gnmkey=ltMsgGetVar_s(ltMsgPk,"gnmkey");
	 if(!gnmkey){
	 	 dbPtr=lt_dbinit();
	   lt_db_htmlpage(dbPtr,"utf-8");
	 	 ltMsgPk->msgpktype=1;
		 lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/datacenter/AccessList.htm",dbPtr->head,0);
		 ltMsgFree(ltMsgPk);
		 return 0;
	 }
	 ilen=strlen(gnmkey);
	 if(ilen<5){
	 	 dbPtr=lt_dbinit();
	   lt_db_htmlpage(dbPtr,"utf-8");
	 	 ltMsgPk->msgpktype=1;
		 lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/datacenter/AccessList.htm",dbPtr->head,0);
		 ltMsgFree(ltMsgPk);
		 return 0;
	 }
	 ilen=gnmkey[ilen-1]-48;
	 
	 sprintf(caFile1,"%s/AccessList/%d/%s/",datacenterdir,ilen,gnmkey);
   iReturn = access(caFile1,F_OK);
   if(iReturn!=0) {
     dbPtr=lt_dbinit();
	   lt_db_htmlpage(dbPtr,"utf-8");
	 	 ltMsgPk->msgpktype=1;
		 lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/datacenter/AccessList.htm",dbPtr->head,0);
		 ltMsgFree(ltMsgPk);
		 return 0;
   }
   len=0;
	 fnames=mydirlist(caFile1,&len);
	 if(fnames){
	 	  qsort(fnames,len,sizeof(msasDir),dirtimecmp); //给文件名按字母排序
	 }
	 
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 lt_dbput_rootvars(dbPtr,1,"gnmkey",gnmkey);
	 sprintf(dfile,"%d/%s",ilen,gnmkey);
	 lt_dbput_rootvars(dbPtr,1,"dfile",dfile);
	 tablePtr=lt_dbput_table(dbPtr,"datelist");
	 while(len--) //free all allocated memory blocks
   {
   	lt_dbput_recordvars(tablePtr,1,"strdate",LT_TYPE_STRING,fnames[len].caDirName);
   }
   
   ltMsgPk->msgpktype=1;
	 lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/datacenter/AccessList.htm",dbPtr->head,0);
	 ltMsgFree(ltMsgPk);
	 if(fnames){
 		free(fnames);
   }
	 return 0;
   
}


int msasAccessList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 	
 	char   *gnmkey;
	char   *cadate;
	char   *catime;
	char   caFile1[256];
	long   lRowNum,lStartRec,lSumRec,lCount,lNum;
  int    k;
  stralloc strTemp;
  char  caTmpp[1024];
  char  caFile2[256];
  int   fd;
  int   totalnum;
  int   nownum;
  char  inbuf[8192]; 
  int   iCount;
  int   inum;
  char  seps[]="\t";
	char  *token;
  char  linebuf[2048];
  int   iIndex=0;
	int   xIndex=0;
	char  tempStr[512];
	int   ilen;
	char  *pppp;
	int   lastnum;
	int   iReturn;
	char  caTTT[512];
	char  *fileurl;
	

	fileurl=ltMsgGetVar_s(ltMsgPk,"fileurl");
	if(fileurl){
		sprintf(caFile1,"%s/%s/search.csv",datacenterdir,fileurl);
		sprintf(caFile2,"%s/%s/search.count",datacenterdir,fileurl);
	}else{
			gnmkey=ltMsgGetVar_s(ltMsgPk,"gnmkey");
			if(!gnmkey){
			 	 gnmkey="";
			}
			ilen=strlen(gnmkey);
			if(ilen<5){
		     ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");   
				 ltMsgFree(ltMsgPk);
				 return 0;
			}
			ilen=gnmkey[ilen-1]-48;
			
			cadate=ltMsgGetVar_s(ltMsgPk,"cadate");
			if(!cadate){
			 	 cadate="";
			}
			catime=ltMsgGetVar_s(ltMsgPk,"catime");
			if(!catime){
			 	 catime="";
			}
			sprintf(caFile1,"%s/AccessList/%d/%s/%s/srvlog-%s.csv",datacenterdir,ilen,gnmkey,cadate,catime);
			sprintf(caFile2,"%s/AccessList/%d/%s/%s/srvlog-%s.count",datacenterdir,ilen,gnmkey,cadate,catime);
  }
  
	iReturn = access(caFile1,F_OK);
   if(iReturn!=0) {
     ltMsgPk->msgpktype=1;
		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");   
		 ltMsgFree(ltMsgPk);
		 return 0;
  }
  
  lRowNum=100; /*每页的行数*/
  lSumRec=0;/*总行数*/
  lCount=0; 
  lStartRec=0;
  lNum=0; /*该页剩余的行数*/
  if(ltMsgGetVar_s(ltMsgPk,"limit")){
    		lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
  }
  if(ltMsgGetVar_s(ltMsgPk,"start")){
    		lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
  }
  
  totalnum=0;
  
	iReturn = access(caFile2,F_OK);
  if(iReturn!=0) {
     /*读取rowline*/
     
     fd=open(caFile1,O_RDONLY | O_NDELAY); 
		 if(fd<1){
		     ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");   
				 ltMsgFree(ltMsgPk);
				 return 0;
		 }
		 totalnum=0;
		 iCount=read(fd,inbuf,8192); 
		 while(iCount){ 
		     for(inum=0;inum<iCount;inum++){
		     		if(inbuf[inum]=='\n'){
		     		   totalnum++; 
		     		}
		     }
		     iCount=read(fd,inbuf,8192);
		 } 
		 close(fd);
  }else{
    FILE  *fp; 
    char ch; 
    char res[12]; 
    char ress[32];
  	fp=fopen(caFile2,"r");
  	ch=fgetc(fp); 
		memset(res,0,sizeof(res)); 
		while(ch!=EOF){  
				sprintf(res,"%c",ch); 
				strcat(ress,res); 
				ch=fgetc(fp);
		}  
    fclose(fp);
    totalnum=atol(ress);
  }
  	
  strTemp.s=0;
  sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",totalnum);
  stralloc_cats(&strTemp,caTmpp);
  k=0;
  
  fd=open(caFile1,O_RDONLY | O_NDELAY); 
  if(fd<1){
    		 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");   
				 ltMsgFree(ltMsgPk);
				 return 0;
  }
  nownum=0;
  memset(linebuf,0,sizeof(linebuf));
  memset(tempStr,0,sizeof(tempStr));
  pppp=linebuf;
  iCount=read(fd,inbuf,8192);   
  while(iCount){
     lastnum=0;
     for(inum=0;inum<iCount;inum++){
     		if(inbuf[inum]=='\n'){
     		   nownum++;
     		   if(nownum>=lStartRec){
     		     memcpy(pppp,&inbuf[lastnum],inum-lastnum-1);
				     xIndex++;
				     token=strtok(linebuf, seps); 
			 
 				     if(xIndex==1){
                   stralloc_cats(&strTemp,"{");  
             }else{
                	 stralloc_cats(&strTemp,",{");  
             }
//gnmkey,Service_name,User_name,Certificate_type,Certificate_code,Net_ending_ip,
//Net_ending_name,Net_ending_mac,Time,Destination_ip,Port,Service_type,
//KeyWord1,KeyWord2,KeyWord3,KeyWord3 16

		  			 while(token!=NULL){
			  	 			 iIndex++;   
			  	 			 if(iIndex==1){
			          		 sprintf(tempStr,"\"v%d\":\"%s\"",iIndex,token);
			          		 stralloc_cats(&strTemp,tempStr);				          
			        	 }else if(iIndex==4){
			        	 	   memset(caTTT,0,sizeof(caTTT));
			        	 	   msasGetName(token,CTYPEINDEXSTART,CTYPEINDEXEND,caTTT);
			          		 sprintf(tempStr,",\"v%d\":\"%s\"",iIndex,caTTT);
			          		 stralloc_cats(&strTemp,tempStr);				          
			        	 }else if(iIndex==12){
			        	 	   memset(caTTT,0,sizeof(caTTT));
			        	 	   msasGetName(token,NETIDINDEXSTART,NETIDINDEXEND,caTTT);
			          		 sprintf(tempStr,",\"v%d\":\"%s\"",iIndex,caTTT);
			          		 stralloc_cats(&strTemp,tempStr);				          
			        	 }else{
					 			     sprintf(tempStr,",\"v%d\":\"%s\"",iIndex,token);
			          		 stralloc_cats(&strTemp,tempStr);
							   }
  	             token  = strtok( NULL, seps);
              } 
              stralloc_cats(&strTemp,"}");  
              iIndex=0;
     		   }
     		   if(nownum==(lRowNum+lStartRec)){
     		   		break;
     		   }
     		   lastnum=inum+1;
     		}	
     		memset(linebuf,0,sizeof(linebuf));
     }
     if(nownum>=(lRowNum+lStartRec)){
     		   	break;
     }
     iCount=read(fd,inbuf,8192);
  } 
  close(fd);
  stralloc_cats(&strTemp,"]}");
	stralloc_0(&strTemp); 
  
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);  
	ltMsgFree(ltMsgPk);
	return 0;
    
}




int msasShowAccessSearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   ltDbHeadPtr dbPtr;
	 ltTablePtr  tablePtr;
	 ltDbCursor *tempCursor;
	 LT_DBROW   tempRow;
   char   sqlBuf[1024];
   int    nowgid;
   int    nowglevel;
   char   groupPath[512];
   char   tmpGroupName[512];
   int    iii;
   char   *pppp;
   
   memset(groupPath,0,sizeof(groupPath));
   memset(tmpGroupName,0,sizeof(tmpGroupName));
   
   nowgid=atol(ltMsgGetVar_s(ltMsgPk,"id"));
   nowglevel=0;
   sprintf(sqlBuf,"select pid,level,name from msasGroup where id=%d ",nowgid);
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow){
					nowglevel=atol(tempRow[1]);		
			}
			ltDbCloseCursor(tempCursor);  
	 }
	 if(nowglevel>=MSAS_MAX_GROUPLEVEL){
			ltMsgPk->msgpktype=1;
		  ltWebMsgErr(_ltPubInfo->pubMsgStr[7].conStr, confd,ltMsgPk);
		  ltMsgFree(ltMsgPk);
			return 0;
	 }
	
	 for(iii=nowglevel;iii>-1;iii--){
	  	sprintf(sqlBuf,"select pid,name from msasGroup where id=%d and level=%d ",nowgid,iii);
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow){
						nowgid=atol(tempRow[0]);
						sprintf(tmpGroupName,"%s",groupPath);
						sprintf(groupPath,"%s-%s",tempRow[1],tmpGroupName);		
				}
				ltDbCloseCursor(tempCursor);
			}
	 }

	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 lt_dbput_rootvars(dbPtr,1,"searcharea",groupPath);
   lt_dbput_rootvars(dbPtr,1,"group",ltMsgGetVar_s(ltMsgPk,"id"));
  
   tablePtr=lt_dbput_table(dbPtr,"srvlist");
   for(iii=NETIDINDEXSTART;iii<=NETIDINDEXEND;iii++){
   	 sprintf(groupPath,"%s",_ltPubInfo->pubMsgStr[iii].conStr);
   	 pppp=strchr(groupPath,'=');
   	 if(pppp){
   	 		*pppp='\0';
   	 		lt_dbput_recordvars(tablePtr,2,"srvid",LT_TYPE_STRING,groupPath,"srvname",LT_TYPE_STRING,pppp+1);
   	 }
   }
   
   
   ltMsgPk->msgpktype=1;
	 lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/datacenter/ipbacksearchlist.htm",dbPtr->head,0);
	 ltMsgFree(ltMsgPk);
	 return 0;
   
}



int msasIpbackSearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){

	
	char   caCmd[256];
	char   *seldate;
  char   *seltime;
  char   *searchconX;
  char   caFile1[256];
  char   caFile2[256];
  int    iReturn;
  char   cpOut[256];
  int    fd;
  //char   seps[]="\t";
  int    nownum;
  int    iIndex=0;
	int    maxv;
	char   **vv;
	char   caTempDir[256];
	int    gid;
	char   cpOut1[256];
	int    fdwrite;
	char   sDate[32];
  int    kkkk;
  int    jjjj;
  int    iiii;
	static char inbuf[4096];
  static stralloc line = {0};
	buffer ss;
  int    match;
  char   *exportaction;
  char   *searchname;
  char   *manageruser;
  char   sqlBuf[1024];
  int    nowglevel;
  ltDbCursor *tempCursor;
	LT_DBROW   tempRow;
	char   gnmkey[48];
	int    ilen;
	char   caGroupname[128];
	
  
  fdwrite=-1;
	manageruser=ltMsgGetVar_s(ltMsgPk,"manageruser");
	memset(sDate,0,sizeof(sDate));
	nasTimeFormat(sDate,time(0));
	maxv=32;
	vv=(char **)malloc(maxv*sizeof(char *));
	for(iIndex=0;iIndex<maxv;iIndex++){
		vv[iIndex]=NULL;
	}

	iIndex=0;
  memset(cpOut,0,256);
  gid=0;
  gid=atol(ltMsgGetVar_s(ltMsgPk,"gid"));
  
  
  
  seldate=ltMsgGetVar_s(ltMsgPk,"seldate");
  seltime=ltMsgGetVar_s(ltMsgPk,"seltime");
	
	//gnmkey,Service_name,User_name,Certificate_type,Certificate_code,Net_ending_ip,
//Net_ending_name,Net_ending_mac,Time,Destination_ip,Port,Service_type,
//KeyWord1,KeyWord2,KeyWord3,KeyWord3 16
	
	vv[5]=ltMsgGetVar_s(ltMsgPk,"sip");
	vv[9]=ltMsgGetVar_s(ltMsgPk,"dip");
	vv[10]=ltMsgGetVar_s(ltMsgPk,"dport");
	vv[11]=ltMsgGetVar_s(ltMsgPk,"selsrv");
  
  
    
  searchconX=ltMsgGetVar_s(ltMsgPk,"searchconX");
  if(strlen(searchconX)<1){
  				searchconX="";
  }
  exportaction=NULL;
  exportaction=ltMsgGetVar_s(ltMsgPk,"exportaction");
  
  memset(cpOut1,0,256);
  searchname=ltMsgGetVar_s(ltMsgPk,"searchname");
  if(strlen(searchname)<1){
  				searchname="";
  }
//  else{
//    if(strutf82gb(searchname,cpOut1,strlen(searchname))==0){
//		  		searchname=cpOut1;
//		}
//  }
  nowglevel=0;
  memset(caGroupname,0,sizeof(caGroupname));
  sprintf(sqlBuf,"select pid,level,name from msasGroup where id=%d ",gid);
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow){
					nowglevel=atol(tempRow[1]);	
					sprintf(caGroupname,"%s",	tempRow[2]);	
			}
			ltDbCloseCursor(tempCursor);  
	}
	
  if(exportaction==NULL){

		  sprintf(caTempDir,"%s/LocalSearch/",datacenterdir);
		  if(ltFileIsExist(caTempDir)!=1) {
		        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
		            /// 生成该报告目录错误 
		            return 0;
		        }
		  }
		  sprintf(caTempDir,"%s/LocalSearch/%s_%lu/",datacenterdir,manageruser,ltStrGetId());
		  if(ltFileIsExist(caTempDir)!=1) {
		        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
		            /// 生成该报告目录错误 
		            return 0;
		        }
		  }
		  sprintf(caFile2,"%s/info.ini",caTempDir);
		  //printf("caFile2:%s\n",caFile2);
		  fdwrite = open(caFile2, O_APPEND | O_WRONLY | O_CREAT, 0644);
			if(fdwrite == (-1)) {
				 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n",
						"lthtml",LT_TYPE_STRING,"0");		
				 ltMsgFree(ltMsgPk);
				 return 0;
			}
			sprintf(caCmd,"sname=%s\n",searchname);
			write(fdwrite,caCmd,strlen(caCmd));
			sprintf(caCmd,"sdate=%s\n",sDate);
			write(fdwrite,caCmd,strlen(caCmd));
			sprintf(caCmd,"suser=%s\n",manageruser);
			write(fdwrite,caCmd,strlen(caCmd));			
			sprintf(caCmd,"%s","searchcon=");
			write(fdwrite,caCmd,strlen(caCmd));
			
		
		  sprintf(caCmd,"%s_%s",seldate,seltime);
			write(fdwrite,caCmd,strlen(caCmd));
			
			sprintf(caCmd,"|%s:%s",_ltPubInfo->pubMsgStr[250].conStr,caGroupname);
			write(fdwrite,caCmd,strlen(caCmd));
					
			if(vv[5] ){
				if(vv[5][0] ){
					sprintf(caCmd,"|%s:%s",_ltPubInfo->pubMsgStr[251].conStr,vv[5]);
					write(fdwrite,caCmd,strlen(caCmd));
				}
			}
			if(vv[9] ){
				if(vv[9][0] ){
					sprintf(caCmd,"|%s:%s",_ltPubInfo->pubMsgStr[252].conStr,vv[9]);
					write(fdwrite,caCmd,strlen(caCmd));
				}
			}
			if(vv[10] ){
				if(vv[10][0] ){
					sprintf(caCmd,"|%s:%s",_ltPubInfo->pubMsgStr[253].conStr,vv[10]);
					write(fdwrite,caCmd,strlen(caCmd));
				}
			}
			if(vv[11] ){
				if(vv[11][0] ){
					char   caSrvName[64];
					memset(caSrvName,0,sizeof(caSrvName));
					msasGetName(vv[11],NETIDINDEXSTART,NETIDINDEXEND,caSrvName);
					sprintf(caCmd,"|%s:%s",_ltPubInfo->pubMsgStr[254].conStr,caSrvName);
					write(fdwrite,caCmd,strlen(caCmd));
				}
			}
			
				
			if(searchconX){
					if(searchconX[0] ){
						sprintf(caCmd,"|%s:%s",_ltPubInfo->pubMsgStr[255].conStr,searchconX);
						write(fdwrite,caCmd,strlen(caCmd));
					}
			}
		  close(fdwrite);
  
  }else{
  	ltMsgPk->msgpktype=2;
		lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,"search.csv","contenttype","Application/MS-DOWNLOAD");
  }
  
  if(exportaction==NULL){
						  sprintf(caFile2,"echo '1'>%s/proccess.ini",caTempDir);
						  system(caFile2);
						  sprintf(caFile2,"%s/search.csv",caTempDir);
						  //printf("caFile2:%s\n",caFile2);
						  
						  fdwrite = open(caFile2, O_APPEND | O_WRONLY | O_CREAT, 0644);
						  if(fdwrite == (-1)) {
								 ltMsgPk->msgpktype=1;
								 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n",
										"lthtml",LT_TYPE_STRING,"0");		
								 ltMsgFree(ltMsgPk);
								 return 0;
							}
	}
	nownum=0;
  sprintf(sqlBuf,"select gnmkey,Service_name from msasclientinfo where groupid%d=%d ",nowglevel,gid);
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow){
					sprintf(gnmkey,"%s",tempRow[0]);
					ilen=strlen(gnmkey);
					ilen=gnmkey[ilen-1]-48;
	 
	        sprintf(caFile1,"%s/AccessList/%d/%s/%s/srvlog-%s.csv",datacenterdir,ilen,gnmkey,seldate,seltime);	
				  //printf("caFile1:%s\n",caFile1);
					iReturn = access(caFile1,F_OK);
				  if(iReturn!=0) {
				  	tempRow= ltDbFetchRow(tempCursor);
				  	continue;
				  }
					fd=open(caFile1,O_RDONLY | O_NDELAY); 
				  if(fd<1){
				  	tempRow= ltDbFetchRow(tempCursor);
				    continue;
				  }
				  
					
					buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
					for (;;)
					{
						if(nownum>=100000){
							break;
						}
						if (getln(&ss,&line,&match,'\n') == -1) break;
						if (!match && !line.len) { break; }
						if(searchconX){
								if(searchconX[0]){
								   if(!memmem(line.s,line.len,searchconX,strlen(searchconX))){
								     	 	 	continue;
								   }
								}	
						} 
				    iiii=0;
				    iIndex=0;
				    kkkk=0;
						for(jjjj=0;jjjj<line.len;jjjj++){
							if(line.s[jjjj]=='\t'){
								 iIndex++;
								 if(vv[iIndex]){
								 	 if(vv[iIndex][0]){
								 	 	 if(!memmem(&line.s[iiii],jjjj-iiii,vv[iIndex],strlen(vv[iIndex]))){
									 	 	 	kkkk=1;
									 	 	 	break;
								 	 	 }
								 	 }	
								 }
								 jjjj++;
								 iiii=jjjj;
						 }
						}
						if(kkkk==1){
					     continue;
					  } 
					  nownum++;
					  if(exportaction==NULL){
					  	write(fdwrite,line.s,line.len);
					  }else{
					    write(confd,line.s,line.len);
					  }
				  }
          if(exportaction==NULL){ 
          	close(fd);
          }
          tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);  
	}
  if(exportaction==NULL){ 
					  close(fdwrite);
					  sprintf(caFile2,"echo '%d'>%s/search.count",nownum,caTempDir);
					  system(caFile2);
					  sprintf(caFile2,"rm -f %s/proccess.ini",caTempDir);
					  system(caFile2);
						ltMsgPk->msgpktype=1;
						lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n",
									"lthtml",LT_TYPE_STRING,"1");		
	}
  	
	ltMsgFree(ltMsgPk);
  return 0;

}

int msasNewSearchList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	

	stralloc strTemp;
  char caTmpp[2048];
  int  iii;
  int  allnum;
  char caFile1[256];

  char *caDelFile;
  int  fileproc;
  struct stat st;
  int len=0; //sum of file names 
  msasDir *fnames;
  char   *manageruser;
  int    jjj;
  char   sname[256];
  char   sDate[256];
  char   suser[256];
  char   stype[256];
  char   searchcon[256];
  
	
	manageruser=ltMsgGetVar_s(ltMsgPk,"manageruser");

  caDelFile=ltMsgGetVar_s(ltMsgPk,"delfile");
  sprintf(caFile1,"%s/LocalSearch/",datacenterdir);
  
	if(caDelFile){ 
	       char caCmd[256];
	       if(strlen(caDelFile)>1){
	           sprintf(caCmd,"rm -Rf %s/LocalSearch/%s",datacenterdir,caDelFile);
	           system(caCmd);
	       }
	       sleep(1);
	}

 fnames=mydirlist(caFile1,&len);
 if(fnames){
        qsort(fnames,len,sizeof(msasDir),dirtimecmp); //给文件名按字母排序
 }

 allnum=0;
 
 strTemp.s=0;
 sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",len);
 stralloc_cats(&strTemp,caTmpp);
 jjj=0;
 while(len--) //free all allocated memory blocks
 { 
              
              iii=strlen(manageruser);
						 	if(strncmp(manageruser,fnames[len].caDirName,iii)){
						 		continue;
						  }
              jjj++;
//              if(jjj>allnum){
//              	break;
//              }
              memset(sname,0,sizeof(sname));
              memset(sDate,0,sizeof(sDate));
              memset(suser,0,sizeof(suser));
              memset(stype,0,sizeof(stype));
              memset(searchcon,0,sizeof(searchcon));
							fileproc=0;
							sprintf(caFile1,"%s/LocalSearch/%s/proccess.ini",datacenterdir,fnames[len].caDirName);
							
              if(stat(caFile1,&st) == 0){ 
                    fileproc=1; 
              }
              sprintf(caFile1,"%s/LocalSearch/%s/info.ini",datacenterdir,fnames[len].caDirName);
              
              bcCnfGetValue_s(caFile1,"sname",sname);
              bcCnfGetValue_s(caFile1,"sdate",sDate);
              bcCnfGetValue_s(caFile1,"suser",suser);
              bcCnfGetValue_s(caFile1,"stype",stype);
              bcCnfGetValue_s(caFile1,"searchcon",searchcon);
              memset(caTmpp,0,sizeof(caTmpp));
              
							if(jjj==1){
								sprintf(caTmpp,"{\"sname\":\"%s\",\"sdate\":\"%s\",\"suser\":\"%s\",\"scon\":\"%s\",\"myurl\":\"%s\",\"stype\":\"%s\",\"fproc\":\"%s\"}",
									sname,sDate,suser,searchcon,fnames[len].caDirName,stype,fileproc==1?_ltPubInfo->pubMsgStr[258].conStr:_ltPubInfo->pubMsgStr[259].conStr);
								//printf("caTmpp:%s\n",caTmpp);   
				        stralloc_cats(&strTemp,caTmpp);			
              }else{
                sprintf(caTmpp,",{\"sname\":\"%s\",\"sdate\":\"%s\",\"suser\":\"%s\",\"scon\":\"%s\",\"myurl\":\"%s\",\"stype\":\"%s\",\"fproc\":\"%s\"}",
									sname,sDate,suser,searchcon,fnames[len].caDirName,stype,fileproc==1?_ltPubInfo->pubMsgStr[258].conStr:_ltPubInfo->pubMsgStr[259].conStr);
				        stralloc_cats(&strTemp,caTmpp);				
              }  
                  
 }
 stralloc_cats(&strTemp,"]}");
 stralloc_0(&strTemp); 
 ltMsgPk->msgpktype=1;
 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);  
 ltMsgFree(ltMsgPk);
 return 0;
 
}


int msasSearchTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char tmpstr[512];
	stralloc strTemp;
  msasDir *fnames;
  char   caFile1[256];
	int    iHead;
	int    len;
	char   sDate[32];
	char   suser[32];
	char   sname[32];


	memset(tmpstr,0,sizeof(tmpstr));

//	sprintf(caFile1,"%s/LocalSearch/",datacenterdir);
//  len=0;
//	fnames=mydirlist(caFile1,&len);
//	if(fnames){
//	 	  qsort(fnames,len,sizeof(msasDir),dirtimecmp); //给文件名按字母排序
//	}
//	iHead=1;
//	strTemp.s=0;
//
//	sprintf(tmpstr,"[{text:'%s',expanded: true,cls:'feeds-node',children:[",_ltPubInfo->pubMsgStr[248].conStr);
//	stralloc_cats(&strTemp,tmpstr);
//	while(len--) //free all allocated memory blocks
//  {
//     	memset(sDate,0,sizeof(sDate));
//     	memset(suser,0,sizeof(suser));
//     	memset(sname,0,sizeof(sname));
//     	sprintf(caFile1,"%s/LocalSearch/%s/info.ini",datacenterdir,fnames[len].caDirName);
//      bcCnfGetValue_s(caFile1,"sdate",sDate);
//      bcCnfGetValue_s(caFile1,"suser",suser);
//      bcCnfGetValue_s(caFile1,"sname",sname);
//   	  if(iHead==1){
//				sprintf(tmpstr,"{id: 'L%s',qtip: '%s',text: '%s',leaf:true}",fnames[len].caDirName,sname,sname);
//				iHead=0;
//			}else{
//			  sprintf(tmpstr,",{id: 'L%s',qtip: '%s',text: '%s',leaf:true}",fnames[len].caDirName,sname,sname);
//		  }
//		  stralloc_cats(&strTemp,tmpstr);
//  }
//  stralloc_cats(&strTemp,"]}");
//  if(fnames){
// 		free(fnames);
//  }
// 
  
  sprintf(caFile1,"%s/SearchCMD/",datacenterdir);
  len=0;
	fnames=mydirlist(caFile1,&len);
	if(fnames){
	 	  qsort(fnames,len,sizeof(msasDir),dirtimecmp); //给文件名按字母排序
	}
	iHead=1;	
	sprintf(tmpstr,"[{text:'%s',expanded: true,cls:'feeds-node',children:[",_ltPubInfo->pubMsgStr[249].conStr);
	stralloc_cats(&strTemp,tmpstr);
	while(len--) //free all allocated memory blocks
  {
     	memset(sDate,0,sizeof(sDate));
     	memset(suser,0,sizeof(suser));
     	memset(sname,0,sizeof(sname));
     	sprintf(caFile1,"%s/SearchCMD/%s/info.ini",datacenterdir,fnames[len].caDirName);
      bcCnfGetValue_s(caFile1,"sdate",sDate);
      bcCnfGetValue_s(caFile1,"suser",suser);
      bcCnfGetValue_s(caFile1,"sname",sname);
   	  if(iHead==1){
				sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s',leaf:true}",fnames[len].caDirName,sname,sname);
				iHead=0;
			}else{
			  sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s',leaf:true}",fnames[len].caDirName,sname,sname);
		  }
		  stralloc_cats(&strTemp,tmpstr);
  }
  stralloc_cats(&strTemp,"]}]");
  stralloc_0(&strTemp);
  if(fnames){
 		free(fnames);
  }

	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
    
	ltMsgFree(ltMsgPk);
	return 0;

}


/* 高级查询---显示查询Form  */
int msasAccesssCmdSet(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  
  char sqlBuf[1024];
	char cmdno[32];
	char cmdpath[256];
	char cmdtime[32];
	ltDbConn *G_DbConTemp;
  ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	char *dbUser;
	char *dbPass;
  char *dbName; 
	char  caCmd[256];

	char   *caDate;
  char   *caTime;
  char   *caCon1;
  char   *caCon2;
  char   *searchflag;
  char   *searchand;
  char   *searchconX;
  char   *toutf;
  char   *searchname;
 
  int    nowglevel;
	int    gid;
	char   *manageruser;
	char   caTempDir[256];
	char   caFile2[256];
	int    fdwrite;
	int    jjj;

	gid=atol(ltMsgGetVar_s(ltMsgPk,"gid"));
	manageruser=ltMsgGetVar_s(ltMsgPk,"manageruser");
	
  toutf=ltMsgGetVar_s(ltMsgPk,"toutf");
  if(!toutf){
  	toutf="0";
  }
  searchname=ltMsgGetVar_s(ltMsgPk,"searchname");
  caDate=ltMsgGetVar_s(ltMsgPk,"sdate");
  caTime=ltMsgGetVar_s(ltMsgPk,"stime");
  searchflag=ltMsgGetVar_s(ltMsgPk,"searchflag");
  caCon1=ltMsgGetVar_s(ltMsgPk,"searchcon");
  caCon2=ltMsgGetVar_s(ltMsgPk,"searchcon1");
  if(strlen(caCon2)<1){
  	caCon2="!";
  }
  searchand=ltMsgGetVar_s(ltMsgPk,"searchand");
  searchconX=ltMsgGetVar_s(ltMsgPk,"searchconX");
  if(strlen(searchconX)<1){
  	searchconX="!";
  }
  
  memset(cmdtime,0,sizeof(cmdtime));
	nasTimeLongFormat(cmdtime,time(0));

	sprintf(cmdpath,"%ld%d",time(0),getpid());
	
  sprintf(caTempDir,"%s/SearchCMD/",datacenterdir);
	if(ltFileIsExist(caTempDir)!=1) {
	      if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
	          /// 生成该报告目录错误 
	          return 0;
	      }
	}
	sprintf(caTempDir,"%s/SearchCMD/%s",datacenterdir,cmdpath);	
	if(ltFileIsExist(caTempDir)!=1) {
	      if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
	          /// 生成该报告目录错误 
	          return 0;
	      }
	}
	sprintf(caFile2,"%s/info.ini",caTempDir);
	fdwrite = open(caFile2, O_APPEND | O_WRONLY | O_CREAT, 0644);
	if(fdwrite == (-1)) {
		 ltMsgPk->msgpktype=1;
		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
		 ltMsgFree(ltMsgPk);
		 return 0;
	}
	
	sprintf(caCmd,"gid=%d\n",gid);
	write(fdwrite,caCmd,strlen(caCmd));
	sprintf(caCmd,"sname=%s\n",searchname);
	write(fdwrite,caCmd,strlen(caCmd));
	sprintf(caCmd,"cdate=%s\n",cmdtime);
	write(fdwrite,caCmd,strlen(caCmd));
	sprintf(caCmd,"cuser=%s\n",manageruser);
	write(fdwrite,caCmd,strlen(caCmd));			
  
  sprintf(caCmd,"toutf=%s\n",toutf);
	write(fdwrite,caCmd,strlen(caCmd));		
	sprintf(caCmd,"sdate=%s\n",caDate);
	write(fdwrite,caCmd,strlen(caCmd));		
	sprintf(caCmd,"stime=%s\n",caTime);
	write(fdwrite,caCmd,strlen(caCmd));		
	sprintf(caCmd,"searchflag=%s\n",searchflag);
	write(fdwrite,caCmd,strlen(caCmd));		
	sprintf(caCmd,"searchcon=%s\n",caCon1);
	write(fdwrite,caCmd,strlen(caCmd));		
	sprintf(caCmd,"searchcon1=%s\n",caCon2);
	write(fdwrite,caCmd,strlen(caCmd));		
	sprintf(caCmd,"searchand=%s\n",searchand);
	write(fdwrite,caCmd,strlen(caCmd));		
	sprintf(caCmd,"searchconX=%s\n",searchconX);
	write(fdwrite,caCmd,strlen(caCmd));		
	
	close(fdwrite);
	
	dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	
	G_DbConTemp=ltDbConnect(dbUser,dbPass,dbName);
	if(G_DbConTemp==NULL){
		 //printf("db connect err\n");
		 ltMsgPk->msgpktype=1;
		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");		
		 ltMsgFree(ltMsgPk);
		 return 0;
	}
	
	nowglevel=0;
  sprintf(sqlBuf,"select pid,level,name from msasGroup where id=%d ",gid);
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow){
					nowglevel=atol(tempRow[1]);	
			}
			ltDbCloseCursor(tempCursor);  
	}
  jjj=0;
  sprintf(sqlBuf,"select gnmkey,Service_name from msasclientinfo where groupid%d=%d ",nowglevel,gid);  
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow){
				  jjj++;
				  sprintf(cmdno,"%ld%d%d",time(0),getpid(),jjj);
					sprintf(sqlBuf,"insert into ClientCmd (cmdno,gnmkey,cmdpath,cmdtype,cmdtime,cmdptime,cmduploadtime,updateflag ) values ('%s','%s','%s','AccessList','%s','','','0') ",cmdno,tempRow[0],cmdpath,cmdtime);					  
			    ltDbExecSql(G_DbConTemp,sqlBuf);  
			    /*
			    update online
			    */ 
			    {
								char *p;
							  int  iii;
							  int indexWalker;
							  shm_hash_link_onlinehostindex *_gnm_hash_index;
								shm_hash_link_onlinehost      *_gnm_hash_content;
								shm_hash_link_onlinehost *walker;
								msasOnlineHost  *nasHashItemInfo;
		            static struct sembuf lock={0,-1,SEM_UNDO};
	  						static struct sembuf unlock={0,1,SEM_UNDO|IPC_NOWAIT};
	  
							  p=(char *)_ltPubInfo;
								_gnm_hash_index=(shm_hash_link_onlinehostindex *)(p+sizeof(msasPubInfo));
							  _gnm_hash_content=(shm_hash_link_onlinehost *)(p+sizeof(msasPubInfo)+_ltPubInfo->_onlinehosthead.conbufptr);
							    
							  for(iii=0;iii<_ltPubInfo->_onlinehosthead.bufnum;iii++){
							  	 semop(_ltPubInfo->_onlinehosthead.locksemid,&lock,1); 
							   	 indexWalker=_gnm_hash_index[iii].next;
							   	 if(indexWalker==-1){
							   	 	semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
							   	 	continue;
							   	 }
							   	 semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
									 walker = &_gnm_hash_content[indexWalker];
									 for (; ; ) {
												  nasHashItemInfo=(msasOnlineHost *)(&walker->item);
												  if(strcmp(nasHashItemInfo->caKey,tempRow[0])==0){
												  	nasHashItemInfo->havecmd=1;
												  }
													semop(_ltPubInfo->_onlinehosthead.locksemid,&lock,1); 
													if(walker->next == -1){
														semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
														break;
													}
													indexWalker = walker->next;
													semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
													walker = &_gnm_hash_content[indexWalker];
									 }
							  }
					}
				  tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor); 
	}
	ltDbClose(G_DbConTemp);
  
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,cmdpath);		
	ltMsgFree(ltMsgPk);
	return 0;

}


int msasYunSearchCMDList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	

	stralloc strTemp;
  char caTmpp[2048];
  ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	char   sqlBuf[1024];
  char   *caDelFile;
  int    jjj,allnum;
  char   *caYunPath;
	
	caYunPath=ltMsgGetVar_s(ltMsgPk,"YunPath");
  
  caDelFile=ltMsgGetVar_s(ltMsgPk,"delfile");
	if(caDelFile){ 
		if(strcmp(caDelFile,"all")==0){
			sprintf(sqlBuf,"delete from ClientCmd where cmdpath='%s'",caYunPath);
	    ltDbExecSql(G_DbCon,sqlBuf);
		}else{
	    sprintf(sqlBuf,"delete from ClientCmd where cmdno='%s' and cmdpath='%s'",caDelFile,caYunPath);
	    ltDbExecSql(G_DbCon,sqlBuf);
	  }
	}

 
 allnum=0;
 sprintf(sqlBuf,"select count(*) from ClientCmd where cmdpath='%s'",caYunPath);
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow){
					allnum=atol(tempRow[0]);	
			}
			ltDbCloseCursor(tempCursor);  
	}
 strTemp.s=0;
 sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allnum);
 stralloc_cats(&strTemp,caTmpp);
 jjj=0;
 
 
 sprintf(sqlBuf,"select cmdno,gnmkey,cmdpath,cmdtype,cmdtime,cmdptime,cmduploadtime,updateflag from ClientCmd where cmdpath='%s'",caYunPath);
 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
 if(tempCursor){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow){
					    jjj++;
              memset(caTmpp,0,sizeof(caTmpp));              
							if(jjj==1){
								sprintf(caTmpp,"{\"cmdno\":\"%s\",\"gnmkey\":\"%s\",\"cmdpath\":\"%s\",\"cmdtype\":\"%s\",\"cmdtime\":\"%s\",\"cmdptime\":\"%s\",\"cmduploadtime\":\"%s\",\"updateflag\":\"%s\"}",
									tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7]);
								//printf("caTmpp:%s\n",caTmpp);   
				        stralloc_cats(&strTemp,caTmpp);			
              }else{
                sprintf(caTmpp,",{\"cmdno\":\"%s\",\"gnmkey\":\"%s\",\"cmdpath\":\"%s\",\"cmdtype\":\"%s\",\"cmdtime\":\"%s\",\"cmdptime\":\"%s\",\"cmduploadtime\":\"%s\",\"updateflag\":\"%s\"}",
									tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7]);
				        stralloc_cats(&strTemp,caTmpp);				
              }  
              tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);  
 }
	

 stralloc_cats(&strTemp,"]}");
 stralloc_0(&strTemp); 
 ltMsgPk->msgpktype=1;
 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);  
 ltMsgFree(ltMsgPk);
 return 0;
 
}

/*
 /app/msas/SearchCMD/11111111/
 /app/msas/SearchCMD/11111111/info.ini search con
 /app/msas/SearchCMD/11111111/gnmkey  upload dir
 /app/msas/SearchCMD/11111111/gnmkey/info.ini  upload info
*/

int msasYunSearchList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	

	stralloc strTemp;
  char caTmpp[2048];
  int  allnum;
  char caFile1[256];

  char *caDelFile;
  int len=0; //sum of file names 
  msasDir *fnames;
  char   *manageruser;
  int    jjj;
  char   sname[256];
  char   sDate[256];
  char   skey[256];
  char   stype[256];
  char   *caYunPath;
  int    datafilesize;
	
	caYunPath=ltMsgGetVar_s(ltMsgPk,"YunPath");
	manageruser=ltMsgGetVar_s(ltMsgPk,"manageruser");
  
  caDelFile=ltMsgGetVar_s(ltMsgPk,"delfile");
  sprintf(caFile1,"%s/SearchCMD/%s",datacenterdir,caYunPath);
  
	if(caDelFile){ 
	       char caCmd[256];
	       if(strlen(caDelFile)>1){
	           sprintf(caCmd,"rm -Rf %s/SearchCMD/%s/%s",datacenterdir,caYunPath,caDelFile);
	           system(caCmd);
	       }
	       sleep(1);
	}

 fnames=mydirlist(caFile1,&len);
 if(fnames){
        qsort(fnames,len,sizeof(msasDir),dirtimecmp); //给文件名按字母排序
 }

 allnum=0;
 
 strTemp.s=0;
 sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",len);
 stralloc_cats(&strTemp,caTmpp);
 jjj=0;
 while(len--) //free all allocated memory blocks
 { 
              
              if(strcmp(fnames[len].caDirName,"info.ini")==0){
              	continue;
              }
              jjj++;
              memset(sname,0,sizeof(sname));
              memset(sDate,0,sizeof(sDate));
              memset(skey,0,sizeof(skey));
              memset(stype,0,sizeof(stype));
              sprintf(caFile1,"%s/SearchCMD/%s/%s/info.ini",datacenterdir,caYunPath,fnames[len].caDirName);
              
              bcCnfGetValue_s(caFile1,"sname",sname);
              bcCnfGetValue_s(caFile1,"sdate",sDate);
              bcCnfGetValue_s(caFile1,"skey",skey);
              bcCnfGetValue_s(caFile1,"stype",stype);
              memset(caTmpp,0,sizeof(caTmpp));
              datafilesize=0;
              sprintf(caFile1,"%s/SearchCMD/%s/%s/search.csv",datacenterdir,caYunPath,fnames[len].caDirName);
              datafilesize=ltFileSize(caFile1);
              if(datafilesize<1){
              	sprintf(caFile1,"%s/SearchCMD/%s/%s/1.json",datacenterdir,caYunPath,fnames[len].caDirName);
                datafilesize=ltFileSize(caFile1);
              }
              if(datafilesize<10){
              	datafilesize=0;
              }
              
							if(jjj==1){
								sprintf(caTmpp,"{\"sname\":\"%s\",\"sdate\":\"%s\",\"skey\":\"%s\",\"myurl\":\"%s\",\"stype\":\"%s\",\"yunpath\":\"%s\",\"datasize\":\"%d\"}",
									sname,sDate,skey,fnames[len].caDirName,stype,caYunPath,datafilesize);
								//printf("caTmpp:%s\n",caTmpp);   
				        stralloc_cats(&strTemp,caTmpp);			
              }else{
                sprintf(caTmpp,",{\"sname\":\"%s\",\"sdate\":\"%s\",\"skey\":\"%s\",\"myurl\":\"%s\",\"stype\":\"%s\",\"yunpath\":\"%s\",\"datasize\":\"%d\"}",
									sname,sDate,skey,fnames[len].caDirName,stype,caYunPath,datafilesize);
				        stralloc_cats(&strTemp,caTmpp);				
              }  
                  
 }
 stralloc_cats(&strTemp,"]}");
 stralloc_0(&strTemp); 
 ltMsgPk->msgpktype=1;
 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);  
 ltMsgFree(ltMsgPk);
 return 0;
 
}

int msasShowYunSearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
   
	 ltDbHeadPtr dbPtr;
	 ltDbCursor *tempCursor;
	 LT_DBROW   tempRow;
   char   sqlBuf[1024];
   int    nowglevel;
   char   groupPath[512];
   char   tmpGroupName[512];
   int    iii;
   char   *caYunPath;
   
   char   caFile1[256];
   char   sdate[128];
   char   searchflag[32];
   char   searchcon[128];
   char   searchconstr[256];
   char   searchand[32];
   int    gid;
   char   caGid[32];
	
	 caYunPath=ltMsgGetVar_s(ltMsgPk,"YunPath");
	 
   sprintf(caFile1,"%s/SearchCMD/%s/info.ini",datacenterdir,caYunPath);
   
   dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 
 
   lt_dbput_rootvars(dbPtr,1,"YunPath",caYunPath);
   memset(caGid,0,sizeof(caGid));
   bcCnfGetValue_s(caFile1,"gid",caGid);
   gid=atol(caGid);
   memset(sdate,0,sizeof(sdate));
   bcCnfGetValue_s(caFile1,"sdate",sdate);
   lt_dbput_rootvars(dbPtr,1,"sdate",sdate);
   memset(searchflag,0,sizeof(searchflag));
   bcCnfGetValue_s(caFile1,"searchflag",searchflag);
	 if(strcmp(searchflag,"1")==0){
	 	 lt_dbput_rootvars(dbPtr,1,"stype","IP");	
	 }else if(strcmp(searchflag,"2")==0){
	 	 lt_dbput_rootvars(dbPtr,1,"stype","HTTP");	
	 }else if(strcmp(searchflag,"3")==0){
	 	 lt_dbput_rootvars(dbPtr,1,"stype","IM");	
	 }else if(strcmp(searchflag,"4")==0){
	 	 lt_dbput_rootvars(dbPtr,1,"stype","WebForm");	
	 }else if(strcmp(searchflag,"5")==0){
	 	 lt_dbput_rootvars(dbPtr,1,"stype","Mail");	
	 }
	 
	 memset(searchcon,0,sizeof(searchcon));
	 memset(searchconstr,0,sizeof(searchconstr));
   bcCnfGetValue_s(caFile1,"searchcon",searchcon);
   sprintf(searchconstr,"%s",searchcon);
   memset(searchcon,0,sizeof(searchcon));
	 bcCnfGetValue_s(caFile1,"searchcon1",searchcon);
	 if(strlen(searchcon)>0 && strcmp(searchcon,"!")){  
	   memset(searchand,0,sizeof(searchand));
	   bcCnfGetValue_s(caFile1,"searchand",searchand);
	   if(strcmp(searchand,"1")==0){
		 	 sprintf(searchconstr,"%s or %s",searchconstr,searchcon);
		 }else if(strcmp(searchand,"2")==0){
		 	 sprintf(searchconstr,"%s and %s",searchconstr,searchcon);
		 }
	 }
	 lt_dbput_rootvars(dbPtr,1,"scon",searchconstr);	
	 
	 memset(searchcon,0,sizeof(searchcon));
	 bcCnfGetValue_s(caFile1,"searchconX",searchcon);
	 if(strcmp(searchcon,"!")){
	 	lt_dbput_rootvars(dbPtr,1,"sconx",searchcon);
	 }
   
   memset(groupPath,0,sizeof(groupPath));
   memset(tmpGroupName,0,sizeof(tmpGroupName));
   

   nowglevel=0;
   sprintf(sqlBuf,"select pid,level,name from msasGroup where id=%d ",gid);
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow){
					nowglevel=atol(tempRow[1]);		
			}
			ltDbCloseCursor(tempCursor);  
	 }
	 if(nowglevel>=MSAS_MAX_GROUPLEVEL){
			ltMsgPk->msgpktype=1;
		  ltWebMsgErr(_ltPubInfo->pubMsgStr[7].conStr, confd,ltMsgPk);
		  ltMsgFree(ltMsgPk);
			return 0;
	 }
	
	 for(iii=nowglevel;iii>-1;iii--){
	  	sprintf(sqlBuf,"select pid,name from msasGroup where id=%d and level=%d ",gid,iii);
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow){
						gid=atol(tempRow[0]);
						sprintf(tmpGroupName,"%s",groupPath);
						sprintf(groupPath,"%s-%s",tempRow[1],tmpGroupName);		
				}
				ltDbCloseCursor(tempCursor);
			}
	 }

	 lt_dbput_rootvars(dbPtr,1,"sarea",groupPath);
   
   
   ltMsgPk->msgpktype=1;
	 lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/datacenter/backsearchlist.htm",dbPtr->head,0);
	 ltMsgFree(ltMsgPk);
	 return 0;
	
 
}


int msasYunSearchDel(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
  char   caFile1[256];
	char   *id;
  
  id=ltMsgGetVar_s(ltMsgPk,"id");
  sprintf(caFile1,"rm -Rf %s/SearchCMD/%s",datacenterdir,id);
  system(caFile1);
  sprintf(caFile1,"delete from ClientCmd where cmdpath='%s'",id);
	ltDbExecSql(G_DbCon,caFile1);
	    
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"1");
    
	ltMsgFree(ltMsgPk);
	return 0;

}

int msasSearchShowList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 	

	char   caFile1[256];
	long   lRowNum,lStartRec,lSumRec,lCount,lNum;
  int    k;
  stralloc strTemp;
  char  caTmpp[1024];
  int   fd;
  int   totalnum;
  int   nownum;
  int   iCount;
  int   inum;
  //char  seps[]="\t";
  char  seps[]=",";
	char  *token;
  char  linebuf[2048];
  int   iIndex=0;
	int   xIndex=0;
	char  tempStr[2048];
	char  tempStrStr[4096];
	char  *pppp;
	char  *fileurl;
	static char inbuf[8192];
  static stralloc line = {0};
	buffer ss;
	int   match;
	

	fileurl=ltMsgGetVar_s(ltMsgPk,"fileurl");
  sprintf(caFile1,"%s/%s/search.csv",datacenterdir,fileurl);
//printf("caFile1:%s\n",caFile1);	
	
  lRowNum=100; /*每页的行数*/
  lSumRec=0;/*总行数*/
  lCount=0; 
  lStartRec=0;
  lNum=0; /*该页剩余的行数*/
  if(ltMsgGetVar_s(ltMsgPk,"limit")){
    		lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
  }
  if(ltMsgGetVar_s(ltMsgPk,"start")){
    		lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
  }
  
  totalnum=0;
  
	
     
  fd=open(caFile1,O_RDONLY | O_NDELAY); 
	if(fd<1){
	    ltMsgPk->msgpktype=1;
		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");   
		 ltMsgFree(ltMsgPk);
		 return 0;
	}
	totalnum=0;
	iCount=read(fd,inbuf,8192); 
	while(iCount){ 
	    for(inum=0;inum<iCount;inum++){
	    		if(inbuf[inum]=='\n'){
	    		   totalnum++; 
	    		}
	    }
	    iCount=read(fd,inbuf,8192);
	} 
	close(fd);  	
  strTemp.s=0;
  sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",totalnum);
  stralloc_cats(&strTemp,caTmpp);
  k=0;
  
  fd=open(caFile1,O_RDONLY | O_NDELAY); 
  if(fd<1){
    		 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");   
				 ltMsgFree(ltMsgPk);
				 return 0;
  }  
  nownum=0;
  memset(linebuf,0,sizeof(linebuf));
  memset(tempStr,0,sizeof(tempStr));
  pppp=linebuf;
  buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
	for (;;)
	{
		if (getln(&ss,&line,&match,'\n') == -1) break;
		if (!match && !line.len) { break; }
		
		while (line.len > 0){
		   if(line.s[line.len - 1]=='\n' || line.s[line.len - 1]==' ' || line.s[line.len - 1]=='\r' ){
		       --line.len;
		    }else{
		       break;
		    }
		 }
    nownum++;
    if(nownum>=lStartRec){
    	       memset(linebuf,0,sizeof(linebuf));
    	       int rrrr;
						 rrrr=code_convert("gb2312","utf-8",line.s,line.len,pppp,2048);
						 if(rrrr==-1){
									continue;
						 }
    	       //memcpy(pppp,line.s,line.len);
				     xIndex++;
				     token=strtok(linebuf, seps); 
			       
			       memset(tempStr,0,sizeof(tempStr));
			       
 				     if(xIndex==1){
 				     	     //sprintf(tempStr,"%s","{");  
                   stralloc_cats(&strTemp,"{");  
             }else{
             	    //sprintf(tempStr,"%s",",{"); 
                	stralloc_cats(&strTemp,",{");  
             }

		  			 while(token!=NULL){
			  	 			 iIndex++;   
			  	 			 if(iIndex==1){
			          		 sprintf(tempStrStr,"\"v%d\":\"%s\"",iIndex,token);
			          		 ltmodstr(tempStrStr);
			          		 //memset(tempStr,0,sizeof(tempStr));
			          		 //strgb2utf8(tempStrStr,tempStr,strlen(tempStrStr));
			          		 //sprintf(tempStr,"%s%s",tempStr,tempStrStr); 
			          		 stralloc_cats(&strTemp,tempStrStr);				          
			        	 }else{
					 			     sprintf(tempStrStr,",\"v%d\":\"%s\"",iIndex,token);
					 			     ltmodstr(tempStrStr);
					 			     //memset(tempStr,0,sizeof(tempStr));
			          		 //strgb2utf8(tempStrStr,tempStr,strlen(tempStrStr));
					 			     //sprintf(tempStr,"%s%s",tempStr,tempStrStr);
			          		 stralloc_cats(&strTemp,tempStrStr);
							   }
  	             token  = strtok( NULL, seps);
              }
              //sprintf(tempStr,"%s%s",tempStr,"}"); 
              //memset(tempStrStr,0,sizeof(tempStrStr));
              
              stralloc_cats(&strTemp,"}");  
              iIndex=0;
    }
    if(nownum==(lRowNum+lStartRec)){
     		   		break;
    }
  }
  close(fd);
  stralloc_cats(&strTemp,"]}");
	stralloc_0(&strTemp); 
 
// char *cpOut;
// cpOut=(char *)malloc(strTemp.len*3);
// memset(cpOut,0,strTemp.len*3);
// strgb2utf8(strTemp.s,cpOut,strTemp.len);
  
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);  
	//free(cpOut);
	ltMsgFree(ltMsgPk);
	return 0;
    
}

int msasShowjson(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 	

	char   caFile1[256];
  int   fd;
	char  *fileurl;
	char  inbuf[8192];
	stralloc strTemp;
	strTemp.s=0;
	char  *pppp;
	char  *ppppcon;
	char  *ppppend;
  int   iCount,readcount;
	
	fileurl=ltMsgGetVar_s(ltMsgPk,"fileurl");
	sprintf(caFile1,"/datacenter/msas/%s",fileurl);
	fd=open(caFile1,O_RDONLY | O_NDELAY); 
	if(fd<1){
	   ltMsgPk->msgpktype=1;
		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"[]");   
		 ltMsgFree(ltMsgPk);
		 return 0;
	}
	memset(inbuf,0,sizeof(inbuf));
	iCount=read(fd,inbuf,8192); 
	readcount=0;
	while(iCount){
		if(iCount<=2){
			break;
		}
		readcount++;
		pppp=inbuf;
		ppppend=pppp+iCount;
		while(*pppp){
		  if(*pppp=='[' && *(pppp+1)==']'){
	    	*pppp=' ';
	    	*(pppp+1)=' ';
	    	iCount=iCount-2;
	    	pppp=pppp+2;
	    }else{
	    	break;
	    }
	  }
	  ppppcon=pppp;
	  while(ppppcon<ppppend){
		  if(*ppppcon=='[' && *(ppppcon+1)==']'){
	    	*ppppcon='\0';
	    	break;
	    }else{
	    	ppppcon++;
	    }
	  }
	  
//		while(ppppend>pppp){
//		  if(*(ppppend-1)==']' && *(ppppend-2)=='['){
//	    	*(ppppend-1)='\0';
//	    	*(ppppend-2)='\0';
//	    	ppppend=ppppend-2;
//	    }else{
//	    	break;
//	    }
//	  }
	  
	  if(readcount==6){
	  	while(ppppend>pppp){
			  if(*(ppppend-1)==',' && *(ppppend-2)==']'){//],结尾
		    	*(ppppend-1)=']';
		    	*ppppend='\0';
          break;
		    }else{
		    	ppppend=ppppend-1;
		    }
		  }
	  	stralloc_catb(&strTemp,pppp,iCount);
	  	break;
	  }

    stralloc_catb(&strTemp,pppp,iCount);
	  iCount=read(fd,inbuf,8192);
	}
	stralloc_0(&strTemp); 
	
	 char *cpOut;
 cpOut=(char *)malloc(strTemp.len*2);
 memset(cpOut,0,strTemp.len*2);
 strgb2utf8(strTemp.s,cpOut,strTemp.len);
	
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,cpOut);  
	ltMsgFree(ltMsgPk);

	 
	close(fd);  	
	return 0;
	
}


int msasSearchShowListt(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 	

	char   caFile1[256];
	long   lRowNum,lStartRec,lSumRec,lCount,lNum;
  int    k;
  stralloc strTemp;
  char  caTmpp[1024];
  int   fd;
  int   totalnum;
  int   nownum;
  int   iCount;
  int   inum;
  char  seps[]="\t";
  //char  seps[]=",";
	char  *token;
  char  linebuf[2048];
  int   iIndex=0;
	int   xIndex=0;
	char  tempStr[512];
	char  *pppp;
	char  *fileurl;
	static char inbuf[8192];
  static stralloc line = {0};
	buffer ss;
	int   match;
	

	fileurl=ltMsgGetVar_s(ltMsgPk,"fileurl");
  sprintf(caFile1,"%s/%s/search.csv",datacenterdir,fileurl);
 //printf("caFile1:%s\n",caFile1);	
	
  lRowNum=100; /*每页的行数*/
  lSumRec=0;/*总行数*/
  lCount=0; 
  lStartRec=0;
  lNum=0; /*该页剩余的行数*/
  if(ltMsgGetVar_s(ltMsgPk,"limit")){
    		lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
  }
  if(ltMsgGetVar_s(ltMsgPk,"start")){
    		lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
  }
  
  totalnum=0;
  
	
     
  fd=open(caFile1,O_RDONLY | O_NDELAY); 
	if(fd<1){
	    ltMsgPk->msgpktype=1;
		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");   
		 ltMsgFree(ltMsgPk);
		 return 0;
	}
	totalnum=0;
	iCount=read(fd,inbuf,8192); 
	while(iCount){ 
	    for(inum=0;inum<iCount;inum++){
	    		if(inbuf[inum]=='\n'){
	    		   totalnum++; 
	    		}
	    }
	    iCount=read(fd,inbuf,8192);
	} 
	close(fd);  	
  strTemp.s=0;
  sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",totalnum);
  stralloc_cats(&strTemp,caTmpp);
  k=0;
  
  fd=open(caFile1,O_RDONLY | O_NDELAY); 
  if(fd<1){
    		 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"{\"totalCount\":\"0\",\"Results\":[]}");   
				 ltMsgFree(ltMsgPk);
				 return 0;
  }  
  nownum=0;
  memset(linebuf,0,sizeof(linebuf));
  memset(tempStr,0,sizeof(tempStr));
  pppp=linebuf;
  buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
	for (;;)
	{
		if (getln(&ss,&line,&match,'\n') == -1) break;
		if (!match && !line.len) { break; }
		
		while (line.len > 0){
		   if(line.s[line.len - 1]=='\n' || line.s[line.len - 1]=='\r' ){
		       --line.len;
		    }else{
		       break;
		    }
		 }
    nownum++;
    if(nownum>=lStartRec){
    	       memset(linebuf,0,sizeof(linebuf));
    	       memcpy(pppp,line.s,line.len);
				     xIndex++;
				     token=strtok(linebuf, seps); 
			 
 				     if(xIndex==1){
                   stralloc_cats(&strTemp,"{");  
             }else{
                	 stralloc_cats(&strTemp,",{");  
             }

		  			 while(token!=NULL){
			  	 			 iIndex++;   
			  	 			 if(iIndex==1){
			          		 sprintf(tempStr,"\"v%d\":\"%s\"",iIndex,token);
			          		 ltmodstr(tempStr);
			          		 stralloc_cats(&strTemp,tempStr);				          
			        	 }else{
					 			     sprintf(tempStr,",\"v%d\":\"%s\"",iIndex,token);
					 			     ltmodstr(tempStr);
			          		 stralloc_cats(&strTemp,tempStr);
							   }
  	             token  = strtok( NULL, seps);
              } 
              stralloc_cats(&strTemp,"}");  
              iIndex=0;
    }
    if(nownum==(lRowNum+lStartRec)){
     		   		break;
    }
  }
  close(fd);
  stralloc_cats(&strTemp,"]}");
	stralloc_0(&strTemp); 
 
 char *cpOut;
 cpOut=(char *)malloc(strTemp.len*2);
 memset(cpOut,0,strTemp.len*2);
 strgb2utf8(strTemp.s,cpOut,strTemp.len);
  
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,cpOut);  
	free(cpOut);
	ltMsgFree(ltMsgPk);
	return 0;
    
}



int msasGetInfoByKey(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  char  *gnmkey;
  static struct sembuf lock={0,-1,SEM_UNDO};
	static struct sembuf unlock={0,1,SEM_UNDO|IPC_NOWAIT};

	gnmkey=ltMsgGetVar_s(ltMsgPk,"gnmkey");
	if(!gnmkey){
		ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"");
		ltMsgFree(ltMsgPk);
		return 0;
  }
 
  /*拷贝得到列表*/
  {
						char *p;
					  int  iii;
					  int indexWalker;
					  shm_hash_link_onlinehostindex *_gnm_hash_index;
						shm_hash_link_onlinehost      *_gnm_hash_content;
						shm_hash_link_onlinehost *walker;
						msasOnlineHost  *nasHashItemInfo;

					  p=(char *)_ltPubInfo;
						_gnm_hash_index=(shm_hash_link_onlinehostindex *)(p+sizeof(msasPubInfo));
					  _gnm_hash_content=(shm_hash_link_onlinehost *)(p+sizeof(msasPubInfo)+_ltPubInfo->_onlinehosthead.conbufptr);
					    
					  for(iii=0;iii<_ltPubInfo->_onlinehosthead.bufnum;iii++){
					  	 semop(_ltPubInfo->_onlinehosthead.locksemid,&lock,1); 
					   	 indexWalker=_gnm_hash_index[iii].next;
					   	 if(indexWalker==-1){
					   	 	semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
					   	 	continue;
					   	 }
					   	 semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
							 walker = &_gnm_hash_content[indexWalker];
							 for (; ; ) {
										  nasHashItemInfo=&walker->item;
										  if( strcmp(nasHashItemInfo->caKey,gnmkey)==0 ) {/*符合条件*/
                          char myipadd[32];
													struct in_addr s;
													s.s_addr = htonl(nasHashItemInfo->ipadd);
													sprintf(myipadd,"%s",inet_ntoa(s));
													ltMsgPk->msgpktype=1;
													lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,myipadd);
													ltMsgFree(ltMsgPk);
													return 0;
										  }						
											semop(_ltPubInfo->_onlinehosthead.locksemid,&lock,1); 
											if(walker->next == -1){
												semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
												break;
											}
											indexWalker = walker->next;
											semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
											walker = &_gnm_hash_content[indexWalker];
							 }
					  }
	}
  	
  
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"");
    
	ltMsgFree(ltMsgPk);
	return 0;
	
}

int msasShKeyList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	 char *type=NULL;
	 ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   if(ltMsgGetVar_s(ltMsgPk,"type")){
      type=ltMsgGetVar_s(ltMsgPk,"type");
   }
//printf(type);
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   lt_dbput_rootvars(dbPtr,1,"myRule_Type",type);
   
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/datacenter/shfenandkeywordalertlist.htm","Content-type: text/html; charset=utf-8\r\n",0);
   ltMsgFree(ltMsgPk); 
   return 0; 
}

int msasShKeySearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char *myRule_type=NULL;
   char *mykeyWord=NULL;
   LT_DBROW   tempRow;
	 char strBuf[1024];
   ltDbCursor *tempCursor;
   char caTmpp[512];
   long lRowNum,lStartRec,lSumRec,lCount,lNum;
   int k;
   stralloc strTemp;
   
   
   if(ltMsgGetVar_s(ltMsgPk,"AlertmyRule_type")){
     myRule_type=ltMsgGetVar_s(ltMsgPk,"AlertmyRule_type");
   }
   if(ltMsgGetVar_s(ltMsgPk,"myKey")){
     mykeyWord=ltMsgGetVar_s(ltMsgPk,"myKey");
   }
      
	  lRowNum=20; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0; 
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
    if(ltMsgGetVar_s(ltMsgPk,"limit"))
		{
	    lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	  }
    if(ltMsgGetVar_s(ltMsgPk,"start"))
		{
	    lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	  }
    sprintf(strBuf,"select count(*) from AlertList where  Rule_type='%s' and keyword1='%s'",myRule_type,mykeyWord);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
		{
		  tempRow= ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL)
		  {
			  lCount=atol(tempRow[0]);
		  }
	  }
    ltDbCloseCursor(tempCursor);
    strTemp.s=0;
	  //printf("lCount:%ld\n",lCount);
    sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);
    k=0;
    sprintf(strBuf,"select Matching_id,gnmkey,sname,Rule_id,Rule_name,anjian_id,Matching_time,Net_ending_ip,Net_ending_code,Net_ending_mac,Destination_ip,Service_type,KeyWord1,KeyWord2,KeyWord3,Customer_name,Certificate_type,Certificate_code,Rule_type,contenturl,updateflag from AlertList where Rule_type='%s' and keyword1='%s'  order by matching_time desc limit %lu offset %lu ",myRule_type,mykeyWord,lRowNum,lStartRec);
    tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {                
          if(k==0)
					{
            sprintf(caTmpp,"{\"Matching_id\":\"%s\",\"gnmkey\":\"%s\",\"sname\":\"%s\",\"Rule_id\":\"%s\",\"Rule_name\":\"%s\",\"anjian_id\":\"%s\",\"Matching_time\":\"%s\",\"Net_ending_ip\":\"%s\",\"Net_ending_code\":\"%s\",\"Net_ending_mac\":\"%s\",\"Destination_ip\":\"%s\",\"Service_type\":\"%s\",\"KeyWord1\":\"%s\",\"KeyWord2\":\"%s\",\"KeyWord3\":\"%s\",\"Customer_name\":\"%s\",\"Certificate_type\":\"%s\",\"Certificate_code\":\"%s\",\"Rule_type\":\"%s\",\"contenturl\":\"%s\",\"updateflag\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14],tempRow[15],tempRow[16],tempRow[17],tempRow[18],tempRow[19],tempRow[20]);
	          stralloc_cats(&strTemp,caTmpp);     
					}
					else
					{
			      sprintf(caTmpp,",{\"Matching_id\":\"%s\",\"gnmkey\":\"%s\",\"sname\":\"%s\",\"Rule_id\":\"%s\",\"Rule_name\":\"%s\",\"anjian_id\":\"%s\",\"Matching_time\":\"%s\",\"Net_ending_ip\":\"%s\",\"Net_ending_code\":\"%s\",\"Net_ending_mac\":\"%s\",\"Destination_ip\":\"%s\",\"Service_type\":\"%s\",\"KeyWord1\":\"%s\",\"KeyWord2\":\"%s\",\"KeyWord3\":\"%s\",\"Customer_name\":\"%s\",\"Certificate_type\":\"%s\",\"Certificate_code\":\"%s\",\"Rule_type\":\"%s\",\"contenturl\":\"%s\",\"updateflag\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14],tempRow[15],tempRow[16],tempRow[17],tempRow[18],tempRow[19],tempRow[20]);
		        stralloc_cats(&strTemp,caTmpp);     
					}                         
					tempRow= ltDbFetchRow(tempCursor);
					k++;
		    }
       ltDbCloseCursor(tempCursor);      
    }
   stralloc_cats(&strTemp,"]}");
   stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
   ltMsgFree(ltMsgPk);
   return 0;
}
