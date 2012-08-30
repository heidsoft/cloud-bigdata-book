#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
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
#include <dirent.h>
#include "msa.h"
#define MAX_HTTPACESS_TYPE  235
#define JAVACMD "/app/java/jdk1.6.0_01/bin/java -Duser.language=zh_CN.UTF-8  -Dfile.encoding=UTF-8 -cp /app/java/poi/poi-3.7-20101029.jar:./ "
#define BINCMD	"/app/msa/bin"
int nasRNCvtData(char *pIn,unsigned lBytes);
int nasRNCvtData(char *pIn,unsigned lBytes)
{
    register int i;
    i=0;
    while(i<lBytes) {
        switch (pIn[i]) {
            case '\r':
                pIn[i] = '-';
                break;
            case '\'':
                pIn[i] = '-';
                break;
            case '\"':
                pIn[i] = '-';
                break;
            case '\n':
                pIn[i] = '-';
                break;
            case '&':
                pIn[i] = '\0';
                return 0;
            case '?':
                pIn[i] = '\0';
                return 0;
            case ';':
                pIn[i] = '\0';
                return 0;
            default:
                break;
        }
        i++;
    }
    return 0;
}


//MSA:数据中心页面链接调用函数
int msaShowAccessList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  checkTimeOut(confd,ltMsgPk,lt_MMHead);
	  ltDbHeadPtr dbPtr;
	 	ltTablePtr  tablePtr;
	 	int iii;
		int    showtype=0;
	  long  lTime;
	  char *caSdate=NULL;
	  char sDate[36];
	  char eDate[36];
	  char selDate[36];
	  char selTime[36];

    memset(sDate,0,sizeof(sDate));
    memset(eDate,0,sizeof(eDate));
    memset(selDate,0,sizeof(selDate));
    memset(selTime,0,sizeof(selTime));

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

	  if(ltMsgGetVar_s(ltMsgPk,"showtype")){
	 		showtype=atoi(ltMsgGetVar_s(ltMsgPk,"showtype"));
	  }

	  if(ltMsgGetVar_s(ltMsgPk,"sdate")){
	 		caSdate=ltMsgGetVar_s(ltMsgPk,"sdate");
	  }
    dbPtr=lt_dbinit();
	  lt_db_htmlpage(dbPtr,"utf-8");
	  lTime = time(0);
	  nasTimeGetDate(sDate,time(0)-24*3600);//把当前时间转换为  12-03-16
	  nasTimeGetDate(eDate,time(0)+24*3600);//把当前时间转换为  12-03-16
	  //strcpy(sDate,ltTimeFormat("%Y-%m-%d",time(0)-24*3600));
	  //strcpy(eDate,ltTimeFormat("%Y-%m-%d",time(0)+24*3600));
	  nasCvtStime(lTime,selDate,selTime);
	  if(selTime[0]=='0'){
	  	selTime[0]=selTime[1];
	  	selTime[1]='\0';
	  }else{
	    selTime[2]='\0';
	  }
	  //if(showtype!=3){
				if(caSdate==NULL){
					lt_dbput_rootvars(dbPtr,1,"sdate",sDate);
					lt_dbput_rootvars(dbPtr,1,"edate",eDate);
				}else{
					lt_dbput_rootvars(dbPtr,1,"sdate",sDate);
				}
		    lt_dbput_rootvars(dbPtr,1,"stime",selTime);
		//}
	  //虚拟通道
	  tablePtr=lt_dbput_table(dbPtr,"vpathlist");
	  for(iii=0;iii<_MAXVPATHNUM;iii++){
	   	 if(strlen(_ltPubInfo->_VPathList[iii].vPathName)>0){
	   	 	lt_dbput_recordvars(tablePtr,2,"vpathid",LT_TYPE_LONG,iii,"vpathname",LT_TYPE_STRING,_ltPubInfo->_VPathList[iii].vPathName);
	   	 }else{
	   	   break;
	   	 }

	  }

	  ltMsgPk->msgpktype=1;
	  if(showtype==1){
		 	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/AccessList.htm",dbPtr->head,0);
		}else if(showtype==2){
		 	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/AlertList.htm",dbPtr->head,0);
		}else if(showtype==3){
			char dd[16];
			nasTimeGetDate(dd,time(0));
			//lt_dbput_rootvars(dbPtr,1,"sdate",dd);
			//lt_dbput_rootvars(dbPtr,1,"sdate",sDate);
		 	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/datacenter/AccountList.htm",dbPtr->head,0);
		}
		ltMsgFree(ltMsgPk);
  	lt_dbfree(dbPtr);
	 return 0;
}

