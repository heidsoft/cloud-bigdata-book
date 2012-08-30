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
#include "msa.h"

/*
*在线管理员
*/
int msaAdminUserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	checkTimeOut(confd,ltMsgPk,lt_MMHead);
	int i,rightype;
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
  rightype=0;

  char *dbUser;
	char *dbPass;
	char *dbName;
	dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"db error!");
			ltMsgFree(ltMsgPk);
			return 0;
	}
  strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
  if(!strUserName){
  	strUserName="";
  }
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	tablePtr=lt_dbput_table(dbPtr,"list");
  sprintf(sqlBuf,"select name,lasttime,roleid from nasadmuser");
	inum=0;
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			inum++;
			nasTimeFormat(caTime,atol(tempRow[2]));
			lt_dbput_recordvars(tablePtr,3,"num",LT_TYPE_LONG,inum,"logname",LT_TYPE_STRING,tempRow[0],"lasttime",LT_TYPE_STRING,caTime);
			if(strcmp(strUserName,tempRow[0])==0){
				lt_dbput_rootvars(dbPtr,1,"logname",tempRow[0]);
				lt_dbput_rootvars(dbPtr,1,"roleid",tempRow[2]);
				memset(caRight,0,sizeof(caRight));
				sprintf(caRight,"%s",tempRow[1]);
				lt_dbput_rootvars(dbPtr,1,"userright",caRight);
				for(i=0;i<9;i++){
					sprintf(caSel,"c%d",i+1);
					if(caRight[i]=='1'){
						lt_dbput_rootvars(dbPtr,1,caSel,"true");
					}else{
						lt_dbput_rootvars(dbPtr,1,caSel,"false");
					}
				}
			}
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
		if(ltMsgGetVar_s(ltMsgPk,"rightype")!=NULL){
		rightype=atol(ltMsgGetVar_s(ltMsgPk,"rightype"));
		}
		if(rightype==0){
			lt_dbput_rootvars(dbPtr,8,
                			 "c1","false",
                       "c2","false",
                       "c3","false",
                       "c4","false",
                       "c5","false",
                       "c6","false",
                       "c7","false",
                       "c8","false"
                       );
    	lt_dbput_rootvars(dbPtr,1,"grouptype","1");
    }else {
    	lt_dbput_rootvars(dbPtr,1,"grouptype","3");
    }
		lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/adminuser.htm",dbPtr->head,0);
		ltMsgFree(ltMsgPk);
		lt_dbfree(dbPtr);
		ltDbClose(G_DbCon);
		return 0;
 	}
 ltDbClose(G_DbCon);
 return 0;
}
/*
*管理员操作
*/
int msaAdminAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char sqlBuf[2048];
	char *strUserName;
	char *strPassword;
  char newPass[50];

	char sIp[24];
	char caMsg[256];
	int  passisok;
  int  actiontype;

  char *dbUser;
	char *dbPass;
	char *dbName;
	dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"db error!");
			ltMsgFree(ltMsgPk);
			return 0;
	}
  //ltDbClose(G_DbCon);

  actiontype=0;
	passisok=0;
	memset(sIp,0,sizeof(sIp));
	memset(newPass,0,sizeof(newPass));
	int roleid;
	actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));
	strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
	strPassword=ltMsgGetVar_s(ltMsgPk,"pass");
	if(strcmp((ltMsgGetVar_s(ltMsgPk,"roleid")),"")==0){
			roleid=0;
	}else {
			roleid=atoi(ltMsgGetVar_s(ltMsgPk,"roleid"));
	}
	if(actiontype==1){/*add*/
			char utf8str[512];
			memset(utf8str,0,sizeof(utf8str));
			passisok=ltPassIsOk(strPassword);
			if(passisok==0){
				ltMsgPk->msgpktype=1;
				ltWebMsgErr(_ltPubInfo->pubMsgStr[8].conStr, confd,ltMsgPk);
				ltMsgFree(ltMsgPk);
				return 0;
			}
			ltMd5Encrypto(strPassword, strlen(strPassword),newPass);
   			sprintf(sqlBuf,"insert into nasadmuser(name,password,lasttime,roleid)values ('%s','%s',%ld,%d)",
					strUserName,
					newPass,
					time(0),
					roleid);
			printf("%s\n",sqlBuf);
			ltDbExecSql(G_DbCon,sqlBuf);
//			sprintf(caMsg,"add new user %s success.",strUserName);
//			logWrite(ltMsgPk,caMsg,5);
			//写日志
			int lTime;
			char caDate[64];
			lTime = time(0);
			strcpy(caDate,ltTimeFormat("%Y-%m-%d %H:%M:%S",lTime));
			sprintf(caMsg,"insert into msaadmlog values('%s','%s','add new %s success',5)",strUserName,caDate,strUserName);
			printf("%s\n",caMsg);
			ltDbExecSql(G_DbCon,caMsg);
			memset(caMsg,0,sizeof(caMsg));
			sprintf(caMsg,"添加管理员%s成功添加，如有疑问请联系管理员。",strUserName);
			strgb2utf8(caMsg,utf8str,strlen(caMsg));
			alertWrite(ltMsgPk,utf8str,1);
	}else if(actiontype==2){/*del*/
			sprintf(sqlBuf,"delete from nasadmuser where name='%s' ",	strUserName);
			ltDbExecSql(G_DbCon,sqlBuf);
			//写日志
			int lTime;
			char caDate[64];
			lTime = time(0);
			strcpy(caDate,ltTimeFormat("%Y-%m-%d %H:%M:%S",lTime));
			sprintf(caMsg,"insert into msaadmlog values('%s','%s','delete %s success',5)",strUserName,caDate,strUserName);
			printf("%s\n",caMsg);
			ltDbExecSql(G_DbCon,caMsg);
	}else if(actiontype==3){/*change power*/

	}else if(actiontype==4){/*change password indfo*/
			memset(newPass,0,sizeof(newPass));
			passisok=0;
		  passisok=ltPassIsOk(strPassword);
			if(passisok==0){
				ltMsgPk->msgpktype=1;
				ltWebMsgErr(_ltPubInfo->pubMsgStr[8].conStr,confd,ltMsgPk);
				ltMsgFree(ltMsgPk);
				return 0;
			}
			ltMd5Encrypto(strPassword, strlen(strPassword),newPass);
			sprintf(sqlBuf,"update nasadmuser set password='%s' where name='%s' ",newPass,strUserName);
			ltDbExecSql(G_DbCon,sqlBuf);
			sprintf(caMsg,"change %s password success.",strUserName);
			logWrite(ltMsgPk,caMsg,2);
	}else if(actiontype==5){/*change ip*/
//			sprintf(sqlBuf,"update nasadmuser set bindip='%s'where name='%s' ",bindip,strUserName);
//		  ltDbExecSql(G_DbCon,sqlBuf);
//			sprintf(caMsg,"update %s ip success.",strUserName);
//			logWrite(ltMsgPk,caMsg,2);
	}else if(actiontype==6){/*change group*/
//		    memset(g1,0,sizeof(g1));
//			  memset(g2,0,sizeof(g2));
//			  memset(g3,0,sizeof(g3));
//			  memset(g4,0,sizeof(g4));
//				memset(g5,0,sizeof(g5));
//				memcpy(g1,strGroupManager,200);
//				memcpy(g2,strGroupManager+200,200);
//				memcpy(g3,strGroupManager+400,200);
//				memcpy(g4,strGroupManager+600,200);
//				memcpy(g5,strGroupManager+800,200);
//
//			sprintf(sqlBuf,"update nasadmuser set grouplist1='%s',grouplist2='%s',grouplist3='%s',grouplist4='%s',grouplist5='%s' where name='%s' ",g1,g2,g3,g4,g5,strUserName);
//			ltDbExecSql(G_DbCon,sqlBuf);
//			sprintf(caMsg,"update %s group success.",strUserName);
//			logWrite(ltMsgPk,caMsg,2);
	}else if(actiontype==7){
//		  memset(newPass,0,sizeof(newPass));
//			passisok=0;
//		  passisok=ltPassIsOk(strPassword);
//			if(passisok==0){
//				ltMsgPk->msgpktype=1;
//				ltWebMsgErr(_ltPubInfo->pubMsgStr[8].conStr,confd,ltMsgPk);
//				ltMsgFree(ltMsgPk);
//				return 0;
//			}
//			ltMd5Encrypto(strPassword, strlen(strPassword),newPass);
//			sprintf(sqlBuf,"update msaadmuser set password='%s',bindip='%s' where name='%s' ",newPass,bindip,strUserName);
//			ltDbExecSql(G_DbCon,sqlBuf);
//			sprintf(caMsg,"change %s password and ip success.",strUserName);
//			logWrite(ltMsgPk,caMsg,2);
	}
	ltDbClose(G_DbCon);
	msaAdminUserList(confd,ltMsgPk,lt_MMHead);
	return 0;
}

/*
*在线管理员列表
*/
int msaAdmuserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 checkTimeOut(confd,ltMsgPk,lt_MMHead);
   stralloc strTemp;
   int  allcount;
   int  i,k;
   char caTmpp[512],strip[128],xStatus[32];
	 unsigned int bindip[4];
	 bindip[0]=0;
	 bindip[1]=0;
	 bindip[2]=0;
	 bindip[3]=0;
   allcount=0;
   for(i=0;i<MAX_ADMIN_SESSION;i++){
	  	if( (_ltPubInfo->admSession[i].lSip[0]>0||_ltPubInfo->admSession[i].lSip[1]>0||_ltPubInfo->admSession[i].lSip[2]>0||_ltPubInfo->admSession[i].lSip[3]>0) && ((time(0)-_ltPubInfo->admSession[i].lasttime)<7200)  ){
	  			allcount++;
	  	}
   }
   strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
   k=0;

	 struct in_addr s;
   for(i=0;i<MAX_ADMIN_SESSION;i++){
  	if((_ltPubInfo->admSession[i].lSip[0]>0||_ltPubInfo->admSession[i].lSip[1]>0||_ltPubInfo->admSession[i].lSip[2]>0||_ltPubInfo->admSession[i].lSip[3]>0) && ((time(0)-_ltPubInfo->admSession[i].lasttime) < 7200)){
				
				memset(strip,0,sizeof(strip));
				if(_ltPubInfo->admSession[i].lSip[0]==0&&_ltPubInfo->admSession[i].lSip[1]==0&&_ltPubInfo->admSession[i].lSip[2]==0 ){//ipv4
			    s.s_addr = htonl(_ltPubInfo->admSession[i].lSip[3]);
			    sprintf(strip,"%s",inet_ntoa(s));
			  }else{//ipv6
			  	memset(bindip,0,sizeof(int)*4);
			    bindip[0]=htonl(_ltPubInfo->admSession[i].lSip[0]);
			    bindip[1]=htonl(_ltPubInfo->admSession[i].lSip[1]);
			    bindip[2]=htonl(_ltPubInfo->admSession[i].lSip[2]);
			    bindip[3]=htonl(_ltPubInfo->admSession[i].lSip[3]);
			    inet_ntop(AF_INET6, (void *)(bindip), strip, 64);
			  }
  			memset(xStatus,0,sizeof(xStatus));
  			if(_ltPubInfo->admSession[i].lSid<=1){
		  		sprintf(xStatus,"%s","Trying");
		  	}else{
		  		sprintf(xStatus,"%s","Success");
		    }
  			if(k==0){
	     	  	 sprintf(caTmpp,"{\"xindex\":\"%d\",\"ip\":\"%s\",\"lasttime\":\"%s\",\"trytime\":\"%d\",\"status\":\"%s\"}",
	     	  	 i,strip,ltTimeFormat("%Y-%m-%d %H:%M:%S",_ltPubInfo->admSession[i].lasttime),_ltPubInfo->admSession[i].trytimes,xStatus );
	     	     stralloc_cats(&strTemp,caTmpp);
	     	}else{
	     	  	 sprintf(caTmpp,",{\"xindex\":\"%d\",\"ip\":\"%s\",\"lasttime\":\"%s\",\"trytime\":\"%d\",\"status\":\"%s\"}",
	     	  	 i,strip,ltTimeFormat("%Y-%m-%d %H:%M:%S",_ltPubInfo->admSession[i].lasttime),_ltPubInfo->admSession[i].trytimes,xStatus );
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
//强制下线
int msaCheckOutAdmin(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	checkTimeOut(confd,ltMsgPk,lt_MMHead);
	char sIp[128];
	memset(sIp,0,sizeof(sIp));
	sprintf(sIp,"%s",ltMsgGetVar_s(ltMsgPk,"ip"));
	unsigned int bindip[4];
	bindip[0]=0;
	bindip[1]=0;
	bindip[2]=0;
	bindip[3]=0;
	//判断地址格式 包括.是ipv4地址  :ipv6地址
	memset(bindip,0,sizeof(int)*4);
	char  *_ipaddr;
	char  _ch;
	int   ipv4flag;
	ipv4flag=0;
	_ipaddr=sIp;
	while ((_ch = *_ipaddr++) != '\0') {
		if(_ch == '.'){
			ipv4flag=1;
			break;
		}
	}
  if(ipv4flag==1){
  	_ipaddr=sIp;
		bindip[3]=ntohl(inet_addr(sIp));
	}else{
		_ipaddr=sIp;
		inet_pton(AF_INET6, _ipaddr, (void *)(bindip));  //实际网络顺序问题
		bindip[0]=ntohl(bindip[0]);
		bindip[1]=ntohl(bindip[1]);
		bindip[2]=ntohl(bindip[2]);
		bindip[3]=ntohl(bindip[3]);
	}
	int i=0;
	for(i=0;i<MAX_ADMIN_SESSION;i++){
  	if(_ltPubInfo->admSession[i].lSip[0]==bindip[0]&&_ltPubInfo->admSession[i].lSip[1]==bindip[1]&&_ltPubInfo->admSession[i].lSip[2]==bindip[2]&&_ltPubInfo->admSession[i].lSip[3]==bindip[3]){
          _ltPubInfo->admSession[i].lSip[0]=0;
          _ltPubInfo->admSession[i].lSip[1]=0;
          _ltPubInfo->admSession[i].lSip[2]=0;
          _ltPubInfo->admSession[i].lSip[3]=0;
  			  _ltPubInfo->admSession[i].lasttime=0;
  				_ltPubInfo->admSession[i].trytimes=0;
  		    _ltPubInfo->admSession[i].lSid=0;
  		    sprintf(_ltPubInfo->admSession[i].lright,"%s","0000000000000000000000000000000000000000000");
  	}
  }
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
	ltMsgFree(ltMsgPk);
	return 0;
}
/*
*管理员操作日志
*/
int msaAdmlog(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
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
    char *dbUser;
		char *dbPass;
		char *dbName;
		dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"db error!");
				ltMsgFree(ltMsgPk);
				return 0;
		}
	  //ltDbClose(G_DbCon);

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
       sprintf(caFileName,"admlogexport-%ld.csv",time(0));
	     k=0;
	     ltMsgPk->msgpktype=2;
			 lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,caFileName,"contenttype","Application/MS-DOWNLOAD");
		   sprintf(strBuf,"select name,sdate,loglevel,subject from msaadmlog where %s  order by sdate desc",caWhere);
		   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		   if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
				  while(tempRow!=NULL){
			     	sprintf(caTmpp,"%s,%s,%s,%s\n",tempRow[0],tempRow[1],tempRow[2],tempRow[3]);
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
    sprintf(strBuf,"select count(*) from msaadmlog where %s ",caWhere);

		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					lCount=atol(tempRow[0]);
				}
		}
		ltDbCloseCursor(tempCursor);

    strTemp.s=0;
    sprintf(caTmpp,"{\"totalCount\":\"%lu\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);

   k=0;
   sprintf(strBuf,"select name,sdate,loglevel,subject from msaadmlog where %s order by sdate desc limit %lu offset %lu ",caWhere,lRowNum,lStartRec);
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
   ltDbClose(G_DbCon);
	 ltMsgFree(ltMsgPk);
	 return 0;

}
/*
*设备重启
*/
int msaSysReset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char sIp[24];
	  char strUserName[64];
	  char caMsg[256];
	  memset(sIp,0,sizeof(sIp));
	  memset(strUserName,0,sizeof(strUserName));

    ltMsgGetSomeNVar(ltMsgPk,2,"clientip", LT_TYPE_STRING, 19, sIp,"manageruser",LT_TYPE_STRING, 32,strUserName);

    sprintf(caMsg,"%s reboot system success.",strUserName);
	  if(G_DbCon){
		  logWrite(ltMsgPk,caMsg,9);
		  ltDbClose(G_DbCon);
	  }
    system("/sbin/reboot -f");
    return 0;
}
/*
*设备关机
*/
int msaSysShutdown(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char sIp[24];
	  char strUserName[64];
	  char caMsg[256];
	  memset(sIp,0,sizeof(sIp));
	  memset(strUserName,0,sizeof(strUserName));

    ltMsgGetSomeNVar(ltMsgPk,2,"clientip", LT_TYPE_STRING, 19, sIp,"manageruser",LT_TYPE_STRING, 32,strUserName);

    sprintf(caMsg,"%s shutdown system success.",strUserName);
	  if(G_DbCon){
		  logWrite(ltMsgPk,caMsg,9);
		  ltDbClose(G_DbCon);
	  }
    system("/sbin/poweroff");
    system("/sbin/init 0");
    return 0;
}

int ltnetareaaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char *caipstart;
    char *caipend;
    char *canetflag;
    char *casnmpnum;
    char *canohashch;
    char *cahomeurl;
    char *cagroupid;
    char *capolicyid;

	  char caStr[2048];
    ltDbHeadPtr dbPtr;
    dbPtr=lt_dbinit();
    char sqlBuf[2048];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char caMsg[256];
    long lCount;
    char *caipdesp;

    char *dbUser;
		char *dbPass;
		char *dbName;
		dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
				ltMsgFree(ltMsgPk);
				return 0;
		}
	  //ltDbClose(G_DbCon);

    sprintf(sqlBuf,"select count(*) from nasNetArea ");
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
            sprintf(sqlBuf,"select max(id)+1 from nasNetArea");
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
		caipstart=ltMsgGetVar_s(ltMsgPk,"ipstart");
	     if(!caipstart){
	         caipstart="";
		 }
		caipend=ltMsgGetVar_s(ltMsgPk,"ipend");
	     if(!caipend){
	         caipend="";
		 }
		canetflag=ltMsgGetVar_s(ltMsgPk,"netflag");
	     if(!canetflag){
	         canetflag="";
		 }
		casnmpnum=ltMsgGetVar_s(ltMsgPk,"snmpnum");
	     if(!casnmpnum){
	         casnmpnum="";
		 }
		casnmpnum=ltMsgGetVar_s(ltMsgPk,"snmpnum");
	     if(!casnmpnum){
	         casnmpnum="";
		 }
		canohashch=ltMsgGetVar_s(ltMsgPk,"nohashcheck");
	     if(!canohashch){
	         canohashch="";
		 }
		cahomeurl=ltMsgGetVar_s(ltMsgPk,"homeurl");
	     if(!cahomeurl){
	         cahomeurl="";
		 }
		cagroupid=ltMsgGetVar_s(ltMsgPk,"groupid");
	     if(!cagroupid){
	         cagroupid="";
		 }
		capolicyid=ltMsgGetVar_s(ltMsgPk,"policyid");
	     if(!capolicyid){
	         capolicyid="";
		 }
		 caipdesp=ltMsgGetVar_s(ltMsgPk,"ipdesp");
		 if(!caipdesp){
	         caipdesp="";
		 }

   sprintf(sqlBuf,"insert into nasNetArea(id,ipstart,ipend,netflag,ipdesp,snmpnum,\
 nohashcheck,homeurl,groupid,policyid)values\
 (%ld,'%s','%s',%ld,'%s',%s,%ld,'%s',%ld,%ld)",lCount,caipstart,caipend,atol(canetflag),caipdesp,casnmpnum,atol(canohashch),cahomeurl,atol(cagroupid),atol(capolicyid));
   ltDbExecSql(G_DbCon,sqlBuf);
printf("sqlBuf:%s\n",sqlBuf);
	 snprintf(caMsg,255,"%s","add nasNetArea success.");
	 logWrite(ltMsgPk,caMsg,3);
   memset(caStr,0,sizeof(caStr));
   ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");
   ltMsgFree(ltMsgPk);
   ltDbClose(G_DbCon);
   return 0;
}

