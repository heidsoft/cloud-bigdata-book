#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
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
#include <sys/socket.h>
#include <dirent.h>
#include "msa.h"
#include <net/if.h>
#include <netinet/in.h>

#define MSACONF	"/etc/msa/msa/option/locktime"
#define IDELCONF	"/etc/msa/msa/option/ideltime"
int checkTimeOut(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	int  iIndex;
	//char sIp[128];
	char msalogonid[12],sid[12];
	ltDbHeadPtr dbPtr;
	dbPtr=lt_dbinit();
 	lt_db_htmlpage(dbPtr,"utf-8"); 	
  char flowInfo[1000];
	sprintf(flowInfo,"%s","<html><script type=\"text/javascript\">function confirmation() {parent.parent.location.href=\"/msa/main.xp?Fun=msaAdminLogout\";}</script><body onload= \"confirmation()\"></body></html>");
	
	memset(msalogonid,0,sizeof(msalogonid));
	memset(sid,0,sizeof(sid));
	
	//memset(sIp,0,sizeof(sIp));
//	ltMsgGetSomeNVar(ltMsgPk,1,"clientip",LT_TYPE_STRING, 19, sIp);
//	ltMsgGetSomeNVar(ltMsgPk,1,"msalogonid",LT_TYPE_STRING, 12, msalogonid,"msaindex",LT_TYPE_STRING, 12, sid);
//	ltMsgGetSomeNVar(ltMsgPk,1,"msaindex",LT_TYPE_STRING, 12, sid);
	//sprintf(sIp,"%s",ltMsgGetVar_s(ltMsgPk,"clientip"));
	if(ltMsgGetVar_s(ltMsgPk,"msaindex")){
  	sprintf(sid,"%s",ltMsgGetVar_s(ltMsgPk,"msaindex"));
  }
  if(ltMsgGetVar_s(ltMsgPk,"msalogonid")){
  	sprintf(msalogonid,"%s",ltMsgGetVar_s(ltMsgPk,"msalogonid"));
  }
//	printf("msalogonid :  %s\n",msalogonid);
//	printf("sid   %s\n",sid);
	if(strlen(msalogonid)<1){
		 printf("msalogonid < 1 !\n");
		 ltMsgPk->msgpktype=1;
		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,flowInfo);
		 ltMsgFree(ltMsgPk);
	   return 0;
	}
	iIndex=-1;
	if(_ltPubInfo->admSession[atoi(sid)].lSid==atol(msalogonid)){
  	iIndex=atoi(sid);
  }
	if(iIndex==-1 ){
		 printf("iIndex <0!\n");
		 ltMsgPk->msgpktype=1;
		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,flowInfo);
		 ltMsgFree(ltMsgPk);
	   return 0;
	}
	char ideltime[64];
	bcCnfGetValue_s(IDELCONF,"ideltime",ideltime);
	printf("ideltime   %d\n",atoi(ideltime));
	if(atoi(ideltime)==0){
		memset(ideltime,0,strlen(ideltime));
		sprintf(ideltime,"%d",15);
	}
	if( (time(0)-_ltPubInfo->admSession[iIndex].lasttime) > atoi(ideltime)*60 ){//超时
		 printf("time %ld\n",(time(0)-_ltPubInfo->admSession[iIndex].lasttime));
		 printf("checkTimeOut.lasttime out !\n");
	 	 ltMsgPk->msgpktype=1;
		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type:text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,flowInfo);
		 ltMsgFree(ltMsgPk);
	   return 0;
	}else {
		printf("time %ld\n",(time(0)-_ltPubInfo->admSession[iIndex].lasttime));
		_ltPubInfo->admSession[iIndex].lasttime=time(0);
		//_ltPubInfo->admSession[iIndex].lSip[3]=ntohl(inet_addr(sIp));
		return 1;
	}
}
int msaShowAdminLogon(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	ltDbHeadPtr dbPtr;
	dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
  ltMsgPk->msgpktype=1;
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/index.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;
}

 /*
 函数说明： 注销
 日期：2012-05-07
 修改：2012-05-07
 */
int msaAdminLogout(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	ltDbHeadPtr dbPtr;

	char sIp[128];
	char strUserName[64];

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
	memset(sIp,0,sizeof(sIp));


	//ltMsgGetSomeNVar(ltMsgPk,1,"clientip",LT_TYPE_STRING, 19, sIp);
	sprintf(sIp,"%s",ltMsgGetVar_s(ltMsgPk,"clientip"));
	char msalogonid[12],sid[12];
	memset(msalogonid,0,sizeof(msalogonid));
	memset(sid,0,sizeof(sid));
	sprintf(sid,"%s",ltMsgGetVar_s(ltMsgPk,"msaindex"));
  sprintf(msalogonid,"%s",ltMsgGetVar_s(ltMsgPk,"msalogonid"));
	dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
  
	if(_ltPubInfo->admSession[atoi(sid)].lSid==atol(msalogonid)){
				printf("%s logout.",_ltPubInfo->admSession[atoi(sid)].caname);
        _ltPubInfo->admSession[atoi(sid)].lSip[0]=0;
        _ltPubInfo->admSession[atoi(sid)].lSip[1]=0;
        _ltPubInfo->admSession[atoi(sid)].lSip[2]=0;
        _ltPubInfo->admSession[atoi(sid)].lSip[3]=0;
			  _ltPubInfo->admSession[atoi(sid)].lasttime=0;
				_ltPubInfo->admSession[atoi(sid)].trytimes=0;
		    _ltPubInfo->admSession[atoi(sid)].lSid=0;
		    sprintf(_ltPubInfo->admSession[atoi(sid)].lright,"%s","0000000000000000000000000000000000000000000");
		   	sprintf(strUserName,"%s",_ltPubInfo->admSession[atoi(sid)].caname);
	}
  
	int lTime;
	char  caDate[64],str[256];
	char  sqlBuf[1024];
	lTime = time(0);
	strcpy(caDate,ltTimeFormat("%Y-%m-%d %H:%M:%S",lTime));
	memset(str,0,sizeof(str));
	sprintf(str,"user %s logout success. <from %s >",strUserName,sIp);
	sprintf(sqlBuf,"insert into msaadmlog values('%s','%s','%s','01')",strUserName,caDate,str);
	printf("%s\n",sqlBuf);
	ltDbExecSql(G_DbCon,sqlBuf); 
	ltDbClose(G_DbCon);
  ltMsgPk->msgpktype=1;
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/index.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	ltDbClose(G_DbCon);
	return 0;
}


 /*
 函数说明： 登陆日志
 日期：2012-05-07
 修改：2012-05-07
 */
