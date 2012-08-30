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
#include "icm.h"

/*
*在线管理员
*/
int icmAdminUserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	int i;
	char caSel[32];
	char sqlBuf[1024];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	char     caRight[128];
	long inum;
	char caTime[40];
  char *strUserName;
  strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
  if(!strUserName){
  	strUserName="";
  }
  
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	
	lt_dbput_rootvars(dbPtr,1,"checkall"," checked ");
	tablePtr=lt_dbput_table(dbPtr,"list");

  sprintf(sqlBuf,"select name,lright,lasttime,bindip from icmadmuser ");
	inum=0;
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			inum++;
			nasTimeFormat(caTime,atol(tempRow[2]));
			lt_dbput_recordvars(tablePtr,3,
				"num",LT_TYPE_LONG,inum,
				"logname",LT_TYPE_STRING,tempRow[0],
				"lasttime",LT_TYPE_STRING,caTime);
				
			if(strcmp(strUserName,tempRow[0])==0){				
				lt_dbput_rootvars(dbPtr,1,"logname",tempRow[0]);
				memset(caRight,0,sizeof(caRight));
				sprintf(caRight,"%s",tempRow[1]);
				for(i=0;i<5;i++){
					sprintf(caSel,"chsel%d",i+1);
					if(caRight[i]=='1'){
						lt_dbput_rootvars(dbPtr,1,caSel," checked ");
					}
				}
				lt_dbput_rootvars(dbPtr,1,"bindip",tempRow[3]);
			}
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/icm/htmlplt/manager/adminuser.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}
/*
*密码修改判断
*/
int ltPassIsOk(char *InStr)
{
   register int i;
   int   findD;
   int   findS;
   findD=0;
   findS=0;
   for(i=0;InStr[i]!=0;i++) {
     if(InStr[i]>'9'||InStr[i]<'0') {
         findS=1;
     }else{
       findD=1;
     }
   }
   if(findD && findS){
   			return 1;
   }
   return 0;
}

/*
*管理员操作
*/
int icmAdminAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
	char *strUserName;
	char *strPassword;
  char newPass[50];
	char sRight[100];

	int  i;
	char caSel[20];
	char sIp[24];
	char caMsg[256];
	int  passisok;
	char *bindip;
  int  actiontype;
  
  actiontype=0;
	passisok=0;
	memset(sIp,0,sizeof(sIp));
	memset(newPass,0,sizeof(newPass));
	memset(sRight,0,sizeof(sRight));

	actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));

	if(actiontype==1){/*add*/
			strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
			strPassword=ltMsgGetVar_s(ltMsgPk,"pass");
			bindip=ltMsgGetVar_s(ltMsgPk,"bindip");
			if(!bindip){
				bindip="";
			}
//			if(strlen(strPassword)<8){
//				ltMsgPk->msgpktype=1;
//				ltWebMsgErr(_ltPubInfo->pubMsgStr[8].conStr, confd,ltMsgPk);
//				ltMsgFree(ltMsgPk);
//				return 0;
//			}
			passisok=ltPassIsOk(strPassword);
			if(passisok==0){
				ltMsgPk->msgpktype=1;
				ltWebMsgErr(_ltPubInfo->pubMsgStr[8].conStr, confd,ltMsgPk);
				ltMsgFree(ltMsgPk);
				return 0;
			}
			ltMd5Encrypto(strPassword, strlen(strPassword),newPass);
			ltMsgGetSomeNVar(ltMsgPk,1,"clientip",LT_TYPE_STRING, 19, sIp);
			memset(sRight,0,sizeof(sRight));
			memset(sRight,'0',27);
			for(i=0;i<5;i++){
				sprintf(caSel,"chr%d",i+1);
				if(ltMsgGetVar_s(ltMsgPk,caSel)!=NULL){
					sRight[i]='1';
				}
			}
			sprintf(sqlBuf,"insert into icmadmuser values ('%s','%s','%s','',%ld,'%s')",
					strUserName,
					newPass,
					sRight,
					time(0),bindip);
			ltDbExecSql(G_DbCon,sqlBuf); 
			sprintf(caMsg,"add new user %s success.",strUserName);
			logWrite(ltMsgPk,caMsg,5);
	}else if(actiontype==2){/*del*/
			strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
			sprintf(sqlBuf,"delete from icmadmuser where name='%s' ",	strUserName);
			ltDbExecSql(G_DbCon,sqlBuf); 
			//char caMsg[256];
			sprintf(caMsg,"delete %s success.",strUserName);
			logWrite(ltMsgPk,caMsg,5);
	}else if(actiontype==3){/*update indfo*/
			memset(sRight,0,sizeof(sRight));
			strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
			memset(sRight,'0',5);	
			for(i=0;i<5;i++){
				sprintf(caSel,"chr%d",i+1);
				if(ltMsgGetVar_s(ltMsgPk,caSel)!=NULL){
					sRight[i]='1';
				}
			}
			sprintf(sqlBuf,"update icmadmuser set lright='%s' where name='%s' ",sRight,strUserName);
		  ltDbExecSql(G_DbCon,sqlBuf); 
			sprintf(caMsg,"update %s right success.",strUserName);
			logWrite(ltMsgPk,caMsg,2);
	}else if(actiontype==4){/*change password indfo*/
			memset(newPass,0,sizeof(newPass));
			strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
			strPassword=ltMsgGetVar_s(ltMsgPk,"pass");
			passisok=0;
		  passisok=ltPassIsOk(strPassword);
			if(passisok==0){
				ltMsgPk->msgpktype=1;
				ltWebMsgErr(_ltPubInfo->pubMsgStr[8].conStr,confd,ltMsgPk);
				ltMsgFree(ltMsgPk);
				return 0;
			}
			ltMd5Encrypto(strPassword, strlen(strPassword),newPass);
			sprintf(sqlBuf,"update icmadmuser set password='%s' where name='%s' ",newPass,strUserName);
			ltDbExecSql(G_DbCon,sqlBuf); 
			sprintf(caMsg,"change %s password success.",strUserName);
			logWrite(ltMsgPk,caMsg,2);
	}else if(actiontype==5){/*change ip*/
			strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
			bindip=ltMsgGetVar_s(ltMsgPk,"bindip");
			sprintf(sqlBuf,"update icmadmuser set bindip='%s'where name='%s' ",bindip,strUserName);
		  ltDbExecSql(G_DbCon,sqlBuf); 
			sprintf(caMsg,"update %s ip success.",strUserName);
			logWrite(ltMsgPk,caMsg,2);
	}
	icmAdminUserList(confd,ltMsgPk,lt_MMHead);
	return 0;
}