/*实现读取、删除功能*/
int ltnetarealist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
     char strBuf[1024];
     ltDbCursor *tempCursor;
     LT_DBROW tempRow;
     char caTmpp[512];
     long lRowNum,lStartRec,lSumRec,lCount,lNum;
     int k;

	   stralloc strTemp;
		 ltDbHeadPtr dbPtr;

     char caMsg[256];


     char *dbUser;
		char *dbPass;
		char *dbName;
		dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
				ltMsgFree(ltMsgPk);
				return 0;
		}

     dbPtr=lt_dbinit();
     lt_db_htmlpage(dbPtr,"utf-8");
    //根据传的参数判断删除以及执行删除
	  int cadeflag;
		char sqlBuf[2048];
		if(ltMsgGetVar_s(ltMsgPk,"deflag")!=NULL)
		{
			cadeflag=atol(ltMsgGetVar_s(ltMsgPk,"deflag"));
			if(cadeflag==1)
			{
				 char *my_id;
				 my_id=ltMsgGetVar_s(ltMsgPk,"del_id");
				 sprintf(sqlBuf,"delete from nasnetArea where id='%s'",my_id);
				 ltDbExecSql(G_DbCon,sqlBuf);
				 snprintf(caMsg,255,"%s","delete nasNetArea success.");
				 logWrite(ltMsgPk,caMsg,3);
			}
			 ltMsgPk->msgpktype=1;
		   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
		   ltMsgFree(ltMsgPk);
		   return 0;
		}


    lRowNum=100; /*每页的行数*/
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
    sprintf(strBuf,"select count(*) from nasNetArea");
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
    sprintf(strBuf,"select id,ipstart,ipend,netflag,ipdesp,snmpnum,nohashcheck,homeurl,groupid,policyid from nasNetArea limit %lu offset %lu ",lRowNum,lStartRec);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {
          if(k==0)
					{
            sprintf(caTmpp,"{\"id\":\"%s\",\"ipstart\":\"%s\",\"ipend\":\"%s\",\"netflag\":\"%s\",\"snmpnum\":\"%s\",\"homeurl\":\"%s\",\"groupname\":\"%s\",\"policyname\":\"%s\",\"ipdesp\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[5],tempRow[7],_ltPubInfo->_GroupList[atoi(tempRow[8])].name,_ltPubInfo->_PolicyList[atoi(tempRow[9])].pname,tempRow[4]);
            stralloc_cats(&strTemp,caTmpp);
					}
					else
					{
				    sprintf(caTmpp,",{\"id\":\"%s\",\"ipstart\":\"%s\",\"ipend\":\"%s\",\"netflag\":\"%s\",\"snmpnum\":\"%s\",\"homeurl\":\"%s\",\"groupname\":\"%s\",\"policyname\":\"%s\",\"ipdesp\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[5],tempRow[7],_ltPubInfo->_GroupList[atoi(tempRow[8])].name,_ltPubInfo->_PolicyList[atoi(tempRow[9])].pname,tempRow[4]);
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
   ltDbClose(G_DbCon);
   return 0;
}


int ltapplynetArea(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){

	nasInitIpHostInfo(_ltPubInfo);
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");
	ltMsgFree(ltMsgPk);
	return 0;

}

/*
port info
*/
int ltportinfolist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){

	char sqlBuf[1024];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	char strDesc[30];
	char myDesc[32];
	char myLocal[32];
	char strProt[20];
	char strLocal[30];
	int iNum;
	char sNum[20];

  char *dbUser;
	char *dbPass;
	char *dbName;
	dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"db error!");
				ltMsgFree(ltMsgPk);
				return 0;
	}
	//ltDbClose(G_DbCon);

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	tablePtr=lt_dbput_table(dbPtr,"portinfolist");
	iNum=0;

  sprintf(sqlBuf,"select id,lPortStart,lPortEnd,lFlag,lProp,isLocal from nasPortInfo order by lPortStart asc,lFlag desc");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			iNum++;
				if(atol(tempRow[3])==10){
					sprintf(myDesc,"%s","禁止访问");//禁止访问
					strgb2utf8(myDesc,strDesc,strlen(myDesc));
				}else if(atol(tempRow[3])==9){
					sprintf(myDesc,"%s","免于监控");//免于监控
					strgb2utf8(myDesc,strDesc,strlen(myDesc));
				}else if(atol(tempRow[3])==3){
					sprintf(myDesc,"%s","进行监控");//进行监控
					strgb2utf8(myDesc,strDesc,strlen(myDesc));
				}else if(atol(tempRow[3])==2){
					sprintf(myDesc,"%s","监控并记录");//监控并记录
					strgb2utf8(myDesc,strDesc,strlen(myDesc));
				}
				sprintf(strProt,"%s","unknown");
				if(atol(tempRow[4])==1){
					sprintf(strProt,"%s","TCP");
				}else if(atol(tempRow[4])==2){
					sprintf(strProt,"%s","UDP");
				}

				if(atol(tempRow[5])==0){
					sprintf(myLocal,"%s","远端端口");//远端端口
					strgb2utf8(myLocal,strLocal,strlen(myLocal));
				}else if(atol(tempRow[5])==1){
					sprintf(myLocal,"%s","本地端口");//本地端口
					strgb2utf8(myLocal,strLocal,strlen(myLocal));
				}else if(atol(tempRow[5])==2){
					sprintf(myLocal,"%s","双向端口");//双向端口
					strgb2utf8(myLocal,strLocal,strlen(myLocal));
				}



			lt_dbput_recordvars(tablePtr,7,
				"num",LT_TYPE_LONG,iNum,
				"listindex",LT_TYPE_STRING,tempRow[0],
				"bport",LT_TYPE_STRING,tempRow[1],
				"eport",LT_TYPE_STRING,tempRow[2],
				"listdesc",LT_TYPE_STRING,strDesc,
				"listprot",LT_TYPE_STRING,strProt,
				"strlocal",LT_TYPE_STRING,strLocal);
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	sprintf(sNum,"%d",iNum);
	lt_dbput_rootvars(dbPtr,1,"allnum",sNum);
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/portinfo.htm",dbPtr->head,0);

  ltDbClose(G_DbCon);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}
int ltportinfoaction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		char sqlBuf[1024];
		int  actiontype;
		ltDbCursor *tempCursor;
		LT_DBROW tempRow;
	  char *dbUser;
		char *dbPass;
		char *dbName;
		dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"db error!");
				ltMsgFree(ltMsgPk);
				return 0;
		}
	//ltDbClose(G_DbCon);
		actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));
		if(actiontype==1){/*add*/
				long lCount=0;
				sprintf(sqlBuf,"select count(*) from nasPortInfo ");
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
		      sprintf(sqlBuf,"select max(id)+1 from nasPortInfo");
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
				sprintf(sqlBuf,"insert into nasPortInfo values (%ld,%s,%s,%s,%s,%s)",lCount,
					ltMsgGetVar_s(ltMsgPk,"bport"),
					ltMsgGetVar_s(ltMsgPk,"eport"),
					ltMsgGetVar_s(ltMsgPk,"netflag"),
					ltMsgGetVar_s(ltMsgPk,"netprot"),
					ltMsgGetVar_s(ltMsgPk,"islocal"));
				ltDbExecSql(G_DbCon,sqlBuf);
	  }else if (actiontype==2){/*del*/
	        int allNum,i;
	        char sNum[30];
	        allNum=atol(ltMsgGetVar_s(ltMsgPk,"allnum"));
	        for(i=1;i<allNum+1;i++){
	        	sprintf(sNum,"checkbox%d",i);
						sprintf(sqlBuf,"delete from nasPortInfo where id = %s",ltMsgGetVar_s(ltMsgPk,sNum));
						ltDbExecSql(G_DbCon,sqlBuf);
					}
		}
		ltDbClose(G_DbCon);
		ltportinfolist(confd,ltMsgPk,lt_MMHead);
		return 0;
}
int ltapplyportinfo(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){

	msaInitPortInfo(_ltPubInfo);
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");
	ltMsgFree(ltMsgPk);
	return 0;

}
//功能参数
int ltappFunctionok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	ltDbHeadPtr dbPtr;
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");

  char *myonlineusertype,*mynoautoadduser,*myattprev,*mynopicblock,*mydefaultuserflag;//功能参数
  char *mymaxusertimeout,*mymaxuseronlietime,*myuserproccesstime,*mymaxtcpsessiontimeout,*mytcpproccesstime,*mytimerate,*myflowrate;//系统参数

  char *myipmon,*myhttpmon,*myipreportmon,*myhttpreportmon,*mysmtpmon,*myformmon,*mypop3mon,*myblockmon,*myimmon,*myfilemon,*mysearchmon,*myusermon;//监视内容
  char *myuserkeepday,*myvidkeepday,*mynomondisk,*myrecordkeepday,*mymailkeepday,*myaimkeepday,*myformkeepday,*myhistorykeepday,*myloglevel;//配置参数

  char *myauthport,*mystrAuthIP,*mydenyUrl,*mysupportTCP,*mysupportUDP;   //验证服务参数


  //snmp参数
  char *mysnmpebable;
  char *mysnmpcommunity,*mysnmpsip,*mysnmpoid,*mysnmpv;
  char *mysnmpcommunity2,*mysnmpsip2,*mysnmpoid2,*mysnmpv2;
  char *mysnmpcommunity3,*mysnmpsip3,*mysnmpoid3,*mysnmpv3;
  char *mysnmpcommunity4,*mysnmpsip4,*mysnmpoid4,*mysnmpv4;
  char *mysnmpcommunity5,*mysnmpsip5,*mysnmpoid5,*mysnmpv5;
  char *mysnmpcommunity6,*mysnmpsip6,*mysnmpoid6,*mysnmpv6;

  //登录参数
  char *myideltime;
  char *mylocktime;

	int  actiontype;
	if(ltMsgGetVar_s(ltMsgPk,"actiontype")!=NULL){
		actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));
		char caCmdCon[128];
		if(actiontype==1){//功能参数
			  //mysupportEmail=ltMsgGetVar_s(ltMsgPk,"supportEmail");
			  //mysupportForm=ltMsgGetVar_s(ltMsgPk,"supportForm");
			  myonlineusertype=ltMsgGetVar_s(ltMsgPk,"onlineusertype");
			  mynoautoadduser=ltMsgGetVar_s(ltMsgPk,"noautoadduser");
			  myattprev=ltMsgGetVar_s(ltMsgPk,"attprev");
			  mynopicblock=ltMsgGetVar_s(ltMsgPk,"nopicblock");
			  mydefaultuserflag=ltMsgGetVar_s(ltMsgPk,"defaultuserflag");

//	      memset(caCmdCon,0,sizeof(caCmdCon));
//				sprintf(caCmdCon,"echo 'supportEmail=%s' > %s",mysupportEmail,"/etc/msa/msa/option/supportEmail");
//				system(caCmdCon);

//	      memset(caCmdCon,0,sizeof(caCmdCon));
//				sprintf(caCmdCon,"echo 'supportForm=%s' > %s",mysupportForm,"/etc/msa/msa/option/supportForm");
//				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'onlineusertype=%s' > %s",myonlineusertype,"/etc/msa/msa/option/onlineusertype");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'noautoadduser=%s' > %s",mynoautoadduser,"/etc/msa/msa/option/noautoadduser");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'attprev=%s' > %s",myattprev,"/etc/msa/msa/option/attprev");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'nopicblock=%s' > %s",mynopicblock,"/etc/msa/msa/option/nopicblock");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'defaultuserflag=%s' > %s",mydefaultuserflag,"/etc/msa/msa/option/defaultuserflag");
				system(caCmdCon);

				///////////////更新内存
//				_ltPubInfo->_msaOptionHead.supportEmail=bcCnfGetValueL(_pubconfdir,"option","supportEmail");//邮件数据包重组
//			  _ltPubInfo->_msaOptionHead.supportForm=bcCnfGetValueL(_pubconfdir,"option","supportForm");//表单数据包重组
			  _ltPubInfo->_msaOptionHead.onlineusertype=bcCnfGetValueL(_pubconfdir,"option","onlineusertype");//无法同时在线踢出前一用户
			  _ltPubInfo->_msaOptionHead.noautoadduser=bcCnfGetValueL(_pubconfdir,"option","noautoadduser");//默认禁止自动添加用户
			  _ltPubInfo->_msaOptionHead.attprev=bcCnfGetValueL(_pubconfdir,"option","attprev"); //启动附件预览和内容检索
			  _ltPubInfo->_msaOptionHead.nopicblock=bcCnfGetValueL(_pubconfdir,"option","nopicblock"); //不阻断辅助图片或者辅助脚本
			  _ltPubInfo->_msaOptionHead.defaultuserflag=bcCnfGetValueL(_pubconfdir,"option","defaultuserflag"); //用户超量时默认状态为免监控99或禁止访问100

		}else if(actiontype==2){//系统参数
			  mymaxusertimeout=ltMsgGetVar_s(ltMsgPk,"maxusertimeout");
			  mymaxuseronlietime=ltMsgGetVar_s(ltMsgPk,"maxuseronlietime");
			  myuserproccesstime=ltMsgGetVar_s(ltMsgPk,"userproccesstime");
			  mymaxtcpsessiontimeout=ltMsgGetVar_s(ltMsgPk,"maxtcpsessiontimeout");
			  mytcpproccesstime=ltMsgGetVar_s(ltMsgPk,"tcpproccesstime");
				mytimerate=ltMsgGetVar_s(ltMsgPk,"timerate");
				myflowrate=ltMsgGetVar_s(ltMsgPk,"flowrate");

				memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'maxusertimeout=%s' > %s",mymaxusertimeout,"/etc/msa/msa/option/maxusertimeout");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'maxuseronlietime=%s' > %s",mymaxuseronlietime,"/etc/msa/msa/option/maxuseronlietime");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'userproccesstime=%s' > %s",myuserproccesstime,"/etc/msa/msa/option/userproccesstime");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'maxtcpsessiontimeout=%s' > %s",mymaxtcpsessiontimeout,"/etc/msa/msa/option/maxtcpsessiontimeout");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'tcpproccesstime=%s' > %s",mytcpproccesstime,"/etc/msa/msa/option/tcpproccesstime");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'timerate=%s' > %s",mytimerate,"/etc/msa/msa/option/timerate");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'flowrate=%s' > %s",myflowrate,"/etc/msa/msa/option/flowrate");
				system(caCmdCon);


				///////////////更新内存
				_ltPubInfo->_msaOptionHead.maxuseronlietime=bcCnfGetValueL(_pubconfdir,"option","maxuseronlietime"); //允许用户最长在线时间/秒
			  if( _ltPubInfo->_msaOptionHead.maxuseronlietime<300){
			   		 _ltPubInfo->_msaOptionHead.maxuseronlietime=300;
			  }
			  _ltPubInfo->_msaOptionHead.maxusertimeout=bcCnfGetValueL(_pubconfdir,"option","maxusertimeout");//在线用户超时时间/秒
			  if(_ltPubInfo->_msaOptionHead.maxusertimeout<30){
			   		_ltPubInfo->_msaOptionHead.maxusertimeout=30;
			  }
			  _ltPubInfo->_msaOptionHead.userproccesstime=bcCnfGetValueL(_pubconfdir,"option","userproccesstime");//在线用户进程检查时间间隔
			  if(_ltPubInfo->_msaOptionHead.userproccesstime<10){
			   		_ltPubInfo->_msaOptionHead.userproccesstime=10;
			  }
			  if(_ltPubInfo->_msaOptionHead.userproccesstime>900){
			   		_ltPubInfo->_msaOptionHead.userproccesstime=900;
			  }
			  if(_ltPubInfo->_msaOptionHead.userproccesstime<10){
			   		_ltPubInfo->_msaOptionHead.userproccesstime=10;
			  }
			  _ltPubInfo->_msaOptionHead.maxtcpsessiontimeout=bcCnfGetValueL(_pubconfdir,"option","maxtcpsessiontimeout");//连接超时时间/秒
			  _ltPubInfo->_msaOptionHead.tcpproccesstime=bcCnfGetValueL(_pubconfdir,"option","tcpproccesstime");//连接进程检查时间间隔
			  if(_ltPubInfo->_msaOptionHead.tcpproccesstime<10){
			   		_ltPubInfo->_msaOptionHead.tcpproccesstime=10;
			  }
			  if(_ltPubInfo->_msaOptionHead.tcpproccesstime>900){
			   		_ltPubInfo->_msaOptionHead.tcpproccesstime=900;
			  }
			  _ltPubInfo->_msaOptionHead.flowrate=atof(bcCnfGetValueS(_pubconfdir,"option","flowrate"));
			  _ltPubInfo->_msaOptionHead.flowrate =_ltPubInfo->_msaOptionHead.flowrate/1000000;//流量费率
			  _ltPubInfo->_msaOptionHead.timerate=atof(bcCnfGetValueS(_pubconfdir,"option","timerate"));
			  _ltPubInfo->_msaOptionHead.timerate =_ltPubInfo->_msaOptionHead.timerate/3600;//时间费率

		}else if(actiontype==3){//系统参数
			  myipmon=ltMsgGetVar_s(ltMsgPk,"ipmon");
			  myhttpmon=ltMsgGetVar_s(ltMsgPk,"httpmon");
			  myipreportmon=ltMsgGetVar_s(ltMsgPk,"ipreportmon");
			  myhttpreportmon=ltMsgGetVar_s(ltMsgPk,"httpreportmon");
			  mysmtpmon=ltMsgGetVar_s(ltMsgPk,"smtpmon");
			  myformmon=ltMsgGetVar_s(ltMsgPk,"formmon");
			  mypop3mon=ltMsgGetVar_s(ltMsgPk,"pop3mon");
			  myblockmon=ltMsgGetVar_s(ltMsgPk,"blockmon");
			  myimmon=ltMsgGetVar_s(ltMsgPk,"immon");
			  myfilemon=ltMsgGetVar_s(ltMsgPk,"filemon");
			  mysearchmon=ltMsgGetVar_s(ltMsgPk,"searchmon");
			  myusermon=ltMsgGetVar_s(ltMsgPk,"usermon");

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'ipmon=%s' > %s",myipmon,"/etc/msa/msa/option/ipmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'httpmon=%s' > %s",myhttpmon,"/etc/msa/msa/option/httpmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'ipreportmon=%s' > %s",myipreportmon,"/etc/msa/msa/option/ipreportmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'httpreportmon=%s' > %s",myhttpreportmon,"/etc/msa/msa/option/httpreportmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'smtpmon=%s' > %s",mysmtpmon,"/etc/msa/msa/option/smtpmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'formmon=%s' > %s",myformmon,"/etc/msa/msa/option/formmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'pop3mon=%s' > %s",mypop3mon,"/etc/msa/msa/option/pop3mon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'blockmon=%s' > %s",myblockmon,"/etc/msa/msa/option/blockmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'immon=%s' > %s",myimmon,"/etc/msa/msa/option/immon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'filemon=%s' > %s",myfilemon,"/etc/msa/msa/option/filemon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'searchmon=%s' > %s",mysearchmon,"/etc/msa/msa/option/searchmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'usermon=%s' > %s",myusermon,"/etc/msa/msa/option/usermon");
				system(caCmdCon);

				//////更新内存
				_ltPubInfo->_msaOptionHead.ipmon=bcCnfGetValueL(_pubconfdir,"option","ipmon");
		    _ltPubInfo->_msaOptionHead.httpmon=bcCnfGetValueL(_pubconfdir,"option","httpmon");
		    _ltPubInfo->_msaOptionHead.ipreportmon=bcCnfGetValueL(_pubconfdir,"option","ipreportmon");
		    _ltPubInfo->_msaOptionHead.httpreportmon=bcCnfGetValueL(_pubconfdir,"option","httpreportmon");
		    _ltPubInfo->_msaOptionHead.smtpmon=bcCnfGetValueL(_pubconfdir,"option","smtpmon");
		    _ltPubInfo->_msaOptionHead.formmon=bcCnfGetValueL(_pubconfdir,"option","formmon");
		    _ltPubInfo->_msaOptionHead.pop3mon=bcCnfGetValueL(_pubconfdir,"option","pop3mon");
		    _ltPubInfo->_msaOptionHead.blockmon=bcCnfGetValueL(_pubconfdir,"option","blockmon");

		    _ltPubInfo->_msaOptionHead.immon=bcCnfGetValueL(_pubconfdir,"option","immon");
		    _ltPubInfo->_msaOptionHead.filemon=bcCnfGetValueL(_pubconfdir,"option","filemon");
		    _ltPubInfo->_msaOptionHead.searchmon=bcCnfGetValueL(_pubconfdir,"option","searchmon");
		    _ltPubInfo->_msaOptionHead.usermon=bcCnfGetValueL(_pubconfdir,"option","usermon"); //虚拟身份

		}else if(actiontype==4){//记录保存参数
	      mynomondisk=ltMsgGetVar_s(ltMsgPk,"nomondisk");
				myrecordkeepday=ltMsgGetVar_s(ltMsgPk,"recordkeepday");
				mymailkeepday=ltMsgGetVar_s(ltMsgPk,"mailkeepday");
				myaimkeepday=ltMsgGetVar_s(ltMsgPk,"aimkeepday");
				myvidkeepday=ltMsgGetVar_s(ltMsgPk,"vidkeepday");
				myuserkeepday=ltMsgGetVar_s(ltMsgPk,"userkeepday");
				myformkeepday=ltMsgGetVar_s(ltMsgPk,"formkeepday");
				myhistorykeepday=ltMsgGetVar_s(ltMsgPk,"historykeepday");
			  myloglevel=ltMsgGetVar_s(ltMsgPk,"loglevel");

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'nomondisk=%s' > %s",mynomondisk,"/etc/msa/msa/option/nomondisk");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'recordkeepday=%s' > %s",myrecordkeepday,"/etc/msa/msa/option/recordkeepday");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'mailkeepday=%s' > %s",mymailkeepday,"/etc/msa/msa/option/mailkeepday");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'aimkeepday=%s' > %s",myaimkeepday,"/etc/msa/msa/option/aimkeepday");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'vidkeepday=%s' > %s",myvidkeepday,"/etc/msa/msa/option/vidkeepday");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'userkeepday=%s' > %s",myuserkeepday,"/etc/msa/msa/option/userkeepday");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'formkeepday=%s' > %s",myformkeepday,"/etc/msa/msa/option/formkeepday");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'historykeepday=%s' > %s",myhistorykeepday,"/etc/msa/msa/option/historykeepday");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'loglevel=%s' > %s",myloglevel,"/etc/msa/msa/option/loglevel");
				system(caCmdCon);

				////////更新内存
		    _ltPubInfo->_msaOptionHead.nomondisk=bcCnfGetValueL(_pubconfdir,"option","nomondisk");
		    _ltPubInfo->_msaOptionHead.recordkeepday=bcCnfGetValueL(_pubconfdir,"option","recordkeepday");
		    _ltPubInfo->_msaOptionHead.mailkeepday=bcCnfGetValueL(_pubconfdir,"option","mailkeepday");
		    _ltPubInfo->_msaOptionHead.aimkeepday=bcCnfGetValueL(_pubconfdir,"option","aimkeepday");
		    _ltPubInfo->_msaOptionHead.formkeepday=bcCnfGetValueL(_pubconfdir,"option","formkeepday");
		    _ltPubInfo->_msaOptionHead.historykeepday=bcCnfGetValueL(_pubconfdir,"option","historykeepday");
		    _ltPubInfo->_msaOptionHead.vidkeepday=bcCnfGetValueL(_pubconfdir,"option","vidkeepday");
		    _ltPubInfo->_msaOptionHead.userkeepday=bcCnfGetValueL(_pubconfdir,"option","userkeepday");
		    _ltPubInfo->_msaOptionHead.loglevel=bcCnfGetValueL(_pubconfdir,"option","loglevel"); //记录级别

		}else if(actiontype==5){//snmp参数
			  mysnmpebable=ltMsgGetVar_s(ltMsgPk,"snmpebable");
			  mysnmpcommunity=ltMsgGetVar_s(ltMsgPk,"snmpcommunity");
			  mysnmpsip=ltMsgGetVar_s(ltMsgPk,"snmpsip");
			  mysnmpoid=ltMsgGetVar_s(ltMsgPk,"snmpoid");
			  mysnmpv=ltMsgGetVar_s(ltMsgPk,"snmpv");

			  mysnmpcommunity2=ltMsgGetVar_s(ltMsgPk,"snmpcommunity2");
			  mysnmpsip2=ltMsgGetVar_s(ltMsgPk,"snmpsip2");
			  mysnmpoid2=ltMsgGetVar_s(ltMsgPk,"snmpoid2");
			  mysnmpv2=ltMsgGetVar_s(ltMsgPk,"snmpv2");
			  mysnmpcommunity3=ltMsgGetVar_s(ltMsgPk,"snmpcommunity3");
			  mysnmpsip3=ltMsgGetVar_s(ltMsgPk,"snmpsip3");
			  mysnmpoid3=ltMsgGetVar_s(ltMsgPk,"snmpoid3");
			  mysnmpv3=ltMsgGetVar_s(ltMsgPk,"snmpv3");
			  mysnmpcommunity4=ltMsgGetVar_s(ltMsgPk,"snmpcommunity4");
			  mysnmpsip4=ltMsgGetVar_s(ltMsgPk,"snmpsip4");
			  mysnmpoid4=ltMsgGetVar_s(ltMsgPk,"snmpoid4");
			  mysnmpv4=ltMsgGetVar_s(ltMsgPk,"snmpv4");
			  mysnmpcommunity5=ltMsgGetVar_s(ltMsgPk,"snmpcommunity5");
			  mysnmpsip5=ltMsgGetVar_s(ltMsgPk,"snmpsip5");
			  mysnmpoid5=ltMsgGetVar_s(ltMsgPk,"snmpoid5");
			  mysnmpv5=ltMsgGetVar_s(ltMsgPk,"snmpv5");
			  mysnmpcommunity6=ltMsgGetVar_s(ltMsgPk,"snmpcommunity6");
			  mysnmpsip6=ltMsgGetVar_s(ltMsgPk,"snmpsip6");
			  mysnmpoid6=ltMsgGetVar_s(ltMsgPk,"snmpoid6");
			  mysnmpv6=ltMsgGetVar_s(ltMsgPk,"snmpv6");

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpebable=%s' > %s",mysnmpebable,"/etc/msa/msa/option/snmpebable");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpcommunity=%s' > %s",mysnmpcommunity,"/etc/msa/msa/option/snmpcommunity");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpsip=%s' > %s",mysnmpsip,"/etc/msa/msa/option/snmpsip");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpoid=%s' > %s",mysnmpoid,"/etc/msa/msa/option/snmpoid");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpv=%s' > %s",mysnmpv,"/etc/msa/msa/option/snmpv");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpcommunity2=%s' > %s",mysnmpcommunity2,"/etc/msa/msa/option/snmpcommunity2");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpsip2=%s' > %s",mysnmpsip2,"/etc/msa/msa/option/snmpsip2");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpoid2=%s' > %s",mysnmpoid2,"/etc/msa/msa/option/snmpoid2");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpv2=%s' > %s",mysnmpv2,"/etc/msa/msa/option/snmpv2");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpcommunity3=%s' > %s",mysnmpcommunity3,"/etc/msa/msa/option/snmpcommunity3");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpsip3=%s' > %s",mysnmpsip3,"/etc/msa/msa/option/snmpsip3");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpoid3=%s' > %s",mysnmpoid3,"/etc/msa/msa/option/snmpoid3");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpv3=%s' > %s",mysnmpv3,"/etc/msa/msa/option/snmpv3");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpcommunity4=%s' > %s",mysnmpcommunity4,"/etc/msa/msa/option/snmpcommunity4");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpsip4=%s' > %s",mysnmpsip4,"/etc/msa/msa/option/snmpsip4");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpoid4=%s' > %s",mysnmpoid4,"/etc/msa/msa/option/snmpoid4");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpv4=%s' > %s",mysnmpv4,"/etc/msa/msa/option/snmpv4");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpcommunity5=%s' > %s",mysnmpcommunity5,"/etc/msa/msa/option/snmpcommunity5");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpsip5=%s' > %s",mysnmpsip5,"/etc/msa/msa/option/snmpsip5");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpoid5=%s' > %s",mysnmpoid5,"/etc/msa/msa/option/snmpoid5");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpv5=%s' > %s",mysnmpv5,"/etc/msa/msa/option/snmpv5");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpcommunity6=%s' > %s",mysnmpcommunity6,"/etc/msa/msa/option/snmpcommunity6");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpsip6=%s' > %s",mysnmpsip6,"/etc/msa/msa/option/snmpsip6");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpoid6=%s' > %s",mysnmpoid6,"/etc/msa/msa/option/snmpoid6");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'snmpv6=%s' > %s",mysnmpv6,"/etc/msa/msa/option/snmpv6");
				system(caCmdCon);

				//////////////内存更新
				int i=0;
				_ltPubInfo->_msaOptionHead.snmpebable=bcCnfGetValueL(_pubconfdir,"option","snmpebable");//连接进程检查时间间隔
		    memset((char *)_ltPubInfo->_msaOptionHead.gnmsnmpgroup,0,sizeof(SNMPGROUP)*_MAX_SNMP_NUM);
		    for(i=0;i<_MAX_SNMP_NUM;i++){
					  char mytmpstr[64];
					  char *p;
					  sprintf(mytmpstr,"snmpcommunity%d",i+1);
						p=bcCnfGetValueS(_pubconfdir,"option", mytmpstr);
						if(p!=NULL){
							sprintf( _ltPubInfo->_msaOptionHead.gnmsnmpgroup[i].snmpcommunity,"%s",p);
						}
						sprintf(mytmpstr,"snmpv%d",i+1);
						p=bcCnfGetValueS(_pubconfdir,"option", mytmpstr);
						if(p!=NULL){
							sprintf(_ltPubInfo->_msaOptionHead.gnmsnmpgroup[i].snmpv,"%s",p);
						}
						sprintf(mytmpstr,"snmpsip%d",i+1);
						p=bcCnfGetValueS(_pubconfdir,"option",mytmpstr);
						if(p!=NULL){
							sprintf(_ltPubInfo->_msaOptionHead.gnmsnmpgroup[i].snmpsip,"%s",p);
						}
						sprintf(mytmpstr,"snmpoid%d",i+1);
						p=bcCnfGetValueS(_pubconfdir,"option", mytmpstr);
						if(p!=NULL){
							sprintf(_ltPubInfo->_msaOptionHead.gnmsnmpgroup[i].snmpoid,"%s",p);
						}
						if(!_ltPubInfo->_msaOptionHead.gnmsnmpgroup[i].snmpoid[0]
							|| !_ltPubInfo->_msaOptionHead.gnmsnmpgroup[i].snmpsip[0]
							|| !_ltPubInfo->_msaOptionHead.gnmsnmpgroup[i].snmpv[0]
							|| !_ltPubInfo->_msaOptionHead.gnmsnmpgroup[i].snmpcommunity[0]){
							memset((char *)&_ltPubInfo->_msaOptionHead.gnmsnmpgroup[i],0,sizeof(SNMPGROUP));
						}
	 				}
		}else if(actiontype==6){//验证服务参数
			  myauthport=ltMsgGetVar_s(ltMsgPk,"authport");
			  mystrAuthIP=ltMsgGetVar_s(ltMsgPk,"strAuthIP");
			  mydenyUrl=ltMsgGetVar_s(ltMsgPk,"denyUrl");
			  mysupportTCP=ltMsgGetVar_s(ltMsgPk,"supportTCP");
			  mysupportUDP=ltMsgGetVar_s(ltMsgPk,"supportUDP");


	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'authport=%s' > %s",myauthport,"/etc/msa/msa/option/authport");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'strAuthIP=%s' > %s",mystrAuthIP,"/etc/msa/msa/option/strAuthIP");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'denyUrl=%s' > %s",mydenyUrl,"/etc/msa/msa/option/denyUrl");
				system(caCmdCon);
	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'supportTCP=%s' > %s",mysupportTCP,"/etc/msa/msa/option/supportTCP");
				system(caCmdCon);
	  		memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'supportUDP=%s' > %s",mysupportUDP,"/etc/msa/msa/option/supportUDP");
				system(caCmdCon);

				///////////内存更新
				_ltPubInfo->_msaOptionHead.authport=bcCnfGetValueL(_pubconfdir,"option","authport");//内置验证服务端口
			  sprintf(_ltPubInfo->_msaOptionHead.strAuthIP,"%s",bcCnfGetValueS(_pubconfdir,"option", "strAuthIP"));
			  sprintf(_ltPubInfo->_msaOptionHead.denyUrl,"%s",bcCnfGetValueS(_pubconfdir,"option", "denyUrl"));//第三方拒绝访问页面地址
			  _ltPubInfo->_msaOptionHead.supportTCP=bcCnfGetValueL(_pubconfdir,"option","supportTCP");//tcp阻断
			  _ltPubInfo->_msaOptionHead.supportUDP=bcCnfGetValueL(_pubconfdir,"option","supportUDP"); //udp阻断
		}else if(actiontype==7){//登录参数
			  myideltime=ltMsgGetVar_s(ltMsgPk,"ideltime");
			  mylocktime=ltMsgGetVar_s(ltMsgPk,"locktime");

			  memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'ideltime=%s' > %s",myideltime,"/etc/msa/msa/option/ideltime");
				system(caCmdCon);
			  memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'locktime=%s' >> %s",mylocktime,"/etc/msa/msa/option/locktime");
				system(caCmdCon);
		}

	}

	ltappFunction(confd,ltMsgPk,lt_MMHead);
	return 0;

}

