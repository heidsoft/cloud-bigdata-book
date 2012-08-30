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
int showServiceSimp(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
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
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/serviceAddSimp.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//添加服务策略页面
int showRuleSrvSimp(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
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
	 lt_dbput_rootvars(dbPtr,1,"ruleid",ltMsgGetVar_s(ltMsgPk,"sid"));
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
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/ruleSrvAddSimp.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//显示规则修改
int showEditServiceSimp(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
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
	 ltDbClose(G_DbCon);
	 ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/SrvRulelistinfoSimp.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
//报警参数配置
int msaalertLink(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  		char mailalert[24],smsalert[24];
  		char diskalert[24],memalert[24],mail[64],telphone[64];
  		char newadminalert[24],newrulealert[24],delrulealert[24];
  		char outflowalert[24],p2pflowalert[24];
      char *pFile="/etc/msa/msa/alert";
    
     	ltDbHeadPtr dbPtr;
     	dbPtr=lt_dbinit();
	    lt_db_htmlpage(dbPtr,"utf-8");
	    
	    //报警方式
	    memset(mailalert,0,sizeof(mailalert));
		  bcCnfGetValue_s(pFile,"mailalert",mailalert); 
		  if(strcmp(mailalert,"yes")==0){
		   	lt_dbput_rootvars(dbPtr,1,"mailalert","checked"); 
		  }else{
		   	lt_dbput_rootvars(dbPtr,1,"mailalert","");
		  }
	    memset(mail,0,sizeof(mail));
		  bcCnfGetValue_s(pFile,"mail",mail);
		  lt_dbput_rootvars(dbPtr,1,"mail",mail);
	    memset(smsalert,0,sizeof(smsalert));
		  bcCnfGetValue_s(pFile,"smsalert",smsalert);
		  if(strcmp(smsalert,"yes")==0){
		   	lt_dbput_rootvars(dbPtr,1,"smsalert","checked"); 
		  }else{
		   	lt_dbput_rootvars(dbPtr,1,"smsalert","");
		  }
	    memset(telphone,0,sizeof(telphone));
		  bcCnfGetValue_s(pFile,"telphone",telphone);
		  lt_dbput_rootvars(dbPtr,1,"telphone",telphone);
	    //系统异常报警
	    memset(diskalert,0,sizeof(diskalert));
		  bcCnfGetValue_s(pFile,"diskalert",diskalert);
		  lt_dbput_rootvars(dbPtr,1,"diskalert",diskalert);
	    memset(memalert,0,sizeof(memalert));
		  bcCnfGetValue_s(pFile,"memalert",memalert);
		  lt_dbput_rootvars(dbPtr,1,"memalert",memalert); 
	    //关键操作报警
	    memset(newadminalert,0,sizeof(newadminalert));
		  bcCnfGetValue_s(pFile,"newadminalert",newadminalert);
		  if(strcmp(newadminalert,"yes")==0){
		   	lt_dbput_rootvars(dbPtr,1,"newadminalert","checked"); 
		  }else{
		   	lt_dbput_rootvars(dbPtr,1,"newadminalert","");
		  }
	    memset(newrulealert,0,sizeof(newrulealert));
		  bcCnfGetValue_s(pFile,"newrulealert",newrulealert);
		  if(strcmp(newrulealert,"yes")==0){
		   	lt_dbput_rootvars(dbPtr,1,"newrulealert","checked"); 
		  }else{
		   	lt_dbput_rootvars(dbPtr,1,"newrulealert","");
		  }
	    memset(delrulealert,0,sizeof(delrulealert));
		  bcCnfGetValue_s(pFile,"delrulealert",delrulealert);
		  if(strcmp(delrulealert,"yes")==0){
		   	lt_dbput_rootvars(dbPtr,1,"delrulealert","checked"); 
		  }else{
		   	lt_dbput_rootvars(dbPtr,1,"delrulealert","");
		  }
	    //网络异常报警
	    memset(outflowalert,0,sizeof(outflowalert));
		  bcCnfGetValue_s(pFile,"outflowalert",outflowalert);
		  lt_dbput_rootvars(dbPtr,1,"outflowalert",outflowalert);
	    memset(p2pflowalert,0,sizeof(p2pflowalert));
		  bcCnfGetValue_s(pFile,"p2pflowalert",p2pflowalert);
		  lt_dbput_rootvars(dbPtr,1,"p2pflowalert",p2pflowalert);  
	    
			
	    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/rule/sysalertset.htm",dbPtr->head,0);
			ltMsgFree(ltMsgPk);
			lt_dbfree(dbPtr);
			return 0;
}
int msaalertset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char *pFile="/etc/msa/msa/alert";
 	 char caCmdCon[128];
 	 
 	 char *mailalert,*smsalert;
   char *diskalert,*memalert,*mail,*telphone;
   char *newadminalert,*newrulealert,*delrulealert;
   char *outflowalert,*p2pflowalert;
	

	 //报警方式  邮件check   短信check  
	 //关闭Ping
	 if(ltMsgGetVar_s(ltMsgPk,"mailalert")){
	 	 mailalert=ltMsgGetVar_s(ltMsgPk,"mailalert");
	 }else{
	 	 mailalert="no";
	 }
   memset(caCmdCon,0,sizeof(caCmdCon));
   sprintf(caCmdCon,"echo 'mailalert=%s' > %s",mailalert,pFile);
   system(caCmdCon);
   
	 if(ltMsgGetVar_s(ltMsgPk,"smsalert")){
	 	 smsalert=ltMsgGetVar_s(ltMsgPk,"smsalert");
	 }else{
	   smsalert="no";	
	 }
   memset(caCmdCon,0,sizeof(caCmdCon));
   sprintf(caCmdCon,"echo 'smsalert=%s' >> %s",smsalert,pFile);
   system(caCmdCon);
	 //系统异常报警
	 if(ltMsgGetVar_s(ltMsgPk,"diskalert")){
	 	 diskalert=ltMsgGetVar_s(ltMsgPk,"diskalert");
	 }else{
	   diskalert="";	
	 }
   memset(caCmdCon,0,sizeof(caCmdCon));
   sprintf(caCmdCon,"echo 'diskalert=%s' >> %s",diskalert,pFile);
   system(caCmdCon);
   
	 if(ltMsgGetVar_s(ltMsgPk,"mail")){
	 	 mail=ltMsgGetVar_s(ltMsgPk,"mail");
	 }else{
	   mail="";
	 }
   memset(caCmdCon,0,sizeof(caCmdCon));
   sprintf(caCmdCon,"echo 'mail=%s' >> %s",mail,pFile);
   system(caCmdCon);
	 
	 if(ltMsgGetVar_s(ltMsgPk,"memalert")){
	 	 memalert=ltMsgGetVar_s(ltMsgPk,"memalert");
	 }else{
	   memalert="no";	
	 }
   memset(caCmdCon,0,sizeof(caCmdCon));
   sprintf(caCmdCon,"echo 'memalert=%s' >> %s",memalert,pFile);
   system(caCmdCon);
   
	 if(ltMsgGetVar_s(ltMsgPk,"telphone")){
	 	 telphone=ltMsgGetVar_s(ltMsgPk,"telphone");
	 }else{
	   telphone="";
	 }
   memset(caCmdCon,0,sizeof(caCmdCon));
   sprintf(caCmdCon,"echo 'telphone=%s' >> %s",telphone,pFile);
   system(caCmdCon);
   
	 //关键操作报警
	 if(ltMsgGetVar_s(ltMsgPk,"newadminalert")){
	 	 newadminalert=ltMsgGetVar_s(ltMsgPk,"newadminalert");
	 }else{
	   newadminalert="no";	
	 }
   memset(caCmdCon,0,sizeof(caCmdCon));
   sprintf(caCmdCon,"echo 'newadminalert=%s' >> %s",newadminalert,pFile);
   system(caCmdCon);
   
	 if(ltMsgGetVar_s(ltMsgPk,"newrulealert")){
	 	 newrulealert=ltMsgGetVar_s(ltMsgPk,"newrulealert");
	 }else{
	   newrulealert="no";	
	 }
   memset(caCmdCon,0,sizeof(caCmdCon));
   sprintf(caCmdCon,"echo 'newrulealert=%s' >> %s",newrulealert,pFile);
   system(caCmdCon);
   
	 if(ltMsgGetVar_s(ltMsgPk,"delrulealert")){
	 	 delrulealert=ltMsgGetVar_s(ltMsgPk,"delrulealert");
	 }else{
	   delrulealert="no";	
	 }
   memset(caCmdCon,0,sizeof(caCmdCon));
   sprintf(caCmdCon,"echo 'delrulealert=%s' >> %s",delrulealert,pFile);
   system(caCmdCon);
	 //网络异常报警
	 if(ltMsgGetVar_s(ltMsgPk,"outflowalert")){
	 	 outflowalert=ltMsgGetVar_s(ltMsgPk,"outflowalert");
	 }else{
	   outflowalert="";
	 }
   memset(caCmdCon,0,sizeof(caCmdCon));
   sprintf(caCmdCon,"echo 'outflowalert=%s' >> %s",outflowalert,pFile);
   system(caCmdCon);

	 
	 if(ltMsgGetVar_s(ltMsgPk,"p2pflowalert")){
	 	 p2pflowalert=ltMsgGetVar_s(ltMsgPk,"p2pflowalert");
	 }else{
	   p2pflowalert="";
	 }
   memset(caCmdCon,0,sizeof(caCmdCon));
   sprintf(caCmdCon,"echo 'p2pflowalert=%s' >> %s",p2pflowalert,pFile);
   system(caCmdCon);

	 ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=uft-8\n",
	 "lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >系统安全设置成功!</body></html>");
	 ltMsgFree(ltMsgPk);
   
   return 0;
}

////////////////////自定义服务
/*加载服务分类页面*/
int lturlsort(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 ltDbHeadPtr dbPtr;
	 dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
  
   ltTablePtr  tablePtr;
   tablePtr=lt_dbput_table(dbPtr,"srvlist");	
   int iii,alltopsrvnum=0; 
   long lCount;
   char  myalltopsrvnum[16];
   char  mylCount[16];
   char  myallurl[100];
   long  allurl=10000000;
	 long  lTime = nasCvtLongTime("2011-10-01","00:00:00");
	 long  lTime1=time(0)-lTime;
   char  myallsrvrule[16];
	 char strBuf[1024];
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
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"");
		 	 ltMsgFree(ltMsgPk);			
			 return 0;
	 }
   lCount=0; 
   sprintf(strBuf,"select count(*) from nassrv"); 
   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
   if(tempCursor!=NULL){
	  tempRow= ltDbFetchRow(tempCursor);
	  if(tempRow!=NULL){
		  lCount=atol(tempRow[0]);
	  }
   }
   ltDbCloseCursor(tempCursor);
   
	for(iii=0;iii<MAX_TOP_SRVNUM;iii++){
		if(strcmp(_ltPubInfo->topSrvName[iii].srvname,"")!=0){
 			alltopsrvnum++;
			lt_dbput_recordvars(tablePtr,2,"topsrvid",LT_TYPE_LONG,iii,"topsrvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[iii].srvname);
		}
	} 
	sprintf(myalltopsrvnum,"%d",alltopsrvnum);//组
	lt_dbput_rootvars(dbPtr,1,"alltopsrvnum",myalltopsrvnum);
	
	sprintf(mylCount,"%ld",lCount);//总服务
	lt_dbput_rootvars(dbPtr,1,"allsrvnum",mylCount);
	
	sprintf(myallsrvrule,"%ld",lCount*2+27);//特征
	lt_dbput_rootvars(dbPtr,1,"allsrvrule",myallsrvrule);
	
	
	sprintf(myallurl,"%ld",allurl+(lTime1)/3600/24*100);
	lt_dbput_rootvars(dbPtr,1,"allurl",myallurl);
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/nasurlsort.htm",dbPtr->head,0);        
	ltMsgFree(ltMsgPk); 
	lt_dbfree(dbPtr);
	return 0;
}
/*服务分类查询*/
int lturlsortlist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char strBuf[1024];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char caTmpp[512];
    long lRowNum,lStartRec,lSumRec,lCount,lNum;
    int k;
    char *topsrvid;
    stralloc strTemp;
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
    topsrvid=ltMsgGetVar_s(ltMsgPk,"topsrvid");//一级服务
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
    if(strcmp(topsrvid,"-1")==0){
    	sprintf(strBuf,"select count(*) from nassrv");
    }else{
    	sprintf(strBuf,"select count(*) from nassrv where topsrvid=%s",topsrvid);
    }

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
   if(strcmp(topsrvid,"-1")==0){
   		sprintf(strBuf,"select srvid,topsrvid,denytype,srvname,srvdesc,level from nassrv order by srvid asc limit %lu offset %lu ",lRowNum,lStartRec);
   }else{
   		sprintf(strBuf,"select srvid,topsrvid,denytype,srvname,srvdesc,level from nassrv where topsrvid=%s order by srvid asc limit %lu offset %lu ",topsrvid,lRowNum,lStartRec);
   }
   printf("%s\n",strBuf);
   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
   if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
		  while(tempRow!=NULL){
		  	 if(k==0){
	     	  	sprintf(caTmpp,"{\"srvid\":\"%s\",\"topsrvname\":\"%s\",\"srvname\":\"%s\",\"srvdesc\":\"%s\",\"level\":\"%s\",\"denytype\":\"%s\"}",
	     	  	 tempRow[0],_ltPubInfo->topSrvName[atoi(tempRow[1])].srvname,tempRow[3],tempRow[4],tempRow[5],tempRow[2]);
	     	 		stralloc_cats(&strTemp,caTmpp);
	     	 }else{
	     	  	sprintf(caTmpp,",{\"srvid\":\"%s\",\"topsrvname\":\"%s\",\"srvname\":\"%s\",\"srvdesc\":\"%s\",\"level\":\"%s\",\"denytype\":\"%s\"}",
	     	  	 tempRow[0],_ltPubInfo->topSrvName[atoi(tempRow[1])].srvname,tempRow[3],tempRow[4],tempRow[5],tempRow[2]);
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
/*加载服务修改页面*/
int ltupdateurlsort(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char *my_id;
	 char sqlBuf[2048];
   ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   ltDbCursor *tempCursor;
   LT_DBROW   tempRow;
   
   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL)
	 {
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"db error!");
			ltMsgFree(ltMsgPk);			
			return 0;
	 }
   my_id=ltMsgGetVar_s(ltMsgPk,"my_id");
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   if(my_id!=NULL){
		sprintf(sqlBuf,"select srvid,topsrvid,denytype,srvname,srvdesc,level from nassrv where srvid='%s'",my_id);
printf("sqlBuf:%s\n",sqlBuf);
			 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			 if(tempCursor!=NULL){
					 tempRow= ltDbFetchRow(tempCursor);
					 if(tempRow!=NULL){
						 char str[16];
						 
						 memset(str,0,sizeof(str));
						 sprintf(str,"denytype%ssel",tempRow[2]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");//服务类型
 						 
						 lt_dbput_rootvars(dbPtr,1,"srvname",tempRow[3]);
						 lt_dbput_rootvars(dbPtr,1,"srvdesc",tempRow[4]);
						 
						 memset(str,0,sizeof(str));
						 sprintf(str,"level%ssel",tempRow[5]);
 						 lt_dbput_rootvars(dbPtr,1,str,"selected");//危害级别
 						 
						 lt_dbput_rootvars(dbPtr,1,"rid",tempRow[0]);
					 }
				 ltDbCloseCursor(tempCursor);
			 }
   }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/updateSrvPage.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   ltDbClose(G_DbCon); 
   return 0;
}

/*加载服务特征修改页面*/
int ltupdateurlType(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char *my_id;
	 char pFile[512];
   stralloc strTemp;
   strTemp.s=0;
	 char strFile[256];
	 
   ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   
   my_id=ltMsgGetVar_s(ltMsgPk,"my_id");
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   if(my_id!=NULL){
   	   sprintf(pFile,"/etc/msa/msa/srvlist/srvdefine/%s",my_id);
   		 
	     int   match;
			 static char inbuf[4096];
			 int   fd;
			 fd=open(pFile,O_RDONLY | O_NDELAY);
		   match=read(fd,inbuf,sizeof(inbuf));
			 int tt=access(pFile,0);//配置文件是否存在  不存在的话在页面上打印空
			 if(tt!=-1){
				 while(match){
				 	stralloc_catb(&strTemp,inbuf,match);
				 	match=read(fd,inbuf,sizeof(inbuf));
				 }
			   stralloc_0(&strTemp); 
	     }else{
	     	lt_dbput_rootvars(dbPtr,1,"srvid",my_id);
		    ltMsgPk->msgpktype=1;
		    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/updateSrvType.htm",dbPtr->head,0);
		    ltMsgFree(ltMsgPk);
		    ltDbClose(G_DbCon); 
		    return 0;
	     }
			 sprintf(strFile,"%s",strTemp.s);
			 char str[256];
			 char str1[256];		 
			 memset(str,0,sizeof(str));
			 memset(str1,0,sizeof(str1));
			 str[0]=strFile[0];
			 sprintf(str1,"matchAgree%ssel",str);
			 lt_dbput_rootvars(dbPtr,1,str1,"selected");//匹配协议

			 
			 memset(str,0,sizeof(str));
			 memset(str1,0,sizeof(str1));
			 str[0]=strFile[1];
			 sprintf(str1,"matchdirect%ssel",str);
			 lt_dbput_rootvars(dbPtr,1,str1,"selected");//匹配方向

			 
			 memset(str,0,sizeof(str));
			 memset(str1,0,sizeof(str1));
			 str[0]=strFile[2];
			 str[1]=strFile[3];
			 str[2]=strFile[4];
			 str[3]=strFile[5];
			 str[4]=strFile[6];
			 lt_dbput_rootvars(dbPtr,1,"matchportL",str);//端口L

			 
			 memset(str,0,sizeof(str));
			 memset(str1,0,sizeof(str1));
			 str[0]=strFile[7];
			 str[1]=strFile[8];
			 str[2]=strFile[9];
			 str[3]=strFile[10];
			 str[4]=strFile[11];
			 lt_dbput_rootvars(dbPtr,1,"matchportT",str);//端口T

			 
			 memset(str,0,sizeof(str));
			 memset(str1,0,sizeof(str1));
			 str[0]=strFile[12];
			 sprintf(str1,"matchload%ssel",str);
			 lt_dbput_rootvars(dbPtr,1,str1,"selected");//匹配方式

			 
			 memset(str,0,sizeof(str));
			 memset(str1,0,sizeof(str1));
			 str[0]=strFile[13];
			 str[1]=strFile[14];
			 str[2]=strFile[15];
			 str[3]=strFile[16];
			 lt_dbput_rootvars(dbPtr,1,"srvdescmatch1",str);//起始字节

			 
			 memset(str,0,sizeof(str));
			 memset(str1,0,sizeof(str1));
			 str[0]=strFile[17];
			 str[1]=strFile[18];
			 lt_dbput_rootvars(dbPtr,1,"srvdescmatch2",str);//字节总数

			 int strString=2*atoi(str);		 
			 memset(str,0,sizeof(str));
			 memset(str1,0,sizeof(str1));
		   int iii;int jjj=0;
		 	 for(iii=19;iii<=strString+18;iii++){
				 str[jjj]=strFile[iii];
				 jjj++;
			 }
printf("str:::%s\n",str);
			 lt_dbput_rootvars(dbPtr,1,"srvdescmatch3",str);//字节总数
			 
	     lt_dbput_rootvars(dbPtr,1,"srvid",my_id);
   }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/updateSrvType.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   ltDbClose(G_DbCon); 
   return 0;
}
/*修改服务和特征*/
int ltsaveurlsort(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	   char sqlBuf[1024];
		 char *casrvid;
		 
 		 char *camatchDirect;
 		 char *camatchAgree;
 		 char *camatchload;
 		 char *camatchportL;
 		 char *camatchportT;
 		 char *cadescmatch1;
 		 char *cadescmatch2;
 		 char *cadescmatch3;
 		 
 		 char *caflag;
	   
	   char *casrvname;
	   char *casrvdesc;
 		 char *cadenytype;
	   char *calevel;
	   
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
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"");
				ltMsgFree(ltMsgPk);			
				return 0;
		 }
    casrvid=ltMsgGetVar_s(ltMsgPk,"srvid");
    
		camatchDirect=ltMsgGetVar_s(ltMsgPk,"matchDirect");
		camatchAgree=ltMsgGetVar_s(ltMsgPk,"matchAgree");
		camatchportL=ltMsgGetVar_s(ltMsgPk,"matchportL");
		camatchportT=ltMsgGetVar_s(ltMsgPk,"matchportT");
		camatchload=ltMsgGetVar_s(ltMsgPk,"matchload");
		
		cadescmatch1=ltMsgGetVar_s(ltMsgPk,"srvdescmatch1");
		cadescmatch2=ltMsgGetVar_s(ltMsgPk,"srvdescmatch2");
		cadescmatch3=ltMsgGetVar_s(ltMsgPk,"srvdescmatch3");
		
		casrvname=ltMsgGetVar_s(ltMsgPk,"srvname");
		casrvdesc=ltMsgGetVar_s(ltMsgPk,"srvdesc");
		cadenytype=ltMsgGetVar_s(ltMsgPk,"denytype");
		calevel=ltMsgGetVar_s(ltMsgPk,"level");
		
		caflag=ltMsgGetVar_s(ltMsgPk,"flag");