int msaAccessList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  checkTimeOut(confd,ltMsgPk,lt_MMHead);
	  char strBuf[1024];
	  char caWhere[512];
	  ltDbCursor *tempCursor;
	  LT_DBROW tempRow;
	  char caTmpp[1024];
	  long lRowNum,lStartRec,lSumRec,lCount,lNum;
	  int k;
	  stralloc strTemp;
		ltDbHeadPtr dbPtr;
	  dbPtr=lt_dbinit();
	  lt_db_htmlpage(dbPtr,"utf-8");
    //根据传的参数判断删除以及执行删除
    char protocol[10];
    char fangX[10];
		char *sdate;
		char *edate;
		char *stime;
		char *etime;
		char groupname[120];
		char topSrvName[120];
		char pName[120];
		char *keyword;
		char *sip;
		char *mip;
		char *sport;
		char *mport;
		char *xntd;
		char *evttype;
		char *evtlevel;
		char *evtres;

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

	  if(ltMsgGetVar_s(ltMsgPk,"sdate"))
		{
	    sdate=ltMsgGetVar_s(ltMsgPk,"sdate");
	  }else{
	  	sdate=NULL;
	  }
	  if(ltMsgGetVar_s(ltMsgPk,"stime"))
		{
	    stime=ltMsgGetVar_s(ltMsgPk,"stime");
	  }else{
	  	stime=NULL;
	  }
	  if(ltMsgGetVar_s(ltMsgPk,"edate"))
		{
	    edate=ltMsgGetVar_s(ltMsgPk,"edate");
	  }else{
	  	edate=NULL;
	  }
	  if(ltMsgGetVar_s(ltMsgPk,"etime"))
		{
	    etime=ltMsgGetVar_s(ltMsgPk,"etime");
	  }else{
	  	etime=NULL;
	  }

		memset(caWhere,0,sizeof(caWhere));
		sprintf(caWhere,"%s"," 1=1 ");

		keyword=ltMsgGetVar_s(ltMsgPk,"keyword");
    if(keyword && keyword[0] ){
    	sprintf(caWhere,"%s and keyword like '%c%s%c'",caWhere,'%',keyword,'%');
    }

		sport=ltMsgGetVar_s(ltMsgPk,"sport");
    if(sport && sport[0] ){
    	sprintf(caWhere,"%s and sport='%s'",caWhere,sport);
    }

		mport=ltMsgGetVar_s(ltMsgPk,"mport");
    if(mport && mport[0] ){
    	sprintf(caWhere,"%s and mport='%s'",caWhere,mport);
    }

		sip=ltMsgGetVar_s(ltMsgPk,"sip");
    if(sip && sip[0] ){
    	sprintf(caWhere,"%s and sip='%s'",caWhere,sip);
    }

		mip=ltMsgGetVar_s(ltMsgPk,"mip");
    if(mip && mip[0] ){
    	sprintf(caWhere,"%s and mip='%s'",caWhere,mip);
    }

		xntd=ltMsgGetVar_s(ltMsgPk,"xntd");
    if(xntd && xntd[0] ){
    	if(!(atoi(xntd)==-1)){
    		sprintf(caWhere,"%s and xntd='%s'",caWhere,xntd);
    	}
    }

		evtres=ltMsgGetVar_s(ltMsgPk,"evtres");
    if(evtres && evtres[0] ){
    	if(!(atoi(evtres)==-1)){
    		sprintf(caWhere,"%s and evtres='%s'",caWhere,evtres);
    	}
    }

		evtlevel=ltMsgGetVar_s(ltMsgPk,"evtlevel");
    if(evtlevel && evtlevel[0] ){
    	if(!(atoi(evtlevel)==-1)){
    		sprintf(caWhere,"%s and evtlevel='%s'",caWhere,evtlevel);
    	}
    }

		evttype=ltMsgGetVar_s(ltMsgPk,"evttype");
    if(evttype && evttype[0] ){
    	if(!(atoi(evttype)==-1)){
    		sprintf(caWhere,"%s and evttype='%s'",caWhere,evttype);
    	}
    }
    //服务
    int selsrv=-1;
    int twoselsrv=-1;
    if(ltMsgGetVar_s(ltMsgPk,"selsrv")!=NULL){
    		selsrv=atoi(ltMsgGetVar_s(ltMsgPk,"selsrv"));
    }
    if(ltMsgGetVar_s(ltMsgPk,"twoselsrv")!=NULL){
    		twoselsrv=atoi(ltMsgGetVar_s(ltMsgPk,"twoselsrv"));
    }
    if(selsrv!=-1){
    		sprintf(caWhere,"%s and topsrvname=%d",caWhere,selsrv);
    }
    if(selsrv!=-1&&twoselsrv!=-1){
    	sprintf(caWhere,"%s and srvname=%d",caWhere,twoselsrv);
    }