int msaAdminLogon(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
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
	LT_DBROW tempRow;
	char sqlBuf[1024];
	ltDbHeadPtr dbPtr;
  char *strUserName;
  char newPass[32];
	ltDbCursor *tempCursor;
	int  i,freesid=-1,logFlag=-1,searchflag=0;
	unsigned int pid;
	pid=ltStrGetId();
	char sIp[128];
	int newlogon=0;
	unsigned int bindip[4];
	bindip[0]=0;
	bindip[1]=0;
	bindip[2]=0;
	bindip[3]=0;
	memset(sIp,0,sizeof(sIp));
	memset(newPass,0,sizeof(newPass));
	//snprintf(sIp,19,ltMsgGetVar_s(ltMsgPk,"clientip"));
	sprintf(sIp,"%s",ltMsgGetVar_s(ltMsgPk,"clientip"));
	printf("msaAdminLogon ip  %s\n",sIp);
	strUserName=ltMsgGetVar_s(ltMsgPk,"username");
	ltMd5Encrypto(ltMsgGetVar_s(ltMsgPk,"password"),strlen(ltMsgGetVar_s(ltMsgPk,"password")),newPass);
	dbPtr=lt_dbinit();
	//锁定登陆
	char aftertime[64];
	memset(aftertime,0,sizeof(aftertime));
	
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
	for(i=0;i<MAX_ADMIN_SESSION;i++){
  	if(_ltPubInfo->admSession[i].lSip[0]==bindip[0]&&_ltPubInfo->admSession[i].lSip[1]==bindip[1]&&_ltPubInfo->admSession[i].lSip[2]==bindip[2]&&_ltPubInfo->admSession[i].lSip[3]==bindip[3]){
  				searchflag=1;
  				printf("try ones \n");
  				if(_ltPubInfo->admSession[i].trytimes>=5){
  						_ltPubInfo->admSession[i].trytimes+=1;
  						_ltPubInfo->admSession[i].lasttime=time(0);
  						//ltMsgGetSomeNVar(ltMsgPk,1,"aftertime",LT_TYPE_STRING,12,aftertime);
  						
  						sprintf(aftertime,"%s",ltMsgGetVar_s(ltMsgPk,"aftertime"));
  						printf("aftertime   %s\n",aftertime);
  						printf("Nowtime     %ld\n",time(0));
  						if(strcmp(aftertime,"")!=0&&time(0)>atol(aftertime)){
  							printf("can longin again! \n");
  							_ltPubInfo->admSession[i].trytimes=0;
  							break;
  						};
  						printf("not break \n");
  						
  						int lTime;
							char  caDate[64],str[256];
							lTime = time(0);
							strcpy(caDate,ltTimeFormat("%Y-%m-%d %H:%M:%S",lTime));
							memset(str,0,sizeof(str));
							sprintf(str,"waring user try to log in %d times! <from %s>",_ltPubInfo->admSession[i].trytimes,sIp);
							sprintf(sqlBuf,"insert into msaadmlog values('%s','%s','%s','1')",strUserName,caDate,str);
							printf("%s\n",sqlBuf);
					    ltDbExecSql(G_DbCon,sqlBuf);
					    char locktime[64];
					    memset(locktime,0,sizeof(locktime));
					    bcCnfGetValue_s(MSACONF,"locktime",locktime);
					    printf("locktime  %s\n",locktime);
					    if(strcmp(locktime,"")==0){
					    	sprintf(locktime,"%d",4);
					    }
  						sprintf(sqlBuf,"[{errtype:1,aftertime:'%ld'}]",_ltPubInfo->admSession[i].lasttime+atoi(locktime)*60);
				    	ltMsgPk->msgpktype=1;
				 		  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,sqlBuf);	
				 		  return 0;	
  				}
  				//有此IP 且尝试次数不超过5次
  			  _ltPubInfo->admSession[i].lasttime=time(0);
  				_ltPubInfo->admSession[i].trytimes+=1;
  		    //sprintf(strUserName,"%s",_ltPubInfo->admSession[i].caname);
  		    printf("Trys times %d\n",_ltPubInfo->admSession[i].trytimes);
  		    freesid=i;
  		    break;
  	}else if(searchflag==0&&i==MAX_ADMIN_SESSION-1){//如果没有IP记录 查找free ID 开始记录该IP
  			printf("search free id () \n");
  			for(i=0;i<MAX_ADMIN_SESSION;i++){
						  	if((_ltPubInfo->admSession[i].lSip[0]==0&&_ltPubInfo->admSession[i].lSip[1]==0&&_ltPubInfo->admSession[i].lSip[2]==0&&_ltPubInfo->admSession[i].lSip[3]==0) || (time(0) -_ltPubInfo->admSession[i].lasttime ) > 7200){
						  				printf("find free id %d\n",i);
						          _ltPubInfo->admSession[i].lSip[0]=bindip[0];
						          _ltPubInfo->admSession[i].lSip[1]=bindip[1];
						          _ltPubInfo->admSession[i].lSip[2]=bindip[2];
						          _ltPubInfo->admSession[i].lSip[3]=bindip[3];
						          _ltPubInfo->admSession[i].lasttime=time(0);
											_ltPubInfo->admSession[i].trytimes=1;
											sprintf(_ltPubInfo->admSession[i].caname,"%s",strUserName);
											freesid=i;
											newlogon=1;
					  		      break;
						  	}
				}
				break;
		}
  }
	i=0;
	sprintf(sqlBuf,"select a.name,b.lright from nasadmuser a left join msarole b on a.roleid=b.roleid where a.name='%s' and a.password='%s'",strUserName,newPass);
	printf("%s\n",sqlBuf);
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
							          //_ltPubInfo->admSession[freesid].lSip[3]=ntohl(inet_addr(sIp));
							          //_ltPubInfo->admSession[freesid].lasttime=time(0);
							          if(newlogon==1) _ltPubInfo->admSession[freesid].lSid=pid; //没有IP记录
												sprintf(_ltPubInfo->admSession[freesid].caname,"%s",strUserName);
												sprintf(_ltPubInfo->admSession[freesid].lright,"%s",tempRow[1]);
												printf("%s right is %s \n",strUserName,_ltPubInfo->admSession[freesid].lright);
												logFlag=0;
			}
		ltDbCloseCursor(tempCursor);
	}
	if(logFlag==0){
				char str[128],tmpstr[128];
				memset(str,0,sizeof(str));
				memset(tmpstr,0,sizeof(tmpstr));
				memset(sqlBuf,0,sizeof(sqlBuf));
				
	  		ltMd5Encrypto(strUserName,strlen(strUserName),str);
				
				sprintf(sqlBuf,"select password from nasadmuser where name='%s'",strUserName);
				printf("maakey     %s\n",sqlBuf);
				tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
				if(tempCursor!=NULL){
						tempRow= ltDbFetchRow(tempCursor);
						if(tempRow!=NULL){
							 sprintf(str,"%s%s",str,tempRow[0]);
						}
					ltDbCloseCursor(tempCursor);
				}
				printf("%s\n",str);
				sprintf(tmpstr,"%s",ltMsgGetVar_s(ltMsgPk,"msakey"));
				if(strcmp(str,tmpstr)!=0){
					printf("maakey error.\n");
					logFlag=2;
				}
	}
	if(logFlag==0){
				sprintf(sqlBuf,"update msasadmuser set lasttime=%ld where name='%s' ",time(0),strUserName); 
				ltDbExecSql(G_DbCon,sqlBuf);
				
				int lTime;
				char  caDate[64],str[256];
				lTime = time(0);
				strcpy(caDate,ltTimeFormat("%Y-%m-%d %H:%M:%S",lTime));
				memset(str,0,sizeof(str));
				sprintf(str,"user login success! <from %s>",sIp);
				sprintf(sqlBuf,"insert into msaadmlog values('%s','%s','%s','1')",strUserName,caDate,str);
				//printf("%s\n",sqlBuf);
		    ltDbExecSql(G_DbCon,sqlBuf);		
	}else{
				int lTime;
				char  caDate[64],str[256];
				lTime = time(0);
				strcpy(caDate,ltTimeFormat("%Y-%m-%d %H:%M:%S",lTime));
				memset(str,0,sizeof(str));
				sprintf(str,"user login failed! <from %s>",sIp);
				//printf("b64str   %s\n",b64str);
				sprintf(sqlBuf,"insert into msaadmlog values('%s','%s','%s','1')",strUserName,caDate,str);
				//printf("%s\n",sqlBuf);
		    ltDbExecSql(G_DbCon,sqlBuf);
	}
	ltDbClose(G_DbCon);
	memset(sqlBuf,0,sizeof(sqlBuf));
  sprintf(sqlBuf,"[{errtype:%d,sessionid:'%ld',sessionindex:'%d'}]",logFlag,_ltPubInfo->admSession[freesid].lSid,freesid);
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,sqlBuf);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;
}
 /*
 函数说明： 显示主页面
 日期：2012-05-07
 修改：2012-05-07
 */
