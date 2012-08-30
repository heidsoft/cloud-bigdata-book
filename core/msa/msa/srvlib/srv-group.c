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


int msalooparea(stralloc *strTemp,int level,int pid){
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	ltDbConn *Tmp_DbCon; /*各个包处理进程的数据库连接*/
	char *dbUser;
	char *dbPass;
	char *dbName;
	char sqlBuf[512];
	char tmpstr[512]; 
	int iHead;
	level++;
  dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	Tmp_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if (Tmp_DbCon==NULL){
		return -1;
	}
	sprintf(sqlBuf,"select id,pid,name,mydesc,level from msaGroup where level=%d and pid=%d order by id asc ",level,pid );
	tempCursor=ltDbOpenCursor(Tmp_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		if(tempRow==NULL){
			sprintf(tmpstr,"%s",",leaf: true");
			stralloc_cats(strTemp,tmpstr);
			ltDbCloseCursor(tempCursor);
			ltDbClose(Tmp_DbCon);
	    return 0;
		}else{
		  sprintf(tmpstr,"%s",",children: [");
			stralloc_cats(strTemp,tmpstr);
	  }
		iHead=1;
		while(tempRow!=NULL){
			if(iHead==1){
				sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s'",tempRow[0],tempRow[3],tempRow[2]);
				iHead=0;
			}else{
			  sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s'",tempRow[0],tempRow[3],tempRow[2]);
		  }			
			stralloc_cats(strTemp,tmpstr);
			pid=atol(tempRow[0]);
			
			if(level<5){
				msalooparea(strTemp,level,pid);
			}
			stralloc_cats(strTemp,"}");
			tempRow= ltDbFetchRow(tempCursor);

		}
		stralloc_cats(strTemp,"]");
		ltDbCloseCursor(tempCursor);
	}
	ltDbClose(Tmp_DbCon);

	return 0;
}

int msaloopareacheckbox(stralloc *strTemp,int level,int pid){
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	ltDbConn *Tmp_DbCon; /*各个包处理进程的数据库连接*/
	char *dbUser;
	char *dbPass;
	char *dbName;
	char sqlBuf[512];
	char tmpstr[512];
	int iHead;
	level++;
  dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	Tmp_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if (Tmp_DbCon==NULL){
		return -1;
	}
	sprintf(sqlBuf,"select id,pid,name,mydesc,level from msaGroup where level=%d and pid=%d order by id asc ",level,pid );
	tempCursor=ltDbOpenCursor(Tmp_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		if(tempRow==NULL){
			sprintf(tmpstr,"%s",",leaf: true");
			stralloc_cats(strTemp,tmpstr);
			ltDbCloseCursor(tempCursor);
			ltDbClose(Tmp_DbCon);
	    return 0;
		}else{
		  sprintf(tmpstr,"%s",",children: [");
			stralloc_cats(strTemp,tmpstr);
	  }
		iHead=1;
		while(tempRow!=NULL){
			if(iHead==1){
				sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s',level:'%d',checked:false",tempRow[0],tempRow[3],tempRow[2],level);
				iHead=0;
			}else{
			  sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s',level:'%d',checked:false",tempRow[0],tempRow[3],tempRow[2],level);
		  }
			stralloc_cats(strTemp,tmpstr);
			pid=atol(tempRow[0]);

			if(level<5){
				msaloopareacheckbox(strTemp,level,pid);
			}
			stralloc_cats(strTemp,"}");
			tempRow= ltDbFetchRow(tempCursor);

		}
		stralloc_cats(strTemp,"]");
		ltDbCloseCursor(tempCursor);
	}
	ltDbClose(Tmp_DbCon);

	return 0;
}

int msaGroupTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char sqlBuf[512];
	char tmpstr[512];
	char name[32];
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	int level,pid,type;
	stralloc strTemp;

	int iHead;
	memset(tmpstr,0,sizeof(tmpstr));
	type=0;
	pid=0;
	level=0;
	iHead=1;
	strTemp.s=0;
	
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
	
	if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
		type=atol(ltMsgGetVar_s(ltMsgPk,"type"));
	}
	if(ltMsgGetVar_s(ltMsgPk,"name")!=NULL){
		sprintf(name,"%s",ltMsgGetVar_s(ltMsgPk,"name"));
	}
	//stralloc_cats(&strTemp,"\xef\xbb\xbf[");
	stralloc_cats(&strTemp,"[");
 	sprintf(sqlBuf,"select id,pid,name,mydesc,level from msaGroup where level=0 and pid=0 order by id asc");
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(type==0){
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				/*判断有没有子节点*/
				if(iHead==1){
					sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s'",tempRow[0],tempRow[3],tempRow[2]);
					iHead=0;
				}else{
				  sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s'",tempRow[0],tempRow[3],tempRow[2]);
			  }
			  stralloc_cats(&strTemp,tmpstr);
			  pid=atol(tempRow[0]);
				if(level<5){
					msalooparea(&strTemp,level,pid);
				}
				stralloc_cats(&strTemp,"}");
				tempRow= ltDbFetchRow(tempCursor);
			}
		}
			ltDbCloseCursor(tempCursor);
	}else if(type==2){
		char allgroup[1024],msassessionid[12];
		memset(allgroup,0,sizeof(allgroup));
		memset(msassessionid,0,sizeof(msassessionid));
		ltMsgGetSomeNVar(ltMsgPk,1,"msasessionid",LT_TYPE_STRING, 12, msassessionid);
		sprintf(allgroup,"%s",_ltPubInfo->admSession[atol(msassessionid)].allgroup);
		printf("\n ALLGROUP:%s \n",allgroup);
		if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				/*判断管理员的权限*/
				printf("ALLGROUP:%c \n",allgroup[atol(tempRow[0])]);
				if(allgroup[atol(tempRow[0])]==49){
							if(iHead==1){
								sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s',level:'%d'",tempRow[0],tempRow[3],tempRow[2],level);
								iHead=0;
							}else{
							  sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s',level:'%d'",tempRow[0],tempRow[3],tempRow[2],level);
						  }
						  stralloc_cats(&strTemp,tmpstr);
							pid=atol(tempRow[0]);
							if(level<5){
								msalooparea(&strTemp,level,pid);
							}
							stralloc_cats(&strTemp,"}");
				}
				tempRow= ltDbFetchRow(tempCursor);
			}
			ltDbCloseCursor(tempCursor);
		}
	}else if(type==1){
				if(tempCursor!=NULL){
						tempRow= ltDbFetchRow(tempCursor);
						while(tempRow!=NULL){
						/*判断有没有子节点*/
						if(iHead==1){
							sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s',checked: false",tempRow[0],tempRow[3],tempRow[2]);
							iHead=0;
						}else{
						  sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s',checked: false",tempRow[0],tempRow[3],tempRow[2]);
					  }
					  stralloc_cats(&strTemp,tmpstr);
						stralloc_cats(&strTemp,",leaf: true}");
						tempRow= ltDbFetchRow(tempCursor);
						}
					ltDbCloseCursor(tempCursor);
				}
	}else if(type==3){
						char *dbUser;
						char *dbPass;
						char *dbName;
						char sqlstr[512],groupmanager[1024];
						ltDbConn *Tmp_DbCon;
						dbName=_ltPubInfo->_dbname;
						dbUser=_ltPubInfo->_dbuser;
						dbPass=_ltPubInfo->_dbpass;
						Tmp_DbCon=ltDbConnect(dbUser,dbPass,dbName);
						ltDbCursor *tempCursor1;
						LT_DBROW tempRow1;
						memset(groupmanager,0,sizeof(groupmanager));
						sprintf(sqlstr,"select substr(grouplist1,1,200),substr(grouplist2,1,200),substr(grouplist3,1,200),substr(grouplist4,1,200),substr(grouplist5,1,200) from msaadmuser where name='%s'",name);
						tempCursor1=ltDbOpenCursor(Tmp_DbCon,sqlstr);
						tempRow1= ltDbFetchRow(tempCursor1);
						sprintf(groupmanager,"%s%s%s%s%s",tempRow1[0],tempRow1[1],tempRow1[2],tempRow1[3],tempRow1[4]);
						if(tempCursor!=NULL){
						tempRow= ltDbFetchRow(tempCursor);
						while(tempRow!=NULL){
								if(iHead==1){
									sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s'",tempRow[0],tempRow[3],tempRow[2]);
									iHead=0;
								}else{
								  sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s'",tempRow[0],tempRow[3],tempRow[2]);
							  }
							  stralloc_cats(&strTemp,tmpstr);
							  if(groupmanager[atol(tempRow[0])]==49){
							  	stralloc_cats(&strTemp,",leaf: true,checked: true}");
							  	}else{
							  	stralloc_cats(&strTemp,",leaf: true,checked: false}");
							  	}
								tempRow= ltDbFetchRow(tempCursor);
						}
					ltDbCloseCursor(tempCursor);
					ltDbClose(Tmp_DbCon);
				}
	}else if(type==4){
			if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
			while(tempRow!=NULL){
				/*判断有没有子节点*/
				if(iHead==1){
					sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s',checked:false",tempRow[0],tempRow[3],tempRow[2]);
					iHead=0;
				}else{
				  sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s',checked:false",tempRow[0],tempRow[3],tempRow[2]);
			  }
			  stralloc_cats(&strTemp,tmpstr);
			  pid=atol(tempRow[0]);
				if(level<5){
					msaloopareacheckbox(&strTemp,level,pid);
				}
				stralloc_cats(&strTemp,"}");
				tempRow= ltDbFetchRow(tempCursor);
			}
		}
			ltDbCloseCursor(tempCursor);
	}
	stralloc_cats(&strTemp,"]");
  stralloc_0(&strTemp);
  ltDbClose(G_DbCon);
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	ltMsgFree(ltMsgPk);
	return 0;
}

int msaGroupAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char sqlBuf[1024];
	int  actiontype,iii;
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
	actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));
	if(actiontype==1){/*add*/
			//find free groupid
			for(iii=0;iii<_MAX_ALLGRPNUM;iii++){
				if(strcmp(_ltPubInfo->_GroupList[iii].name,ltMsgGetVar_s(ltMsgPk,"name"))==0){
				  ltMsgPk->msgpktype=1;
					lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"-");
					ltMsgFree(ltMsgPk);
					ltDbClose(G_DbCon);
					return 0;
				}
			}
			for(iii=0;iii<_MAX_ALLGRPNUM;iii++){
				if(strcmp(_ltPubInfo->_GroupList[iii].name,"")==0){
					  char strgroupid[32];
						snprintf(_ltPubInfo->_GroupList[iii].name,64,"%s",ltMsgGetVar_s(ltMsgPk,"name"));
						_ltPubInfo->_GroupList[iii].groupid=iii;
						_ltPubInfo->_GroupList[iii].pid=atoi(ltMsgGetVar_s(ltMsgPk,"pid"));
						_ltPubInfo->_GroupList[iii].level=atoi(ltMsgGetVar_s(ltMsgPk,"level"));
						snprintf(_ltPubInfo->_GroupList[iii].mydesc,64,"%s",ltMsgGetVar_s(ltMsgPk,"mydesc"));
						_ltPubInfo->_GroupList[iii].lastflowcounttime=time(0); 
						memset(sqlBuf,0,sizeof(sqlBuf));
						sprintf(sqlBuf,"insert into msagroup (id,name,level,pid,mydesc) values (%d,'%s',%d,%d,'%s')",iii,ltMsgGetVar_s(ltMsgPk,"name"),atoi(ltMsgGetVar_s(ltMsgPk,"level")),atoi(ltMsgGetVar_s(ltMsgPk,"pid")),ltMsgGetVar_s(ltMsgPk,"mydesc"));
						ltDbExecSql(G_DbCon,sqlBuf);
				 		
					  ltMsgPk->msgpktype=1;
					  sprintf(strgroupid,"%d",iii);
						lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,strgroupid);
						ltMsgFree(ltMsgPk);
						ltDbClose(G_DbCon);
						return 0;
				}
			}
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"-");
			ltMsgFree(ltMsgPk);
			ltDbClose(G_DbCon);
			return 0;

	}else if (actiontype==2){/*del*/
		iii=atoi(ltMsgGetVar_s(ltMsgPk,"id"));
		/*kkk没拿到或为空就不能删除*/
		if(iii>0 &&iii< _MAX_ALLGRPNUM){
			int  jjj;
			
			sprintf(sqlBuf,"delete from msaGroup where id=%d ",iii);
			ltDbExecSql(G_DbCon,sqlBuf);
			
			sprintf(sqlBuf,"update msauser set groupid%d=0 where groupid%d=%d ",_ltPubInfo->_GroupList[iii].level,_ltPubInfo->_GroupList[iii].level,iii);
//printf("sqlBuf:%s\n",sqlBuf);			
			ltDbExecSql(G_DbCon,sqlBuf);
			
			_ltPubInfo->_GroupList[iii].groupid=0;
			_ltPubInfo->_GroupList[iii].pid=0;
			_ltPubInfo->_GroupList[iii].level=0;
			memset(_ltPubInfo->_GroupList[iii].name,0,sizeof(_ltPubInfo->_GroupList[iii].name));
			memset(_ltPubInfo->_GroupList[iii].mydesc,0,sizeof(_ltPubInfo->_GroupList[iii].mydesc));
			
			//重新归档该部门下的用户
			for(jjj=0;jjj<_ltPubInfo->_dbUserInfoHead.bufnum;jjj++){
		 	  if(strlen(_ltDbUserInfoList[jjj].username)>0){
		 	 	  if(_ltDbUserInfoList[jjj].groupid[_ltPubInfo->_GroupList[iii].level]==iii){
		 	 	  	_ltDbUserInfoList[jjj].groupid[_ltPubInfo->_GroupList[iii].level]=0;
		 	 	  }
		 	  }
			}
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
			ltMsgFree(ltMsgPk);
			ltDbClose(G_DbCon);
			return 0;
	  }else{
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
			ltMsgFree(ltMsgPk);
			ltDbClose(G_DbCon);
			return 0;
		}
	}else if (actiontype==3){/*update*/
		iii=atoi(ltMsgGetVar_s(ltMsgPk,"id"));
		if(iii<=0 || iii>=_MAX_ALLGRPNUM){
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
				ltMsgFree(ltMsgPk);
				ltDbClose(G_DbCon);
				return 0;
		}
		if(strcmp(_ltPubInfo->_GroupList[iii].name,ltMsgGetVar_s(ltMsgPk,"name"))==0){
			  ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
				ltMsgFree(ltMsgPk);
				ltDbClose(G_DbCon);
				return 0;
		}
		if(iii>0 && iii<_MAX_ALLGRPNUM){
				memset(sqlBuf,0,sizeof(sqlBuf));
				sprintf(sqlBuf,"update msaGroup set name='%s',mydesc='%s'where id=%d ",
			  ltMsgGetVar_s(ltMsgPk,"name"),ltMsgGetVar_s(ltMsgPk,"mydesc"),iii);
				ltDbExecSql(G_DbCon,sqlBuf);
				
				snprintf(_ltPubInfo->_GroupList[iii].name,64,"%s",ltMsgGetVar_s(ltMsgPk,"name"));
				snprintf(_ltPubInfo->_GroupList[iii].mydesc,64,"%s",ltMsgGetVar_s(ltMsgPk,"mydesc"));
				
				ltMsgPk->msgpktype=1;
				lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
				ltMsgFree(ltMsgPk);
				ltDbClose(G_DbCon);
				return 0;
		}
	}
	return 0;
}

int msaPolicyTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	   char tmpstr[512];
		 char tempname[32];
	   int i,type,uid;
	   stralloc strTemp;
	   int iHead;
	   type=0;
	   uid=-1;
	   memset(tmpstr,0,sizeof(tmpstr));
	   iHead=1;
	   strTemp.s=0;
	 	 if(ltMsgGetVar_s(ltMsgPk,"uid")!=NULL){
	   		uid=atol(ltMsgGetVar_s(ltMsgPk,"uid"));
	 	 }
	   if(ltMsgGetVar_s(ltMsgPk,"type")!=NULL){
	   		type=atol(ltMsgGetVar_s(ltMsgPk,"type"));
	 	 }
  	 char policyid[129];
 		 nasDbUserInfo *tmpdbuserinfo;
		 memset(policyid,0,sizeof(policyid));
 		 for(i=0;i<_ltPubInfo->_dbUserInfoHead.bufnum;i++){
 		 		tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,i);
     		if(tmpdbuserinfo!=NULL&&tmpdbuserinfo->userid==uid){
     			memcpy(policyid,tmpdbuserinfo->policyid,128);
					break;
     		}
   	 }
	   stralloc_cats(&strTemp,"[");
		 if(type==0){
				   for(i=0;i<_MAX_ALLPOLICY;i++){
					   	if(strcmp(_ltPubInfo->_PolicyList[i].pname,"")!=0){
					   		memset(tempname,0,sizeof(tempname));
					   		sprintf(tempname,"%s",_ltPubInfo->_PolicyList[i].pname);
								if(iHead==1){
									 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true}",i,tempname);
									 iHead=0;
								}else{
									 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true}",i,tempname);
								}
								stralloc_cats(&strTemp,tmpstr);
				     }
				   }
		 }else if(type==1){
		 	 		char topPolicyNameList[_MAX_ALLPOLICY];
		 	 		nasDbUserInfo *tmpdbuserinfo;
		 	 		for(i=0;i<_ltPubInfo->_dbUserInfoHead.bufnum;i++){
			 		 	tmpdbuserinfo=msaLookUpDbUserById(_ltPubInfo,i);
		       	if(tmpdbuserinfo!=NULL&&tmpdbuserinfo->userid==uid){
		       		memset(topPolicyNameList,0,sizeof(topPolicyNameList));
							memcpy(topPolicyNameList,tmpdbuserinfo->policyid,_MAX_ALLPOLICY);
		       	 }
	       	}
	 	 			for(i=0;i<_MAX_ALLPOLICY;i++){
				   	if(strcmp(_ltPubInfo->_PolicyList[i].pname,"")!=0){
				   		memset(tempname,0,sizeof(tempname));
				   		sprintf(tempname,"%s",_ltPubInfo->_PolicyList[i].pname);
				   		if(topPolicyNameList[i]==48){
								if(iHead==1)
								{
									 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked: false}",i,tempname);
									 iHead=0;
								}
								else
								{
									 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked: false}",i,tempname);
								}
							}else if(topPolicyNameList[i]==49){
								if(iHead==1)
								{
									 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
									 iHead=0;
								}
								else
								{
									 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked: true}",i,tempname);
								}
							}
							stralloc_cats(&strTemp,tmpstr);
			   			}
			   		}
		 }else if(type==2){
				   for(i=0;i<_MAX_ALLPOLICY;i++){
					   	if(strcmp(_ltPubInfo->_PolicyList[i].pname,"")!=0){
					   		memset(tempname,0,sizeof(tempname));
					   		sprintf(tempname,"%s",_ltPubInfo->_PolicyList[i].pname);
								if(iHead==1){
									 sprintf(tmpstr,"{id: '%d',text: '%s',leaf: true,checked:true}",i,tempname);
									 iHead=0;
								}else{
									 sprintf(tmpstr,",{id: '%d',text: '%s',leaf: true,checked:true}",i,tempname);
								}
								stralloc_cats(&strTemp,tmpstr);
				     }
				   }
		 }
	   stralloc_cats(&strTemp,"]");
	   stralloc_0(&strTemp);
	   ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	   ltMsgFree(ltMsgPk);
	   return 0;
}

int msaPolicyAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char sqlBuf[1024];
	int  actiontype,jjj;
	
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
	
	actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));
	if(actiontype==1){/*add*/
		//find free policyid
		char *addpname;
		addpname=ltMsgGetVar_s(ltMsgPk,"name");
		for(jjj=0;jjj<_MAX_ALLPOLICY;jjj++){
			 if(strcmp(_ltPubInfo->_PolicyList[jjj].pname,addpname)==0){
				  ltMsgPk->msgpktype=1;
					lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"-");
					ltMsgFree(ltMsgPk);
					ltDbClose(G_DbCon);
					return 0;
			 }
		}
		for(jjj=0;jjj<_MAX_ALLPOLICY;jjj++){
			 if(strcmp(_ltPubInfo->_PolicyList[jjj].pname,"")==0){
			 	  char restrid[32];
					snprintf(_ltPubInfo->_PolicyList[jjj].pname,64,"%s",addpname);
					_ltPubInfo->_PolicyList[jjj].pid=jjj;
					snprintf(_ltPubInfo->_PolicyList[jjj].pdesc,64,"%s",ltMsgGetVar_s(ltMsgPk,"mydesc"));
					memset(sqlBuf,0,sizeof(sqlBuf));
					sprintf(sqlBuf,"insert into msapolicy(pid,pname,pdesc)values (%d,'%s','%s')",jjj,ltMsgGetVar_s(ltMsgPk,"name"),ltMsgGetVar_s(ltMsgPk,"mydesc"));				
					ltDbExecSql(G_DbCon,sqlBuf);
					//policyid=jjj;
			 		ltMsgPk->msgpktype=1;
			 		sprintf(restrid,"%d",jjj);
					lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,restrid);
					ltMsgFree(ltMsgPk);
					ltDbClose(G_DbCon);
					return 0;
			 }
		}
		ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"-");
		ltMsgFree(ltMsgPk);
		return 0;
	}else if (actiontype==2){/*del*/
		jjj=atoi(ltMsgGetVar_s(ltMsgPk,"id"));
		if(jjj>0 && jjj<_MAX_ALLPOLICY){
			sprintf(sqlBuf,"delete from msapolicy where pid=%d ",jjj);
			ltDbExecSql(G_DbCon,sqlBuf);
			
			_ltPubInfo->_GroupList[jjj].pid=0;
			memset(_ltPubInfo->_PolicyList[jjj].pname,0,sizeof(_ltPubInfo->_PolicyList[jjj].pname));
			memset(_ltPubInfo->_PolicyList[jjj].pdesc,0,sizeof(_ltPubInfo->_PolicyList[jjj].pdesc));
			
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
			ltMsgFree(ltMsgPk);
			ltDbClose(G_DbCon);
			return 0;
	  }else{
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
			ltMsgFree(ltMsgPk);
			ltDbClose(G_DbCon);
			return 0;
	  }
	}else if (actiontype==3){/*update*/
	  jjj=atoi(ltMsgGetVar_s(ltMsgPk,"id"));
	  if(jjj<=0 || jjj>=_MAX_ALLPOLICY){
	  	ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
			ltMsgFree(ltMsgPk);
			ltDbClose(G_DbCon);
			return 0;
	  }
		if(strcmp(ltMsgGetVar_s(ltMsgPk,"name"),_ltPubInfo->_PolicyList[jjj].pname)==0){
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
			ltMsgFree(ltMsgPk);
			ltDbClose(G_DbCon);
			return 0;
		}
		if(strcmp(ltMsgGetVar_s(ltMsgPk,"id"),"")!=0){
			memset(sqlBuf,0,sizeof(sqlBuf));
			sprintf(sqlBuf,"update msapolicy set pname='%s',pdesc='%s'where pid=%d",
		  ltMsgGetVar_s(ltMsgPk,"name"),
		  ltMsgGetVar_s(ltMsgPk,"mydesc"),
		  jjj);
			ltDbExecSql(G_DbCon,sqlBuf);
			
			//policyid=_ltPubInfo->_PolicyList[jjj].pid;
			snprintf(_ltPubInfo->_PolicyList[jjj].pname,64,"%s",ltMsgGetVar_s(ltMsgPk,"name"));
			snprintf(_ltPubInfo->_PolicyList[jjj].pdesc,64,"%s",ltMsgGetVar_s(ltMsgPk,"mydesc"));
			
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
			ltMsgFree(ltMsgPk);
			ltDbClose(G_DbCon);
			return 0;
		}
	}
	ltDbClose(G_DbCon);
	return 0;
}
//显示部门用户列表模板
int msaShowClientList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	checkTimeOut(confd,ltMsgPk,lt_MMHead);
	 char *gid;
	 char *gnmlevel;
	 int  level;
   ltDbHeadPtr dbPtr;
   char  strlevel[32];
   
   gid="";
   gnmlevel="0";
   if(ltMsgGetVar_s(ltMsgPk,"gid")){
   		gnmlevel=ltMsgGetVar_s(ltMsgPk,"level");
   		gid=ltMsgGetVar_s(ltMsgPk,"gid");
   }
   
   char caPolicy[129];
   int  pid;
   memset(caPolicy,0,129);
   if(!ltMsgGetVar_s(ltMsgPk,"pid")){
   	 sprintf(caPolicy,"%s","11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111");
   }else{
	   pid=atol(ltMsgGetVar_s(ltMsgPk,"pid"));
	   sprintf(caPolicy,"%s","00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
	   if(pid<128 && pid>-1){
	  	caPolicy[pid]='1';
	   }else{
	  	caPolicy[0]='1';
	   }
	 }

   
	 level=0;
	 if(!gnmlevel){
	 		gnmlevel="0";
	 }
	 if(!gid){
	 		gid="";
	 }
	 level=atol(gnmlevel);
	 if(level<0 || level>=_MAX_GROUPLEVEL ){
	 	 level=0;
	}
   sprintf(strlevel,"%d",level);
	 dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");

   lt_dbput_rootvars(dbPtr,1,"gid",gid);
   lt_dbput_rootvars(dbPtr,1,"level",strlevel);
   lt_dbput_rootvars(dbPtr,1,"policy",caPolicy);
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/users/clientlistinfo.htm",dbPtr->head,0);    
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}
	 
	 
	 