//printf("caWhere:%s\n",caWhere);
		char groupname0[128],groupname1[128],groupname2[128],groupname3[128],groupname4[128];
		char myevttype[128],myevtlevel[128],myevtres[128],myfangx[128],myprotocol[128],mysip[128],mydip[128],mysport[128],mydport[128],mymac[128],mysdate[128],mystime[128],myupflow[128],mydownflow[128],mypacknum[128],mytopsrvname[128],mysrvname[128],myxntd[128],mykeyword[128];
		memset(groupname0,0,sizeof(groupname0));
		memset(groupname1,0,sizeof(groupname1));
		memset(groupname2,0,sizeof(groupname2));
		memset(groupname3,0,sizeof(groupname3));
		memset(groupname4,0,sizeof(groupname4));
		memset(myevttype,0,sizeof(myevttype));
		memset(myevtlevel,0,sizeof(myevtlevel));
		memset(myevtres,0,sizeof(myevtres));
		memset(mykeyword,0,sizeof(mykeyword));
		if(ltMsgGetVar_s(ltMsgPk,"flag")){
	    if(strcmp(ltMsgGetVar_s(ltMsgPk,"flag"),"1")==0){/*导出到excel*/
	    	 char caFileName[128];
	       sprintf(caFileName,"accesslistexport-%ld.csv",time(0));
		     k=0;
		     ltMsgPk->msgpktype=2;
				 lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,caFileName,"contenttype","Application/MS-DOWNLOAD");
			   if(stime!=NULL){
			    	sprintf(strBuf,"select a.username,a.dispname,a.groupid0,a.groupid1,a.groupid2,a.groupid3,a.groupid4,b.evtid,b.userid,b.evttype,b.evtlevel,b.evtres,b.fangx,b.protocol,b.sip,b.dip,b.sport,b.dport,b.mac,b.sdate,b.stime,b.upflow,b.downflow,b.packnum,b.topsrvname,c.srvname,b.xntd,b.keyword from msauser a left join msaAccessList b on a.userid=b.userid left join nassrv c on b.srvname=c.srvid where %s and sdate='%s' and stime>='%s' and stime<'%s'", caWhere,sdate,stime,etime);
			   }else{
			    	sprintf(strBuf,"select a.username,a.dispname,a.groupid0,a.groupid1,a.groupid2,a.groupid3,a.groupid4,b.evtid,b.userid,b.evttype,b.evtlevel,b.evtres,b.fangx,b.protocol,b.sip,b.dip,b.sport,b.dport,b.mac,b.sdate,b.stime,b.upflow,b.downflow,b.packnum,b.topsrvname,c.srvname,b.xntd,b.keyword from msauser a left join msaAccessList b on a.userid=b.userid left join nassrv c on b.srvname=c.srvid where %s and sdate>'%s' and sdate<'%s'", caWhere,sdate,edate);
			   }
//printf("strBuf_export:%s\n",strBuf);
			   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
			   if(tempCursor!=NULL){
						tempRow= ltDbFetchRow(tempCursor);
					  while(tempRow!=NULL){
							if(_ltPubInfo->_GroupList[atoi(tempRow[2])].name!=NULL){
								sprintf(groupname0,"%s",_ltPubInfo->_GroupList[atoi(tempRow[2])].name);
		 						if(_ltPubInfo->_GroupList[atoi(tempRow[3])].name!=NULL&&atoi(tempRow[3])!=0){
		 							sprintf(groupname1,"%s",_ltPubInfo->_GroupList[atoi(tempRow[3])].name);
		 							if(_ltPubInfo->_GroupList[atoi(tempRow[4])].name!=NULL&&atoi(tempRow[4])!=0){
		 								sprintf(groupname2,"%s",_ltPubInfo->_GroupList[atoi(tempRow[4])].name);
		 								if(_ltPubInfo->_GroupList[atoi(tempRow[5])].name!=NULL&&atoi(tempRow[5])!=0){
		 									sprintf(groupname3,"%s",_ltPubInfo->_GroupList[atoi(tempRow[5])].name);
		 									if(_ltPubInfo->_GroupList[atoi(tempRow[6])].name!=NULL&&atoi(tempRow[6])!=0){
		 										sprintf(groupname4,"%s",_ltPubInfo->_GroupList[atoi(tempRow[6])].name);
		 									}else{
												sprintf(groupname4,"%s","");
											}
		 								}else{
											sprintf(groupname3,"%s","");
											sprintf(groupname4,"%s","");
										}
		 							}else{
										sprintf(groupname2,"%s","");
										sprintf(groupname3,"%s","");
										sprintf(groupname4,"%s","");
									}
		 						}else{
									sprintf(groupname1,"%s","");
									sprintf(groupname2,"%s","");
									sprintf(groupname3,"%s","");
									sprintf(groupname4,"%s","");
								}
							}else{
								sprintf(groupname0,"%s","");
								sprintf(groupname1,"%s","");
								sprintf(groupname2,"%s","");
								sprintf(groupname3,"%s","");
								sprintf(groupname4,"%s","");
							}
							//evttype 1正常上网时间  2 阻断事件  3虚拟身份活动事件   4搜索引擎搜索事件
							if(atoi(tempRow[9])==1){
								strgb2utf8("正常上网时间",myevttype,strlen("正常上网时间"));
							}else if(atoi(tempRow[9])==2){
								strgb2utf8("阻断事件",myevttype,strlen("阻断事件"));
							}else if(atoi(tempRow[9])==3){
								strgb2utf8("虚拟身份活动事件",myevttype,strlen("虚拟身份活动事件"));
							}else if(atoi(tempRow[9])==4){
								strgb2utf8("搜索引擎搜索事件",myevttype,strlen("搜索引擎搜索事件"));
							}
							//evtlevel 1一般访问记录  2 阻断事件   3 高网络影响事件
							if(atoi(tempRow[10])==1){
								strgb2utf8("一般访问记录",myevtlevel,strlen("一般访问记录"));
							}else if(atoi(tempRow[10])==2){
								strgb2utf8("阻断事件",myevtlevel,strlen("阻断事件"));
							}else if(atoi(tempRow[10])==3){
								strgb2utf8("网络影响事件",myevtlevel,strlen("网络影响事件"));
							}
							sprintf(myevtres,"%s",tempRow[11]);
							//b.mac,b.sdate,b.stime,b.upflow,b.downflow,b.packnum,b.topsrvname,c.srvname,b.xntd,b.keyword
							if(atoi(tempRow[13])==0){
								strgb2utf8("TCP",myprotocol,strlen("TCP"));
							}else if(atoi(tempRow[13])==1){
								strgb2utf8("UDP",myprotocol,strlen("UDP"));
							}
							sprintf(mytopsrvname,"%s",_ltPubInfo->topSrvName[atoi(tempRow[24])].srvname);
							sprintf(mysrvname,"%s",_ltPubInfo->ltService[atoi(tempRow[25])].srvName);
							sprintf(myxntd,"%s",_ltPubInfo->_VPathList[atoi(tempRow[26])].vPathName);
							sprintf(mykeyword,"%s",tempRow[27]);
							sprintf(caTmpp,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",tempRow[1],groupname0,groupname1,groupname2,groupname3,groupname4,tempRow[7],myevttype,myevtlevel,myprotocol,tempRow[14],tempRow[15],tempRow[16],tempRow[17],tempRow[18],tempRow[19],tempRow[20],tempRow[21],tempRow[22],tempRow[23],mytopsrvname,mysrvname,myxntd,mykeyword);
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
	  }

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

	  if(stime!=NULL){
    	sprintf(strBuf,"select count(*) from msauser a left join msaAccessList b on a.userid=b.userid left join nassrv c on b.srvname=c.srvid where %s and sdate='%s' and stime>='%s' and stime<'%s' ", caWhere,sdate,stime,etime);
    }else{
    	sprintf(strBuf,"select count(*) from msauser a left join msaAccessList b on a.userid=b.userid left join nassrv c on b.srvname=c.srvid where %s and sdate>'%s' and sdate<'%s' ", caWhere,sdate,edate);
    }
//printf("strBuf:%s\n",strBuf);

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
    if(stime!=NULL){
    	sprintf(strBuf,"select a.username,a.dispname,a.groupid0,a.groupid1,a.groupid2,a.groupid3,a.groupid4,b.evtid,b.userid,b.evttype,b.evtlevel,b.evtres,b.fangx,b.protocol,b.sip,b.dip,b.sport,b.dport,b.mac,b.sdate,b.stime,b.upflow,b.downflow,b.packnum,b.topsrvname,c.srvname,b.xntd,b.keyword from msauser a left join msaAccessList b on a.userid=b.userid left join nassrv c on b.srvname=c.srvid where %s and sdate='%s' and stime>='%s' and stime<'%s' limit %lu offset %lu", caWhere,sdate,stime,etime,lRowNum,lStartRec);
    }else{
    	sprintf(strBuf,"select a.username,a.dispname,a.groupid0,a.groupid1,a.groupid2,a.groupid3,a.groupid4,b.evtid,b.userid,b.evttype,b.evtlevel,b.evtres,b.fangx,b.protocol,b.sip,b.dip,b.sport,b.dport,b.mac,b.sdate,b.stime,b.upflow,b.downflow,b.packnum,b.topsrvname,c.srvname,b.xntd,b.keyword from msauser a left join msaAccessList b on a.userid=b.userid left join nassrv c on b.srvname=c.srvid where %s and sdate>'%s' and sdate<'%s'  limit %lu offset %lu ", caWhere,sdate,edate,lRowNum,lStartRec);
    }
//printf("strBuf:%s\n",strBuf);
		char mykey[512];

	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {
					if(atoi(tempRow[12])==0){
						sprintf(fangX,"%s","->");
					}else if(atoi(tempRow[12])==1){
						sprintf(fangX,"%s","<-");
					}else{
						sprintf(fangX,"%s",tempRow[12]);
					}
					if(atoi(tempRow[13])==0){
						sprintf(protocol,"%s","UDP");
					}else{
						sprintf(protocol,"%s","TCP");
					}
					if(_ltPubInfo->_GroupList[atoi(tempRow[2])].name!=NULL){
						sprintf(groupname,"%s",_ltPubInfo->_GroupList[atoi(tempRow[2])].name);
 						if(_ltPubInfo->_GroupList[atoi(tempRow[3])].name!=NULL&&atoi(tempRow[3])!=0){
 							sprintf(groupname,"%s/%s",groupname,_ltPubInfo->_GroupList[atoi(tempRow[3])].name);
 							if(_ltPubInfo->_GroupList[atoi(tempRow[4])].name!=NULL&&atoi(tempRow[4])!=0){
 								sprintf(groupname,"%s/%s",groupname,_ltPubInfo->_GroupList[atoi(tempRow[4])].name);
 								if(_ltPubInfo->_GroupList[atoi(tempRow[5])].name!=NULL&&atoi(tempRow[5])!=0){
 									sprintf(groupname,"%s/%s",groupname,_ltPubInfo->_GroupList[atoi(tempRow[5])].name);
 									if(_ltPubInfo->_GroupList[atoi(tempRow[6])].name!=NULL&&atoi(tempRow[6])!=0){
 										sprintf(groupname,"%s/%s",groupname,_ltPubInfo->_GroupList[atoi(tempRow[6])].name);
 									}
 								}
 							}
 						}
					}
					sprintf(mykey,"%s",tempRow[27]);
					nasRNCvtData(mykey,strlen(mykey));
					sprintf(topSrvName,"%s",_ltPubInfo->topSrvName[atoi(tempRow[24])].srvname);
					sprintf(pName,"%s",_ltPubInfo->_VPathList[atoi(tempRow[26])].vPathName);
//printf("mykey:%s\n",mykey);
          if(k==0)
					{
            //sprintf(caTmpp,"{\"username\":\"%s\",\"dispname\":\"%s\",\"groupname\":\"%s\",\"evtid\":\"%s\",\"userid\":\"%s\",\"evttype\":\"%s\",\"evtlevel\":\"%s\",\"evtres\":\"%s\",\"fangx\":\"%s&%s@%s(%s)%s#%s!%s^%s*%s[%s]%s{%s}%s|%s=\",\"mac\":\"%s\",\"stime\":\"%s %s\",\"upflow\":\"%s\",\"downflow\":\"%s\",\"packnum\":\"%s\",\"topsrvname\":\"%s\",\"srvname\":\"%s\",\"xntd\":\"%s\",\"keyword\":\"%s\"}",
            //tempRow[0],tempRow[1],groupname,tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],protocol,tempRow[14],tempRow[16],fangX,tempRow[15],tempRow[17],tempRow[21],tempRow[22],tempRow[23],topSrvName,tempRow[25],pName,tempRow[27],tempRow[9],tempRow[18],tempRow[19],tempRow[20],tempRow[21],tempRow[22],tempRow[23],topSrvName,tempRow[25],pName,tempRow[27]);
            sprintf(caTmpp,"{\"username\":\"%s\",\"dispname\":\"%s\",\"groupname\":\"%s\",\"evtid\":\"%s\",\"userid\":\"%s\",\"evttype\":\"%s\",\"evtlevel\":\"%s\",\"evtres\":\"%s\",\"fangx\":\"%s&%s@%s(%s)%s#%s!%s^%s*%s[%s]%s{%s}%s|%s=\",\"mac\":\"%s\",\"stime\":\"%s %s\"}",
            tempRow[0],tempRow[1],groupname,tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],protocol,tempRow[14],tempRow[16],fangX,tempRow[15],tempRow[17],tempRow[21],tempRow[22],tempRow[23],topSrvName,tempRow[25],pName,mykey,tempRow[9],tempRow[18],tempRow[19],tempRow[20]);
            stralloc_cats(&strTemp,caTmpp);
//printf("caTmpp1:%s\n",caTmpp);
					}
					else
					{
				    //sprintf(caTmpp,",{\"username\":\"%s\",\"dispname\":\"%s\",\"groupname\":\"%s\",\"evtid\":\"%s\",\"userid\":\"%s\",\"evttype\":\"%s\",\"evtlevel\":\"%s\",\"evtres\":\"%s\",\"fangx\":\"%s&%s@%s(%s)%s#%s!%s^%s*%s[%s]%s{%s}%s|%s=\",\"mac\":\"%s\",\"stime\":\"%s %s\",\"upflow\":\"%s\",\"downflow\":\"%s\",\"packnum\":\"%s\",\"topsrvname\":\"%s\",\"srvname\":\"%s\",\"xntd\":\"%s\",\"keyword\":\"%s\"}",
				    //tempRow[0],tempRow[1],groupname,tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],protocol,tempRow[14],tempRow[16],fangX,tempRow[15],tempRow[17],tempRow[21],tempRow[22],tempRow[23],topSrvName,tempRow[25],pName,tempRow[27],tempRow[9],tempRow[18],tempRow[19],tempRow[20],tempRow[21],tempRow[22],tempRow[23],topSrvName,tempRow[25],pName,tempRow[27]);
				    sprintf(caTmpp,",{\"username\":\"%s\",\"dispname\":\"%s\",\"groupname\":\"%s\",\"evtid\":\"%s\",\"userid\":\"%s\",\"evttype\":\"%s\",\"evtlevel\":\"%s\",\"evtres\":\"%s\",\"fangx\":\"%s&%s@%s(%s)%s#%s!%s^%s*%s[%s]%s{%s}%s|%s=\",\"mac\":\"%s\",\"stime\":\"%s %s\"}",
				    tempRow[0],tempRow[1],groupname,tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],protocol,tempRow[14],tempRow[16],fangX,tempRow[15],tempRow[17],tempRow[21],tempRow[22],tempRow[23],topSrvName,tempRow[25],pName,mykey,tempRow[9],tempRow[18],tempRow[19],tempRow[20]);
				    //用户     用户       部门      事件id      审计源id   事件类型    事件级别    事件内容   // 协议   SIP         Sport     方向12   dip       dport         上流量    下流量        数据包    一级服务24   二级服务  虚拟通道26 关键字  事件类型//  mac        日期        时间       上流量        下流量     数据包      一级服务   二级服务   虚拟通道  关键字
 		        stralloc_cats(&strTemp,caTmpp);
					}
					tempRow= ltDbFetchRow(tempCursor);
					k++;
		    }
       ltDbCloseCursor(tempCursor);
    }
    stralloc_cats(&strTemp,"]}");
    stralloc_0(&strTemp);