int msaShowTopview(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
  ltDbHeadPtr dbPtr;
	char sRight[50];
	int  iIndex;
	//char sIp[128];
	char msalogonid[12],sid[12];
	memset(msalogonid,0,sizeof(msalogonid));
	memset(sid,0,sizeof(sid));
	
  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	//memset(sIp,0,sizeof(sIp));
//	ltMsgGetSomeNVar(ltMsgPk,1,"clientip",LT_TYPE_STRING, 19, sIp);
//	ltMsgGetSomeNVar(ltMsgPk,1,"msalogonid",LT_TYPE_STRING, 12, msalogonid,"msaindex",LT_TYPE_STRING, 12, sid);
//	ltMsgGetSomeNVar(ltMsgPk,1,"msaindex",LT_TYPE_STRING, 12, sid);
	//sprintf(sIp,"%s",ltMsgGetVar_s(ltMsgPk,"clientip"));
  sprintf(sid,"%s",ltMsgGetVar_s(ltMsgPk,"msaindex"));
  sprintf(msalogonid,"%s",ltMsgGetVar_s(ltMsgPk,"msalogonid"));
	printf("msalogonid   %s\n",msalogonid);
	printf("sid   %s\n",sid);
	if(strlen(msalogonid)<1){
	  ltMsgPk->msgpktype=1;
	  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/index.htm",dbPtr->head,0);
	  ltMsgFree(ltMsgPk);
		return 0;
	}
	
	iIndex=-1;
//  for(i=0;i<MAX_ADMIN_SESSION;i++){
//  	//printf("%d    %ld \n",i,_ltPubInfo->admSession[i].lSid);
//  	if(_ltPubInfo->admSession[i].lSid==atol(msalogonid)){
//  		   iIndex=i;
//  		   break;
//  	}
//  }
  if(_ltPubInfo->admSession[atoi(sid)].lSid==atol(msalogonid)){
  	iIndex=atoi(sid);
  }
  if(iIndex==-1){
  	printf("iIndex < 0 !!!  \n");
  	ltMsgPk->msgpktype=1;
	  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/index.htm",dbPtr->head,0);
	  ltMsgFree(ltMsgPk);
		return 0;
	}
	if( (time(0)-_ltPubInfo->admSession[iIndex].lasttime) > 2400 ){
		printf("msaShowTopview timeout !\n");
		ltMsgPk->msgpktype=1;
	  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/index.htm",dbPtr->head,0);
	  ltMsgFree(ltMsgPk);
		return 0;
	}
	_ltPubInfo->admSession[iIndex].lasttime=time(0);
	//_ltPubInfo->admSession[iIndex].lSip[3]=ntohl(inet_addr(sIp));
	memset(sRight,0,sizeof(sRight));
	sprintf(sRight,"%s",_ltPubInfo->admSession[iIndex].lright);
	printf("sRight is %s\n",sRight);
  lt_dbput_rootvars(dbPtr,1,"admuser",_ltPubInfo->admSession[iIndex].caname);
  if(strcmp(_ltPubInfo->admSession[iIndex].caname,"admin")==0){
  		memset(_ltPubInfo->admSession[iIndex].lright,1,sizeof(_ltPubInfo->admSession[iIndex].lright));
  }
  lt_dbput_rootvars(dbPtr,1,"lright",_ltPubInfo->admSession[iIndex].lright);
  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/main/main.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;
}
/*
函数说明：到账号添加页面（查询所有人） 
日期：2012-05-09
作者：司智凯 
修改：
*/
int toadminadd(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  checkTimeOut(confd,ltMsgPk,lt_MMHead);
	char sqlBuf[1024];

  ltDbHeadPtr dbPtr;
  ltTablePtr  tablePtr1;
  ltDbCursor  *tempCursor;
  LT_DBROW    tempRow;

	ltDbConn *G_DbCon;

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
		
  dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
  
  tablePtr1=lt_dbput_table(dbPtr,"rolelist");
  sprintf(sqlBuf,"select roleid,rolename from  msarole order by roleid");
    

  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
  if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr1,2,"roleid",LT_TYPE_STRING,tempRow[0],"rolename",LT_TYPE_STRING,tempRow[1]);
				tempRow= ltDbFetchRow(tempCursor);
		}
	  ltDbCloseCursor(tempCursor);
	}
	lt_dbput_rootvars(dbPtr,1,"Fun","adminadd");
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/htmlplt/users/adminadd.htm",dbPtr->head,0);

  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);
  return 0;
}

