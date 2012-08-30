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



/*显示用户信息模板*/
int msaTermInfoView(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 int uid;
	 nasDbUserInfo *tmpdbuserinfo;
	 ltDbHeadPtr dbPtr;

	 char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
	 		fprintf(stderr,"db connect error\n");
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
			ltMsgFree(ltMsgPk);
			return 0;
	 }


	 dbPtr=lt_dbinit();
	 lt_db_htmlpage(dbPtr,"utf-8");
	 uid=atol(ltMsgGetVar_s(ltMsgPk,"uid"));

	 tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,uid);
   if(tmpdbuserinfo!=NULL&&tmpdbuserinfo->userid==uid){
       		char userid[32],ratelimit[32],locktime[32],lLimitBytes[64],lLimitTimes[32],lAllUpFlow[64],lAllDownFlow[64],lAllTime[32],lLimitBytesw[64],lLimitTimesw[32],lAllUpFloww[64],lAllDownFloww[64],lAllTimew[32],lLimitBytesm[64],lLimitTimesm[32],lAllUpFlowm[64],lAllDownFlowm[64],accountnum[32],maxpackage[32],maxBytes[32],maxudp[32], maxtcp[32],maxudpp[32],maxtcpp[32],maxupp[32],maxdownp[32], maxsp[32],maxnc[32];
	 				char seconds[32];
	 				int  jjj;

	 				memset(locktime,0,sizeof(locktime));
	 				if(tmpdbuserinfo->locktime>0){
	 					nasCvtStime(tmpdbuserinfo->locktime,locktime,seconds);
	 				}
       		sprintf(userid,"%d",tmpdbuserinfo->userid);
					sprintf(ratelimit,"%d",tmpdbuserinfo->ratelimit);
					sprintf(lLimitBytes,"%lld",tmpdbuserinfo->lLimitBytes);
					sprintf(lLimitTimes,"%d",tmpdbuserinfo->lLimitTimes);
					sprintf(lAllUpFlow,"%lld",tmpdbuserinfo->lAllUpFlow);
					sprintf(lAllDownFlow,"%lld",tmpdbuserinfo->lAllDownFlow);
					sprintf(lAllTime,"%d",tmpdbuserinfo->lAllTime);
					sprintf(lLimitBytesw,"%lld",tmpdbuserinfo->lLimitBytesw);
					sprintf(lLimitTimesw,"%d",tmpdbuserinfo->lLimitTimesw);
					sprintf(lAllUpFloww,"%lld",tmpdbuserinfo->lAllUpFloww);
					sprintf(lAllDownFloww,"%lld",tmpdbuserinfo->lAllDownFloww);
					sprintf(lAllTimew,"%d",tmpdbuserinfo->lAllTimew);
					sprintf(lLimitBytesm,"%lld",tmpdbuserinfo->lLimitBytesm);
					sprintf(lLimitTimesm,"%d",tmpdbuserinfo->lLimitTimesm);
					sprintf(lAllUpFlowm,"%lld",tmpdbuserinfo->lAllUpFlowm);
					sprintf(lAllDownFlowm,"%lld",tmpdbuserinfo->lAllDownFlowm);
					sprintf(accountnum,"%0.3f",tmpdbuserinfo->accountnum);
					sprintf(maxpackage,"%d",tmpdbuserinfo->maxpackage);
					sprintf(maxBytes,"%d",tmpdbuserinfo->maxBytes);
					sprintf(maxudp,"%d",tmpdbuserinfo->maxudp);
					sprintf(maxtcp,"%d",tmpdbuserinfo->maxtcp);
					sprintf(maxudpp,"%d",tmpdbuserinfo->maxudpp);
					sprintf(maxtcpp,"%d",tmpdbuserinfo->maxtcpp);
					sprintf(maxupp,"%d",tmpdbuserinfo->maxupp);
					sprintf(maxdownp,"%d",tmpdbuserinfo->maxdownp);
					sprintf(maxsp,"%d",tmpdbuserinfo->maxsp);
					sprintf(maxnc,"%d",tmpdbuserinfo->maxnc);

					//设置groupid,显示为最后的级别
					for(jjj=(_MAX_GROUPLEVEL-1);jjj>-1;jjj--){
						if(jjj>0){
	            if(tmpdbuserinfo->groupid[jjj]!=0){
	            	char mygid[32];
	            	sprintf(mygid,"%d",tmpdbuserinfo->groupid[jjj]);
	            	lt_dbput_rootvars(dbPtr,1,"gid",mygid);
	            	break;
							}
					  }else{
					  		char mygid[32];
	            	sprintf(mygid,"%d",tmpdbuserinfo->groupid[0]);
	            	lt_dbput_rootvars(dbPtr,1,"gid",mygid);
					  }
					}

       	 	lt_dbput_rootvars(dbPtr,28,
       	 		"userid",userid,
						"ratelimit",ratelimit,
						"locktime",locktime,
						"lLimitBytes",lLimitBytes,
						"lLimitTimes",lLimitTimes,
						"lAllUpFlow",lAllUpFlow,
						"lAllDownFlow",lAllDownFlow,
						"lAllTime",lAllTime,
						"lLimitBytesw",lLimitBytesw,
						"lLimitTimesw",lLimitTimesw,
						"lAllUpFloww",lAllUpFloww,
						"lAllDownFloww",lAllDownFloww,
						"lAllTimew",lAllTimew,
						"lLimitBytesm",lLimitBytesm,
						"lLimitTimesm",lLimitTimesm,
						"lAllUpFlowm",lAllUpFlowm,
						"lAllDownFlowm",lAllDownFlowm,
						"accountnum",accountnum,
						"maxpackage",maxpackage,
						"maxBytes",maxBytes,
						"maxudp",maxudp,
						"maxtcp",maxtcp,
						"maxudpp",maxudpp,
						"maxtcpp",maxtcpp,
						"maxupp",maxupp,
						"maxdownp",maxdownp,
						"maxsp",maxsp,
						"maxnc",maxnc
       	 	);
       	 	char str[32];
       	 	char moreflag[33],caSel[16];
       	 	sprintf(str,"sex%d",tmpdbuserinfo->sex);
       	 	lt_dbput_rootvars(dbPtr,1,str,"selected");

       	 	memset(str,0,sizeof(str));
       	 	sprintf(str,"idcard%d",tmpdbuserinfo->Certificate_type);
       	 	lt_dbput_rootvars(dbPtr,1,str,"selected");

       	 	memset(str,0,sizeof(str));
       	 	sprintf(str,"jftype%d",tmpdbuserinfo->jftype);
       	 	lt_dbput_rootvars(dbPtr,1,str,"selected");
       	 	memset(str,0,sizeof(str));
       	 	sprintf(str,"ratetype%d",tmpdbuserinfo->ratetype);
       	 	lt_dbput_rootvars(dbPtr,1,str,"selected");

       	 	memset(str,0,sizeof(str));
       	 	sprintf(str,"useflags%d",tmpdbuserinfo->useflags);
       	 	lt_dbput_rootvars(dbPtr,1,str,"selected");


       	 	memset(moreflag,0,sizeof(moreflag));
					memcpy(moreflag,tmpdbuserinfo->moreflag,32);
       	 	int i;
       	 	for(i=0;i<32;i++){
       	 		memset(caSel,0,sizeof(caSel));
       	 		sprintf(caSel,"c%d",i);
       	 		if(moreflag[i]=='1'){
       	 			lt_dbput_rootvars(dbPtr,1,caSel,"true");
       	 		}else{
       	 			lt_dbput_rootvars(dbPtr,1,caSel,"false");
       	 		}
       	 	}
       	 	//QOS 状态
       	 	char sqlBuf[128];
       	 	ltTablePtr tablePtr;
       	 	ltDbCursor *tempCursor;
					LT_DBROW tempRow;
//					memset(str,0,sizeof(str));
//       	 	sprintf(str,"qsel%d",tmpdbuserinfo->ratelimit);
       	 	tablePtr=lt_dbput_table(dbPtr,"qoslist");
					sprintf(sqlBuf,"select sid,qosname from msaqos where sid>0 order by sid asc");
					tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
					if(tempCursor!=NULL){
								tempRow= ltDbFetchRow(tempCursor);
								while(tempRow!=NULL){
									if(tmpdbuserinfo->ratelimit==atol(tempRow[0])){
										lt_dbput_recordvars(tablePtr,3,
											"qosid",LT_TYPE_STRING,tempRow[0],
											"qsel1",LT_TYPE_STRING,"selected",
											"qosname",LT_TYPE_STRING,tempRow[1]);
									}else{
										lt_dbput_recordvars(tablePtr,3,
											"qosid",LT_TYPE_STRING,tempRow[0],
											"qsel1",LT_TYPE_STRING,"",
											"qosname",LT_TYPE_STRING,tempRow[1]);
									}
									tempRow= ltDbFetchRow(tempCursor);
								}
								ltDbCloseCursor(tempCursor);
					}
       	 	ltDbClose(G_DbCon);

       	 	char strip[128];
       	 	char strmac[32];
       	 	memset(strip,0,sizeof(strip));
     	 		//Ip转换
     	 		struct in_addr s;
					if(tmpdbuserinfo->bindip[0]==0&&tmpdbuserinfo->bindip[1]==0&&tmpdbuserinfo->bindip[2]==0 ){//ipv4
				    printf("IPV4\n");
				    s.s_addr = htonl(tmpdbuserinfo->bindip[3]);
				    sprintf(strip,"%s",inet_ntoa(s));
				  }else{//ipv6
				  	printf("IPV6\n");		    	  
				    tmpdbuserinfo->bindip[0]=htonl(tmpdbuserinfo->bindip[0]);
				    tmpdbuserinfo->bindip[1]=htonl(tmpdbuserinfo->bindip[1]);
				    tmpdbuserinfo->bindip[2]=htonl(tmpdbuserinfo->bindip[2]);
				    tmpdbuserinfo->bindip[3]=htonl(tmpdbuserinfo->bindip[3]);
				   inet_ntop(AF_INET6, (void *)(tmpdbuserinfo->bindip), strip, 64);
				  }
				  memset(strmac,0,sizeof(strmac));
				  if(tmpdbuserinfo->bindmac[0]
				  	|| tmpdbuserinfo->bindmac[1]
				  	|| tmpdbuserinfo->bindmac[2]
				  	|| tmpdbuserinfo->bindmac[3]
				  	|| tmpdbuserinfo->bindmac[4]
				  	|| tmpdbuserinfo->bindmac[5] ){
				  	nasCvtMac(tmpdbuserinfo->bindmac,strmac);
				  }
					char policyid[129],topSrvNameList[MAX_TOP_SRVNUM+1];
       	 	memset(policyid,0,sizeof(policyid));
					memcpy(policyid,tmpdbuserinfo->policyid,128);
       	 	memset(topSrvNameList,0,sizeof(topSrvNameList));
					memcpy(topSrvNameList,tmpdbuserinfo->topSrvNameList,MAX_TOP_SRVNUM);
  				lt_dbput_rootvars(dbPtr,9,
  					"username",tmpdbuserinfo->username,
  					"dispname",tmpdbuserinfo->dispname,
  					"Certificate_code",tmpdbuserinfo->Certificate_code,
  					"email",tmpdbuserinfo->bindemail,
  					"ip",strip,
  					"mac",strmac,
  					"policyid",policyid,
  					"moreflag",moreflag,
  					"topSrvNameList",topSrvNameList
  			);
	 }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/msaterminfodetail.htm","Content-type: text/html;charset=utf-8\n",0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}



