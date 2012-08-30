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
#include "msas.h"

int  saveJPG(void *temp,int size, char *fn){
	
	int fd;
	//printf("11111111111111111111111\n");
	fd=open(fn,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
	//printf("22222222222222222222\n");
	write(fd,temp,size);
	//printf("33333333333333333333333\n");
        free(temp);
	close(fd);

}


int msasShowAdminLogon(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
	ltDbHeadPtr dbPtr;
	
	unsigned int sid;
	char sIp[24];
  int  i;
	char strsid[12];

	int  adminipcount;
	int  ipok;
	
  char sqlBuf[1024];
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	
	sid=0;
	memset(sIp,0,sizeof(sIp));


	ltMsgGetSomeNVar(ltMsgPk,1,"clientip",    LT_TYPE_STRING, 19, sIp);



	
	dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
  

  adminipcount=0;
  ipok=0;
 

  
  sprintf(sqlBuf,"select count(*) from msasAdminArea ");
			
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					adminipcount=atol(tempRow[0]);
				}
				ltDbCloseCursor(tempCursor);
	}
	

  if(adminipcount==0){
  	ipok=1;
  }else{
      unsigned int s_ip,e_ip,my_ip;
      my_ip=ntohl(inet_addr(sIp));
      
      sprintf(sqlBuf,"select ipstart,ipend from msasAdminArea ");
			
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
						tempRow= ltDbFetchRow(tempCursor);
						while(tempRow!=NULL){
							s_ip=ntohl(inet_addr(tempRow[0]));
							e_ip=ntohl(inet_addr(tempRow[1]));
							if( (my_ip >=s_ip) && (my_ip <=e_ip) ){ 
							   ipok=1;
							   break;
							}
						}
						ltDbCloseCursor(tempCursor);
			}
  }
	 

  if(ipok==0){
  	

		 ltMsgPk->msgpktype=1;
 		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\">Ip Area error!</body></html>");		
		 ltMsgFree(ltMsgPk);
		 return 0;
  	
  }
  
  for(i=0;i<MAX_ADMIN_SESSION;i++){
  	if(strcmp(_ltPubInfo->admSession[i].lright,"0000000000000000000000000000000000000000000")==0){
  		if( (time(0)-_ltPubInfo->admSession[i].lasttime) > MAX_ADMIN_TRYTIMEOUT  ){
          _ltPubInfo->admSession[i].lSip=0;
  			  _ltPubInfo->admSession[i].lasttime=0;
  				_ltPubInfo->admSession[i].trytimes=0;
  		    _ltPubInfo->admSession[i].lSid=0;
  		    sprintf(_ltPubInfo->admSession[i].lright,"%s","0000000000000000000000000000000000000000000");		   
  	 }
  	}else{
  		if( (time(0)-_ltPubInfo->admSession[i].lasttime) > MAX_ADMIN_TIMEOUT  ){
          _ltPubInfo->admSession[i].lSip=0;
  			  _ltPubInfo->admSession[i].lasttime=0;
  				_ltPubInfo->admSession[i].trytimes=0;
  		    _ltPubInfo->admSession[i].lSid=0;
  		    sprintf(_ltPubInfo->admSession[i].lright,"%s","0000000000000000000000000000000000000000000");		   
  		}
    }
  }
   
  for(i=0;i<MAX_ADMIN_SESSION;i++){	
  	if(_ltPubInfo->admSession[i].lSip==ntohl(inet_addr(sIp))   ){
          _ltPubInfo->admSession[i].lSip=0;
  			  _ltPubInfo->admSession[i].lasttime=0;
  				_ltPubInfo->admSession[i].trytimes=0;
  		    _ltPubInfo->admSession[i].lSid=0;
  		    sprintf(_ltPubInfo->admSession[i].lright,"%s","0000000000000000000000000000000000000000000");		   
  	}
  }
	      
	{
	    
				gdImagePtr im;
				int   white,red;
				char caTempDir[128];
        char caTempFile[128];
        void *pJpg;
				int  lSize;
				
				
				sid=time(0)%10000;
				sprintf(strsid,"%04d",sid);
				
				im = gdImageCreate(40,16);
				red= gdImageColorAllocate(im, 0, 0, 0);
				white= gdImageColorAllocate(im, 255, 255, 255);
				
				gdImageRectangle(im, 0, 0, 40, 16, red);
				gdImageString(im, gdFontMediumBold, 5, 2, strsid, white);
				
				sprintf(caTempDir,"%s/%lu/",_TMPDIR,ltStrGetId());
		    if(ltFileIsExist(caTempDir)!=1) {
		        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {

		            return 0;
		        }
		    }
		    sprintf(caTempFile,"%scheck.jpg",caTempDir);
		    pJpg = gdImageJpegPtr( im,&lSize,-1);
		    gdImageDestroy(im);

		    saveJPG(pJpg,lSize, caTempFile);

		    lt_db_setcookie(dbPtr,"strsid", strsid);
		        
		    lt_dbput_rootvars(dbPtr,1,"file1",caTempFile+strlen(_TMPDIR)+1);
 
        ltMsgPk->msgpktype=1;
				lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/index.htm",dbPtr->head,0);
   }
	
 
	
	 ltMsgFree(ltMsgPk);
	 lt_dbfree(dbPtr);
	 return 0;

}


