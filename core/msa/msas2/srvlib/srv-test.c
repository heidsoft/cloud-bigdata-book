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
#include "msas.h"
/*
*注册后查看所有用户
*/

int msasTestApp(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char *name;
	char *password;
	char *rePass;
    char myre[1024];
	char caStr[2048];
	char sqlBuf[2048];
    int  maxuid;

    ltDbCursor *tempCursor;
    LT_DBROW   tempRow;

    name=ltMsgGetVar_s(ltMsgPk,"txtName");
    if(!name){
         name="";
   }
	password=ltMsgGetVar_s(ltMsgPk,"txtPass");
    if(!password){
         password="";
   }
   rePass=ltMsgGetVar_s(ltMsgPk,"txtRePass");
    if(!rePass){
         rePass="";
   }
   if(strcmp(password,rePass)!=0){
       ltMsgPk->msgpktype=1;
       lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n",
				"lthtml",LT_TYPE_STRING,"error!");   
      ltMsgFree(ltMsgPk);
      return 0;	
   }
   
   maxuid=1;
   sprintf(sqlBuf,"select max(uid)+1 from msasusertest" );
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);   
   if(tempCursor!=NULL){
          tempRow= ltDbFetchRow(tempCursor);
          if(tempRow!=NULL){
			  maxuid=atol(tempRow[0]);	
          }
     ltDbCloseCursor(tempCursor);
   }


   sprintf(sqlBuf,"insert into msasusertest values (%d,'%s','%s')",maxuid,name,password);
   ltDbExecSql(G_DbCon,sqlBuf); 
   
   memset(caStr,0,sizeof(caStr));
   sprintf(sqlBuf,"select * from msasusertest" );
   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);   
   if(tempCursor!=NULL){ 
          tempRow= ltDbFetchRow(tempCursor);
		  //sprintf("%s%s%s",_ltPubInfo->pubMsgStr[1].conStr,_ltPubInfo->pubMsgStr[2].conStr,_ltPubInfo->pubMsgStr[3].conStr); 
          while(tempRow!=NULL){
			 sprintf(caStr,"%s%s %s %s\r\n<br>",caStr,tempRow[0],tempRow[1],tempRow[2]);
			 tempRow= ltDbFetchRow(tempCursor);
          }
        ltDbCloseCursor(tempCursor);
   }

	ltMsgPk->msgpktype=1;
    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n",
				"lthtml",LT_TYPE_STRING,caStr);   
    ltMsgFree(ltMsgPk);
    return 0;
} 
/*
*显示输入的用户名和密码
*/
int msasTestApp1(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char *name;
	char *password;
	char *rePass;
    char myre[1024];
	char caStr[2048];
	char sqlBuf[2048];
    name=ltMsgGetVar_s(ltMsgPk,"txtName");
    if(!name){
         name="";
   }
	password=ltMsgGetVar_s(ltMsgPk,"txtPass");
    if(!password){
         password="";
   }
   rePass=ltMsgGetVar_s(ltMsgPk,"txtRePass");
    if(!rePass){
         rePass="";
   }
   if(strcmp(password,rePass)!=0){
       ltMsgPk->msgpktype=1;
       lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n",
				"lthtml",LT_TYPE_STRING,"error!");   
      ltMsgFree(ltMsgPk);
      return 0;	
   }

   sprintf(caStr,"insert into msasusertest values (3,'%s','%s')",name,password);
   ltDbExecSql(G_DbCon,sqlBuf); 

	sprintf(caStr,"%s is:%s password:%s",_ltPubInfo->pubMsgStr[0].conStr,name,password);
   // sprintf(caStr,"my name is:%s password:%s",name,password);
	ltMsgPk->msgpktype=1;
    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n",
				"lthtml",LT_TYPE_STRING,caStr);   
    ltMsgFree(ltMsgPk);
    return 0;
}
/*
*显示输入的用户名和密码
*/
int ErrorTest(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char *name;
	char *password;
	char *rePass;
    char myre[1024];
	char caStr[2048];
	char sqlBuf[2048];
    name=ltMsgGetVar_s(ltMsgPk,"txtName");
    if(!name){
         name="";
   }
	password=ltMsgGetVar_s(ltMsgPk,"txtPass");
    if(!password){
         password="";
   }
   rePass=ltMsgGetVar_s(ltMsgPk,"txtRePass");
    if(!rePass){
         rePass="";
   }
   if(strcmp(password,rePass)!=0){
       ltMsgPk->msgpktype=1;
       lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n",
				"lthtml",LT_TYPE_STRING,"error!");   
      ltMsgFree(ltMsgPk);
      return 0;	
   }

   sprintf(caStr,"insert into msasusertest values (3,'%s','%s')",name,password);
   ltDbExecSql(G_DbCon,sqlBuf); 

    //sprintf(caStr,"my name is:%s password:%s",name,password);
    sprintf(caStr,"%s is:%s password:%s",_ltPubInfo->pubMsgStr[0].conStr,name,password);
	ltMsgPk->msgpktype=1;
    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n",
				"lthtml",LT_TYPE_STRING,caStr);   
    ltMsgFree(ltMsgPk);
    return 0;
}