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
#include <iconv.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include "msa.h"

//策略组列表
int msaListUserPolicy(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char sqlStr[1024];
	 int allcount,k;
	 char *delflag;
	 delflag=NULL;

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

	 if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 if(delflag){
	 		char *sid=ltMsgGetVar_s(ltMsgPk,"sid");
	 		sprintf(sqlStr,"delete from msauserpolicy where sid='%s'",sid);
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
	 sprintf(sqlStr,"%s","select count(*) from msauserpolicy");
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow=ltDbFetchRow(tempCursor);
       	if(tempRow!=NULL){
       		allcount=atol(tempRow[0]);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"%s","select msauserpolicy.sid,msauserpolicy.policyname,msaqos.qosname,msauserpolicy.ratetype,msauserpolicy.maxudp,msauserpolicy.maxtcp,msauserpolicy.maxudpp,msauserpolicy.maxtcpp,msauserpolicy.prio from msauserpolicy left join msaqos on msauserpolicy.ratelimit=msaqos.sid order by prio DESC");
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"sid\":\"%s\",\"policyname\":\"%s\",\"ratelimit\":\"%s\",\"ratetype\":\"%s\",\"maxudp\":%s,\"maxtcp\":%s,\"maxudpp\":%s,\"maxtcpp\":%s,\"sorderNum\":%s}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8]);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"sid\":\"%s\",\"policyname\":\"%s\",\"ratelimit\":\"%s\",\"ratetype\":\"%s\",\"maxudp\":%s,\"maxtcp\":%s,\"maxudpp\":%s,\"maxtcpp\":%s,\"sorderNum\":%s}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8]);
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
	 ltDbClose(G_DbCon);
	 return 0;
}


//优先级别修改
int msaUpdatePrio(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char *before,*after,*table;
	 char sql[128];
	 int type;
	 type=atoi(ltMsgGetVar_s(ltMsgPk,"type"));
	 table=ltMsgGetVar_s(ltMsgPk,"table");
	 before=ltMsgGetVar_s(ltMsgPk,"before");
	 after=ltMsgGetVar_s(ltMsgPk,"after");
	 if(type==0){
	 		sprintf(sql,"update %s set prio=%s where sid=%s",table,after,before);
	 }else if(type==1){
	 		sprintf(sql,"update %s set rulerate=%s where ruleid=%s",table,after,before);
	 }
	 ltDbExecSql(G_DbCon,sql);
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n",LT_TYPE_STRING,0);
	 ltMsgFree(ltMsgPk);
	 return 0;
}
//策略组修改添加
int msaAddPrio(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char policy[129],name[128],topSrvNameList[36],moreflag[33],g1[251],g2[251],g3[251],g4[251],group[1001];
	 int sid,jftype,ratelimit,ratetype,i;
	 float accountnum;
	 unsigned int maxudp,lLimitTimes,maxtcp,maxudpp,lLimitTimesw,maxtcpp,maxupp,lLimitTimesm,maxdownp,maxpackage,maxsp,maxBytes,maxnc;
	 unsigned long long lLimitBytes,lLimitBytesw,lLimitBytesm;
	 //数据库连接
	 char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
			ltMsgFree(ltMsgPk);
			return 0;
	 }
	 //ltDbClose(G_DbCon);
	 char sqlBuf[4096];
	 memset(sqlBuf,0,sizeof(sqlBuf));
	 memset(policy,0,sizeof(policy));
	 memset(group,0,sizeof(group));
	 memset(moreflag,0,sizeof(moreflag));
	 memset(topSrvNameList,0,sizeof(topSrvNameList));
	 memset(name,0,sizeof(name));
	 sid=0;
	 i=0;

   ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
	 if(strcmp(ltMsgGetVar_s(ltMsgPk,"sid"),"")!=0){
   	sid=atoi(ltMsgGetVar_s(ltMsgPk,"sid"));
   }else{
   		for(i=0;i<1000;i++){
   					memset(sqlBuf,0,sizeof(sqlBuf));
		        sprintf(sqlBuf,"select count(*) from msauserpolicy where sid=%d ",i);
						tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
						if(tempCursor){
							tempRow= ltDbFetchRow(tempCursor);
							if(tempRow){
								if(atoi(tempRow[0])==0){
									sid=i;
									ltDbCloseCursor(tempCursor);
									break;
								}
							}
							ltDbCloseCursor(tempCursor);
						}
   		}
   }
   if(ltMsgGetVar_s(ltMsgPk,"jftype")){
   	jftype=atoi(ltMsgGetVar_s(ltMsgPk,"jftype"));
   }else{
   	jftype=0;
   }

   if(ltMsgGetVar_s(ltMsgPk,"accountnum")){
   	accountnum=atof(ltMsgGetVar_s(ltMsgPk,"accountnum"));
   }else{
   	accountnum=0.0;
   }

   if(ltMsgGetVar_s(ltMsgPk,"ratelimit")){
   	ratelimit=atoi(ltMsgGetVar_s(ltMsgPk,"ratelimit"));
   }else{
   	ratelimit=0;
   }

   if(ltMsgGetVar_s(ltMsgPk,"ratetype")){
   	ratetype=atoi(ltMsgGetVar_s(ltMsgPk,"ratetype"));
   	printf("ratetype:%d\n",ratetype);
   }else{
   	ratetype=0;
   }

   if(ltMsgGetVar_s(ltMsgPk,"topSrvNameList")){
   	sprintf(topSrvNameList,"%s",ltMsgGetVar_s(ltMsgPk,"topSrvNameList"));
   }else{
   	sprintf(topSrvNameList,"%s","11111111111111111111111111111111111");
   }

   if(ltMsgGetVar_s(ltMsgPk,"lLimitBytes")){
   	lLimitBytes=atoll(ltMsgGetVar_s(ltMsgPk,"lLimitBytes"));
   }else{
   	lLimitBytes=0;
   }

   if(ltMsgGetVar_s(ltMsgPk,"maxudp")){
   	maxudp=atol(ltMsgGetVar_s(ltMsgPk,"maxudp"));
   }else{
   	maxudp=0;
   }

   if(ltMsgGetVar_s(ltMsgPk,"lLimitTimes")){
   	lLimitTimes=atol(ltMsgGetVar_s(ltMsgPk,"lLimitTimes"));
   }else{
   	lLimitTimes=0;
   }

   if(ltMsgGetVar_s(ltMsgPk,"maxtcp")){
   	maxtcp=atol(ltMsgGetVar_s(ltMsgPk,"maxtcp"));
   }else{
   	maxtcp=0;
   }

   if(ltMsgGetVar_s(ltMsgPk,"lLimitBytesw")){
   	lLimitBytesw=atoll(ltMsgGetVar_s(ltMsgPk,"lLimitBytesw"));
   }else{
   	lLimitBytesw=0;
   }

	 if(ltMsgGetVar_s(ltMsgPk,"maxudpp")){
	 	maxudpp=atol(ltMsgGetVar_s(ltMsgPk,"maxudpp"));
	 }else{
	 	maxudpp=0;
	 }

	 if(ltMsgGetVar_s(ltMsgPk,"lLimitTimesw")){
	 	lLimitTimesw=atol(ltMsgGetVar_s(ltMsgPk,"lLimitTimesw"));
	 }else{
	 	lLimitTimesw=0;
	 }

	 if(ltMsgGetVar_s(ltMsgPk,"maxtcpp")){
	 	maxtcpp=atol(ltMsgGetVar_s(ltMsgPk,"maxtcpp"));
	 }else{
	 	maxtcpp=0;
	 }

	 if(ltMsgGetVar_s(ltMsgPk,"lLimitBytesm")){
	 	lLimitBytesm=atoll(ltMsgGetVar_s(ltMsgPk,"lLimitBytesm"));
	 }else{
	 	lLimitBytesm=0;
	 }


	 if(ltMsgGetVar_s(ltMsgPk,"maxupp")){
	 	maxupp=atol(ltMsgGetVar_s(ltMsgPk,"maxupp"));
	 }else{
	 	maxupp=0;
	 }

	 if(ltMsgGetVar_s(ltMsgPk,"lLimitTimesm")){
	 	lLimitTimesm=atol(ltMsgGetVar_s(ltMsgPk,"lLimitTimesm"));
	 }else{
	  lLimitTimesm=0;
	 }

	 if(ltMsgGetVar_s(ltMsgPk,"maxdownp")){
	 	maxdownp=atol(ltMsgGetVar_s(ltMsgPk,"maxdownp"));
	 }else{
	 	maxdownp=0;
	 }

	 if(ltMsgGetVar_s(ltMsgPk,"maxpackage")){
	 	 maxpackage=atol(ltMsgGetVar_s(ltMsgPk,"maxpackage"));
	 }else{
	   maxpackage=0;
	 }

	 if(ltMsgGetVar_s(ltMsgPk,"maxsp")){
	 	 maxsp=atol(ltMsgGetVar_s(ltMsgPk,"maxsp"));
	 }else{
	 	 maxsp=0;
	 }

	 if(ltMsgGetVar_s(ltMsgPk,"maxBytes")){
	 	 maxBytes=atol(ltMsgGetVar_s(ltMsgPk,"maxBytes"));
	 }else{
	   maxBytes=0;
	 }

	 if(ltMsgGetVar_s(ltMsgPk,"maxnc")){
	 	 maxnc=atol(ltMsgGetVar_s(ltMsgPk,"maxnc"));
	 }else{
	 	 maxnc=0;
	 }


	 if(ltMsgGetVar_s(ltMsgPk,"policy")){
	 	 sprintf(policy,"%s",ltMsgGetVar_s(ltMsgPk,"policy"));
	 }else{
	   sprintf(policy,"%s","0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	 }
	 if(ltMsgGetVar_s(ltMsgPk,"pname")){
   	  sprintf(name,"%s",ltMsgGetVar_s(ltMsgPk,"pname"));
   }

	 if(ltMsgGetVar_s(ltMsgPk,"group")){
	 	sprintf(group,"%s",ltMsgGetVar_s(ltMsgPk,"group"));
	 }else{
	 	sprintf(group,"%s","0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	 }

	 if(ltMsgGetVar_s(ltMsgPk,"moreflag")){
	 	sprintf(moreflag,"%s",ltMsgGetVar_s(ltMsgPk,"moreflag"));
	 }else{
	 	sprintf(moreflag,"%s","00000000000000000000000000000000");
	 }

   memset(g1,0,sizeof(g1));
	 memset(g2,0,sizeof(g2));
	 memset(g3,0,sizeof(g3));
	 memset(g4,0,sizeof(g4));

 	 memcpy(g1,group,250);
	 memcpy(g2,group+250,250);
	 memcpy(g3,group+500,250);
	 memcpy(g4,group+750,250);
	 memset(sqlBuf,0,sizeof(sqlBuf));

   if(ltMsgGetVar_s(ltMsgPk,"addflag")){
   		sprintf(sqlBuf,"insert into msauserpolicy (policyid,policyname,topSrvNameList,moreflag,lLimitBytesw,lLimitBytes,lLimitBytesm,maxBytes,accountnum,lLimitTimes,lLimitTimesw,lLimitTimesm,jftype,maxpackage,maxudp,maxtcp,maxudpp,maxtcpp,maxupp,maxdownp,maxsp,maxnc,sid,group0,group1,group2,group3,ratelimit,ratetype)values \
			('%s','%s','%s','%s',%lld,%lld,%lld,%d,%0.6f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,'%s','%s','%s','%s',%d,%d)",policy,name,topSrvNameList,moreflag,lLimitBytesw,lLimitBytes,lLimitBytesm,maxBytes,accountnum,lLimitTimes,lLimitTimesw,lLimitTimesm,jftype,maxpackage,maxudp,maxtcp,maxudpp,maxtcpp,maxupp,maxdownp,maxsp,maxnc,sid,g1,g2,g3,g4,ratelimit,ratetype);
		  printf("insert:%s \n",sqlBuf);
		  ltDbExecSql(G_DbCon,sqlBuf);
		  ltDbClose(G_DbCon);
	 }else{
	 	  sprintf(sqlBuf,"update msauserpolicy set policyid='%s',policyname='%s',ratelimit=%d,ratetype=%d,topsrvnamelist='%s',moreflag='%s',lLimitBytesw=%lld,lLimitBytes=%lld,lLimitBytesm=%lld,maxBytes=%d,accountnum=%0.6f,lLimitTimes=%d,lLimitTimesw=%d,lLimitTimesm=%d,jftype=%d,maxpackage=%d,maxudp=%d,maxtcp=%d,maxudpp=%d,maxtcpp=%d,maxupp=%d,maxdownp=%d,maxsp=%d,maxnc=%d,group0='%s',group1='%s',group2='%s',group3='%s' where sid=%d",policy,name,ratelimit,ratetype,topSrvNameList,moreflag,lLimitBytesw,lLimitBytes,lLimitBytesm,maxBytes,accountnum,lLimitTimes,lLimitTimesw,lLimitTimesm,jftype,maxpackage,maxudp,maxtcp,maxudpp,maxtcpp,maxupp,maxdownp,maxsp,maxnc,g1,g2,g3,g4,sid);
		  printf("update:%s \n",sqlBuf);
		  ltDbExecSql(G_DbCon,sqlBuf);
		  ltDbClose(G_DbCon);
	 }
	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");
   ltMsgFree(ltMsgPk);
   ltDbClose(G_DbCon);
   return 0;
}


//编辑策略组
int msaEditPrio(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 int sid=0;
	 char sqlBuf[1024];
	 ltDbHeadPtr dbPtr;
	 ltTablePtr  tablePtr;
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
   int myratelimit=0;

   //数据库连接
	 char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,"db error!");
			ltMsgFree(ltMsgPk);
			return 0;
	 }
	 //ltDbClose(G_DbCon);

   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");

   if(ltMsgGetVar_s(ltMsgPk,"sid")){
   	 sid=atol(ltMsgGetVar_s(ltMsgPk,"sid"));
   }


   sprintf(sqlBuf,"select sid,policyname,prio,ratelimit,ratetype,topsrvnamelist,llimitbytes,llimittimes,llimitbytesw,llimittimesw,llimitbytesm,llimittimesm,moreflag,jftype,accountnum,maxpackage,maxbytes,maxudp,maxtcp,maxudpp,maxtcpp,maxupp,maxdownp,maxsp,maxnc,group0,group1,group2,group3,policyid from msauserpolicy where sid=%d",sid);
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	 if(tempCursor){
		    tempRow=ltDbFetchRow(tempCursor);
				if(tempRow){
					//基本参数
					lt_dbput_rootvars(dbPtr,1,"sid",tempRow[0]);
					lt_dbput_rootvars(dbPtr,1,"policyname",tempRow[1]);

					char tempjftype[32];
					sprintf(tempjftype,"jftype%s",tempRow[13]);
					lt_dbput_rootvars(dbPtr,1,tempjftype,"selected");

					lt_dbput_rootvars(dbPtr,1,"accountnum",tempRow[14]);

					myratelimit=atoi(tempRow[3]);

					char tempratetype[32];
					sprintf(tempratetype,"ratetype%s",tempRow[4]);
					lt_dbput_rootvars(dbPtr,1,tempratetype,"selected");

					//service 服务按钮控制
					lt_dbput_rootvars(dbPtr,1,"topsrvnamelist",tempRow[5]);
					printf("tempRow[5]:%s\n",tempRow[5]);

					lt_dbput_rootvars(dbPtr,1,"lLimitBytes",tempRow[6]);
					lt_dbput_rootvars(dbPtr,1,"maxudp",tempRow[17]);

					lt_dbput_rootvars(dbPtr,1,"lLimitTimes",tempRow[7]);
					lt_dbput_rootvars(dbPtr,1,"maxtcp",tempRow[18]);

					lt_dbput_rootvars(dbPtr,1,"lLimitBytesw",tempRow[8]);
					lt_dbput_rootvars(dbPtr,1,"maxudpp",tempRow[19]);

					lt_dbput_rootvars(dbPtr,1,"lLimitTimesw",tempRow[9]);
					lt_dbput_rootvars(dbPtr,1,"maxtcpp",tempRow[20]);

					lt_dbput_rootvars(dbPtr,1,"lLimitBytesm",tempRow[10]);
					lt_dbput_rootvars(dbPtr,1,"maxupp",tempRow[21]);

					lt_dbput_rootvars(dbPtr,1,"lLimitTimesm",tempRow[11]);
					lt_dbput_rootvars(dbPtr,1,"maxdownp",tempRow[22]);

					lt_dbput_rootvars(dbPtr,1,"maxpackage",tempRow[15]);
					lt_dbput_rootvars(dbPtr,1,"maxsp",tempRow[23]);

					lt_dbput_rootvars(dbPtr,1,"maxBytes",tempRow[16]);
					lt_dbput_rootvars(dbPtr,1,"maxnc",tempRow[24]);

					//部门
					char mygid[1001];
					memset(mygid,0,sizeof(mygid));
					sprintf(mygid,"%s%s%s%s",tempRow[25],tempRow[26],tempRow[27],tempRow[28]);
					lt_dbput_rootvars(dbPtr,1,"mygid",mygid);

					//策略
					lt_dbput_rootvars(dbPtr,1,"policyid",tempRow[29]);
					printf("tempRow[25]:%s\n",tempRow[29]);

					//附加属性
					lt_dbput_rootvars(dbPtr,1,"mymoreflag",tempRow[12]);
					char moreflag[33];
					int i;
				  memcpy(moreflag,tempRow[12],32);
				  printf("moreflag:%s\n",moreflag);
       	 	for(i=0;i<32;i++){
       	 		char tempsel[32];
       	 		sprintf(tempsel,"c%d",i);
       	 		if(moreflag[i]=='1'){
       	 			lt_dbput_rootvars(dbPtr,1,tempsel,"true");
       	 		}else{
       	 			lt_dbput_rootvars(dbPtr,1,tempsel,"false");
       	 		}
       	 	}


			}
		 ltDbCloseCursor(tempCursor);
		}


	 	char sel[32];
		tablePtr=lt_dbput_table(dbPtr,"qoslist");
		memset(sqlBuf,0,sizeof(sqlBuf));
		sprintf(sqlBuf,"select sid,qosname from msaqos where sid>0 order by sid asc");
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
					tempRow=ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
							memset(sel,0,sizeof(sel));
							if(atoi(tempRow[0])==myratelimit){
									sprintf(sel,"%s","selected");
							}
							lt_dbput_recordvars(tablePtr,3,
							 "qosid",LT_TYPE_STRING,tempRow[0],
								"sel",LT_TYPE_STRING,sel,
								"qosname",LT_TYPE_STRING,tempRow[1]);
						    tempRow=ltDbFetchRow(tempCursor);
					}
			ltDbCloseCursor(tempCursor);
		}

	 	ltDbClose(G_DbCon);


	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/policylistinfo.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   ltDbClose(G_DbCon);
   return 0;
}