int msasAdminLogout(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	ltDbHeadPtr dbPtr;
	int i;
	
	char sIp[24];
	char strUserName[64];

	memset(sIp,0,sizeof(sIp));
	

	ltMsgGetSomeNVar(ltMsgPk,1,"clientip",    LT_TYPE_STRING, 19, sIp);
	
	dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
  
  
  for(i=0;i<MAX_ADMIN_SESSION;i++){
  	
  	if(_ltPubInfo->admSession[i].lSip==ntohl(inet_addr(sIp)) ){
  		
          _ltPubInfo->admSession[i].lSip=0;
  			  _ltPubInfo->admSession[i].lasttime=0;
  				_ltPubInfo->admSession[i].trytimes=0;
  		    _ltPubInfo->admSession[i].lSid=0;
  		    sprintf(_ltPubInfo->admSession[i].lright,"%s","0000000000000000000000000000000000000000000");		   
  		   sprintf(strUserName,"%s",_ltPubInfo->admSession[i].caname );
  		
  	}
  }
	

	
	
	 
	 {
				int lTime;
				char  caDate[64];
				char  sqlBuf[1024];
				lTime = time(0);
	    	strcpy(caDate,ltTimeFormat("%Y-%m-%d %H:%M:%S",lTime));
	    	sprintf(sqlBuf,"insert into msasadmlog values('%s','%s','user %s logout success. <from %s >','01')",strUserName,caDate,strUserName,sIp); 
				ltDbExecSql(G_DbCon,sqlBuf); 
   }
		    
   ltMsgPk->msgpktype=1;
   ltWebMsgErr("close!", confd,ltMsgPk);
	
	 ltMsgFree(ltMsgPk);
	 lt_dbfree(dbPtr);
	 return 0;

}