/*
*在线管理员列表
*/
int icmAdmuserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   stralloc strTemp;
   int  allcount;
   int  i,k;
   char caTmpp[512];
   
   allcount=0;
   for(i=0;i<20;i++){
  	if( (_ltPubInfo->admSession[i].lSip>0) && ((time(0)-_ltPubInfo->admSession[i].lasttime)<7200)  ){
  			allcount++;   
  	}
  }
   
   strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
   k=0;

   for(i=0;i<20;i++){
  	if( (_ltPubInfo->admSession[i].lSip>0) && ((time(0)-_ltPubInfo->admSession[i].lasttime) < 7200)  ){
  			struct in_addr s;
  			char   xStatus[24];
  			s.s_addr = htonl(_ltPubInfo->admSession[i].lSip);
  			if(strcmp(_ltPubInfo->admSession[i].lright,"0000000000000000000000000000000000000000000")==0){
		  		sprintf(xStatus,"%s","Trying");
		  	}else{
		  		sprintf(xStatus,"%s","Success");
		    }
  			if(k==0){
	     	  	 sprintf(caTmpp,"{\"xindex\":\"%d\",\"ip\":\"%s\",\"lasttime\":\"%s\",\"trytime\":\"%d\",\"status\":\"%s\"}",
	     	  	 i,inet_ntoa(s),ltTimeFormat("%Y-%m-%d %H:%M:%S",_ltPubInfo->admSession[i].lasttime),_ltPubInfo->admSession[i].trytimes,xStatus );
	     	     stralloc_cats(&strTemp,caTmpp);
	     	}else{
	     	  	 sprintf(caTmpp,",{\"xindex\":\"%d\",\"ip\":\"%s\",\"lasttime\":\"%s\",\"trytime\":\"%d\",\"status\":\"%s\"}",
	     	  	 i,inet_ntoa(s),ltTimeFormat("%Y-%m-%d %H:%M:%S",_ltPubInfo->admSession[i].lasttime),_ltPubInfo->admSession[i].trytimes,xStatus );
	     	     stralloc_cats(&strTemp,caTmpp);
	     	}
	     	k++; 
  	}
  } 
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
	 
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
    
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//强制下线////////////////////////////////////////////////////////////////////////////////////////////////////
int icmCheckOutAdmin(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	int  iindex;
	char strIndex[12];
	memset(strIndex,0,sizeof(strIndex));
  ltMsgGetSomeNVar(ltMsgPk,1,"xindex", LT_TYPE_STRING, 12, strIndex);
	iindex=atol(strIndex);
	if(iindex<20 && iindex>-1 ){
	     _ltPubInfo->admSession[iindex].lSip=0;
			_ltPubInfo->admSession[iindex].trytimes=0;
		  _ltPubInfo->admSession[iindex].lSid=0;
		  sprintf(_ltPubInfo->admSession[iindex].lright,"%s","0000000000000000000000000000000000000000000");		     
  }
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"SUCCESS"); 
	ltMsgFree(ltMsgPk);
	return 0;
}