/*显示添加用户模板*/
int msaShowAddClient(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char sqlBuf[128];
  ltDbHeadPtr dbPtr;
  ltTablePtr tablePtr;
  ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	char caPolicy[129];
  int  pid;

  char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
	 		fprintf(stderr,"db connect error\n");
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
			ltMsgFree(ltMsgPk);
			return 0;
	 }

  dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
  pid=atol(ltMsgGetVar_s(ltMsgPk,"pid"));
  memset(caPolicy,0,129);
  sprintf(caPolicy,"%s","00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
  if(pid<128 && pid>-1){
  	caPolicy[pid]='1';
  }else{
  	caPolicy[0]='1';
  }
  lt_dbput_rootvars(dbPtr,1,"policy",caPolicy);


  lt_dbput_rootvars(dbPtr,1,"gid",ltMsgGetVar_s(ltMsgPk,"gid"));

  tablePtr=lt_dbput_table(dbPtr,"qoslist");
	sprintf(sqlBuf,"select sid,qosname from msaqos where sid>0 order by sid asc");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				while(tempRow!=NULL){
						lt_dbput_recordvars(tablePtr,3,
							"qosid",LT_TYPE_STRING,tempRow[0],
							"qosname",LT_TYPE_STRING,tempRow[1]);
					tempRow= ltDbFetchRow(tempCursor);
				}
				ltDbCloseCursor(tempCursor);
	}

  ltMsgPk->msgpktype=1;
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/msaterminfonew.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	ltDbClose(G_DbCon);
	return 0;
}




