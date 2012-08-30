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
#include "msa.h"
#define _sortreportplt  "/reportplt/sortreportplt"
#define JAVACMD "/app/java/jdk1.6.0_01/bin/java -Duser.language=zh_CN.UTF-8 -Dfile.encoding=UTF-8 -cp /app/java/poi/poi-3.7-20101029.jar:./ "
#define BINCMD	"/app/msa/bin"

/*排序分析页面链接调用函数*/
int msasortreportlink(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	  ltDbHeadPtr dbPtr;
		ltTablePtr  tablePtr;
		int j;
		j=0;
	  dbPtr=lt_dbinit();
		lt_db_htmlpage(dbPtr,"utf-8");


	  LT_DBROW 		tempRow;
 	  ltDbCursor *tempCursor;

		char *dbUser;
		char *dbPass;
		char *dbName;
		dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
			fprintf(stderr,"db connect error\n");
			return 0;
		}

	  char sqlBuf[512];
	  memset(sqlBuf,0,sizeof(sqlBuf));
		//日报告
		sprintf(sqlBuf,"select tabdesc,tabname from msareportlist  where tabtype=1 and reptype=1 order by sdate desc ");
		//printf("sql:%s\n",sqlBuf);
		tablePtr=lt_dbput_table(dbPtr,"daylist");
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
		  tempRow=ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
					lt_dbput_recordvars(tablePtr,2,"daytime",LT_TYPE_STRING,tempRow[0],"day",LT_TYPE_STRING,tempRow[1]);
		  		tempRow=ltDbFetchRow(tempCursor);
			}
		}
		ltDbCloseCursor(tempCursor);

		sprintf(sqlBuf,"select tabdesc,tabname from msareportlist  where tabtype=2 and reptype=1 order by sdate desc ");
		//printf("sql:%s\n",sqlBuf);
		tablePtr=lt_dbput_table(dbPtr,"weeklist");
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
		  tempRow=ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
					lt_dbput_recordvars(tablePtr,2,"weektime",LT_TYPE_STRING,tempRow[0],"week",LT_TYPE_STRING,tempRow[1]);
		  		tempRow=ltDbFetchRow(tempCursor);
			}
		}
		ltDbCloseCursor(tempCursor);

		sprintf(sqlBuf,"select tabdesc,tabname from msareportlist  where tabtype=3 and reptype=1 order by sdate desc ");
		//printf("sql:%s\n",sqlBuf);
		tablePtr=lt_dbput_table(dbPtr,"monthlist");
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
		  tempRow=ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){//当天有记录
					lt_dbput_recordvars(tablePtr,2,"monthtime",LT_TYPE_STRING,tempRow[0],"month",LT_TYPE_STRING,tempRow[1]);
		  		tempRow=ltDbFetchRow(tempCursor);
			}
		}
		ltDbCloseCursor(tempCursor);

		sprintf(sqlBuf,"select tabdesc,tabname from msareportlist where tabtype=5 and reptype=1 order by sdate desc ");
		//printf("sql:%s\n",sqlBuf);
		tablePtr=lt_dbput_table(dbPtr,"jidulist");
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
		  tempRow=ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
					lt_dbput_recordvars(tablePtr,2,"jidutime",LT_TYPE_STRING,tempRow[0],"jidu",LT_TYPE_STRING,tempRow[1]);
		  		tempRow=ltDbFetchRow(tempCursor);
			}
		}
		ltDbCloseCursor(tempCursor);

		sprintf(sqlBuf,"select tabdesc,tabname from msareportlist where tabtype=4 and reptype=1 order by sdate desc ");
		//printf("sql:%s\n",sqlBuf);
		tablePtr=lt_dbput_table(dbPtr,"yearlist");
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
		  tempRow=ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
					lt_dbput_recordvars(tablePtr,2,"yeartime",LT_TYPE_STRING,tempRow[0],"year",LT_TYPE_STRING,tempRow[1]);
		  		tempRow=ltDbFetchRow(tempCursor);
			}
		}
		ltDbCloseCursor(tempCursor);
		ltDbClose(G_DbCon);

		tablePtr=lt_dbput_table(dbPtr,"list");
	 	for(j=1;j<121;j++){
	 			if(strcmp(_ltPubInfo->topSrvName[j].srvname,"")!=0){
	 				lt_dbput_recordvars(tablePtr,2,
	   	     "twosrvid",LT_TYPE_LONG,j,
	   	     "twosrvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[j].srvname);
			  }
		}

		char *type;
	  if(ltMsgGetVar_s(ltMsgPk,"type")){
	  	type=ltMsgGetVar_s(ltMsgPk,"type");
	  }else{
	  	type="";
	  }

	  char htmlpage[256];
  	memset(htmlpage,0,sizeof(htmlpage));
  	sprintf(htmlpage,"/app/msa/msa/htmlplt/report/%s.htm",type);
  	//printf("htmlpage:%s\n",htmlpage);

		ltMsgPk->msgpktype=1;
		lt_dbput_html(confd,ltMsgPk,dbPtr->doc,htmlpage,dbPtr->head,0);
		ltMsgFree(ltMsgPk);

	  return 0;
}
/*主线分析返回页面调用函数*/
int msaReportShowPage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	ltDbHeadPtr dbPtr;
	ltTablePtr  tablePtr;
	LT_DBROW 		tempRow;
	ltDbCursor *tempCursor;

	char *dbUser;
	char *dbPass;
	char *dbName;
	dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if(G_DbCon==NULL){
			fprintf(stderr,"db connect error\n");
			return 0;
	}

  char sqlBuf[512];
  memset(sqlBuf,0,sizeof(sqlBuf));
  //日报告
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
  sprintf(sqlBuf,"select tabdesc,tabname from msareportlist  where tabtype=1 and reptype=1 order by sdate desc ");
  printf("sql:%s\n",sqlBuf);
	tablePtr=lt_dbput_table(dbPtr,"daylist");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
    tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,2,"daytime",LT_TYPE_STRING,tempRow[0],"day",LT_TYPE_STRING,tempRow[1]);
    		tempRow=ltDbFetchRow(tempCursor);
		}
	}
	ltDbCloseCursor(tempCursor);


	sprintf(sqlBuf,"select tabdesc,tabname from msareportlist  where tabtype=2 and reptype=1 order by sdate desc ");
  printf("sql:%s\n",sqlBuf);
	tablePtr=lt_dbput_table(dbPtr,"weeklist");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
    tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){//当天有记录
				lt_dbput_recordvars(tablePtr,2,"weektime",LT_TYPE_STRING,tempRow[0],"week",LT_TYPE_STRING,tempRow[1]);
    		tempRow=ltDbFetchRow(tempCursor);
		}
	}
	ltDbCloseCursor(tempCursor);

	sprintf(sqlBuf,"select tabdesc,tabname from msareportlist  where tabtype=3 and reptype=1 order by sdate desc ");
  printf("sql:%s\n",sqlBuf);
	tablePtr=lt_dbput_table(dbPtr,"monthlist");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
    tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){//当天有记录

				lt_dbput_recordvars(tablePtr,2,"monthtime",LT_TYPE_STRING,tempRow[0],"month",LT_TYPE_STRING,tempRow[1]);
    		tempRow=ltDbFetchRow(tempCursor);
		}
	}
	ltDbCloseCursor(tempCursor);


	sprintf(sqlBuf,"select tabdesc,tabname from msareportlist where tabtype=4 and reptype=1 order by sdate desc ");
  printf("sql:%s\n",sqlBuf);
	tablePtr=lt_dbput_table(dbPtr,"yearlist");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
    tempRow=ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){//当天有记录
				lt_dbput_recordvars(tablePtr,2,"yeartime",LT_TYPE_STRING,tempRow[0],"year",LT_TYPE_STRING,tempRow[1]);
    		tempRow=ltDbFetchRow(tempCursor);
		}
	}
	ltDbCloseCursor(tempCursor);
	ltDbClose(G_DbCon);

  char *type;
  if(ltMsgGetVar_s(ltMsgPk,"type")){
  	type=ltMsgGetVar_s(ltMsgPk,"type");
  }else{
  	type="";
  }

  char htmlpage[256];
  memset(htmlpage,0,sizeof(htmlpage));
  sprintf(htmlpage,"/app/msa/msa/htmlplt/report/%s.htm",type);

	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,htmlpage,dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;
}
//
//int msasortreport(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
//{
//		char *gid=NULL;
//		char *work=NULL;
//		char *lt_page_content=NULL;
//		ltDbCursor *tempCursor;
//    LT_DBROW tempRow;
//    ltDbHeadPtr dbPtr;
//    ltTablePtr tablePtr;
//    char sqlBuf[5000];
//    char caSdate[32],caEdate[32];
//    char caSdate1[32],caTime1[32];
//    char caSdate2[32];
//   	char fileDate[32];
//    char caTempDir[256];
//    char htmlpage[256];
//    char caLabel[256];
//    char caTmpp[2048];
//    char name[256];
//		long lTime,lTime1,lTime2,lTime0;
//		stralloc strTemp;
//		long num;
//		unsigned    long long lMaxBytes;
//		int   jjj;
//		int   iFd;
//		int   type;
//		int   groupby;
//    int   i;
//    int   index;
//		int gtype;
//		gtype=0;
//		iFd=-1;
//    type=0;
//		lTime=0;
//    lTime1=0;
//    lTime2=0;
//    lTime0=0;
//    lMaxBytes=1;
//    index=1;
//    groupby=0;
//    memset(name,0,sizeof(name));
//    memset(htmlpage,0,sizeof(htmlpage));
//    memset(sqlBuf,0,sizeof(sqlBuf));
//    memset(caSdate,0,sizeof(caSdate));
//    memset(caEdate,0,sizeof(caEdate));
//    memset(caSdate1,0,sizeof(caSdate1));
//    memset(caTime1,0,sizeof(caTime1));
//    memset(caSdate2,0,sizeof(caSdate2));
//    memset(fileDate,0,sizeof(fileDate));
//    memset(caTempDir,0,sizeof(caTempDir));
//    memset(caLabel,0,sizeof(caLabel));
//    memset(caTmpp,0,sizeof(caTmpp));
//    tempCursor=NULL;
//
//		char *dbName;
//		char *dbUser;
//		char *dbPass;
//	  dbName=_ltPubInfo->_dbname;
//		dbUser=_ltPubInfo->_dbuser;
//		dbPass=_ltPubInfo->_dbpass;
//		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
//		if(G_DbCon==NULL){
//			return 0;
//		}
//		gtype=atoi(ltMsgGetVar_s(ltMsgPk,"gtype"));
// 		if(ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
//				sprintf(caSdate,"%s","");
//    }else{
//    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
//    }
//    if(ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
//				sprintf(caEdate,"%s","");
//    }else{
//    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
//    }
//    if(ltMsgGetVar_s(ltMsgPk,"gid")==NULL){
//				gid="";
//    }else{
//    		gid=ltMsgGetVar_s(ltMsgPk,"gid");
//    }
//    if(ltMsgGetVar_s(ltMsgPk,"work")==NULL){
//				work="";
//    }else{
//    		work=ltMsgGetVar_s(ltMsgPk,"work");
//    }
//
//    if(ltMsgGetVar_s(ltMsgPk,"num")==NULL){
//				num=0;
//    }else{
//    		num=atol(ltMsgGetVar_s(ltMsgPk,"num"));
//    }
//
//    if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
//    		type=atol(ltMsgGetVar_s(ltMsgPk,"type"));
//    }
//
//    if(ltMsgGetVar_s(ltMsgPk,"groupby")!=NULL){
//    		groupby=atoi(ltMsgGetVar_s(ltMsgPk,"groupby"));
//    }
//    if(ltMsgGetVar_s(ltMsgPk,"name")==NULL){
//    		sprintf(name,"%s",ltMsgGetVar_s(ltMsgPk,"name"));
//    		printf("reportname %s\n",name);
//    }
//    lTime = time(0);
//    if(!ltStrIsSpaces(caSdate)) {
//          lTime1 = nasCvtLongTime(caSdate,"00:00:00");
//    }else {
//          lTime1 = 0;
//    }
//
//    if(!ltStrIsSpaces(caEdate)) {
//          lTime2 = nasCvtLongTime(caEdate,"23:59:59");
//    }else {
//          lTime2 = 0;
//    }
//
//    if(lTime1 == 0) {
//          lTime1 = lTime;
//    }
//
//		if(lTime2 == 0) {
//          lTime2 = lTime;
//    }
//
//		if(lTime1 > lTime) {
//          lTime1 = lTime;
//    }
//
//    if(lTime2>lTime){
//		 			lTime2 = lTime;
//    }
//
//
//
//    strTemp.s=0;
//	  jjj=0;
//
//		sprintf(caTmpp,"%s","");
//		stralloc_cats(&strTemp,caTmpp);
//		if(gtype!=1){
//				for(i=0;i<1000;i++){
//					if(gid[i]=='1'){
//							jjj++;
//						  if(jjj==1){
//								sprintf(caTmpp,"%d",i);
//				        stralloc_cats(&strTemp,caTmpp);
//		          }else{
//		            sprintf(caTmpp,",%d",i);
//				        stralloc_cats(&strTemp,caTmpp);
//		          }
//					}
//				}
//		}
//		stralloc_cats(&strTemp,"");
//		stralloc_0(&strTemp);
//    memset(caTempDir,0,sizeof(caTempDir));
//    if(type==2){
//    	 sprintf(caTempDir,"%s/report/userflowsort/",_datacenterdir);
//    }else if(type==3){
//    	 sprintf(caTempDir,"%s/report/serviceflowsort/",_datacenterdir);
//    }
//		printf("%s\n",caTempDir);
//		if(ltFileIsExist(caTempDir)!=1) {
//        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
//        		printf("building path error in msasortreport function.\n");
//            return 0;
//        }
//    }
//		sprintf(caTempDir,"%s%lu/",caTempDir,ltStrGetId());
//    mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
//    memset(caTime1,0,sizeof(caTime1));
//    memset(fileDate,0,sizeof(fileDate));
//    nasCvtStime(lTime,fileDate,caTime1);
//    memset(caLabel,0,sizeof(caLabel));
//		sprintf(caLabel,"echo 'date=%s_%s' > %sinfo.ini",fileDate,caTime1,caTempDir);
//		system(caLabel);
//    sprintf(caLabel,"echo 'reportname=%s' >> %sinfo.ini",name,caTempDir);
//    system(caLabel);
//    sprintf(caLabel,"echo 'condition=%s-%s' >> %sinfo.ini",caSdate,caEdate,caTempDir);
//    system(caLabel);
//    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);
//    system(caLabel);
//		sprintf(caLabel,"/bin/cp -f %s/reportplt/dailyreport_ch/*.gif %s",_msahtmldir,caTempDir);
//		system(caLabel);
//		sprintf(caLabel,"/bin/cp -f %s/reportplt/dailyreport_ch/*.jpg %s",_msahtmldir,caTempDir);
//		system(caLabel);
//		memset(caSdate1,0,sizeof(caSdate1));
//		memset(caSdate2,0,sizeof(caSdate2));
//	  nasTimeGetDate(caSdate1,lTime1);
//    nasTimeGetDate(caSdate2,lTime2);
//		memset(sqlBuf,0,sizeof(sqlBuf));
//		if(type==2){
//			 sprintf(sqlBuf,"select b.dispname,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by b.dispname order by abyte desc limit %ld offset 0",caSdate1,caSdate2,strTemp.s,work,num);
//			 if(gtype==1){
//					memset(sqlBuf,0,sizeof(sqlBuf));
//					sprintf(sqlBuf,"select b.dispname,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by b.dispname order by abyte desc limit %ld offset 0",caSdate1,caSdate2,work,num);
//			 }
//		}else if(type==3){//服务
//			 sprintf(sqlBuf,"select a.service,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by a.service order by abyte desc limit %ld offset 0",caSdate1,caSdate2,strTemp.s,work,num);
//			 if(gtype==1){
//					memset(sqlBuf,0,sizeof(sqlBuf));
//					sprintf(sqlBuf,"select a.service,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and workflag=%s group by a.service order by abyte desc limit %ld offset 0",caSdate1,caSdate2,work,num);
//			 }
//		}
//		printf("SORT SQL %s\n",sqlBuf);
//    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
//		if(tempCursor == NULL){
//			sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
//			system(caLabel);/*建立目录和正在处理标志*/
//			return 0;
//		}
//		if(type==3||type==4){
//	     sprintf(caLabel,"%sdata.ini",caTempDir);
//       iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
//	     if(iFd == (-1)) {
//					sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
//		      system(caLabel);/*建立目录和正在处理标志*/
//		      ltDbCloseCursor(tempCursor);
//		      ltMsgFree(ltMsgPk);
//		      return 0;
//		   }
//	  }
//
//		dbPtr=lt_dbinit();
//    lt_db_htmlpage(dbPtr,"utf-8");
//    tablePtr=lt_dbput_table(dbPtr,"flowlist");
//    char srvStr[64];
//    char flow[64];
//    memset(srvStr,0,sizeof(srvStr));
//    memset(flow,0,sizeof(flow));
//	  tempRow=ltDbFetchRow(tempCursor);
//	  while(tempRow!=NULL){
//	  	  if(type==2){//人员流量排序
//	  	  		if(atoll(tempRow[1])>1024000){
//               sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
//            }else{
//               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
//            }
//	  	  	  lt_dbput_recordvars(tablePtr,2,
//							"uname",LT_TYPE_STRING,tempRow[0],
//			   			"lbyte",LT_TYPE_STRING,flow
//	   		    );
//	  	  }else if(type==3){//服务流量排序
//	  	  		sprintf(srvStr,"NO.%d",index);
//	  	  		if(atoll(tempRow[1])>1024000){
//               sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
//            }else{
//               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
//            }
//	  	  	  lt_dbput_recordvars(tablePtr,3,
//	  	  	  	"index",LT_TYPE_STRING,srvStr,
//							"sname",LT_TYPE_STRING,_ltPubInfo->ltService[atol(tempRow[0])].srvName,
//			   			"lbyte",LT_TYPE_STRING,flow
//	   		    );
//
//	  	  	  sprintf(caLabel,"%d,%lld\r\n",index,atoll(tempRow[1]));
//						write(iFd, caLabel, strlen(caLabel));
//						if(lMaxBytes<atoll(tempRow[1])){
//					       lMaxBytes=atoll(tempRow[1]);
//						}
//	  	  }
//	  	  if(type==3){
//					index++;
//				}
//    		tempRow=ltDbFetchRow(tempCursor);
//    }
//    ltDbCloseCursor(tempCursor);
//		lt_dbput_rootvars(dbPtr,3,"reportname","system","bdate",caSdate1,"edate",caSdate2);
//		if(type==3){
//			close(iFd);
//      sprintf(caLabel,"%s/Scripts/onlinesrv1.pl  %sdata.ini %ssrvreporta.gif %llu ",_msahtmldir,caTempDir,caTempDir,lMaxBytes);
//	    system(caLabel);
//			lt_dbput_rootvars(dbPtr,1,"piechart","srvreporta.gif" );
//	  }
//		sprintf(htmlpage,"%s%s/sortplt%d.htm",_msahtmldir,_sortreportplt,type);
//		lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
//		if(lt_page_content==NULL){
//				sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
//		    system(caLabel);
//	      lt_dbfree(dbPtr);
//		    return 0;
//		}else{
//				lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
//				sprintf(caLabel,"%sindex.htm",caTempDir);
//				iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
//				if(iFd >0) {
//		       write(iFd,lt_page_content,strlen(lt_page_content));
//		    }
//		    close(iFd);
//		}
//		//Date source 查询
//		char caFile[128],caLine[512],dateSourceSql[1024],tojava[512];
//		int fd=-1,datenum=30;
//
//		ltDbCursor *tempCursor1;
//    LT_DBROW tempRow1;
//		memset(caFile,0,sizeof(caFile));
//		memset(caLine,0,sizeof(caLine));
//		memset(tojava,0,sizeof(tojava));
//		memset(dateSourceSql,0,sizeof(dateSourceSql));
//		sprintf(caFile,"%ssource.csv",caTempDir);
//		fd = open(caFile, O_APPEND | O_WRONLY | O_CREAT, 0644);
//		if(fd == (-1)) {
//				 printf("file can't open in function msasortreport.\n");
//				 ltMsgPk->msgpktype=1;
//				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
//				 ltMsgFree(ltMsgPk);
//				 close(fd);
//				 return 0;
//		}
//		if(type==2){
//			 sprintf(dateSourceSql,"select a.userid,b.dispname,sum(bytes) as abyte  from msauserhttpreport a,msauser b ,msagroup c where b.groupid0 in(%s) and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by a.userid,b.dispname order by abyte desc",strTemp.s,work);
//			 if(gtype==1){
//					memset(dateSourceSql,0,sizeof(dateSourceSql));
//					sprintf(dateSourceSql,"select a.userid,b.dispname,sum(bytes) as abyte  from msauserhttpreport a,msauser b ,msagroup c where a.userid=b.userid and b.groupid0=c.id and workflag=%s group by a.userid,b.dispname order by abyte desc",work);
//			 }
//		}else if(type==3){//服务
//			 sprintf(dateSourceSql,"select a.service,sum(bytes) as abyte from msauserhttpreport a,msauser b ,msagroup c where  a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by a.service order by abyte desc",strTemp.s,work);
//			 if(gtype==1){
//					memset(dateSourceSql,0,sizeof(dateSourceSql));
//					sprintf(dateSourceSql,"select a.service,sum(bytes) as abyte from msauserhttpreport a,msauser b ,msagroup c where a.userid=b.userid and b.groupid0=c.id  and workflag=%s group by a.service order by abyte desc",work);
//			 }
//			// printf("aaa:%s\n",dateSourceSql);
//		}
//		printf("xxx:%s\n",dateSourceSql);
//
//		memset(srvStr,0,sizeof(srvStr));
//    memset(flow,0,sizeof(flow));
//    tempCursor = ltDbOpenCursor(G_DbCon,dateSourceSql);
//	  tempRow=ltDbFetchRow(tempCursor);
//	  while(tempRow!=NULL){
//	  	  if(type==2){//人员流量排序
//		  	  		memset(caLine,0,sizeof(caLine));
//							sprintf(caLine,"%s, , , , , ,",tempRow[1]);
//							write(fd,caLine,strlen(caLine));
//							write(fd,"\r\n",strlen("\r\n"));
//							memset(dateSourceSql,0,sizeof(dateSourceSql));
//							sprintf(dateSourceSql,"select a.userid,a.service,sum(bytes) as abyte,sum(onlinetime) as stime from msauserhttpreport a where a.workflag=%s and a.userid=%s group by a.userid,a.service order by abyte desc",work,tempRow[0]);
//						  printf("\n DateSourceSql  %s\n",dateSourceSql);
//						  //二次查询
//					    tempCursor1 = ltDbOpenCursor(G_DbCon,dateSourceSql);
//						  tempRow1=ltDbFetchRow(tempCursor1);
//						  while(tempRow1!=NULL){
//						 			 if(atoll(tempRow1[2])>1024000){
//               			 	 sprintf(flow,"%0.3fM",atoll(tempRow1[2])/1024000.00);
//				           }else{
//				               sprintf(flow,"%0.3fKB",atoll(tempRow1[2])/1024.00);
//				           }
//				           int uid=atol(tempRow1[0]);
//				           nasDbUserInfo *tmpdbuserinfo;
//									 tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,uid);
//									 int gid=tmpdbuserinfo[uid].groupid[0];
//									 printf("gid:%d\n",gid);
//				           memset(caLine,0,sizeof(caLine));
//									 sprintf(caLine," ,%s,%s,%s,%s",
//														_ltPubInfo->_GroupList[gid].name,
//														_ltPubInfo->topSrvName[atoi(tempRow1[1])].srvname,
//														flow,
//														time2str(atoi(tempRow1[3]))
//									 );
//									 write(fd,caLine,strlen(caLine));
//									 write(fd,"\r\n",strlen("\r\n"));
//									 tempRow1=ltDbFetchRow(tempCursor1);
//						  }
//						  ltDbCloseCursor(tempCursor1);
//	  	  }else if(type==3){//服务流量排序
//		  	  	 memset(caLine,0,sizeof(caLine));
//						 sprintf(caLine,"%s, , , , ,",_ltPubInfo->topSrvName[atoi(tempRow[0])].srvname);
//						 write(fd,caLine,strlen(caLine));
//						 write(fd,"\r\n",strlen("\r\n"));
//						//每个服务的前十名用户排序
//						 memset(dateSourceSql,0,sizeof(dateSourceSql));
//						 sprintf(dateSourceSql,"select sum(bytes) as abyte,b.dispname,c.name,sum(onlinetime) as stime  from msauserhttpreport a,msauser b ,msagroup c where  a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s and a.service=%ld group by b.dispname,c.name order by abyte desc limit %d offset 0",strTemp.s,work,atol(tempRow[0]),datenum*5);
//						 if(gtype==1){
//								memset(dateSourceSql,0,sizeof(dateSourceSql));
//								sprintf(dateSourceSql,"select sum(bytes) as abyte,b.dispname,c.name,sum(onlinetime) as stime   from msauserhttpreport a,msauser b ,msagroup c where a.userid=b.userid and b.groupid0=c.id  and workflag=%s and a.service=%ld group by b.dispname,c.name order by abyte desc limit %d offset 0",work,atol(tempRow[0]),datenum*5);
//						 }
//						 printf("\n DateSourceSql  %s\n",dateSourceSql);
//						 //二次查询
//					   tempCursor1 = ltDbOpenCursor(G_DbCon,dateSourceSql);
//						 tempRow1=ltDbFetchRow(tempCursor1);
//						 while(tempRow1!=NULL){
//						 			 if(atoll(tempRow1[0])>1024000){
//	             			 sprintf(flow,"%0.3fM",atoll(tempRow1[0])/1024000.00);
//				           }else{
//				             sprintf(flow,"%0.3fKB",atoll(tempRow1[0])/1024.00);
//				           }
//				           memset(caLine,0,sizeof(caLine));
//									 sprintf(caLine," ,%s,%s,%s,%s",
//														tempRow1[1],
//														tempRow1[2],
//														flow,
//														time2str(atoi(tempRow1[3]))
//									 );
//									 write(fd,caLine,strlen(caLine));
//									 write(fd,"\r\n",strlen("\r\n"));
//									 tempRow1=ltDbFetchRow(tempCursor1);
//						 }
//						 ltDbCloseCursor(tempCursor1);
//	  	  }
//    		tempRow=ltDbFetchRow(tempCursor);
//    }
//    ltDbCloseCursor(tempCursor);
//    close(fd);
//
//		switch(type){
//	      case 2:
//				sprintf(tojava,"%s sortSourceExcel %s %ssource.xls %d",JAVACMD,caFile,caTempDir,2);
//				chdir(BINCMD);
//				system(tojava);
//	      break;
//	      case 3:
//				sprintf(tojava,"%s sortSourceExcel %s %ssource.xls %d",JAVACMD,caFile,caTempDir,3);
//				chdir(BINCMD);
//				system(tojava);
//	      break;
//		}
//		chdir(caTempDir);
//    system("rm -f report.tgz");
//	  system("tar -cvzf report.tgz *");
//	  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
//	  system(caLabel);
//	  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
//	  system(caLabel);
//		ltMsgPk->msgpktype=1;
//	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");
//	  ltMsgFree(ltMsgPk);
//	  lt_dbfree(dbPtr);
//    return 0;
//}
/*MSA:报告中心功能：生成的报告列表JSONstore*/
int msaReportJsonStore(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
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
			sprintf(caCmd,"rm -Rf %s/report/%s/%s",_datacenterdir,reportdir,filedir);
	    system(caCmd);

	    ltMsgPk->msgpktype=1;
	    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n", "lthtml",LT_TYPE_STRING,"1");
	    ltMsgFree(ltMsgPk);
	    return 0;
	 }

	 sprintf(caFile1,"%s/report/%s/",_datacenterdir,reportdir);
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
            jjj++;
            memset(condition,0,sizeof(condition));
            memset(reportname,0,sizeof(reportname));
				    memset(reportuser,0,sizeof(reportuser));
				    memset(templname,0,sizeof(templname));
				    memset(reportstate,0,sizeof(reportstate));
				    memset(cDate,0,sizeof(cDate));
				    memset(cDate2,0,sizeof(cDate2));
					  sprintf(caFile1,"%s/report/%s/%s/info.ini",_datacenterdir,reportdir,fnames[len].caDirName);
				    bcCnfGetValue_s(caFile1,"date",cDate);
				    bcCnfGetValue_s(caFile1,"date",cDate2);
				    bcCnfGetValue_s(caFile1,"reportname",reportname);
				    bcCnfGetValue_s(caFile1,"condition",condition);
				    cDate2[10]='\0';
					  sprintf(reportstate,"报告已生成");
					  sprintf(caFile1,"%s/report/%s/%s/errmsg.ini",_datacenterdir,reportdir,fnames[len].caDirName);
				    if(stat(caFile1,&st) == 0){
				    	sprintf(caFile1,"<a href=\"/report/%s/%s/errmsg.ini\">Error:生成报告错误!</a>",reportdir,fnames[len].caDirName);
						  sprintf(reportstate,caFile1);
				    }
				    sprintf(caFile1,"%s/report/%s/%s/proccess.ini",_datacenterdir,reportdir,fnames[len].caDirName);
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
//汇总分析
#define _dailyreport_ch	"/reportplt/dailyreport_ch"
int msaSummaryReport(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;
    stralloc 		strTemp;
    char  			caTmpp[2048];
    char        htmlpage[512];
    char        oldname[256];
    char        sqlBuf[1024];
    char        caLabel[128];
    char        caSrv[64];
    char        name[256];
    char 				email[32];
    int         iFd=0,jjj,i;
    int         type=0;
    long 				num;
    int         tabtype=0;
		char 				*gid;
		char 				*work;
		char 				*lt_page_content;
		ltDbCursor  *tempCursor;
		LT_DBROW 	  tempRow;
    char 		 		*tabname;
    char 		 		caTempDir[256];
		char 		 		*dbName;
		char 		 		*dbUser;
		char 		 		*dbPass;
		ltDbConn 		*G_DbCon;
	  dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
			return 0;
		}

    if(ltMsgGetVar_s(ltMsgPk,"gid")==NULL){
				gid="";
    }else{
    		gid=ltMsgGetVar_s(ltMsgPk,"gid");
    }
    if(ltMsgGetVar_s(ltMsgPk,"work")==NULL){
				work="";
    }else{
    		work=ltMsgGetVar_s(ltMsgPk,"work");
    }
    if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
    		type=atoi(ltMsgGetVar_s(ltMsgPk,"type"));
    }
    if(ltMsgGetVar_s(ltMsgPk,"tabtype")!=NULL){
	    	tabtype=atoi(ltMsgGetVar_s(ltMsgPk,"tabtype"));
	  }
	  if(ltMsgGetVar_s(ltMsgPk,"tabname")==NULL){
				tabname="";
	  }else{
	    	tabname=ltMsgGetVar_s(ltMsgPk,"tabname");
	  }
    if(ltMsgGetVar_s(ltMsgPk,"name")!=NULL){
    		sprintf(name,"%s",ltMsgGetVar_s(ltMsgPk,"name"));
    }

    if(ltMsgGetVar_s(ltMsgPk,"num")==NULL){
				num=0;
    }else{
    		num=atol(ltMsgGetVar_s(ltMsgPk,"num"));
    }

    if( ltMsgGetVar_s(ltMsgPk,"email")==NULL){
				  sprintf(email,"%s","");
	  }else{
	    		sprintf(email,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
	  }

    printf("type:%d\n",type);
    strTemp.s=0;
	  jjj=0;
		sprintf(caTmpp,"%s","");
		stralloc_cats(&strTemp,caTmpp);
		for(i=0;i<_MAX_ALLGRPNUM;i++){
			if(gid[i]=='1'){
					jjj++;
				  if(jjj==1){
						sprintf(caTmpp,"%d",i);
		        stralloc_cats(&strTemp,caTmpp);
          }else{
            sprintf(caTmpp,",%d",i);
		        stralloc_cats(&strTemp,caTmpp);
          }
			}
		}
		stralloc_cats(&strTemp,"");
		stralloc_0(&strTemp);

    if(type==1){
    	 sprintf(caTempDir,"%s/report/grouphuizong/",_datacenterdir);
    }else if(type==2){
    	 sprintf(caTempDir,"%s/report/userhuizong/",_datacenterdir);
    }
    char caSdate[32],caEdate[32];
    char caSdate1[32];
    char caSdate2[32];
    long lTime,lTime1,lTime2;
    lTime=0;
    lTime1=0;
    lTime2=0;
		memset(caSdate1,0,sizeof(caSdate1));
		memset(caSdate2,0,sizeof(caSdate2));
		if(ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
				sprintf(caSdate,"%s","");
    }else{
    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
    }
    if(ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
				sprintf(caEdate,"%s","");
    }else{
    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
    }

		lTime = time(0);
    if(!ltStrIsSpaces(caSdate)) {
          lTime1 = nasCvtLongTime(caSdate,"00:00:00");
    }else {
          lTime1 = 0;
    }

    if(!ltStrIsSpaces(caEdate)) {
          lTime2 = nasCvtLongTime(caEdate,"23:59:59");
    }else {
          lTime2 = 0;
    }

    if(lTime1 == 0) {
          lTime1 = lTime;
    }

		if(lTime2 == 0) {
          lTime2 = lTime;
    }

		if(lTime1 > lTime) {
          lTime1 = lTime;
    }

    if(lTime2>lTime){
		 			lTime2 = lTime;
    }

		memset(caSdate1,0,sizeof(caSdate1));
		memset(caSdate2,0,sizeof(caSdate2));
	  nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            return 0;
        }
    }

    unsigned long tmpDir=0;
    tmpDir=ltStrGetId();
    char responseDir[256];
    sprintf(responseDir,"%lu",tmpDir);
    sprintf(caTempDir,"%s%lu/",caTempDir,tmpDir);
    mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    lTime = time(0);
    char sDate[32];
    char sTime[32];
    nasCvtStime(lTime,sDate,sTime);
    memset(caLabel,0,sizeof(caLabel));
    sprintf(caLabel,"echo 'date=%s_%s' > %sinfo.ini",sDate,sTime,caTempDir);
		system(caLabel);
    sprintf(caLabel,"echo 'reportname=%s' >> %sinfo.ini",name,caTempDir);
    system(caLabel);
    sprintf(caLabel,"echo 'condition=%s-%s' >> %sinfo.ini",ltMsgGetVar_s(ltMsgPk,"sdate"),ltMsgGetVar_s(ltMsgPk,"edate"),caTempDir);
    system(caLabel);
    sprintf(caLabel,"echo '1' > %sproccess.ini",caTempDir);
    system(caLabel);
		sprintf(caLabel,"/bin/cp -f %s%s/*.gif %s",_msahtmldir,_dailyreport_ch,caTempDir);
		system(caLabel);
		sprintf(caLabel,"/bin/cp -f %s%s/*.jpg %s",_msahtmldir,_dailyreport_ch,caTempDir);
		system(caLabel);

		ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,responseDir);

    sprintf(caLabel,"echo '10' > %sproccess.ini",caTempDir);
    system(caLabel);
		if(type==2){				/*按用户流量 按用户连接*/

					sprintf(caLabel,"echo '20' > %sproccess.ini",caTempDir);
    			system(caLabel);

					int       iTemp,usersrvrequest,*srvrequest;
					long long *srvflow,usersrvflow;
					char      caLine[30000],caggg[64];
					memset(oldname,0,sizeof(oldname));
					memset(caggg,0,sizeof(caggg));
					srvflow=(long long *)malloc(sizeof(long long)*(MAX_TOP_SRVNUM+1));
					srvrequest=(int *)malloc(sizeof(int)*(MAX_TOP_SRVNUM+1));

					for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
						srvflow[iTemp]=0;
						srvrequest[iTemp]=0;
					}
					usersrvflow=0;
					usersrvrequest=0;
					sprintf(caLabel,"echo '30' > %sproccess.ini",caTempDir);
    			system(caLabel);
					if(jjj==1000){
						sprintf(sqlBuf,"select b.username,service,sum(bytes)/1024+1 as abyte,sum(onlinetime) as arequest,c.name from msasrvreport a,msauser b,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s  group by b.username,service,c.name order by abyte desc limit %ld offset 0",caSdate1,caSdate2,work,num);
					}else{
						sprintf(sqlBuf,"select b.username,service,sum(bytes)/1024+1 as abyte,sum(onlinetime) as arequest,c.name from msasrvreport a,msauser b,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and b.groupid0 in (%s) and workflag=%s  group by b.username,service,c.name order by abyte desc limit %ld offset 0",caSdate1,caSdate2,strTemp.s,work,num);
					}
				  printf("sql4:%s\n",sqlBuf);
					tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
					if(tempCursor == NULL) {
						sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
						system(caLabel);
						return 0;
					}
					sprintf(caLabel,"echo '40' > %sproccess.ini",caTempDir);
    			system(caLabel);
					dbPtr=lt_dbinit();
					lt_db_htmlpage(dbPtr,"utf-8");
					tablePtr=lt_dbput_table(dbPtr,"glist");
					for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
						 if(strlen(_ltPubInfo->topSrvName[iTemp].srvname)<=0){
						 	break;
						 }
						 lt_dbput_recordvars(tablePtr,1,"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[iTemp].srvname);
					}
					sprintf(caLabel,"echo '50' > %sproccess.ini",caTempDir);
    			system(caLabel);
					tablePtr=lt_dbput_table(dbPtr,"llist");
					tempRow= ltDbFetchRow(tempCursor);
					while(tempRow!=NULL){
						if(strcmp(oldname,tempRow[0])!=0){								/*新的一行*/
							if(oldname[0]!='\0'){
								sprintf(caLine,"<tr><td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n <td class=xl25724 style='border-top:none;border-left:none'><table align=center><tr><td>%lldKB</td>\
																</tr><tr><td>%s</td>\
															  </tr></table></td>",oldname,caggg,usersrvflow,time2str(usersrvrequest));
								for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
										if(strlen(_ltPubInfo->topSrvName[iTemp].srvname)<=0){break;}
										   sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>\
										   									<table align=center><tr><td>%lldKB</td></tr>\
																			  <tr><td>%s</td>\
																			  </tr></table></td>",caLine,srvflow[iTemp],time2str(srvrequest[iTemp]));
								}
								lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
							}
							sprintf(oldname,"%s",tempRow[0]);
							sprintf(caggg,"%s",tempRow[4]);
							usersrvflow=0;
							usersrvrequest=0;
							for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
									srvflow[iTemp]=0;
									srvrequest[iTemp]=0;
							}
							srvflow[atol(tempRow[1])]=atol(tempRow[2]);
							srvrequest[atol(tempRow[1])]=atol(tempRow[3]);
							usersrvflow=usersrvflow+atol(tempRow[2]);
							usersrvrequest=usersrvrequest+atol(tempRow[3]);
						}else{
							srvflow[atol(tempRow[1])]=atol(tempRow[2]);			/*每一个服务*/
							srvrequest[atol(tempRow[1])]=atol(tempRow[3]);	/*每一个服务*/
							usersrvflow=usersrvflow+atol(tempRow[2]);				/*所有服务总流量*/
							usersrvrequest=usersrvrequest+atol(tempRow[3]);	/*所有服务总时间*/
						}
						tempRow= ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor);
					sprintf(caLabel,"echo '60' > %sproccess.ini",caTempDir);
    			system(caLabel);
					if(oldname[0]!='\0'){																/*新的一行开始写*/
						sprintf(caLine,"<tr><td height=19 class=xl29724 style='height:14.25pt;border-top:none'>%s[%s]</td>\r\n <td class=xl25724 style='border-top:none;border-left:none'><table align=center><tr><td>%lldKB</td>\
																</tr><tr><td>%s</td>\
															  </tr></table></td>",oldname,caggg,usersrvflow,time2str(usersrvrequest));
						for(iTemp=0;iTemp<MAX_TOP_SRVNUM;iTemp++){
							if(strlen(_ltPubInfo->topSrvName[iTemp].srvname)<=0)break;
								sprintf(caLine,"%s\r\n<td class=xl25724 style='border-top:none;border-left:none'>\
																				<table align=center><tr><td>%lldKB</td>\
																			  </tr><tr><td>%s</td>\
																			  </tr></table></td>",caLine,srvflow[iTemp],time2str(srvrequest[iTemp]));
						}
						sprintf(caLine,"%s</tr>",caLine);
						lt_dbput_recordvars(tablePtr,1,"linestring",LT_TYPE_STRING,caLine);
					}
					sprintf(caLabel,"echo '70' > %sproccess.ini",caTempDir);
    			system(caLabel);
					sprintf(htmlpage,"%s%s/usersrvplt.htm",_msahtmldir,_dailyreport_ch);
					lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
					if(lt_page_content==NULL){
							sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
							system(caLabel);
							lt_dbfree(dbPtr);
							return 0;
					}else{
							lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
							sprintf(caLabel,"%sindex.htm",caTempDir);
							iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
							if(iFd >0) {
							  write(iFd,lt_page_content,strlen(lt_page_content));
							}
							close(iFd);
					}
					sprintf(caLabel,"echo '80' > %sproccess.ini",caTempDir);
    			system(caLabel);
    			sleep(1);

    			sprintf(caLabel,"%s/Scripts/html2mail  %s index.htm %sindex.mht report %s",_msahtmldir,caTempDir,caTempDir,email);
  				system(caLabel);

    			sprintf(caLabel,"echo '90' > %sproccess.ini",caTempDir);
    			system(caLabel);
    			sleep(1);

    			sprintf(caLabel,"echo '100' > %sproccess.ini",caTempDir);
    			system(caLabel);
    			sleep(1);

    			sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
				  system(caLabel);
				  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
				  system(caLabel);

					chdir(caTempDir);
			    system("rm -f report.tgz");
				  system("tar -cvzf report.tgz *.jpg *.gif *.htm");

				  ltMsgFree(ltMsgPk);
				  lt_dbfree(dbPtr);

					return 0;
	  }
	  sprintf(caLabel,"echo '50' > %sproccess.ini",caTempDir);
		system(caLabel);
		sleep(3);
		tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor == NULL){
				sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
		    system(caLabel);
		    ltDbCloseCursor(tempCursor);
		    return 0;
		}
		sprintf(caLabel,"echo '60' > %sproccess.ini",caTempDir);
		system(caLabel);
		sleep(3);
		memset(oldname,0,sizeof(oldname));
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    tablePtr=lt_dbput_table(dbPtr,"flowlist");
    tempRow=ltDbFetchRow(tempCursor);
    while(tempRow!=NULL){
        if(strcmp(oldname,tempRow[0])==0){
						  sprintf(caSrv,"%s",_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);
							lt_dbput_recordvars(tablePtr,5,
								"gname",LT_TYPE_STRING,"",
								"sname",LT_TYPE_STRING,caSrv,
								"sbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
								"st",LT_TYPE_STRING,time2str(atoll(tempRow[3])
								)
							);
        }else{
							char caGname[512];
							sprintf(caSrv,"%s",_ltPubInfo->topSrvName[atol(tempRow[1])].srvname);
					    sprintf(caGname,"%s",tempRow[0]);
							sprintf(oldname,"%s",tempRow[0]);
							lt_dbput_recordvars(tablePtr,5,
								"gname",LT_TYPE_STRING,caGname,
								"sname",LT_TYPE_STRING,caSrv,
								"sbyte",LT_TYPE_LONGLONG,atoll(tempRow[2]),
								"st",LT_TYPE_STRING,time2str(atoll(tempRow[3]))
							);
				}
				tempRow= ltDbFetchRow(tempCursor);
		}
  	ltDbCloseCursor(tempCursor);
  	sprintf(caLabel,"echo '70' > %sproccess.ini",caTempDir);
		system(caLabel);
		sleep(3);
	  memset(htmlpage,0,sizeof(htmlpage));
	  sprintf(htmlpage,"%s%s/srvreportplta.htm",_msahtmldir,_dailyreport_ch);
		lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
	  if(lt_page_content==NULL){
		    sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
	    	system(caLabel);
	    	lt_dbfree(dbPtr);
	      return 0;
		}else{
		    lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
		    sprintf(caLabel,"%sindex.htm",caTempDir);
				iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
	      if(iFd >0) {
	         write(iFd,lt_page_content,strlen(lt_page_content));
	      }
	      close(iFd);
	  }

	  sprintf(caLabel,"echo '80' > %sproccess.ini",caTempDir);
		system(caLabel);
		sleep(1);

		sprintf(caLabel,"%s/Scripts/html2mail  %s index.htm %sindex.mht report %s",_msahtmldir,caTempDir,caTempDir,email);
  	system(caLabel);

	  sprintf(caLabel,"echo '90' > %sproccess.ini",caTempDir);
		system(caLabel);
		sleep(1);

		sprintf(caLabel,"echo '100' > %sproccess.ini",caTempDir);
		system(caLabel);
		sleep(1);

	  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	  system(caLabel);

	  chdir(caTempDir);
    system("rm -f report.tgz");
	  system("tar -cvzf report.tgz *.jpg *.gif *.htm");

	  ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");
	  ltMsgFree(ltMsgPk);
	  lt_dbfree(dbPtr);
    return 0;
}
#ifndef __FLOWCOMPARAREPORT_DIR__
	#define __FLOWCOMPARAREPORT_DIR__
	#define _flowcomparereport   "/report/flowcomparereport"
	#define _flowcompareplt 	"/reportplt/flowcompareplt"