/*
*管理员操作日志
*/
int icmAdmlog(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    char strBuf[1024];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char caTmpp[512];
    long lRowNum,lStartRec,lSumRec,lCount,lNum;
    int k;
    char *caName;
    char *caSubject;
    char *caloglevel;
    char *cadtype;
    char *casdate;
    char *caedate;
    char *casdate3;
    char *castime;
    char *caetime;
    char caWhere[512];
    stralloc strTemp;
    char *exportaction;
    
    caName=ltMsgGetVar_s(ltMsgPk,"name");
    caSubject=ltMsgGetVar_s(ltMsgPk,"subject");
    caloglevel=ltMsgGetVar_s(ltMsgPk,"loglevel");
    cadtype=ltMsgGetVar_s(ltMsgPk,"dtype");
    casdate=ltMsgGetVar_s(ltMsgPk,"sdate");
    caedate=ltMsgGetVar_s(ltMsgPk,"edate");
    casdate3=ltMsgGetVar_s(ltMsgPk,"sdate3");
    castime=ltMsgGetVar_s(ltMsgPk,"stime");
    caetime=ltMsgGetVar_s(ltMsgPk,"etime");
    exportaction=ltMsgGetVar_s(ltMsgPk,"export");
    if(!exportaction){
    	exportaction="";
    }
		memset(caWhere,0,sizeof(caWhere));
		sprintf(caWhere,"%s"," 1=1 ");
		
    if(caName && caName[0] ){
    	sprintf(caWhere,"%s and name like '%c%s%c'",caWhere,'%',caName,'%');
    }
    if(caSubject && caSubject[0] ){
    	sprintf(caWhere,"%s and subject like '%c%s%c'",caWhere,'%',caSubject,'%');
    }
    if(caloglevel && caloglevel[0] && caloglevel[0]!='0' ){
    	sprintf(caWhere,"%s and  loglevel = '%s'",caWhere,caloglevel);
    }
    if(cadtype==NULL){
    	cadtype="0";
    }
    if( cadtype[0]=='1' ){
    	if(casdate && casdate[0] ){
	    	sprintf(caWhere,"%s and  sdate >= '%s 00:00:00'",caWhere,casdate);
	    }
    	if(caedate && caedate[0] ){
	    	sprintf(caWhere,"%s and  sdate <= '%s 00:00:00'",caWhere,caedate);
	    }
    }else if( cadtype[0]=='2' ){
    	if(casdate3 && casdate3[0] && castime && castime[0] ){
	    	sprintf(caWhere,"%s and  sdate >= '%s %s'",caWhere,casdate3,castime);
	    }
    	if(casdate3 && casdate3[0] && caetime && caetime[0] ){
	    	sprintf(caWhere,"%s and  sdate <= '%s %s'",caWhere,casdate3,caetime);
	    }
    }
    if(strcmp(exportaction,"1")==0){/*导出到excel*/
    	 char caFileName[128];
       sprintf(caFileName,"admlogexport-%d.csv",time(0));
	     k=0;
	     ltMsgPk->msgpktype=2;
			 lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,caFileName,"contenttype","Application/MS-DOWNLOAD");
		   sprintf(strBuf,"select name,sdate,loglevel,subject from icmadmlog where %s ",caWhere);
		   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		   if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
				  while(tempRow!=NULL){		
			     	sprintf(caTmpp,"%s\t%s\t%s\t%s\r\n",tempRow[0],tempRow[1],tempRow[2],tempRow[3]);	
			     	write(confd,caTmpp,strlen(caTmpp));
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
    lRowNum=50; /*每页的行数*/
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
    sprintf(strBuf,"select count(*) from icmadmlog where %s ",caWhere);

		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					lCount=atol(tempRow[0]);
				}
		}
		ltDbCloseCursor(tempCursor);
   
    strTemp.s=0;
    sprintf(caTmpp,"{\"totalCount\":\"%u\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);

   k=0;
   sprintf(strBuf,"select name,sdate,loglevel,subject from icmadmlog where %s limit %lu offset %lu ",caWhere,lRowNum,lStartRec);
   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
   if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
		  while(tempRow!=NULL){		
		  	 if(k==0){
	     	  	sprintf(caTmpp,"{\"name\":\"%s\",\"sdate\":\"%s\",\"loglevel\":\"%s\",\"subject\":\"%s\"}",
	     	  	 tempRow[0],tempRow[1],tempRow[2],tempRow[3]);
	     	 		stralloc_cats(&strTemp,caTmpp);	
	     	 }else{
	     	  	sprintf(caTmpp,",{\"name\":\"%s\",\"sdate\":\"%s\",\"loglevel\":\"%s\",\"subject\":\"%s\"}",
	     	  	 tempRow[0],tempRow[1],tempRow[2],tempRow[3]);
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
/*
*设备重启
*/
int icmSysReset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    char sIp[24];
	  char strUserName[64];
	  char caMsg[256];
	  memset(sIp,0,sizeof(sIp));
	  memset(strUserName,0,sizeof(strUserName));
    
    ltMsgGetSomeNVar(ltMsgPk,2,"clientip", LT_TYPE_STRING, 19, sIp,"manageruser",LT_TYPE_STRING, 32,strUserName);
    
    sprintf(caMsg,"%s reboot system success.",strUserName);
	  logWrite(ltMsgPk,caMsg,9);	
	  system("/app/msa/bin/icmshutdownall");	
    system("/sbin/reboot");	  
    system("/sbin/reboot");
    return 0;
}
/*
*设备关机
*/
int icmSysShutdown(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    char sIp[24];
	  char strUserName[64];
	  char caMsg[256];
	  memset(sIp,0,sizeof(sIp));
	  memset(strUserName,0,sizeof(strUserName));
    
    ltMsgGetSomeNVar(ltMsgPk,2,"clientip", LT_TYPE_STRING, 19, sIp,"manageruser",LT_TYPE_STRING, 32,strUserName);
    
    sprintf(caMsg,"%s shutdown system success.",strUserName);
	  logWrite(ltMsgPk,caMsg,9);	
	  system("/app/msa/bin/icmshutdownall");	
    system("/sbin/poweroff");
    system("/sbin/poweroff");	  
    system("/sbin/init 0");	   
    return 0;
}

///////////////////////////
int ltnetareaaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
         char sqlBuf[1024];
         ltDbCursor *tempCursor;
         LT_DBROW tempRow;
         int  actiontype;
         char caMsg[256];
         actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));
         if(actiontype==1){/*add*/
                   long lCount;
                   sprintf(sqlBuf,"select count(*) from icmNetArea ");
                   lCount =0;
                   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
                   if(tempCursor!=NULL){
                            tempRow= ltDbFetchRow(tempCursor);
                            if(tempRow!=NULL){
                               lCount=atol(tempRow[0]);
                            }
                            ltDbCloseCursor(tempCursor);
                   }
                   if(lCount == 0) {
                            lCount=1;
                   }else{
                            sprintf(sqlBuf,"select max(id)+1 from icmNetArea ");
                            lCount = 1;
                            tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
                            if(tempCursor!=NULL){
                                     tempRow= ltDbFetchRow(tempCursor);
                                     if(tempRow!=NULL){
                                               lCount=atol(tempRow[0]);
                                     }
                                     ltDbCloseCursor(tempCursor);
                            }
                   }        
                   sprintf(sqlBuf,"insert into icmNetArea(id,ipstart,ipend,netflag,ipdesp,snmpnum) values (%ld,'%s','%s',%s,'%s',%s)",
                   					lCount,
                            ltMsgGetVar_s(ltMsgPk,"bip"),
                            ltMsgGetVar_s(ltMsgPk,"eip"),
                            ltMsgGetVar_s(ltMsgPk,"netflag"),
                            "",
                            ltMsgGetVar_s(ltMsgPk,"snmpnum"));
					                  ltDbExecSql(G_DbCon,sqlBuf);
					                  snprintf(caMsg,255,"%s","update netarea success.");
			                      logWrite(ltMsgPk,caMsg,3);
         }else if (actiontype==2){/*del*/
                   long allnum,i;
                   char tmpVal[20];
                   allnum=atol(ltMsgGetVar_s(ltMsgPk,"allnum"));
                   for(i=0;i<allnum;i++){
                            sprintf(tmpVal,"checkbox%ld",i);
                            if(ltMsgGetVar_s(ltMsgPk,tmpVal)!=NULL){
                                     snprintf(sqlBuf,1024,"delete from icmNetArea where id =%s",ltMsgGetVar_s(ltMsgPk,tmpVal));                 
                                     ltDbExecSql(G_DbCon,sqlBuf); 
                            }
                   }
                   snprintf(caMsg,255,"%s","delete netarea success.");
			             logWrite(ltMsgPk,caMsg,3);
         }
         ltnetarealist(confd,ltMsgPk,lt_MMHead);
         return 0;
}
int ltnetarealist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
         char sqlBuf[1024];
         ltDbHeadPtr dbPtr;
         ltTablePtr tablePtr;
         ltDbCursor *tempCursor;
         LT_DBROW tempRow;
         char strDesc[64];
         char strDescutf8[64];
         char strSnmp[64];
         char strSnmputf8[64];
         long iIndex;
         memset(strSnmp,0,sizeof(strSnmp));
         dbPtr=lt_dbinit();
         lt_db_htmlpage(dbPtr,"utf-8");
         tablePtr=lt_dbput_table(dbPtr,"ipbindlist");
         sprintf(sqlBuf,"select id,ipstart,ipend,netflag,ipdesp,snmpnum from icmNetArea order by  netflag desc,ipstart desc;");
         iIndex=0;
         tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
         if(tempCursor!=NULL){
                   tempRow= ltDbFetchRow(tempCursor);
                   while(tempRow!=NULL){
                   	       memset(strSnmp,0,sizeof(strSnmp));
                   	       memset(strDesc,0,sizeof(strDesc));
                   	       memset(strSnmputf8,0,sizeof(strSnmputf8));
                   	       memset(strDescutf8,0,sizeof(strDescutf8));
                           if( atol(tempRow[5])== 0 ){
                                 sprintf(strSnmp,"%s","不启动snmp读取");
                           }else{
                                 sprintf(strSnmp,"启动第[%u]组snmp读取",atol(tempRow[5]));
                           }
     	                     strgb2utf8(strSnmp,strSnmputf8,strlen(strSnmp));

                           if( atol(tempRow[3])== 0 ){
                                 sprintf(strDesc,"%s","本网段不支持IP/MAC绑定");
                           }else if( atol(tempRow[3])== 1 ){
                                 sprintf(strDesc,"%s","本网段IP/MAC手工绑定");
                           }else if( atol(tempRow[3])== 2 ){
                                 sprintf(strDesc,"%s","本网段IP/MAC通过DHCP绑定");
                           }else if( atol(tempRow[3])== 99 ){
                                 sprintf(strDesc,"%s","不监控该网段");
                           }else if( atol(tempRow[3])== 100 ){
                                 sprintf(strDesc,"%s","禁止使用该网段");
                           }
                           strgb2utf8(strDesc,strDescutf8,strlen(strDesc));
                           lt_dbput_recordvars(tablePtr,6,
                                     "inum",LT_TYPE_LONG,iIndex,
                                     "listindex",LT_TYPE_STRING,tempRow[0],
                                     "bip",LT_TYPE_STRING,tempRow[1],
                                     "eip",LT_TYPE_STRING,tempRow[2],
                                     "listdesc",LT_TYPE_STRING,strDescutf8,
                                     "snmpnum",LT_TYPE_STRING,strSnmputf8);
                            tempRow= ltDbFetchRow(tempCursor);
                            iIndex++;
                   }
                   ltDbCloseCursor(tempCursor);
         }
         sprintf(strDesc,"%ld",iIndex);
         lt_dbput_rootvars(dbPtr,1,"allnum",strDesc);
         lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/icm/htmlplt/manager/netarea.htm",dbPtr->head,0);
         ltMsgFree(ltMsgPk);
         lt_dbfree(dbPtr);
         return 0;
}
///////////////////////
////////////////////////
int ltlanarealist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
         char sqlBuf[1024];
         ltDbHeadPtr dbPtr;
         ltTablePtr tablePtr;
         ltDbCursor *tempCursor;
         LT_DBROW tempRow;
         char strDesc[20];
         long iIndex;
         dbPtr=lt_dbinit();
         lt_db_htmlpage(dbPtr,"utf-8");
         tablePtr=lt_dbput_table(dbPtr,"ipbindlist");
  			 sprintf(sqlBuf,"select id,vlanid,ip,mask,dev from icmVLanArea order by  vlanid asc ");
         iIndex=0;
         tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
         if(tempCursor!=NULL){
                   tempRow= ltDbFetchRow(tempCursor);
                   while(tempRow!=NULL){
                            lt_dbput_recordvars(tablePtr,6,
                                    "inum",LT_TYPE_LONG,iIndex,
                                     "listindex",LT_TYPE_STRING,tempRow[0],
                                     "vid",LT_TYPE_STRING,tempRow[1],
                                     "ip",LT_TYPE_STRING,tempRow[2],
                                     "mask",LT_TYPE_STRING,tempRow[3],
                                     "dev",LT_TYPE_STRING,tempRow[4]);
                            tempRow= ltDbFetchRow(tempCursor);
                            iIndex++;
                   }
                   ltDbCloseCursor(tempCursor);
         }
         sprintf(strDesc,"%ld",iIndex);
         lt_dbput_rootvars(dbPtr,1,"allnum",strDesc);
         lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/icm/htmlplt/manager/naslanlist.htm",dbPtr->head,0);
         ltMsgFree(ltMsgPk);
         lt_dbfree(dbPtr);
         return 0;
}
int ltlanaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
         char sqlBuf[1024];
         ltDbCursor *tempCursor;
         LT_DBROW tempRow;
         int  actiontype;
         actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));
         if(actiontype==1){/*add*/
                   long lCount;
                   sprintf(sqlBuf,"select count(*) from icmVLanArea ");
                   lCount =0;
                   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
                   if(tempCursor!=NULL){
                            tempRow= ltDbFetchRow(tempCursor);
                            if(tempRow!=NULL){
                                     lCount=atol(tempRow[0]);
                            }
                            ltDbCloseCursor(tempCursor);
                   }
                   if(lCount == 0) {
                            lCount=1;
                   }else{
                            sprintf(sqlBuf,"select max(id)+1 from icmVLanArea ");
                            lCount = 1;
                            tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
                            if(tempCursor!=NULL){
                                     tempRow= ltDbFetchRow(tempCursor);
                                     if(tempRow!=NULL){
                                               lCount=atol(tempRow[0]);
                                     }
                                     ltDbCloseCursor(tempCursor);
                            }
                   }        
                   sprintf(sqlBuf,"insert into icmVLanArea values (%ld,%d,'%s','%s','%s')",
                            lCount,
                            atol(ltMsgGetVar_s(ltMsgPk,"vid")),
                            ltMsgGetVar_s(ltMsgPk,"ip"),
                            ltMsgGetVar_s(ltMsgPk,"mask"),ltMsgGetVar_s(ltMsgPk,"dev"));
                   					ltDbExecSql(G_DbCon,sqlBuf); 
                   }else if (actiontype==2){/*del*/
                   long allnum,i;
                   char tmpVal[20];
                   allnum=atol(ltMsgGetVar_s(ltMsgPk,"allnum"));
                   for(i=0;i<allnum;i++){
                            sprintf(tmpVal,"checkbox%ld",i);
                            if(ltMsgGetVar_s(ltMsgPk,tmpVal)!=NULL){
                                     snprintf(sqlBuf,1024,"delete from icmVLanArea where id =%s",ltMsgGetVar_s(ltMsgPk,tmpVal));              
                                     ltDbExecSql(G_DbCon,sqlBuf); 
                            }
                   }
         }
         ltlanarealist(confd,ltMsgPk,lt_MMHead);
         return 0;
}
////////////////////////////
/////////////////////////////
/////////////////////////////
int ltdhcpaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
         char sqlBuf[1024];
         ltDbCursor *tempCursor;
         LT_DBROW tempRow;
         int  actiontype;
         actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));
         if(actiontype==1){/*add*/
                   long lCount;
                   sprintf(sqlBuf,"select count(*) from icmDHCPGroup ");
                   lCount =0;
                   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
                   if(tempCursor!=NULL){
                            tempRow= ltDbFetchRow(tempCursor);
                            if(tempRow!=NULL){
                                     lCount=atol(tempRow[0]);
                            }
                            ltDbCloseCursor(tempCursor);
                   }
                   if(lCount == 0) {
                            lCount=1;
                   }else{
                            sprintf(sqlBuf,"select max(id)+1 from icmDHCPGroup ");
                            lCount = 1;
                            tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
                            if(tempCursor!=NULL){
                                     tempRow= ltDbFetchRow(tempCursor);
                                     if(tempRow!=NULL){
                                               lCount=atol(tempRow[0]);
                                     }
                                     ltDbCloseCursor(tempCursor);
                            }
                   }
                   if(ltMsgGetVar_s(ltMsgPk,"dnsip1")!=NULL && ltMsgGetVar_s(ltMsgPk,"dnsip2")!=NULL){        
                   sprintf(sqlBuf,"insert into icmDHCPGroup values (%ld,'%s','%s','%s','%s','%s','%s')",
                            lCount,
                            ltMsgGetVar_s(ltMsgPk,"groupname"),
                            ltMsgGetVar_s(ltMsgPk,"netmask"),
                            ltMsgGetVar_s(ltMsgPk,"routeip"),
                            ltMsgGetVar_s(ltMsgPk,"dnsip0"),
                            ltMsgGetVar_s(ltMsgPk,"dnsip1"),
                            ltMsgGetVar_s(ltMsgPk,"dnsip2"));
                   					ltDbExecSql(G_DbCon,sqlBuf);
                   				}else if(ltMsgGetVar_s(ltMsgPk,"dnsip1")==NULL && ltMsgGetVar_s(ltMsgPk,"dnsip2")!=NULL)
                   					{
                   					sprintf(sqlBuf,"insert into icmDHCPGroup values (%ld,'%s','%s','%s','%s','%s','%s')",
                            lCount,
                            ltMsgGetVar_s(ltMsgPk,"groupname"),
                            ltMsgGetVar_s(ltMsgPk,"netmask"),
                            ltMsgGetVar_s(ltMsgPk,"routeip"),
                            ltMsgGetVar_s(ltMsgPk,"dnsip0"),
                            "",
                            ltMsgGetVar_s(ltMsgPk,"dnsip2"));
                   					ltDbExecSql(G_DbCon,sqlBuf);
                   					} 
                   				 else if(ltMsgGetVar_s(ltMsgPk,"dnsip1")==NULL && ltMsgGetVar_s(ltMsgPk,"dnsip2")==NULL)
                   					{
                   					sprintf(sqlBuf,"insert into icmDHCPGroup values (%ld,'%s','%s','%s','%s','%s','%s')",
                            lCount,
                            ltMsgGetVar_s(ltMsgPk,"groupname"),
                            ltMsgGetVar_s(ltMsgPk,"netmask"),
                            ltMsgGetVar_s(ltMsgPk,"routeip"),
                            ltMsgGetVar_s(ltMsgPk,"dnsip0"),
                            "",
                            "");
                   					ltDbExecSql(G_DbCon,sqlBuf);
                   					}
                   				 else if(ltMsgGetVar_s(ltMsgPk,"dnsip1")!=NULL && ltMsgGetVar_s(ltMsgPk,"dnsip2")==NULL)
                   					{
                   					sprintf(sqlBuf,"insert into icmDHCPGroup values (%ld,'%s','%s','%s','%s','%s','%s')",
                            lCount,
                            ltMsgGetVar_s(ltMsgPk,"groupname"),
                            ltMsgGetVar_s(ltMsgPk,"netmask"),
                            ltMsgGetVar_s(ltMsgPk,"routeip"),
                            ltMsgGetVar_s(ltMsgPk,"dnsip0"),
                            ltMsgGetVar_s(ltMsgPk,"dnsip1"),
                            "");
                   					ltDbExecSql(G_DbCon,sqlBuf);
                   					} 
                   }else if (actiontype==2){/*del*/
                   long allnum,i;
                   char tmpVal[20];
                   allnum=atol(ltMsgGetVar_s(ltMsgPk,"allnum"));
                   for(i=0;i<allnum;i++){
                            sprintf(tmpVal,"checkbox%ld",i);
                            if(ltMsgGetVar_s(ltMsgPk,tmpVal)!=NULL){
                                     snprintf(sqlBuf,1024,"delete from icmDHCPGroup where id =%s",ltMsgGetVar_s(ltMsgPk,tmpVal));              
                                     ltDbExecSql(G_DbCon,sqlBuf); 
                            }
                   }
         }
         ltdhcparealist(confd,ltMsgPk,lt_MMHead);
         return 0;
}
int ltdhcparealist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
         char sqlBuf[1024];
         ltDbHeadPtr dbPtr;
         ltTablePtr tablePtr;
         ltDbCursor *tempCursor;
         LT_DBROW tempRow;
         char strDesc[20];
         long iIndex;
         dbPtr=lt_dbinit();
         lt_db_htmlpage(dbPtr,"utf-8");
         tablePtr=lt_dbput_table(dbPtr,"ipbindlist");
  			 sprintf(sqlBuf,"select id,groupname,netmask,routeip,dnsip0,dnsip1,dnsip2 from icmdhcpGroup order by  id asc ");
         iIndex=0;
         tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
         if(tempCursor!=NULL){
                   tempRow= ltDbFetchRow(tempCursor);
                   while(tempRow!=NULL){
                            lt_dbput_recordvars(tablePtr,8,
                                    "inum",LT_TYPE_LONG,iIndex,
                                     "listindex",LT_TYPE_STRING,tempRow[0],
                                     "groupname",LT_TYPE_STRING,tempRow[1],
                                     "netmask",LT_TYPE_STRING,tempRow[2],
                                     "routeip",LT_TYPE_STRING,tempRow[3],
                                     "dnsip0",LT_TYPE_STRING,tempRow[4],
                                     "dnsip1",LT_TYPE_STRING,tempRow[5],
                                     "dnsip2",LT_TYPE_STRING,tempRow[6]);
                            tempRow= ltDbFetchRow(tempCursor);
                            iIndex++;
                   }
                   ltDbCloseCursor(tempCursor);
         }
         sprintf(strDesc,"%ld",iIndex);
         lt_dbput_rootvars(dbPtr,1,"allnum",strDesc);
         lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/icm/htmlplt/manager/nasdhcplist.htm",dbPtr->head,0);
         ltMsgFree(ltMsgPk);
         lt_dbfree(dbPtr);
         return 0;
}