int ltappFunction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char str[16];
	//监视内容
  int myipmon,myhttpmon,myipreportmon,myhttpreportmon,mysmtpmon,myformmon,mypop3mon,myblockmon,myimmon,myfilemon,mysearchmon,myusermon;
	//显示方式
	char mynomondisk[32],myrecordkeepday[32],mymailkeepday[32],myaimkeepday[32],myformkeepday[32],myhistorykeepday[32],myloglevel[32];
	char myvidkeepday[32],myuserkeepday[32];
	//功能参数
	int mysupportEmail,mysupportForm,myonlineusertype,mynoautoadduser,myattprev,mynopicblock;
	char mydefaultuserflag[32];
	//系统参数
  char mymaxusertimeout[32],mymaxuseronlietime[32],myuserproccesstime[32],mytimerate[32],myflowrate[32],mymaxtcpsessiontimeout[32],mytcpproccesstime[32];
  //验证服务参数
  int mysupportTCP,mysupportUDP;
  char myauthport[256],mystrAuthIP[32],mydenyUrl[256];
  //snmp参数
  int mysnmpebable;
  char mysnmpcommunity[256],mysnmpsip[32],mysnmpoid[32],mysnmpv[256];
  char mysnmpcommunity2[256],mysnmpsip2[32],mysnmpoid2[32],mysnmpv2[256];
  char mysnmpcommunity3[256],mysnmpsip3[32],mysnmpoid3[32],mysnmpv3[256];
  char mysnmpcommunity4[256],mysnmpsip4[32],mysnmpoid4[32],mysnmpv4[256];
  char mysnmpcommunity5[256],mysnmpsip5[32],mysnmpoid5[32],mysnmpv5[256];
  char mysnmpcommunity6[256],mysnmpsip6[32],mysnmpoid6[32],mysnmpv6[256];
  //登录参数
	char mylocktime[32];
	char myideltime[32];

	ltDbHeadPtr dbPtr;
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	//监视内容
	myipmon=bcCnfGetValueL("/etc/msa/msa","option","ipmon");
	myhttpmon=bcCnfGetValueL("/etc/msa/msa","option","httpmon");
	myipreportmon=bcCnfGetValueL("/etc/msa/msa","option","ipreportmon");
	myhttpreportmon=bcCnfGetValueL("/etc/msa/msa","option","httpreportmon");
	mysmtpmon=bcCnfGetValueL("/etc/msa/msa","option","smtpmon");
	myformmon=bcCnfGetValueL("/etc/msa/msa","option","formmon");
	mypop3mon=bcCnfGetValueL("/etc/msa/msa","option","pop3mon");
	myblockmon=bcCnfGetValueL("/etc/msa/msa","option","blockmon");
	myimmon=bcCnfGetValueL("/etc/msa/msa","option","immon");
	myfilemon=bcCnfGetValueL("/etc/msa/msa","option","filemon");
	mysearchmon=bcCnfGetValueL("/etc/msa/msa","option","searchmon");
	myusermon=bcCnfGetValueL("/etc/msa/msa","option","usermon");

	memset(str,0,sizeof(str));//1
	sprintf(str,"ipmonchk%d",myipmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//2
	sprintf(str,"httpmonchk%d",myhttpmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//3
	sprintf(str,"ipreportmonchk%d",myipreportmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//4
	sprintf(str,"httpreportmonchk%d",myhttpreportmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//5
	sprintf(str,"smtpmonchk%d",mysmtpmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//6
	sprintf(str,"formmonchk%d",myformmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//7
	sprintf(str,"pop3monchk%d",mypop3mon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//8
	sprintf(str,"blockmonchk%d",myblockmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//9
	sprintf(str,"immonchk%d",myimmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//10
	sprintf(str,"filemonchk%d",myfilemon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//11
	sprintf(str,"searchmonchk%d",mysearchmon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));//12
	sprintf(str,"usermonchk%d",myusermon);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	//功能参数
	mysupportEmail=bcCnfGetValueL("/etc/msa/msa","option","supportEmail");
	mysupportForm=bcCnfGetValueL("/etc/msa/msa","option","supportForm");
	myonlineusertype=bcCnfGetValueL("/etc/msa/msa","option","onlineusertype");
	mynoautoadduser=bcCnfGetValueL("/etc/msa/msa","option","noautoadduser");
	myattprev=bcCnfGetValueL("/etc/msa/msa","option","attprev");
	mynopicblock=bcCnfGetValueL("/etc/msa/msa","option","nopicblock");
	bcCnfGetValue_s("/etc/msa/msa/option/defaultuserflag","defaultuserflag",mydefaultuserflag);

	memset(str,0,sizeof(str));
	sprintf(str,"supportEmailchk%d",mysupportEmail);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));
	sprintf(str,"supportFormchk%d",mysupportForm);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));
	sprintf(str,"onlineusertypechk%d",myonlineusertype);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));
	sprintf(str,"noautoadduserchk%d",mynoautoadduser);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));
	sprintf(str,"attprevchk%d",myattprev);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));
	sprintf(str,"nopicblockchk%d",mynopicblock);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(str,0,sizeof(str));
 	sprintf(str,"defaultuserflag%ssel",mydefaultuserflag);
 	lt_dbput_rootvars(dbPtr,1,str,"selected");

 	//登录参数
 	bcCnfGetValue_s("/etc/msa/msa/option/locktime","locktime",mylocktime);
 	lt_dbput_rootvars(dbPtr,1,"locktime",mylocktime);
 	sprintf(myideltime,"%d",3);
 	bcCnfGetValue_s("/etc/msa/msa/option/ideltime","ideltime",myideltime);
 	lt_dbput_rootvars(dbPtr,1,"ideltime",myideltime);


 	//系统参数
	memset(mymaxusertimeout,0,sizeof(mymaxusertimeout));
	bcCnfGetValue_s("/etc/msa/msa/option/maxusertimeout","maxusertimeout",mymaxusertimeout);
	lt_dbput_rootvars(dbPtr,1,"maxusertimeout",mymaxusertimeout);

	memset(mymaxuseronlietime,0,sizeof(mymaxuseronlietime));
	bcCnfGetValue_s("/etc/msa/msa/option/maxuseronlietime","maxuseronlietime",mymaxuseronlietime);
	lt_dbput_rootvars(dbPtr,1,"maxuseronlietime",mymaxuseronlietime);

	memset(myuserproccesstime,0,sizeof(myuserproccesstime));
	bcCnfGetValue_s("/etc/msa/msa/option/userproccesstime","userproccesstime",myuserproccesstime);
	lt_dbput_rootvars(dbPtr,1,"userproccesstime",myuserproccesstime);

	memset(mymaxtcpsessiontimeout,0,sizeof(mymaxtcpsessiontimeout));
	bcCnfGetValue_s("/etc/msa/msa/option/maxtcpsessiontimeout","maxtcpsessiontimeout",mymaxtcpsessiontimeout);
	lt_dbput_rootvars(dbPtr,1,"maxtcpsessiontimeout",mymaxtcpsessiontimeout);

	memset(mytcpproccesstime,0,sizeof(mytcpproccesstime));
	bcCnfGetValue_s("/etc/msa/msa/option/tcpproccesstime","tcpproccesstime",mytcpproccesstime);
	lt_dbput_rootvars(dbPtr,1,"tcpproccesstime",mytcpproccesstime);

	memset(mytimerate,0,sizeof(mytimerate));
	bcCnfGetValue_s("/etc/msa/msa/option/timerate","timerate",mytimerate);
	lt_dbput_rootvars(dbPtr,1,"timerate",mytimerate);

	memset(myflowrate,0,sizeof(myflowrate));
	bcCnfGetValue_s("/etc/msa/msa/option/flowrate","flowrate",myflowrate);
	lt_dbput_rootvars(dbPtr,1,"flowrate",myflowrate);

  //验证服务参数
	memset(myauthport,0,sizeof(myauthport));
	bcCnfGetValue_s("/etc/msa/msa/option/authport","authport",myauthport);
	lt_dbput_rootvars(dbPtr,1,"authport",myauthport);

	memset(mystrAuthIP,0,sizeof(mystrAuthIP));
	bcCnfGetValue_s("/etc/msa/msa/option/strAuthIP","strAuthIP",mystrAuthIP);
	lt_dbput_rootvars(dbPtr,1,"strAuthIP",mystrAuthIP);

	memset(mydenyUrl,0,sizeof(mydenyUrl));
	bcCnfGetValue_s("/etc/msa/msa/option/denyUrl","denyUrl",mydenyUrl);
	lt_dbput_rootvars(dbPtr,1,"denyUrl",mydenyUrl);

	mysupportTCP=bcCnfGetValueL("/etc/msa/msa","option","supportTCP");
	memset(str,0,sizeof(str));//3
	sprintf(str,"supportTCPchk%d",mysupportTCP);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	mysupportUDP=bcCnfGetValueL("/etc/msa/msa","option","supportUDP");
	memset(str,0,sizeof(str));//4
	sprintf(str,"supportUDPchk%d",mysupportUDP);
	lt_dbput_rootvars(dbPtr,1,str,"checked");


	//记录保存参数
	memset(mynomondisk,0,sizeof(mynomondisk));//6
	bcCnfGetValue_s("/etc/msa/msa/option/nomondisk","nomondisk",mynomondisk);
	lt_dbput_rootvars(dbPtr,1,"nomondisk",mynomondisk);

	memset(myrecordkeepday,0,sizeof(myrecordkeepday));//7
	bcCnfGetValue_s("/etc/msa/msa/option/recordkeepday","recordkeepday",myrecordkeepday);
	lt_dbput_rootvars(dbPtr,1,"recordkeepday",myrecordkeepday);

	memset(mymailkeepday,0,sizeof(mymailkeepday));//9
	bcCnfGetValue_s("/etc/msa/msa/option/mailkeepday","mailkeepday",mymailkeepday);
	lt_dbput_rootvars(dbPtr,1,"mailkeepday",mymailkeepday);

	memset(myaimkeepday,0,sizeof(myaimkeepday));//10
	bcCnfGetValue_s("/etc/msa/msa/option/aimkeepday","aimkeepday",myaimkeepday);
	lt_dbput_rootvars(dbPtr,1,"aimkeepday",myaimkeepday);

	memset(myformkeepday,0,sizeof(myformkeepday));//13
	bcCnfGetValue_s("/etc/msa/msa/option/formkeepday","formkeepday",myformkeepday);
	lt_dbput_rootvars(dbPtr,1,"formkeepday",myformkeepday);

	memset(myhistorykeepday,0,sizeof(myhistorykeepday));//14
	bcCnfGetValue_s("/etc/msa/msa/option/historykeepday","historykeepday",myhistorykeepday);
	lt_dbput_rootvars(dbPtr,1,"historykeepday",myhistorykeepday);


  memset(myvidkeepday,0,sizeof(myvidkeepday));//虚拟身份保存天数
	bcCnfGetValue_s("/etc/msa/msa/option/vidkeepday","vidkeepday",myvidkeepday);
	lt_dbput_rootvars(dbPtr,1,"vidkeepday",myvidkeepday);

	memset(myuserkeepday,0,sizeof(myuserkeepday));//用户保存天数
	bcCnfGetValue_s("/etc/msa/msa/option/userkeepday","userkeepday",myuserkeepday);
	lt_dbput_rootvars(dbPtr,1,"userkeepday",myuserkeepday);

 	bcCnfGetValue_s("/etc/msa/msa/option/loglevel","loglevel",myloglevel);//审计记录记录级别
	memset(str,0,sizeof(str));
 	sprintf(str,"loglevel%ssel",myloglevel);
 	lt_dbput_rootvars(dbPtr,1,str,"selected");

	//snmp参数
	mysnmpebable=bcCnfGetValueL("/etc/msa/msa","option","snmpebable");
	memset(str,0,sizeof(str));//启动SNMP学习功能
	sprintf(str,"snmpebablechk%d",mysnmpebable);
	lt_dbput_rootvars(dbPtr,1,str,"checked");

	memset(mysnmpcommunity,0,sizeof(mysnmpcommunity));//SNMP1的团体名[community]
	bcCnfGetValue_s("/etc/msa/msa/option/snmpcommunity","snmpcommunity",mysnmpcommunity);
	lt_dbput_rootvars(dbPtr,1,"snmpcommunity",mysnmpcommunity);

	memset(mysnmpsip,0,sizeof(mysnmpsip));//SNMP1的服务IP
	bcCnfGetValue_s("/etc/msa/msa/option/snmpsip","snmpsip",mysnmpsip);
	lt_dbput_rootvars(dbPtr,1,"snmpsip",mysnmpsip);

	memset(mysnmpoid,0,sizeof(mysnmpoid));//SNMP1的对象标示oid
	bcCnfGetValue_s("/etc/msa/msa/option/snmpoid","snmpoid",mysnmpoid);
	lt_dbput_rootvars(dbPtr,1,"snmpoid",mysnmpoid);

	memset(mysnmpv,0,sizeof(mysnmpv));//SNMP1的版本
	bcCnfGetValue_s("/etc/msa/msa/option/snmpv","snmpv",mysnmpv);
	lt_dbput_rootvars(dbPtr,1,"snmpv",mysnmpv);

	memset(mysnmpcommunity2,0,sizeof(mysnmpcommunity2));//SNMP2的团体名[community]
	bcCnfGetValue_s("/etc/msa/msa/option/snmpcommunity2","snmpcommunity2",mysnmpcommunity2);
	lt_dbput_rootvars(dbPtr,1,"snmpcommunity2",mysnmpcommunity2);

	memset(mysnmpsip2,0,sizeof(mysnmpsip2));//SNMP2的服务IP
	bcCnfGetValue_s("/etc/msa/msa/option/snmpsip2","snmpsip2",mysnmpsip2);
	lt_dbput_rootvars(dbPtr,1,"snmpsip2",mysnmpsip2);

	memset(mysnmpoid2,0,sizeof(mysnmpoid2));//SNMP2的对象标示oid
	bcCnfGetValue_s("/etc/msa/msa/option/snmpoid2","snmpoid2",mysnmpoid2);
	lt_dbput_rootvars(dbPtr,1,"snmpoid2",mysnmpoid2);

	memset(mysnmpv2,0,sizeof(mysnmpv2));//SNMP2的版本
	bcCnfGetValue_s("/etc/msa/msa/option/snmpv2","snmpv2",mysnmpv2);
	lt_dbput_rootvars(dbPtr,1,"snmpv2",mysnmpv2);

	memset(mysnmpcommunity3,0,sizeof(mysnmpcommunity3));//SNMP3的团体名[community]
	bcCnfGetValue_s("/etc/msa/msa/option/snmpcommunity3","snmpcommunity3",mysnmpcommunity3);
	lt_dbput_rootvars(dbPtr,1,"snmpcommunity3",mysnmpcommunity3);

	memset(mysnmpsip3,0,sizeof(mysnmpsip3));//SNMP3的服务IP
	bcCnfGetValue_s("/etc/msa/msa/option/snmpsip3","snmpsip3",mysnmpsip3);
	lt_dbput_rootvars(dbPtr,1,"snmpsip3",mysnmpsip3);

	memset(mysnmpoid3,0,sizeof(mysnmpoid3));//SNMP3的对象标示oid
	bcCnfGetValue_s("/etc/msa/msa/option/snmpoid3","snmpoid3",mysnmpoid3);
	lt_dbput_rootvars(dbPtr,1,"snmpoid3",mysnmpoid3);

	memset(mysnmpv3,0,sizeof(mysnmpv3));//SNMP3的版本
	bcCnfGetValue_s("/etc/msa/msa/option/snmpv3","snmpv3",mysnmpv3);
	lt_dbput_rootvars(dbPtr,1,"snmpv3",mysnmpv3);

	memset(mysnmpcommunity4,0,sizeof(mysnmpcommunity4));//SNMP4的团体名[community]
	bcCnfGetValue_s("/etc/msa/msa/option/snmpcommunity4","snmpcommunity4",mysnmpcommunity4);
	lt_dbput_rootvars(dbPtr,1,"snmpcommunity4",mysnmpcommunity4);

	memset(mysnmpsip4,0,sizeof(mysnmpsip4));//SNMP4的服务IP
	bcCnfGetValue_s("/etc/msa/msa/option/snmpsip4","snmpsip4",mysnmpsip4);
	lt_dbput_rootvars(dbPtr,1,"snmpsip4",mysnmpsip4);

	memset(mysnmpoid4,0,sizeof(mysnmpoid4));//SNMP4的对象标示oid
	bcCnfGetValue_s("/etc/msa/msa/option/snmpoid4","snmpoid4",mysnmpoid4);
	lt_dbput_rootvars(dbPtr,1,"snmpoid4",mysnmpoid4);

	memset(mysnmpv4,0,sizeof(mysnmpv4));//SNMP4的版本
	bcCnfGetValue_s("/etc/msa/msa/option/snmpv4","snmpv4",mysnmpv4);
	lt_dbput_rootvars(dbPtr,1,"snmpv4",mysnmpv4);

	memset(mysnmpcommunity5,0,sizeof(mysnmpcommunity5));//SNMP5的团体名[community]
	bcCnfGetValue_s("/etc/msa/msa/option/snmpcommunity5","snmpcommunity5",mysnmpcommunity5);
	lt_dbput_rootvars(dbPtr,1,"snmpcommunity5",mysnmpcommunity5);

	memset(mysnmpsip5,0,sizeof(mysnmpsip5));//SNMP5的服务IP
	bcCnfGetValue_s("/etc/msa/msa/option/snmpsip5","snmpsip5",mysnmpsip5);
	lt_dbput_rootvars(dbPtr,1,"snmpsip5",mysnmpsip5);

	memset(mysnmpoid5,0,sizeof(mysnmpoid5));//SNMP5的对象标示oid
	bcCnfGetValue_s("/etc/msa/msa/option/snmpoid5","snmpoid5",mysnmpoid5);
	lt_dbput_rootvars(dbPtr,1,"snmpoid5",mysnmpoid5);

	memset(mysnmpv5,0,sizeof(mysnmpv5));//SNMP5的版本
	bcCnfGetValue_s("/etc/msa/msa/option/snmpv5","snmpv5",mysnmpv5);
	lt_dbput_rootvars(dbPtr,1,"snmpv5",mysnmpv5);

	memset(mysnmpcommunity6,0,sizeof(mysnmpcommunity6));//SNMP6的团体名[community]
	bcCnfGetValue_s("/etc/msa/msa/option/snmpcommunity6","snmpcommunity6",mysnmpcommunity6);
	lt_dbput_rootvars(dbPtr,1,"snmpcommunity6",mysnmpcommunity6);

	memset(mysnmpsip6,0,sizeof(mysnmpsip6));//SNMP6的服务IP
	bcCnfGetValue_s("/etc/msa/msa/option/snmpsip6","snmpsip6",mysnmpsip6);
	lt_dbput_rootvars(dbPtr,1,"snmpsip6",mysnmpsip6);

	memset(mysnmpoid6,0,sizeof(mysnmpoid6));//SNMP5的对象标示oid
	bcCnfGetValue_s("/etc/msa/msa/option/snmpoid6","snmpoid6",mysnmpoid6);
	lt_dbput_rootvars(dbPtr,1,"snmpoid6",mysnmpoid6);

	memset(mysnmpv6,0,sizeof(mysnmpv6));//SNMP6的版本
	bcCnfGetValue_s("/etc/msa/msa/option/snmpv6","snmpv6",mysnmpv6);
	lt_dbput_rootvars(dbPtr,1,"snmpv6",mysnmpv6);

	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/appFunction.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}

