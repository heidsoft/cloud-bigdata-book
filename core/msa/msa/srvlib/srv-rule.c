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
#include <dirent.h>
#include <gdfontmb.h>
#include <ctype.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <asm/types.h>
#include <stdarg.h>
#include "msa.h"

#define SMSname       "zsz6181"
#define SMSpwd        "6043f37cc255e4b1c77d"
//优先级别修改 param 表名 当前id 当前prio 类型
int msaUpPrio(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	//数据库连接
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
	 char *table;
	 char sqlBuf[256],prioname[32],idname[32],condition[64];
	 int ruleid,type,num,rruleid=0;
	 float prevPrio,pprevPrio,prio;
	 num=0;
	 prevPrio=-1.0;
	 pprevPrio=-1.0;
	 if(ltMsgGetVar_s(ltMsgPk,"rruleid")){
	 		rruleid=atoi(ltMsgGetVar_s(ltMsgPk,"rruleid"));
	 }
	 ruleid=atoi(ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 table=ltMsgGetVar_s(ltMsgPk,"table");
	 type=atoi(ltMsgGetVar_s(ltMsgPk,"type"));
	 memset(condition,0,sizeof(condition));
	 strcpy(prioname,"rulerate");
	 strcpy(idname,"ruleid");
	 if(type==2){
	 		memset(prioname,0,sizeof(prioname));
	 		strcpy(prioname,"prio");
	 		strcpy(idname,"sid");
	 }
	 if(type==3){
	 		memset(prioname,0,sizeof(prioname));
	 		sprintf(condition," where ruleid=%d",rruleid);
	 		strcpy(prioname,"rulerate");
	 		strcpy(idname,"sid");
	 }
	 LT_DBROW tempRow;
	 ltDbCursor *tempCursor;
	 
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select %s,%s from %s%s order by %s DESC",prioname,idname,table,condition,prioname);
	 printf("%s\n",sqlBuf);
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				if(atoi(tempRow[1])==ruleid){
					break;
				}
				if(num==0){
						pprevPrio=atof(tempRow[0]);
						num=1;
				}else if(num==1){
						prevPrio=atof(tempRow[0]);
						num=0;
				}
				tempRow=ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
	 }
	 if(pprevPrio==-1){
	 		ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"-1");
		  ltMsgFree(ltMsgPk);
		  return 0;
	 }else if(prevPrio==-1){
	 		prio=pprevPrio+1;
	 }else {
	 		prio=(pprevPrio+prevPrio)/2;
	 }
	 
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"update %s set %s=%0.6f where %s=%d",table,prioname,prio,idname,ruleid);
	 printf("%s\n",sqlBuf);
	 ltDbExecSql(G_DbCon,sqlBuf);
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
	 ltMsgFree(ltMsgPk);
	 return 0;
}
int msaDownPrio(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	//数据库连接
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
	 char *table;
	 char sqlBuf[256],prioname[32],idname[32],condition[64];
	 int ruleid,type,num,flag,rruleid=0;
	 float prevPrio,pprevPrio,prio;
	 num=0;
	 flag=1;
	 prevPrio=-1.0;
	 pprevPrio=-1.0;
	 if(ltMsgGetVar_s(ltMsgPk,"rruleid")){
	 		rruleid=atoi(ltMsgGetVar_s(ltMsgPk,"rruleid"));
	 }
	 ruleid=atoi(ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 table=ltMsgGetVar_s(ltMsgPk,"table");
	 type=atoi(ltMsgGetVar_s(ltMsgPk,"type"));
	 memset(condition,0,sizeof(condition));
	 strcpy(prioname,"rulerate");
	 strcpy(idname,"ruleid");
	 if(type==2){
	 		memset(prioname,0,sizeof(prioname));
	 		strcpy(prioname,"prio");
	 		strcpy(idname,"sid");
	 }
	 if(type==3){
	 		memset(prioname,0,sizeof(prioname));
	 		sprintf(condition," where ruleid=%d",rruleid);
	 		strcpy(prioname,"rulerate");
	 		strcpy(idname,"sid");
	 }
	 LT_DBROW tempRow;
	 ltDbCursor *tempCursor;
	 
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select %s,%s from %s%s order by %s DESC",prioname,idname,table,condition,prioname);
	 printf("%s\n",sqlBuf);
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				if(atoi(tempRow[1])==ruleid){
					flag=0;
					tempRow=ltDbFetchRow(tempCursor);
					continue;
				}
				if(flag==0){
					if(num==0){
						pprevPrio=atof(tempRow[0]);
						num=1;
					}else if(num==1){
						prevPrio=atof(tempRow[0]);
						break;
					}
				}
				tempRow=ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
	 }
	 if(pprevPrio==-1){
		 	ltMsgPk->msgpktype=1;
	 		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"-1");
	 		ltMsgFree(ltMsgPk);
	 		return 0;
	 }else if(prevPrio==-1){
	 		prio=pprevPrio-1;
	 }else {
	 		prio=(pprevPrio+prevPrio)/2;
	 }
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"update %s set %s=%0.6f where %s=%d",table,prioname,prio,idname,ruleid);
	 printf("%s\n",sqlBuf);
	 ltDbExecSql(G_DbCon,sqlBuf);
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//显示规则修改
int showServicePolicy(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 checkTimeOut(confd,ltMsgPk,lt_MMHead);
	 char viewtype[4];
	 strcpy(viewtype,"0");
	 ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
	 			strcpy(viewtype,ltMsgGetVar_s(ltMsgPk,"type"));
	 }
	 lt_dbput_rootvars(dbPtr,1,"type",viewtype);
   lt_db_htmlpage(dbPtr,"utf-8");
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/servicepolicy.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//显示规则服务修改页面
int showEditRuleSrv(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	//数据库连接
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
	 char rulesrv[1001];
	 char sqlBuf[128];
	 int myqosid;
	 char str[16];
	 myqosid=-1;
	 ltDbHeadPtr dbPtr;
	 ltTablePtr  tablePtr;
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   lt_dbput_rootvars(dbPtr,1,"sid",ltMsgGetVar_s(ltMsgPk,"sid"));
	 LT_DBROW tempRow;
	 ltDbCursor *tempCursor;
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select * from nassrvrulesrv where sid=%s",ltMsgGetVar_s(ltMsgPk,"sid"));
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow){
				strcpy(rulesrv,tempRow[3]);
				strcat(rulesrv,tempRow[4]);
				strcat(rulesrv,tempRow[5]);
				strcat(rulesrv,tempRow[6]);
				memset(str,0,sizeof(str));
				sprintf(str,"ruleaction%s",tempRow[7]);
				lt_dbput_rootvars(dbPtr,1,str,"selected");
				lt_dbput_rootvars(dbPtr,2,
					"sname",tempRow[1],
					"rulesrv",rulesrv
				);
    		myqosid=atoi(tempRow[8]);
			}
			ltDbCloseCursor(tempCursor);
		}
		
		char qossel[32];
		tablePtr=lt_dbput_table(dbPtr,"qoslist");
		memset(sqlBuf,0,sizeof(sqlBuf));
		sprintf(sqlBuf,"select sid,qosname from msaqos order by sid asc");
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
					tempRow=ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
							memset(qossel,0,sizeof(qossel));
							if(atoi(tempRow[0])==myqosid){
									sprintf(qossel,"%s","selected");
							}
							lt_dbput_recordvars(tablePtr,3,
							  "qosid",LT_TYPE_STRING,tempRow[0],
								"sel",LT_TYPE_STRING,qossel,
								"qosname",LT_TYPE_STRING,tempRow[1]);
						    tempRow=ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor);
	  }
	 ltDbClose(G_DbCon);
	 lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 lt_dbput_rootvars(dbPtr,1,"sid",ltMsgGetVar_s(ltMsgPk,"sid"));
	 lt_dbput_rootvars(dbPtr,1,"rulesrv",rulesrv);
	 lt_dbput_rootvars(dbPtr,1,"function","msaEditRuleSrv");
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/ruleSrvAdd.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//update规则服务
int msaEditRuleSrv(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
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
	 int sid,ruleaction,qosid;
	 char sname[128],sqlstr[4096],rulesrv[1001],rulesrv0[251],rulesrv1[251],rulesrv2[251],rulesrv3[251];
	 sid=-1;
	 ruleaction=atoi(ltMsgGetVar_s(ltMsgPk,"ruleaction"));
	 qosid=atoi(ltMsgGetVar_s(ltMsgPk,"qosid"));
	 sid=atoi(ltMsgGetVar_s(ltMsgPk,"sid"));
	 sprintf(rulesrv,"%s",ltMsgGetVar_s(ltMsgPk,"rulesrv"));
	 sprintf(sname,"%s",ltMsgGetVar_s(ltMsgPk,"sname"));

	 memset(rulesrv0,0,sizeof(rulesrv0));
	 memset(rulesrv1,0,sizeof(rulesrv1));
	 memset(rulesrv2,0,sizeof(rulesrv2));
	 memset(rulesrv3,0,sizeof(rulesrv3));
 	 memcpy(rulesrv0,rulesrv,250);
	 memcpy(rulesrv1,rulesrv+250,250);
	 memcpy(rulesrv2,rulesrv+500,250);
	 memcpy(rulesrv3,rulesrv+750,250);
	 
	 memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"update nassrvrulesrv set sname='%s',rulesrv0='%s',rulesrv1='%s',rulesrv2='%s',rulesrv3='%s',ruleaction=%d,qosid=%d where sid=%d",sname,rulesrv0,rulesrv1,rulesrv2,rulesrv3,ruleaction,qosid,sid);