/*
*查询网口配置信息
*/
int icmVLanListAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
     char strBuf[1024];
     ltDbCursor *tempCursor;
     LT_DBROW tempRow;     
     char caTmpp[512];
     long lRowNum,lStartRec,lSumRec,lCount,lNum;
     int k;
     
     char *mywk;
     mywk=ltMsgGetVar_s(ltMsgPk,"mywk");
     	
	   char caWhere[512];
	   stralloc strTemp;
        
		memset(caWhere,0,sizeof(caWhere));
		sprintf(caWhere,"%s","1=1 ");
		
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
    if(strcmp(mywk,"-1")==0){
	  	sprintf(strBuf,"select count(*) from icmVLanArea where %s '",caWhere);
	  }else{
    	sprintf(strBuf,"select count(*) from icmVLanArea where %s and dev='%s'",caWhere,mywk);
    }
    
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
	  sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);
    k=0;
    if(strcmp(mywk,"-1")==0){
    	sprintf(strBuf,"select id,vlanid,ip,mask,dev,intercept from icmVLanArea where %s  limit %lu offset %lu ",caWhere,lRowNum,lStartRec);
	  }else{
	  	sprintf(strBuf,"select id,vlanid,ip,mask,dev,intercept from icmVLanArea where %s and dev='%s' limit %lu offset %lu ",caWhere,mywk,lRowNum,lStartRec);
	  }
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {                
          if(k==0)
					{
            sprintf(caTmpp,"{\"id\":\"%s\",\"vlanid\":\"%s\",\"ip\":\"%s\",\"mask\":\"%s\",\"dev\":\"%s\",\"intercept\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5]);
            stralloc_cats(&strTemp,caTmpp);     
					}
					else
					{
				    sprintf(caTmpp,",{\"id\":\"%s\",\"vlanid\":\"%s\",\"ip\":\"%s\",\"mask\":\"%s\",\"dev\":\"%s\",\"intercept\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5]);
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

/*
*配置新端口
*/
int icmAddVlanAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	   char *caVlanid;  //Vlanid
	   char *caIp;  //IP地址
	   char *caMask;  //子网掩码
	   char *caDev;//网口
	   char *caIntercept;//侦听
     char sqlBuf[2048];
	   int  maxId;
	   
	   char *mywk;
     mywk=ltMsgGetVar_s(ltMsgPk,"Mywk");
	   
	   ltDbCursor *tempCursor;
	   LT_DBROW tempRow; 
	   maxId=1;
	   sprintf(sqlBuf,"select max(id)+1 from icmVLanArea" );	  
	   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);   
	     if(tempCursor!=NULL){
	          tempRow= ltDbFetchRow(tempCursor);
	          if(tempRow!=NULL){
				      maxId=atol(tempRow[0]);				  
	          }else
				   	{
				     maxId=1;
				   	}
	     ltDbCloseCursor(tempCursor);
	   }
    
    
	  char caStr[2048];
	  
    ltDbHeadPtr dbPtr;
    dbPtr=lt_dbinit();
    
	 caVlanid=ltMsgGetVar_s(ltMsgPk,"vlanid");
     if(!caVlanid){
         caVlanid="";
	 }
    caIp=ltMsgGetVar_s(ltMsgPk,"ip");
     if(!caIp){
         caIp="";
	 }
    caMask=ltMsgGetVar_s(ltMsgPk,"mask");
     if(!caMask){
         caMask="";
	 }
    caDev=mywk;
    caIntercept=ltMsgGetVar_s(ltMsgPk,"intercept");
     if(!caIntercept){
         caIntercept="";
	 }   
     sprintf(sqlBuf,"insert into icmVLanArea(id,vlanid,ip,mask,dev,intercept)values (%d,'%s','%s','%s','%s','%s')",maxId,caVlanid,caIp,caMask,caDev,caIntercept);
	   ltDbExecSql(G_DbCon,sqlBuf); 
     memset(caStr,0,sizeof(caStr));
     lt_dbput_rootvars(dbPtr,1,"dev",mywk);
  	 ltMsgPk->msgpktype=1;
     lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");   
	   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/icm/htmlplt/manager/page.htm",dbPtr->head,0);
     ltMsgFree(ltMsgPk);
     ltDbCloseCursor(tempCursor);
    return 0;
}

/*
*网口信息删除函数
*/
int icmDeleteAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    int cadeflag;
		char strSql[2048];
		if(ltMsgGetVar_s(ltMsgPk,"deflag")!=NULL)
		{
			cadeflag=atol(ltMsgGetVar_s(ltMsgPk,"deflag"));
			if(cadeflag==1)
			{
				 char *myId;
				 myId=ltMsgGetVar_s(ltMsgPk,"delId");
				 sprintf(strSql,"delete from icmVLanArea where id='%s'",myId);
				 ltDbExecSql(G_DbCon,strSql); 
			}
  	  ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");
	    ltMsgFree(ltMsgPk);
	    return 0;
      }
}

/*
*数据清空函数
*/
int icmCleanAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		  int cadeflag;
		  char strSql[2048];		
		  char caTempDir[256];
			char caLabel[256];
			 
			 ltMsgPk->msgpktype=1;
		   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");
	     ltMsgFree(ltMsgPk);
			 
		  if(ltMsgGetVar_s(ltMsgPk,"deflag")!=NULL)
			{
				cadeflag=atol(ltMsgGetVar_s(ltMsgPk,"deflag"));
				if(cadeflag==0)
				{ 
						sprintf(caTempDir,"/datacenter/tmp/log.ini");	
						
						sprintf(strSql,"truncate table icmAdminArea");
						ltDbExecSql(G_DbCon,strSql);
						sprintf(caLabel,"/bin/echo '1' > %s",caTempDir);
					  system(caLabel);/*建立目录标志*/
					  sleep(1); 
					  
					  sprintf(strSql,"delete from Icmadmuser where name<>'admin'");
					  ltDbExecSql(G_DbCon,strSql);
						sprintf(caLabel,"/bin/echo '2' > %s",caTempDir);
					  system(caLabel);/*建立目录标志*/
					  sleep(1);
					     
					  sprintf(strSql,"truncate table Icmadmlog");
					  ltDbExecSql(G_DbCon,strSql);
						sprintf(caLabel,"/bin/echo '3' > %s",caTempDir);
					  system(caLabel);/*建立目录标志*/
					  sleep(1);  
					   
					  sprintf(strSql,"truncate table icmGroup"); 
					  ltDbExecSql(G_DbCon,strSql);
					  sprintf(strSql,"insert into icmGroup(id,pid,name,mydesc,level) values(0,0,'DefaultGroup','',0)"); 
					  ltDbExecSql(G_DbCon,strSql);
						sprintf(caLabel,"/bin/echo '4' > %s",caTempDir);
					  system(caLabel);/*建立目录标志*/
					  sleep(1);
					     
					  sprintf(strSql,"truncate table icmDHCPGroup"); 
					  ltDbExecSql(G_DbCon,strSql);
						sprintf(caLabel,"/bin/echo '5' > %s",caTempDir);
					  system(caLabel);/*建立目录标志*/
					  sleep(1);  
					   
					  sprintf(strSql,"truncate table icmuserinfo"); 
					  ltDbExecSql(G_DbCon,strSql);
						sprintf(caLabel,"/bin/echo '6' > %s",caTempDir);
					  system(caLabel);/*建立目录标志*/
					  sleep(1);   
					  
					  sprintf(strSql,"truncate table icmNetArea");
					  ltDbExecSql(G_DbCon,strSql);
					  sprintf(strSql,"insert into icmNetArea(id,lipstart,ipstart,lipend,ipend,netflag,ipdesp,snmpnum) values(1,0,'192.168.0.87',0,'192.168.0.88',0,'',0)"); 
					  ltDbExecSql(G_DbCon,strSql);
						sprintf(caLabel,"/bin/echo '7' > %s",caTempDir);
					  system(caLabel);/*建立目录标志*/
					  sleep(1);  
					  
					  sprintf(strSql,"truncate table icmVLanArea");
					  ltDbExecSql(G_DbCon,strSql);
						sprintf(caLabel,"/bin/echo '8' > %s",caTempDir);
					  system(caLabel);/*建立目录标志*/
					  sleep(1); 
					   
					  sprintf(strSql,"truncate table icmsysinfolog");
					  ltDbExecSql(G_DbCon,strSql);
						sprintf(caLabel,"/bin/echo '9' > %s",caTempDir);
					  system(caLabel);/*建立目录标志*/
					  sleep(1);  
					  
					  sprintf(strSql,"truncate table ClientList");
					  ltDbExecSql(G_DbCon,strSql);
						sprintf(caLabel,"/bin/echo '10' > %s",caTempDir);
					  system(caLabel);/*建立目录标志*/
					  sleep(1);  
					  
					  sprintf(strSql,"truncate table Arpreport");
					  ltDbExecSql(G_DbCon,strSql);
						sprintf(caLabel,"/bin/echo '11' > %s",caTempDir);
					  system(caLabel);/*建立目录标志*/
					  sleep(1);
					  
					  sprintf(caLabel,"/bin/echo '0' > %s",caTempDir);
					  system(caLabel);/*建立目录标志*/
					  sleep(1);
				 } 
			 
	    }
			 //删除临时文件
       chdir("/datacenter/tmp/");
	     system("rm -f log.ini");
		return 0;  
}
int icmStatusAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char   flag[256];
    FILE *fd=NULL;
		char str[1024];
		fd=fopen("/datacenter/tmp/log.ini","r");
		fgets(str,256,fd);
		sprintf(flag,"%s",str);
		fclose(fd);
    if((atol(flag))==0)
    {
  	 ltMsgPk->msgpktype=1;
		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");
	   ltMsgFree(ltMsgPk);
	   return 0;
    }
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,str);
   ltMsgFree(ltMsgPk);
   
}

	
	
	  
