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

int msasTestApp2(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  char *name;
	char *password;
	char *rePass;
  //char myre[1024];
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
       lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\r\n",
				"lthtml",LT_TYPE_STRING,"error!");   
      ltMsgFree(ltMsgPk);
      return 0;	
   }

   sprintf(caStr,"insert into msasusertest values (3,'%s','%s')",name,password);
   ltDbExecSql(G_DbCon,sqlBuf); 

   sprintf(caStr,"%s is %s password:%s",_ltPubInfo->pubMsgStr[0].conStr,name,password);
	 ltMsgPk->msgpktype=1;
    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n",
				"lthtml",LT_TYPE_STRING,caStr);   
    ltMsgFree(ltMsgPk);
    return 0;
}