/*
**邮件分类加载页面
*/
int msamailGroup(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		  ltDbHeadPtr dbPtr;
		  dbPtr=lt_dbinit();
	    lt_db_htmlpage(dbPtr,"utf-8");
	    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/mail.htm",dbPtr->head,0);
			ltMsgFree(ltMsgPk);
			lt_dbfree(dbPtr);
			return 0;
}
/*
**添加新的记录
*/
int ltmailfilteraction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
 		char *canum;
 		char *carname;
 		char *camailfrom;
 		char *camailto;
 		char *camailsub;
 		char *camailcont;
 		char *camailsize;
 		char *caattname;
 		char *caattsize;
 		char *caflags;
 		char *casflags;
	  char *casmail;
 		char *carnum;
 		char *camaction;

	  char caStr[2048];
    ltDbHeadPtr dbPtr;
    dbPtr=lt_dbinit();
    char sqlBuf[2048];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char caMsg[256];
    long lCount;

	  char *dbUser;
		char *dbPass;
		char *dbName;
		dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		 if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"db error!");
				ltMsgFree(ltMsgPk);
				return 0;
		 }

    sprintf(sqlBuf,"select count(*) from msamailfilter");
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
            sprintf(sqlBuf,"select max(rid)+1 from msamailfilter");
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

		canum=ltMsgGetVar_s(ltMsgPk,"num");
	     if(!canum){
	         canum="";
		 }
		carname=ltMsgGetVar_s(ltMsgPk,"rname");
	     if(!carname){
	         carname="";
		 }
		camailfrom=ltMsgGetVar_s(ltMsgPk,"mailfrom");
	     if(!camailfrom){
	         camailfrom="";
		 }
		camailto=ltMsgGetVar_s(ltMsgPk,"mailto");
	     if(!camailto){
	         camailto="";
		 }
		camailsub=ltMsgGetVar_s(ltMsgPk,"mailsub");
	     if(!camailsub){
	         camailsub="";
		 }
		camailcont=ltMsgGetVar_s(ltMsgPk,"mailcont");
	     if(!camailcont){
	         camailcont="";
		 }
		camailsize=ltMsgGetVar_s(ltMsgPk,"mailsize");
	     if(!camailsize){
	         camailsize="";
		 }
		caattname=ltMsgGetVar_s(ltMsgPk,"attname");
	     if(!caattname){
	         caattname="";
		 }
		caattsize=ltMsgGetVar_s(ltMsgPk,"attsize");
	     if(!caattsize){
	         caattsize="";
		 }
		caflags=ltMsgGetVar_s(ltMsgPk,"flags");
	     if(!caflags){
	         caflags="";
		 }
		casflags=ltMsgGetVar_s(ltMsgPk,"sflags");
	     if(!casflags){
	         casflags="";
		 }
		casmail=ltMsgGetVar_s(ltMsgPk,"smail");
	     if(!casmail){
	         casmail="";
		 }
		carnum=ltMsgGetVar_s(ltMsgPk,"rnum");
	     if(!carnum){
	         carnum="";
		 }
		camaction=ltMsgGetVar_s(ltMsgPk,"maction");
	     if(!camaction){
	         camaction="";
		 }
   sprintf(sqlBuf,"insert into msamailfilter(rid,num,rname,mailfrom,mailto,mailsub,mailcont,mailsize,attname,attsize,flags,sflags,smail,rnum,maction)values (%ld,%ld,'%s','%s','%s','%s','%s',%ld,'%s',%ld,%ld,%ld,'%s',%ld,%ld)",
                   					lCount,
                            atol(ltMsgGetVar_s(ltMsgPk,"num")),
                            ltMsgGetVar_s(ltMsgPk,"rname"),
                            ltMsgGetVar_s(ltMsgPk,"mailfrom"),
                            ltMsgGetVar_s(ltMsgPk,"mailto"),
                            ltMsgGetVar_s(ltMsgPk,"mailsub"),
                            ltMsgGetVar_s(ltMsgPk,"mailcont"),
                            atol(ltMsgGetVar_s(ltMsgPk,"mailsize")),
                            ltMsgGetVar_s(ltMsgPk,"attname"),
                            atol(ltMsgGetVar_s(ltMsgPk,"attsize")),
                            atol(ltMsgGetVar_s(ltMsgPk,"flags")),
                            atol(ltMsgGetVar_s(ltMsgPk,"sflags")),
                            ltMsgGetVar_s(ltMsgPk,"smail"),
                            atol(ltMsgGetVar_s(ltMsgPk,"rnum")),
                            atol(ltMsgGetVar_s(ltMsgPk,"maction")));
   ltDbExecSql(G_DbCon,sqlBuf);
   ///内存更新
   sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[lCount].caRname,"%s",ltMsgGetVar_s(ltMsgPk,"rname"));
	 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[lCount].caFrom,"%s",ltMsgGetVar_s(ltMsgPk,"mailfrom"));
	 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[lCount].caTo,"%s",ltMsgGetVar_s(ltMsgPk,"mailto"));
	 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[lCount].caSub,"%s",ltMsgGetVar_s(ltMsgPk,"mailsub"));
	 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[lCount].caCont,"%s",ltMsgGetVar_s(ltMsgPk,"mailcont"));
	 _ltPubInfo->_FilterListHead._ltMailFilter[lCount].lSize=atol(ltMsgGetVar_s(ltMsgPk,"mailsize"));
	 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[lCount].caAtt,"%s",ltMsgGetVar_s(ltMsgPk,"attname"));
	 _ltPubInfo->_FilterListHead._ltMailFilter[lCount].lAsize=atol(ltMsgGetVar_s(ltMsgPk,"attsize"));
	 _ltPubInfo->_FilterListHead._ltMailFilter[lCount].nFlags=atol(ltMsgGetVar_s(ltMsgPk,"flags"));
	 _ltPubInfo->_FilterListHead._ltMailFilter[lCount].nSflags=atol(ltMsgGetVar_s(ltMsgPk,"sflags"));
	 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[lCount].caSmail,"%s",ltMsgGetVar_s(ltMsgPk,"smail"));
	 _ltPubInfo->_FilterListHead._ltMailFilter[lCount].lRnum=atol(ltMsgGetVar_s(ltMsgPk,"rnum"));
	 _ltPubInfo->_FilterListHead._ltMailFilter[lCount].lMaction=atol(ltMsgGetVar_s(ltMsgPk,"maction"));

	 snprintf(caMsg,255,"%s","add msamailfilter success.");
	 logWrite(ltMsgPk,caMsg,3);
   memset(caStr,0,sizeof(caStr));
   ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");
   ltMsgFree(ltMsgPk);
   ltDbClose(G_DbCon);
   return 0;
}
/*
**显示记录  根据传的参数删除记录
*/
int ltmailfilterlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  char strBuf[1024];
	  ltDbCursor *tempCursor;
	  LT_DBROW tempRow;
	  char caTmpp[512];
	  long lRowNum,lStartRec,lSumRec,lCount,lNum;
	  int k;

		char *carid;
		char *canum;
 		char *carname;
 		char *camailfrom;
 		char *camailto;
 		char *camailsub;
 		char *camailcont;
 		char *camailsize;
 		char *caattname;
 		char *caattsize;
 		char *caflags;
 		char *casflags;
	  char *casmail;
 		char *carnum;
 		char *camaction;

	  stralloc strTemp;
		ltDbHeadPtr dbPtr;
	  dbPtr=lt_dbinit();
	  char caMsg[256];
	  lt_db_htmlpage(dbPtr,"utf-8");

    carid=ltMsgGetVar_s(ltMsgPk,"rid");
    canum=ltMsgGetVar_s(ltMsgPk,"num");
		carname=ltMsgGetVar_s(ltMsgPk,"rname");
		camailfrom=ltMsgGetVar_s(ltMsgPk,"mailfrom");
		camailto=ltMsgGetVar_s(ltMsgPk,"mailto");
		camailsub=ltMsgGetVar_s(ltMsgPk,"mailsub");
		camailcont=ltMsgGetVar_s(ltMsgPk,"mailcont");
		camailsize=ltMsgGetVar_s(ltMsgPk,"mailsize");
		caattname=ltMsgGetVar_s(ltMsgPk,"attname");
		caattsize=ltMsgGetVar_s(ltMsgPk,"attsize");
		caflags=ltMsgGetVar_s(ltMsgPk,"flags");
		casflags=ltMsgGetVar_s(ltMsgPk,"sflags");
		casmail=ltMsgGetVar_s(ltMsgPk,"smail");
		carnum=ltMsgGetVar_s(ltMsgPk,"rnum");
		camaction=ltMsgGetVar_s(ltMsgPk,"maction");
    //根据传的参数判断删除以及执行删除
	  int cadeflag;
		char sqlBuf[2048];

		char *dbUser;
		char *dbPass;
		char *dbName;
		dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
			ltMsgFree(ltMsgPk);
			return 0;
		}

		if(ltMsgGetVar_s(ltMsgPk,"deflag")!=NULL)
		{
			cadeflag=atol(ltMsgGetVar_s(ltMsgPk,"deflag"));
			if(cadeflag==1)
			{
				 sprintf(sqlBuf,"delete from msamailfilter where rid='%s'",carid);
				 ltDbExecSql(G_DbCon,sqlBuf);

			   ///内存更新
			   sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].caRname,"%s","");
				 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].caFrom,"%s","");
				 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].caTo,"%s","");
				 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].caSub,"%s","");
				 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].caCont,"%s","");
				 _ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].lSize=0;
				 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].caAtt,"%s","");
				 _ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].lAsize=0;
				 _ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].nFlags=0;
				 _ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].nSflags=0;
				 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].caSmail,"%s","");
				 _ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].lRnum=0;
				 _ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].lMaction=0;

				 snprintf(caMsg,255,"%s","delete msamailfilter success.");
			}
			if(cadeflag==2)//编辑
			{
				 sprintf(sqlBuf,"update msamailfilter set num='%s',rname='%s',mailfrom='%s',mailto='%s',mailsub='%s',mailcont='%s',mailsize='%s',attname='%s',attsize='%s',flags='%s',sflags='%s',smail='%s',rnum='%s',maction='%s' where rid='%s'",canum,carname,camailfrom,camailto,camailsub,camailcont,camailsize,caattname,caattsize,caflags,casflags,casmail,carnum,camaction,carid);
				 ltDbExecSql(G_DbCon,sqlBuf);

				 ///内存更新
			   sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].caRname,"%s",carname);
				 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].caFrom,"%s",camailfrom);
				 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].caTo,"%s",camailto);
				 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].caSub,"%s",camailsub);
				 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].caCont,"%s",camailcont);
				 _ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].lSize=atol(camailsize);
				 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].caAtt,"%s",caattname);
				 _ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].lAsize=atol(caattsize);
				 _ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].nFlags=atol(caflags);
				 _ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].nSflags=atol(casflags);
				 sprintf(_ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].caSmail,"%s",casmail);
				 _ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].lRnum=atol(carnum);
				 _ltPubInfo->_FilterListHead._ltMailFilter[atol(carid)].lMaction=atol(camaction);

				 snprintf(caMsg,255,"%s","update msamailfilter success.");
			}

				 logWrite(ltMsgPk,caMsg,3);
				 ltMsgPk->msgpktype=1;
			   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
			   ltMsgFree(ltMsgPk);
			   return 0;
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

    sprintf(strBuf,"select count(*) from msamailfilter");
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
    sprintf(strBuf,"select rid,num,rname,mailfrom,mailto,mailsub,mailcont,mailsize,attname,attsize,flags,sflags,smail,rnum,maction from msamailfilter limit %lu offset %lu ",lRowNum,lStartRec);

	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {
          if(k==0)
					{
            sprintf(caTmpp,"{\"rid\":\"%s\",\"num\":\"%s\",\"rname\":\"%s\",\"mailfrom\":\"%s\",\"mailto\":\"%s\",\"mailsub\":\"%s\",\"mailcont\":\"%s\",\"mailsize\":\"%s\",\"attname\":\"%s\",\"attsize\":\"%s\",\"flags\":\"%s\",\"sflags\":\"%s\",\"smail\":\"%s\",\"rnum\":\"%s\",\"maction\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14]);
            stralloc_cats(&strTemp,caTmpp);
					}
					else
					{
				    sprintf(caTmpp,",{\"rid\":\"%s\",\"num\":\"%s\",\"rname\":\"%s\",\"mailfrom\":\"%s\",\"mailto\":\"%s\",\"mailsub\":\"%s\",\"mailcont\":\"%s\",\"mailsize\":\"%s\",\"attname\":\"%s\",\"attsize\":\"%s\",\"flags\":\"%s\",\"sflags\":\"%s\",\"smail\":\"%s\",\"rnum\":\"%s\",\"maction\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14]);
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
   ltDbClose(G_DbCon);
   return 0;
}
/*
*邮件详情对话框  根据rid查询
*/
int msaShowMail(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char *my_id;
	 char sqlBuf[2048];
   ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   ltDbCursor *tempCursor;
   LT_DBROW   tempRow;
   my_id=ltMsgGetVar_s(ltMsgPk,"my_id");

   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"db error");
			ltMsgFree(ltMsgPk);
			return 0;
	 }

   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   if(my_id!=NULL){
		sprintf(sqlBuf,"select num,rname,mailfrom,mailto,mailsub,mailcont,mailsize,attname,attsize,flags,sflags,smail,rnum,maction from msamailfilter where rid='%s'",my_id);
			 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			 if(tempCursor!=NULL)
			 {
					 tempRow= ltDbFetchRow(tempCursor);
					 if(tempRow!=NULL)
					 	{
						 lt_dbput_rootvars(dbPtr,1,"rid",my_id);

						 char str[16];
						 memset(str,0,sizeof(str));
						 sprintf(str,"num%ssel",tempRow[0]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");

						 lt_dbput_rootvars(dbPtr,1,"rname",tempRow[1]);
						 lt_dbput_rootvars(dbPtr,1,"mailfrom",tempRow[2]);
						 lt_dbput_rootvars(dbPtr,1,"mailto",tempRow[3]);
						 lt_dbput_rootvars(dbPtr,1,"mailsub",tempRow[4]);
						 lt_dbput_rootvars(dbPtr,1,"mailcont",tempRow[5]);
						 lt_dbput_rootvars(dbPtr,1,"mailsize",tempRow[6]);
						 lt_dbput_rootvars(dbPtr,1,"attname",tempRow[7]);
						 lt_dbput_rootvars(dbPtr,1,"attsize",tempRow[8]);

						 memset(str,0,sizeof(str));
						 sprintf(str,"flags%ssel",tempRow[9]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");

						 lt_dbput_rootvars(dbPtr,1,"%s",str);

						 memset(str,0,sizeof(str));
						 sprintf(str,"sflags%ssel",tempRow[10]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");

						 lt_dbput_rootvars(dbPtr,1,"smail",tempRow[11]);
						 lt_dbput_rootvars(dbPtr,1,"rnum",tempRow[12]);

						 memset(str,0,sizeof(str));
						 sprintf(str,"maction%ssel",tempRow[13]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");

						 lt_dbput_rootvars(dbPtr,1,"my_id",my_id);
					 }
				 ltDbCloseCursor(tempCursor);
			 }
   }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/updateMailPage.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   ltDbClose(G_DbCon);
   return 0;
}

/*
**表单分类加载页面
*/
int msaformGroup(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		  ltDbHeadPtr dbPtr;
		  dbPtr=lt_dbinit();
	    lt_db_htmlpage(dbPtr,"utf-8");
	    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/form.htm",dbPtr->head,0);
			ltMsgFree(ltMsgPk);
			lt_dbfree(dbPtr);
			return 0;
}
/*
**添加新的记录
*/
int ltformfilteraction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char *canum;
	 char *carname;
	 char *caformfrom;
	 char *caformhost;
	 char *caformurl;
	 char *caformlcont;
	 char *camailsize;
	 char *caattname;
	 char *caattsize;
	 char *caflags;
	 char *casflags;
   char *casmail;
	 char *camaction;

   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"db error");
			ltMsgFree(ltMsgPk);
			return 0;
	 }

   char sqlBuf[2048];
   ltDbCursor *tempCursor;
   LT_DBROW tempRow;
   char caMsg[256];
   long lCount;
   sprintf(sqlBuf,"select count(*) from msaformfilter");
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
        sprintf(sqlBuf,"select max(rid)+1 from msaformfilter");
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

	 canum=ltMsgGetVar_s(ltMsgPk,"num");
     if(!canum){
         canum="";
	 }
	 carname=ltMsgGetVar_s(ltMsgPk,"rname");
     if(!carname){
         carname="";
	 }
	 caformfrom=ltMsgGetVar_s(ltMsgPk,"formfrom");
     if(!caformfrom){
         caformfrom="";
	 }
	 caformhost=ltMsgGetVar_s(ltMsgPk,"formhost");
     if(!caformhost){
         caformhost="";
	 }
	 caformurl=ltMsgGetVar_s(ltMsgPk,"formurl");
     if(!caformurl){
         caformurl="";
	 }
	 caformlcont=ltMsgGetVar_s(ltMsgPk,"formlcont");
     if(!caformlcont){
         caformlcont="";
	 }
	 camailsize=ltMsgGetVar_s(ltMsgPk,"mailsize");
     if(!camailsize){
         camailsize="";
	 }
	 caattname=ltMsgGetVar_s(ltMsgPk,"attname");
     if(!caattname){
         caattname="";
	 }
	 caattsize=ltMsgGetVar_s(ltMsgPk,"attsize");
     if(!caattsize){
         caattsize="";
	 }
	 caflags=ltMsgGetVar_s(ltMsgPk,"flags");
     if(!caflags){
         caflags="";
	 }
	 casflags=ltMsgGetVar_s(ltMsgPk,"sflags");
     if(!casflags){
         casflags="";
	 }
	 casmail=ltMsgGetVar_s(ltMsgPk,"smail");
     if(!casmail){
         casmail="";
	 }
	 camaction=ltMsgGetVar_s(ltMsgPk,"maction");
     if(!camaction){
         camaction="";
	 }
	 sprintf(sqlBuf,"insert into msaformfilter(rid,num,rname,formfrom,formhost,formurl,formlcont,formsize,attname,attsize,flags,sflags,smail,maction)values(%ld,%ld,'%s','%s','%s','%s','%s',%ld,'%s',%ld,%ld,%ld,'%s',%ld)",
	                 					lCount,
	                          atol(ltMsgGetVar_s(ltMsgPk,"num")),
	                          ltMsgGetVar_s(ltMsgPk,"rname"),
	                          ltMsgGetVar_s(ltMsgPk,"formfrom"),
	                          ltMsgGetVar_s(ltMsgPk,"formhost"),
	                          ltMsgGetVar_s(ltMsgPk,"formurl"),
	                          ltMsgGetVar_s(ltMsgPk,"formlcont"),
	                          atol(ltMsgGetVar_s(ltMsgPk,"formsize")),
	                          ltMsgGetVar_s(ltMsgPk,"attname"),
	                          atol(ltMsgGetVar_s(ltMsgPk,"attsize")),
	                          atol(ltMsgGetVar_s(ltMsgPk,"flags")),
	                          atol(ltMsgGetVar_s(ltMsgPk,"sflags")),
	                          ltMsgGetVar_s(ltMsgPk,"smail"),
	                          atol(ltMsgGetVar_s(ltMsgPk,"maction")));
	 ltDbExecSql(G_DbCon,sqlBuf);
	 ///内存更新
	 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[lCount].caRname,"%s",ltMsgGetVar_s(ltMsgPk,"rname"));
	 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[lCount].caFrom,"%s",ltMsgGetVar_s(ltMsgPk,"formfrom"));
	 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[lCount].caHost,"%s",ltMsgGetVar_s(ltMsgPk,"formhost"));
	 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[lCount].caURL,"%s",ltMsgGetVar_s(ltMsgPk,"formurl"));
	 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[lCount].caCon,"%s",ltMsgGetVar_s(ltMsgPk,"formlcont"));
	 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[lCount].caAtt,"%s",ltMsgGetVar_s(ltMsgPk,"attname"));
	 _ltPubInfo->_FilterListHead._ltFormFilter[lCount].lSize=atol(ltMsgGetVar_s(ltMsgPk,"formsize"));
	 _ltPubInfo->_FilterListHead._ltFormFilter[lCount].lAsize=atol(ltMsgGetVar_s(ltMsgPk,"attsize"));
	 _ltPubInfo->_FilterListHead._ltFormFilter[lCount].nFlags=atol(ltMsgGetVar_s(ltMsgPk,"flags"));
	 _ltPubInfo->_FilterListHead._ltFormFilter[lCount].nSflags=atol(ltMsgGetVar_s(ltMsgPk,"sflags"));
	 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[lCount].caSmail,"%s",ltMsgGetVar_s(ltMsgPk,"smail"));
	 _ltPubInfo->_FilterListHead._ltFormFilter[lCount].lMaction=atol(ltMsgGetVar_s(ltMsgPk,"maction"));

	 snprintf(caMsg,255,"%s","add msaformfilter success.");
	 logWrite(ltMsgPk,caMsg,3);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");
	 ltMsgFree(ltMsgPk);
	 ltDbClose(G_DbCon);
	 return 0;
}

