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
//显示规则模板
int showService(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 int j;
   char sid[32];
   ltDbHeadPtr dbPtr;
   ltTablePtr tablePtr;
	 dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   tablePtr=lt_dbput_table(dbPtr,"qoslist");
   for(j=0;j<_MAX_QOSSTATIC_CHANEL;j++){
   	if(strcmp(_ltPubInfo->_QosList[j].qosname,"")!=0){
	 		memset(sid,0,sizeof(sid));
	 		sprintf(sid,"%d",_ltPubInfo->_QosList[j].sid);
   		lt_dbput_recordvars(tablePtr,2,
			"qosid",LT_TYPE_STRING,sid,
			"qosname",LT_TYPE_STRING,_ltPubInfo->_QosList[j].qosname);
   	}
   }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/serviceAdd.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//规则添加
int addService(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//数据库连接
//		  char *dbUser;
//		  char *dbPass;
//		  char *dbName;
//		  dbName=_ltPubInfo->_dbname;
//			dbUser=_ltPubInfo->_dbuser;
//			dbPass=_ltPubInfo->_dbpass;
//			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
//			if(G_DbCon!=NULL){
//				//printf("db connect ok\n");
//			}else{
//				fprintf(stderr,"db connect error\n");
//			}
	 int ruleflag,ruleaction,logaction,conaction,qosid,ruleschedule,rulejjr;
	 char rulename[32],ruleurl[128],sqlstr[2048],policy[129],group[1001],g1[251],g2[251],g3[251],g4[251];
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
	 ruleaction=atoi(ltMsgGetVar_s(ltMsgPk,"ruleaction"));
	 logaction=atoi(ltMsgGetVar_s(ltMsgPk,"logaction"));
	 conaction=atoi(ltMsgGetVar_s(ltMsgPk,"conaction"));
	 qosid=atoi(ltMsgGetVar_s(ltMsgPk,"qosid"));
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
	 sprintf(sqlstr,"insert into nassrvrule(ruleid,ruleflag,ruleaction,logaction,conaction,qosid,ruleschedule,rulejjr,rulename,ruleurl,group0,group1,group2,group3,policyid,rulerate) values (%d,%d,%d,%d,%d,%d,%d,%d,%s,'%s','%s','%s','%s','%s','%s',%f)",lCount,ruleflag,ruleaction,logaction,conaction,qosid,ruleschedule,rulejjr,rulename,ruleurl,g1,g2,g3,g4,policy,temprio);
	 ltDbExecSql(G_DbCon,sqlstr);
	 printf("%s \n",sqlstr);
	 
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
   ltMsgFree(ltMsgPk);
   return 0;
}
////规则修改
//int updateService(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//	 int ruleid,ruleflag,ruleaction,ruleschedule,rulejjr,qosid,logaction,conaction;
//	 char rulename[32],rulesrv0[251],rulesrv1[251],group0[251],group1[251],group2[251],group3[251],policyid[129],ruleurlsort[101],ruleurl[128];
//	 ruleid=atoi(ltMsgGetVar_s(ltMsgPk,"urleid"));
//	 ruleflag=atoi(ltMsgGetVar_s(ltMsgPk,"ruleflag"));
//	 ruleaction=atoi(ltMsgGetVar_s(ltMsgPk,"ruleaction"));
//	 ruleschedule=atoi(ltMsgGetVar_s(ltMsgPk,"ruleschedule"));
//	 rulejjr=atoi(ltMsgGetVar_s(ltMsgPk,"rulejjr"));
//	 qosid=atoi(ltMsgGetVar_s(ltMsgPk,"qosid"));
//	 logaction=atoi(ltMsgGetVar_s(ltMsgPk,"logaction"));
//	 conaction=atoi(ltMsgGetVar_s(ltMsgPk,"conaction"));
//	 sprintf(rulename,"%s",ltMsgGetVar_s(ltMsgPk,"rulename"));
//}
//编辑规则
int msaEditSrvRule(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char *type;
	 type=NULL;
	 char url[32];
	 if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
	 	 type=ltMsgGetVar_s(ltMsgPk,"type");
	 }
	 sprintf(url,"/app/msa/msa/htmlplt/users/%s.htm",type);
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
//规则列表
int msaSrvRuleList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
//数据库连接
//		  char *dbUser;
//		  char *dbPass;
//		  char *dbName;
//		  dbName=_ltPubInfo->_dbname;
//			dbUser=_ltPubInfo->_dbuser;
//			dbPass=_ltPubInfo->_dbpass;
//			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
//			if(G_DbCon!=NULL){
//				//printf("db connect ok\n");
//			}else{
//				fprintf(stderr,"db connect error\n");
//			}
	 char sqlStr[1024];
	 int allcount,k;
	 char *delflag;
	 delflag=NULL;
	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(delflag){
	 		char *ruleid=ltMsgGetVar_s(ltMsgPk,"ruleid");
	 		sprintf(sqlStr,"delete from nassrvrule where ruleid=%s",ruleid);
	 		printf("%s \n",sqlStr);
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
	 sprintf(sqlStr,"%s","select count(*) from nassrvrule");
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
	 sprintf(sqlStr,"%s","select ruleid,rulename,ruleflag,ruleaction,rulerate from nassrvrule order by rulerate DESC");
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"ruleid\":\"%s\",\"rulename\":\"%s\",\"ruleflag\":\"%s\",\"ruleaction\":\"%s\",\"orderNum\":%s}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4]);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"ruleid\":\"%s\",\"rulename\":\"%s\",\"ruleflag\":\"%s\",\"ruleaction\":\"%s\",\"orderNum\":%s}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4]);
	     	     stralloc_cats(&strTemp,caTmpp);
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