//用户列表
int msaClientList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char *delflag;
	 char sqlStr[1024];
   
	 char *gid;
	 char *pid;
	 char *glevel;
	 int  level;
	 char *keyword;
	 char *userflag;
	 
	 struct in_addr s;//IP
	 char strip[32];
	 char strmac[32];
   int k,g,iii,jjj,kkk;
   char caTmpp[1024];
   stralloc strTemp;
   
   
	 nasDbUserInfo *tmpdbuserinfo;
	 
   long lRowNum,lStartRec,allcount;
   int int_gid,int_pid;
   char addtime[64],lasttime[64],mypid[129]; 
   
   delflag=NULL;
   if(ltMsgGetVar_s(ltMsgPk,"delflag")!=NULL){
	 	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 }
	 
	 if(delflag){
	 	   
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
	 	   
	 		char *uid=ltMsgGetVar_s(ltMsgPk,"uid");
	 		sprintf(sqlStr,"delete from msauser where userid='%s'",uid);
	 		
//printf("sqlStr:%s\n",sqlStr);
	 		
	 		ltDbExecSql(G_DbCon,sqlStr);
	 		
	 		//删除所有该用户的在线用户
	 		
	 		msaHashDelDbUserById(_ltPubInfo,atol(uid));

	 		
	 		ltDbClose(G_DbCon);
	 		
	 		ltMsgPk->msgpktype=1;
			 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
			 ltMsgFree(ltMsgPk);
			 
			 return 0;
	 }
   
	 lRowNum=100; //每页的行数
	 allcount=0; 
	 lStartRec=0;
	 
	 if(ltMsgGetVar_s(ltMsgPk,"limit")){
	   lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	   if(lRowNum<=0 || lRowNum>500){
	   	lRowNum=100;
	   }
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"start")){
	   lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	   if(lStartRec<0){
	   	lStartRec=0;
	   }
	 }
//printf("lStartRec:%d lRowNum:%d\n",lStartRec,lRowNum);

	 
	 k=0;
	 g=0;
   allcount=0;
   level=0;
   gid=ltMsgGetVar_s(ltMsgPk,"gid");  
   memset(mypid,0,sizeof(mypid));
   
   pid=ltMsgGetVar_s(ltMsgPk,"pid");
   if(strlen(pid)==128){//多策略组
   	 sprintf(mypid,"%s",pid);
   	 int_pid=-1;
   }else{//单策略组
     if(strlen(pid)==0){
     	 	memset(mypid,'1',_MAX_ALLPOLICY);
     	 	int_pid=-1;
     }else{
	   	 int_pid=atoi(pid);
	   	 if(int_pid>=0 && int_pid<_MAX_ALLPOLICY ){
	 				memset(mypid,'0',_MAX_ALLPOLICY);
	 				mypid[int_pid]='1';
	 		 }	
 		 }	
   }
//printf("pid:%s mypid:%s\n",pid,mypid);
 		 
   if(strlen(ltMsgGetVar_s(ltMsgPk,"gid"))>0){
	 	 gid=ltMsgGetVar_s(ltMsgPk,"gid");
	 	 int_gid=atoi(gid);
	 }else{
	 	 int_gid=-1;
	 }

//printf("gid:%s int_gid:%d\n",gid,int_gid);
	 
	 if(ltMsgGetVar_s(ltMsgPk,"level")){
	 	 glevel=ltMsgGetVar_s(ltMsgPk,"level");
	 	 level=atoi(glevel);
	 }else{
	 	 level=0;
	 }
//printf("glevel:%s level:%d\n",glevel,level);	 	 
	 
	 if(level<0 || level>=_MAX_GROUPLEVEL){
	 	 level=0;
	 }
	 
	 if(ltMsgGetVar_s(ltMsgPk,"keyword")){
	   keyword=ltMsgGetVar_s(ltMsgPk,"keyword");
	 }else{
	 	 keyword="";
	 }
//printf("keyword:%s\n",keyword);	 
	 
	 int user_flag=-1;
	 if(ltMsgGetVar_s(ltMsgPk,"userflag")){
	   userflag=ltMsgGetVar_s(ltMsgPk,"userflag");
	   user_flag=atoi(userflag);
	 }