/*
**显示记录  根据传的参数删除记录
*/
int ltformfilterlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		 char strBuf[1024];
	   ltDbCursor *tempCursor;
	   LT_DBROW tempRow;
	   char caTmpp[512];
	   long lRowNum,lStartRec,lSumRec,lCount,lNum;
	   int k;

		 char *carid;
		 char *canum;
 		 char *carname;
 		 char *caformfrom;
 		 char *caformhost;
 		 char *caformurl;
 		 char *caformlcont;
 		 char *caformsize;
 		 char *caattname;
 		 char *caattsize;
 		 char *caflags;
 		 char *casflags;
	   char *casmail;
 		 char *camaction;

	   stralloc strTemp;
		 ltDbHeadPtr dbPtr;
	   dbPtr=lt_dbinit();
	   char caMsg[256];
	   lt_db_htmlpage(dbPtr,"utf-8");

    carid=ltMsgGetVar_s(ltMsgPk,"rid");
    canum=ltMsgGetVar_s(ltMsgPk,"num");
		carname=ltMsgGetVar_s(ltMsgPk,"rname");
		caformfrom=ltMsgGetVar_s(ltMsgPk,"formfrom");
		caformhost=ltMsgGetVar_s(ltMsgPk,"formhost");
		caformurl=ltMsgGetVar_s(ltMsgPk,"formurl");
		caformlcont=ltMsgGetVar_s(ltMsgPk,"formlcont");
		caformsize=ltMsgGetVar_s(ltMsgPk,"formsize");
		caattname=ltMsgGetVar_s(ltMsgPk,"attname");
		caattsize=ltMsgGetVar_s(ltMsgPk,"attsize");
		caflags=ltMsgGetVar_s(ltMsgPk,"flags");
		casflags=ltMsgGetVar_s(ltMsgPk,"sflags");
		casmail=ltMsgGetVar_s(ltMsgPk,"smail");
		camaction=ltMsgGetVar_s(ltMsgPk,"maction");
    //根据传的参数判断删除以及执行删除

    char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"");
			ltMsgFree(ltMsgPk);
			return 0;
	 }

	  int cadeflag;
		char sqlBuf[2048];
		if(ltMsgGetVar_s(ltMsgPk,"deflag")!=NULL)
		{
			cadeflag=atol(ltMsgGetVar_s(ltMsgPk,"deflag"));
			if(cadeflag==1)
			{
				 sprintf(sqlBuf,"delete from msaformfilter where rid='%s'",carid);
				 ltDbExecSql(G_DbCon,sqlBuf);
				 ///内存更新
				 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].caRname,"%s","");
				 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].caFrom,"%s","");
				 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].caHost,"%s","");
				 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].caURL,"%s","");
				 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].caCon,"%s","");
				 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].caAtt,"%s","");
				 _ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].lSize=0;
				 _ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].lAsize=0;
				 _ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].nFlags=0;
				 _ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].nSflags=0;
				 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].caSmail,"%s","");
				 _ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].lMaction=0;

				 snprintf(caMsg,255,"%s","delete msaformfilter success.");
			}
			if(cadeflag==2)//编辑
			{
				 sprintf(sqlBuf,"update msaformfilter set num='%s',rname='%s',formfrom='%s',formhost='%s',formurl='%s',formlcont='%s',formsize='%s',attname='%s',attsize='%s',flags='%s',sflags='%s',smail='%s',maction='%s' where rid='%s'",canum,carname,caformfrom,caformhost,caformurl,caformlcont,caformsize,caattname,caattsize,caflags,casflags,casmail,camaction,carid);
				 ltDbExecSql(G_DbCon,sqlBuf);
				 ///内存更新
				 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].caRname,"%s",ltMsgGetVar_s(ltMsgPk,"rname"));
				 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].caFrom,"%s",ltMsgGetVar_s(ltMsgPk,"formfrom"));
				 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].caHost,"%s",ltMsgGetVar_s(ltMsgPk,"formhost"));
				 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].caURL,"%s",ltMsgGetVar_s(ltMsgPk,"formurl"));
				 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].caCon,"%s",ltMsgGetVar_s(ltMsgPk,"formlcont"));
				 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].caAtt,"%s",ltMsgGetVar_s(ltMsgPk,"attname"));
				 _ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].lSize=atol(ltMsgGetVar_s(ltMsgPk,"formsize"));
				 _ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].lAsize=atol(ltMsgGetVar_s(ltMsgPk,"attsize"));
				 _ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].nFlags=atol(ltMsgGetVar_s(ltMsgPk,"flags"));
				 _ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].nSflags=atol(ltMsgGetVar_s(ltMsgPk,"sflags"));
				 sprintf(_ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].caSmail,"%s",ltMsgGetVar_s(ltMsgPk,"smail"));
				 _ltPubInfo->_FilterListHead._ltFormFilter[atol(carid)].lMaction=atol(ltMsgGetVar_s(ltMsgPk,"maction"));

				 snprintf(caMsg,255,"%s","update msaformfilter success.");
			}
				 logWrite(ltMsgPk,caMsg,3);
				 ltMsgPk->msgpktype=1;
			   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
			   ltMsgFree(ltMsgPk);
			   return 0;
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

    sprintf(strBuf,"select count(*) from msaformfilter");
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
    sprintf(strBuf,"select rid,num,rname,formfrom,formhost,formurl,formlcont,formsize,attname,attsize,flags,sflags,smail,maction from msaformfilter where flags=0  limit %lu offset %lu ",lRowNum,lStartRec);
    tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {
          if(k==0)
					{
            sprintf(caTmpp,"{\"rid\":\"%s\",\"num\":\"%s\",\"rname\":\"%s\",\"formfrom\":\"%s\",\"formhost\":\"%s\",\"formurl\":\"%s\",\"formlcont\":\"%s\",\"formsize\":\"%s\",\"attname\":\"%s\",\"attsize\":\"%s\",\"flags\":\"%s\",\"sflags\":\"%s\",\"smail\":\"%s\",\"maction\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13]);
						stralloc_cats(&strTemp,caTmpp);
					}
					else
					{
				    sprintf(caTmpp,",{\"rid\":\"%s\",\"num\":\"%s\",\"rname\":\"%s\",\"formfrom\":\"%s\",\"formhost\":\"%s\",\"formurl\":\"%s\",\"formlcont\":\"%s\",\"formsize\":\"%s\",\"attname\":\"%s\",\"attsize\":\"%s\",\"flags\":\"%s\",\"sflags\":\"%s\",\"smail\":\"%s\",\"maction\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13]);
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
   ltDbClose(G_DbCon);
   return 0;
}

int msaShowForm(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char *my_id;
	 char sqlBuf[2048];
   ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   ltDbCursor *tempCursor;
   LT_DBROW   tempRow;
   my_id=ltMsgGetVar_s(ltMsgPk,"my_id");

   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
			ltMsgFree(ltMsgPk);
			return 0;
	 }

   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   if(my_id!=NULL){
		sprintf(sqlBuf,"select num,rname,formfrom,formhost,formurl,formlcont,formsize,attname,attsize,flags,sflags,smail,maction from msaformfilter where rid='%s'",my_id);

			 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			 if(tempCursor!=NULL)
			 {
					 tempRow= ltDbFetchRow(tempCursor);
					 if(tempRow!=NULL)
					 	{
						 lt_dbput_rootvars(dbPtr,1,"rid",my_id);

						 char str[16];
						 memset(str,0,sizeof(str));
						 sprintf(str,"num%ssel",tempRow[0]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");

						 lt_dbput_rootvars(dbPtr,1,"rname",tempRow[1]);
						 lt_dbput_rootvars(dbPtr,1,"formfrom",tempRow[2]);
						 lt_dbput_rootvars(dbPtr,1,"formhost",tempRow[3]);
						 lt_dbput_rootvars(dbPtr,1,"formurl",tempRow[4]);
						 lt_dbput_rootvars(dbPtr,1,"formlcont",tempRow[5]);
						 lt_dbput_rootvars(dbPtr,1,"formsize",tempRow[6]);
						 lt_dbput_rootvars(dbPtr,1,"attname",tempRow[7]);
						 lt_dbput_rootvars(dbPtr,1,"attsize",tempRow[8]);

						 memset(str,0,sizeof(str));
						 sprintf(str,"flags%ssel",tempRow[9]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");

						 lt_dbput_rootvars(dbPtr,1,"%s",str);

						 memset(str,0,sizeof(str));
						 sprintf(str,"sflags%ssel",tempRow[10]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");

						 lt_dbput_rootvars(dbPtr,1,"smail",tempRow[11]);

						 memset(str,0,sizeof(str));
						 sprintf(str,"maction%ssel",tempRow[12]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");

						 lt_dbput_rootvars(dbPtr,1,"my_id",my_id);
					 }
				 ltDbCloseCursor(tempCursor);
			 }
   }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/updateFormPage.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   ltDbClose(G_DbCon);
   return 0;
}
/////////////////////////////////////////////////////////////

/*
**内核表单表单分类加载页面
*/
int msaformOneGroup(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		  ltDbHeadPtr dbPtr;
		  dbPtr=lt_dbinit();
	    lt_db_htmlpage(dbPtr,"utf-8");
	    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/noListenForm.htm",dbPtr->head,0);
			ltMsgFree(ltMsgPk);
			lt_dbfree(dbPtr);
			return 0;
}
/*
**添加新的记录
*/
int ltformOnefilteraction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char *canum;
	char *carname;
	char *caformfrom;
	char *caformhost;
	char *caformurl;
	char *camaction;

  char *dbUser;
	char *dbPass;
	char *dbName;
	dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"db error");
			ltMsgFree(ltMsgPk);
			return 0;
	}

  char sqlBuf[2048];
  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
  char caMsg[256];
  long lCount;
  sprintf(sqlBuf,"select count(*) from msaformfilterOne");
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
        sprintf(sqlBuf,"select max(rid)+1 from msaformfilterOne");
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

	canum=ltMsgGetVar_s(ltMsgPk,"num");
     if(!canum){
         canum="";
	 }
	carname=ltMsgGetVar_s(ltMsgPk,"rname");
     if(!carname){
         carname="";
	 }
	caformfrom=ltMsgGetVar_s(ltMsgPk,"formfrom");
     if(!caformfrom){
         caformfrom="";
	 }
	caformhost=ltMsgGetVar_s(ltMsgPk,"formhost");
     if(!caformhost){
         caformhost="";
	 }
	caformurl=ltMsgGetVar_s(ltMsgPk,"formurl");
     if(!caformurl){
         caformurl="";
	 }
	camaction=ltMsgGetVar_s(ltMsgPk,"maction");
     if(!camaction){
         camaction="";
	 }
	 sprintf(sqlBuf,"insert into msaformfilterOne(rid,num,rname,formfrom,formhost,formurl,maction)values(%ld,%ld,'%s','%s','%s','%s',%ld)",
	                 					lCount,
	                          atol(ltMsgGetVar_s(ltMsgPk,"num")),
	                          ltMsgGetVar_s(ltMsgPk,"rname"),
	                          ltMsgGetVar_s(ltMsgPk,"formfrom"),
	                          ltMsgGetVar_s(ltMsgPk,"formhost"),
	                          ltMsgGetVar_s(ltMsgPk,"formurl"),
	                          atol(ltMsgGetVar_s(ltMsgPk,"maction")));
	 ltDbExecSql(G_DbCon,sqlBuf);
	 printf("sqlBuf:%s\n",sqlBuf);
	 ///内存更新
	 sprintf(_ltPubInfo->_FilterListHead._ltFormNoMon[lCount].caRname,"%s",ltMsgGetVar_s(ltMsgPk,"rname"));
	 sprintf(_ltPubInfo->_FilterListHead._ltFormNoMon[lCount].caFrom,"%s",ltMsgGetVar_s(ltMsgPk,"formfrom"));
	 sprintf(_ltPubInfo->_FilterListHead._ltFormNoMon[lCount].caHost,"%s",ltMsgGetVar_s(ltMsgPk,"formhost"));
	 sprintf(_ltPubInfo->_FilterListHead._ltFormNoMon[lCount].caURL,"%s",ltMsgGetVar_s(ltMsgPk,"formurl"));
	 _ltPubInfo->_FilterListHead._ltFormNoMon[lCount].lMaction=atol(ltMsgGetVar_s(ltMsgPk,"maction"));

	 snprintf(caMsg,255,"%s","add msaformfilterOne success.");
	 logWrite(ltMsgPk,caMsg,3);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");
	 ltMsgFree(ltMsgPk);
	 ltDbClose(G_DbCon);
	 return 0;
}

/*
**显示记录  根据传的参数删除记录
*/
int ltformOnefilterlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	   char strBuf[1024];
	   ltDbCursor *tempCursor;
	   LT_DBROW tempRow;
	   char caTmpp[512];
	   long lRowNum,lStartRec,lSumRec,lCount,lNum;
	   int k;

		 char *carid;
		 char *canum;
 		 char *carname;
 		 char *caformfrom;
 		 char *caformhost;
 		 char *caformurl;
 		 char *camaction;

	   stralloc strTemp;
		 ltDbHeadPtr dbPtr;
	   dbPtr=lt_dbinit();
	   char caMsg[256];
	   lt_db_htmlpage(dbPtr,"utf-8");

    carid=ltMsgGetVar_s(ltMsgPk,"rid");
    canum=ltMsgGetVar_s(ltMsgPk,"num");
		carname=ltMsgGetVar_s(ltMsgPk,"rname");
		caformfrom=ltMsgGetVar_s(ltMsgPk,"formfrom");
		caformhost=ltMsgGetVar_s(ltMsgPk,"formhost");
		caformurl=ltMsgGetVar_s(ltMsgPk,"formurl");
		camaction=ltMsgGetVar_s(ltMsgPk,"maction");
    //根据传的参数判断删除以及执行删除

   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"");
			ltMsgFree(ltMsgPk);
			return 0;
	 }

	  int cadeflag;
		char sqlBuf[2048];
		if(ltMsgGetVar_s(ltMsgPk,"deflag")!=NULL)
		{
			cadeflag=atol(ltMsgGetVar_s(ltMsgPk,"deflag"));
			if(cadeflag==1)
			{
				 sprintf(sqlBuf,"delete from msaformfilterOne where rid='%s'",carid);
				 ltDbExecSql(G_DbCon,sqlBuf);
				 ///内存更新
				 sprintf(_ltPubInfo->_FilterListHead._ltFormNoMon[atol(carid)].caRname,"%s","");
				 sprintf(_ltPubInfo->_FilterListHead._ltFormNoMon[atol(carid)].caFrom,"%s","");
				 sprintf(_ltPubInfo->_FilterListHead._ltFormNoMon[atol(carid)].caHost,"%s","");
				 sprintf(_ltPubInfo->_FilterListHead._ltFormNoMon[atol(carid)].caURL,"%s","");
				 _ltPubInfo->_FilterListHead._ltFormNoMon[atol(carid)].lMaction=0;
				 snprintf(caMsg,255,"%s","delete msaformfilterOne success.");
			}
			if(cadeflag==2)//编辑
			{
				 sprintf(sqlBuf,"update msaformfilterOne set num='%s',rname='%s',formfrom='%s',formhost='%s',formurl='%s',maction='%s' where rid='%s'",canum,carname,caformfrom,caformhost,caformurl,camaction,carid);
				 ltDbExecSql(G_DbCon,sqlBuf);
				 ///内存更新
				 sprintf(_ltPubInfo->_FilterListHead._ltFormNoMon[atol(carid)].caRname,"%s",ltMsgGetVar_s(ltMsgPk,"rname"));
				 sprintf(_ltPubInfo->_FilterListHead._ltFormNoMon[atol(carid)].caFrom,"%s",ltMsgGetVar_s(ltMsgPk,"formfrom"));
				 sprintf(_ltPubInfo->_FilterListHead._ltFormNoMon[atol(carid)].caHost,"%s",ltMsgGetVar_s(ltMsgPk,"formhost"));
				 sprintf(_ltPubInfo->_FilterListHead._ltFormNoMon[atol(carid)].caURL,"%s",ltMsgGetVar_s(ltMsgPk,"formurl"));
				 _ltPubInfo->_FilterListHead._ltFormNoMon[atol(carid)].lMaction=atol(ltMsgGetVar_s(ltMsgPk,"maction"));
				 snprintf(caMsg,255,"%s","update msaformfilterOne success.");
			}
				 logWrite(ltMsgPk,caMsg,3);
				 ltMsgPk->msgpktype=1;
			   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
			   ltMsgFree(ltMsgPk);
			   return 0;
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

    sprintf(strBuf,"select count(*) from msaformfilterOne");
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
    sprintf(strBuf,"select rid,num,rname,formfrom,formhost,formurl,maction from msaformfilterOne limit %lu offset %lu ",lRowNum,lStartRec);
    tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {
          if(k==0)
					{
            sprintf(caTmpp,"{\"rid\":\"%s\",\"num\":\"%s\",\"rname\":\"%s\",\"formfrom\":\"%s\",\"formhost\":\"%s\",\"formurl\":\"%s\",\"maction\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6]);
						stralloc_cats(&strTemp,caTmpp);
					}
					else
					{
				    sprintf(caTmpp,",{\"rid\":\"%s\",\"num\":\"%s\",\"rname\":\"%s\",\"formfrom\":\"%s\",\"formhost\":\"%s\",\"formurl\":\"%s\",\"maction\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6]);
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
   ltDbClose(G_DbCon);
   return 0;
}

int msaShowFormOne(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char *my_id;
	 char sqlBuf[2048];
   ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   ltDbCursor *tempCursor;
   LT_DBROW   tempRow;
   my_id=ltMsgGetVar_s(ltMsgPk,"my_id");

   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
			ltMsgFree(ltMsgPk);
			return 0;
	 }

   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   if(my_id!=NULL){
		sprintf(sqlBuf,"select num,rname,formfrom,formhost,formurl,maction from msaformfilterOne where rid='%s'",my_id);
			 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			 if(tempCursor!=NULL)
			 {
					 tempRow= ltDbFetchRow(tempCursor);
					 if(tempRow!=NULL)
					 	{
						 lt_dbput_rootvars(dbPtr,1,"rid",my_id);

						 char str[16];
						 memset(str,0,sizeof(str));
						 sprintf(str,"num%ssel",tempRow[0]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");

						 lt_dbput_rootvars(dbPtr,1,"rname",tempRow[1]);
						 lt_dbput_rootvars(dbPtr,1,"formfrom",tempRow[2]);
						 lt_dbput_rootvars(dbPtr,1,"formhost",tempRow[3]);
						 lt_dbput_rootvars(dbPtr,1,"formurl",tempRow[4]);

						 memset(str,0,sizeof(str));
						 sprintf(str,"maction%ssel",tempRow[5]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");

						 lt_dbput_rootvars(dbPtr,1,"my_id",my_id);
					 }
				 ltDbCloseCursor(tempCursor);
			 }
   }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/updateFormOnePage.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   ltDbClose(G_DbCon);
   return 0;
}

/////////////////////////////////////////////////////////////

/*
**时间策略加载页面
*/
int ltschinfolist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char sqlBuf[1024];
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char mytime[400];
    memset(mytime,0,sizeof(mytime));

    char *dbUser;
		 char *dbPass;
		 char *dbName;
		 dbName=_ltPubInfo->_dbname;
		 dbUser=_ltPubInfo->_dbuser;
		 dbPass=_ltPubInfo->_dbpass;
		 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		 if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
				ltMsgFree(ltMsgPk);
				return 0;
		 }

    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    tablePtr=lt_dbput_table(dbPtr,"infolist");
    sprintf(sqlBuf,"select schid,schname,schdesc,schtime,schtime1 from nasschedule");
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
              tempRow= ltDbFetchRow(tempCursor);
              while(tempRow!=NULL){
                       sprintf(mytime,"%s%s",tempRow[3],tempRow[4]);
                       lt_dbput_recordvars(tablePtr,4,
                                "schid",LT_TYPE_STRING,tempRow[0],
                                "schname",LT_TYPE_STRING,tempRow[1],
                                "schdesc",LT_TYPE_STRING,tempRow[2],
                                "schtime",LT_TYPE_STRING,mytime);
                       tempRow= ltDbFetchRow(tempCursor);
              }
              ltDbCloseCursor(tempCursor);
    }
    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/schedit.htm",dbPtr->head,0);
    ltMsgFree(ltMsgPk);
    lt_dbfree(dbPtr);
    ltDbClose(G_DbCon);
    return 0;
}
/*
**添加新的时间策略
*/
int ltaddsch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char sqlBuf[1024];
	 ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
   char caMsg[256];
   char mytime1[200];
   char mytime2[200];
   memset(mytime1,0,sizeof(mytime1));
   memset(mytime2,0,sizeof(mytime2));
   memcpy(mytime1,ltMsgGetVar_s(ltMsgPk,"schtime"),198);
   memcpy(mytime2,ltMsgGetVar_s(ltMsgPk,"schtime")+198,198);
   long lCount=0;

   	 char *dbUser;
		 char *dbPass;
		 char *dbName;
		 dbName=_ltPubInfo->_dbname;
		 dbUser=_ltPubInfo->_dbuser;
		 dbPass=_ltPubInfo->_dbpass;
		 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		 if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
				ltMsgFree(ltMsgPk);
				return 0;
		 }

   sprintf(sqlBuf,"select count(*) from nasschedule");
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
    sprintf(sqlBuf,"select max(schid)+1 from nasschedule");
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
    sprintf(sqlBuf,"insert into nasschedule values (%ld,'%s','%s','%s','%s')",lCount,
                      ltMsgGetVar_s(ltMsgPk,"schname"),
                      ltMsgGetVar_s(ltMsgPk,"schdesc"),
                      mytime1,mytime2);
    ltDbExecSql(G_DbCon,sqlBuf);
    ///内存更新
		_ltPubInfo->_schtimelist[lCount].schid=lCount;
		sprintf(_ltPubInfo->_schtimelist[lCount].schname,"%s",ltMsgGetVar_s(ltMsgPk,"schname"));
		sprintf(_ltPubInfo->_schtimelist[lCount].schdesc,"%s",ltMsgGetVar_s(ltMsgPk,"schdesc"));
		sprintf(_ltPubInfo->_schtimelist[lCount].schtime,"%s%s",mytime1,mytime2);

    snprintf(caMsg,255,"%s","add nasschedule success.");
	  logWrite(ltMsgPk,caMsg,3);
	  ltDbClose(G_DbCon);
	  ltschinfolist(confd,ltMsgPk,lt_MMHead);
 return 0;
}
/*
**修改时间策略
*/
int ltupdatesch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char sqlBuf[1024];
   char caMsg[256];
   char mytime1[200];
   char mytime2[200];

   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
				ltMsgFree(ltMsgPk);
				return 0;
	 }

   memset(mytime1,0,sizeof(mytime1));
   memset(mytime2,0,sizeof(mytime2));
   memcpy(mytime1,ltMsgGetVar_s(ltMsgPk,"schtime"),198);
   memcpy(mytime2,ltMsgGetVar_s(ltMsgPk,"schtime")+198,198);
   sprintf(sqlBuf,"update nasschedule set schname='%s',schdesc='%s',schtime='%s',schtime1='%s' where schid=%s",
                      ltMsgGetVar_s(ltMsgPk,"schname"),
                      ltMsgGetVar_s(ltMsgPk,"schdesc"),
                      mytime1,mytime2,
                      ltMsgGetVar_s(ltMsgPk,"schid"));
   ltDbExecSql(G_DbCon,sqlBuf);

	 sprintf(_ltPubInfo->_schtimelist[atol(ltMsgGetVar_s(ltMsgPk,"schid"))].schname,"%s",ltMsgGetVar_s(ltMsgPk,"schname"));
	 sprintf(_ltPubInfo->_schtimelist[atol(ltMsgGetVar_s(ltMsgPk,"schid"))].schdesc,"%s",ltMsgGetVar_s(ltMsgPk,"schdesc"));
	 sprintf(_ltPubInfo->_schtimelist[atol(ltMsgGetVar_s(ltMsgPk,"schid"))].schtime,"%s%s",mytime1,mytime2);

   snprintf(caMsg,255,"%s","update nasschedule success.");
	 logWrite(ltMsgPk,caMsg,3);
	 ltDbClose(G_DbCon);
   ltschinfolist(confd,ltMsgPk,lt_MMHead);
   return 0;
}
/*
**删除时间策略
*/
int ltdelsch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
        char sqlBuf[1048];
        char caMsg[256];
        char *dbUser;
				 char *dbPass;
				 char *dbName;
				 dbName=_ltPubInfo->_dbname;
				 dbUser=_ltPubInfo->_dbuser;
				 dbPass=_ltPubInfo->_dbpass;
				 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
				 if(G_DbCon==NULL){
							ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
							ltMsgFree(ltMsgPk);
							return 0;
				 }
        sprintf(sqlBuf,"delete from nasschedule where schid in (%s) ",
        ltMsgGetVar_s(ltMsgPk,"checkbox"));
        ltDbExecSql(G_DbCon,sqlBuf);

        _ltPubInfo->_schtimelist[atol(ltMsgGetVar_s(ltMsgPk,"checkbox"))].schid=0;
        sprintf(_ltPubInfo->_schtimelist[atol(ltMsgGetVar_s(ltMsgPk,"checkbox"))].schname,"%s","");
			  sprintf(_ltPubInfo->_schtimelist[atol(ltMsgGetVar_s(ltMsgPk,"checkbox"))].schdesc,"%s","");
			  sprintf(_ltPubInfo->_schtimelist[atol(ltMsgGetVar_s(ltMsgPk,"checkbox"))].schtime,"%s","");

			  snprintf(caMsg,255,"%s","delete nasschedule success.");
				logWrite(ltMsgPk,caMsg,3);
				ltDbClose(G_DbCon);
        ltschinfolist(confd,ltMsgPk,lt_MMHead);
        return 0;
}
/*
**节假日加载页面
*/
int ltjjrinfolist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 	 char sqlBuf[1024];
   ltDbHeadPtr dbPtr;
   ltTablePtr tablePtr;
   ltDbCursor *tempCursor;
   LT_DBROW tempRow;
   char mytime[400];

   char *dbUser;
				 char *dbPass;
				 char *dbName;
				 dbName=_ltPubInfo->_dbname;
				 dbUser=_ltPubInfo->_dbuser;
				 dbPass=_ltPubInfo->_dbpass;
				 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
				 if(G_DbCon==NULL){
							ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
							ltMsgFree(ltMsgPk);
							return 0;
				 }

   memset(mytime,0,sizeof(mytime));