/*
函数说明：用户信息 添加  
日期：2012-05-09
作者：司智凯 
修改：
*/
int adminadd(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  checkTimeOut(confd,ltMsgPk,lt_MMHead);
	char sqlBuf[2048];

  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
 
	ltDbConn *G_DbCon;

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
 
 	char *strPassword;
  char newPass[50];
  memset(newPass,0,sizeof(newPass)); 
  strPassword=ltMsgGetVar_s(ltMsgPk,"pass");
  ltMd5Encrypto(strPassword, strlen(strPassword),newPass);
  
   ltDbConn *G_DbCon2;
 	 G_DbCon2=ltDbConnect(dbUser,dbPass,dbName);
 	 
	 memset(sqlBuf,0,sizeof(sqlBuf)); 
   sprintf(sqlBuf,"select *  from nasadmuser where name='%s' ",ltMsgGetVar_s(ltMsgPk,"logname"));
   tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
   if(tempCursor != NULL) {
	   tempRow= ltDbFetchRow(tempCursor);
	   if(tempRow!=NULL){
	   	    ltDbCloseCursor(tempCursor);
	   		  ltMsgPk->msgpktype=1;
	        lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" ><font color=\"#ff0000\">该用户名已经被使用!</font<br><a href=\"javascript:history.back(1);\" >返回</a></body></html>");	
					return 0;
	   }
	   ltDbCloseCursor(tempCursor);
	 }
	 
	 memset(sqlBuf,0,sizeof(sqlBuf));	
	 sprintf(sqlBuf,"insert into nasadmuser (name,password,email,tel,rmark,truename,roleid) values ('%s','%s','%s','%s','%s','%s','%d')",
			ltMsgGetVar_s(ltMsgPk,"logname"),
			newPass, 
			ltMsgGetVar_s(ltMsgPk,"email"),
			ltMsgGetVar_s(ltMsgPk,"tel"),
			ltMsgGetVar_s(ltMsgPk,"rmark"),
			ltMsgGetVar_s(ltMsgPk,"truename"),
			atoi(ltMsgGetVar_s(ltMsgPk,"roleid"))
	 );
	 ltMd5Encrypto(strPassword, strlen(strPassword),newPass);
	 ltDbExecSql(G_DbCon,sqlBuf);