int msaloop(int level,int nowpid,int *pgid){
		 ltDbCursor *tempCursor1;
		 LT_DBROW tempRow1;
     int  nowglevel;
     int  iii;
     char sqlstr[128];
     nowglevel=0;
		 char *dbUser;
		 char *dbPass;
		 char *dbName;
		 ltDbConn *Tmp_DbCon;
		 dbName=_ltPubInfo->_dbname;
	 	 dbUser=_ltPubInfo->_dbuser;
		 dbPass=_ltPubInfo->_dbpass;
		 Tmp_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		 if (Tmp_DbCon==NULL){
			return -1;
		 }
     sprintf(sqlstr,"select pid,level from msaGroup where id=%d",nowpid);
     tempCursor1=ltDbOpenCursor(Tmp_DbCon,sqlstr);
     if(tempCursor1){
              tempRow1= ltDbFetchRow(tempCursor1);
              if(tempRow1){
                 nowglevel=atol(tempRow1[1]);
              }
              ltDbCloseCursor(tempCursor1);
     }
     if(nowglevel>=_MAX_GROUPLEVEL){
              return -1;
     }
     for(iii=nowglevel;iii>-1;iii--){
              sprintf(sqlstr,"select pid from msaGroup where id=%d and level=%d",nowpid,iii);
              tempCursor1=ltDbOpenCursor(Tmp_DbCon,sqlstr);
              if(tempCursor1){
                 tempRow1= ltDbFetchRow(tempCursor1);
                 if(tempRow1){
                    pgid[iii]=nowpid;
                    nowpid=atol(tempRow1[0]);
                 }
                 ltDbCloseCursor(tempCursor1);
              }
     }
     ltDbClose(Tmp_DbCon);
     return 0;
}