printf("123casrvid:%s\n",casrvid);		
    //根据传的参数判断
    if(atoi(caflag)==0){
		  //sprintf(sqlBuf,"update nassrv set matchDirect='%s',matchAgree='%s',matchportL='%s',matchportT='%s',matchload='%s',srvdescmatch1='%s',srvdescmatch2='%s',srvdescmatch3='%s' where srvid='%s'",camatchDirect,camatchAgree,camatchportL,camatchportT,camatchload,cadescmatch1,cadescmatch2,cadescmatch3,casrvid);	  
		  //ltDbExecSql(G_DbCon,sqlBuf);
	  	
		  char pFile[512];
		  if(casrvid==NULL){
		   	sprintf(pFile,"/etc/msa/msa/srvlist/srvdefine/-1");
		 	}else{
		   	sprintf(pFile,"/etc/msa/msa/srvlist/srvdefine/%s",casrvid);
		  }  
		  char    caCmdCon[2048];
	    sprintf(caCmdCon,"echo '%s%s%s%s%s%s%s%s' > %s",camatchAgree,camatchDirect,camatchportL,camatchportT,camatchload,cadescmatch1,cadescmatch2,cadescmatch3,pFile);
	    system(caCmdCon);
	    
		  snprintf(caMsg,255,"%s","update nassrv srvType success.");
			logWrite(ltMsgPk,caMsg,3);
		}else if(atoi(caflag)==1){
		  sprintf(sqlBuf,"update nassrv set denytype='%s',srvname='%s',srvdesc='%s',level='%s' where srvid='%s'",cadenytype,casrvname,casrvdesc,calevel,casrvid);	  
		  ltDbExecSql(G_DbCon,sqlBuf); 
		  snprintf(caMsg,255,"%s","update nassrv success.");
			logWrite(ltMsgPk,caMsg,3);
		}