printf("sqlBuf_insert:%s\n",sqlBuf);

	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
   ltMsgFree(ltMsgPk);
   return 0;
}
/*
函数说明：加载用户列表页面
日期：2012-05-09
作者：司智凯 
修改：
*/
int ltadminlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	checkTimeOut(confd,ltMsgPk,lt_MMHead);
	char sqlBuf[1024];

  ltDbHeadPtr dbPtr;
  ltTablePtr  tablePtr;
  ltDbCursor  *tempCursor;
  LT_DBROW    tempRow;

	ltDbConn *G_DbCon;

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
		
  dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
  tablePtr=lt_dbput_table(dbPtr,"rolelist");
  sprintf(sqlBuf,"select roleid,rolename from  msarole order by roleid");
  
  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
  if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,2,"roleid",LT_TYPE_STRING,tempRow[0],"rolename",LT_TYPE_STRING,tempRow[1]);
				tempRow= ltDbFetchRow(tempCursor);
		}
	  ltDbCloseCursor(tempCursor);
	}
	
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/htmlplt/users/adminlistinfo.htm",dbPtr->head,0);        
	ltMsgFree(ltMsgPk); 
	lt_dbfree(dbPtr);
	ltDbClose(G_DbCon);
	return 0;
}
/*
函数说明：用户列表  根据传的参数删除修改注销用户
日期：2012-05-09
作者：司智凯 
修改：
*/
int adminlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  checkTimeOut(confd,ltMsgPk,lt_MMHead);
	  char strBuf[1024];
	  ltDbCursor *tempCursor;
	  LT_DBROW tempRow;
	  char caTmpp[512];
	  long lRowNum,lStartRec,lSumRec,lCount,lNum;
	  int k;
		char  lasttime[64];
 		
	  stralloc strTemp;
		ltDbHeadPtr dbPtr;
	  dbPtr=lt_dbinit();
	  lt_db_htmlpage(dbPtr,"utf-8");
    //根据传的参数判断删除以及执行删除
	  int cadeflag;
		char sqlBuf[2048];
		char *roleid;
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
			cadeflag=atoi(ltMsgGetVar_s(ltMsgPk,"deflag"));   
			if(cadeflag==1)
			{
				 sprintf(sqlBuf,"delete from nasadmuser where name='%s'",ltMsgGetVar_s(ltMsgPk,"gnmkey"));
printf("sqlBuf:%s\n",sqlBuf); 
				 ltDbExecSql(G_DbCon,sqlBuf);
				 
				 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
				 ltMsgFree(ltMsgPk);		
	    	 return 0;
			}
		}
		roleid=NULL;
    lRowNum=50; /*每页的行数*/
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
	  if(ltMsgGetVar_s(ltMsgPk,"roleid"))
		{
	    roleid=ltMsgGetVar_s(ltMsgPk,"roleid");
	  }
	  if(!roleid){
    	sprintf(strBuf,"select count(*) from nasadmuser");
    }else{
    	sprintf(strBuf,"select count(*) from nasadmuser where roleid=%s",roleid);
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
    if(!roleid){
    	sprintf(strBuf,"select a.name,truename,tel,email,rmark,a.roleid,rolename,a.lasttime from nasadmuser a left join msarole b on a.roleid=b.roleid  order by a.lasttime asc limit %lu offset %lu ",lRowNum,lStartRec);
    }else{
    	sprintf(strBuf,"select a.name,truename,tel,email,rmark,a.roleid,rolename,a.lasttime from nasadmuser a left join msarole b on a.roleid=b.roleid  where b.roleid='%s' order by a.lasttime asc limit %lu offset %lu ",roleid,lRowNum,lStartRec);
    }
printf("strBuf:%s\n",strBuf);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {
          if(k==0)
					{
 						strcpy(lasttime,ltTimeFormat("%Y-%m-%d",atoi(tempRow[7])));
            sprintf(caTmpp,"{\"name\":\"%s\",\"truename\":\"%s\",\"tel\":\"%s\",\"email\":\"%s\",\"rmark\":\"%s\",\"roleid\":\"%s\",\"role\":\"%s\",\"lasttime\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],lasttime);
            stralloc_cats(&strTemp,caTmpp);
					}
					else
					{
 						strcpy(lasttime,ltTimeFormat("%Y-%m-%d",atoi(tempRow[7])));
				    sprintf(caTmpp,",{\"name\":\"%s\",\"truename\":\"%s\",\"tel\":\"%s\",\"email\":\"%s\",\"rmark\":\"%s\",\"roleid\":\"%s\",\"role\":\"%s\",\"lasttime\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],lasttime);
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
函数说明：用户详细资料 
日期：2012-05-09
作者：司智凯 
修改：
*/
int adminedit(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
    checkTimeOut(confd,ltMsgPk,lt_MMHead);
	  char strBuf[2048],sqlBuf1[2048];

		ltDbHeadPtr dbPtr;
		
		ltTablePtr  tablePtr5;
    
    ltDbCursor  *tempCursor,*tempCursor3;
    LT_DBROW    tempRow,tempRow3;
    char        *uname;
    
    char roleid[64];

    ltDbConn *G_DbCon;
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
   
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    
    uname=ltMsgGetVar_s(ltMsgPk,"uname");

    sprintf(strBuf,"select name,truename,a.roleid,tel,email,rmark,password from  nasadmuser a left join msarole b on a.roleid=b.roleid where a.name='%s' ",uname);   
printf("strBuf_up:%s\n:",strBuf);
   	tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
   	if(tempCursor!=NULL){
		      tempRow= ltDbFetchRow(tempCursor);
	       	if(tempRow!=NULL){
						lt_dbput_rootvars(dbPtr,6,
						"name",tempRow[0],
						"truename",tempRow[1],
						"tel",tempRow[3],
						"email",tempRow[4],
						"rmark",tempRow[5],
						"pass",tempRow[6]
						);
						sprintf(roleid,tempRow[2]);
	        }
        	ltDbCloseCursor(tempCursor);
    }
	  tablePtr5=lt_dbput_table(dbPtr,"rolelist");

    sprintf(sqlBuf1,"select roleid,rolename from  msarole");  
    tempCursor3=ltDbOpenCursor(G_DbCon,sqlBuf1);
    if(tempCursor3!=NULL){
			tempRow3= ltDbFetchRow(tempCursor3);			
			while(tempRow3!=NULL){
						if(strcmp(tempRow3[0],roleid)==0){
							lt_dbput_recordvars(tablePtr5,3,		
								"roleid",LT_TYPE_STRING,tempRow3[0],
								"rolename",LT_TYPE_STRING,tempRow3[1],
								"role",LT_TYPE_STRING,"selected"
								);
						}else{
								lt_dbput_recordvars(tablePtr5,3,
								"roleid",LT_TYPE_STRING,tempRow3[0],
								"rolename",LT_TYPE_STRING,tempRow3[1],
								"role",LT_TYPE_STRING,""
								);
							 
						}
					  tempRow3= ltDbFetchRow(tempCursor3);
					      
			}
			
		  ltDbCloseCursor(tempCursor3);
	  }
	  
	  
	  if(ltMsgGetVar_s(ltMsgPk,"type")){
	  	if(atoi(ltMsgGetVar_s(ltMsgPk,"type"))==1){	  
	 			lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/htmlplt/users/adminout.htm",dbPtr->head,0);
	  	}
	  }else{
	  	lt_dbput_rootvars(dbPtr,1,"Fun","adminupdate");
	  	lt_dbput_rootvars(dbPtr,1,"readonly","readonly");
	 		lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/htmlplt/users/adminadd.htm",dbPtr->head,0);
	  }
   
	  ltMsgFree(ltMsgPk);
	  lt_dbfree(dbPtr);
	  return 0;
}

/*
函数说明：修改用户信息 
日期：2012-05-09
作者：司智凯 
修改：
*/
int adminupdate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		checkTimeOut(confd,ltMsgPk,lt_MMHead);
	  char strBuf[2048];
    
    ltDbConn *G_DbCon;
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
	 
		ltDbConn *G_DbCon2;
	 	G_DbCon2=ltDbConnect(dbUser,dbPass,dbName);	  
		if(G_DbCon2==NULL){
						ltMsgPk->msgpktype=1;
						lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"db error!");
						ltMsgFree(ltMsgPk);			
						return 0;
		}	

	  memset(strBuf,0,sizeof(strBuf));	
    sprintf(strBuf,"update  nasadmuser set name='%s',email='%s',tel='%s',rmark='%s',truename='%s',roleid='%s' where name='%s' ",
ltMsgGetVar_s(ltMsgPk,"logname"),
ltMsgGetVar_s(ltMsgPk,"email"),
ltMsgGetVar_s(ltMsgPk,"tel"),
ltMsgGetVar_s(ltMsgPk,"rmark"),
ltMsgGetVar_s(ltMsgPk,"truename"),
ltMsgGetVar_s(ltMsgPk,"roleid"),
ltMsgGetVar_s(ltMsgPk,"logname"));
printf("strBuf_update:%s\n",strBuf);

   ltDbExecSql(G_DbCon,strBuf);
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
   ltMsgFree(ltMsgPk);
   return 0;
}

/*
函数说明：删除账号 
日期：2012-05-09
作者：司智凯
修改：
*/
int admindel(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    //ltadminlist(confd,ltMsgPk,lt_MMHead);		

    return 0;
}

/*
函数说明：到部门管理页面 
日期：2012-05-11   //改为torolemanager
作者：司智凯 
修改：
*/
int torolemanager(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  checkTimeOut(confd,ltMsgPk,lt_MMHead);

  ltDbHeadPtr dbPtr;
  dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");

  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/htmlplt/users/managergroup.htm",dbPtr->head,0);

  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);
  return 0;
}
/*
函数说明：角色详情
日期：2012-05-11
作者：司智凯 
修改：
*/
int groupmanager(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    //checkTimeOut(confd,ltMsgPk,lt_MMHead);
	  ltDbConn *G_DbCon;
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
		ltDbCursor *tempCursor;
	  LT_DBROW tempRow;
		long lRowNum,lStartRec,lSumRec,lCount,lNum;
		char *keyword;
	  char strBuf[1024];
	  int k;
	  stralloc strTemp;
	  char caTmpp[512];
		keyword=ltMsgGetVar_s(ltMsgPk,"keyword");
    lRowNum=50; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0;
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
    if(ltMsgGetVar_s(ltMsgPk,"flag")){
printf("flag:%s\n",ltMsgGetVar_s(ltMsgPk,"flag"));
	    if(atoi(ltMsgGetVar_s(ltMsgPk,"flag"))==1){
			 	sprintf(strBuf,"delete from msarole where roleid='%s'",ltMsgGetVar_s(ltMsgPk,"gnmkey"));
		    ltDbExecSql(G_DbCon,strBuf);
			 	sprintf(strBuf,"update nasadmuser set roleid=1 where roleid='%s'",ltMsgGetVar_s(ltMsgPk,"gnmkey"));
		    ltDbExecSql(G_DbCon,strBuf);
		    
			  ltMsgPk->msgpktype=1;
			  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
			  ltMsgFree(ltMsgPk);
			  ltDbClose(G_DbCon);	
			  return 0; 
			}
		}
    if(ltMsgGetVar_s(ltMsgPk,"limit"))
		{
	    lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	  }
    if(ltMsgGetVar_s(ltMsgPk,"start"))
		{
	    lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	  }
	  	  
	  if(!keyword){
    	sprintf(strBuf,"select count(*) from msarole");
    }else{
    	sprintf(strBuf,"select count(*) from msarole where rolename like '%c%s%c'",'%',keyword,'%');
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
    if(!keyword){
    	sprintf(strBuf,"select roleid,rolename,roledesc from msarole order by roleid asc limit %lu offset %lu ",lRowNum,lStartRec);
    }else{
    	sprintf(strBuf,"select roleid,rolename,roledesc from msarole where rolename like '%c%s%c' order by roleid asc limit %lu offset %lu ",'%',keyword,'%',lRowNum,lStartRec);
    }
printf("strBuf11111:%s\n",strBuf);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {
          if(k==0)
					{
            sprintf(caTmpp,"{\"roleid\":\"%s\",\"rolename\":\"%s\",\"roledesc\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2]);
            stralloc_cats(&strTemp,caTmpp);
					}
					else
					{
				    sprintf(caTmpp,",{\"roleid\":\"%s\",\"rolename\":\"%s\",\"roledesc\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2]);
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
函数说明：角色详细资料 
日期：2012-05-09
作者：司智凯 
修改：
*/
int roleedit(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
    checkTimeOut(confd,ltMsgPk,lt_MMHead);
	  char strBuf[2048];

		ltDbHeadPtr dbPtr;
    
    ltDbCursor  *tempCursor;
    LT_DBROW    tempRow;
    char        *gnmkey;
    
    char sRight[128];//char role[64];

    ltDbConn *G_DbCon;
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
   
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    
    gnmkey=ltMsgGetVar_s(ltMsgPk,"gnmkey");

    sprintf(strBuf,"select roleid,rolename,lright,roledesc from msarole where roleid='%s' ",gnmkey);   
printf("strBuf_up_role: %s\n",strBuf);
   	tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
   	if(tempCursor!=NULL){
		      tempRow= ltDbFetchRow(tempCursor);
	       	if(tempRow!=NULL){	
	       	sprintf(sRight,"%s",tempRow[2]);
					if(sRight[0]=='1' ){   
						lt_dbput_rootvars(dbPtr,1,"chsel1","checked");
					}
					if(sRight[1]=='1'  ){
						lt_dbput_rootvars(dbPtr,1,"chsel2","checked");
					}
					if(sRight[2]=='1'  ){
						lt_dbput_rootvars(dbPtr,1,"chsel3","checked");
					}
					if(sRight[3]=='1'  ){
						lt_dbput_rootvars(dbPtr,1,"chsel4","checked");
					}
					if(sRight[4]=='1' ){
						lt_dbput_rootvars(dbPtr,1,"chsel5","checked");
					}
					if(sRight[5]=='1' ){
						lt_dbput_rootvars(dbPtr,1,"chsel6","checked");
					}
					if(sRight[6]=='1' ){
						lt_dbput_rootvars(dbPtr,1,"chsel7","checked");
					}
					lt_dbput_rootvars(dbPtr,3,"roleid",tempRow[0],"rolename",tempRow[1],"roledesc",tempRow[3]);
	      }
        ltDbCloseCursor(tempCursor);
    }
  	lt_dbput_rootvars(dbPtr,1,"Fun","roleupdate");
	  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/roleadd.htm",dbPtr->head,0);
   
	  ltMsgFree(ltMsgPk);
	  lt_dbfree(dbPtr);
	  return 0;
}

/*
函数说明：修改角色信息 
日期：2012-05-09
作者：司智凯 
修改：
*/
int roleupdate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		checkTimeOut(confd,ltMsgPk,lt_MMHead);
	  char strBuf[2048];
		char *gnmkey;
    
    char sRight[128];
    
    ltDbConn *G_DbCon;
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
	 
		ltDbConn *G_DbCon2;
	 	G_DbCon2=ltDbConnect(dbUser,dbPass,dbName);	  
		if(G_DbCon2==NULL){
						ltMsgPk->msgpktype=1;
						lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"db error!");
						ltMsgFree(ltMsgPk);			
						return 0;
		}	
	 	
    memset(sRight,0,sizeof(sRight));
		if(atoi(ltMsgGetVar_s(ltMsgPk,"chr1"))==1){
	     sRight[0]='1';
	  }else {
	  	 sRight[0]='0';
	  }
		if(atoi(ltMsgGetVar_s(ltMsgPk,"chr2"))==1){
	     sRight[1]='1';
	  }else {
	  	 sRight[1]='0';
	  }
		if(atoi(ltMsgGetVar_s(ltMsgPk,"chr3"))==1){
	     sRight[2]='1';
	  }else {
	  	 sRight[2]='0';
	  }
		if(atoi(ltMsgGetVar_s(ltMsgPk,"chr4"))==1){
	     sRight[3]='1';
	  }else {
	  	 sRight[3]='0';
	  }
		if(atoi(ltMsgGetVar_s(ltMsgPk,"chr5"))==1){
	     sRight[4]='1';
	  }else {
	  	 sRight[4]='0';
	  }
	  if(atoi(ltMsgGetVar_s(ltMsgPk,"chr6"))==1){
	     sRight[5]='1';
	  }else {
	  	 sRight[5]='0';
	  }
		if(atoi(ltMsgGetVar_s(ltMsgPk,"chr7"))==1){
	     sRight[6]='1';
	  }else {
	  	 sRight[6]='0';
	  }

    gnmkey=ltMsgGetVar_s(ltMsgPk,"id");
	  memset(strBuf,0,sizeof(strBuf));	
    sprintf(strBuf,"update  msarole set rolename='%s',roledesc='%s',lright='%s' where roleid='%s' ",ltMsgGetVar_s(ltMsgPk,"rolename"),ltMsgGetVar_s(ltMsgPk,"roledesc"),sRight,gnmkey);
printf("strBuf_update:%s\n",strBuf);
   ltDbExecSql(G_DbCon,strBuf);
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
   ltMsgFree(ltMsgPk);
   return 0;
}
/*
函数说明：到角色添加页面
日期：2012-05-09
作者：司智凯 
修改：
*/
int toroleadd(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){  
  ltDbCursor  *tempCursor;
  LT_DBROW    tempRow;
  char sqlBuf[2048];  
  ltDbConn *G_DbCon;
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

  ltDbHeadPtr dbPtr;	
  dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
	 
  char* lCount="0";
  sprintf(sqlBuf,"select count(*) from msarole");
  tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
  if(tempCursor!=NULL){
      tempRow= ltDbFetchRow(tempCursor);
      if(tempRow!=NULL){
         lCount=tempRow[0];
      }
      ltDbCloseCursor(tempCursor);
  }
  if(lCount == "0") {
		lCount="1";
  }else{
    sprintf(sqlBuf,"select max(roleid)+1 from msarole");
    lCount = "1";
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
     tempRow= ltDbFetchRow(tempCursor);
     if(tempRow!=NULL){
               lCount=tempRow[0];
     }
     ltDbCloseCursor(tempCursor);
    }
  } 
printf("lCount:%s\n",lCount);  
  lt_dbput_rootvars(dbPtr,1,"roleid",lCount);
  lt_dbput_rootvars(dbPtr,1,"Fun","roleadd");
  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/roleadd.htm",dbPtr->head,0);

  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);
  return 0;
}

/*
函数说明：角色信息 添加  
日期：2012-05-09
作者：司智凯 
修改：
*/
int roleadd(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char sqlBuf[2048];

  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
  char sRight[128];
 
	ltDbConn *G_DbCon;

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
	
  memset(sRight,0,sizeof(sRight));
  
	if(atoi(ltMsgGetVar_s(ltMsgPk,"chr1"))==1){
     sRight[0]='1';
  }
	if(atoi(ltMsgGetVar_s(ltMsgPk,"chr2"))==1){
     sRight[1]='1';
  }
	if(atoi(ltMsgGetVar_s(ltMsgPk,"chr3"))==1){
     sRight[2]='1';
  }
	if(atoi(ltMsgGetVar_s(ltMsgPk,"chr4"))==1){
     sRight[3]='1';
  }
	if(atoi(ltMsgGetVar_s(ltMsgPk,"chr5"))==1){
     sRight[4]='1';
  }
	if(atoi(ltMsgGetVar_s(ltMsgPk,"chr6"))==1){
     sRight[5]='1';
  }
	if(atoi(ltMsgGetVar_s(ltMsgPk,"chr7"))==1){
     sRight[6]='1';
  }
   ltDbConn *G_DbCon2;
 	 G_DbCon2=ltDbConnect(dbUser,dbPass,dbName);	
 	 
	 memset(sqlBuf,0,sizeof(sqlBuf)); 
   sprintf(sqlBuf,"select *  from msarole where rolename='%s' ",ltMsgGetVar_s(ltMsgPk,"rolename"));
   tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
   if(tempCursor != NULL) {
	   tempRow= ltDbFetchRow(tempCursor);
	   if(tempRow!=NULL){
	   	    ltDbCloseCursor(tempCursor);
	   		  ltMsgPk->msgpktype=1;
	        lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" ><font color=\"#ff0000\">该角色名已经被使用!</font<br><a href=\"javascript:history.back(1);\" >返回</a></body></html>");	
					return 0;
	   }
	   ltDbCloseCursor(tempCursor);
	 }
	 
	 int tttid,lCount;
	 lCount=100;
	 tttid=1;
	 for(tttid=1;tttid<2000;tttid++){
	 					memset(sqlBuf,0,sizeof(sqlBuf));
		        sprintf(sqlBuf,"select count(*) from msarole where roleid=%d ",tttid);
						tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
						if(tempCursor){
							tempRow= ltDbFetchRow(tempCursor);
							if(tempRow){
								if(atoi(tempRow[0])==0){
									lCount=tttid;
									ltDbCloseCursor(tempCursor);
									break;
								}
							}
							ltDbCloseCursor(tempCursor);
						}
   }
	 memset(sqlBuf,0,sizeof(sqlBuf));	
	 sprintf(sqlBuf,"insert into msarole (roleid,rolename,roledesc,lright) values (%d,'%s','%s','%s')",
			lCount,
			ltMsgGetVar_s(ltMsgPk,"rolename"),
			ltMsgGetVar_s(ltMsgPk,"roledesc"),
			sRight
	 );
	 ltDbExecSql(G_DbCon,sqlBuf);
	 printf("sqlBuf_insert:%s\n",sqlBuf);
	 
	 //写日志
	 int lTime;
	 char caDate[64],caMsg[256];
	 memset(caMsg,0,256);
	 lTime = time(0);
	 strcpy(caDate,ltTimeFormat("%Y-%m-%d %H:%M:%S",lTime));
	 sprintf(caMsg,"insert into msaadmlog values('%s','%s','add role %s success.',2)",ltMsgGetVar_s(ltMsgPk,"rolename"),caDate,ltMsgGetVar_s(ltMsgPk,"rolename"));
	 printf("%s\n",caMsg);
	 ltDbExecSql(G_DbCon,caMsg);
	 
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
   ltMsgFree(ltMsgPk);
   return 0;
}
//配置页面
int sysconfshow(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char Encode64[512],locktime[64],idletime[64],str[64];
  memset(Encode64,0,sizeof(Encode64));
  memset(locktime,0,sizeof(locktime));
  memset(idletime,0,sizeof(idletime));
  memset(str,0,sizeof(str));
  
  ltDbHeadPtr dbPtr;
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	bcCnfGetValue_s(MSACONF,"Encode64",Encode64);
	bcCnfGetValue_s(MSACONF,"locktime",locktime);
	bcCnfGetValue_s(MSACONF,"idletime",idletime);
	if(strcmp(Encode64,"")!=0){
			sprintf(str,"Encode64%s",Encode64);
			lt_dbput_rootvars(dbPtr,1,str,"selected");
	}
	if(strcmp(locktime,"")==0){
			sprintf(locktime,"%d",4);
	}
	lt_dbput_rootvars(dbPtr,1,"locktime",locktime);
	lt_dbput_rootvars(dbPtr,1,"idletime",idletime);
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/sysconfset.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	 return 0;
}
int sysconfset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	ltDbHeadPtr dbPtr;
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");

  char *Encode64;
  char *locktime;
  char *idletime;
  char caCmdCon[64];
  Encode64=ltMsgGetVar_s(ltMsgPk,"Encode64");
  locktime=ltMsgGetVar_s(ltMsgPk,"locktime");
  idletime=ltMsgGetVar_s(ltMsgPk,"idletime");
  
  memset(caCmdCon,0,sizeof(caCmdCon));
	sprintf(caCmdCon,"echo 'Encode64=%s' > %s",Encode64,MSACONF);
	system(caCmdCon);
  
  memset(caCmdCon,0,sizeof(caCmdCon));
	sprintf(caCmdCon,"echo 'locktime=%s' >> %s",locktime,MSACONF);
	system(caCmdCon);	
	
  memset(caCmdCon,0,sizeof(caCmdCon));
	sprintf(caCmdCon,"echo 'idletime=%s' >> %s",idletime,MSACONF);
	system(caCmdCon);	
	ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
  ltMsgFree(ltMsgPk);
  return 0;
}
//页面显示
int checkoutShowpage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	checkTimeOut(confd,ltMsgPk,lt_MMHead);
	char path[32],pagename[32],surl[128];
	memset(path,0,sizeof(path));
	memset(pagename,0,sizeof(pagename));
	memset(surl,0,sizeof(surl));
	sprintf(path,"%s",ltMsgGetVar_s(ltMsgPk,"path"));
	sprintf(pagename,"%s",ltMsgGetVar_s(ltMsgPk,"name"));
	sprintf(surl,"/app/msa/msa/htmlplt/%s/%s.htm",path,pagename);
	
  ltDbHeadPtr dbPtr;
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	
  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,surl,dbPtr->head,0);
  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);
  return 0;
}