//添加用户
int msaTerminfoAdd(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  char sqlBuf[2048];
    int Certificate_type,sex,ratelimit,ratetype,userflag,accountnum,jftype;
		char username[32],dispname[32],Certificate_code[32],password[32],bindemail[64],bindmac[32],moreflag[33],policyid[129],topsrvnamelist[MAX_TOP_SRVNUM],strip[32];
	  unsigned int endtime,addtime,bindip[4];
		bindip[0]=0;
		bindip[1]=0;
		bindip[2]=0;
		bindip[3]=0;
    ltDbHeadPtr dbPtr;

    char  searchg[32];
    char  searchgl[32];

    memset(searchg,0,sizeof(searchg));
		memset(searchgl,0,sizeof(searchgl));
    sprintf(searchgl,"%s","-1");

		memset(policyid,0,sizeof(policyid));
		memset(moreflag,0,sizeof(moreflag));
		memset(topsrvnamelist,1,sizeof(topsrvnamelist));
		memset(strip,0,sizeof(strip));
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
			fprintf(stderr,"db connect error\n");
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain; charset=utf-8\n","lthtml",LT_TYPE_STRING,_ltPubInfo->pubMsgStr[5].conStr);
			ltMsgFree(ltMsgPk);
			return 0;
		}

	 	int groupid=0;
	 	int level,iii;
	 	int  pgid[_MAX_GROUPLEVEL];
	 	for(iii=0;iii<_MAX_GROUPLEVEL;iii++){
      	pgid[iii]=0;
    }
	 	if(strcmp(ltMsgGetVar_s(ltMsgPk,"groupid"),"")!=0){
	 		groupid=atoi(ltMsgGetVar_s(ltMsgPk,"groupid"));
	 		sprintf(searchg,"%d",groupid);
	 	}
 		sprintf(sqlBuf,"select pid,level from msagroup where id=%d",groupid);
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor!=NULL){
			 tempRow= ltDbFetchRow(tempCursor);
			 if(tempRow!=NULL){
				 	level=atol(tempRow[1]);
				 	sprintf(searchgl,"%d",level);
				 	pgid[level]=groupid;
					 	if(level>0){
						  msaloop(level,atol(tempRow[0]),pgid);
						}
			 }
		   ltDbCloseCursor(tempCursor);
		}
		sprintf(policyid,"%s",ltMsgGetVar_s(ltMsgPk,"policy"));
	 	sprintf(moreflag,"%s",ltMsgGetVar_s(ltMsgPk,"annex"));
		sprintf(topsrvnamelist,"%s",ltMsgGetVar_s(ltMsgPk,"qosid"));
    sprintf(username,"%s",ltMsgGetVar_s(ltMsgPk,"username"));
    sprintf(dispname,"%s",ltMsgGetVar_s(ltMsgPk,"dispname"));
    Certificate_type=atol(ltMsgGetVar_s(ltMsgPk,"Certificate_type"));
    sprintf(Certificate_code,"%s",ltMsgGetVar_s(ltMsgPk,"Certificate_code"));
    sex=atol(ltMsgGetVar_s(ltMsgPk,"sex"));
    ltMd5Encrypto(ltMsgGetVar_s(ltMsgPk,"password"),strlen(ltMsgGetVar_s(ltMsgPk,"password")),password);
    sprintf(bindemail,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
    sprintf(strip,"%s",ltMsgGetVar_s(ltMsgPk,"ip"));
    if(strcmp(ltMsgGetVar_s(ltMsgPk,"ip"),"")!=0){
    		printf("%s\n",ltMsgGetVar_s(ltMsgPk,"ip"));
    					memset(bindip,0,sizeof(int)*4);
            	//判断地址格式 包括.是ipv4地址  :ipv6地址
							char  *_ipaddr;
							char  _ch;
							int   ipv4flag;
							ipv4flag=0;
							_ipaddr=strip;
							while ((_ch = *_ipaddr++) != '\0') {
								if(_ch == '.'){
									ipv4flag=1;
									break;
								}
							}
              if(ipv4flag==1){
              	_ipaddr=strip;
            		bindip[3]=ntohl(inet_addr(strip));
            	}else{
            		_ipaddr=strip;
            		inet_pton(AF_INET6, _ipaddr, (void *)(bindip));  //实际网络顺序问题
            		bindip[0]=ntohl(bindip[0]);
            		bindip[1]=ntohl(bindip[1]);
            		bindip[2]=ntohl(bindip[2]);
            		bindip[3]=ntohl(bindip[3]);
            	}
  	}
  	memset(bindmac,0,sizeof(bindmac));
    sprintf(bindmac,"%s",ltMsgGetVar_s(ltMsgPk,"mac"));

    ratelimit=atol(ltMsgGetVar_s(ltMsgPk,"ratelimit"));
    ratetype=atol(ltMsgGetVar_s(ltMsgPk,"solo"));
    jftype=atol(ltMsgGetVar_s(ltMsgPk,"billing"));
    accountnum=atol(ltMsgGetVar_s(ltMsgPk,"money"));
    endtime=0;
	  if(strlen(ltMsgGetVar_s(ltMsgPk,"edate"))>0){
			    		endtime=nasCvtLongTime(ltMsgGetVar_s(ltMsgPk,"edate"),"00:00:00");
		}
		addtime=time(0);
	  userflag=atol(ltMsgGetVar_s(ltMsgPk,"userflag"));

	  //判断ip/mac不能同ip/mac绑定的冲突



    if(bindip[3]&&bindip[2]&&bindip[1]&&bindip[0]){
    	  nasDbUserInfo  *tmpDbUserInfoListIp=NULL;
   			tmpDbUserInfoListIp=msaLookUpDbUserByIp(_ltPubInfo,bindip);
   			if(tmpDbUserInfoListIp){
				   	int  ipmacflag=0;
				   	//nasSetMoreFlagS(tmpDbUserInfoListIp,&ipmacflag);//查找批量属性规则
				    if( tmpDbUserInfoListIp->moreflag[MSA_DBUSER_FLAG_IPMAC]=='1' || ipmacflag==1 ){/*判断ip地址和mac地址是否一致,一种情况，以前没有设置，后来设置后要一个一个修改*/
										//对不起，该ip启动了ip/mac绑定，不能有重复，请采用其他ip
										ltMsgPk->msgpktype=1;
										lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain; charset=utf-8\n","lthtml",LT_TYPE_STRING,_ltPubInfo->pubMsgStr[60].conStr);
										ltMsgFree(ltMsgPk);
										return 0;
						}
				}
   	}

   	if(strlen(bindmac)==17 || strlen(bindmac)==12){
           char   intmac[6];
           nasDbUserInfo   *tmpDbUserInfoListMac=NULL;
           memset(intmac,0,6);

           nasCvtMacI(bindmac,intmac);
           tmpDbUserInfoListMac=msaLookUpDbUserByMac(_ltPubInfo,intmac);
           if(tmpDbUserInfoListMac){
					   	int  ipmacflag=0;
					   	//nasSetMoreFlagS(tmpDbUserInfoListMac,&ipmacflag);//查找批量属性规则
					    if( tmpDbUserInfoListMac->moreflag[MSA_DBUSER_FLAG_IPMAC]=='1' || ipmacflag==1 ){/*判断ip地址和mac地址是否一致,一种情况，以前没有设置，后来设置后要一个一个修改*/
										//对不起，该mac启动了ip/mac绑定，不能有重复，请采用其他mac
										ltMsgPk->msgpktype=1;
										lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain; charset=utf-8\n","lthtml",LT_TYPE_STRING,_ltPubInfo->pubMsgStr[61].conStr);
										ltMsgFree(ltMsgPk);
										return 0;
							}
					 }
    }

		//内存更新
		{
		    nasDbUserInfo item;
		    nasDbUserInfo *itemnew;
				memset(&item,0,sizeof(nasDbUserInfo));
        snprintf(item.username,32,"%s",username);
        snprintf(item.dispname,32,"%s",dispname);
        item.Certificate_type=Certificate_type;
        snprintf(item.Certificate_code,24,"%s",Certificate_code);
        item.sex=sex;
        snprintf(item.password,32,"%s",password);
        item.groupid[0]=pgid[0];
        item.groupid[1]=pgid[1];
        item.groupid[2]=pgid[2];
        item.groupid[3]=pgid[3];
        item.groupid[4]=pgid[4];
        snprintf(item.bindemail,64,"%s",bindemail);
        item.bindip[0]=bindip[0];
        item.bindip[1]=bindip[1];
        item.bindip[2]=bindip[2];
        item.bindip[3]=bindip[3];
        if(strlen(bindmac)==17 || strlen(bindmac)==12){
           nasCvtMacI(bindmac,item.bindmac);
        }
        item.ratelimit=ratelimit;
        item.ratetype=(unsigned char)ratetype;
				item.addtime=(unsigned int)addtime;
        item.locktime=(unsigned int)endtime;
        memcpy(item.moreflag,moreflag,32);
        memcpy(item.policyid,policyid,128);
        memcpy(item.topSrvNameList,topsrvnamelist,MAX_TOP_SRVNUM);
        item.useflags=(unsigned char)userflag;
        item.lasttime=0;
        item.lLimitBytes=0;
        item.lLimitTimes=0;
        item.lAllUpFlow=0;
        item.lAllDownFlow=0;
				item.lAllTime=0;
        item.lLimitBytesw=0;
				item.lLimitTimesw=0;
        item.lAllUpFloww=0;
        item.lAllDownFloww=0;
        item.lAllTimew=0;
        item.lLimitBytesm=0;
				item.lLimitTimesm=0;
        item.lAllUpFlowm=0;
        item.lAllDownFlowm=0;
        item.lAllTimem=0;
        item.jftype=jftype;
        item.accountnum=accountnum;
        item.maxpackage=0;
        item.maxBytes=0;
        item.maxudp=0;
        item.maxtcp=0;
        item.maxudpp=0;
        item.maxtcpp=0;
        item.maxupp=0;
        item.maxdownp=0;
        item.maxsp=0;
        item.maxnc=0;
    	  itemnew=msaNewInsertDbUser(_ltPubInfo,&item);
    	  printf("username  %s\n",item.username);
    	  printf("%u|%x \n",item.bindip[0],item.bindip[0]);
    	  printf("%u|%x \n",item.bindip[1],item.bindip[1]);
    	  printf("%u|%x \n",item.bindip[2],item.bindip[2]);
    	  printf("%u|%x \n",item.bindip[3],item.bindip[3]);
    }
     dbPtr=lt_dbinit();
     lt_db_htmlpage(dbPtr,"utf-8");
		 lt_dbput_rootvars(dbPtr,1,"gid",searchg);
   	 lt_dbput_rootvars(dbPtr,1,"level",searchgl);
   	 lt_dbput_rootvars(dbPtr,1,"policy",policyid);

		 ltMsgPk->msgpktype=1;


	   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/clientlistinfo.htm",dbPtr->head,0);

	   ltMsgFree(ltMsgPk);
	   lt_dbfree(dbPtr);
//     ltMsgPk->msgpktype=1;
//		 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
//		 ltMsgFree(ltMsgPk);

	   ltDbClose(G_DbCon);
	   return 0;
}