//   if(checkRight(confd,ltMsgPk,21)==-1){//权限判断
//             return -1;
//   }
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   tablePtr=lt_dbput_table(dbPtr,"infolist");
   sprintf(sqlBuf,"select jjrid,jjrname,jjrdesc,jjrtime,jjrtime1 from nasjjr");
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
   if(tempCursor!=NULL){
             tempRow= ltDbFetchRow(tempCursor);
             while(tempRow!=NULL){
                      sprintf(mytime,"%s%s",tempRow[3],tempRow[4]);
                      lt_dbput_recordvars(tablePtr,4,
                               "jjrid",LT_TYPE_STRING,tempRow[0],
                               "jjrname",LT_TYPE_STRING,tempRow[1],
                               "jjrdesc",LT_TYPE_STRING,tempRow[2],
                               "jjrtime",LT_TYPE_STRING,mytime);
                      tempRow= ltDbFetchRow(tempCursor);
             }
             ltDbCloseCursor(tempCursor);
   }
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/jjredit.htm",dbPtr->head,0);
	 ltMsgFree(ltMsgPk);
	 lt_dbfree(dbPtr);
	 ltDbClose(G_DbCon);
	 return 0;
}


/*
**添加新的节假日
*/
int ltaddjjr(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char sqlBuf[1024];
	 ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
   char caMsg[256];
   char mytime1[200];
   char mytime2[200];
   memset(mytime1,0,sizeof(mytime1));
   memset(mytime2,0,sizeof(mytime2));
   memcpy(mytime1,ltMsgGetVar_s(ltMsgPk,"jjrtime"),198);
   memcpy(mytime2,ltMsgGetVar_s(ltMsgPk,"jjrtime")+198,198);
   long lCount=0;

   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
							ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
							ltMsgFree(ltMsgPk);
							return 0;
	 }

   sprintf(sqlBuf,"select count(*) from nasjjr");
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
    sprintf(sqlBuf,"select max(jjrid)+1 from nasjjr");
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
    sprintf(sqlBuf,"insert into nasjjr values (%ld,'%s','%s','%s','%s')",lCount,
                      ltMsgGetVar_s(ltMsgPk,"jjrname"),
                      ltMsgGetVar_s(ltMsgPk,"jjrdesc"),
                      mytime1,mytime2);
    ltDbExecSql(G_DbCon,sqlBuf);
    ///内存更新
		_ltPubInfo->_jjrtimelist[lCount].jjrid=lCount;
		sprintf(_ltPubInfo->_jjrtimelist[lCount].jjrname,"%s",ltMsgGetVar_s(ltMsgPk,"jjrname"));
		sprintf(_ltPubInfo->_jjrtimelist[lCount].jjrdesc,"%s",ltMsgGetVar_s(ltMsgPk,"jjrdesc"));
		sprintf(_ltPubInfo->_jjrtimelist[lCount].jjrtime,"%s%s",mytime1,mytime2);

		snprintf(caMsg,255,"%s","add nasjjr success.");
	  logWrite(ltMsgPk,caMsg,3);
	  ltDbClose(G_DbCon);
	  ltjjrinfolist(confd,ltMsgPk,lt_MMHead);
    return 0;
}