//printf("strTemp:%s\n",strTemp.s);
    ltMsgPk->msgpktype=1;
    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
    ltMsgFree(ltMsgPk);
    ltDbClose(G_DbCon);
    return 0;
}

int msaAlertList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  checkTimeOut(confd,ltMsgPk,lt_MMHead);
	  char strBuf[1024];
	  char caWhere[512];
	  ltDbCursor *tempCursor;
	  LT_DBROW tempRow;
	  char caTmpp[512];
	  long lRowNum,lStartRec,lSumRec,lCount,lNum;
	  int k;
	  stralloc strTemp;
		ltDbHeadPtr dbPtr;
	  dbPtr=lt_dbinit();
	  lt_db_htmlpage(dbPtr,"utf-8");
    //根据传的参数判断删除以及执行删除
    char *sdate;
		char *edate;
		char *stime;
		char *etime;
		char *evttype;
		char *evtlevel;
		char *evtres;

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

	  if(ltMsgGetVar_s(ltMsgPk,"sdate"))
		{
	    sdate=ltMsgGetVar_s(ltMsgPk,"sdate");
	  }else{
	  	sdate=NULL;
	  }
	  if(ltMsgGetVar_s(ltMsgPk,"stime"))
		{
	    stime=ltMsgGetVar_s(ltMsgPk,"stime");
	  }else{
	  	stime=NULL;
	  }
	  if(ltMsgGetVar_s(ltMsgPk,"edate"))
		{
	    edate=ltMsgGetVar_s(ltMsgPk,"edate");
	  }else{
	  	edate=NULL;
	  }
	  if(ltMsgGetVar_s(ltMsgPk,"etime"))
		{
	    etime=ltMsgGetVar_s(ltMsgPk,"etime");
	  }else{
	  	etime=NULL;
	  }

		memset(caWhere,0,sizeof(caWhere));
		sprintf(caWhere,"%s"," 1=1 ");

		evtres=ltMsgGetVar_s(ltMsgPk,"evtres");
    if(evtres && evtres[0] ){
    	if(!(atoi(evtres)==-1)){
    		sprintf(caWhere,"%s and evtres='%s'",caWhere,evtres);
    	}
    }

		evtlevel=ltMsgGetVar_s(ltMsgPk,"evtlevel");
    if(evtlevel && evtlevel[0] ){
    	if(!(atoi(evtlevel)==-1)){
    		sprintf(caWhere,"%s and evtlevel='%s'",caWhere,evtlevel);
    	}
    }

		evttype=ltMsgGetVar_s(ltMsgPk,"evttype");
    if(evttype && evttype[0] ){
    	if(!(atoi(evttype)==-1)){
    		sprintf(caWhere,"%s and evttype='%s'",caWhere,evttype);
    	}
    }