//修改用户
int msaTerminfoUp(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		int type,id,iii;
		nasDbUserInfo *tmpdbuserinfo;
		ltDbCursor *tempCursor;
		LT_DBROW tempRow;
		type=-1;
		id=-1;
		if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
	 		type=atol(ltMsgGetVar_s(ltMsgPk,"type"));
	 	}
	 	if(ltMsgGetVar_s(ltMsgPk,"userid")!=NULL){
	 		id=atol(ltMsgGetVar_s(ltMsgPk,"userid"));
	 	}

	 	char *dbUser;
	  char *dbPass;
	  char *dbName;
	  dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
			fprintf(stderr,"db connect error\n");
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
			ltMsgFree(ltMsgPk);
			return 0;
		}
		if(type==0){
			unsigned int maxudp,lLimitTimes,maxtcp,maxudpp,lLimitTimesw,maxtcpp,maxupp,lLimitTimesm,maxdownp,maxpackage,maxsp,maxBytes,maxnc;
			unsigned long long lLimitBytes,lLimitBytesw,lLimitBytesm;

			maxudp=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxudp"));
			lLimitTimes=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"lLimitTimes"));
			maxtcp=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxtcp"));
			maxudpp=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxudpp"));
			lLimitTimesw=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"lLimitTimesw"));
			maxtcpp=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxtcpp"));
			maxupp=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxupp"));
			lLimitTimesm=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"lLimitTimesm"));
			maxdownp=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxdownp"));
			maxpackage=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxpackage"));
			maxsp=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxsp"));
			maxBytes=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxBytes"));
			maxnc=(unsigned int)atol(ltMsgGetVar_s(ltMsgPk,"maxnc"));

			lLimitBytes=atoll(ltMsgGetVar_s(ltMsgPk,"lLimitBytes"));
			lLimitBytesw=atoll(ltMsgGetVar_s(ltMsgPk,"lLimitBytesw"));
			lLimitBytesm=atoll(ltMsgGetVar_s(ltMsgPk,"lLimitBytesm"));
			char sqlstr[1024];
			sprintf(sqlstr,"update msauser set maxudp=%d,lLimitTimes=%d,maxtcp=%d,maxudpp=%d,lLimitTimesw=%d,maxtcpp=%d,maxupp=%d,lLimitTimesm=%d,maxdownp=%d,maxpackage=%d,maxsp=%d,maxBytes=%d,maxnc=%d,lLimitBytes=%lld,lLimitBytesw=%lld,lLimitBytesm=%lld where userid=%d",maxudp,lLimitTimes,maxtcp,maxudpp,lLimitTimesw,maxtcpp,maxupp,lLimitTimesm,maxdownp,maxpackage,maxsp,maxBytes,maxnc,lLimitBytes,lLimitBytesw,lLimitBytesm,id);

			ltDbExecSql(G_DbCon,sqlstr);


			//内存更新
	 		tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,id);
      if(tmpdbuserinfo!=NULL){
      			tmpdbuserinfo->maxudp=maxudp;
      			tmpdbuserinfo->lLimitTimes=lLimitTimes;
      			tmpdbuserinfo->maxtcp=maxtcp;
      			tmpdbuserinfo->maxudpp=maxudpp;
      			tmpdbuserinfo->lLimitTimesw=lLimitTimesw;
      			tmpdbuserinfo->maxtcpp=maxtcpp;
      			tmpdbuserinfo->maxupp=maxupp;
      			tmpdbuserinfo->lLimitTimesm=lLimitTimesm;
      			tmpdbuserinfo->maxdownp=maxdownp;
      			tmpdbuserinfo->maxpackage=maxpackage;
      			tmpdbuserinfo->maxsp=maxsp;
      			tmpdbuserinfo->maxBytes=maxBytes;
      			tmpdbuserinfo->maxnc=maxnc;
      			tmpdbuserinfo->lLimitBytes=lLimitBytes;
      			tmpdbuserinfo->lLimitBytesw=lLimitBytesw;
      			tmpdbuserinfo->lLimitBytesm=lLimitBytesm;

      }

      ltDbClose(G_DbCon);
			ltMsgPk->msgpktype=1;
		  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
	    ltMsgFree(ltMsgPk);
	    return 0;
		}else if(type==1){
	 		tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,id);
      if(tmpdbuserinfo!=NULL&&tmpdbuserinfo->userid==id){
       		char sqlBuf[2048];
			    int Certificate_type,sex,ratelimit,ratetype,userflag,jftype;
			    float accountnum;
					char username[32],dispname[32],Certificate_code[32],bindemail[64],bindmac[32],strbindip[32],moreflag[33],policyid[129],topsrvnamelist[MAX_TOP_SRVNUM];
					int gid;int level;
					int pgid[_MAX_GROUPLEVEL];
					unsigned int endtime,bindip[4];
					bindip[0]=0;
					bindip[1]=0;
					bindip[2]=0;
					bindip[3]=0;
					sprintf(strbindip,"%s",ltMsgGetVar_s(ltMsgPk,"ip"));
			    if(strcmp(strbindip,"")!=0){
			    		memset(bindip,0,sizeof(int)*4);
            	//判断地址格式 包括.是ipv4地址  :ipv6地址
							char  *_ipaddr;
							char  _ch;
							int   ipv4flag;
							ipv4flag=0;
							_ipaddr=strbindip;
							while ((_ch = *_ipaddr++) != '\0') {
								if(_ch == '.'){
									ipv4flag=1;
									break;
								}
							}
              if(ipv4flag==1){
              	printf("IPV4\n");
              	_ipaddr=strbindip;
            		bindip[3]=ntohl(inet_addr(strbindip));
            	}else{
              	printf("IPV6\n");
              	_ipaddr=strbindip;
            		inet_pton(AF_INET6, _ipaddr, (void *)(bindip));  //实际网络顺序问题
            		bindip[0]=ntohl(bindip[0]);
            		bindip[1]=ntohl(bindip[1]);
            		bindip[2]=ntohl(bindip[2]);
            		bindip[3]=ntohl(bindip[3]);
            	}
			  	}
			    sprintf(bindmac,"%s",ltMsgGetVar_s(ltMsgPk,"mac"));
			    printf("11111111111111111\n");
			    printf("%u|%x\n",bindip[0],bindip[0]);
			    printf("%u|%x\n",bindip[1],bindip[1]);
			    printf("%u|%x\n",bindip[2],bindip[2]);
			    printf("%u|%x\n",bindip[3],bindip[3]);
			    printf("2222222222222222222\n");
					if(bindip[3]){
			    	  nasDbUserInfo  *tmpDbUserInfoListIp=NULL;
			   			tmpDbUserInfoListIp=msaLookUpDbUserByIp(_ltPubInfo,bindip);
			   			if(tmpDbUserInfoListIp){
			   				if(tmpDbUserInfoListIp->userid!=id){
							   	int  ipmacflag=0;
							   	//nasSetMoreFlagS(tmpDbUserInfoListIp,&ipmacflag);//查找批量属性规则
							    if( tmpDbUserInfoListIp->moreflag[MSA_DBUSER_FLAG_IPMAC]=='1' || ipmacflag==1 ){/*判断ip地址和mac地址是否一致,一种情况，以前没有设置，后来设置后要一个一个修改*/
													//对不起，该ip启动了ip/mac绑定，不能有重复，请采用其他ip
													ltMsgPk->msgpktype=1;
													lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain; charset=utf-8\n","lthtml",LT_TYPE_STRING,_ltPubInfo->pubMsgStr[60].conStr);
													ltMsgFree(ltMsgPk);
													return 0;
									}
								}
							}
			   	}

			   	if(strlen(bindmac)==17 || strlen(bindmac)==12){
			           char   intmac[6];
			           nasDbUserInfo   *tmpDbUserInfoListMac=NULL;
			           memset(intmac,0,6);

			           nasCvtMacI(bindmac,intmac);
			           tmpDbUserInfoListMac=msaLookUpDbUserByMac(_ltPubInfo,intmac);
			           if(tmpDbUserInfoListMac){
			           	if(tmpDbUserInfoListMac->userid!=id){
								   	int  ipmacflag=0;
								   	//nasSetMoreFlagS(tmpDbUserInfoListMac,&ipmacflag);//查找批量属性规则
								    if( tmpDbUserInfoListMac->moreflag[MSA_DBUSER_FLAG_IPMAC]=='1' || ipmacflag==1 ){/*判断ip地址和mac地址是否一致,一种情况，以前没有设置，后来设置后要一个一个修改*/
													//对不起，该mac启动了ip/mac绑定，不能有重复，请采用其他mac
													ltMsgPk->msgpktype=1;
													lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain; charset=utf-8\n","lthtml",LT_TYPE_STRING,_ltPubInfo->pubMsgStr[61].conStr);
													ltMsgFree(ltMsgPk);
													return 0;
										}
									}
								 }
			    }


			    gid=atol(ltMsgGetVar_s(ltMsgPk,"groupid"));

					for(iii=0;iii<_MAX_GROUPLEVEL;iii++){
					   pgid[iii]=0;
					}
					sprintf(sqlBuf,"select pid,level from msagroup where id=%d",gid);
					tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
					if(tempCursor!=NULL){
						 tempRow= ltDbFetchRow(tempCursor);
							 if(tempRow!=NULL){
							 	level=atol(tempRow[1]);
							 	pgid[level]=gid;
							 	if(level>0){
								 msaloop(level,atol(tempRow[0]),pgid);
								}
							 }
						 ltDbCloseCursor(tempCursor);
					 }
					 for(iii=0;iii<_MAX_GROUPLEVEL;iii++){
						 memset(sqlBuf,0,sizeof(sqlBuf));
					 	 sprintf(sqlBuf,"update msauser set groupid%d=%d where userid=%d",iii,pgid[iii],id);
					 	 ltDbExecSql(G_DbCon,sqlBuf);
					 }
					 tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,id);
				   if(tmpdbuserinfo!=NULL&&tmpdbuserinfo->userid==id){
				       	 tmpdbuserinfo->groupid[0]=pgid[0];
				       	 tmpdbuserinfo->groupid[1]=pgid[1];
				       	 tmpdbuserinfo->groupid[2]=pgid[2];
				       	 tmpdbuserinfo->groupid[3]=pgid[3];
				       	 tmpdbuserinfo->groupid[4]=pgid[4];
				   }



					sprintf(username,"%s",ltMsgGetVar_s(ltMsgPk,"username"));
			    sprintf(dispname,"%s",ltMsgGetVar_s(ltMsgPk,"dispname"));
			    Certificate_type=atol(ltMsgGetVar_s(ltMsgPk,"Certificate_type"));
			    sprintf(Certificate_code,"%s",ltMsgGetVar_s(ltMsgPk,"Certificate_code"));
			    sex=atol(ltMsgGetVar_s(ltMsgPk,"sex"));
			    sprintf(bindemail,"%s",ltMsgGetVar_s(ltMsgPk,"email"));



			    ratelimit=atol(ltMsgGetVar_s(ltMsgPk,"ratelimit"));
			    ratetype=atol(ltMsgGetVar_s(ltMsgPk,"solo"));
			    jftype=atol(ltMsgGetVar_s(ltMsgPk,"billing"));
			    accountnum=atof(ltMsgGetVar_s(ltMsgPk,"money"));
			    endtime=0;
			    if(strlen(ltMsgGetVar_s(ltMsgPk,"edate"))>0){
			    		endtime=nasCvtLongTime(ltMsgGetVar_s(ltMsgPk,"edate"),"00:00:00");
			    }
				  sprintf(moreflag,"%s",ltMsgGetVar_s(ltMsgPk,"annex"));
				  if(strlen(moreflag)==0){
				  	memset(moreflag,0,sizeof(moreflag));
				  	memset(moreflag,'0',32);
				  }
				  memset(policyid,0,sizeof(policyid));
			    sprintf(policyid,"%s",ltMsgGetVar_s(ltMsgPk,"policy"));
			    sprintf(topsrvnamelist,"%s",ltMsgGetVar_s(ltMsgPk,"qosid"));
					userflag=atol(ltMsgGetVar_s(ltMsgPk,"userflag"));


				 	sprintf(sqlBuf,"update msauser set username='%s',dispname='%s',Certificate_type=%d,Certificate_code='%s',sex=%d,bindemail='%s',bindip='%s',bindmac='%s',ratelimit=%d,ratetype=%d,moreflag='%s',policyid='%s',locktime=%d,useflags=%d,jftype=%d,accountnum=%0.3f where userid=%d",username,dispname,Certificate_type,Certificate_code,sex,bindemail,strbindip,bindmac,ratelimit,ratetype,moreflag,policyid,endtime,userflag,jftype,accountnum,id);
					printf("%s\n",sqlBuf);
				 	ltDbExecSql(G_DbCon,sqlBuf);

				 	//内存更新
			    tmpdbuserinfo->Certificate_type=Certificate_type;
				  tmpdbuserinfo->sex=sex;
					tmpdbuserinfo->ratelimit=ratelimit;
					tmpdbuserinfo->ratetype=ratetype;
					tmpdbuserinfo->useflags=userflag;


					tmpdbuserinfo->locktime=endtime;

					tmpdbuserinfo->jftype=jftype;
					tmpdbuserinfo->accountnum=accountnum;

					sprintf(tmpdbuserinfo->Certificate_code,"%s",Certificate_code);
					sprintf(tmpdbuserinfo->bindemail,"%s",bindemail);
					//sprintf(tmpdbuserinfo->bindmac,"%s",bindmac);
					memcpy(tmpdbuserinfo->policyid,policyid,128);
					//memcpy(tmpdbuserinfo->groupid,groupid,1000);
					memcpy(tmpdbuserinfo->moreflag,moreflag,32);
					memcpy(tmpdbuserinfo->topSrvNameList,topsrvnamelist,MAX_TOP_SRVNUM);


					sprintf(tmpdbuserinfo->dispname,"%s",dispname);

					if(strcmp(tmpdbuserinfo->username,username)){
						msaDbUserName  _DbUserName;
						_DbUserName.userid=tmpdbuserinfo->userid;
						sprintf(_DbUserName.username,"%s",tmpdbuserinfo->username);
						msaDeleteDbUserName(_ltPubInfo,tmpdbuserinfo->username);

						snprintf(tmpdbuserinfo->username,31,"%s",username);
					  msaHashInsertDbUserName(_ltPubInfo,&_DbUserName);
				  }
					//首先删除以前的ip索引
					msaDbUserIp    _DbUserIp;
					msaDeleteDbUserIp(_ltPubInfo,tmpdbuserinfo->bindip,tmpdbuserinfo->userid);
					tmpdbuserinfo->bindip[0]=bindip[0];
					tmpdbuserinfo->bindip[1]=bindip[1];
					tmpdbuserinfo->bindip[2]=bindip[2];
					tmpdbuserinfo->bindip[3]=bindip[3];
					
					//重新建索引
					if(!msaHashLookUpDbUserIp(_ltPubInfo,tmpdbuserinfo->bindip)){
						_DbUserIp.userid=tmpdbuserinfo->userid;
						_DbUserIp.ipadd[0]=tmpdbuserinfo->bindip[0];
						_DbUserIp.ipadd[1]=tmpdbuserinfo->bindip[1];
						_DbUserIp.ipadd[2]=tmpdbuserinfo->bindip[2];
						_DbUserIp.ipadd[3]=tmpdbuserinfo->bindip[3];
						msaHashInsertDbUserIp(_ltPubInfo,&_DbUserIp);
					}
          if(strlen(bindmac)==17 || strlen(bindmac)==12){
          	char cvtmac[6];
          	memset(cvtmac,0,6);
          	nasCvtMacI(bindmac,cvtmac);
          	if(memcmp(tmpdbuserinfo->bindmac,cvtmac,6)){
          		 msaDbUserMac   _DbUserMac;
          		 //首先删除以前的mac索引
          		 msaDeleteDbUserMac(_ltPubInfo,tmpdbuserinfo->bindmac,tmpdbuserinfo->userid);
          		 memcpy(tmpdbuserinfo->bindmac,cvtmac,6);
          		 //重新建索引
          		 if(!msaHashLookUpDbUserMac(_ltPubInfo,cvtmac)){
		          		 _DbUserMac.userid=tmpdbuserinfo->userid;
					         memcpy(_DbUserMac.caMac,cvtmac,6);
		               msaHashInsertDbUserMac(_ltPubInfo,&_DbUserMac);
               }
            }
          }else{
          	memset(tmpdbuserinfo->bindmac,0,6);
          }

          ltDbClose(G_DbCon);

					ltMsgPk->msgpktype=1;
				  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");
			    ltMsgFree(ltMsgPk);