#endif
/*MSA:报告中心流量趋势分析报告创建调用函数*///MAX_TOP_SRVNUM
int msacompareFlowAjaxSubmit(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    ltDbCursor  *tempCursor=NULL;
    ltDbCursor  *tempCursorNew;
    LT_DBROW    tempRow;
    LT_DBROW    tempRowNew;
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;
    stralloc 		strTemp;
		char        *reportname=NULL;
		char        *email=NULL;
		char        *lt_page_content=NULL;
		char        *gid=NULL;
		char        *work=NULL;
		char        caTempDir[256];
		char        responseDir[256];
    char        caLabel[256];
    char        htmlpage[512];
    char        strBuf[4096];
    char        sDate[64],sTime[64];
	  char        caFile1[256];
    char        caCmd[256];
    char        atime[64];
    char        srvName[255];
    char 				caTmpp[2048];
    char 				flow[128];
    int         iFd,fdwrite,jjj,tabtype,j;
    unsigned long tmpDir=0;
    unsigned long fileName=0;
    //unsigned int
    unsigned long long i,data1[MAX_TOP_SRVNUM],data2[MAX_TOP_SRVNUM],data3[MAX_TOP_SRVNUM],data4[MAX_TOP_SRVNUM],data5[MAX_TOP_SRVNUM],data6[MAX_TOP_SRVNUM],data7[MAX_TOP_SRVNUM],data8[MAX_TOP_SRVNUM],data9[MAX_TOP_SRVNUM],data10[MAX_TOP_SRVNUM],data11[MAX_TOP_SRVNUM],data12[MAX_TOP_SRVNUM];
    char mydata1[32],mydata2[32],mydata3[32],mydata4[32],mydata5[32],mydata6[32],mydata7[32],mydata8[32],mydata9[32],mydata10[32],mydata11[32],mydata12[32];
    //int count=0;
		//初始化
		for(i=0;i<MAX_TOP_SRVNUM;i++){
			data1[i]=0;
			data2[i]=0;
			data3[i]=0;
			data4[i]=0;
			data5[i]=0;
			data6[i]=0;
			data7[i]=0;
			data8[i]=0;
			data9[i]=0;
			data10[i]=0;
			data11[i]=0;
			data12[i]=0;
		}

	  dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");

    i=0;
    strTemp.s=0;
	  jjj=tabtype=0;

    memset(caTempDir,0,sizeof(caTempDir));
    memset(responseDir,0,sizeof(responseDir));
    memset(caLabel,0,sizeof(caLabel));
    memset(htmlpage,0,sizeof(htmlpage));
    memset(atime,0,sizeof(atime));
    memset(srvName,0,sizeof(srvName));
   	memset(caTmpp,0,sizeof(caTmpp));
   	memset(flow,0,sizeof(flow));

  	char *dbUser;
	  char *dbPass;
	  char *dbName;
	  dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		ltDbConn     *G_DbConNew;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		G_DbConNew=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
			   memset(caLabel,0,sizeof(caLabel));
				 sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
		     system(caLabel);

		     return 0;
		}
		if(G_DbConNew==NULL){
			   memset(caLabel,0,sizeof(caLabel));
				 sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
		     system(caLabel);

		     return 0;
		}

	  if(ltMsgGetVar_s(ltMsgPk,"tabtype")){
	  	 tabtype=atoi(ltMsgGetVar_s(ltMsgPk,"tabtype"));
	  }

	  if(ltMsgGetVar_s(ltMsgPk,"email")){
	  	email=ltMsgGetVar_s(ltMsgPk,"email");
	  }
		reportname=ltMsgGetVar_s(ltMsgPk,"reportname");
    char caSdate[32],caEdate[32];
	  unsigned int time1,time2;
    memset(caSdate,0,sizeof(caSdate));
    memset(caEdate,0,sizeof(caEdate));
		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
		time2 = nasCvtLongTime(caEdate,"00:00:00");
		time1=time2-86400*7;

    memset(caSdate,0,sizeof(caSdate));
    memset(caEdate,0,sizeof(caEdate));
		nasTimeGetDate(caSdate,time1);
		nasTimeGetDate(caEdate,time2);
    if(ltMsgGetVar_s(ltMsgPk,"gid")){
	  		gid=ltMsgGetVar_s(ltMsgPk,"gid");
	  		sprintf(caTmpp,"%s","");
				stralloc_cats(&strTemp,caTmpp);
				int i;
				for(i=0;i<1000;i++){
					if(gid[i]=='1'){
							jjj++;
						  if(jjj==1){
								sprintf(caTmpp,"%d",i);
				        stralloc_cats(&strTemp,caTmpp);
		          }else{
		            sprintf(caTmpp,",%d",i);
				        stralloc_cats(&strTemp,caTmpp);
		          }
					}
				}
				stralloc_cats(&strTemp,"");
				stralloc_0(&strTemp);
	  }
	  if(ltMsgGetVar_s(ltMsgPk,"filedir")){
	  	fileName=atoll(ltMsgGetVar_s(ltMsgPk,"filedir"));
	  }
		if(ltMsgGetVar_s(ltMsgPk,"work")){
	  	work=ltMsgGetVar_s(ltMsgPk,"work");
	  }

    sprintf(caTempDir,"%s%s",_datacenterdir,_flowcomparereport);
		if(ltFileIsExist(caTempDir)!=1) {
		       if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
				     memset(caLabel,0,sizeof(caLabel));
						 sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件,无法生成报告",caTempDir);
				     system(caLabel);

				     return 0;
		       }
		}

    if(fileName==0){
		    tmpDir=ltStrGetId();
		    sprintf(caTempDir,"%s%s/%lu/",_datacenterdir,_flowcomparereport,tmpDir);
		    sprintf(responseDir,"%lu",tmpDir);
		    if(ltFileIsExist(caTempDir)!=1) {
		       if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
				       memset(caLabel,0,sizeof(caLabel));
							 sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件,无法生成报告",caTempDir);
					     system(caLabel);

					     return 0;
		       }
		 		}
    }else{
    	  sprintf(caTempDir,"%s%s/%lu/",_datacenterdir,_flowcomparereport,fileName);
    	  sprintf(responseDir,"%lu",fileName);
    }
	  fdwrite=-1;

	  nasCvtStime(time(0),sDate,sTime);
	  sprintf(caFile1,"%sinfo.ini",caTempDir);
	  fdwrite = open(caFile1, O_APPEND | O_WRONLY | O_CREAT, 0644);
		if(fdwrite == (-1)) {
			 ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
			 ltMsgFree(ltMsgPk);
			 return 0;
		}
		sprintf(caCmd,"date=%s_%s\n",sDate,sTime);
	  write(fdwrite,caCmd,strlen(caCmd));
	  sprintf(caCmd,"condition=%s/%s\n",caSdate,caEdate);
		write(fdwrite,caCmd,strlen(caCmd));
	  sprintf(caCmd,"reportname=%s\n",reportname);
	  write(fdwrite,caCmd,strlen(caCmd));
	  close(fdwrite);
	  sprintf(caLabel,"/bin/cp %s%s/right-top-d.jpg  %s",_msahtmldir,_flowcompareplt,caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/cp %s%s/right-top.jpg  %s",_msahtmldir,_flowcompareplt,caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/cp %s%s/banner.jpg  %s",_msahtmldir,_flowcompareplt,caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/cp %s%s/up-1.jpg  %s",_msahtmldir,_flowcompareplt,caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/cp %s%s/up-2.jpg  %s",_msahtmldir,_flowcompareplt,caTempDir);
	  system(caLabel);
		sprintf(caLabel,"/bin/cp %s%s/up-3.jpg  %s",_msahtmldir,_flowcompareplt,caTempDir);
	  system(caLabel);

		j=1;
	  memset(strBuf,0,sizeof(strBuf));
					if(jjj==1000){
		    		sprintf(strBuf,"select service,sum(bytes) as abyte from msasrvreport a,msauser b,msagroup c  where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and workflag=%s group by service order by service desc ",caSdate,caEdate,work);
		    	}else{
		    		sprintf(strBuf,"select service,sum(bytes) as abyte from msasrvreport a,msauser b,msagroup c  where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by service order by service desc ",caSdate,caEdate,strTemp.s,work);
		    	}