printf("update nassrvrulesrv %s\n",sqlstr);
	 ltDbExecSql(G_DbCon,sqlstr);
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
   ltMsgFree(ltMsgPk);
   return 0;
}
//规则服务添加
int addRuleSrv(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//数据库连接
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
	 int ruleaction,qosid,ruleid;
	 char sname[32],sqlstr[2048],rulesrv[1001],rulesrv0[251],rulesrv1[251],rulesrv2[251],rulesrv3[251];
	 char sid[8];
	 int tttid,lCount;
	 float temprio;
	 temprio=0;
	 lCount=0;
   LT_DBROW tempRow;
	 ltDbCursor *tempCursor;
	 for(tttid=1;tttid<_MAX_SRVRULE_NUM;tttid++){
	 					memset(sqlstr,0,sizeof(sqlstr));
		        sprintf(sqlstr,"select count(*) from nassrvrulesrv where sid=%d ",tttid);
						tempCursor=ltDbOpenCursor(G_DbCon,sqlstr);
						if(tempCursor){
							tempRow= ltDbFetchRow(tempCursor);
							if(tempRow){
								if( atol(tempRow[0])==0){
									lCount=tttid;
									ltDbCloseCursor(tempCursor);
									break;
								}
							}
							ltDbCloseCursor(tempCursor);
						}
   }
   //优先级别查找
	 ruleid=atoi(ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"select rulerate from nassrvrulesrv where ruleid=%d order by rulerate DESC",ruleid);
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlstr);
	 if(tempCursor){
							tempRow= ltDbFetchRow(tempCursor);
							if(tempRow){
								temprio=atoi(tempRow[0])+1.0;
							}
							ltDbCloseCursor(tempCursor);
	 }
	 ruleaction=atoi(ltMsgGetVar_s(ltMsgPk,"ruleaction"));
	 qosid=atoi(ltMsgGetVar_s(ltMsgPk,"qosid"));
	 sprintf(rulesrv,"%s",ltMsgGetVar_s(ltMsgPk,"rulesrv"));
	 sprintf(sname,"%s",ltMsgGetVar_s(ltMsgPk,"sname"));
	 memset(rulesrv0,0,sizeof(rulesrv0));
	 memset(rulesrv1,0,sizeof(rulesrv1));
	 memset(rulesrv2,0,sizeof(rulesrv2));
	 memset(rulesrv3,0,sizeof(rulesrv3));
	 
 	 memcpy(rulesrv0,rulesrv,250);
	 memcpy(rulesrv1,rulesrv+250,250);
	 memcpy(rulesrv2,rulesrv+500,250);
	 memcpy(rulesrv3,rulesrv+750,250);
	 
	 memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"insert into nassrvrulesrv(sid,sname,ruleid,rulesrv0,rulesrv1,rulesrv2,rulesrv3,ruleaction,qosid,rulerate) values (%d,'%s',%d,'%s','%s','%s','%s',%d,%d,%f)",lCount,sname,ruleid,rulesrv0,rulesrv1,rulesrv2,rulesrv3,ruleaction,qosid,temprio);
	 ltDbExecSql(G_DbCon,sqlstr);
//printf("%s\n",sqlstr);
	 sprintf(sid,"%d",lCount);
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,sid);
   ltMsgFree(ltMsgPk);
   return 0;
}
//显示规则服务模板

int showRuleSrv(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
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
	 char sqlBuf[64];
   ltDbHeadPtr dbPtr;
   ltTablePtr tablePtr;
   LT_DBROW tempRow;
	 ltDbCursor *tempCursor;
	 dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
	 tablePtr=lt_dbput_table(dbPtr,"qoslist");
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select sid,qosname from msaqos order by sid asc");
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor!=NULL){
				tempRow=ltDbFetchRow(tempCursor);
				while(tempRow!=NULL){
//						memset(qossel,0,sizeof(qossel));
//						if(atoi(tempRow[0])==myqosid){
//								sprintf(qossel,"%s","selected");
//						}
						lt_dbput_recordvars(tablePtr,2,
						  "qosid",LT_TYPE_STRING,tempRow[0],
							"qosname",LT_TYPE_STRING,tempRow[1]);
					  tempRow=ltDbFetchRow(tempCursor);
				}
				ltDbCloseCursor(tempCursor);
	 }
	 ltDbClose(G_DbCon);
   lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));
   lt_dbput_rootvars(dbPtr,1,"sid",ltMsgGetVar_s(ltMsgPk,"sid"));
   lt_dbput_rootvars(dbPtr,1,"rulesrv","0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	 lt_dbput_rootvars(dbPtr,1,"function","addRuleSrv");
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/ruleSrvAdd.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}