/*
**修改节假日
*/
int ltupdatejjr(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char sqlBuf[1024];
   char caMsg[256];
   char mytime1[200];
   char mytime2[200];

   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
							ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
							ltMsgFree(ltMsgPk);
							return 0;
	 }

   memset(mytime1,0,sizeof(mytime1));
   memset(mytime2,0,sizeof(mytime2));
   memcpy(mytime1,ltMsgGetVar_s(ltMsgPk,"jjrtime"),198);
   memcpy(mytime2,ltMsgGetVar_s(ltMsgPk,"jjrtime")+198,198);
   sprintf(sqlBuf,"update nasjjr set jjrname='%s',jjrdesc='%s',jjrtime='%s',jjrtime1='%s' where jjrid=%s",
                      ltMsgGetVar_s(ltMsgPk,"jjrname"),
                      ltMsgGetVar_s(ltMsgPk,"jjrdesc"),
                      mytime1,mytime2,
                      ltMsgGetVar_s(ltMsgPk,"jjrid"));
   ltDbExecSql(G_DbCon,sqlBuf);
   ///内存更新
   _ltPubInfo->_jjrtimelist[atol(ltMsgGetVar_s(ltMsgPk,"jjrid"))].jjrid=0;
	 sprintf(_ltPubInfo->_jjrtimelist[atol(ltMsgGetVar_s(ltMsgPk,"jjrid"))].jjrname,"%s",ltMsgGetVar_s(ltMsgPk,"jjrname"));
	 sprintf(_ltPubInfo->_jjrtimelist[atol(ltMsgGetVar_s(ltMsgPk,"jjrid"))].jjrdesc,"%s",ltMsgGetVar_s(ltMsgPk,"jjrdesc"));
	 sprintf(_ltPubInfo->_jjrtimelist[atol(ltMsgGetVar_s(ltMsgPk,"jjrid"))].jjrtime,"%s%s",mytime1,mytime2);

	 snprintf(caMsg,255,"%s","update nasjjr success.");
	 logWrite(ltMsgPk,caMsg,3);
	 ltDbClose(G_DbCon);
   ltjjrinfolist(confd,ltMsgPk,lt_MMHead);
   return 0;
}
/*
**删除节假日
*/
int ltdeljjr(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char sqlBuf[1048];
    char caMsg[256];
    char *dbUser;
	  char *dbPass;
	  char *dbName;
	  dbName=_ltPubInfo->_dbname;
	  dbUser=_ltPubInfo->_dbuser;
	  dbPass=_ltPubInfo->_dbpass;
	  G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	  if(G_DbCon==NULL){
							ltMsgPk->msgpktype=1;
							lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
							ltMsgFree(ltMsgPk);
							return 0;
	  }
    sprintf(sqlBuf,"delete from nasjjr where jjrid in (%s) ",
    ltMsgGetVar_s(ltMsgPk,"checkbox"));
    ltDbExecSql(G_DbCon,sqlBuf);
    ///内存更新
		_ltPubInfo->_jjrtimelist[atol(ltMsgGetVar_s(ltMsgPk,"checkbox"))].jjrid=0;
		sprintf(_ltPubInfo->_jjrtimelist[atol(ltMsgGetVar_s(ltMsgPk,"checkbox"))].jjrname,"%s","");
		sprintf(_ltPubInfo->_jjrtimelist[atol(ltMsgGetVar_s(ltMsgPk,"checkbox"))].jjrdesc,"%s","");
		sprintf(_ltPubInfo->_jjrtimelist[atol(ltMsgGetVar_s(ltMsgPk,"checkbox"))].jjrtime,"%s","");

		snprintf(caMsg,255,"%s","delete nasjjr success.");
	  logWrite(ltMsgPk,caMsg,3);
	  ltDbClose(G_DbCon);
    ltjjrinfolist(confd,ltMsgPk,lt_MMHead);
    return 0;
}

/* 初始化时间服务页面  */
int lttimesetting(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   ltDbHeadPtr dbPtr;
   char ENABLENTP[24];
   char TIMEZONE[24];
   char ntpadd1[24];
   char ntpadd2[24];
   struct tm * sTim;
	 char   pFormat[24];
	 long   lTemp;
	 long   lTime;
	 char str[16];

   char *pFile="/etc/msa/msa/time/settings";
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   lTime=time(0);

   memset(TIMEZONE,0,sizeof(TIMEZONE));
   bcCnfGetValue_s(pFile,"TIMEZONE",TIMEZONE);
	 memset(str,0,sizeof(str));
	 sprintf(str,"timezone%ssel",TIMEZONE);
	 lt_dbput_rootvars(dbPtr,1,str,"selected");


   memset(ENABLENTP,0,sizeof(ENABLENTP));
   bcCnfGetValue_s(pFile,"ENABLENTP",ENABLENTP);
   if( strcmp(ENABLENTP,"YES")==0){
       lt_dbput_rootvars(dbPtr,1,"ENABLENTPSEL","checked");
   }else{
       lt_dbput_rootvars(dbPtr,1,"ENABLENTPSEL"," ");
   }
   memset(ntpadd1,0,sizeof(ntpadd1));
   bcCnfGetValue_s(pFile,"NTP_ADDR_1",ntpadd1);
   lt_dbput_rootvars(dbPtr,1,"NTP_ADDR_1",ntpadd1 );
   memset(ntpadd2,0,sizeof(ntpadd2));
   bcCnfGetValue_s(pFile,"NTP_ADDR_2",ntpadd2);
   lt_dbput_rootvars(dbPtr,1,"NTP_ADDR_2",ntpadd2);
   sTim = localtime(&lTime);
	 lTemp = sTim->tm_year;
	 if(lTemp > 100) lTemp = lTemp - 100;
	 if(lTemp<10){
	     sprintf(pFormat,"200%ld",lTemp);
	 }else{
       sprintf(pFormat,"20%ld",lTemp);
	 }
	 lt_dbput_rootvars(dbPtr,1,"year",pFormat);
	 lTemp = sTim->tm_mon + 1;
	 if(lTemp<10){
       sprintf(pFormat,"0%ld",lTemp);
	 }else{
       sprintf(pFormat,"%ld",lTemp);
	 }
   lt_dbput_rootvars(dbPtr,1,"month",pFormat);
   lTemp = sTim->tm_mday;
	 if(lTemp<10){
       sprintf(pFormat,"0%ld",lTemp);
	 }else{
       sprintf(pFormat,"%ld",lTemp);
   }
   lt_dbput_rootvars(dbPtr,1,"day",pFormat);
   lTemp = sTim->tm_hour;
   if(lTemp<10){
       sprintf(pFormat,"0%ld",lTemp);
   }else{
       sprintf(pFormat,"%ld",lTemp);
   }
   lt_dbput_rootvars(dbPtr,1,"day",pFormat);
   lTemp = sTim->tm_hour;
   if(lTemp<10){
       sprintf(pFormat,"0%ld",lTemp);
   }else{
       sprintf(pFormat,"%ld",lTemp);
   }
   lt_dbput_rootvars(dbPtr,1,"hour",pFormat);
   lTemp = sTim->tm_min;
   if(lTemp<10){
       sprintf(pFormat,"0%ld",lTemp);
   }else{
       sprintf(pFormat,"%ld",lTemp);
   }
   lt_dbput_rootvars(dbPtr,1,"minute",pFormat);
   lTemp = sTim->tm_sec;
	 if(lTemp<10){
	     sprintf(pFormat,"0%ld",lTemp);
	 }else{
	     sprintf(pFormat,"%ld",lTemp);
	 }
   lt_dbput_rootvars(dbPtr,1,"second",pFormat);
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/timeface.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}

/*更改服务配置*/
int lttimeok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char    *pFile="/etc/msa/msa/time/settings";
   char    *ntpadd1;
   char    *ntpadd2;
   char    *TIMEZONE;
   char    *ENABLENTP;
   char    caCmdCon[256];
   ntpadd1=ltMsgGetVar_s(ltMsgPk,"NTP_ADDR_1");
   ntpadd2=ltMsgGetVar_s(ltMsgPk,"NTP_ADDR_2");
   TIMEZONE=ltMsgGetVar_s(ltMsgPk,"TIMEZONE");
   ENABLENTP=ltMsgGetVar_s(ltMsgPk,"ENABLENTP");
   if(ENABLENTP==NULL){
   ENABLENTP="NO";
   }
   system("/bin/cp /etc/msa/msa/time/settings /etc/msa/msa/time/settings.old");
   /*连接方式*/
   sprintf(caCmdCon,"echo 'TIMEZONE=%s' > %s",TIMEZONE,pFile);
   system(caCmdCon);
   sprintf(caCmdCon,"echo 'ENABLENTP=%s' >> %s",ENABLENTP,pFile);
   system(caCmdCon);
   sprintf(caCmdCon,"echo 'NTP_ADDR_1=%s' >> %s",ntpadd1,pFile);
   system(caCmdCon);
   sprintf(caCmdCon,"echo 'NTP_ADDR_2=%s' >> %s",ntpadd2,pFile);
   system(caCmdCon);
   if(strcmp(TIMEZONE,"-12")==0){//埃尼威托克岛，瓜加林岛代表
   		system("/bin/cp /usr/share/zoneinfo/Kwajalein /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-11")==0){//中途岛，萨摩亚群岛
   		system("/bin/cp /usr/share/zoneinfo/US/Samoa /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-10")==0){//夏威夷代表
   		system("/bin/cp /usr/share/zoneinfo/US/Hawaii /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-9")==0){//阿拉斯加代表
   		system("/bin/cp /usr/share/zoneinfo/US/Alaska /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-8")==0){//蒂华纳，加利福尼亚代表
   		system("/bin/cp /usr/share/zoneinfo/America/Tijuana /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-7")==0){//亚利桑那州(美国西南部的州)代表
   		system("/bin/cp /usr/share/zoneinfo/US/Arizona /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-6")==0){//中部时间（美国和加拿大）代表
   		system("/bin/cp /usr/share/zoneinfo/US/Central /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-5")==0){//波哥大，利马，里奥布朗库代表
   		system("/bin/cp /usr/share/zoneinfo/America/Bogota /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-4")==0){//大西洋时间（加拿大）代表
   		system("/bin/cp /usr/share/zoneinfo/Canada/Atlantic /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-3.5")==0){//纽芬兰代表
   		system("/bin/cp /usr/share/zoneinfo/Canada/Newfoundland /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-3")==0){//巴西利亚代表/////////////////////////////////
   		system("/bin/cp /usr/share/zoneinfo/Asia/Tokyo /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-2")==0){//中大西洋代表
   		system("/bin/cp /usr/share/zoneinfo/Canada/Atlantic /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"-1")==0){//亚速尔群岛代表
   		system("/bin/cp /usr/share/zoneinfo/Atlantic/Azores /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"0")==0){//格林威治标准时：都柏林，爱丁堡，伦敦，里斯本
   		system("/bin/cp /usr/share/zoneinfo/Europe/Dublin /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"1")==0){//阿姆斯特丹，柏林，伯尔尼代表
   		system("/bin/cp /usr/share/zoneinfo/Europe/Berlin /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"2")==0){//雅典，布加勒斯特，伊斯坦布尔代表
   		system("/bin/cp /usr/share/zoneinfo/Europe/Athens /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"3")==0){//巴格达， 科威特， 利雅得代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Baghdad /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"4")==0){//阿布扎比，巴库，马斯喀特代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Baku /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"4.5")==0){//喀布尔代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Kabul /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"5")==0){//叶卡特琳堡，伊斯兰堡，卡拉奇，塔什干代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Yekaterinburg /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"5.5")==0){//新德里代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Dili /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"6")==0){//阿斯塔纳，达卡，阿拉木图，新西伯利亚代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Dhaka /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"7")==0){//曼谷，河内，雅加达代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Bangkok /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"8")==0){//北京，重庆，香港，乌鲁木齐，台北代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Shanghai /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"9")==0){//日本，韩国代表
   		system("/bin/cp /usr/share/zoneinfo/Asia/Tokyo /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"9.5")==0){//阿德莱德，达尔文代表
   		system("/bin/cp /usr/share/zoneinfo/Australia/Adelaide /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"10")==0){//堪培拉，关岛，莫尔兹比港代表
   		system("/bin/cp /usr/share/zoneinfo/Australia/Atlantic /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"11")==0){//马伽迪斯琴，索罗门群岛代表、、、、、、、、、、、、、、、、、、、、
   		system("/bin/cp /usr/share/zoneinfo/Asia/Shanghai /etc/localtime");
 	 }else if(strcmp(TIMEZONE,"12")==0){//斐济，堪察加半岛，马绍尔群岛代表
   		system("/bin/cp /usr/share/zoneinfo/Pacific/Fiji /etc/localtime");
 	 }
   lttimesetting(confd,ltMsgPk,lt_MMHead);
   return 0;
}
/*更改时间*/
int lttimeset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char    *year;
   char    *month;
   char    *day;
   char    *hour;
   char    *minute;
   char    *second;
   char    caCmdCon[256];
   year=ltMsgGetVar_s(ltMsgPk,"year");
   month=ltMsgGetVar_s(ltMsgPk,"month");
   day=ltMsgGetVar_s(ltMsgPk,"day");
   hour=ltMsgGetVar_s(ltMsgPk,"hour");
   minute=ltMsgGetVar_s(ltMsgPk,"minute");
   second=ltMsgGetVar_s(ltMsgPk,"second");
   sprintf(caCmdCon,"/bin/date -s '%s-%s-%s'",year,month,day);
   system(caCmdCon);
   sprintf(caCmdCon,"/bin/date -s '%s:%s:%s'",hour,minute,second);
   system(caCmdCon);
   lttimesetting(confd,ltMsgPk,lt_MMHead);
   return 0;
}

/*
*附加路由
*/
int msaRout(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >System Upgrading......<br><a href=\"javascript:history.back(1);\" >BACK</a></body></html>");
	ltMsgFree(ltMsgPk);
	return 0;
}

/*
*表单记录级别
*/
int ltformlevelok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	ltDbHeadPtr dbPtr;
	char caMsg[256];
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");

  char *mylevel;//记录级别

  mylevel=ltMsgGetVar_s(ltMsgPk,"level");
  if(mylevel==NULL){
    mylevel="0";
  }
	char caCmdCon[128];
  memset(caCmdCon,0,sizeof(caCmdCon));
	sprintf(caCmdCon,"echo 'level=%s' > %s",mylevel,"/etc/msa/msa/option/formloglevel");
	system(caCmdCon);

	_ltPubInfo->_msaOptionHead.formloglevel==atol(mylevel);

	snprintf(caMsg,255,"%s","update formLevel setting success.");
	logWrite(ltMsgPk,caMsg,3);
	ltformlevel(confd,ltMsgPk,lt_MMHead);
	return 0;

}

int ltformlevel(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  char mylevel[16];
		ltDbHeadPtr dbPtr;
		dbPtr=lt_dbinit();
		lt_db_htmlpage(dbPtr,"utf-8");
		//记录级别
		memset(mylevel,0,sizeof(mylevel));
	  bcCnfGetValue_s("/etc/msa/msa/option/formloglevel","level",mylevel);
	  if(strcmp(mylevel,"1")==0){
	     lt_dbput_rootvars(dbPtr,1,"level1radio","checked");
	  }else if(strcmp(mylevel,"2")==0){
	     lt_dbput_rootvars(dbPtr,1,"level2radio","checked");
	  }else if(strcmp(mylevel,"3")==0){
	     lt_dbput_rootvars(dbPtr,1,"level3radio","checked");
	  }else if(strcmp(mylevel,"4")==0){
	     lt_dbput_rootvars(dbPtr,1,"level4radio","checked");
	  }else if(strcmp(mylevel,"5")==0){
	     lt_dbput_rootvars(dbPtr,1,"level5radio","checked");
	  }else{
	  	 lt_dbput_rootvars(dbPtr,1,"level1radio"," ");
	  	 lt_dbput_rootvars(dbPtr,1,"level2radio"," ");
	  	 lt_dbput_rootvars(dbPtr,1,"level3radio"," ");
	  	 lt_dbput_rootvars(dbPtr,1,"level4radio"," ");
	  	 lt_dbput_rootvars(dbPtr,1,"level5radio"," ");
	  }

		lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/formlevel.htm",dbPtr->head,0);
		ltMsgFree(ltMsgPk);
		lt_dbfree(dbPtr);
		return 0;

}

/*
*邮件记录级别
*/
int ltmaillevelok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	ltDbHeadPtr dbPtr;
	char caMsg[256];
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
  char *mylevel;//记录级别
  mylevel=ltMsgGetVar_s(ltMsgPk,"level");
  if(mylevel==NULL){
    mylevel="0";
  }

	char caCmdCon[128];
  memset(caCmdCon,0,sizeof(caCmdCon));
	sprintf(caCmdCon,"echo 'level=%s' > %s",mylevel,"/etc/msa/msa/option/mailloglevel");
	system(caCmdCon);

	_ltPubInfo->_msaOptionHead.mailloglevel=atol(mylevel);//mailloglevel

	snprintf(caMsg,255,"%s","update maillevel setting success.");
	logWrite(ltMsgPk,caMsg,3);
	ltmaillevel(confd,ltMsgPk,lt_MMHead);
	return 0;

}

int ltmaillevel(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  char mylevel[16];
		ltDbHeadPtr dbPtr;
		dbPtr=lt_dbinit();
		lt_db_htmlpage(dbPtr,"utf-8");
		//记录级别
		memset(mylevel,0,sizeof(mylevel));
	  bcCnfGetValue_s("/etc/msa/msa/option/mailloglevel","level",mylevel);
	  if(strcmp(mylevel,"1")==0){
	     lt_dbput_rootvars(dbPtr,1,"level1radio","checked");
	  }else if(strcmp(mylevel,"2")==0){
	     lt_dbput_rootvars(dbPtr,1,"level2radio","checked");
	  }else if(strcmp(mylevel,"3")==0){
	     lt_dbput_rootvars(dbPtr,1,"level3radio","checked");
	  }else if(strcmp(mylevel,"4")==0){
	     lt_dbput_rootvars(dbPtr,1,"level4radio","checked");
	  }else if(strcmp(mylevel,"5")==0){
	     lt_dbput_rootvars(dbPtr,1,"level5radio","checked");
	  }else{
	  	 lt_dbput_rootvars(dbPtr,1,"level1radio"," ");
	  	 lt_dbput_rootvars(dbPtr,1,"level2radio"," ");
	  	 lt_dbput_rootvars(dbPtr,1,"level3radio"," ");
	  	 lt_dbput_rootvars(dbPtr,1,"level4radio"," ");
	  	 lt_dbput_rootvars(dbPtr,1,"level5radio"," ");
	  }

		lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/maillevel.htm",dbPtr->head,0);
		ltMsgFree(ltMsgPk);
		lt_dbfree(dbPtr);
		return 0;
}
int creatMsakey(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
				char *dbUser;
			  char *dbPass;
			  char *dbName;
			  dbName=_ltPubInfo->_dbname;
				dbUser=_ltPubInfo->_dbuser;
				dbPass=_ltPubInfo->_dbpass;
				G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
				if(G_DbCon!=NULL){
					//printf("db connect ok\n");
				}else{
					fprintf(stderr,"db connect error\n");
				}
		char str[128],sqlBuf[128],tempstr[64],caLabel[128];
		int fdwrite=-1;
		memset(str,0,sizeof(str));
		memset(sqlBuf,0,sizeof(sqlBuf));
		memset(tempstr,0,sizeof(tempstr));
		memset(caLabel,0,sizeof(caLabel));
    LT_DBROW tempRow;
	  ltDbCursor *tempCursor;
	  sprintf(tempstr,"%s",ltMsgGetVar_s(ltMsgPk,"uname"));
	  ltMd5Encrypto(tempstr,strlen(tempstr),str);
		sprintf(sqlBuf,"select password from nasadmuser where name='%s'",ltMsgGetVar_s(ltMsgPk,"uname"));
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow){
				sprintf(str,"%s%s",str,tempRow[0]);
			}
			ltDbCloseCursor(tempCursor);
		}
		memset(tempstr,0,sizeof(tempstr));
		sprintf(tempstr,"%s/msakey.dat","/datacenter/msa");
		sprintf(caLabel,"/bin/rm  %s",tempstr);
	  system(caLabel);
		sleep(1);

    fdwrite = open(tempstr, O_APPEND | O_WRONLY | O_CREAT, 0644);
		if(fdwrite == (-1)) {
			 ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"-1");
			 ltMsgFree(ltMsgPk);
			 return 0;
		}
	  write(fdwrite,str,strlen(str));
	  close(fdwrite);
	  sleep(1);
	  ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
	  ltMsgFree(ltMsgPk);
	  return 0;
}