//				  msaTermInfoView(confd,ltMsgPk,lt_MMHead);
				  return 0;
      }
		}

    return -1;
}

int msaUpdatePwd(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char *newpwd;
	 int  uid;
	 stralloc strTemp;
	 strTemp.s=0;
	 char caTmpp[1024];
	 char newmd5pwd[32],sql[512];
	 nasDbUserInfo *tmpdbuserinfo;
	 char *dbUser;
	 char *dbPass;
	 char *dbName;

	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
			ltMsgFree(ltMsgPk);
			return 0;
	 }

	 newpwd=ltMsgGetVar_s(ltMsgPk,"newpwd");
	 uid=atol(ltMsgGetVar_s(ltMsgPk,"uid"));
	 if(uid<0 || uid>=_ltPubInfo->_dbusernamehead.bufnum ){
	 	  sprintf(caTmpp,"{success:false,errors:[{id:'uid',msg:'用户id错误!'}]}");
     	stralloc_cats(&strTemp,caTmpp);
	 }else{
			 tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,uid );
		   if(tmpdbuserinfo!=NULL&&tmpdbuserinfo->userid==uid){

		     				ltMd5Encrypto(newpwd,strlen(newpwd),newmd5pwd);
		     				sprintf(tmpdbuserinfo->password,"%s",newmd5pwd);
		     				sprintf(sql,"update msauser set password='%s' where userid=%d",newmd5pwd,uid);
		     				ltDbExecSql(G_DbCon,sql);
		     				sprintf(caTmpp,"{success:true}");
		     				stralloc_cats(&strTemp,caTmpp);

		   }
   }
   stralloc_0(&strTemp);

   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 ltDbClose(G_DbCon);
	 return 0;
}