printf("caWhere:%s\n",caWhere);
		char myevttype[128],myevtlevel[128],myevtres[128];
		if(ltMsgGetVar_s(ltMsgPk,"flag")){
	    if(strcmp(ltMsgGetVar_s(ltMsgPk,"flag"),"1")==0){/*导出到excel*/
	    	 char caFileName[128];
	       sprintf(caFileName,"AlertListexport-%ld.csv",time(0));
		     k=0;
		     ltMsgPk->msgpktype=2;
				 lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,caFileName,"contenttype","Application/MS-DOWNLOAD");
			   if(stime!=NULL){
			    	sprintf(strBuf,"select evtid,evtlevel,evttype,evtres,evtdesc,sdate,stime,sip from msaAlertList where %s and sdate='%s' and stime>='%s' and stime<'%s'", caWhere,sdate,stime,etime);
			   }else{
			    	sprintf(strBuf,"select evtid,evtlevel,evttype,evtres,evtdesc,sdate,stime,sip from msaAlertList where %s and sdate>'%s' and sdate<'%s'", caWhere,sdate,edate);
			   }
printf("strBuf_export:%s\n",strBuf);
			   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
			   if(tempCursor!=NULL){
						tempRow= ltDbFetchRow(tempCursor);
					  while(tempRow!=NULL){
							//evttype 1正常上网时间  2 阻断事件  3虚拟身份活动事件   4搜索引擎搜索事件
							if(atoi(tempRow[2])==1){
								strgb2utf8("正常上网时间",myevttype,strlen("正常上网时间"));
							}else if(atoi(tempRow[2])==2){
								strgb2utf8("阻断事件",myevttype,strlen("阻断事件"));
							}else if(atoi(tempRow[2])==3){
								strgb2utf8("虚拟身份活动事件",myevttype,strlen("虚拟身份活动事件"));
							}else if(atoi(tempRow[2])==4){
								strgb2utf8("搜索引擎搜索事件",myevttype,strlen("搜索引擎搜索事件"));
							}
							//evtlevel 1一般访问记录  2 阻断事件   3 高网络影响事件
							if(atoi(tempRow[1])==1){
								strgb2utf8("一般访问记录",myevtlevel,strlen("一般访问记录"));
							}else if(atoi(tempRow[1])==2){
								strgb2utf8("阻断事件",myevtlevel,strlen("阻断事件"));
							}else if(atoi(tempRow[1])==3){
								strgb2utf8("网络影响事件",myevtlevel,strlen("高网络影响事件"));
							}
							if(atoi(tempRow[3])==1){
								strgb2utf8("成功",myevtres,strlen("成功"));
							}else if(atoi(tempRow[3])==2){
								strgb2utf8("失败",myevtres,strlen("失败"));
							}
				     	sprintf(caTmpp,"%s,%s,%s,%s,%s,%s,%s,%s\n",tempRow[0],myevtlevel,myevttype,myevtres,tempRow[4],tempRow[5],tempRow[6],tempRow[7]);
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
	  }

	  if(stime!=NULL){
    	sprintf(strBuf,"select count(*) from msaAlertList where %s and sdate='%s' and stime>='%s' and stime<'%s' ", caWhere,sdate,stime,etime);
    }else{
    	sprintf(strBuf,"select count(*) from msaAlertList where %s and sdate>'%s' and sdate<'%s' ", caWhere,sdate,edate);
    }
printf("strBuf:%s\n",strBuf);

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
    strTemp.s=0;
	  sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);
    k=0;
    if(stime!=NULL){
    	sprintf(strBuf,"select evtid,evtlevel,evttype,evtres,evtdesc,sdate,stime,sip from msaAlertList where %s and sdate='%s' and stime>='%s' and stime<'%s' limit %lu offset %lu", caWhere,sdate,stime,etime,lRowNum,lStartRec);
    }else{
    	sprintf(strBuf,"select evtid,evtlevel,evttype,evtres,evtdesc,sdate,stime,sip from msaAlertList where %s and sdate>'%s' and sdate<'%s'  limit %lu offset %lu ", caWhere,sdate,edate,lRowNum,lStartRec);
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
            sprintf(caTmpp,"{\"evtid\":\"%s\",\"evtlevel\":\"%s\",\"evttype\":\"%s\",\"evtres\":\"%s\",\"evtdesc\":\"%s\",\"stime\":\"%s %s\",\"bindip\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7]);
            stralloc_cats(&strTemp,caTmpp);
					}
					else
					{
				    sprintf(caTmpp,",{\"evtid\":\"%s\",\"evtlevel\":\"%s\",\"evttype\":\"%s\",\"evtres\":\"%s\",\"evtdesc\":\"%s\",\"stime\":\"%s %s\",\"bindip\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7]);
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

int msaAccountList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  checkTimeOut(confd,ltMsgPk,lt_MMHead);
	  char strBuf[1024];
	  char caWhere[512];
	  ltDbCursor *tempCursor;
	  LT_DBROW tempRow;
	  char caTmpp[512];
	  char *sip;
	  char ontime[512];
	  char offtime[512];
	  char ubytes[512];
	  char dbytes[512];
	  long lRowNum,lStartRec,lSumRec,lCount,lNum;
	  int k;
	  stralloc strTemp;
		ltDbHeadPtr dbPtr;
	  dbPtr=lt_dbinit();
	  lt_db_htmlpage(dbPtr,"utf-8");
    //根据传的参数判断删除以及执行删除
    char *sdate;

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
	  if(ltMsgGetVar_s(ltMsgPk,"sdate"))
		{
	    sdate=ltMsgGetVar_s(ltMsgPk,"sdate");
	  }else{
	  	sdate=NULL;
	  }
		memset(caWhere,0,sizeof(caWhere));
		sprintf(caWhere,"%s"," 1=1 ");

		sip=ltMsgGetVar_s(ltMsgPk,"sip");
    if(sip && sip[0] ){
    		sprintf(caWhere,"%s and net_ending_ip='%s'",caWhere,sip);
    }

		if(ltMsgGetVar_s(ltMsgPk,"flag")){
	    if(strcmp(ltMsgGetVar_s(ltMsgPk,"flag"),"1")==0){/*导出到excel*/
	    	 char caFileName[128];
	       sprintf(caFileName,"AccountListexport-%ld.csv",time(0));
		     k=0;
		     ltMsgPk->msgpktype=2;
				 lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,caFileName,"contenttype","Application/MS-DOWNLOAD");
			   if(sdate!=NULL){
		    		sprintf(strBuf,"select sdate,ubytes,dbytes,ctime,onlinetime,online_time,offline_time,net_ending_ip,net_ending_mac,account,nowaccount,workflag,b.dispname from msaaccountreport a join msaUser b on a.userid=b.userid where sdate='%s' and %s",sdate,caWhere);
		     }else{
		    		sprintf(strBuf,"select sdate,ubytes,dbytes,ctime,onlinetime,online_time,offline_time,net_ending_ip,net_ending_mac,account,nowaccount,workflag,b.dispname from msaaccountreport a join msaUser b on a.userid=b.userid and %s",caWhere);
		     }
printf("strBuf_export:%s\n",strBuf);
			   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
			   if(tempCursor!=NULL){
						tempRow= ltDbFetchRow(tempCursor);
					  while(tempRow!=NULL){
				     	sprintf(caTmpp,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12]);
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
	  }

	  if(sdate!=NULL){
    	sprintf(strBuf,"select count(*) from msaaccountreport where sdate='%s' and %s",sdate,caWhere);
    }else{
    	sprintf(strBuf,"select count(*) from msaaccountreport and %s",caWhere);
    }
printf("msaaccountreport_strBuf:%s\n",strBuf);

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
    if(sdate!=NULL){
    	sprintf(strBuf,"select sdate,ubytes,dbytes,ctime,onlinetime,online_time,offline_time,net_ending_ip,net_ending_mac,account,nowaccount,workflag,b.dispname from msaaccountreport a join msaUser b on a.userid=b.userid where sdate='%s' and %s limit %lu offset %lu",sdate,caWhere,lRowNum,lStartRec);
    }else{
    	sprintf(strBuf,"select sdate,ubytes,dbytes,ctime,onlinetime,online_time,offline_time,net_ending_ip,net_ending_mac,account,nowaccount,workflag,b.dispname from msaaccountreport a join msaUser b on a.userid=b.userid where %s limit %lu offset %lu ",caWhere,lRowNum,lStartRec);
    }
printf("msaaccountreport_strBuf:%s\n",strBuf);

	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {
					if(atoi(tempRow[1])>1024000000){
	    			 sprintf(ubytes,"%0.3fG",atoi(tempRow[1])/1024000000.00);
	    		}else if(atoi(tempRow[1])>1024000){
	    			 sprintf(ubytes,"%0.3fM",atoi(tempRow[1])/1024000.00);
	    		}else{
	    			 sprintf(ubytes,"%0.3fK",atoi(tempRow[1])/1024.00);
	    		}
					if(atoi(tempRow[2])>1024000000){
	    			 sprintf(dbytes,"%0.3fG",atoi(tempRow[2])/1024000000.00);
	    		}else if(atoi(tempRow[2])>1024000){
	    			 sprintf(dbytes,"%0.3fM",atoi(tempRow[2])/1024000.00);
	    		}else{
	    			 sprintf(dbytes,"%0.3fK",atoi(tempRow[2])/1024.00);
	    		}
	    		strcpy(ontime,ltTimeFormat("%Y-%m-%d %H:%M:%S",atol(tempRow[5])));
	    		strcpy(offtime,ltTimeFormat("%Y-%m-%d %H:%M:%S",atol(tempRow[6])));
					if(k==0)
					{
            sprintf(caTmpp,"{\"sdate\":\"%s\",\"ubytes\":\"%s\",\"dbytes\":\"%s\",\"ctime\":\"%s\",\"onlinetime\":\"%s\",\"online_time\":\"%s\",\"offline_time\":\"%s\",\"net_ending_ip\":\"%s\",\"net_ending_mac\":\"%s\",\"account\":\"%s\",\"nowaccount\":\"%s\",\"workflag\":\"%s\",\"dispname\":\"%s\"}",tempRow[0],ubytes,dbytes,tempRow[3],tempRow[4],ontime,offtime,tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12]);
            stralloc_cats(&strTemp,caTmpp);
					}
					else
					{
				    sprintf(caTmpp,",{\"sdate\":\"%s\",\"ubytes\":\"%s\",\"dbytes\":\"%s\",\"ctime\":\"%s\",\"onlinetime\":\"%s\",\"online_time\":\"%s\",\"offline_time\":\"%s\",\"net_ending_ip\":\"%s\",\"net_ending_mac\":\"%s\",\"account\":\"%s\",\"nowaccount\":\"%s\",\"workflag\":\"%s\",\"dispname\":\"%s\"}",tempRow[0],ubytes,dbytes,tempRow[3],tempRow[4],ontime,offtime,tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12]);
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