//printf("userflag:%s user_flag:%d\n",userflag,user_flag);	

   //用户列表
	 tmpdbuserinfo=malloc(lRowNum*sizeof(nasDbUserInfo));//定义数组先是当页面的内容 
	 memset(tmpdbuserinfo,0,lRowNum*sizeof(nasDbUserInfo));
	 
	 for(jjj=0;jjj<_ltPubInfo->_dbUserInfoHead.bufnum;jjj++){	
		 	 
		 	 if(strlen(_ltDbUserInfoList[jjj].username)==0){
		 	 	 continue;
		 	 }
     	//部门筛选
	 	  if(int_gid!=-1 ){
	 	  	if( strlen(_ltPubInfo->_GroupList[int_gid].name)>0 ){
	 	  		if(_ltDbUserInfoList[jjj].groupid[level]!=int_gid){ 
              continue;
          }
        }
	 		}
	 			
      //策略筛选      
 			if(strlen(mypid)>0){
 				 for(iii=0;iii<_MAX_ALLPOLICY;iii++){	
	 				 if(_ltDbUserInfoList[jjj].policyid[iii]=='1'&& mypid[iii]=='1'){
	 				  goto passpolicy;
	 				 }	 					
 				}
 				continue;
 			}

passpolicy: 			
 						
 		  //关键字筛选
 			if(strlen(keyword)>0){ 				
				s.s_addr = htonl(_ltDbUserInfoList[jjj].bindip);
				sprintf(strip,"%s",inet_ntoa(s));			   		  
   		  if(strstr(_ltDbUserInfoList[jjj].username,keyword)==0 && 
 			     strstr(_ltDbUserInfoList[jjj].dispname,keyword)==0 &&
 			     strstr(strip,keyword)==0 && 
 			     strstr(_ltDbUserInfoList[jjj].bindemail,keyword)==0){
 			     	 	  	continue;	     
	 		  }
 		  }
 		  		  
 			//用户状态筛选
 			if(user_flag!=-1){
 				if(user_flag==1){//免监空的
 					if((int)_ltDbUserInfoList[jjj].useflags!=99){
	 					continue;
	 				}
 				}
	 			if(user_flag==0){//禁止访问的
 					if((int)_ltDbUserInfoList[jjj].useflags!=100){
	 					continue;
	 				}
 				}
 				
 				if(user_flag==2){//启动有个人QoS通道的用户
 					if((int)_ltDbUserInfoList[jjj].ratelimit==0){
	 					continue;
	 				}
 				}
 				
 				if(user_flag==3){//启动有天流量限制的用户
 					if((int)_ltDbUserInfoList[jjj].lLimitBytes==0){
	 					continue;
	 				}
 				}
 				
 				if(user_flag==4){//启动有天时间限制的用户
 					if((int)_ltDbUserInfoList[jjj].lLimitTimes==0){
	 					continue;
	 				}
 				}
 				
 				if(user_flag==5){//启动计费服务的用户
 					if((int)_ltDbUserInfoList[jjj].jftype==0){
	 					continue;
	 				}
 				}
 				
 			}
 			
      allcount++;//记录累加
      
      if(allcount>lStartRec && k<lRowNum){     	
         memcpy(&tmpdbuserinfo[k],&_ltDbUserInfoList[jjj],sizeof(nasDbUserInfo));
         k++;
	    }
//	    if(k>=lRowNum){
//	       break;	
//	    }  
	 }
  
  
  
  //json构建
  memset(addtime,0,sizeof(addtime));
  memset(lasttime,0,sizeof(lasttime));
  
  strTemp.s=0;
  sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",allcount);
  stralloc_cats(&strTemp,caTmpp);
  
  for(jjj=0;jjj<k;jjj++){
  	if(strlen(tmpdbuserinfo[jjj].username)>0){
  		
				  	printf("username  %s\n",tmpdbuserinfo[jjj].username);
		    	  printf("%u|%x \n",tmpdbuserinfo[jjj].bindip[0],tmpdbuserinfo[jjj].bindip[0]);
		    	  printf("%u|%x \n",tmpdbuserinfo[jjj].bindip[1],tmpdbuserinfo[jjj].bindip[1]);
		    	  printf("%u|%x \n",tmpdbuserinfo[jjj].bindip[2],tmpdbuserinfo[jjj].bindip[2]);
		    	  printf("%u|%x \n",tmpdbuserinfo[jjj].bindip[3],tmpdbuserinfo[jjj].bindip[3]);
	  		  //Ip转换
					if(tmpdbuserinfo[jjj].bindip[0]==0&&tmpdbuserinfo[jjj].bindip[1]==0&&tmpdbuserinfo[jjj].bindip[2]==0 ){//ipv4
				    printf("IPV4\n");
				    s.s_addr = htonl(tmpdbuserinfo[jjj].bindip[3]);
				    sprintf(strip,"%s",inet_ntoa(s));
				  }else{//ipv6
				  	printf("IPV6\n");		    	  
				    tmpdbuserinfo[jjj].bindip[0]=htonl(tmpdbuserinfo[jjj].bindip[0]);
				    tmpdbuserinfo[jjj].bindip[1]=htonl(tmpdbuserinfo[jjj].bindip[1]);
				    tmpdbuserinfo[jjj].bindip[2]=htonl(tmpdbuserinfo[jjj].bindip[2]);
				    tmpdbuserinfo[jjj].bindip[3]=htonl(tmpdbuserinfo[jjj].bindip[3]);
				   inet_ntop(AF_INET6, (void *)(tmpdbuserinfo[jjj].bindip), strip, 64);
				  }
					//mac转换
		      nasCvtMac(tmpdbuserinfo[jjj].bindmac,strmac);
		      
		      //部门名称处理
		      char groupname[1024];
		      memset(groupname,0,sizeof(groupname));

					sprintf(groupname,"%s",_ltPubInfo->_GroupList[(int)tmpdbuserinfo[jjj].groupid[0]].name);
					for(kkk=1;kkk<_MAX_GROUPLEVEL;kkk++){	
						 if((int)tmpdbuserinfo[jjj].groupid[kkk]==0){
						 	   break;
						 }else if((int)tmpdbuserinfo[jjj].groupid[kkk]>0){
 						  	sprintf(groupname,"%s/%s",groupname,_ltPubInfo->_GroupList[(int)tmpdbuserinfo[jjj].groupid[kkk]].name);
             } 		
					}
					
					
		      //策略名称处理
		      char policyname[256];
		      memset(policyname,0,sizeof(policyname));
//		      if(int_pid!=-1){//单策略组
//		      	  if(tmpdbuserinfo[jjj].policyid[kkk]=='0'){
//		      		 continue;
//		      	  }
//		      	  if(tmpdbuserinfo[jjj].policyid[kkk]==49){
//		      	   sprintf(policyname,"%s",_ltPubInfo->_PolicyList[int_pid].pname);		      	 
//					    }
//				  }else
				  //多策略组
						for(kkk=0;kkk<_MAX_ALLPOLICY;kkk++){ 	 
							if(tmpdbuserinfo[jjj].policyid[kkk]=='1'){
						 	 if(strcmp(_ltPubInfo->_PolicyList[kkk].pname,"")){
						 	 	 if(strlen(policyname)==0){
						 	 	 	 snprintf(policyname,255,"%s",_ltPubInfo->_PolicyList[kkk].pname);		
						 	 	 }else{
						 	 	 	 sprintf(sqlStr,"%s",policyname);
						 	 	 	 snprintf(policyname,255,"%s;%s",sqlStr,_ltPubInfo->_PolicyList[kkk].pname);				 	      	 	      
						 	   }				 	    
						 	 }	
						 }			 	   				 	 				   						    				
				    }
			    
							      
		      //添加时间处理
					//nasTimeLongFormat(addtime,tmpdbuserinfo[jjj].addtime);
					
					sprintf(addtime,ltTimeFormat("%Y-%m-%d %H:%M:%S",tmpdbuserinfo[jjj].addtime));
					
					//最后上线时间处理
					if(tmpdbuserinfo[jjj].lasttime){
						//nasTimeLongFormat(lasttime,tmpdbuserinfo[jjj].lasttime);
						sprintf(lasttime,ltTimeFormat("%Y-%m-%d %H:%M:%S",tmpdbuserinfo[jjj].lasttime));
					}else{
						sprintf(lasttime,"%s","");
					}
									 
	   			if(g==0){
	     	  	 sprintf(caTmpp,"{\"userid\":\"%d\",\"username\":\"%s\",\"dispname\":\"%s\",\"groupname\":\"%s\",\"policyname\":\"%s\",\"bindip\":\"%s\",\"bindmac\":\"%s\",\"lasttime\":\"%s\",\"useraddtime\":\"%s\",\"bindemail\":\"%s\",\"userstate\":\"%d\"}",tmpdbuserinfo[jjj].userid,tmpdbuserinfo[jjj].username,tmpdbuserinfo[jjj].dispname,groupname,policyname,strip,strmac,lasttime,addtime,tmpdbuserinfo[jjj].bindemail,tmpdbuserinfo[jjj].useflags);//
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	 sprintf(caTmpp,",{\"userid\":\"%d\",\"username\":\"%s\",\"dispname\":\"%s\",\"groupname\":\"%s\",\"policyname\":\"%s\",\"bindip\":\"%s\",\"bindmac\":\"%s\",\"lasttime\":\"%s\",\"useraddtime\":\"%s\",\"bindemail\":\"%s\",\"userstate\":\"%d\"}",tmpdbuserinfo[jjj].userid,tmpdbuserinfo[jjj].username,tmpdbuserinfo[jjj].dispname,groupname,policyname,strip,strmac,lasttime,addtime,tmpdbuserinfo[jjj].bindemail,tmpdbuserinfo[jjj].useflags);//
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }
	     	  g++;
   	}
  }
   stralloc_cats(&strTemp,"]}");
   stralloc_0(&strTemp);
   
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 
	 return 0;
	 
}