//显示批量策略模板
int showPolicyApply(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
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
   lt_dbput_rootvars(dbPtr,1,"policyid","00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
   lt_dbput_rootvars(dbPtr,1,"topsrvnamelist","11111111111111111111111111111111111");
   lt_dbput_rootvars(dbPtr,1,"mymoreflag","00000000000000000000000000000000");
   lt_dbput_rootvars(dbPtr,1,"mygid","0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");

   lt_dbput_rootvars(dbPtr,1,"addflag","addflag:'add',");

   lt_dbput_rootvars(dbPtr,1,"c0","false");
   lt_dbput_rootvars(dbPtr,1,"c1","false");
   lt_dbput_rootvars(dbPtr,1,"c2","false");
   lt_dbput_rootvars(dbPtr,1,"c3","false");
   lt_dbput_rootvars(dbPtr,1,"c4","false");
   lt_dbput_rootvars(dbPtr,1,"c5","false");
   lt_dbput_rootvars(dbPtr,1,"c6","false");
   lt_dbput_rootvars(dbPtr,1,"c7","false");
   lt_dbput_rootvars(dbPtr,1,"c8","false");
   lt_dbput_rootvars(dbPtr,1,"c9","false");
   lt_dbput_rootvars(dbPtr,1,"c10","false");
   lt_dbput_rootvars(dbPtr,1,"c11","false");
   lt_dbput_rootvars(dbPtr,1,"c12","false");
   lt_dbput_rootvars(dbPtr,1,"c13","false");
   lt_dbput_rootvars(dbPtr,1,"c14","false");
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/policylistinfo.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}

//服务
int msaServiceToPageTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		 int i;
		 char tmpstr[1024];
		 char tempname[32];
		 stralloc strTemp;
		 char topSrvNameList[MAX_TOP_SRVNUM+1];
	   int iHead;
	   memset(tmpstr,0,sizeof(tmpstr));
	   iHead=1;

	   strTemp.s=0;

	   memset(topSrvNameList,0,MAX_TOP_SRVNUM+1);
	   memset(topSrvNameList,'1',MAX_TOP_SRVNUM);
	   if(ltMsgGetVar_s(ltMsgPk,"topsrvnamelist")){
	   	  sprintf(topSrvNameList,"%s",ltMsgGetVar_s(ltMsgPk,"topsrvnamelist"));
	 	 }

	  stralloc_cats(&strTemp,"[");

 	 	for(i=0;i<MAX_TOP_SRVNUM;i++){
			   	if(strcmp(_ltPubInfo->topSrvName[i].srvname,"")!=0){
			   		memset(tempname,0,sizeof(tempname));
			   		sprintf(tempname,"%s",_ltPubInfo->topSrvName[i].srvname);
			   		if(topSrvNameList[i]=='0'){
							if(iHead==1){
								 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked: false}",i,tempname);
								 iHead=0;
							}else{
								 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked: false}",i,tempname);
							}
						}else if(topSrvNameList[i]=='1'){
							if(iHead==1){
								 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
								 iHead=0;
							}else{
								 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
							}
						}
						stralloc_cats(&strTemp,tmpstr);
		  }
		 }

	   stralloc_cats(&strTemp,"]");
	   stralloc_0(&strTemp);
	    printf("servicetree:%s\n",strTemp.s);

	   ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	   ltMsgFree(ltMsgPk);

	   return 0;
}



int msaPolicyToPageTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	   char tmpstr[512];
		 char tempname[32];
	   int i;
	   stralloc strTemp;
	   int iHead;
	   char topPolicyNameList[_MAX_ALLPOLICY+1];
	   memset(tmpstr,0,sizeof(tmpstr));
	   iHead=1;
	   strTemp.s=0;

	   memset(topPolicyNameList,0,_MAX_ALLPOLICY+1);
	   memset(topPolicyNameList,'0',_MAX_ALLPOLICY);
	 	 if(ltMsgGetVar_s(ltMsgPk,"policyid")){
	 	 	  sprintf(topPolicyNameList,"%s",ltMsgGetVar_s(ltMsgPk,"policyid"));
	 	 }

	 	 stralloc_cats(&strTemp,"[");
	 		 for(i=0;i<_MAX_ALLPOLICY;i++){
			   	if(strcmp(_ltPubInfo->_PolicyList[i].pname,"")!=0){
			   		memset(tempname,0,sizeof(tempname));
			   		sprintf(tempname,"%s",_ltPubInfo->_PolicyList[i].pname);
			   		if(topPolicyNameList[i]=='0'){
							if(iHead==1){
								 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked: false}",i,tempname);
								 iHead=0;
							}else{
								 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked: false}",i,tempname);
							}
						}else if(topPolicyNameList[i]=='1'){
							if(iHead==1){
								 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
								 iHead=0;
							}else{
								 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
							}
						}
						stralloc_cats(&strTemp,tmpstr);
		   	 }
		   }
	   stralloc_cats(&strTemp,"]");
	   stralloc_0(&strTemp);


	   printf("policytree:%s\n",strTemp.s);

	   ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	   ltMsgFree(ltMsgPk);

	   return 0;

}