printf("sqlBuf:%s\n",sqlBuf);	 
	  ltMsgPk->msgpktype=1;
    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
    ltMsgFree(ltMsgPk);
    return 0;
		

}

/*加载IP修改页面*/
int msaUpdateIP(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char *my_id;
   stralloc strTemp;
   strTemp.s=0;
   
   ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   
   my_id=ltMsgGetVar_s(ltMsgPk,"my_id");
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   char pFile[512];
   if(my_id==NULL){
   		sprintf(pFile,"/etc/msa/msa/srvlist/diplist/1");
 	 }else{
   		sprintf(pFile,"/etc/msa/msa/srvlist/diplist/%s",my_id);
   }
   if(my_id!=NULL){
     int   match;
		 static char inbuf[4096];
		 int   fd;
		 fd=open(pFile,O_RDONLY | O_NDELAY);
	   match=read(fd,inbuf,sizeof(inbuf));
		 int tt=access(pFile,0);//配置文件是否存在  不存在的话在页面上打印空
		 if(tt!=-1){
			 while(match){
			 	stralloc_catb(&strTemp,inbuf,match);
			 	match=read(fd,inbuf,sizeof(inbuf));
			 }
		   stralloc_0(&strTemp); 
	     lt_dbput_rootvars(dbPtr,1,"srvip",strTemp.s);
     }else{
     	lt_dbput_rootvars(dbPtr,1,"srvip","");
     }  
     lt_dbput_rootvars(dbPtr,1,"srvid",my_id);
   }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/updateSrvIP.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   ltDbClose(G_DbCon); 
   return 0;
}