int msasAdminLogon(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	LT_DBROW tempRow;
	char sqlBuf[1024];
	ltDbHeadPtr dbPtr;
  char *strUserName;
	char *strPassword;
  char newPass[50];
	ltDbCursor *tempCursor;
	int logFlag;
	char sRight[64];

	int  i;
	
	char sIp[24];


	char strsid[12];
	char tmpsid[12];

	
	
  char langsel[12];
	char stylesel[12];
	char strusertype[12];
	
	
	memset(langsel,0,sizeof(langsel));
	memset(stylesel,0,sizeof(stylesel));
	memset(strusertype,0,sizeof(strusertype));
	
	sprintf(langsel,"%s","0");
	sprintf(stylesel,"%s","0");
	
	memset(sIp,0,sizeof(sIp));
	memset(strsid,0,sizeof(strsid));
	memset(tmpsid,0,sizeof(tmpsid));
	

	
	ltMsgGetSomeNVar(ltMsgPk,2,"sid",LT_TYPE_STRING, 19, strsid,
														"strsid",LT_TYPE_STRING, 19, tmpsid );
														
	sprintf(strsid,"%s",ltMsgGetVar_s(ltMsgPk,"sid"));
	
  if( strlen(langsel)<1 ){
  	sprintf(langsel,"%s","0");
  }
  if( strlen(stylesel)<1 ){
  	sprintf(stylesel,"%s","0");
  }
  
	logFlag=0;
	memset(newPass,0,sizeof(newPass));
	snprintf(sIp,19,ltMsgGetVar_s(ltMsgPk,"clientip"));
	strUserName=ltMsgGetVar_s(ltMsgPk,"username");
	strPassword=ltMsgGetVar_s(ltMsgPk,"password");
	
//printf("strsIndex:%s  sip:%s strUserName:%s stylesel:%s strsid:%s\n",strsIndex,sIp,strUserName,stylesel,strsid);

	ltMd5Encrypto(strPassword, strlen(strPassword),newPass);
	
	dbPtr=lt_dbinit();
	
//
//printf("strsid:%s ,tmpsid:%s\n",strsid,tmpsid);
//

  for(i=0;i<MAX_ADMIN_SESSION;i++){
  	if(_ltPubInfo->admSession[i].lSip==ntohl(inet_addr(sIp)) ){
          _ltPubInfo->admSession[i].lSip=0;
  			  _ltPubInfo->admSession[i].lasttime=0;
  				_ltPubInfo->admSession[i].trytimes=0;
  		    _ltPubInfo->admSession[i].lSid=0;
  		    sprintf(_ltPubInfo->admSession[i].lright,"%s","0000000000000000000000000000000000000000000");		   
  		   sprintf(strUserName,"%s",_ltPubInfo->admSession[i].caname );
  		
  	}
  }
	
  
	if( strcmp(strsid,tmpsid) ){
		ltMsgPk->msgpktype=1;
	  //ltWebMsgErr(strGmsg[atol(langsel)][atol(stylesel)][4], confd,ltMsgPk);
	  ltWebMsgErr(_ltPubInfo->pubMsgStr[0].conStr, confd,ltMsgPk);
	  
	  ltMsgFree(ltMsgPk);
		return 0;
	}
			

	i=0;
	sprintf(sqlBuf,"select name,lright,bindip from msasadmuser where name='%s' and password='%s'",strUserName,newPass);		
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
					for(i=0;i<MAX_ADMIN_SESSION;i++){
						  	if(_ltPubInfo->admSession[i].lSip==0 || (time(0) -_ltPubInfo->admSession[i].lasttime ) > 7200   ){
						  		
						          _ltPubInfo->admSession[i].lSip=ntohl(inet_addr(sIp));
						          _ltPubInfo->admSession[i].lasttime=time(0);
											_ltPubInfo->admSession[i].trytimes=0;
											if(tempRow[2][0]){
												if(strcmp(tempRow[2],sIp)){
													logFlag=2;
												  break;
												}
											}
											sprintf(_ltPubInfo->admSession[i].caname,"%s",strUserName);
											sprintf(_ltPubInfo->admSession[i].lright,"%s",tempRow[1]);
											sprintf(sRight,"%s",tempRow[1]);
											logFlag=1;
					  		      break;
						  	}
					}
		}
		ltDbCloseCursor(tempCursor);
	}
	
  
	if(logFlag==1){
		/*
		unsigned long onlinetime;
		unsigned int sessionId; 
		char   sid[30];
		*/
		char   sid[30];
		lt_db_htmlpage(dbPtr,"utf-8");
		lt_db_setcookie(dbPtr,"manageruser", strUserName);
		//printf("set sRight:%s\n",sRight);
		//lt_db_setcookie(dbPtr,"sright", sRight);
		sprintf(sid,"%d",i);
		lt_db_setcookie(dbPtr,"langsel", langsel);
    lt_db_setcookie(dbPtr,"msassessionid", sid);
				  
		lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/main/main.htm",dbPtr->head,0);
		sprintf(sqlBuf,"update msasadmuser set lasttime=%ld where name='%s' ",time(0),strUserName); 
		ltDbExecSql(G_DbCon,sqlBuf); 
		  
			
			{
				int lTime;
				char  caDate[64];
				lTime = time(0);
	    	strcpy(caDate,ltTimeFormat("%Y-%m-%d %H:%M:%S",lTime));
				sprintf(sqlBuf,"insert into msasadmlog values('%s','%s','user login success! <from %s>','1')",strUserName,caDate,sIp); 
		    ltDbExecSql(G_DbCon,sqlBuf); 
		  }

		
	}else if(logFlag==2){
		 ltMsgPk->msgpktype=1;
 		 ltWebMsgErr(_ltPubInfo->pubMsgStr[1].conStr, confd,ltMsgPk);
		 return 0;
	}else{
	  
	  	{
				int lTime;
				char  caDate[64];
				lTime = time(0);
	    	strcpy(caDate,ltTimeFormat("%Y-%m-%d %H:%M:%S",lTime));
				sprintf(sqlBuf,"insert into msasadmlog values('%s','%s','user login failed! <from %s>','1')",strUserName,caDate,sIp); 
		    ltDbExecSql(G_DbCon,sqlBuf); 
		  }
	
		lt_db_htmlpage(dbPtr,"utf-8");
    
    ltWebMsgErr(_ltPubInfo->pubMsgStr[2].conStr, confd,ltMsgPk);
		
	  ltMsgFree(ltMsgPk);
		return 0;
		
	}
	
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}