//部门主线树
int msaloopgroupToPagetree(stralloc *strTemp,int level,int pid,char gid[]){
	char tmpstr[512];
	int iHead;
	int j;
	iHead=1;
	level++;

	for(j=0;j<_MAX_ALLGRPNUM;j++){
			/*判断有没有子节点*/
		if(_ltPubInfo->_GroupList[j].level==level && _ltPubInfo->_GroupList[j].pid==pid && _ltPubInfo->_GroupList[j].name[0] ){
			if(iHead==1){
				sprintf(tmpstr,"%s",",children: [");
			  stralloc_cats(strTemp,tmpstr);

			  if(gid[j]=='1'){
					sprintf(tmpstr,"{id: '%u',qtip: '%s',text: '%s',expanded:true,checked:true",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
				}else{
				  sprintf(tmpstr,"{id: '%u',qtip: '%s',text: '%s',expanded:true,checked:false",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
			  }
				iHead=0;
			}else{
				if(gid[j]=='1'){
					 sprintf(tmpstr,",{id: '%u',qtip: '%s',text: '%s',expanded:true,checked:true",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
				}else{
			     sprintf(tmpstr,",{id: '%u',qtip: '%s',text: '%s',expanded:true,checked:false",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
		    }
		  }
			stralloc_cats(strTemp,tmpstr);
			pid=_ltPubInfo->_GroupList[j].groupid;

			if(level<5){
				msaloopgroupToPagetree(strTemp,level,pid,gid);
			}
			stralloc_cats(strTemp,"}");
	  }
  }
  if(iHead==1){
  	  sprintf(tmpstr,"%s",",leaf: true");
			stralloc_cats(strTemp,tmpstr);
			return 0;
  }
  stralloc_cats(strTemp,"]");
	return 0;

}


int msaGroupToPageTreeView(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char tmpstr[512];
	int level,pid;
	stralloc strTemp;
  int j;
	int iHead;
	memset(tmpstr,0,sizeof(tmpstr));

  char gid[1001];
  printf("test>>>>>>>>>>\n");
  printf("test:%s\n",ltMsgGetVar_s(ltMsgPk,"mygid"));
  if(ltMsgGetVar_s(ltMsgPk,"mygid")){
     sprintf(gid,"%s",ltMsgGetVar_s(ltMsgPk,"mygid"));
  }

  printf("gid:%s\n",gid);

	pid=0;
	level=0;
	iHead=1;
	strTemp.s=0;
	stralloc_cats(&strTemp,"[");

 	for(j=0;j<_MAX_ALLGRPNUM;j++){
			/*判断有没有子节点*/
		if(_ltPubInfo->_GroupList[j].level==0 && _ltPubInfo->_GroupList[j].pid==0 && _ltPubInfo->_GroupList[j].name[0] ){
			if(iHead==1){
				if(gid[j]=='1'){
					sprintf(tmpstr,"{id: '%u',qtip: '%s',text: '%s',expanded:true,checked:true",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
				}else{
				 sprintf(tmpstr,"{id: '%u',qtip: '%s',text: '%s',expanded:true,checked:false",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
				}
				iHead=0;
			}else{
				if(gid[j]=='1'){
				  sprintf(tmpstr,",{id: '%u',qtip: '%s',text: '%s',expanded:true,checked:true",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
				}else{
			   sprintf(tmpstr,",{id: '%u',qtip: '%s',text: '%s',expanded:true,checked:false",_ltPubInfo->_GroupList[j].groupid,_ltPubInfo->_GroupList[j].mydesc,_ltPubInfo->_GroupList[j].name);
		    }
		  }
		  stralloc_cats(&strTemp,tmpstr);
			pid=_ltPubInfo->_GroupList[j].groupid;
			if(level<5){
				msaloopgroupToPagetree(&strTemp,level,pid,gid);
			}
			stralloc_cats(&strTemp,"}");
		}
	}
	stralloc_cats(&strTemp,"]");
  stralloc_0(&strTemp);
printf("strTemp.s:%s\n",strTemp.s);
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);
	return 0;
}





int msaPiLiangAppPolicy(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char sqlStr[1024];
	 int allcount;
	 ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;

  //数据库连接
	 char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
			ltMsgFree(ltMsgPk);
			return 0;
	 }
	 //ltDbClose(G_DbCon);


	 allcount=0;
	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"%s","select count(*) from msauserpolicy");
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow=ltDbFetchRow(tempCursor);
       	if(tempRow!=NULL){
       		allcount=atol(tempRow[0]);
       		if(allcount>_MAX_USERPOLICY_NUM){
       			allcount=_MAX_USERPOLICY_NUM;
       		}
       	}
       	ltDbCloseCursor(tempCursor);
   }

	 memset(sqlStr,0,sizeof(sqlStr));
	 sprintf(sqlStr,"%s","select sid,policyname,prio,ratelimit,ratetype,topsrvnamelist,llimitbytes,llimittimes,llimitbytesw,llimittimesw,llimitbytesm,llimittimesm,moreflag,jftype,accountnum,maxpackage,maxbytes,maxudp,maxtcp,maxudpp,maxtcpp,maxupp,maxdownp,maxsp,maxnc,group0,group1,group2,group3,policyid from msauserpolicy order by prio DESC");
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow=ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
       		  int i;
            for(i=0;i<allcount;i++){
							 	 _ltPubInfo->_userPolicylist[i].useflag=0;
							 	 _ltPubInfo->_userPolicylist[i].sid=atoi(tempRow[0]);
							 	 sprintf(_ltPubInfo->_userPolicylist[i].policyname,"%s",tempRow[1]);
							 	 _ltPubInfo->_userPolicylist[i].prio=atof(tempRow[2]);
							 	 _ltPubInfo->_userPolicylist[i].ratelimit=atoi(tempRow[3]);
							 	 _ltPubInfo->_userPolicylist[i].ratetype=atoi(tempRow[4]);
                 sprintf(_ltPubInfo->_userPolicylist[i].topSrvNameList,"%s",tempRow[5]);
                 _ltPubInfo->_userPolicylist[i].lLimitBytes=atoll(tempRow[6]);
                 _ltPubInfo->_userPolicylist[i].lLimitTimes=atoi(tempRow[7]);
                 _ltPubInfo->_userPolicylist[i].lLimitBytesw=atoll(tempRow[8]);
                 _ltPubInfo->_userPolicylist[i].lLimitTimesw=atoi(tempRow[9]);
                 _ltPubInfo->_userPolicylist[i].lLimitBytesm=atoll(tempRow[10]);
                 _ltPubInfo->_userPolicylist[i].lLimitTimesm=atoi(tempRow[11]);
                 sprintf(_ltPubInfo->_userPolicylist[i].moreflag,"%s",tempRow[12]);
                 _ltPubInfo->_userPolicylist[i].jftype=atoi(tempRow[13]);
                 _ltPubInfo->_userPolicylist[i].accountnum=atof(tempRow[14]);
						     _ltPubInfo->_userPolicylist[i].maxpackage=atoi(tempRow[15]);
						     _ltPubInfo->_userPolicylist[i].maxBytes=atoll(tempRow[16]);
                 _ltPubInfo->_userPolicylist[i].maxudp=atoi(tempRow[17]);
                 _ltPubInfo->_userPolicylist[i].maxtcp=atoi(tempRow[18]);
                 _ltPubInfo->_userPolicylist[i].maxudpp=atoi(tempRow[19]);
                 _ltPubInfo->_userPolicylist[i].maxtcpp=atoi(tempRow[20]);
                 _ltPubInfo->_userPolicylist[i].maxupp=atoi(tempRow[21]);
                 _ltPubInfo->_userPolicylist[i].maxdownp=atoi(tempRow[22]);
                 _ltPubInfo->_userPolicylist[i].maxsp=atoi(tempRow[23]);
                 _ltPubInfo->_userPolicylist[i].maxnc=atoi(tempRow[24]);
                 	//部门
									char mygid[1001];
									memset(mygid,0,sizeof(mygid));
									sprintf(mygid,"%s%s%s%s",tempRow[25],tempRow[26],tempRow[27],tempRow[28]);
                  sprintf(_ltPubInfo->_userPolicylist[i].grouplist,"%s",mygid);

						      sprintf(_ltPubInfo->_userPolicylist[i].pid,"%s",tempRow[29]);
						      //printf("apply load data success..............\n");
						 }

       		tempRow=ltDbFetchRow(tempCursor);
       	}
       	ltDbCloseCursor(tempCursor);
   }
   ltDbClose(G_DbCon);
   
   //应用在线的人员
   //int  nasSetMoreFlag(nasOnlineUserInfo  *psUserInfo,nasDbUserInfo *tmpDbUserInfoList);
   //nasSetMoreFlag(&psUserInfo,tmpDbUserInfoList);
   //如果条件超了就强制离线
   //
   
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html;charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");
   ltMsgFree(ltMsgPk);
   ltDbClose(G_DbCon);
	 return 0;
}