/*加载域名修改页面*/
int msaUpdateArea(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char *my_id;
   stralloc strTemp;
   strTemp.s=0;
   
   ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   
   my_id=ltMsgGetVar_s(ltMsgPk,"my_id");
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   char pFile[512];
   if(my_id==NULL){
   		sprintf(pFile,"/etc/msa/msa/srvlist/urllist/1");
 	 }else{
   		sprintf(pFile,"/etc/msa/msa/srvlist/urllist/%s",my_id);
   }
   if(my_id!=NULL){
     int   match;
		 static char inbuf[4096];
		 int   fd;
		 
		 fd=open(pFile,O_RDONLY | O_NDELAY);
	   match=read(fd,inbuf,sizeof(inbuf));
		 int tt=access(pFile,0);//配置文件是否存在  不存在的话在页面上打印空
		 if(tt!=-1){
			 while(match){
			 	stralloc_catb(&strTemp,inbuf,match);
			 	match=read(fd,inbuf,sizeof(inbuf));
			 }
		   stralloc_0(&strTemp); 
	     lt_dbput_rootvars(dbPtr,1,"srvarea",strTemp.s);
     }else{
     	lt_dbput_rootvars(dbPtr,1,"srvarea","");
     }  
     lt_dbput_rootvars(dbPtr,1,"srvid",my_id);
   }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/updateSrvArea.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   ltDbClose(G_DbCon); 
   return 0;
}