int msaShowTopview(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
		

  ltDbHeadPtr dbPtr; 

	char sRight[50];

	
	int  iIndex;
	char sIp[24];
	char langsel[12];
	int  i;

	memset(langsel,0,sizeof(langsel));

	sprintf(langsel,"%s","0");

		
  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	
	memset(sIp,0,sizeof(sIp));
	ltMsgGetSomeNVar(ltMsgPk,2,"clientip",LT_TYPE_STRING, 19, sIp,"langsel",LT_TYPE_STRING, 12, langsel );
	
	if( strlen(langsel)<1 ){
	  	sprintf(langsel,"%s","0");
	}

	iIndex=-1;
  for(i=0;i<MAX_ADMIN_SESSION;i++){
  	
  	if(_ltPubInfo->admSession[i].lSip==ntohl(inet_addr(sIp)) ){
  		
  		   iIndex=i;
  		   break;
  	}
  }
  
  if(iIndex==-1 ){
  	ltWebMsgErr(_ltPubInfo->pubMsgStr[3].conStr, confd,ltMsgPk);
	  ltMsgFree(ltMsgPk);
		return 0;
	}

	
	
	if( (time(0)-_ltPubInfo->admSession[iIndex].lasttime) > 3600 ){
		ltWebMsgErr(_ltPubInfo->pubMsgStr[4].conStr, confd,ltMsgPk);
	  ltMsgFree(ltMsgPk);
		return 0;
	}
	
	_ltPubInfo->admSession[iIndex].lasttime=time(0);
	memset(sRight,0,sizeof(sRight));
	sprintf(sRight,"%s",_ltPubInfo->admSession[iIndex].lright);
    
  
//  if(sRight[0]=='1'){
//  	sprintf(caMsg,"%s","<td width=70 id=menu1 class=3dtable1_x  onClick=\"goto(this,'/adduser/adduser.htm');\" onmouseover=menuover(this)   onmouseout=menuout(this)>\r\n        ?????  \r\n</td>");
//  	lt_dbput_rootvars(dbPtr,1,"strhead1",caMsg);
//  }
//  

	 lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/main/top_header.htm",dbPtr->head,0);
		
		
	 ltMsgFree(ltMsgPk);
	 lt_dbfree(dbPtr);
	 return 0;

}


int msasSysReset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{

    char sIp[24];
	  char strUserName[64];
	  char caMsg[256];
	  memset(sIp,0,sizeof(sIp));
	  memset(strUserName,0,sizeof(strUserName));
    
    if(checkRight(confd,ltMsgPk,MSAS_RIGHT_SYSADMIN,lt_MMHead)==-1){
			return -1;
    }
    ltMsgGetSomeNVar(ltMsgPk,2,"clientip", LT_TYPE_STRING, 19, sIp,"manageruser",LT_TYPE_STRING, 32,strUserName);
    
    sprintf(caMsg,"%s reboot system success.",strUserName);
	  logWrite(ltMsgPk,caMsg,9);	
	  system("/app/msa/bin/msasshutdownall");	
    system("/sbin/reboot");	  
    system("/sbin/reboot");
    return 0;
}

int msasSysShutdown(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
  
    char sIp[24];
	  char strUserName[64];
	  char caMsg[256];
	  memset(sIp,0,sizeof(sIp));
	  memset(strUserName,0,sizeof(strUserName));
    
    if(checkRight(confd,ltMsgPk,MSAS_RIGHT_SYSADMIN,lt_MMHead)==-1){
			return -1;
    }
    ltMsgGetSomeNVar(ltMsgPk,2,"clientip", LT_TYPE_STRING, 19, sIp,"manageruser",LT_TYPE_STRING, 32,strUserName);
    
    sprintf(caMsg,"%s shutdown system success.",strUserName);
	  logWrite(ltMsgPk,caMsg,9);	
	  system("/app/msa/bin/msasshutdownall");	
	
    system("/sbin/poweroff");
    system("/sbin/poweroff");	  
    system("/sbin/init 0");	   
    return 0;
}