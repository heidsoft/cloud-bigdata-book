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
/*
*某端口的配置信息
*/
int icmVLanListAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	   /* 触发规则查询  */
     char strBuf[1024];
     ltDbCursor *tempCursor;
     LT_DBROW tempRow;     
     char caTmpp[512];
     long lRowNum,lStartRec,lSumRec,lCount,lNum;
     int k;
     
     char *mydev;
     mydev=ltMsgGetVar_s(ltMsgPk,"Alertdev");
     
	   char *caId;    //Id
	   char *caVlanid;  //Vlanid
	   char *caIp;  //IP地址
	   char *caMask;  //子网掩码
	   char *caDev;//网口
	   char *caIntercept;//侦听
	
	   char caWhere[512];
	   stralloc strTemp;
		 ltDbHeadPtr dbPtr;
     dbPtr=lt_dbinit();
     lt_db_htmlpage(dbPtr,"utf-8");

    caId=ltMsgGetVar_s(ltMsgPk,"id");  //报警规则Id
    caVlanid=ltMsgGetVar_s(ltMsgPk,"vlanid");//案件Id
    caIp=ltMsgGetVar_s(ltMsgPk,"ip");//报警规则名称
    caMask=ltMsgGetVar_s(ltMsgPk,"mask");//报警规则类型
    caDev=ltMsgGetVar_s(ltMsgPk,"dev");  
    caIntercept=ltMsgGetVar_s(ltMsgPk,"intercept"); 
    
		memset(caWhere,0,sizeof(caWhere));
		sprintf(caWhere,"%s","1=1 ");
			
	  if(caId && caId[0] )
		{
			sprintf(caWhere,"%s and id like '%c%s%c'",caWhere,'%',caId,'%');
		}
		 if(caVlanid && caVlanid[0] )
		{
			sprintf(caWhere,"%s and vlanid like '%c%s%c'",caWhere,'%',caVlanid,'%');
		}
		 if(caIp && caIp[0] )
		{
			sprintf(caWhere,"%s and ip like '%c%s%c'",caWhere,'%',caIp,'%');
		}
		 if(caMask && caMask[0] )
		{
			sprintf(caWhere,"%s and mask like '%c%s%c'",caWhere,'%',caMask,'%');
		}
	  if(caDev && caDev[0] )
		{
			sprintf(caWhere,"%s and dev like '%c%s%c'",caWhere,'%',caDev,'%');
		}
	  if(caIntercept && caIntercept[0] )
		{
			sprintf(caWhere,"%s and intercept like '%c%s%c'",caWhere,'%',caIntercept,'%');
		}
    lRowNum=20; /*每页的行数*/
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
    if(strcmp(mydev,"-1")==0){
	  	sprintf(strBuf,"select count(*) from icmVLanArea where %s '",caWhere);
	  }else{
    	sprintf(strBuf,"select count(*) from icmVLanArea where %s and dev='%s'",caWhere,mydev);
    }
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
    if(strcmp(mydev,"-1")==0){
    	sprintf(strBuf,"select id,vlanid,ip,mask,dev,intercept from icmVLanArea where %s  limit %lu offset %lu ",caWhere,lRowNum,lStartRec);
	  }else{
	  	sprintf(strBuf,"select id,vlanid,ip,mask,dev,intercept from icmVLanArea where %s and dev='%s' limit %lu offset %lu ",caWhere,mydev,lRowNum,lStartRec);
	  }
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {                
          if(k==0)
					{
            sprintf(caTmpp,"{\"id\":\"%s\",\"vlanid\":\"%s\",\"ip\":\"%s\",\"mask\":\"%s\",\"dev\":\"%s\",\"intercept\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5]);
            stralloc_cats(&strTemp,caTmpp);     
					}
					else
					{
				    sprintf(caTmpp,",{\"id\":\"%s\",\"vlanid\":\"%s\",\"ip\":\"%s\",\"mask\":\"%s\",\"dev\":\"%s\",\"intercept\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5]);
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
   return 0;
}

/*
*配置新端口
*/
int icmAddVlanAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	   char *caVlanid;  //Vlanid
	   char *caIp;  //IP地址
	   char *caMask;  //子网掩码
	   char *caDev;//网口
	   char *caIntercept;//侦听
    
	   int  maxId;
	   maxId=1;
	   sprintf(sqlBuf,"select max(id)+1 from icmVLanArea" );
	   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);   
	     if(tempCursor!=NULL){
	          tempRow= ltDbFetchRow(tempCursor);
	          if(tempRow!=NULL){
				      maxId=atol(tempRow[0]);	
	          }
	     ltDbCloseCursor(tempCursor);
	   }
    
    
	  char caStr[2048];
	  char sqlBuf[2048];
    ltDbHeadPtr dbPtr;
    dbPtr=lt_dbinit();
    
	 caVlanid=ltMsgGetVar_s(ltMsgPk,"vlanid");
     if(!caVlanid){
         caVlanid="";
	 }
    caIp=ltMsgGetVar_s(ltMsgPk,"ip");
     if(!caIp){
         caIp="";
	 }
    caMask=ltMsgGetVar_s(ltMsgPk,"mask");
     if(!caMask){
         caMask="";
	 }
    caDev=ltMsgGetVar_s(ltMsgPk,"dev");
     if(!caDev){
         caDev="";
	 }
    caIntercept=ltMsgGetVar_s(ltMsgPk,"intercept");
     if(!caIntercept){
         caIntercept="";
	 }

     sprintf(sqlBuf,"insert into icmVLanArea(id,vlanid,ip,mask,dev,intercept)values (%d,'%s','%s','%s','%s','%s')",maxId,caVlanid,caIp,caMask,caDev,caIntercept);
     printf("sqlBuf:%s\n",sqlBuf);   
	   ltDbExecSql(G_DbCon,sqlBuf); 
     memset(caStr,0,sizeof(caStr));
  	 ltMsgPk->msgpktype=1;
     lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");   
	   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/icm/htmlplt/manager/page.htm",dbPtr->head,0);
     ltMsgFree(ltMsgPk);
    return 0;
}