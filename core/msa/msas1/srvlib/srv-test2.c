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

int showView(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    
    char *caMatching_id;
    char *cagnmkey;
    char *casname;
    char *caRule_id;
    char *caRule_name;
    char *caMatching_time;
    char *caNet_ending_ip;
    char *caNet_ending_code;
    char *caNet_ending_mac;
    char *caDestination_ip;
    char *caService_type;
    char *caKeyWord1;
    char *caKeyWord2;
    char *caKeyWord3;
    char *caCustomer_name;
    char *caCertificate_type;
    char *caCertificate_code;
    char *caRule_type;
    char *cacontenturl;
    char *caupdateflag;



    //char myre[1024];
	char caStr[2048];
	char sqlBuf[2048];
    int  maxuid;

    ltDbCursor *tempCursor;
    LT_DBROW   tempRow;

	 caMatching_id=ltMsgGetVar_s(ltMsgPk,"txt_Matching_id");
    if(!caMatching_id){
         caMatching_id="";
   }
    cagnmkey=ltMsgGetVar_s(ltMsgPk,"txt_gnmkey");
    if(!cagnmkey){
         cagnmkey="";
   }
    casname=ltMsgGetVar_s(ltMsgPk,"txt_name");
    if(!casname){
         casname="";
   }
    caRule_id=ltMsgGetVar_s(ltMsgPk,"txt_Rule_id");
    if(!caRule_id){
         caRule_id="";
   }
    caRule_name=ltMsgGetVar_s(ltMsgPk,"txt_Rule_name");
    if(!caRule_name){
         caRule_name="";
   }
    caMatching_time=ltMsgGetVar_s(ltMsgPk,"txt_Matching_time");
    if(!caMatching_time){
         caMatching_time="";
   }
    caNet_ending_ip=ltMsgGetVar_s(ltMsgPk,"txt_Net_ending_ip");
    if(!caNet_ending_ip){
         caNet_ending_ip="";
   }
    caNet_ending_code=ltMsgGetVar_s(ltMsgPk,"txt_Net_ending_code");
    if(!caNet_ending_code){
         caNet_ending_code="";
   }
    caNet_ending_mac=ltMsgGetVar_s(ltMsgPk,"txt_Net_ending_mac");
    if(!caNet_ending_mac){
         caNet_ending_mac="";
   }
    caDestination_ip=ltMsgGetVar_s(ltMsgPk,"txt_Destination_ip");
    if(!caDestination_ip){
         caDestination_ip="";
   }
    caService_type=ltMsgGetVar_s(ltMsgPk,"txt_Service_type");
    if(!caService_type){
         caService_type="";
   }
    caKeyWord1=ltMsgGetVar_s(ltMsgPk,"txt_KeyWord1");
    if(!caKeyWord1){
         caKeyWord1="";
   }
    caKeyWord2=ltMsgGetVar_s(ltMsgPk,"txt_KeyWord2");
    if(!caKeyWord2){
         caKeyWord2="";
   }
    caKeyWord3=ltMsgGetVar_s(ltMsgPk,"txt_KeyWord3");
    if(!caKeyWord3){
         caKeyWord3="";
   }
    caCustomer_name=ltMsgGetVar_s(ltMsgPk,"txt_Customer_name");
    if(!caCustomer_name){
         caCustomer_name="";
   }
    caCertificate_type=ltMsgGetVar_s(ltMsgPk,"txt_Certificate_type");
    if(!caCertificate_type){
         caCertificate_type="";
   }
    caCertificate_code=ltMsgGetVar_s(ltMsgPk,"txtName");
    if(!caCertificate_code){
         caCertificate_code="";
   }
    caRule_type=ltMsgGetVar_s(ltMsgPk,"txt_Certificate_code");
    if(!caRule_type){
         caRule_type="";
   }
    cacontenturl=ltMsgGetVar_s(ltMsgPk,"txt_contenturl");
    if(!cacontenturl){
         cacontenturl="";
   }
    caupdateflag=ltMsgGetVar_s(ltMsgPk,"txt_updateflag");
    if(!caupdateflag){
         caupdateflag="";
   }
   
   maxuid=1;
   sprintf(sqlBuf,"select max(caMatching_id)+1 from AlertList" );
   tempCursor=ltDbOpenCursorS(G_DbCon,sqlBuf);   
   if(tempCursor!=NULL){
          tempRow= ltDbFetchRow(tempCursor);
          if(tempRow!=NULL){
			  maxuid=atol(tempRow[0]);	
          }
     ltDbCloseCursor(tempCursor);
   }
   
   memset(caStr,0,sizeof(caStr));
   sprintf(sqlBuf,"select * from AlertList" );
   tempCursor=ltDbOpenCursorS(G_DbCon,sqlBuf);   
   if(tempCursor!=NULL){ 
          tempRow= ltDbFetchRow(tempCursor);
          while(tempRow!=NULL){
			 sprintf(caStr,"%s%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s\r\n<br>",caStr,tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14],tempRow[15],tempRow[16],tempRow[17],tempRow[18],tempRow[19]);
			 tempRow= ltDbFetchRow(tempCursor);
          }
        ltDbCloseCursor(tempCursor);
   }

	ltMsgPk->msgpktype=1;
    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=gb2312\r\n",
				"lthtml",LT_TYPE_STRING,caStr);   
    ltMsgFree(ltMsgPk);
    return 0;
}