printf("strBuf:%s\n",strBuf);
				  tempCursorNew = ltDbOpenCursor(G_DbConNew,strBuf);
				  if(tempCursorNew != NULL){
						//lMaxBytes=1;
					  tempRowNew= ltDbFetchRow(tempCursorNew);
					  while(tempRowNew!=NULL){
						  if(j==1){
						  	data1[atoll(tempRowNew[0])]=atoll(tempRowNew[1]);
							}else if(j==2){
								data2[atoll(tempRowNew[0])]=atoll(tempRowNew[1]);
							}else if(j==3){
								data3[atoll(tempRowNew[0])]=atoll(tempRowNew[1]);
							}else if(j==4){
								data4[atoll(tempRowNew[0])]=atoll(tempRowNew[1]);
							}else if(j==5){
								data5[atoll(tempRowNew[0])]=atoll(tempRowNew[1]);
							}else if(j==6){
								data6[atoll(tempRowNew[0])]=atoll(tempRowNew[1]);
							}else if(j==7){
								data7[atoll(tempRowNew[0])]=atoll(tempRowNew[1]);
							}else if(j==8){
								data8[atoll(tempRowNew[0])]=atoll(tempRowNew[1]);
							}else if(j==9){
								data9[atoll(tempRowNew[0])]=atoll(tempRowNew[1]);
							}else if(j==10){
								data10[atoll(tempRowNew[0])]=atoll(tempRowNew[1]);
							}else if(j==11){
								data11[atoll(tempRowNew[0])]=atoll(tempRowNew[1]);
							}else if(j==12){
								data12[atoll(tempRowNew[0])]=atoll(tempRowNew[1]);
							}
				      tempRowNew=ltDbFetchRow(tempCursorNew);
				    }
				    ltDbCloseCursor(tempCursorNew);
			    }else{
					      sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
				    	  system(caLabel);
				    	  sprintf(caLabel,"echo '0' > %sproccess.ini",caTempDir);
				  			system(caLabel);
				    	  ltMsgFree(ltMsgPk);
				        return 0;
				  }
	    		tempRow=ltDbFetchRow(tempCursor);
  				j++;
		ltDbCloseCursor(tempCursor);

    sprintf(caLabel,"%sdata.ini",caTempDir);
    iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
    if(iFd == (-1)){
				sprintf(caLabel,"echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
	    	system(caLabel);

	      ltDbCloseCursor(tempCursor);
	      ltMsgFree(ltMsgPk);
	      return 0;
	  }

		tablePtr=lt_dbput_table(dbPtr,"list");

  	char reportdate[64];
  	sprintf(reportdate,"%s-%s",sDate,sTime);
		lt_dbput_rootvars(dbPtr,2,"reportname",reportname,"date",reportdate);


		for(i=0;i<MAX_TOP_SRVNUM;i++){
			if(!(_ltPubInfo->topSrvName[i].srvname[0])){
				continue;
			}
			memset(mydata1,0,sizeof(mydata1));
			memset(mydata2,0,sizeof(mydata2));
			memset(mydata3,0,sizeof(mydata3));
			memset(mydata4,0,sizeof(mydata4));
			memset(mydata5,0,sizeof(mydata5));
			memset(mydata6,0,sizeof(mydata6));
			memset(mydata7,0,sizeof(mydata7));
			memset(mydata8,0,sizeof(mydata8));
			memset(mydata9,0,sizeof(mydata9));
			memset(mydata10,0,sizeof(mydata10));
			memset(mydata11,0,sizeof(mydata11));
			memset(mydata12,0,sizeof(mydata12));
			if(data1[i]>=1024000){
				sprintf(mydata1,"%0.3fM",data1[i]/1024000.00);
			}else{
				sprintf(mydata1,"%0.3fKB",data1[i]/1024.00);
			}
//printf("data1[%lld]:%lld\n",i,data1[i]);
//printf("mydata1:%s\n",mydata1);
			if(data2[i]>=1024000){
				sprintf(mydata2,"%0.3fM",data2[i]/1024000.00);
			}else{
				sprintf(mydata2,"%0.3fKB",data2[i]/1024.00);
			}
			if(data3[i]>=1024000){
				sprintf(mydata3,"%0.3fM",data3[i]/1024000.00);
			}else{
				sprintf(mydata3,"%0.3fKB",data3[i]/1024.00);
			}
			if(data4[i]>=1024000){
				sprintf(mydata4,"%0.3fM",data4[i]/1024000.00);
			}else{
				sprintf(mydata4,"%0.3fKB",data4[i]/1024.00);
			}
			if(data5[i]>=1024000){
				sprintf(mydata5,"%0.3fM",data5[i]/1024000.00);
			}else{
				sprintf(mydata5,"%0.3fKB",data5[i]/1024.00);
			}
			if(data6[i]>=1024000){
				sprintf(mydata6,"%0.3fM",data6[i]/1024000.00);
			}else{
				sprintf(mydata6,"%0.3fKB",data6[i]/1024.00);
			}
			if(data7[i]>=1024000){
				sprintf(mydata7,"%0.3fM",data7[i]/1024000.00);
			}else{
				sprintf(mydata7,"%0.3fKB",data7[i]/1024.00);
			}
			if(data8[i]>=1024000){
				sprintf(mydata8,"%0.3fM",data8[i]/1024000.00);
			}else{
				sprintf(mydata8,"%0.3fKB",data8[i]/1024.00);
			}
			if(data9[i]>=1024000){
				sprintf(mydata9,"%0.3fM",data9[i]/1024000.00);
			}else{
				sprintf(mydata9,"%0.3fKB",data9[i]/1024.00);
			}
			if(data10[i]>=1024000){
				sprintf(mydata10,"%0.3fM",data10[i]/1024000.00);
			}else{
				sprintf(mydata10,"%0.3fKB",data10[i]/1024.00);
			}
			if(data11[i]>=1024000){
				sprintf(mydata11,"%0.3fM",data11[i]/1024000.00);
			}else{
				sprintf(mydata11,"%0.3fKB",data11[i]/1024.00);
			}
			if(data12[i]>=1024000){
				sprintf(mydata12,"%0.3fM",data12[i]/1024000.00);
			}else{
				sprintf(mydata12,"%0.3fKB",data12[i]/1024.00);
			}

			lt_dbput_recordvars(tablePtr,13,
			"srvname",LT_TYPE_STRING,_ltPubInfo->topSrvName[i].srvname,
			"srvljflowA",LT_TYPE_STRING,mydata1,
			"srvljflowB",LT_TYPE_STRING,mydata2,
			"srvljflowC",LT_TYPE_STRING,mydata3,
			"srvljflowD",LT_TYPE_STRING,mydata4,
			"srvljflowE",LT_TYPE_STRING,mydata5,
			"srvljflowF",LT_TYPE_STRING,mydata6,
			"srvljflowG",LT_TYPE_STRING,mydata7,
			"srvljflowH",LT_TYPE_STRING,mydata8,
			"srvljflowI",LT_TYPE_STRING,mydata9,
			"srvljflowJ",LT_TYPE_STRING,mydata10,
			"srvljflowK",LT_TYPE_STRING,mydata11,
			"srvljflowL",LT_TYPE_STRING,mydata12
			);
	  }
		sprintf(htmlpage,"%s%s/flow1.htm",_msahtmldir,_flowcompareplt);
		printf("%s\n",htmlpage);
	  lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);

    memset(caLabel,0,sizeof(caLabel));
    sprintf(caLabel,"echo '50' > %sproccess.ini",caTempDir);
	  system(caLabel);

	  if(lt_page_content==NULL){
	  		printf("no date souce.\n");
		    sprintf(caLabel,"echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
	  		system(caLabel);
	  		ltMsgFree(ltMsgPk);
	  		lt_dbfree(dbPtr);
     		return 0;
		}else{
				lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
			  sprintf(caLabel,"%sindex.htm",caTempDir);
			  iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
		    if(iFd >0) {
		      write(iFd,lt_page_content,strlen(lt_page_content));
		    }
	    	close(iFd);
	  }
    memset(caLabel,0,sizeof(caLabel));
    sprintf(caLabel,"echo '80' > %sproccess.ini",caTempDir);
	  system(caLabel);

    memset(caLabel,0,sizeof(caLabel));
   	sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	  system(caLabel);

    memset(caLabel,0,sizeof(caLabel));
	  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	  system(caLabel);

	  chdir(caTempDir);
    system("rm -f report.tar");
	  system("tar -cf report.tar *");

	  memset(caLabel,0,sizeof(caLabel));
	  sprintf(caLabel,"/app/msa/bin/html2mail  %sindex.htm %sindex.mht report %s",caTempDir,caTempDir,email);
  	system(caLabel);
		
    ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,responseDir);
		ltMsgFree(ltMsgPk);
		return 0;
}
/*
MSA:报告中心
部门流量排序
*/
int msasortreport(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
		char *dbName;
		char *dbUser;
		char *dbPass;
	  dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
			return 0;
		}
		ltDbConn	*G_DbCon1;
		G_DbCon1=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon1==NULL){
			return 0;
		}

		char *gid=NULL;
		char *work=NULL;
		char *lt_page_content=NULL;
		ltDbCursor *tempCursor;
    LT_DBROW tempRow;
		ltDbCursor *tempCursor1;
    LT_DBROW tempRow1;
    ltDbHeadPtr dbPtr;
    ltTablePtr tablePtr;
    char sqlBuf[4096],dateSourceSql[4096];
    char caSdate[32],caEdate[32];
    char caSdate1[32],caTime1[32];
    char caSdate2[32];
   	char fileDate[32];
    char caTempDir[256];
    char htmlpage[256];
    char caLabel[256];
    char caTmpp[2048];
    char name[256];
    char caLine[2048],tojava[1024],utf8Str[2046],urlsortstr[64];
    char caFile[256];
		long lTime,lTime1,lTime2,lTime0;
		stralloc strTemp;
		long num;
		int datenum;
		datenum=30;
		unsigned    long long lMaxBytes;
		int   jjj;
		int   iFd;
		int   fd;
		int   type;
    int   i;
    int groupby;
    int   index;
		int gtype;
		gtype=0;
		iFd=-1;
		fd=-1;
    type=0;
		lTime=0;
    lTime1=0;
    lTime2=0;
    lTime0=0;
    groupby=0;
    lMaxBytes=1;
    index=1;
    memset(name,0,sizeof(name));
    memset(htmlpage,0,sizeof(htmlpage));
    memset(sqlBuf,0,sizeof(sqlBuf));
    memset(caSdate,0,sizeof(caSdate));
    memset(caEdate,0,sizeof(caEdate));
    memset(caSdate1,0,sizeof(caSdate1));
    memset(caTime1,0,sizeof(caTime1));
    memset(caSdate2,0,sizeof(caSdate2));
    memset(fileDate,0,sizeof(fileDate));
    memset(caTempDir,0,sizeof(caTempDir));
    memset(caLabel,0,sizeof(caLabel));
    memset(caTmpp,0,sizeof(caTmpp));
    tempCursor=NULL;
    tempCursor1=NULL;
		gtype=atoi(ltMsgGetVar_s(ltMsgPk,"gtype"));
 		if(ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
				sprintf(caSdate,"%s","");
    }else{
    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
    }
    if(ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
				sprintf(caEdate,"%s","");
    }else{
    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
    }
    if(ltMsgGetVar_s(ltMsgPk,"gid")==NULL){
				gid="";
    }else{
    		gid=ltMsgGetVar_s(ltMsgPk,"gid");
    }
    if(ltMsgGetVar_s(ltMsgPk,"work")==NULL){
				work="";
    }else{
    		work=ltMsgGetVar_s(ltMsgPk,"work");
    }
    if(ltMsgGetVar_s(ltMsgPk,"num")==NULL){
				num=0;
    }else{
    		num=atol(ltMsgGetVar_s(ltMsgPk,"num"));
    }

    if(ltMsgGetVar_s(ltMsgPk,"datenum")!=NULL){
				datenum=atoi(ltMsgGetVar_s(ltMsgPk,"datenum"));
    }

    if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
    		type=atol(ltMsgGetVar_s(ltMsgPk,"type"));
    }

    if(ltMsgGetVar_s(ltMsgPk,"groupby")!=NULL){
    		groupby=atoi(ltMsgGetVar_s(ltMsgPk,"groupby"));
    }

		memset(urlsortstr,0,sizeof(urlsortstr));
    if(ltMsgGetVar_s(ltMsgPk,"urlid")!=NULL){
    		if(atoi(ltMsgGetVar_s(ltMsgPk,"urlid"))!=-1){
    			sprintf(urlsortstr," and urlsort=%s",ltMsgGetVar_s(ltMsgPk,"urlid"));
    		}
    }
    if(ltMsgGetVar_s(ltMsgPk,"name")!=NULL){
    		sprintf(name,"%s",ltMsgGetVar_s(ltMsgPk,"name"));
    }
    lTime = time(0);
    if(!ltStrIsSpaces(caSdate)) {
          lTime1 = nasCvtLongTime(caSdate,"00:00:00");
    }else {
          lTime1 = 0;
    }

    if(!ltStrIsSpaces(caEdate)) {
          lTime2 = nasCvtLongTime(caEdate,"23:59:59");
    }else {
          lTime2 = 0;
    }

    if(lTime1 == 0) {
          lTime1 = lTime;
    }

		if(lTime2 == 0) {
          lTime2 = lTime;
    }

		if(lTime1 > lTime) {
          lTime1 = lTime;
    }

    if(lTime2>lTime){
		 			lTime2 = lTime;
    }
    strTemp.s=0;
	  jjj=0;
		sprintf(caTmpp,"%s","");
		stralloc_cats(&strTemp,caTmpp);
		if(gtype!=1){
				for(i=0;i<1000;i++){
					if(gid[i]=='1'){
							jjj++;
						  if(jjj==1){
								sprintf(caTmpp,"%d",i);
				        stralloc_cats(&strTemp,caTmpp);
		          }else{
		            sprintf(caTmpp,",%d",i);
				        stralloc_cats(&strTemp,caTmpp);
		          }
					}
				}
		}
		stralloc_cats(&strTemp,"");
		stralloc_0(&strTemp);
    memset(caTempDir,0,sizeof(caTempDir));
    if(type==1){
    	 sprintf(caTempDir,"%s/report/groupflowsort/",_datacenterdir);
    }else if(type==2){
    	 sprintf(caTempDir,"%s/report/userflowsort/",_datacenterdir);
    }else if(type==3){
    	 sprintf(caTempDir,"%s/report/serviceflowsort/",_datacenterdir);
    }else if(type==4){
    	 sprintf(caTempDir,"%s/report/exportratesort/",_datacenterdir);
    }else if(type==5){
    	 sprintf(caTempDir,"%s/report/datetotalflowsort/",_datacenterdir);
    }else if(type==6){
    	 sprintf(caTempDir,"%s/report/siteclasssort/",_datacenterdir);
    }else if(type==7){
    	 sprintf(caTempDir,"%s/report/webaccesssort/",_datacenterdir);
    }

		if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            return 0;
        }
    }

		sprintf(caTempDir,"%s%lu/",caTempDir,ltStrGetId());
    mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    memset(caTime1,0,sizeof(caTime1));
    memset(fileDate,0,sizeof(fileDate));
    nasCvtStime(lTime,fileDate,caTime1);
    memset(caLabel,0,sizeof(caLabel));
		sprintf(caLabel,"echo 'date=%s_%s' > %sinfo.ini",fileDate,caTime1,caTempDir);
		system(caLabel);
    sprintf(caLabel,"echo 'reportname=%s' >> %sinfo.ini",name,caTempDir);
    system(caLabel);
    sprintf(caLabel,"echo 'condition=%s-%s' >> %sinfo.ini",caSdate,caEdate,caTempDir);
    system(caLabel);
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);
    system(caLabel);
		sprintf(caLabel,"/bin/cp -f %s/reportplt/dailyreport_ch/banner.jpg %s/reportplt/dailyreport_ch/right-top.jpg %s/reportplt/dailyreport_ch/right-top.jpg %s/reportplt/dailyreport_ch/up-1.jpg %s/reportplt/dailyreport_ch/up-2.jpg %s/reportplt/dailyreport_ch/up-3.jpg %s",_msahtmldir,_msahtmldir,_msahtmldir,_msahtmldir,_msahtmldir,_msahtmldir,caTempDir);
		system(caLabel);
		//创建CSV文件
		sprintf(caFile,"%sreport.csv",caTempDir);
		fd = open(caFile, O_APPEND | O_WRONLY | O_CREAT, 0644);
		if(fd == (-1)) {
				 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
				 ltMsgFree(ltMsgPk);
				 close(fd);
				 return 0;
		}
		memset(caSdate1,0,sizeof(caSdate1));
		memset(caSdate2,0,sizeof(caSdate2));
	  nasTimeGetDate(caSdate1,lTime1);
    nasTimeGetDate(caSdate2,lTime2);
		memset(sqlBuf,0,sizeof(sqlBuf));
		if(type==1){
			 sprintf(sqlBuf,"select c.name,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by c.name order by abyte desc limit %ld offset 0",caSdate1,caSdate2,strTemp.s,work,num);
			 if(gtype==1){
					memset(sqlBuf,0,sizeof(sqlBuf));
					sprintf(sqlBuf,"select c.name,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by c.name order by abyte desc limit %ld offset 0",caSdate1,caSdate2,work,num);
			 }
		}else if(type==2){
			 sprintf(sqlBuf,"select b.dispname,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by b.dispname order by abyte desc limit %ld offset 0",caSdate1,caSdate2,strTemp.s,work,num);
			 if(gtype==1){
					memset(sqlBuf,0,sizeof(sqlBuf));
					sprintf(sqlBuf,"select b.dispname,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by b.dispname order by abyte desc limit %ld offset 0",caSdate1,caSdate2,work,num);
			 }
		}else if(type==3){//服务
			 sprintf(sqlBuf,"select a.service,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by a.service order by abyte desc limit %ld offset 0",caSdate1,caSdate2,strTemp.s,work,num);
			 if(gtype==1){
					memset(sqlBuf,0,sizeof(sqlBuf));
					sprintf(sqlBuf,"select a.service,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and workflag=%s group by a.service order by abyte desc limit %ld offset 0",caSdate1,caSdate2,work,num);
			 }
		}else if(type==4){
			 if(groupby==0){
			 	 			sprintf(sqlBuf,"select hours,avg(ubytes+dbytes) as abyte from msatimereport  where sdate>='%s' and sdate<='%s'  group by hours order by abyte desc ",caSdate1,caSdate2);
			 }else if(groupby==1){
			 				sprintf(sqlBuf,"select sdate,avg(ubytes+dbytes) as abyte from msatimereport  where sdate>='%s' and sdate<='%s'  group by sdate order by abyte desc ",caSdate1,caSdate2);
			 }
		}else if(type==5){
			 sprintf(sqlBuf,"select a.sdate,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s) and workflag=%s group by a.sdate order by abyte desc limit %ld offset 0",caSdate1,caSdate2,strTemp.s,work,num);
			 if(gtype==1){
					memset(sqlBuf,0,sizeof(sqlBuf));
					sprintf(sqlBuf,"select a.sdate,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by a.sdate order by abyte desc limit %ld offset 0",caSdate1,caSdate2,work,num);
			 }
		}else if(type==6){
			 sprintf(sqlBuf,"select urlsort,sum(bytes) as abyte  from msahttpreport where sdate>='%s' and sdate<='%s' and workflag=%s group by urlsort order by abyte desc ",caSdate1,caSdate2,work);
		}else if(type==7){
			 sprintf(sqlBuf,"select a.host,sum(bytes) as abyte,sum(lcount) as allcount,sum(ctime) as allctime from msauserhttpreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id  and c.id in(%s)%s and workflag=%s group by a.host order by abyte desc limit %ld offset 0",caSdate1,caSdate2,strTemp.s,urlsortstr,work,num);
			 if(gtype==1){
					memset(sqlBuf,0,sizeof(sqlBuf));
					sprintf(sqlBuf,"select a.host,sum(bytes) as abyte,sum(lcount) as allcount,sum(ctime) as allctime from msauserhttpreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id%s and workflag=%s group by a.host order by abyte desc limit %ld offset 0",caSdate1,caSdate2,urlsortstr,work,num);
			 }
		}
		printf("SORT SQL %s\n",sqlBuf);
    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor == NULL){
			sprintf(caLabel,"echo '%s' > %serrmsg.ini","数据库错误,无法生成报告",caTempDir);
			system(caLabel);/*建立目录和正在处理标志*/
			return 0;
		}
		if(type==3||type==4){
	     sprintf(caLabel,"%sdata.ini",caTempDir);
       iFd = open(caLabel,O_WRONLY | O_CREAT | O_TRUNC);
	     if(iFd == (-1)) {
					sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无法建立数据文件",caTempDir);
		      system(caLabel);/*建立目录和正在处理标志*/
		      ltDbCloseCursor(tempCursor);
		      ltMsgFree(ltMsgPk);
		      return 0;
		   }
	  }

		dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    tablePtr=lt_dbput_table(dbPtr,"flowlist");
    char srvStr[64];
    char flow[64];
    memset(srvStr,0,sizeof(srvStr));
    memset(flow,0,sizeof(flow));
	  tempRow=ltDbFetchRow(tempCursor);
	  while(tempRow!=NULL){
	  	  if(type==1){//部门流量排序
	  	  		if(atoll(tempRow[1])>1024000){
               sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
            }else{
               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
            }
            memset(caLine,0,sizeof(caLine));
						sprintf(caLine,"%s,%s",
											tempRow[0],
											flow
						);
						write(fd,caLine,strlen(caLine));
						write(fd,"\r\n",strlen("\r\n"));

		  	  	lt_dbput_recordvars(tablePtr,2,
							"groupname",LT_TYPE_STRING,tempRow[0],
			   			"lbyte",LT_TYPE_STRING,flow
	   		    );
	  	  }else if(type==2){//人员流量排序
	  	  		if(atoll(tempRow[1])>1024000){
               sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
            }else{
               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
            }
            memset(caLine,0,sizeof(caLine));
						sprintf(caLine,"%s,%s",
											tempRow[0],
											flow
						);
						write(fd,caLine,strlen(caLine));
						write(fd,"\r\n",strlen("\r\n"));
	  	  	  lt_dbput_recordvars(tablePtr,2,
							"uname",LT_TYPE_STRING,tempRow[0],
			   			"lbyte",LT_TYPE_STRING,flow
	   		    );
	  	  }else if(type==3){//服务流量排序
	  	  		sprintf(srvStr,"NO.%d",index);
	  	  		if(atoll(tempRow[1])>1024000){
               sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
            }else{
               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
            }
            memset(caLine,0,sizeof(caLine));
						sprintf(caLine,"%s,%s",
											tempRow[0],
											flow
						);
						write(fd,caLine,strlen(caLine));
						write(fd,"\r\n",strlen("\r\n"));
	  	  	  lt_dbput_recordvars(tablePtr,3,
	  	  	  	"index",LT_TYPE_STRING,srvStr,
							"sname",LT_TYPE_STRING,_ltPubInfo->ltService[atol(tempRow[0])].srvName,
			   			"lbyte",LT_TYPE_STRING,flow
	   		    );

	  	  	  sprintf(caLabel,"%d,%lld\r\n",index,atoll(tempRow[1]));
						write(iFd, caLabel, strlen(caLabel));
						if(lMaxBytes<atoll(tempRow[1])){
								lMaxBytes=atoll(tempRow[1])+10;
						}
	  	  }else if(type==4){//出口速率分布
	  	  	  if(groupby==0){//小时
	  	  	  	  lt_dbput_recordvars(tablePtr,2,
					   			"flowbps",LT_TYPE_LONGLONG,atoll(tempRow[1])/60,
					   			"onlinetime",LT_TYPE_STRING,tempRow[0]
   		   		 		);
	  	  	  }else if(groupby==1){//天
	  	  	  	  lt_dbput_recordvars(tablePtr,2,
					   			"flowbps",LT_TYPE_LONGLONG,atoll(tempRow[1])/60,
					   			"onlinetime",LT_TYPE_STRING,tempRow[0]
   		   		 		);
	  	  	  }
						memset(caLine,0,sizeof(caLine));
						sprintf(caLine,"%s,%lld",
											tempRow[0],
											atoll(tempRow[1])/60
						);
						write(fd,caLine,strlen(caLine));
						write(fd,"\r\n",strlen("\r\n"));
	  	  	  sprintf(caLabel,"%s,%lld\r\n",tempRow[0],atoll(tempRow[1])/60);
						write(iFd, caLabel, strlen(caLabel));
						if(lMaxBytes<atoll(tempRow[1])/60){
								lMaxBytes=atoll(tempRow[1])/60+10;
						}
	  	  }else if(type==5){
	  	  		  if(atoll(tempRow[1])>1024000){
               	 sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
	            }else{
	               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
	            }
						memset(caLine,0,sizeof(caLine));
						sprintf(caLine,"%s|%s",
											tempRow[0],
											flow
						);
						write(fd,caLine,strlen(caLine));
						write(fd,"\r\n",strlen("\r\n"));
	  	  	   lt_dbput_recordvars(tablePtr,2,
			   			"lbyte",LT_TYPE_STRING,flow,
			   			"date",LT_TYPE_STRING,tempRow[0]
	   		     );
	  	  }else if(type==6){
	  	  		 if(atoll(tempRow[1])>1024000){
               	 sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
	            }else{
	               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
	            }

							memset(caLine,0,sizeof(caLine));
							sprintf(caLine,"%s,%s",
												tempRow[0],
												flow
							);
							write(fd,caLine,strlen(caLine));
							write(fd,"\r\n",strlen("\r\n"));
	   		      lt_dbput_recordvars(tablePtr,2,
	  	  	    "urlsort",LT_TYPE_STRING,_ltPubInfo->ltService[atol(tempRow[0])].srvName,
			   			"lbyte",LT_TYPE_STRING,flow
	   		      );
	  	  }else if(type==7){
	  	  		 if(atoll(tempRow[1])>1024000){
               	 sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
	           }else{
	               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
	           }
	           memset(caLine,0,sizeof(caLine));
						 sprintf(caLine,"%s,%s,%s,%s",
											tempRow[0],
											flow,
											tempRow[2],
											tempRow[3]
						 );
						 write(fd,caLine,strlen(caLine));
						 write(fd,"\r\n",strlen("\r\n"));
	  	  	   lt_dbput_recordvars(tablePtr,4,
							"host",LT_TYPE_STRING,tempRow[0],
			   			"lbyte",LT_TYPE_STRING,flow,
			   			"lcount",LT_TYPE_STRING,tempRow[2],
			   			"ctime",LT_TYPE_STRING,tempRow[3]
	   		     );
	  	  }
	  	  if(type==3){
					index++;
				}

    		tempRow=ltDbFetchRow(tempCursor);
    }
    ltDbCloseCursor(tempCursor);
    close(fd);
    switch(type){
	      case 2:
	      strgb2utf8("人员流量排序 用户名称 流量",utf8Str,strlen("人员流量排序 用户名称 流量"));
				sprintf(tojava,"%s sortReportExcel %s %sreport.xls %s",JAVACMD,caFile,caTempDir,utf8Str);
				chdir(BINCMD);
				printf("JAVA111： %s\n",tojava);
				system(tojava);
	      break;
	      case 3:
	      strgb2utf8("服务流量排序 服务名称 流量",utf8Str,strlen("服务流量排序 服务名称 流量"));
				sprintf(tojava,"%s sortReportExcel %s %sreport.xls %s",JAVACMD,caFile,caTempDir,utf8Str);
				chdir(BINCMD);
				printf("JAVA1111： %s\n",tojava);
				system(tojava);
	      break;
		}


		lt_dbput_rootvars(dbPtr,3,"reportname","system","bdate",caSdate1,"edate",caSdate2);
		if(type==3){
			close(iFd);
      sprintf(caLabel,"%s/Scripts/onlinesrv1.pl  %sdata.ini %ssrvreporta.gif %llu ",_msahtmldir,caTempDir,caTempDir,lMaxBytes);
	    system(caLabel);
			lt_dbput_rootvars(dbPtr,1,"piechart","srvreporta.gif" );
	  }
	  if(type==4){
			close(iFd);
      sprintf(caLabel,"%s/Scripts/outflowpltline.pl  %sdata.ini %sflowline.gif %llu ",_msahtmldir,caTempDir,caTempDir,lMaxBytes);
	    system(caLabel);
			lt_dbput_rootvars(dbPtr,1,"flowline","flowline.gif" );
	  }
		sprintf(htmlpage,"%s%s/sortplt%d.htm",_msahtmldir,_sortreportplt,type);
		lt_page_content=ltPltFileParse(htmlpage,dbPtr->doc,0);
		if(lt_page_content==NULL){
				sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
		    system(caLabel);
	      lt_dbfree(dbPtr);
		    return 0;
		}else{
				lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
				sprintf(caLabel,"%sindex.htm",caTempDir);
				iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644);
				if(iFd >0) {
		       write(iFd,lt_page_content,strlen(lt_page_content));
		    }
		    close(iFd);
		}
		chdir(caTempDir);
		//Date source 查询
		sprintf(caFile,"%ssource.csv",caTempDir);
		fd = open(caFile, O_APPEND | O_WRONLY | O_CREAT, 0644);
		if(fd == (-1)) {
				 ltMsgPk->msgpktype=1;
				 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
				 ltMsgFree(ltMsgPk);
				 close(fd);
				 return 0;
		}
		if(type==1){
			 sprintf(dateSourceSql,"select c.name,c.id from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s) and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by c.name,c.id order by c.id desc",caSdate1,caSdate2,strTemp.s,work);
			 if(gtype==1){
					memset(dateSourceSql,0,sizeof(dateSourceSql));
					sprintf(dateSourceSql,"select c.name,c.id from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by c.name,c.id  order by c.id desc",caSdate1,caSdate2,work);
			 }
		}else if(type==2){
			 sprintf(dateSourceSql,"select a.userid,b.dispname from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s) and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by a.userid,b.dispname order by a.userid desc",caSdate1,caSdate2,strTemp.s,work);
			 if(gtype==1){
					memset(dateSourceSql,0,sizeof(dateSourceSql));
					sprintf(dateSourceSql,"select a.userid,b.dispname from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by a.userid,b.dispname order by a.userid desc",caSdate1,caSdate2,work);
			 }
		}else if(type==3){//服务
			 sprintf(dateSourceSql,"select b.username,a.userid from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s) and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by b.username,a.userid order by a.userid desc",caSdate1,caSdate2,strTemp.s,work);
			 if(gtype==1){
					memset(dateSourceSql,0,sizeof(dateSourceSql));
					sprintf(dateSourceSql,"select b.username,a.userid from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by b.username,a.userid order by a.userid desc",caSdate1,caSdate2,work);
			 }
		}else if(type==4){
			 if(groupby==0){
			 	 			sprintf(dateSourceSql,"select hours,avg(ubytes+dbytes) as abyte from msatimereport  where sdate>='%s' and sdate<='%s'  group by hours order by abyte desc ",caSdate1,caSdate2);
			 }else if(groupby==1){
			 				sprintf(dateSourceSql,"select sdate,avg(ubytes+dbytes) as abyte from msatimereport  where sdate>='%s' and sdate<='%s'  group by sdate order by abyte desc ",caSdate1,caSdate2);
			 }
		}else if(type==5){
			 //sprintf(dateSourceSql,"select a.sdate,sum(bytes) as abyte from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by a.sdate order by abyte desc",caSdate1,caSdate2,work);
			 sprintf(dateSourceSql,"select a.sdate from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s) and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by a.sdate order by a.sdate ASC",caSdate1,caSdate2,strTemp.s,work);
			 if(gtype==1){
					memset(dateSourceSql,0,sizeof(dateSourceSql));
					sprintf(dateSourceSql,"select a.sdate from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by a.sdate order by a.sdate ASC",caSdate1,caSdate2,work);
			 }
		}else if(type==6){
			 sprintf(dateSourceSql,"select urlsort,sum(bytes) as abyte  from msahttpreport where sdate>='%s' and sdate<='%s' and workflag=%s group by urlsort order by abyte desc ",caSdate1,caSdate2,work);
		}else if(type==7){
			 sprintf(dateSourceSql,"select b.dispname,a.userid from msauserhttpreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s)%s and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by b.dispname,a.userid order by b.dispname desc",caSdate1,caSdate2,strTemp.s,urlsortstr,work);
			 if(gtype==1){
					memset(dateSourceSql,0,sizeof(dateSourceSql));
					sprintf(dateSourceSql,"select b.dispname,a.userid from msauserhttpreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s'%s and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by b.dispname,a.userid order by b.dispname desc",caSdate1,caSdate2,urlsortstr,work);
			 }
		}

		memset(srvStr,0,sizeof(srvStr));
    memset(flow,0,sizeof(flow));
    printf("\n DateSourceSql  %s\n",dateSourceSql);
    tempCursor = ltDbOpenCursor(G_DbCon,dateSourceSql);
	  tempRow=ltDbFetchRow(tempCursor);
	  char sTime[64];
	  while(tempRow!=NULL){
	  	  if(type==1){//部门流量排序
		       	memset(caLine,0,sizeof(caLine));
						sprintf(caLine,"%s, , , , , ,",tempRow[0]);
						write(fd,caLine,strlen(caLine));
						write(fd,"\r\n",strlen("\r\n"));

						memset(dateSourceSql,0,sizeof(dateSourceSql));
						sprintf(dateSourceSql,"select b.dispname,a.sdate,a.service,sum(bytes) as abyte,sum(onlinetime) as stime from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0=%s and a.userid=b.userid and b.groupid0=c.id and workflag=%s group by b.dispname,a.sdate,a,service order by abyte desc limit %d offset 0",caSdate1,caSdate2,tempRow[1],work,datenum);
						printf("\n DateSourceSql  %s\n",dateSourceSql);


						//二次查询
					   tempCursor1 = ltDbOpenCursor(G_DbCon1,dateSourceSql);
						 tempRow1=ltDbFetchRow(tempCursor1);
						 while(tempRow1!=NULL){
						 			 if(atoll(tempRow1[3])>1024000){
               			 sprintf(flow,"%0.3fM",atoll(tempRow1[3])/1024000.00);
				           }else{
				               sprintf(flow,"%0.3fKB",atoll(tempRow1[3])/1024.00);
				           }
				           memset(caLine,0,sizeof(caLine));
				           memset(sTime,0,sizeof(sTime));
			             sprintf(sTime,"%ld",atol(tempRow1[4])/3600);
							     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])/60);
							     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])%60);
									 sprintf(caLine," ,%s,%s,%s,%s,%s",
														tempRow1[0],
														tempRow1[1],
														_ltPubInfo->topSrvName[atoi(tempRow1[2])].srvname,
														flow,
														sTime
									 );
									 write(fd,caLine,strlen(caLine));
									 write(fd,"\r\n",strlen("\r\n"));
									 tempRow1=ltDbFetchRow(tempCursor1);
						 }
						ltDbCloseCursor(tempCursor1);
	  	  }else if(type==2){//人员流量排序
	  	  		memset(caLine,0,sizeof(caLine));
						sprintf(caLine,"%s, , , , , ,",tempRow[1]);
						write(fd,caLine,strlen(caLine));
						write(fd,"\r\n",strlen("\r\n"));

						memset(dateSourceSql,0,sizeof(dateSourceSql));
						sprintf(dateSourceSql,"select c.name,a.sdate,a.service,sum(bytes) as abyte,sum(onlinetime) as stime from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s) and a.userid=b.userid and b.groupid0=c.id and workflag=%s and a.userid=%s group by c.name,a.sdate,a,service order by abyte desc limit %d offset 0",caSdate1,caSdate2,strTemp.s,work,tempRow[0],datenum);
						if(gtype==1){
								memset(dateSourceSql,0,sizeof(dateSourceSql));
								sprintf(dateSourceSql,"select c.name,a.sdate,a.service,sum(bytes) as abyte,sum(onlinetime) as stime from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s and a.userid=%s group by c.name,a.sdate,a,service order by abyte desc limit %d offset 0",caSdate1,caSdate2,work,tempRow[0],datenum);
						}
						printf("\n DateSourceSql  %s\n",dateSourceSql);
						 //二次查询
					   tempCursor1 = ltDbOpenCursor(G_DbCon1,dateSourceSql);
						 tempRow1=ltDbFetchRow(tempCursor1);
						 while(tempRow1!=NULL){
						 			 if(atoll(tempRow1[3])>1024000){
               			 sprintf(flow,"%0.3fM",atoll(tempRow1[3])/1024000.00);
				           }else{
				               sprintf(flow,"%0.3fKB",atoll(tempRow1[3])/1024.00);
				           }
				           memset(caLine,0,sizeof(caLine));
				           memset(sTime,0,sizeof(sTime));
			             sprintf(sTime,"%ld",atol(tempRow1[4])/3600);
							     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])/60);
							     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])%60);
									 sprintf(caLine," ,%s,%s,%s,%s,%s",
														tempRow1[0],
														tempRow1[1],
														_ltPubInfo->topSrvName[atoi(tempRow1[2])].srvname,
														flow,
														sTime
									 );
									 write(fd,caLine,strlen(caLine));
									 write(fd,"\r\n",strlen("\r\n"));
									 tempRow1=ltDbFetchRow(tempCursor1);
						 }
						 ltDbCloseCursor(tempCursor1);
	  	  }else if(type==3){//服务流量排序
	  	  	 memset(caLine,0,sizeof(caLine));
					 sprintf(caLine,"%s, , , , , ,",tempRow[0]);
					 write(fd,caLine,strlen(caLine));
					 write(fd,"\r\n",strlen("\r\n"));

		  	   memset(dateSourceSql,0,sizeof(dateSourceSql));
		  	   sprintf(dateSourceSql,"select a.service,sum(bytes) as flow,c.name,a.sdate,sum(onlinetime) as stime from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s) and a.userid=b.userid and b.groupid0=c.id and workflag=%s and a.userid=%s group by a.service,c.name,a.sdate order by flow desc limit %d offset 0",caSdate1,caSdate2,strTemp.s,work,tempRow[1],datenum);
			 		 if(gtype==1){
							memset(dateSourceSql,0,sizeof(dateSourceSql));
							sprintf(dateSourceSql,"select a.service,sum(bytes) as flow,c.name,a.sdate,sum(onlinetime) as stime from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s and a.userid=%s group by a.service,b.username,c.name,a.sdate order by flow desc limit %d offset 0",caSdate1,caSdate2,work,tempRow[1],datenum);
			 		 }
					 printf("\n DateSourceSql  %s\n",dateSourceSql);
					 //二次查询
				   tempCursor1 = ltDbOpenCursor(G_DbCon1,dateSourceSql);
					 tempRow1=ltDbFetchRow(tempCursor1);
					 while(tempRow1!=NULL){
					 			 if(atoll(tempRow1[1])>1024000){
             			 sprintf(flow,"%0.3fM",atoll(tempRow1[1])/1024000.00);
			           }else{
			               sprintf(flow,"%0.3fKB",atoll(tempRow1[1])/1024.00);
			           }
			           memset(caLine,0,sizeof(caLine));
			           memset(sTime,0,sizeof(sTime));
		             sprintf(sTime,"%ld",atol(tempRow1[4])/3600);
						     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])/60);
						     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])%60);
								 sprintf(caLine," ,%s,%s,%s,%s,%s",
													_ltPubInfo->topSrvName[atoi(tempRow1[0])].srvname,
													tempRow1[2],
													flow,
													tempRow1[3],
													sTime
								 );
								 write(fd,caLine,strlen(caLine));
								 write(fd,"\r\n",strlen("\r\n"));
								 tempRow1=ltDbFetchRow(tempCursor1);
					 }
					 ltDbCloseCursor(tempCursor1);
	  	  }else if(type==4){//出口速率分布
	  	  	  if(groupby==0){//小时
	  	  	  	  lt_dbput_recordvars(tablePtr,2,
					   			"flowbps",LT_TYPE_LONGLONG,atoll(tempRow[1])/60,
					   			"onlinetime",LT_TYPE_STRING,tempRow[0]
   		   		 		);
	  	  	  }else if(groupby==1){//天
	  	  	  	  lt_dbput_recordvars(tablePtr,2,
					   			"flowbps",LT_TYPE_LONGLONG,atoll(tempRow[1])/60,
					   			"onlinetime",LT_TYPE_STRING,tempRow[0]
   		   		 		);
	  	  	  }
						memset(caLine,0,sizeof(caLine));
						sprintf(caLine,"%s,%lld",
											tempRow[0],
											atoll(tempRow[1])/60
						);
						write(fd,caLine,strlen(caLine));
						write(fd,"\r\n",strlen("\r\n"));
	  	  }else if(type==5){
	  	  		memset(caLine,0,sizeof(caLine));
					  sprintf(caLine,"%s, , , , , ,",tempRow[0]);
					  write(fd,caLine,strlen(caLine));
					  write(fd,"\r\n",strlen("\r\n"));

			  	  memset(dateSourceSql,0,sizeof(dateSourceSql));
			  	  sprintf(dateSourceSql,"select b.dispname,c.name,a.service,sum(bytes) as flow,sum(onlinetime) as stime from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s) and a.userid=b.userid and b.groupid0=c.id and workflag=%s and a.sdate='%s' group by b.dispname,c.name,a.service order by flow desc limit %d offset 0",caSdate1,caSdate2,strTemp.s,work,tempRow[0],datenum);
				 		if(gtype==1){
							memset(dateSourceSql,0,sizeof(dateSourceSql));
							sprintf(dateSourceSql,"select b.dispname,c.name,a.service,sum(bytes) as flow,sum(onlinetime) as stime from msasrvreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and a.userid=b.userid and b.groupid0=c.id and workflag=%s and a.sdate='%s' group by b.dispname,c.name,a.service order by flow desc limit %d offset 0",caSdate1,caSdate2,work,tempRow[0],datenum);
				 		}
						printf("\n DateSourceSql  %s\n",dateSourceSql);
						//二次查询
					  tempCursor1 = ltDbOpenCursor(G_DbCon1,dateSourceSql);
						tempRow1=ltDbFetchRow(tempCursor1);
						while(tempRow1!=NULL){
						 			 if(atoll(tempRow1[3])>1024000){
	             			 sprintf(flow,"%0.3fM",atoll(tempRow1[3])/1024000.00);
				           }else{
				               sprintf(flow,"%0.3fKB",atoll(tempRow1[3])/1024.00);
				           }
				           memset(caLine,0,sizeof(caLine));
				           memset(sTime,0,sizeof(sTime));
			             sprintf(sTime,"%ld",atol(tempRow1[4])/3600);
							     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])/60);
							     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])%60);
									 sprintf(caLine," ,%s,%s,%s,%s,%s",
														tempRow1[0],
														tempRow1[1],
														_ltPubInfo->topSrvName[atoi(tempRow1[2])].srvname,
														flow,
														sTime
									 );
									 write(fd,caLine,strlen(caLine));
									 write(fd,"\r\n",strlen("\r\n"));
									 tempRow1=ltDbFetchRow(tempCursor1);
						}
						ltDbCloseCursor(tempCursor1);
	  	  }else if(type==6){
	  	  		 if(atoll(tempRow[1])>1024000){
               	 sprintf(flow,"%0.3fM",atoll(tempRow[1])/1024000.00);
	            }else{
	               sprintf(flow,"%0.3fKB",atoll(tempRow[1])/1024.00);
	            }

							memset(caLine,0,sizeof(caLine));
							sprintf(caLine,"%s,%s",
												tempRow[0],
												flow
							);
							write(fd,caLine,strlen(caLine));
							write(fd,"\r\n",strlen("\r\n"));
	  	  }else if(type==7){
	  	  		 memset(caLine,0,sizeof(caLine));
						 sprintf(caLine,"%s, , , , , ,",tempRow[0]);
						 write(fd,caLine,strlen(caLine));
						 write(fd,"\r\n",strlen("\r\n"));

			  	   memset(dateSourceSql,0,sizeof(dateSourceSql));
			  	   sprintf(dateSourceSql,"select c.name,a.host,sum(bytes) as abyte,sum(lcount) as allcount,sum(ctime) as allctime from msauserhttpreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s' and b.groupid0 in(%s)%s and a.userid=b.userid and b.groupid0=c.id and workflag=%s and a.userid=%s group by a.host,b.dispname,c.name order by abyte desc limit %d offset 0",caSdate1,caSdate2,strTemp.s,urlsortstr,work,tempRow[1],datenum);
						 if(gtype==1){
								memset(dateSourceSql,0,sizeof(dateSourceSql));
								sprintf(dateSourceSql,"select c.name,a.host,sum(bytes) as abyte,sum(lcount) as allcount,sum(ctime) as allctime from msauserhttpreport a,msauser b ,msagroup c where sdate>='%s' and sdate<='%s'%s and a.userid=b.userid and b.groupid0=c.id and workflag=%s and a.userid=%s group by a.host,b.dispname,c.name order by abyte desc limit %d offset 0",caSdate1,caSdate2,urlsortstr,work,tempRow[1],datenum);
						 }
						 printf("\n DateSourceSql  %s\n",dateSourceSql);
						 //二次查询
					   tempCursor1 = ltDbOpenCursor(G_DbCon1,dateSourceSql);
						 tempRow1=ltDbFetchRow(tempCursor1);
						 while(tempRow1!=NULL){
						 			 if(atoll(tempRow1[2])>1024000){
               			 sprintf(flow,"%0.3fM",atoll(tempRow1[2])/1024000.00);
				           }else{
				               sprintf(flow,"%0.3fKB",atoll(tempRow1[2])/1024.00);
				           }
				           memset(caLine,0,sizeof(caLine));
				           memset(sTime,0,sizeof(sTime));
			             sprintf(sTime,"%ld",atol(tempRow1[4])/3600);
							     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])/60);
							     sprintf(sTime,"%s:%ld",sTime,atol(tempRow1[4])%60);
									 sprintf(caLine," ,%s,%s,%s,%s,%s",
														tempRow1[1],
														tempRow1[0],
														flow,
														tempRow1[3],
														sTime
									 );
									 write(fd,caLine,strlen(caLine));
									 write(fd,"\r\n",strlen("\r\n"));
									 tempRow1=ltDbFetchRow(tempCursor1);
						 }
						 ltDbCloseCursor(tempCursor1);
	  	  }
    		tempRow=ltDbFetchRow(tempCursor);
    }
    ltDbCloseCursor(tempCursor);
    close(fd);
		switch(type){
	      case 1:
				sprintf(tojava,"%s sortSourceExcel %s %ssource.xls %d",JAVACMD,caFile,caTempDir,1);
				chdir(BINCMD);
				printf("JAVA %s\n",tojava);
				system(tojava);
	      break;
	      case 2:
				sprintf(tojava,"%s sortSourceExcel %s %ssource.xls %d",JAVACMD,caFile,caTempDir,2);
				chdir(BINCMD);
				printf("JAVA： %s\n",tojava);
				system(tojava);
	      break;
	      case 3:
				sprintf(tojava,"%s sortSourceExcel %s %ssource.xls %d",JAVACMD,caFile,caTempDir,3);
				chdir(BINCMD);
				printf("JAVA： %s\n",tojava);
				system(tojava);
	      break;
	      case 4:
	      strgb2utf8("出口速率分布 时间 速率",utf8Str,strlen("出口速率分布 时间 速率"));
				sprintf(tojava,"%s sortReportExcel %s %ssource.xls %s(kbps)",JAVACMD,caFile,caTempDir,utf8Str);
				chdir(BINCMD);
				printf("JAVA： %s\n",tojava);
				system(tojava);
	      break;
	      case 5:
				sprintf(tojava,"%s sortSourceExcel %s %ssource.xls %d",JAVACMD,caFile,caTempDir,5);
				chdir(BINCMD);
				printf("JAVA： %s\n",tojava);
				system(tojava);
	      break;
	      case 6:
	      strgb2utf8("网站分类排序 网站类型 流量",utf8Str,strlen("网站分类排序 网站类型 流量"));
				sprintf(tojava,"%s sortReportExcel %s %ssource.xls %s",JAVACMD,caFile,caTempDir,utf8Str);
				chdir(BINCMD);
				printf("JAVA： %s\n",tojava);
				system(tojava);
	      break;
	      case 7:
			  sprintf(tojava,"%s sortSourceExcel %s %ssource.xls %d",JAVACMD,caFile,caTempDir,7);
			  chdir(BINCMD);
			  system(tojava);
	      break;
		}
		printf("JAVA   %s\n",tojava);
		chdir(caTempDir);
    system("rm -f report.tgz");
	  system("tar -cvzf report.tgz *.jpg *.gif *.htm");
	  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	  system(caLabel);
    ltDbClose(G_DbCon);
    ltDbClose(G_DbCon1);
		ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"1");
	  ltMsgFree(ltMsgPk);
	  lt_dbfree(dbPtr);
    return 0;
}