//显示规则模板
int showService(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   ltDbHeadPtr dbPtr;
   ltTablePtr tablePtr;
	 dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
		tablePtr=lt_dbput_table(dbPtr,"qoslist");
		int i;
		i=0;
		char strid[8];
		memset(strid,0,sizeof(strid));
		tablePtr=lt_dbput_table(dbPtr,"jjrlist");
		for(i=0;i<NAS_MAXJJR_NUM;i++){
				sprintf(strid,"%d",_ltPubInfo->_jjrtimelist[i].jjrid);
				if(strcmp(_ltPubInfo->_jjrtimelist[i].jjrname,"")!=0){
					lt_dbput_recordvars(tablePtr,2,
					  "jjrid",LT_TYPE_STRING,strid,
						"jjrname",LT_TYPE_STRING,_ltPubInfo->_jjrtimelist[i].jjrname);
				}
		}
		tablePtr=lt_dbput_table(dbPtr,"schlist");
		memset(strid,0,sizeof(strid));
		for(i=0;i<NAS_MAXSCH_NUM;i++){
				sprintf(strid,"%d",_ltPubInfo->_schtimelist[i].schid);
				if(strcmp(_ltPubInfo->_schtimelist[i].schname,"")!=0){
					lt_dbput_recordvars(tablePtr,2,
					  "schid",LT_TYPE_STRING,strid,
						"schname",LT_TYPE_STRING,_ltPubInfo->_schtimelist[i].schname);
				}
		}
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/serviceAdd.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//规则添加
int addService(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//数据库连接
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
	 int ruleflag,logaction,conaction,ruleschedule,rulejjr;
	 char rulename[32],ruleurl[128],sqlstr[2048],policy[129],group[1001],g1[251],g2[251],g3[251],g4[251];
	 char ruleid[8];
	 int tttid,lCount;
	 float temprio;
	 temprio=0;
	 lCount=0;
   LT_DBROW tempRow;
	 ltDbCursor *tempCursor;
	 for(tttid=1;tttid<_MAX_SRVRULE_NUM;tttid++){
	 					memset(sqlstr,0,sizeof(sqlstr));
		        sprintf(sqlstr,"select count(*) from nassrvrule where ruleid=%d ",tttid);
						tempCursor=ltDbOpenCursor(G_DbCon,sqlstr);
						if(tempCursor){
							tempRow= ltDbFetchRow(tempCursor);
							if(tempRow){
								if( atol(tempRow[0])==0){
									lCount=tttid;
									ltDbCloseCursor(tempCursor);
									break;
								}
							}
							ltDbCloseCursor(tempCursor);
						}
   }
   //优先级别查找
	 memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"%s","select rulerate from nassrvrule order by rulerate DESC");
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlstr);
	 if(tempCursor){
							tempRow= ltDbFetchRow(tempCursor);
							if(tempRow){
								temprio=atoi(tempRow[0])+1.0;
							}
							ltDbCloseCursor(tempCursor);
	 }
	 memset(policy,0,sizeof(policy));
	 memset(group,0,sizeof(group));
	 ruleflag=atoi(ltMsgGetVar_s(ltMsgPk,"ruleflag"));
	 logaction=atoi(ltMsgGetVar_s(ltMsgPk,"logaction"));
	 conaction=atoi(ltMsgGetVar_s(ltMsgPk,"conaction"));
	 ruleschedule=atoi(ltMsgGetVar_s(ltMsgPk,"ruleschedule"));
	 rulejjr=atoi(ltMsgGetVar_s(ltMsgPk,"rulejjr"));
	 sprintf(rulename,"%s",ltMsgGetVar_s(ltMsgPk,"rulename"));
	 sprintf(ruleurl,"%s",ltMsgGetVar_s(ltMsgPk,"ruleurl"));
	 sprintf(group,"%s",ltMsgGetVar_s(ltMsgPk,"group"));
	 sprintf(policy,"%s",ltMsgGetVar_s(ltMsgPk,"policy"));
	 memset(g1,0,sizeof(g1));
	 memset(g2,0,sizeof(g2));
	 memset(g3,0,sizeof(g3));
	 memset(g4,0,sizeof(g4));
 	 memcpy(g1,group,250);
	 memcpy(g2,group+250,250);
	 memcpy(g3,group+500,250);
	 memcpy(g4,group+750,250);
	 memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"insert into nassrvrule(ruleid,ruleflag,logaction,conaction,ruleschedule,rulejjr,rulename,group0,group1,group2,group3,policyid,rulerate,ruleurl) values (%d,%d,%d,%d,%d,%d,'%s','%s','%s','%s','%s','%s',%f,'%s')",lCount,ruleflag,logaction,conaction,ruleschedule,rulejjr,rulename,g1,g2,g3,g4,policy,temprio,ruleurl);
	 printf("%s\n",sqlstr);
	 ltDbExecSql(G_DbCon,sqlstr);
	 sprintf(ruleid,"%d",lCount);
	 ltDbClose(G_DbCon);
	 
	 char caMsg[256],utf8str[512];
	 memset(caMsg,0,sizeof(caMsg));
	 memset(utf8str,0,sizeof(utf8str));
	 sprintf(caMsg,"上网过滤策略%s添加成功，如有疑问请联系管理员。",rulename);
	 strgb2utf8(caMsg,utf8str,strlen(caMsg));
	 alertWrite(ltMsgPk,utf8str,2);
	 
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,ruleid);
   ltMsgFree(ltMsgPk);
   return 0;
}
//显示规则修改
int showEditService(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	//数据库连接
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
	 char group[1001];
	 char sqlBuf[128];
	 int myratelimit,ruleschedule,rulejjr,grouptype;
	 char str[16];
	 myratelimit=-1;
	 ruleschedule=-1;
	 rulejjr=-1;
	 grouptype=0;
	 ltDbHeadPtr dbPtr;
	 ltTablePtr  tablePtr;
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"sid"));
	 LT_DBROW tempRow;
	 ltDbCursor *tempCursor;
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select * from nassrvrule where ruleid=%s",ltMsgGetVar_s(ltMsgPk,"sid"));
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
						if(tempCursor){
							tempRow= ltDbFetchRow(tempCursor);
							if(tempRow){
								memset(group,0,sizeof(group));
								sprintf(group,"%s%s%s%s",tempRow[5],tempRow[6],tempRow[7],tempRow[8]);

								memset(str,0,sizeof(str));
								sprintf(str,"ruleflag%s",tempRow[2]);
								lt_dbput_rootvars(dbPtr,1,str,"selected");
								memset(str,0,sizeof(str));
								sprintf(str,"logaction%s",tempRow[10]);
								lt_dbput_rootvars(dbPtr,1,str,"selected");
								memset(str,0,sizeof(str));
								sprintf(str,"conaction%s",tempRow[11]);
								lt_dbput_rootvars(dbPtr,1,str,"selected");
								lt_dbput_rootvars(dbPtr,5,
			       	 		"rulename",tempRow[1],
			       	 		"policy",tempRow[9],
			       	 		"group",group,
			       	 		"ruleurl",tempRow[13]
       	 				);
       	 				ruleschedule=atoi(tempRow[3]);
       	 				rulejjr=atoi(tempRow[4]);
							}
							ltDbCloseCursor(tempCursor);
						}
						char schedulesel[32];
						char jjrsel[32];
		int i;
		i=0;
		char strid[8];
		memset(strid,0,sizeof(strid));
		tablePtr=lt_dbput_table(dbPtr,"jjrlist");
		for(i=0;i<NAS_MAXJJR_NUM;i++){
				printf("NAS_MAXJJR_NUM    %d\n",NAS_MAXJJR_NUM);
				sprintf(strid,"%d",_ltPubInfo->_jjrtimelist[i].jjrid);
				if(strcmp(_ltPubInfo->_jjrtimelist[i].jjrname,"")!=0){
					memset(jjrsel,0,sizeof(jjrsel));
					if(_ltPubInfo->_jjrtimelist[i].jjrid==rulejjr){
							sprintf(jjrsel,"%s","selected");
					}
					lt_dbput_recordvars(tablePtr,3,
					  "jjrid",LT_TYPE_STRING,strid,
					  "jjrsel",LT_TYPE_STRING,jjrsel,
						"jjrname",LT_TYPE_STRING,_ltPubInfo->_jjrtimelist[i].jjrname);
				}
		}
		tablePtr=lt_dbput_table(dbPtr,"schlist");
		memset(strid,0,sizeof(strid));
		for(i=0;i<NAS_MAXSCH_NUM;i++){
			
				printf("NAS_MAXSCH_NUM    %d\n",NAS_MAXSCH_NUM);
				sprintf(strid,"%d",_ltPubInfo->_schtimelist[i].schid);
				if(strcmp(_ltPubInfo->_schtimelist[i].schname,"")!=0){
					memset(schedulesel,0,sizeof(schedulesel));
					if(_ltPubInfo->_schtimelist[i].schid==ruleschedule){
													sprintf(schedulesel,"%s","selected");
					}
					lt_dbput_recordvars(tablePtr,3,
					  "schid",LT_TYPE_STRING,strid,
					  "schsel",LT_TYPE_STRING,schedulesel,
						"schname",LT_TYPE_STRING,_ltPubInfo->_schtimelist[i].schname);
				}
		}
	 printf("-------------------END----------------------\n");
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/SrvRulelistinfo.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//编辑规则
int msaEditSrvRule(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
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
	 int ruleid,ruleflag,logaction,conaction,ruleschedule,rulejjr;
	 char ruleurl[128],sqlstr[4096],policy[129],group[1001],g1[251],g2[251],g3[251],g4[251];
	 memset(policy,0,sizeof(policy));
	 memset(group,0,sizeof(group));
	 ruleid=atoi(ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 ruleflag=atoi(ltMsgGetVar_s(ltMsgPk,"ruleflag"));
	 logaction=atoi(ltMsgGetVar_s(ltMsgPk,"logaction"));
	 conaction=atoi(ltMsgGetVar_s(ltMsgPk,"conaction"));
	 ruleschedule=atoi(ltMsgGetVar_s(ltMsgPk,"ruleschedule"));
	 rulejjr=atoi(ltMsgGetVar_s(ltMsgPk,"rulejjr"));
	 sprintf(ruleurl,"%s",ltMsgGetVar_s(ltMsgPk,"ruleurl"));
	 sprintf(group,"%s",ltMsgGetVar_s(ltMsgPk,"group"));
	 sprintf(policy,"%s",ltMsgGetVar_s(ltMsgPk,"policy"));


	 memset(g1,0,sizeof(g1));
	 memset(g2,0,sizeof(g2));
	 memset(g3,0,sizeof(g3));
	 memset(g4,0,sizeof(g4));
 	 memcpy(g1,group,250);
	 memcpy(g2,group+250,250);
	 memcpy(g3,group+500,250);
	 memcpy(g4,group+750,250);
	 memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"update nassrvrule set ruleflag=%d,logaction=%d,conaction=%d,ruleschedule=%d,rulejjr=%d,rulename='%s',ruleurl='%s',group0='%s',group1='%s',group2='%s',group3='%s',policyid='%s' where ruleid=%d",ruleflag,logaction,conaction,ruleschedule,rulejjr,ltMsgGetVar_s(ltMsgPk,"rulename"),ruleurl,g1,g2,g3,g4,policy,ruleid);
	 //printf("UPDATE %s\n",sqlstr);
	 ltDbExecSql(G_DbCon,sqlstr);
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
   ltMsgFree(ltMsgPk);
   return 0;
}
//规则策略列表
int msaSrvRulesrvList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//连接数据库
		ltDbConn	*G_DbConA;
  	char *dbUser;
	  char *dbPass;
	  char *dbName;
	  dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		G_DbConA = ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon!=NULL||G_DbConA!=NULL){
			printf("db connect ok\n");
		}else{
			fprintf(stderr,"db connect error\n");
		}
	 char sqlStr[512],qosname[64],sqlBuf[256];
	 int allcount,k;
	 char *delflag;
	 delflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(delflag){
	 		char *sid=ltMsgGetVar_s(ltMsgPk,"sid");
	 		sprintf(sqlStr,"delete from nassrvrulesrv where sid=%s",sid);
	 		ltDbExecSql(G_DbCon,sqlStr);
	 }
	 k=0;
	 ltDbCursor  *tempCursor;
	 ltDbCursor  *tempCursorA;
   LT_DBROW    tempRow;
   LT_DBROW    tempRowA;
	 stralloc strTemp;
	 char caTmpp[1024];
	 strTemp.s=0;
	 allcount=0;
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"%s","select count(*) from nassrvrulesrv");
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	if(tempRow!=NULL){
       		allcount=atol(tempRow[0]);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"select sid,sname,ruleaction,qosid,rulerate from nassrvrulesrv where ruleid=%s order by rulerate DESC",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 //printf("%s\n",sqlStr);
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
//       		memset(qosname,0,sizeof(qosname));
//		      if(atoi(tempRow[3])==0){
//		      	strgb2utf8("不启动人员QOS",qosname,strlen("不启动人员QOS"));
//		      }else{
//		      	sprintf(qosname,"%s",_ltPubInfo->_QosList[atoi(tempRow[3])].qosname);
//		      }
						memset(qosname,0,sizeof(qosname));
						strgb2utf8("不启动人员QOS",qosname,strlen("不启动人员QOS"));
						if(atoi(tempRow[3])!=0){
							memset(sqlBuf,0,sizeof(sqlBuf));
							sprintf(sqlBuf,"select qosname from msaqos where sid=%s",tempRow[3]);
							//printf("second search  %s\n",sqlBuf);
							tempCursorA=ltDbOpenCursor(G_DbConA,sqlBuf);
							if(tempCursorA!=NULL){
										tempRowA=ltDbFetchRow(tempCursorA);
										if(tempRow!=NULL){
											sprintf(qosname,"%s",tempRowA[0]);
										}
										ltDbCloseCursor(tempCursorA);
						  }
					  }
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"sid\":\"%s\",\"sname\":\"%s\",\"ruleaction\":%s,\"orderNum\":%s,\"qosname\":\"%s\",\"tablename\":\"nassrvrulesrv\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[4],qosname);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"sid\":\"%s\",\"sname\":\"%s\",\"ruleaction\":%s,\"orderNum\":%s,\"qosname\":\"%s\",\"tablename\":\"nassrvrulesrv\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[4],qosname);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }
	     	  k++;
       		tempRow= ltDbFetchRow(tempCursor);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   ltDbClose(G_DbCon);
	 ltDbClose(G_DbConA);
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//规则列表
int msaSrvRuleList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//数据库连接
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
	 char sqlStr[1024],str[128];
	 int allcount,k,viewtype;
	 char *delflag;
	 delflag=NULL;
	 viewtype=0;
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
	 	  viewtype=atoi(ltMsgGetVar_s(ltMsgPk,"type"));
	 }
	 if(delflag){
	 	
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		char *grouptype=ltMsgGetVar_s(ltMsgPk,"grouptype");
	 		sprintf(sqlStr,"delete from %s where ruleid=%s",grouptype,ruleid);
	 		
	 		if(strcmp(grouptype,"msauserpolicy")==0){
	 			
	 			memset(sqlStr,0,sizeof(sqlStr));
	 			sprintf(sqlStr,"delete from %s where sid=%s",grouptype,ruleid);
	 			ltDbExecSql(G_DbCon,sqlStr);
	 			
	 			
	 			
	 		}else  if(strcmp(grouptype,"nassrvrule")==0){
	 			memset(sqlStr,0,sizeof(sqlStr));
	 			sprintf(sqlStr,"delete from %s where ruleid=%s",grouptype,ruleid);
	 			ltDbExecSql(G_DbCon,sqlStr);
	 			
	 			sprintf(sqlStr,"delete from nassrvrulesrv where ruleid=%s",ruleid);
	 			ltDbExecSql(G_DbCon,sqlStr);
	 			
	 			sprintf(sqlStr,"delete from nasruleip where ruleid=%s",ruleid);
	 			ltDbExecSql(G_DbCon,sqlStr);
	 			sprintf(sqlStr,"delete from nasruleurl where ruleid=%s",ruleid);
	 			ltDbExecSql(G_DbCon,sqlStr);
	 			
	 			sprintf(sqlStr,"delete from nasruleuser where ruleid=%s",ruleid);
	 			ltDbExecSql(G_DbCon,sqlStr);
	 			sprintf(sqlStr,"delete from nassrvruledip where ruleid=%s",ruleid);
	 			ltDbExecSql(G_DbCon,sqlStr);
	 				 			
	 			char caMsg[256],utf8str[512];
				memset(caMsg,0,sizeof(caMsg));
				memset(utf8str,0,sizeof(utf8str));
				sprintf(caMsg,"%s","上网过滤策略删除成功，如有疑问请联系管理员。");
				strgb2utf8(caMsg,utf8str,strlen(caMsg));
				alertWrite(ltMsgPk,utf8str,3);
	 		}else{
	 			sprintf(sqlStr,"delete from %s where ruleid=%s",grouptype,ruleid);
	 			ltDbExecSql(G_DbCon,sqlStr);	 
	 		}
	 }
	 k=0;
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
	 stralloc strTemp;
	 char caTmpp[1024];
	 strTemp.s=0;
	 allcount=0;
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"%s","select count(*) from nassrvrule");
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	if(tempRow!=NULL){
       		allcount=atol(tempRow[0]);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   
   sprintf(sqlStr,"%s","select count(*) from msauserpolicy");
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	if(tempRow!=NULL){
       		allcount+=atol(tempRow[0]);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   
   memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"%s","select count(*) from nasfirewall");
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	if(tempRow!=NULL){
       		allcount+=atol(tempRow[0]);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
 	 //规则
 	 if(viewtype==0||viewtype==1){
 	 		 memset(sqlStr,0,sizeof(sqlStr));
			 memset(str,0,sizeof(str));
			 strgb2utf8("上网过滤策略",str,strlen("上网过滤策略"));
			 sprintf(sqlStr,"%s","select ruleid,rulename,rulerate,ruleflag from nassrvrule order by rulerate DESC");
			 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
		   if(tempCursor!=NULL){
			      tempRow= ltDbFetchRow(tempCursor);
		       	while(tempRow!=NULL){
		       		if(k==0){
			     	  	 sprintf(caTmpp,"{\"ruleid\":\"%s\",\"rulename\":\"%s\",\"groupname\":\"%s\",\"grouptype\":\"nassrvrule\",\"orderNum\":%s,\"orderType\":0,\"editFun\":\"showEditService\",\"ruleflag\":%s}",tempRow[0],tempRow[1],str,tempRow[2],tempRow[3]);
			     	     stralloc_cats(&strTemp,caTmpp);
			     	  }else{
			     	  	 sprintf(caTmpp,",{\"ruleid\":\"%s\",\"rulename\":\"%s\",\"groupname\":\"%s\",\"grouptype\":\"nassrvrule\",\"orderNum\":%s,\"orderType\":0,\"editFun\":\"showEditService\",\"ruleflag\":%s}",tempRow[0],tempRow[1],str,tempRow[2],tempRow[3]);
			     	     stralloc_cats(&strTemp,caTmpp);
			     	  }
			     	  k++;
		       		tempRow= ltDbFetchRow(tempCursor);
		       	}
		       	ltDbCloseCursor(tempCursor);
		   }
 	 }
	 
   //防火墙
 	 if(viewtype==0||viewtype==2){
		   memset(sqlStr,0,sizeof(sqlStr));
			 memset(str,0,sizeof(str));
			 strgb2utf8("静态防火墙规则",str,strlen("静态防火墙规则"));
			 sprintf(sqlStr,"%s","select ruleid,rulename,rulerate,ruleflag from nasfirewall order by rulerate DESC");
			 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
		   if(tempCursor!=NULL){
			      tempRow= ltDbFetchRow(tempCursor);
		       	while(tempRow!=NULL){
		       		if(k==0){
			     	  	 sprintf(caTmpp,"{\"ruleid\":\"%s\",\"rulename\":\"%s\",\"groupname\":\"%s\",\"grouptype\":\"nasfirewall\",\"orderNum\":%s,\"orderType\":1,\"editFun\":\"editPWpage\",\"ruleflag\":%s}",tempRow[0],tempRow[1],str,tempRow[2],tempRow[3]);
			     	     stralloc_cats(&strTemp,caTmpp);
			     	  }else{
			     	  	 sprintf(caTmpp,",{\"ruleid\":\"%s\",\"rulename\":\"%s\",\"groupname\":\"%s\",\"grouptype\":\"nasfirewall\",\"orderNum\":%s,\"orderType\":1,\"editFun\":\"editPWpage\",\"ruleflag\":%s}",tempRow[0],tempRow[1],str,tempRow[2],tempRow[3]);
			     	     stralloc_cats(&strTemp,caTmpp);
			     	  }
			     	  k++;
		       		tempRow= ltDbFetchRow(tempCursor);
		       	}
		       	ltDbCloseCursor(tempCursor);
		   }
   }
   //批量策略
 	 if(viewtype==0||viewtype==3){
		   memset(sqlStr,0,sizeof(sqlStr));
			 memset(str,0,sizeof(str));
			 strgb2utf8("用户制定策略",str,strlen("用户制定策略"));
			 sprintf(sqlStr,"%s","select sid,policyname,prio from msauserpolicy order by prio DESC");
			 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
		   if(tempCursor!=NULL){
			      tempRow= ltDbFetchRow(tempCursor);
		       	while(tempRow!=NULL){
		       		if(k==0){
			     	  	 sprintf(caTmpp,"{\"ruleid\":\"%s\",\"rulename\":\"%s\",\"groupname\":\"%s\",\"grouptype\":\"msauserpolicy\",\"orderNum\":%s,\"orderType\":2,\"editFun\":\"msaEditPrio\",\"ruleflag\":-1}",tempRow[0],tempRow[1],str,tempRow[2]);
			     	     stralloc_cats(&strTemp,caTmpp);
			     	  }else{
			     	  	 sprintf(caTmpp,",{\"ruleid\":\"%s\",\"rulename\":\"%s\",\"groupname\":\"%s\",\"grouptype\":\"msauserpolicy\",\"orderNum\":%s,\"orderType\":2,\"editFun\":\"msaEditPrio\",\"ruleflag\":-1}",tempRow[0],tempRow[1],str,tempRow[2]);
			     	     stralloc_cats(&strTemp,caTmpp);
			     	  }
			     	  k++;
		       		tempRow= ltDbFetchRow(tempCursor);
		       	}
		       	ltDbCloseCursor(tempCursor);
		   }
 	 }
   ltDbClose(G_DbCon);
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//编辑webpost
int msaEditWebpost(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
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
	 char sqlBuf[128],str[128],all[32],action[32];
	 char *addflag,*delflag;
	 strgb2utf8("全部",all,strlen("全部"));
	 addflag=NULL;
	 delflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"addflag")!=NULL){
	 	 addflag=ltMsgGetVar_s(ltMsgPk,"addflag");
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(addflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		char *caurl=ltMsgGetVar_s(ltMsgPk,"caurl");
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"insert into nasruleurl values(%s,%s,%s,%s,'%s')",ruleid,ltMsgGetVar_s(ltMsgPk,"utf8"),ltMsgGetVar_s(ltMsgPk,"typeflag"),ltMsgGetVar_s(ltMsgPk,"action"),caurl);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }
	 if(delflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		char *caurl=ltMsgGetVar_s(ltMsgPk,"caurl");
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"delete from nasruleurl where ruleid=%s and caurl='%s'",ruleid,caurl);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }
 	 ltDbHeadPtr dbPtr;
 	 ltTablePtr tablePtr;
   ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 tablePtr=lt_dbput_table(dbPtr,"list");
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select * from nasruleurl where ruleid=%s",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				memset(str,0,sizeof(str));
				if(atoi(tempRow[3])==0){
					strgb2utf8("拒绝",action,strlen("拒绝"));
					sprintf(str,"|%s",action);
				}else if(atoi(tempRow[3])==1){
					strgb2utf8("允许",action,strlen("允许"));
					sprintf(str,"|%s",action);
				}
				if(atoi(tempRow[1])==1){
					sprintf(str,"%s|UTF8",str);
				}
				switch(atoi(tempRow[2])){
					case 0:
					sprintf(str,"%s|webGet",str);
					break;
					case 1:
					sprintf(str,"%s|webPost",str);
					break;
					case 2:
					sprintf(str,"%s|%s",str,all);
					break;
				}
				lt_dbput_recordvars(tablePtr,2,
				"type",LT_TYPE_STRING,str,
				"caurl",LT_TYPE_STRING,tempRow[4]);
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
	 }
	 ltDbClose(G_DbCon);
   lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/webpost.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//编辑weburl