int addsysconf(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		char path[128],info[128],caLabel[128],caTempDir[128];
		char *myipmon,*myhttpmon,*myipreportmon,*myblockmon,*myimmon,*myfilemon,*mysearchmon,*myusermon;
		memset(path,0,sizeof(path));
		memset(info,0,sizeof(info));
		memset(caLabel,0,sizeof(caLabel));
		memset(caTempDir,0,sizeof(caTempDir));
		if(ltMsgGetVar_s(ltMsgPk,"info")!=NULL){
				sprintf(info,"%s",ltMsgGetVar_s(ltMsgPk,"info"));
	  }
	  printf("info %s\n",ltMsgGetVar_s(ltMsgPk,"info"));
	  myipmon=ltMsgGetVar_s(ltMsgPk,"ipmon");
	  myhttpmon=ltMsgGetVar_s(ltMsgPk,"httpmon");
	  myipreportmon=ltMsgGetVar_s(ltMsgPk,"ipreportmon");
	  myblockmon=ltMsgGetVar_s(ltMsgPk,"blockmon");
	  myimmon=ltMsgGetVar_s(ltMsgPk,"immon");
	  myfilemon=ltMsgGetVar_s(ltMsgPk,"filemon");
	  mysearchmon=ltMsgGetVar_s(ltMsgPk,"searchmon");
	  myusermon=ltMsgGetVar_s(ltMsgPk,"usermon");
	  sprintf(caTempDir,"%s/policytemplate",_datacenterdir);
	  if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            return 0;
        }
    }
    sprintf(caTempDir,"%s/%lu",caTempDir,ltStrGetId());
	  if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            return 0;
        }
    }
    memset(caLabel,0,sizeof(caLabel));
    sprintf(caLabel,"echo 'name=%s' > %s/info.ini",info,caTempDir);
		system(caLabel);
    memset(caLabel,0,sizeof(caLabel));
    sprintf(caLabel,"echo 'lright=%s%s%s%s%s%s%s%s' > %s/sysconf.dat",myipmon,myhttpmon,myipreportmon,myblockmon,myimmon,myfilemon,mysearchmon,myusermon,caTempDir);
		system(caLabel);
	  ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
	  ltMsgFree(ltMsgPk);
	  return 0;
}
int policytemplateJsonStore(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
		stralloc strTemp;
	  char caTmpp[2048];
	  char caFile1[256];
	 	char cDate[64];
	 	char cDate2[64];
		char reportname[64];
		char condition[64];
		char reportuser[64];
		char reportstate[123];
		char templname[256];
		char caCmd[256];
	  struct stat st;
	  msasDir *fnames;
	  char   *reportdir;
	  char   *filedir;
	  int    jjj;
	  int    len=0;
	  long 	 lRowNum,lStartRec,lSumRec;

	  lRowNum=100;
	  lStartRec=0;
	  lSumRec=0;

    reportdir=ltMsgGetVar_s(ltMsgPk,"reportdir");

   if((ltMsgGetVar_s(ltMsgPk,"file"))!=NULL){
			filedir=ltMsgGetVar_s(ltMsgPk,"file");
			sprintf(caCmd,"rm -Rf %s/%s/%s",_datacenterdir,reportdir,filedir);
	    system(caCmd);

	    ltMsgPk->msgpktype=1;
	    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n", "lthtml",LT_TYPE_STRING,"1");
	    ltMsgFree(ltMsgPk);
	    return 0;
	 }

	 sprintf(caFile1,"%s/%s/",_datacenterdir,reportdir);
	 fnames=mydirlist(caFile1,&len);
   if(fnames){
	    qsort(fnames,len,sizeof(msasDir),dirtimecmp);
   }

 	 if(ltMsgGetVar_s(ltMsgPk,"limit")){
    lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
   }

   if(ltMsgGetVar_s(ltMsgPk,"start")){
    lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
   }

	 strTemp.s=0;
	 sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",len);
	 stralloc_cats(&strTemp,caTmpp);
	 jjj=0;

	 while(len--)
	 {
	 			lSumRec++;
 				if(lSumRec>lStartRec){
 						if(strcmp(fnames[len].caDirName,"default")==0){
 								continue;
 						}
            jjj++;
            memset(condition,0,sizeof(condition));
            memset(reportname,0,sizeof(reportname));
				    memset(reportuser,0,sizeof(reportuser));
				    memset(templname,0,sizeof(templname));
				    memset(reportstate,0,sizeof(reportstate));
				    memset(cDate,0,sizeof(cDate));
				    memset(cDate2,0,sizeof(cDate2));
					  sprintf(caFile1,"%s/%s/%s/info.ini",_datacenterdir,reportdir,fnames[len].caDirName);
				    bcCnfGetValue_s(caFile1,"date",cDate);
				    bcCnfGetValue_s(caFile1,"date",cDate2);
				    bcCnfGetValue_s(caFile1,"name",reportname);
				    bcCnfGetValue_s(caFile1,"condition",condition);
				    cDate2[10]='\0';
					  sprintf(reportstate,"报告已生成");
					  sprintf(caFile1,"%s/%s/%s/errmsg.ini",_datacenterdir,reportdir,fnames[len].caDirName);
				    if(stat(caFile1,&st) == 0){
				    	sprintf(caFile1,"<a href=\"/%s/%s/errmsg.ini\">Error:生成报告错误!</a>",reportdir,fnames[len].caDirName);
						  sprintf(reportstate,caFile1);
				    }
				    sprintf(caFile1,"%s/%s/%s/proccess.ini",_datacenterdir,reportdir,fnames[len].caDirName);
				    if(stat(caFile1,&st) == 0){
							sprintf(reportstate,"报告正在生成中");
				    }
				    strgb2utf8(reportstate,templname,strlen(reportstate));
			      sprintf(reportstate,"%s",templname);
            memset(caTmpp,0,sizeof(caTmpp));
						if(jjj==1){
							sprintf(caTmpp,"{\"reportname\":\"%s\",\"dates\":\"%s\",\"reportdir\":\"%s\",\"bfile\":\"%s\",\"reportstate\":\"%s\",\"bydate\":\"%s\",\"condition\":\"%s\"}",reportname,cDate,reportdir,fnames[len].caDirName,reportstate,cDate2,condition);
			        stralloc_cats(&strTemp,caTmpp);
            }else{
              sprintf(caTmpp,",{\"reportname\":\"%s\",\"dates\":\"%s\",\"reportdir\":\"%s\",\"bfile\":\"%s\",\"reportstate\":\"%s\",\"bydate\":\"%s\",\"condition\":\"%s\"}",reportname,cDate,reportdir,fnames[len].caDirName,reportstate,cDate2,condition);
			        stralloc_cats(&strTemp,caTmpp);
            }

            if(lSumRec==(lRowNum+lStartRec)){
     						break;
     			  }
				}
	 }
	 stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);

	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;

}
int setsysconf(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
				char pathdir[32],lright[128],caCmdCon[512];
  			int myipmon,myhttpmon,myipreportmon,myblockmon,myimmon,myfilemon,mysearchmon,myusermon;//监视内容
  			memset(pathdir,0,sizeof(pathdir));
  			memset(lright,0,sizeof(lright));
  			sprintf(pathdir,"/datacenter/msa/policytemplate/%s/sysconf.dat",ltMsgGetVar_s(ltMsgPk,"pathdir"));
  			printf("%s\n",pathdir);
		    bcCnfGetValue_s(pathdir,"lright",lright);
		    printf("lright   %s\n",lright);

		    if(lright[0]=='1'){
		    	myipmon=1;
		    }else {
		    	myipmon=0;
		    }


		    if(lright[1]=='1'){
		    	myhttpmon=1;
		    }else {
		    	myhttpmon=0;
		    }


		    if(lright[2]=='1'){
		    	myipreportmon=1;
		    }else {
		    	myipreportmon=0;
		    }


		    if(lright[3]=='1'){
		    	myblockmon=1;
		    }else {
		    	myblockmon=0;
		    }


		    if(lright[4]=='1'){
		    	myimmon=1;
		    }else {
		    	myimmon=0;
		    }


		    if(lright[5]=='1'){
		    	myfilemon=1;
		    }else {
		    	myfilemon=0;
		    }


		    if(lright[6]=='1'){
		    	mysearchmon=1;
		    }else {
		    	mysearchmon=0;
		    }

		    if(lright[7]=='1'){
		    	myusermon=1;
		    }else {
		    	myusermon=0;
		    }

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'ipmon=%d' > %s",myipmon,"/etc/msa/msa/option/ipmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'httpmon=%d' > %s",myhttpmon,"/etc/msa/msa/option/httpmon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'ipreportmon=%d' > %s",myipreportmon,"/etc/msa/msa/option/ipreportmon");
				system(caCmdCon);


	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'blockmon=%d' > %s",myblockmon,"/etc/msa/msa/option/blockmon");
				system(caCmdCon);


	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'immon=%d' > %s",myimmon,"/etc/msa/msa/option/immon");
				system(caCmdCon);

	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'filemon=%d' > %s",myfilemon,"/etc/msa/msa/option/filemon");
				system(caCmdCon);


	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'searchmon=%d' > %s",mysearchmon,"/etc/msa/msa/option/searchmon");
				system(caCmdCon);


	      memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'usermon=%d' > %s",myusermon,"/etc/msa/msa/option/usermon");
				system(caCmdCon);

				//////更新内存
				_ltPubInfo->_msaOptionHead.ipmon=bcCnfGetValueL(_pubconfdir,"option","ipmon");
		    _ltPubInfo->_msaOptionHead.httpmon=bcCnfGetValueL(_pubconfdir,"option","httpmon");
		    _ltPubInfo->_msaOptionHead.ipreportmon=bcCnfGetValueL(_pubconfdir,"option","ipreportmon");
		    _ltPubInfo->_msaOptionHead.blockmon=bcCnfGetValueL(_pubconfdir,"option","blockmon");
		    _ltPubInfo->_msaOptionHead.immon=bcCnfGetValueL(_pubconfdir,"option","immon");
		    _ltPubInfo->_msaOptionHead.filemon=bcCnfGetValueL(_pubconfdir,"option","filemon");
		    _ltPubInfo->_msaOptionHead.searchmon=bcCnfGetValueL(_pubconfdir,"option","searchmon");
		    _ltPubInfo->_msaOptionHead.usermon=bcCnfGetValueL(_pubconfdir,"option","usermon"); //虚拟身份


   			memset(caCmdCon,0,sizeof(caCmdCon));
		    sprintf(caCmdCon,"%s","rm -Rf /datacenter/msa/policytemplate/default");
	    	system(caCmdCon);

				memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'filename=%s' > %s",ltMsgGetVar_s(ltMsgPk,"pathdir"),"/datacenter/msa/policytemplate/default");
				system(caCmdCon);

			  ltMsgPk->msgpktype=1;
			  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
			  ltMsgFree(ltMsgPk);
			  return 0;
}
/*
*设备重启
*/
int msaSysReboot(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    char sIp[24];
	  char strUserName[64];
	  char caMsg[256];
	  memset(sIp,0,sizeof(sIp));
	  memset(strUserName,0,sizeof(strUserName));
    ltMsgGetSomeNVar(ltMsgPk,2,"clientip", LT_TYPE_STRING, 19, sIp,"manageruser",LT_TYPE_STRING, 32,strUserName);
    sprintf(sIp,"%s",ltMsgGetVar_s(ltMsgPk,"clientip"));
  	sprintf(strUserName,"%s",ltMsgGetVar_s(ltMsgPk,"manageruser"));

    sprintf(caMsg,"%s reboot system success.",strUserName);
	  logWrite(ltMsgPk,caMsg,9);
	  system("/app/msa/bin/msashutdownall");
    system("/sbin/reboot");
    system("/sbin/reboot");
    return 0;
}
int viewsysconf(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		char pathdir[32],lright[128],name[128],str[128];
		memset(pathdir,0,sizeof(pathdir));
		memset(lright,0,sizeof(lright));
		memset(name,0,sizeof(name));
		sprintf(pathdir,"/datacenter/msa/policytemplate/%s/sysconf.dat",ltMsgGetVar_s(ltMsgPk,"pathdir"));

		ltDbHeadPtr dbPtr;
		dbPtr=lt_dbinit();
		lt_db_htmlpage(dbPtr,"utf-8");
	  bcCnfGetValue_s(pathdir,"lright",lright);


		memset(pathdir,0,sizeof(pathdir));
		sprintf(pathdir,"/datacenter/msa/policytemplate/%s/info.ini",ltMsgGetVar_s(ltMsgPk,"pathdir"));
	  bcCnfGetValue_s(pathdir,"name",name);

		lt_dbput_rootvars(dbPtr,1,"info",name);
	  memset(str,0,sizeof(str));//1
		sprintf(str,"ipmonchk%c",lright[0]);
		lt_dbput_rootvars(dbPtr,1,str,"checked");

		memset(str,0,sizeof(str));//2
		sprintf(str,"httpmonchk%c",lright[1]);
		lt_dbput_rootvars(dbPtr,1,str,"checked");

		memset(str,0,sizeof(str));//3
		sprintf(str,"ipreportmonchk%c",lright[2]);
		lt_dbput_rootvars(dbPtr,1,str,"checked");

		memset(str,0,sizeof(str));//8
		sprintf(str,"blockmonchk%c",lright[3]);
		lt_dbput_rootvars(dbPtr,1,str,"checked");

		memset(str,0,sizeof(str));//9
		sprintf(str,"immonchk%c",lright[4]);
		lt_dbput_rootvars(dbPtr,1,str,"checked");

		memset(str,0,sizeof(str));//10
		sprintf(str,"filemonchk%c",lright[5]);
		lt_dbput_rootvars(dbPtr,1,str,"checked");

		memset(str,0,sizeof(str));//11
		sprintf(str,"searchmonchk%c",lright[6]);
		lt_dbput_rootvars(dbPtr,1,str,"checked");

		memset(str,0,sizeof(str));//12
		sprintf(str,"usermonchk%c",lright[7]);
		lt_dbput_rootvars(dbPtr,1,str,"checked");

		lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/PolicyTemplate.htm",dbPtr->head,0);
		ltMsgFree(ltMsgPk);
		lt_dbfree(dbPtr);
		return 0;
}

int defaultfile(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		char str[16];
		memset(str,0,sizeof(str));
	  bcCnfGetValue_s("/datacenter/msa/policytemplate/default","filename",str);

	  ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,str);
	  ltMsgFree(ltMsgPk);
	  return 0;
}
int alertWrite(ltMsgHead *ltMsgPk,char *msg,int msgtype){
		 //数据库连接
		ltDbConn     *G_DbConAlertWrite;
	  char *dbUser;
	  char *dbPass;
	  char *dbName;
	  dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbConAlertWrite=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbConAlertWrite!=NULL){
			//printf("db G_DbConAlertWrite ok\n");
		}else{
			fprintf(stderr,"db connect error\n");
		}
		//msgtype   1 添加管理员  newadminalert 2 添加新策略 newrulealert 删除规则 delrulealert
		int i=0;
		char sqlBuf[512],sDate[16],sTime[16],str[512],mailflag[8],msmflag[8],telstr[16],result[1024],mailstr[64];
		memset(sDate,0,sizeof(sDate));
		memset(sTime,0,sizeof(sTime));
		memset(result,0,sizeof(result));
		//nasCvtStime(time(0),sDate,sTime);
		nasTimeGetDate(sDate,time(0));
		nasTimeGetTime(sTime,time(0));
		for(i=0;i<=strlen(sDate);i++){
				if(sDate[i]=='/') sDate[i]='-';
		}
		printf("----------alertWrite--------------\n");
		memset(sqlBuf,0,sizeof(sqlBuf));
		if(msgtype==1){
				//报警方式
				memset(sTime,0,sizeof(sTime));
				bcCnfGetValue_s("/etc/msa/msa/alert","mailalert",mailflag);
				bcCnfGetValue_s("/etc/msa/msa/alert","smsalert",msmflag);
				printf("mailflag  %s\n",mailflag);
				printf("msmflag  %s\n",msmflag);
				if(strcmp(mailflag,"yes")==0){
						memset(mailstr,0,sizeof(mailstr));
						bcCnfGetValue_s("/etc/msa/msa/alert","mail",mailstr);

						memset(str,0,sizeof(str));
						strgb2utf8("统一管理和综合审计系统邮件提醒",str,strlen("统一管理和综合审计系统邮件提醒"));
						nasMailCtlMailSend("alert@isca.com.cn",mailstr,str,msg);
						
						
						memset(str,0,sizeof(str));
						strgb2utf8("添加管理员",str,strlen("添加管理员"));
						sprintf(sqlBuf,"insert into msaAlertList values(%ld%u,'%s','%s','%s',2,'%s',1,%d)",time(0),getpid(),ltMsgGetVar_s(ltMsgPk,"clientip"),sDate,sTime,str,2);
						printf("%s\n",sqlBuf);
						ltDbExecSql(G_DbConAlertWrite,sqlBuf);
				}
				if(strcmp(msmflag,"yes")==0){
						//电话号码
						printf("msmflag action\n");
						memset(telstr,0,sizeof(telstr));
						bcCnfGetValue_s("/etc/msa/msa/alert","telphone",telstr);
						printf("tel   %s\n",telstr);
						//发送
						printf("1111:%s\n",msg);
						msaSMSsend(telstr,msg,result);
						
						
						memset(str,0,sizeof(str));
						strgb2utf8("添加管理员",str,strlen("添加管理员"));
						sprintf(sqlBuf,"insert into msaAlertList values(%ld%u,'%s','%s','%s',2,'%s',2,%d)",time(0),getpid(),ltMsgGetVar_s(ltMsgPk,"clientip"),sDate,sTime,str,2);
						printf("%s\n",sqlBuf);
						ltDbExecSql(G_DbConAlertWrite,sqlBuf);
					 	ltDbClose(G_DbConAlertWrite);
				}
		}else if(msgtype==2){
				//报警方式
				memset(sTime,0,sizeof(sTime));
				bcCnfGetValue_s("/etc/msa/msa/alert","mailalert",mailflag);
				bcCnfGetValue_s("/etc/msa/msa/alert","smsalert",msmflag);
				if(strcmp(mailflag,"yes")==0){

						memset(mailstr,0,sizeof(mailstr));
						bcCnfGetValue_s("/etc/msa/msa/alert","mail",mailstr);
						printf("mail   %s\n",mailstr);

						memset(str,0,sizeof(str));
						strgb2utf8("统一管理和综合审计系统邮件提醒",str,strlen("统一管理和综合审计系统邮件提醒"));
						nasMailCtlMailSend("alert@isca.com.cn",mailstr,str,msg);
						
						memset(str,0,sizeof(str));
						strgb2utf8("添加新规则",str,strlen("添加新规则"));
						sprintf(sqlBuf,"insert into msaAlertList values(%ld%u,'%s','%s','%s',2,'%s',1,%d)",time(0),getpid(),ltMsgGetVar_s(ltMsgPk,"clientip"),sDate,sTime,str,2);
						printf("%s\n",sqlBuf);
						ltDbExecSql(G_DbConAlertWrite,sqlBuf);
				}
				if(strcmp(msmflag,"yes")==0){
						//电话号码
						memset(telstr,0,sizeof(telstr));
						bcCnfGetValue_s("/etc/msa/msa/alert","telphone",telstr);
						printf("tel   %s\n",mailstr);
						//发送
						msaSMSsend(telstr,msg,result);
						
						memset(str,0,sizeof(str));
						strgb2utf8("添加新规则",str,strlen("添加新规则"));
						sprintf(sqlBuf,"insert into msaAlertList values(%ld%u,'%s','%s','%s',2,'%s',2,%d)",time(0),getpid(),ltMsgGetVar_s(ltMsgPk,"clientip"),sDate,sTime,str,2);
						printf("%s\n",sqlBuf);
						ltDbExecSql(G_DbConAlertWrite,sqlBuf);
					 	ltDbClose(G_DbConAlertWrite);
				}
		}else if(msgtype==3){
				//报警方式
				memset(sTime,0,sizeof(sTime));
				bcCnfGetValue_s("/etc/msa/msa/alert","mailalert",mailflag);
				bcCnfGetValue_s("/etc/msa/msa/alert","smsalert",msmflag);
				if(strcmp(mailflag,"yes")==0){

						memset(mailstr,0,sizeof(mailstr));
						bcCnfGetValue_s("/etc/msa/msa/alert","mail",mailstr);
						printf("mail   %s\n",mailstr);

						memset(str,0,sizeof(str));
						strgb2utf8("统一管理和综合审计系统邮件提醒",str,strlen("统一管理和综合审计系统邮件提醒"));
						nasMailCtlMailSend("alert@isca.com.cn",mailstr,str,msg);
						
						memset(str,0,sizeof(str));
						strgb2utf8("删除规则",str,strlen("删除规则"));
						sprintf(sqlBuf,"insert into msaAlertList values(%ld%u,'%s','%s','%s',%d,'%s',%d,%d)",time(0),getpid(),ltMsgGetVar_s(ltMsgPk,"clientip"),sDate,sTime,2,str,1,2);
						printf("%s\n",sqlBuf);
						ltDbExecSql(G_DbConAlertWrite,sqlBuf);
				}
				if(strcmp(msmflag,"yes")==0){
						//电话号码
						memset(telstr,0,sizeof(telstr));
						bcCnfGetValue_s("/etc/msa/msa/alert","telphone",telstr);
						printf("tel   %s\n",mailstr);
						//发送
						msaSMSsend(telstr,msg,result);
						
						memset(str,0,sizeof(str));
						strgb2utf8("删除规则",str,strlen("删除规则"));
						sprintf(sqlBuf,"insert into msaAlertList values(%ld%u,'%s','%s','%s',%d,'%s',%d,%d)",time(0),getpid(),ltMsgGetVar_s(ltMsgPk,"clientip"),sDate,sTime,2,str,2,2);
						printf("%s\n",sqlBuf);
						ltDbExecSql(G_DbConAlertWrite,sqlBuf);
					 	ltDbClose(G_DbConAlertWrite);
				}
		}
		printf("----------END--------------\n");
	 	return 0;
}
