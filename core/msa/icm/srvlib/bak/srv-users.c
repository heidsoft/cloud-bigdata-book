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
#include "icm.h"
/*策略树*/

int icmPolicyTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){	
	   char sqlBuf[512];
	   char tmpstr[512];
	   ltDbCursor *tempCursor;
	   LT_DBROW tempRow;
	   stralloc strTemp;
	   int iHead;
	   memset(tmpstr,0,sizeof(tmpstr));	 
	   iHead=1;
	   strTemp.s=0;
	   stralloc_cats(&strTemp,"[");
	   sprintf(sqlBuf,"select uid,username,ipadd from icmuserinfo order by uid asc");
	   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	   if(tempCursor!=NULL)
	   	{
	       tempRow= ltDbFetchRow(tempCursor);
	       while(tempRow!=NULL)
			   {
						/*判断有没有子节点*/
						if(iHead==1)
						{
							 sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s',leaf: true}",tempRow[0],tempRow[2],tempRow[1]);//qtip:鼠标悬浮时显示提示信息
							 iHead=0;
						}
						else
						{
							 sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s',leaf: true}",tempRow[0],tempRow[2],tempRow[1]);
						}
						stralloc_cats(&strTemp,tmpstr);       
						tempRow= ltDbFetchRow(tempCursor);
	        }
	        ltDbCloseCursor(tempCursor);
	    }
	   stralloc_cats(&strTemp,"]");
	   stralloc_0(&strTemp);
	   ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	   ltMsgFree(ltMsgPk);
	   return 0;
}
/*显示用户信息模板*/
int icmTermInfoView(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{   
		char strBuf[2048];
    char strGroupPath[512];
    ltDbHeadPtr dbPtr;
    ltDbCursor  *tempCursor;
    LT_DBROW    tempRow;
    int uid;
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    uid=atol(ltMsgGetVar_s(ltMsgPk,"uid"));
    memset(strGroupPath,0,sizeof(strGroupPath));
    sprintf(strBuf,"select uid,username,dispname,groupid0,groupid1,groupid2,groupid3,groupid4,ipadd,macadd,email,telphone,endtime,status from icmuserinfo where uid=%d",uid);
		printf("sql:%s\n", strBuf);
   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
   

   if(tempCursor!=NULL){
               tempRow= ltDbFetchRow(tempCursor);
						  if(tempRow!=NULL){ 
						  	   char status1[16],status2[16];
									if(strcmp(tempRow[13],"0")==0)
									{ 
										      sprintf(status1,"%s","selected");
													sprintf(status2,"%s","");
									}else if(strcmp(tempRow[13],"1")==0)
									{
													sprintf(status2,"%s","selected");
													sprintf(status1,"%s","");
								  }
				   
                          lt_dbput_rootvars(dbPtr,9,
                            			 "username",tempRow[2],
                                   "dispname",tempRow[3],
                                   "ipadd",tempRow[8],
                                   "macadd",tempRow[9],
                                   "email",tempRow[10],
                                   "telphone",tempRow[11],
                                   "endtime",tempRow[12],
                                   "status1",status1,
                                   "status2",status2
                                   );
               }
        ltDbCloseCursor(tempCursor);
   }

 /*全局变量*/
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/icm/htmlplt/users/icmterminfodetail.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}


/*显示添加用户模板*/
int icmShowAddClient(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		
  ltDbHeadPtr dbPtr; 
  dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
  lt_dbput_rootvars(dbPtr,1,"id",ltMsgGetVar_s(ltMsgPk,"id"));
  ltMsgPk->msgpktype=1;
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/icm/htmlplt/users/icmterminfonew.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;
}

/*添加用户*/
int icmTerminfoAdd(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{   
    char sqlBuf[2048];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    int  nowgid;
    int  nowglevel;
    int  pgid[_MAX_GROUPLEVEL];
    int  iii;
    int  newgid,type;
   	type=atol(ltMsgGetVar_s(ltMsgPk,"type"));
   	
   	if(type==1){
   		
						    for(iii=0;iii<_MAX_GROUPLEVEL;iii++){
						                 pgid[iii]=0;
						       }
						     nowglevel=0;
						     nowgid=atol(ltMsgGetVar_s(ltMsgPk,"id"));
								 sprintf(sqlBuf,"select pid,level from icmGroup where id=%d ",nowgid);
						     tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
						     if(tempCursor){
						              tempRow= ltDbFetchRow(tempCursor);
						              if(tempRow){
						                 nowglevel=atol(tempRow[1]);
						              }
						              ltDbCloseCursor(tempCursor);
						     }
						     if(nowglevel>=_MAX_GROUPLEVEL){
						              ltMsgPk->msgpktype=1;
						       ltWebMsgErr(_ltPubInfo->pubMsgStr[7].conStr, confd,ltMsgPk);
						       ltMsgFree(ltMsgPk);
						              return 0;
						     }
						     for(iii=nowglevel;iii>-1;iii--){
						  						sprintf(sqlBuf,"select pid from icmGroup where id=%d and level=%d",nowgid,iii);
						              tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
						              if(tempCursor){
						                       tempRow= ltDbFetchRow(tempCursor);
						                       if(tempRow){
						                                          pgid[iii]=nowgid;
						                                          nowgid=atol(tempRow[0]);
						                       }
						                       ltDbCloseCursor(tempCursor);
						              }
						           }
						           /*生成序号,先cunt(*) maxid */
						           newgid=0;
						           sprintf(sqlBuf,"select max(uid)+1 from icmuserinfo");
						                   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
						                   if(tempCursor){
						                            tempRow= ltDbFetchRow(tempCursor);
						                            if(tempRow){
						                                newgid=atol(tempRow[0]);
						                            }
						                            ltDbCloseCursor(tempCursor);
						                   }
           
									sprintf(sqlBuf,"insert into icmuserinfo (uid,username,dispname,groupid0,groupid1,groupid2,groupid3,groupid4,ipadd,macadd,email,telphone,endtime,status) values(%d,'%s','%s',%d,%d,%d,%d,%d,'%s','%s','%s','%s','%s',%s)",
									newgid,ltMsgGetVar_s(ltMsgPk,"username"),ltMsgGetVar_s(ltMsgPk,"dispname"),pgid[0],pgid[1],pgid[2],pgid[3],pgid[4],
									ltMsgGetVar_s(ltMsgPk,"ipadd"),ltMsgGetVar_s(ltMsgPk,"macadd"),ltMsgGetVar_s(ltMsgPk,"email"),ltMsgGetVar_s(ltMsgPk,"telphone"),ltMsgGetVar_s(ltMsgPk,"edate"),ltMsgGetVar_s(ltMsgPk,"status"));
									
									printf("INSERT:%s\n",sqlBuf);
									ltDbExecSql(G_DbCon,sqlBuf);
									char uid[32];
									sprintf(uid,"%d",newgid);
									
									char status1[16],status2[16];
									if(strcmp(ltMsgGetVar_s(ltMsgPk,"status"),"0")==0)
									{ 
										      sprintf(status1,"%s","selected");
													sprintf(status2,"%s","");
									}else if(strcmp(ltMsgGetVar_s(ltMsgPk,"status"),"1")==0)
									{
													sprintf(status2,"%s","selected");
													sprintf(status1,"%s","");
								  }
									
									ltDbHeadPtr dbPtr;
									dbPtr=lt_dbinit();
    							lt_db_htmlpage(dbPtr,"utf-8");
									lt_dbput_rootvars(dbPtr,10,
												 "uid",uid,
                  			 "username",ltMsgGetVar_s(ltMsgPk,"username"),
                         "dispname",ltMsgGetVar_s(ltMsgPk,"dispname"),
                         "ipadd",ltMsgGetVar_s(ltMsgPk,"ipadd"),
                         "macadd",ltMsgGetVar_s(ltMsgPk,"macadd"),
                         "email",ltMsgGetVar_s(ltMsgPk,"email"),
                         "telphone",ltMsgGetVar_s(ltMsgPk,"telphone"),
                         "status1",status1,
                         "status2",status2,
                         "endtime",ltMsgGetVar_s(ltMsgPk,"edate")
                                   );
                  ltMsgPk->msgpktype=1;
									lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/icm/htmlplt/users/icmterminfodetail.htm",dbPtr->head,0);
									ltMsgFree(ltMsgPk);
									lt_dbfree(dbPtr);
					 }else if(type==0)
					 	{			sprintf(sqlBuf,"update icmuserinfo set username='%s',dispname='%s',ipadd='%s',macadd='%s',email='%s',telphone='%s',endtime='%s',status=%s where uid=%s",
									ltMsgGetVar_s(ltMsgPk,"username"),ltMsgGetVar_s(ltMsgPk,"dispname"),ltMsgGetVar_s(ltMsgPk,"ipadd"),ltMsgGetVar_s(ltMsgPk,"macadd"),ltMsgGetVar_s(ltMsgPk,"email"),ltMsgGetVar_s(ltMsgPk,"telphone"),ltMsgGetVar_s(ltMsgPk,"edate"),ltMsgGetVar_s(ltMsgPk,"status"),ltMsgGetVar_s(ltMsgPk,"uid"));
					 		
									printf("UPDATE:%s\n",sqlBuf);
									ltDbExecSql(G_DbCon,sqlBuf);
									char status1[16],status2[16];
									if(strcmp(ltMsgGetVar_s(ltMsgPk,"status"),"0")==0)
									{ 
										      sprintf(status1,"%s","selected");
													sprintf(status2,"%s","");
									}else if(strcmp(ltMsgGetVar_s(ltMsgPk,"status"),"1")==0)
									{
													sprintf(status2,"%s","selected");
													sprintf(status1,"%s","");
								  }
									ltDbHeadPtr dbPtr;
									dbPtr=lt_dbinit();
    							lt_db_htmlpage(dbPtr,"utf-8");
									lt_dbput_rootvars(dbPtr,10,
												 "uid",ltMsgGetVar_s(ltMsgPk,"uid"),
                  			 "username",ltMsgGetVar_s(ltMsgPk,"username"),
                         "dispname",ltMsgGetVar_s(ltMsgPk,"dispname"),
                         "ipadd",ltMsgGetVar_s(ltMsgPk,"ipadd"),
                         "macadd",ltMsgGetVar_s(ltMsgPk,"macadd"),
                         "email",ltMsgGetVar_s(ltMsgPk,"email"),
                         "telphone",ltMsgGetVar_s(ltMsgPk,"telphone"),
                         "status1",status1,
                         	"status2",status2,
                         "endtime",ltMsgGetVar_s(ltMsgPk,"edate")
                                   );
                  ltMsgPk->msgpktype=1;
									lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/icm/htmlplt/users/icmterminfodetail.htm",dbPtr->head,0);
									ltMsgFree(ltMsgPk);
									lt_dbfree(dbPtr);
					  }
									return 0;
}