int msaEditWeburl(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
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
	 char sqlBuf[128];
	 char *addflag,*delflag;
	 addflag=NULL;
	 delflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"addflag")!=NULL){
	 	 addflag=ltMsgGetVar_s(ltMsgPk,"addflag");
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(addflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		char *caurl=ltMsgGetVar_s(ltMsgPk,"caurl");
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"insert into nasruleurl values(%s,'%s')",ruleid,caurl);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }
	 if(delflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		char *caurl=ltMsgGetVar_s(ltMsgPk,"caurl");
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"delete from nasruleurl where ruleid=%s and caurl='%s'",ruleid,caurl);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }
 	 ltDbHeadPtr dbPtr;
 	 ltTablePtr tablePtr;
   ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 tablePtr=lt_dbput_table(dbPtr,"list");
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select * from nasruleurl where ruleid=%s",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,1,
				"caurl",LT_TYPE_STRING,tempRow[1]);
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
	 }
	 ltDbClose(G_DbCon);
   lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/weburl.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//编辑mudip userip
int msaEditip(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
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
	 char sqlBuf[128];
	 char *addflag,*delflag;
	 char url[64];
	 memset(url,0,sizeof(url));
	 if(strcmp(ltMsgGetVar_s(ltMsgPk,"table"),"nassrvruledip")==0){
	 		sprintf(url,"%s","/app/msa/msa/htmlplt/rule/mudip.htm");
	 }else if(strcmp(ltMsgGetVar_s(ltMsgPk,"table"),"nasruleip")==0){
	 		sprintf(url,"%s","/app/msa/msa/htmlplt/rule/userip.htm");
	 }
	 addflag=NULL;
	 delflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"addflag")!=NULL){
	 	 addflag=ltMsgGetVar_s(ltMsgPk,"addflag");
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(addflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		char *ipstart=ltMsgGetVar_s(ltMsgPk,"ipstart");
	 		char *ipend=ltMsgGetVar_s(ltMsgPk,"ipend");
	 		char *table=ltMsgGetVar_s(ltMsgPk,"table");
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"insert into %s values(%s,'%s','%s')",table,ruleid,ipstart,ipend);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }
	 if(delflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		char *ipstart=ltMsgGetVar_s(ltMsgPk,"ipstart");
	 		char *ipend=ltMsgGetVar_s(ltMsgPk,"ipend");
	 		char *table=ltMsgGetVar_s(ltMsgPk,"table");
	 		memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"delete from %s where ruleid=%s and ipstart='%s' and ipend='%s'",table,ruleid,ipstart,ipend);
	 		ltDbExecSql(G_DbCon,sqlBuf);
	 }
 	 ltDbHeadPtr dbPtr;
 	 ltTablePtr tablePtr;
   ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 tablePtr=lt_dbput_table(dbPtr,"list");
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 sprintf(sqlBuf,"select * from %s where ruleid=%s",ltMsgGetVar_s(ltMsgPk,"table"),ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,2,
				"ipstart",LT_TYPE_STRING,tempRow[1],
				"ipend",LT_TYPE_STRING,tempRow[2]);
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
	 }
	 ltDbClose(G_DbCon);
   lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,url,dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//显示选择用户树
int ShowSelectUserTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char url[64];
	 memset(url,0,sizeof(url));
	 sprintf(url,"/app/msa/msa/htmlplt/rule/%s.htm",ltMsgGetVar_s(ltMsgPk,"name"));
   ltDbHeadPtr dbPtr;
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
   lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,url,dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//部门可配置用户
int ruleGroupUserlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
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
	 char sqlBuf[512];
	 int groupid,level,ruleid;
	 int i,iHead,icount,limit,start,rcount;
	 icount=0;
	 rcount=0;
	 i=0;
	 iHead=1;
	 ruleid=-1;
	 limit=0;
	 start=0;
	 stralloc strTemp;
	 char caTmpp[2048];
	 strTemp.s=0;
	 ltDbCursor *tempCursor;
	 LT_DBROW tempRow;
	 groupid=atoi(ltMsgGetVar_s(ltMsgPk,"groupid"));
	 if(ltMsgGetVar_s(ltMsgPk,"ruleid")!=NULL){
	 	  ruleid=atoi(ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 }
	 level=_ltPubInfo->_GroupList[groupid].level;
	 char *countflag;
	 countflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"countflag")!=NULL){
	 	  countflag=ltMsgGetVar_s(ltMsgPk,"countflag");
	 }
	 if(countflag){
	 		char count[32];
	 		memset(count,0,sizeof(count));
	 		memset(sqlBuf,0,sizeof(sqlBuf));
			sprintf(sqlBuf,"select count(*) from msauser where groupid%d=%d",level,groupid);
			//printf("%s\n",sqlBuf);
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor!=NULL){
							tempRow= ltDbFetchRow(tempCursor);
							if(tempRow!=NULL){
									strcpy(count,tempRow[0]);
							}
			}
			ltDbCloseCursor(tempCursor);
			ltDbClose(G_DbCon);
		  ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,count);
			ltMsgFree(ltMsgPk);
			return 0;
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"limit")!=NULL){
	 	  limit=atoi(ltMsgGetVar_s(ltMsgPk,"limit"));
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"start")!=NULL){
	 	  start=atoi(ltMsgGetVar_s(ltMsgPk,"start"));
	 }
	 //printf("limit  %d \n",limit);
	 //printf("start  %d \n",start);
	 memset(caTmpp,0,sizeof(caTmpp));
	 sprintf(caTmpp,"[");
   stralloc_cats(&strTemp,caTmpp);
	 nasDbUserInfo *tmpdbuserinfo;
	 printf("1111111111111\n");
	 for(i=0;i<_ltPubInfo->_dbUserInfoHead.bufnum;i++){
		 		tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,i);
	   		if(tmpdbuserinfo==NULL&&tmpdbuserinfo->groupid[level]!=groupid)continue;
	   								 //printf("icount  %d \n",icount);
	   								 if(icount<start) {
	   								 		icount++;
	   								 		continue;
	   								 }
	   								 if(limit-1<rcount)break;
	   								 memset(sqlBuf,0,sizeof(sqlBuf));
										 sprintf(sqlBuf,"select count(*) from nasruleuser where ruleid=%d and userid=%d",ruleid,tmpdbuserinfo->userid);
										 //printf("nasruleuser %s \n",sqlBuf);
										 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
										 if(tempCursor!=NULL){
												tempRow= ltDbFetchRow(tempCursor);
												while(tempRow!=NULL){
																if(iHead==1){
																	if(atoi(tempRow[0])==0){
																			sprintf(caTmpp,"{id: '%d',text: '%s',checked:false,leaf: true",tmpdbuserinfo->userid,tmpdbuserinfo->dispname);
																	}else{
																			sprintf(caTmpp,"{id: '%d',text: '%s',checked:true,leaf: true",tmpdbuserinfo->userid,tmpdbuserinfo->dispname);
																  }
																	iHead=0;
																}else{
																	if(atoi(tempRow[0])==0){
																  		sprintf(caTmpp,",{id: '%d',text: '%s',checked:false,leaf: true",tmpdbuserinfo->userid,tmpdbuserinfo->dispname);
																	}else{
																  		sprintf(caTmpp,",{id: '%d',text: '%s',checked:true,leaf: true",tmpdbuserinfo->userid,tmpdbuserinfo->dispname);
																  }
															  }
															  stralloc_cats(&strTemp,caTmpp);
																stralloc_cats(&strTemp,"}");
																tempRow= ltDbFetchRow(tempCursor);
												}
												ltDbCloseCursor(tempCursor);
										 }
										 icount++;
										 rcount++;
	 }
   stralloc_cats(&strTemp,"]");
	 stralloc_0(&strTemp);
	 //printf("%s \n",strTemp.s);
	 ltDbClose(G_DbCon);
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;
}


int msaRuleSrvTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{	//数据库连接
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
	char tmpstr[1024],rulesrv[1002],sqlBuf[128];
	stralloc strTemp;
  int j,i;
	int iHead;
	LT_DBROW tempRow;
	ltDbCursor *tempCursor;
	memset(rulesrv,0,sizeof(rulesrv));
	sprintf(rulesrv,"%s","0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	memset(sqlBuf,0,sizeof(sqlBuf));
	sprintf(sqlBuf,"select rulesrv0,rulesrv1,rulesrv2,rulesrv3 from nassrvrulesrv where sid=%s",ltMsgGetVar_s(ltMsgPk,"sid"));
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow!=NULL){
					memset(rulesrv,0,sizeof(rulesrv));
        	sprintf(rulesrv,"%s%s%s%s",tempRow[0],tempRow[1],tempRow[2],tempRow[3]);
        	//printf("%s\n",rulesrv);
			}
			ltDbCloseCursor(tempCursor);
	}
	
	memset(tmpstr,0,sizeof(tmpstr));
	iHead=1;
	strTemp.s=0;
	stralloc_cats(&strTemp,"[");
 	for(j=0;j<MAX_TOP_SRVNUM;j++){
 			if(strcmp(_ltPubInfo->topSrvName[j].srvname,"")!=0){
					if(iHead==1){
							sprintf(tmpstr,"{id: '%d',text: '%s'",j,_ltPubInfo->topSrvName[j].srvname);
					    iHead=0;
					    for(i=0;i<MAX_SYS_SRVNUM;i++){
						 			if(_ltPubInfo->ltService[i].topSrv==j&&strcmp(_ltPubInfo->ltService[i].srvName,"")!=0){
						 					if(rulesrv[i]=='1'){
													sprintf(tmpstr,"%s,cls:'x-tree-node-italic-blue'",tmpstr);
													break;
											}
								  }
							}
					}else{
							sprintf(tmpstr,",{id: '%d',text: '%s'",j,_ltPubInfo->topSrvName[j].srvname);
					  	for(i=0;i<MAX_SYS_SRVNUM;i++){
						 			if(_ltPubInfo->ltService[i].topSrv==j&&strcmp(_ltPubInfo->ltService[i].srvName,"")!=0){
						 					if(rulesrv[i]=='1'){
					  								sprintf(tmpstr,"%s,cls: 'x-tree-node-italic-blue'",tmpstr);
					  								break;
											}
								  }
							}
				  }
				  stralloc_cats(&strTemp,tmpstr);
				  stralloc_cats(&strTemp,",leaf: true}");
		  }
	}
	stralloc_cats(&strTemp,"]");
  stralloc_0(&strTemp);
  ltDbClose(G_DbCon);
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);
	return 0;
}