//移动用户到部门
int msaUpdateGroup(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){

	 int uid,gid,iii;
	 char sqlBuf[1024];
	 int level;
	 nasDbUserInfo *tmpdbuserinfo;
	 uid=atol(ltMsgGetVar_s(ltMsgPk,"uid"));
	 gid=atol(ltMsgGetVar_s(ltMsgPk,"gid"));
	 ltDbCursor *tempCursor;
   LT_DBROW tempRow;
   int pgid[_MAX_GROUPLEVEL];

   char *dbUser;
	 char *dbPass;
	 char *dbName;
	 dbName=_ltPubInfo->_dbname;
	 dbUser=_ltPubInfo->_dbuser;
	 dbPass=_ltPubInfo->_dbpass;
	 G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	 if(G_DbCon==NULL){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
			ltMsgFree(ltMsgPk);
			return 0;
	 }

 	 for(iii=0;iii<_MAX_GROUPLEVEL;iii++){
    	pgid[iii]=0;
   }
   sprintf(sqlBuf,"select pid,level from msagroup where id=%d",gid);
	 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	  if(tempCursor!=NULL){
			 tempRow= ltDbFetchRow(tempCursor);
			 if(tempRow!=NULL){
			 	level=atol(tempRow[1]);
			 	pgid[level]=gid;
			 	if(level>0){
				 msaloop(level,atol(tempRow[0]),pgid);
				}
			 }
		 ltDbCloseCursor(tempCursor);
	  }
	for(iii=0;iii<_MAX_GROUPLEVEL;iii++){
			memset(sqlBuf,0,sizeof(sqlBuf));
	 		sprintf(sqlBuf,"update msauser set groupid%d=%d where userid=%d",iii,pgid[iii],uid);
	 		ltDbExecSql(G_DbCon,sqlBuf);
  }

	tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,uid);
  if(tmpdbuserinfo!=NULL&&tmpdbuserinfo->userid==uid){
       	 tmpdbuserinfo->groupid[0]=pgid[0];
       	 tmpdbuserinfo->groupid[1]=pgid[1];
       	 tmpdbuserinfo->groupid[2]=pgid[2];
       	 tmpdbuserinfo->groupid[3]=pgid[3];
       	 tmpdbuserinfo->groupid[4]=pgid[4];
  }


	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
  ltMsgFree(ltMsgPk);
  ltDbClose(G_DbCon);
  return 0;
}