/*修改IP 域名*/
int msaUpdateType(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	   char *casrvid;
		 char *cadenytype;
		 char *casrvip;
		 char *casrvarea;
		 char *caflag;
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
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"");
				ltMsgFree(ltMsgPk);			
				return 0;
		 }
		casrvid=ltMsgGetVar_s(ltMsgPk,"srvid");//my_id
    cadenytype=ltMsgGetVar_s(ltMsgPk,"denytype");
    casrvip=ltMsgGetVar_s(ltMsgPk,"srvip");
    casrvarea=ltMsgGetVar_s(ltMsgPk,"srvarea");
    caflag=ltMsgGetVar_s(ltMsgPk,"flag");
//printf("casrvid:%s\n",casrvid);
		if(atoi(caflag)==1){
			//修改IP
			char pFile[512];
			if(casrvid!=NULL){
		   		sprintf(pFile,"/etc/msa/msa/srvlist/diplist/%s",casrvid);
		 	}else{
		   		sprintf(pFile,"/etc/msa/msa/srvlist/diplist/1");
		  }
			printf("pFile:%s\n",pFile);
	    char    *srvip;
		  char    caCmdCon[2048];
		  srvip=ltMsgGetVar_s(ltMsgPk,"srvip");

	    sprintf(caCmdCon,"echo '%s' > %s",srvip,pFile);
	    system(caCmdCon);
	    
		  snprintf(caMsg,255,"%s","update nassrv IP success.");
		  logWrite(ltMsgPk,caMsg,3);
		  ltMsgPk->msgpktype=1;
	    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
	    ltMsgFree(ltMsgPk);
	  	return 0;
  	}else if(atoi(caflag)==2){
			//修改域名
			char pFile[512];
			if(casrvid!=NULL){
		   		sprintf(pFile,"/etc/msa/msa/srvlist/urllist/%s",casrvid);
		 	}else{
		   		sprintf(pFile,"/etc/msa/msa/srvlist/urllist/1");
		  }
	    char    *srvarea;
		  char    caCmdCon[2048];
		  srvarea=ltMsgGetVar_s(ltMsgPk,"srvarea");
		  
	    sprintf(caCmdCon,"echo '%s' > %s",srvarea,pFile);
	    system(caCmdCon); 
	    
		  snprintf(caMsg,255,"%s","update nassrv domain success.");
		  logWrite(ltMsgPk,caMsg,3);
		  ltMsgPk->msgpktype=1;
	    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
	    ltMsgFree(ltMsgPk);
	  	return 0;
  	}
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"1");
  ltMsgFree(ltMsgPk);
	return 0;
}