//策略已配置用户列表
int serviceUserlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//数据库连接
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
	 char sqlStr[1024];
	 int allcount,k;
	 char *delflag;
	 delflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	  delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(delflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		char *userid=ltMsgGetVar_s(ltMsgPk,"userid");
	 		sprintf(sqlStr,"delete from nasruleuser where ruleid=%s and userid=%s",ruleid,userid);
	 		ltDbExecSql(G_DbCon,sqlStr);
	 }
	 k=0;
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
	 stralloc strTemp;
	 char caTmpp[1024];
	 strTemp.s=0;
	 allcount=0;
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"select count(*) from nasruleuser where ruleid=%s",ltMsgGetVar_s(ltMsgPk,"ruleid"));
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	if(tempRow!=NULL){
       		allcount=atol(tempRow[0]);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"select * from nasruleuser where ruleid=%s",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
	 nasDbUserInfo *tmpdbuserinfo;
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
	 		 		tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,atoi(tempRow[1]));
       		if(tmpdbuserinfo!=NULL){
       				if(k==0){
			     	  	 sprintf(caTmpp,"{\"userid\":\"%s\",\"username\":\"%s\",\"dispname\":\"%s\"}",tempRow[1],tmpdbuserinfo->username,tmpdbuserinfo->dispname);
			     	     stralloc_cats(&strTemp,caTmpp);
			     	  }else{
			     	  	 sprintf(caTmpp,",{\"userid\":\"%s\",\"username\":\"%s\",\"dispname\":\"%s\"}",tempRow[1],tmpdbuserinfo->username,tmpdbuserinfo->dispname);
			     	     stralloc_cats(&strTemp,caTmpp);
			     	  }
       		}
	     	  k++;
       		tempRow= ltDbFetchRow(tempCursor);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   ltDbClose(G_DbCon);
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//配置用户
int addruleuser(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
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
	 char sqlstr[1024];
   char *delflag;
   delflag=NULL;
	 char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 char *userid=ltMsgGetVar_s(ltMsgPk,"userid");
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	  delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(delflag){
	 		memset(sqlstr,0,sizeof(sqlstr));
		 	sprintf(sqlstr,"delete from nasruleuser where ruleid=%s and userid=%s",ruleid,userid);
		 	//printf("DELETE %s\n",sqlstr);
		 	ltDbExecSql(G_DbCon,sqlstr);
		 	ltDbClose(G_DbCon);
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
		  ltMsgFree(ltMsgPk);
		  return 0;
	 }
	 memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"insert into nasruleuser values(%s,%s)",ruleid,userid);
	 //printf("INSERT %s\n",sqlstr);
	 ltDbExecSql(G_DbCon,sqlstr);
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
   ltMsgFree(ltMsgPk);
   return 0;
}
//规则的URL库修改
int ShowUrlSort(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 //数据库连接
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
	 int i;
	 char ruleurlsort[101],str[8];
	 memset(ruleurlsort,0,sizeof(ruleurlsort));
	 i=0;
	 char sqlstr[1024];
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
   ltDbHeadPtr dbPtr;
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
   lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));

	 memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"select ruleurlsort from nassrvrule where ruleid=%s",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlstr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
       			sprintf(ruleurlsort,"%s",tempRow[0]);
   					lt_dbput_rootvars(dbPtr,1,"ruleurlsort",tempRow[0]);
						tempRow= ltDbFetchRow(tempCursor);
			  }
       	ltDbCloseCursor(tempCursor);
   }
   for(i=0;i<1;i++){
        memset(str,0,sizeof(str));
				sprintf(str,"chksort%d",i);
				if(ruleurlsort[i]==49){
					 lt_dbput_rootvars(dbPtr,1,str,"checked");
				}
   }
   ltDbClose(G_DbCon);
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/urladdress.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//规则的URL库修改
int ShowAddress(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 //数据库连接
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
	 int i;
	 char rulesrv[511],str[8];
	 memset(rulesrv,0,sizeof(rulesrv));
	 i=0;
	 char sqlstr[1024];
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
   ltDbHeadPtr dbPtr;
	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
   lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"ruleid"));
   memset(sqlstr,0,sizeof(sqlstr));
	 sprintf(sqlstr,"select rulesrv0,rulesrv1 from nassrvrule where ruleid=%s",ltMsgGetVar_s(ltMsgPk,"ruleid"));
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlstr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
   					sprintf(rulesrv,"%s%s",tempRow[0],tempRow[1]);
						tempRow= ltDbFetchRow(tempCursor);
			  }
       	ltDbCloseCursor(tempCursor);
   }
   lt_dbput_rootvars(dbPtr,1,"rulesrv",rulesrv);
   for(i=0;i<500;i++){
        memset(str,0,sizeof(str));
				sprintf(str,"chksort%d",i);
				if(rulesrv[i]==49){
					 lt_dbput_rootvars(dbPtr,1,str,"checked");
				}
   }
   ltDbClose(G_DbCon);
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/serviceaddress.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}