//导入导出  部门导入
int msaGroupInport(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  ltDbHeadPtr dbPtr;
	dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");

  ltMsgPk->msgpktype=1;
  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/groupimport.htm",dbPtr->head,0);
  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);

  return 0;
}



//服务
int msaServiceTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		 int i,type,uid;
		 char tmpstr[1024];
		 char tempname[MAX_TOPSRVNAME_SIZE];
		 stralloc strTemp;
	   int iHead;
	   memset(tmpstr,0,sizeof(tmpstr));
	   iHead=1;
	   type=0;
	   uid=-1;
	   strTemp.s=0;
	   if(ltMsgGetVar_s(ltMsgPk,"uid")!=NULL){
	   		uid=atol(ltMsgGetVar_s(ltMsgPk,"uid"));
	 	 }
	   if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
	   		type=atol(ltMsgGetVar_s(ltMsgPk,"type"));
	 	 }
	   stralloc_cats(&strTemp,"[");
	   if(type==0){
			   for(i=0;i<MAX_TOP_SRVNUM;i++){
				   	if(strcmp(_ltPubInfo->topSrvName[i].srvname,"")!=0){
				   		memset(tempname,0,sizeof(tempname));
				   		sprintf(tempname,"%s",_ltPubInfo->topSrvName[i].srvname);
							if(iHead==1){
								 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
								 iHead=0;
							}
							else{
								 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
							}
							stralloc_cats(&strTemp,tmpstr);
			   		}
			   }
	 	 }else if(type==1){
	 	 		char topSrvNameList[MAX_TOP_SRVNUM];
	 	 		nasDbUserInfo *tmpdbuserinfo;
       	tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,uid);
       	memset(topSrvNameList,0,sizeof(topSrvNameList));
       	if(tmpdbuserinfo!=NULL){
						memcpy(topSrvNameList,tmpdbuserinfo->topSrvNameList,MAX_TOP_SRVNUM);
	      }
	 	 		for(i=0;i<MAX_TOP_SRVNUM;i++){
				   	if(_ltPubInfo->topSrvName[i].srvname[0]){
				   		memset(tempname,0,sizeof(tempname));
				   		sprintf(tempname,"%s",_ltPubInfo->topSrvName[i].srvname);
				   		if(topSrvNameList[i]=='1'){
								if(iHead==1)
								{
									 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
									 iHead=0;
								}
								else
								{
									 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
								}
							}else{
								if(iHead==1)
								{
									 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked: false}",i,tempname);
									 iHead=0;
								}
								else
								{
									 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked: false}",i,tempname);
								}
							}
							stralloc_cats(&strTemp,tmpstr);
			   		}
			  }
	 	 }
	   stralloc_cats(&strTemp,"]");
	   stralloc_0(&strTemp);
	   ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);
	   ltMsgFree(ltMsgPk);
	   return 0;
}