/*初始化服务特征*/
int ltreseturlsort(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  char *casrvid;
 		char *caflag;
	  
	  casrvid=ltMsgGetVar_s(ltMsgPk,"srvid");
		caflag=ltMsgGetVar_s(ltMsgPk,"flag");

    if(atoi(caflag)==1){
		  char pFile[512];
		  char caCmdCon[2048];
		  
		  //初始服务特征
		  memset(pFile,0,sizeof(pFile));
		  sprintf(pFile,"/etc/msa/msa/srvlist/srvdefine/%s",casrvid);
		   
		  memset(caCmdCon,0,sizeof(caCmdCon)); 
			sprintf(caCmdCon,"/bin/rm -rf %s",pFile);
		  system(caCmdCon);
		  
		  //初始IP列表
		  memset(pFile,0,sizeof(pFile));
		  sprintf(pFile,"/etc/msa/msa/srvlist/diplist/%s",casrvid);
		   
		  memset(caCmdCon,0,sizeof(caCmdCon)); 
			sprintf(caCmdCon,"/bin/rm -rf %s",pFile);
		  system(caCmdCon);
		  
		  //初始域名列表
		  memset(pFile,0,sizeof(pFile));
		  sprintf(pFile,"/etc/msa/msa/srvlist/urllist/%s",casrvid);
		   
		  memset(caCmdCon,0,sizeof(caCmdCon)); 
			sprintf(caCmdCon,"/bin/rm -rf %s",pFile);
		  system(caCmdCon);
	    
		  ltMsgPk->msgpktype=1;
	    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
	    ltMsgFree(ltMsgPk);
	    return 0;
		}else{
		  ltMsgPk->msgpktype=1;
	    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");
	    ltMsgFree(ltMsgPk);
	    return 0;
		} 
}