//msaService 树
int msaltServiceTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char tmpstr[1024],rulesrv[1001];
	stralloc strTemp;
	int iHead,i,srvnum;
	i=0;
	iHead=1;
	strTemp.s=0;
	stralloc_cats(&strTemp,"[");
	sprintf(rulesrv,"%s",ltMsgGetVar_s(ltMsgPk,"rulesrv"));
	srvnum=atoi(ltMsgGetVar_s(ltMsgPk,"topsrv"));
 	for(i=0;i<MAX_SYS_SRVNUM;i++){
 			if(_ltPubInfo->ltService[i].topSrv==srvnum&&strcmp(_ltPubInfo->ltService[i].srvName,"")!=0){
				if(iHead==1){
					if(rulesrv[i]==49){
							sprintf(tmpstr,"{id: '%d',text: '%s',checked:%s,leaf: true",i,_ltPubInfo->ltService[i].srvName,"true");
					}else{
							sprintf(tmpstr,"{id: '%d',text: '%s',checked:%s,leaf: true",i,_ltPubInfo->ltService[i].srvName,"false");
				  }
					iHead=0;
				}else{
					if(rulesrv[i]==49){
				  		sprintf(tmpstr,",{id: '%d',text: '%s',checked:%s,leaf: true",i,_ltPubInfo->ltService[i].srvName,"true");
					}else{
				  		sprintf(tmpstr,",{id: '%d',text: '%s',checked:%s,leaf: true",i,_ltPubInfo->ltService[i].srvName,"false");
				  }
			  }
			  stralloc_cats(&strTemp,tmpstr);
				stralloc_cats(&strTemp,"}");
			}
	}
	stralloc_cats(&strTemp,"]");
	stralloc_0(&strTemp);
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);
	return 0;
}

int nasAppRule(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	_ltPubInfo->lRunning=0;
	sleep(1);
  nasInitRule(_ltPubInfo);
  _ltPubInfo->lRunning=1;
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"0");
	ltMsgFree(ltMsgPk);
	return 0;
}

//显示规则修改
int showUserPolicy(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 checkTimeOut(confd,ltMsgPk,lt_MMHead);
	 char viewtype[4];
	 strcpy(viewtype,"0");
	 ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
	 			strcpy(viewtype,ltMsgGetVar_s(ltMsgPk,"type"));
	 }
	 lt_dbput_rootvars(dbPtr,1,"type",viewtype);
   lt_db_htmlpage(dbPtr,"utf-8");
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/userPolicy.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//显示规则修改
int showPolicyTemplate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 checkTimeOut(confd,ltMsgPk,lt_MMHead);
	 ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/PolicyTemplateList.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}

int msaOneServiceTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char tmpstr[1024];
	stralloc strTemp;
  int j;
	int iHead;
	memset(tmpstr,0,sizeof(tmpstr));
	iHead=1;
	strTemp.s=0;
	stralloc_cats(&strTemp,"[");
 	for(j=0;j<MAX_TOP_SRVNUM;j++){
 			if(strcmp(_ltPubInfo->topSrvName[j].srvname,"")!=0){
					if(iHead==1){
							sprintf(tmpstr,"{id: '%d',text: '%s'",j,_ltPubInfo->topSrvName[j].srvname);
					    iHead=0;
					}else{
					  	sprintf(tmpstr,",{id: '%d',text: '%s'",j,_ltPubInfo->topSrvName[j].srvname);
				  }
				  stralloc_cats(&strTemp,tmpstr);
				  stralloc_cats(&strTemp,",leaf: true}");
		  }
	}
	stralloc_cats(&strTemp,"]");
  stralloc_0(&strTemp);
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);
	return 0;
}


int msaTwoServiceTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char tmpstr[1024];
	stralloc strTemp;
  int j;
	int iHead;
	memset(tmpstr,0,sizeof(tmpstr));
	iHead=1;
	strTemp.s=0;
	long srvnum=0;
  srvnum=atol(ltMsgGetVar_s(ltMsgPk,"id"));
	stralloc_cats(&strTemp,"[");
 	for(j=0;j<MAX_SYS_SRVNUM;j++){
 			if(_ltPubInfo->ltService[j].topSrv==srvnum&&strcmp(_ltPubInfo->ltService[j].srvName,"")!=0){
					if(iHead==1){
							sprintf(tmpstr,"{id: '%d',text: '%s'",j,_ltPubInfo->ltService[j].srvName);
					    iHead=0;
					}else{
					  	sprintf(tmpstr,",{id: '%d',text: '%s'",j,_ltPubInfo->ltService[j].srvName);
				  }
				  stralloc_cats(&strTemp,tmpstr);
				  stralloc_cats(&strTemp,",leaf: true}");
		  }
	}
	stralloc_cats(&strTemp,"]");
  stralloc_0(&strTemp);
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);
	return 0;
}
char *msaSMSsend(char *telstr,char *msgstr,char *resultstr){
	int  iSocket;
 	struct sockaddr_in dest;
	struct hostent   *hp   =   NULL;
	iSocket = (-1);

	if((hp=gethostbyname("utf8.sms.webchinese.cn"))!=NULL){
		memcpy(&(dest.sin_addr),hp-> h_addr,hp-> h_length);
		dest.sin_family=hp-> h_addrtype;
		//printf( "IP:!dest.sin_addr   =   %s\n ",inet_ntoa(dest.sin_addr));
	}
  iSocket = lt_tcpconnect(inet_ntoa(dest.sin_addr),80); //80 or 443
	if(iSocket < 0){
		perror("oops:iSocket");
		return 0;
	}
	//发送数据 
	char str1[6114],msg[2048],str[2048];
	memset(msg,0,2048);
	memset(str1, 0, 6114);
	memset(str,0,2048);
	sprintf(str,"%s",msgstr);
	
	sprintf(msg,"GET /?Uid=%s&Key=%s&smsMob=%s&smsText=%s HTTP/1.1\r\n",SMSname,SMSpwd,telstr,str);
	//sprintf(msg,"GET /?Uid=%s&Key=%s&smsMob=%s&smsText=上网过滤策略删除成功，如有疑问请联系管理员 HTTP/1.1\r\n",SMSname,SMSpwd,telstr);
	strcat(str1, msg);
	strcat(str1, "Accept: image/gif, image/jpeg, image/pjpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*\r\n");
	strcat(str1, "Accept-Language: zh-cn\r\n");
	strcat(str1, "User-Agent: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; QQDownload 716; InfoPath.2; 360SE)\r\n");
	strcat(str1, "Accept-Enconding: gzip, deflate\r\n");
	strcat(str1, "Host: utf8.sms.webchinese.cn\r\n");
	strcat(str1, "Connection: Keep-Alive\r\n");
	strcat(str1, "Cookie: \r\n");
	strcat(str1, "\r\n");
	
	printf("%s\n",str1);
	write(iSocket,str1,strlen(str1));
	read(iSocket,resultstr,sizeof(resultstr));
	printf("%s\n",resultstr);
	return resultstr;
}
