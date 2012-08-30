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

/*
*设备部署
*初始化页面
*/
int msaEquipment(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  		int   nownum;
      int   match;
  		int   jjjj;
      char  mynownum[16];
      
     	ltDbHeadPtr dbPtr;
     	dbPtr=lt_dbinit();
	    lt_db_htmlpage(dbPtr,"utf-8");
	    
  		static char inbuf[4096];
  		int   fd;
  		fd=open("/proc/net/dev",O_RDONLY | O_NDELAY); 
		  static stralloc line = {0};
		  buffer ss;
			nownum=0;
		  buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
		  for (;;){
				if (getln(&ss,&line,&match,'\n') == -1){ break;}
				if (!match && !line.len) { break; }
				for(jjjj=0;jjjj<line.len;jjjj++){
					 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h'){
						 nownum++;		
				   }
			  }
				for(jjjj=0;jjjj<line.len;jjjj++){
					 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='0'){
					 		lt_dbput_rootvars(dbPtr,1,"eth0","1");
				   }
					 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='1'){
					 		lt_dbput_rootvars(dbPtr,1,"eth1","1");
				   }
					 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='2'){
					 		lt_dbput_rootvars(dbPtr,1,"eth2","1");
				   }
					 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='3'){
					 		lt_dbput_rootvars(dbPtr,1,"eth3","1");
				   }
					 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='4'){
					 		lt_dbput_rootvars(dbPtr,1,"eth4","1");
				   }
					 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='5'){
					 		lt_dbput_rootvars(dbPtr,1,"eth5","1");
				   }
					 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='6'){
					 		lt_dbput_rootvars(dbPtr,1,"eth6","1");
				   }
					 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='7'){
					 		lt_dbput_rootvars(dbPtr,1,"eth7","1");
				   }
					 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='8'){
					 		lt_dbput_rootvars(dbPtr,1,"eth8","1");
				   }
					 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='9'){
					 		lt_dbput_rootvars(dbPtr,1,"eth9","1");
				   }
			  }
		  }
	    sprintf(mynownum,"%d",nownum);
	    
			char strBuf[1024];
			
		  char *dbUser;
			char *dbPass;
			char *dbName;
			dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbCon==NULL){
						ltMsgPk->msgpktype=1;
						lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"db error!");
						ltMsgFree(ltMsgPk);			
						return 0;
			}
			sprintf(strBuf,"delete from msaVLanArea where vlanid>=%d",nownum);
			ltDbExecSql(G_DbCon,strBuf);
			ltDbClose(G_DbCon);
			lt_dbput_rootvars(dbPtr,1,"nownum",mynownum);
			
	    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/naslanlist.htm",dbPtr->head,0);
			ltMsgFree(ltMsgPk);
			lt_dbfree(dbPtr);
			return 0;
}

/*
*查询网口配置信息
*/
int msaVLanListAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char strBuf[1024];
   ltDbCursor *tempCursor;
   LT_DBROW tempRow;     
   char caTmpp[512];
   long lRowNum,lStartRec,lSumRec,lCount,lNum;
   int k;
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
   char *mywk;
   mywk=ltMsgGetVar_s(ltMsgPk,"mywk");
   stralloc strTemp;
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
   if(strcmp(mywk,"-1")==0){
   	 sprintf(strBuf,"select count(*) from msaVLanArea");
   }else{
   	 sprintf(strBuf,"select count(*) from msaVLanArea where vlanid='%s'",mywk);
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
   sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
   stralloc_cats(&strTemp,caTmpp);
   k=0;
   if(strcmp(mywk,"-1")==0){
  	 sprintf(strBuf,"select vlanid,intercept,promisc,switch,netvlan,ipaddress,passaway,apply,mask from msaVLanArea  order by vlanid limit %lu offset %lu",lRowNum,lStartRec);
   }else{
  	 sprintf(strBuf,"select vlanid,intercept,promisc,switch,netvlan,ipaddress,passaway,apply,mask from msaVLanArea where vlanid='%s' limit %lu offset %lu ",mywk,lRowNum,lStartRec);
   }
   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
   if(tempCursor!=NULL){
     tempRow=ltDbFetchRow(tempCursor);
     while(tempRow!=NULL){                
        if(k==0){
          sprintf(caTmpp,"{\"vlanid\":\"%s\",\"intercept\":\"%s\",\"promisc\":\"%s\",\"switch\":\"%s\",\"netvlan\":\"%s\",\"ipaddress\":\"%s\",\"passaway\":\"%s\",\"mask\":\"%s\",\"apply\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[8],tempRow[7]);
          stralloc_cats(&strTemp,caTmpp);     
				}else{
			    sprintf(caTmpp,",{\"vlanid\":\"%s\",\"intercept\":\"%s\",\"promisc\":\"%s\",\"switch\":\"%s\",\"netvlan\":\"%s\",\"ipaddress\":\"%s\",\"passaway\":\"%s\",\"mask\":\"%s\",\"apply\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[8],tempRow[7]);
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

/*
*配置网口信息
*/
int msaAddVlanAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){//msaAddVlanAction
	   char *caIntercept;
	   char *caPromisc;
	   char *caSwitch;
	   char *caNetvlan;
	   char *caIpaddress;
	   char *camask;
	   char *caPassaway;
     char sqlBuf[2048];
	   long lCount;
     lCount=0;
	   char *mywk;
     mywk=ltMsgGetVar_s(ltMsgPk,"Mywk");
		 caIntercept=ltMsgGetVar_s(ltMsgPk,"intercept");
	     if(!caIntercept){
	         caIntercept="";
		 }
	    caPromisc=ltMsgGetVar_s(ltMsgPk,"promisc");
	     if(!caPromisc){
	         caPromisc="";
		 }
	    caSwitch=ltMsgGetVar_s(ltMsgPk,"switch0");
	     if(!caSwitch){
	         caSwitch="";
		 }
	    caNetvlan=ltMsgGetVar_s(ltMsgPk,"netvlan");
	     if(!caNetvlan){
	         caNetvlan="";
		 } 
	    caIpaddress=ltMsgGetVar_s(ltMsgPk,"ipaddress");
	     if(!caIpaddress){
	         caIpaddress="";
		 }
	    camask=ltMsgGetVar_s(ltMsgPk,"mask");
	     if(!camask){
	         camask="";
		 } 
	    caPassaway=ltMsgGetVar_s(ltMsgPk,"passaway");
	     if(!caPassaway){
	         caPassaway="";
		 } 
	 
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
		 
	   ltDbCursor *tempCursor;
	   LT_DBROW tempRow; 
	   sprintf(sqlBuf,"select count(*) from msaVLanArea where vlanid=%s",mywk);	
	   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf); 
	   if(tempCursor!=NULL){
			  tempRow= ltDbFetchRow(tempCursor);
			  if(tempRow!=NULL){
				  lCount=atol(tempRow[0]);
			  }
		 }     
	   if(lCount==0){
		     sprintf(sqlBuf,"insert into msaVLanArea(vlanid,intercept,promisc,switch,netvlan,ipaddress,passaway,mask,apply)values (%s,%s,%s,%s,%s,'%s',%s,'%s',0)",mywk,caIntercept,caPromisc,caSwitch,caNetvlan,caIpaddress,caPassaway,camask);	  
		     ltDbExecSql(G_DbCon,sqlBuf);
	   }else{
		     sprintf(sqlBuf,"update msaVLanArea set intercept=%s,promisc=%s,switch=%s,netvlan=%s,ipaddress='%s',passaway=%s,mask='%s',apply=0 where vlanid=%s",caIntercept,caPromisc,caSwitch,caNetvlan,caIpaddress,caPassaway,camask,mywk); 
		     ltDbExecSql(G_DbCon,sqlBuf); 
		 }
     ltDbCloseCursor(tempCursor);

  	 ltMsgPk->msgpktype=1;
     lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");   
	   ltMsgFree(ltMsgPk);
     ltDbCloseCursor(tempCursor);
     ltDbClose(G_DbCon);
    return 0;
}
//网口应用
int msaApplyAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){

			 	static char inbuf[4096];
	  		int   fd;
	  		
	  		ltMsgPk->msgpktype=1;
		    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"1");
		    
	  		
	  		fd=open("/proc/net/dev",O_RDONLY | O_NDELAY); 
			  static stralloc line = {0};
			  buffer ss;
				int    jjjj;
				int   match;
				
			  buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
			  for (;;){
					if (getln(&ss,&line,&match,'\n') == -1){ break;}
					if (!match && !line.len) { break; }
					for(jjjj=0;jjjj<line.len;jjjj++){
						 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='0'){
						 		system("ifconfig eth0 0.0.0.0 down" );
					   }
						 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='1'){
						 		system("ifconfig eth1 0.0.0.0 down" );
					   }
						 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='2'){
						 		system("ifconfig eth2 0.0.0.0 down" );
					   }
						 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='3'){
						 		system("ifconfig eth3 0.0.0.0 down" );
					   }
						 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='4'){
						 		system("ifconfig eth4 0.0.0.0 down" );
					   }
						 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='5'){
						 		system("ifconfig eth5 0.0.0.0 down" );
					   }
						 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='6'){
						 		system("ifconfig eth6 0.0.0.0 down" );
					   }
						 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='7'){
						 		system("ifconfig eth7 0.0.0.0 down" );
					   }
						 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='8'){
						 		system("ifconfig eth8 0.0.0.0 down" );
					   }
						 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h' && line.s[jjjj+3]=='9'){
						 		system("ifconfig eth9 0.0.0.0 down" );
					   }
				  }
				}
				//通道恢复为默认的状态
			  system("echo 'r' > /proc/baochuang/linkinfo");
	      
	      system("/etc/msa/msa/start/linkdownnow");
	      
	      system("/bin/cp /etc/msa/msa/start/linkdown /etc/msa/msa/start/linkdownnow");
        system("/bin/chmod 755  /etc/msa/msa/start/linkdownnow");
		    
		    system("/etc/msa/msa/start/linkup");
		    
		    system("killall -2  msawdog");
		    sleep(8);
		    
		    ltMsgFree(ltMsgPk);
		    
		 return 0;

}
//加载网口配置页面
int ltappPage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char str[16];
	//是否侦听  是否启用promisc混杂模式
  int myintercept,mypromisc;
  //数据到达后的传输方式   转发镜像网口 虚拟通道 转换IP
	char myswitch[32],mynetvLan[32],mypassAway[32],mymask[32],myipAddress[32];	
	
	int mywk;
  char sqlBuf[1024];
  ltDbHeadPtr dbPtr;
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
		
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	
	
  int   nownum;
  int   match;
  int   jjjj;
	
  static char inbuf[4096];
  int   fd;
  fd=open("/proc/net/dev",O_RDONLY | O_NDELAY); 
	static stralloc line = {0};
	buffer ss;
	nownum=0;
	buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
	for (;;){
		if (getln(&ss,&line,&match,'\n') == -1){ break;}
		if (!match && !line.len) { break; }
		for(jjjj=0;jjjj<line.len;jjjj++){
			 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h'){
				 nownum++;		
		   }
	  }
	}
		
	if(ltMsgGetVar_s(ltMsgPk,"mywk")!=NULL)
	{
		mywk=atol(ltMsgGetVar_s(ltMsgPk,"mywk"));
		sprintf(sqlBuf,"select vlanid,intercept,promisc,switch,netvlan,ipaddress,passaway,apply,mask from msaVLanArea where vlanid=%d",mywk);
	}
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
      tempRow= ltDbFetchRow(tempCursor);
      if(tempRow!=NULL){
         myintercept=atoi(tempRow[1]);
         mypromisc=atoi(tempRow[2]);
         sprintf(myswitch,"%s",tempRow[3]);
         sprintf(mynetvLan,"%s",tempRow[4]);
         sprintf(myipAddress,"%s",tempRow[5]);
         sprintf(mypassAway,"%s",tempRow[6]);
         sprintf(mymask,"%s",tempRow[8]);
         
         memset(str,0,sizeof(str));//1
	       sprintf(str,"interceptchk%d",myintercept);
	       lt_dbput_rootvars(dbPtr,1,str,"checked");
	       
	       memset(str,0,sizeof(str));//2
	       sprintf(str,"promiscchk%d",mypromisc);
	       lt_dbput_rootvars(dbPtr,1,str,"checked");
	       	
	       memset(str,0,sizeof(str));
	       sprintf(str,"switch%ssel",myswitch);
	       lt_dbput_rootvars(dbPtr,1,str,"selected");
	       	
	       memset(str,0,sizeof(str));
	       sprintf(str,"passAway%ssel",mypassAway);
	       lt_dbput_rootvars(dbPtr,1,str,"selected");
       
	       //memset(myipAddress,0,sizeof(myipAddress));
				 lt_dbput_rootvars(dbPtr,1,"ipAddress",myipAddress);
				 
				 lt_dbput_rootvars(dbPtr,1,"mask",mymask);
					
         tempRow= ltDbFetchRow(tempCursor);
      }
      ltDbCloseCursor(tempCursor);
  }
  
  ltTablePtr  tablePtr;
  tablePtr=lt_dbput_table(dbPtr,"vlanlist");	
  int iii;
	for(iii=0;iii<nownum;iii++){
		//printf("nownum:%d\n",nownum);
		
	  if(atol(mynetvLan)==iii){
			lt_dbput_recordvars(tablePtr,2,"vlanid",LT_TYPE_LONG,iii,"selvid",LT_TYPE_STRING," selected ");
		}else{
			lt_dbput_recordvars(tablePtr,1,"vlanid",LT_TYPE_LONG,iii);
		}
		   
	}

	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/page.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	ltDbClose(G_DbCon);
	return 0;
}

/*
*虚拟服务器页面初始化加载
*/
int vserver(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
			int   nownum;
      int   match;
  		int   jjjj;
      char  mynownum[16];
      char enable1[24],enable2[24],enable3[24],enable4[24],enable5[24],enable6[24],enable7[24],enable8[24];
      char Lan1[24],Lan2[24],Lan3[24],Lan4[24],Lan5[24],Lan6[24],Lan7[24],Lan8[24];
      char pro1[24],pro2[24],pro3[24],pro4[24],pro5[24],pro6[24],pro7[24],pro8[24];
      char ip1[24],ip2[24],ip3[24],ip4[24],ip5[24],ip6[24],ip7[24],ip8[24];
      char ip11[24],ip22[24],ip33[24],ip44[24],ip55[24],ip66[24],ip77[24],ip88[24];
      char lan11[24],lan22[24],lan33[24],lan44[24],lan55[24],lan66[24],lan77[24],lan88[24];
      
      
      char str[16];
      char *pFile="/etc/msa/msa/VPS/settings";
	   	ltDbHeadPtr dbPtr;
     	dbPtr=lt_dbinit();
	    lt_db_htmlpage(dbPtr,"utf-8");
	    //启用
	     memset(enable1,0,sizeof(enable1));
		   bcCnfGetValue_s(pFile,"enable1",enable1);
		   if(strcmp(enable1,"1")==0){
		   	lt_dbput_rootvars(dbPtr,1,"enablechk1","checked"); 
		   }else{
		   	lt_dbput_rootvars(dbPtr,1,"enablechk1","");
		   }
	     memset(enable2,0,sizeof(enable2));
		   bcCnfGetValue_s(pFile,"enable2",enable2);
		   if(strcmp(enable2,"1")==0){
		   	lt_dbput_rootvars(dbPtr,1,"enablechk2","checked"); 
		   }else{
		   	lt_dbput_rootvars(dbPtr,1,"enablechk2","");
		   }
	     memset(enable3,0,sizeof(enable3));
		   bcCnfGetValue_s(pFile,"enable3",enable3);
		   if(strcmp(enable3,"1")==0){
		   	lt_dbput_rootvars(dbPtr,1,"enablechk3","checked"); 
		   }else{
		   	lt_dbput_rootvars(dbPtr,1,"enablechk3","");
		   }
	     memset(enable4,0,sizeof(enable4));
		   bcCnfGetValue_s(pFile,"enable4",enable4);
		   if(strcmp(enable4,"1")==0){
		   	lt_dbput_rootvars(dbPtr,1,"enablechk4","checked"); 
		   }else{
		   	lt_dbput_rootvars(dbPtr,1,"enablechk4","");
		   }
	     memset(enable5,0,sizeof(enable5));
		   bcCnfGetValue_s(pFile,"enable5",enable5);
		   if(strcmp(enable5,"1")==0){
		   	lt_dbput_rootvars(dbPtr,1,"enablechk5","checked"); 
		   }else{
		   	lt_dbput_rootvars(dbPtr,1,"enablechk5","");
		   }
	     memset(enable6,0,sizeof(enable6));
		   bcCnfGetValue_s(pFile,"enable6",enable6);
		   if(strcmp(enable6,"1")==0){
		   	lt_dbput_rootvars(dbPtr,1,"enablechk6","checked"); 
		   }else{
		   	lt_dbput_rootvars(dbPtr,1,"enablechk6","");
		   }
	     memset(enable7,0,sizeof(enable7));
		   bcCnfGetValue_s(pFile,"enable7",enable7);
		   if(strcmp(enable7,"1")==0){
		   	lt_dbput_rootvars(dbPtr,1,"enablechk7","checked"); 
		   }else{
		   	lt_dbput_rootvars(dbPtr,1,"enablechk7","");
		   }
	     memset(enable8,0,sizeof(enable8));
		   bcCnfGetValue_s(pFile,"enable8",enable8);
		   if(strcmp(enable8,"1")==0){
		   	lt_dbput_rootvars(dbPtr,1,"enablechk8","checked"); 
		   }else{
		   	lt_dbput_rootvars(dbPtr,1,"enablechk8","");
		   }
		   //接入网口
		   memset(Lan1,0,sizeof(Lan1));
		   bcCnfGetValue_s(pFile,"Lan1",Lan1);
			 memset(str,0,sizeof(str));
			 sprintf(str,"LAN1%s",Lan1);
			 lt_dbput_rootvars(dbPtr,1,str,"selected");
			 
		   memset(Lan2,0,sizeof(Lan2));
		   bcCnfGetValue_s(pFile,"Lan2",Lan2);
			 memset(str,0,sizeof(str));
			 sprintf(str,"LAN2%s",Lan2);
			 lt_dbput_rootvars(dbPtr,1,str,"selected");
			 
		   memset(Lan3,0,sizeof(Lan3));
		   bcCnfGetValue_s(pFile,"Lan3",Lan3);
			 memset(str,0,sizeof(str));
			 sprintf(str,"LAN3%s",Lan3);
			 lt_dbput_rootvars(dbPtr,1,str,"selected");
			 
		   memset(Lan4,0,sizeof(Lan4));
		   bcCnfGetValue_s(pFile,"Lan4",Lan4);
			 memset(str,0,sizeof(str));
			 sprintf(str,"LAN4%s",Lan4);
			 lt_dbput_rootvars(dbPtr,1,str,"selected");
			 
		   memset(Lan5,0,sizeof(Lan5));
		   bcCnfGetValue_s(pFile,"Lan5",Lan5);
			 memset(str,0,sizeof(str));
			 sprintf(str,"LAN5%s",Lan5);
			 lt_dbput_rootvars(dbPtr,1,str,"selected");
			 
		   memset(Lan6,0,sizeof(Lan6));
		   bcCnfGetValue_s(pFile,"Lan6",Lan6);
			 memset(str,0,sizeof(str));
			 sprintf(str,"LAN6%s",Lan6);
			 lt_dbput_rootvars(dbPtr,1,str,"selected");
			 
		   memset(Lan7,0,sizeof(Lan7));
		   bcCnfGetValue_s(pFile,"Lan7",Lan7);
			 memset(str,0,sizeof(str));
			 sprintf(str,"LAN7%s",Lan7);
			 lt_dbput_rootvars(dbPtr,1,str,"selected");
			 
		   memset(Lan8,0,sizeof(Lan8));
		   bcCnfGetValue_s(pFile,"Lan8",Lan8);
			 memset(str,0,sizeof(str));
			 sprintf(str,"LAN8%s",Lan8);
			 lt_dbput_rootvars(dbPtr,1,str,"selected");
			 //协议
		   memset(pro1,0,sizeof(pro1));
		   bcCnfGetValue_s(pFile,"pro1",pro1);
			 memset(str,0,sizeof(str));
			 sprintf(str,"pro1%s",pro1);
			 lt_dbput_rootvars(dbPtr,1,str,"selected");
			 
		   memset(pro2,0,sizeof(pro2));
		   bcCnfGetValue_s(pFile,"pro2",pro2);
			 memset(str,0,sizeof(str));
			 sprintf(str,"pro2%s",pro2);
			 lt_dbput_rootvars(dbPtr,1,str,"selected");
			 
		   memset(pro3,0,sizeof(pro3));
		   bcCnfGetValue_s(pFile,"pro3",pro3);
			 memset(str,0,sizeof(str));
			 sprintf(str,"pro3%s",pro3);
			 lt_dbput_rootvars(dbPtr,1,str,"selected");
			 
		   memset(pro4,0,sizeof(pro4));
		   bcCnfGetValue_s(pFile,"pro4",pro4);
			 memset(str,0,sizeof(str));
			 sprintf(str,"pro4%s",pro4);
			 lt_dbput_rootvars(dbPtr,1,str,"selected");
			 
		   memset(pro5,0,sizeof(pro5));
		   bcCnfGetValue_s(pFile,"pro5",pro5);
			 memset(str,0,sizeof(str));
			 sprintf(str,"pro5%s",pro5);
			 lt_dbput_rootvars(dbPtr,1,str,"selected");
			 
		   memset(pro6,0,sizeof(pro6));
		   bcCnfGetValue_s(pFile,"pro6",pro6);
			 memset(str,0,sizeof(str));
			 sprintf(str,"pro6%s",pro6);
			 lt_dbput_rootvars(dbPtr,1,str,"selected");
			 
		   memset(pro7,0,sizeof(pro7));
		   bcCnfGetValue_s(pFile,"pro7",pro7);
			 memset(str,0,sizeof(str));
			 sprintf(str,"pro7%s",pro7);
			 lt_dbput_rootvars(dbPtr,1,str,"selected");
			 
		   memset(pro8,0,sizeof(pro8));
		   bcCnfGetValue_s(pFile,"pro8",pro8);
			 memset(str,0,sizeof(str));
			 sprintf(str,"pro8%s",pro8);
			 lt_dbput_rootvars(dbPtr,1,str,"selected");
			 //IP地址
		   memset(ip1,0,sizeof(ip1));
		   bcCnfGetValue_s(pFile,"ip1",ip1);
		   lt_dbput_rootvars(dbPtr,1,"ip1",ip1);
			 
		   memset(ip2,0,sizeof(ip2));
		   bcCnfGetValue_s(pFile,"ip2",ip2);
		   lt_dbput_rootvars(dbPtr,1,"ip2",ip2);
			 
		   memset(ip3,0,sizeof(ip3));
		   bcCnfGetValue_s(pFile,"ip3",ip3);
		   lt_dbput_rootvars(dbPtr,1,"ip3",ip3);
			 
		   memset(ip4,0,sizeof(ip4));
		   bcCnfGetValue_s(pFile,"ip4",ip4);
		   lt_dbput_rootvars(dbPtr,1,"ip4",ip4);
			 
		   memset(ip5,0,sizeof(ip5));
		   bcCnfGetValue_s(pFile,"ip5",ip5);
		   lt_dbput_rootvars(dbPtr,1,"ip5",ip5);
			 
		   memset(ip6,0,sizeof(ip6));
		   bcCnfGetValue_s(pFile,"ip6",ip6);
		   lt_dbput_rootvars(dbPtr,1,"ip6",ip6);
			 
		   memset(ip7,0,sizeof(ip7));
		   bcCnfGetValue_s(pFile,"ip7",ip7);
		   lt_dbput_rootvars(dbPtr,1,"ip7",ip7);
			 
		   memset(ip8,0,sizeof(ip8));
		   bcCnfGetValue_s(pFile,"ip8",ip8);
		   lt_dbput_rootvars(dbPtr,1,"ip8",ip8);
		   //映射IP地址
		   memset(ip11,0,sizeof(ip11));
		   bcCnfGetValue_s(pFile,"ip11",ip11);
		   lt_dbput_rootvars(dbPtr,1,"ip11",ip11);
		   
		   memset(ip22,0,sizeof(ip22));
		   bcCnfGetValue_s(pFile,"ip22",ip22);
		   lt_dbput_rootvars(dbPtr,1,"ip22",ip22);
		   
		   memset(ip33,0,sizeof(ip33));
		   bcCnfGetValue_s(pFile,"ip33",ip33);
		   lt_dbput_rootvars(dbPtr,1,"ip33",ip33);
		   
		   memset(ip44,0,sizeof(ip44));
		   bcCnfGetValue_s(pFile,"ip44",ip44);
		   lt_dbput_rootvars(dbPtr,1,"ip44",ip44);
		   
		   memset(ip55,0,sizeof(ip55));
		   bcCnfGetValue_s(pFile,"ip55",ip55);
		   lt_dbput_rootvars(dbPtr,1,"ip55",ip55);
		   
		   memset(ip6,0,sizeof(ip66));
		   bcCnfGetValue_s(pFile,"ip66",ip66);
		   lt_dbput_rootvars(dbPtr,1,"ip66",ip66);
		   
		   memset(ip77,0,sizeof(ip77));
		   bcCnfGetValue_s(pFile,"ip77",ip77);
		   lt_dbput_rootvars(dbPtr,1,"ip77",ip77);
		   
		   memset(ip88,0,sizeof(ip88));
		   bcCnfGetValue_s(pFile,"ip88",ip88);
		   lt_dbput_rootvars(dbPtr,1,"ip88",ip88);
			 //服务端口
		   memset(lan11,0,sizeof(lan11));
		   bcCnfGetValue_s(pFile,"lan11",lan11);
		   lt_dbput_rootvars(dbPtr,1,"lan11",lan11);
		   
		   memset(lan22,0,sizeof(lan22));
		   bcCnfGetValue_s(pFile,"lan22",lan22);
		   lt_dbput_rootvars(dbPtr,1,"lan22",lan22);
		   
		   memset(lan33,0,sizeof(lan33));
		   bcCnfGetValue_s(pFile,"lan33",lan33);
		   lt_dbput_rootvars(dbPtr,1,"lan33",lan33);
		   
		   memset(lan44,0,sizeof(lan44));
		   bcCnfGetValue_s(pFile,"lan44",lan44);
		   lt_dbput_rootvars(dbPtr,1,"lan44",lan44);
		   
		   memset(lan55,0,sizeof(lan55));
		   bcCnfGetValue_s(pFile,"lan55",lan55);
		   lt_dbput_rootvars(dbPtr,1,"lan55",lan55);
		   
		   memset(lan66,0,sizeof(lan66));
		   bcCnfGetValue_s(pFile,"lan66",lan66);
		   lt_dbput_rootvars(dbPtr,1,"lan66",lan66);
		   
		   memset(lan77,0,sizeof(lan77));
		   bcCnfGetValue_s(pFile,"lan77",lan77);
		   lt_dbput_rootvars(dbPtr,1,"lan77",lan77);
		   
		   memset(lan88,0,sizeof(lan88));
		   bcCnfGetValue_s(pFile,"lan88",lan88);
		   lt_dbput_rootvars(dbPtr,1,"lan88",lan88);
			 
	    
	    static char inbuf[4096];
  		int   fd;
  		fd=open("/proc/net/dev",O_RDONLY | O_NDELAY); 
		  static stralloc line = {0};
		  buffer ss;
			nownum=0;
		  buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
		  for (;;){
		  	if (getln(&ss,&line,&match,'\n') == -1){ break;}
				if (!match && !line.len) { break; }
				for(jjjj=0;jjjj<line.len;jjjj++){
					 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h'){
						 nownum++;
				   }
			  }
			}
	    sprintf(mynownum,"%d",nownum);	
	    lt_dbput_rootvars(dbPtr,1,"nownum",mynownum);	
	    
	    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/nasVPSList.htm",dbPtr->head,0);
			ltMsgFree(ltMsgPk);
			lt_dbfree(dbPtr);
			return 0;
}
/*
*配置虚拟服务器
*/
int ltVPSok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
    char *enable1,*enable2,*enable3,*enable4,*enable5,*enable6,*enable7,*enable8;
    char *Lan1,*Lan2,*Lan3,*Lan4,*Lan5,*Lan6,*Lan7,*Lan8;
    char *pro1,*pro2,*pro3,*pro4,*pro5,*pro6,*pro7,*pro8;
    char *ip1,*ip2,*ip3,*ip4,*ip5,*ip6,*ip7,*ip8;
    char *ip11,*ip22,*ip33,*ip44,*ip55,*ip66,*ip77,*ip88;
    char *lan11,*lan22,*lan33,*lan44,*lan55,*lan66,*lan77,*lan88;
    char caCmdCon[128];
    
    //接入网口
     Lan1=ltMsgGetVar_s(ltMsgPk,"Lan1");
     Lan2=ltMsgGetVar_s(ltMsgPk,"Lan2");
     Lan3=ltMsgGetVar_s(ltMsgPk,"Lan3");
     Lan4=ltMsgGetVar_s(ltMsgPk,"Lan4");
     Lan5=ltMsgGetVar_s(ltMsgPk,"Lan5");
     Lan6=ltMsgGetVar_s(ltMsgPk,"Lan6");
     Lan7=ltMsgGetVar_s(ltMsgPk,"Lan7");
     Lan8=ltMsgGetVar_s(ltMsgPk,"Lan8");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'Lan1=%s' > %s",Lan1,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'Lan2=%s' >> %s",Lan2,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'Lan3=%s' >> %s",Lan3,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'Lan4=%s' >> %s",Lan4,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'Lan5=%s' >> %s",Lan5,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'Lan6=%s' >> %s",Lan6,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'Lan7=%s' >> %s",Lan7,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'Lan8=%s' >> %s",Lan8,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
    //映射IP地址
     ip11=ltMsgGetVar_s(ltMsgPk,"txtIp11");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'ip11=%s' >> %s",ip11,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     ip22=ltMsgGetVar_s(ltMsgPk,"txtIp22");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'ip22=%s' >> %s",ip22,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     ip33=ltMsgGetVar_s(ltMsgPk,"txtIp33");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'ip33=%s' >> %s",ip33,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     ip44=ltMsgGetVar_s(ltMsgPk,"txtIp44");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'ip44=%s' >> %s",ip44,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     ip55=ltMsgGetVar_s(ltMsgPk,"txtIp55");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'ip55=%s' >> %s",ip55,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     ip66=ltMsgGetVar_s(ltMsgPk,"txtIp66");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'ip66=%s' >> %s",ip66,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     ip77=ltMsgGetVar_s(ltMsgPk,"txtIp77");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'ip77=%s' >> %s",ip77,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     ip88=ltMsgGetVar_s(ltMsgPk,"txtIp88");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'ip88=%s' >> %s",ip88,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
	   //服务端口
     lan11=ltMsgGetVar_s(ltMsgPk,"lan11");
     lan22=ltMsgGetVar_s(ltMsgPk,"lan22");
     lan33=ltMsgGetVar_s(ltMsgPk,"lan33");
     lan44=ltMsgGetVar_s(ltMsgPk,"lan44");
     lan55=ltMsgGetVar_s(ltMsgPk,"lan55");
     lan66=ltMsgGetVar_s(ltMsgPk,"lan66");
     lan77=ltMsgGetVar_s(ltMsgPk,"lan77");
     lan88=ltMsgGetVar_s(ltMsgPk,"lan88");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'lan11=%s' >> %s",lan11,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'lan22=%s' >> %s",lan22,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'lan33=%s' >> %s",lan33,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'lan44=%s' >> %s",lan44,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'lan55=%s' >> %s",lan55,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'lan66=%s' >> %s",lan66,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'lan77=%s' >> %s",lan77,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'lan88=%s' >> %s",lan88,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
	   //IP地址
     ip1=ltMsgGetVar_s(ltMsgPk,"txtIp1");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'ip1=%s' >> %s",ip1,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     ip2=ltMsgGetVar_s(ltMsgPk,"txtIp2");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'ip2=%s' >> %s",ip2,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     ip3=ltMsgGetVar_s(ltMsgPk,"txtIp3");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'ip3=%s' >> %s",ip3,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     ip4=ltMsgGetVar_s(ltMsgPk,"txtIp4");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'ip4=%s' >> %s",ip4,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     ip5=ltMsgGetVar_s(ltMsgPk,"txtIp5");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'ip5=%s' >> %s",ip5,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     ip6=ltMsgGetVar_s(ltMsgPk,"txtIp6");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'ip6=%s' >> %s",ip6,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     ip7=ltMsgGetVar_s(ltMsgPk,"txtIp7");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'ip7=%s' >> %s",ip7,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     ip8=ltMsgGetVar_s(ltMsgPk,"txtIp8");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'ip8=%s' >> %s",ip8,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
	   //协议
     pro1=ltMsgGetVar_s(ltMsgPk,"pro1");
     pro2=ltMsgGetVar_s(ltMsgPk,"pro2");
     pro3=ltMsgGetVar_s(ltMsgPk,"pro3");
     pro4=ltMsgGetVar_s(ltMsgPk,"pro4");
     pro5=ltMsgGetVar_s(ltMsgPk,"pro5");
     pro6=ltMsgGetVar_s(ltMsgPk,"pro6");
     pro7=ltMsgGetVar_s(ltMsgPk,"pro7");
     pro8=ltMsgGetVar_s(ltMsgPk,"pro8");
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'pro1=%s' >> %s",pro1,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'pro2=%s' >> %s",pro2,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'pro3=%s' >> %s",pro3,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'pro4=%s' >> %s",pro4,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'pro5=%s' >> %s",pro5,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'pro6=%s' >> %s",pro6,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'pro7=%s' >> %s",pro7,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'pro8=%s' >> %s",pro8,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     //启用
     enable1=ltMsgGetVar_s(ltMsgPk,"enable1");
     enable2=ltMsgGetVar_s(ltMsgPk,"enable2");
     enable3=ltMsgGetVar_s(ltMsgPk,"enable3");
     enable4=ltMsgGetVar_s(ltMsgPk,"enable4");
     enable5=ltMsgGetVar_s(ltMsgPk,"enable5");
     enable6=ltMsgGetVar_s(ltMsgPk,"enable6");
     enable7=ltMsgGetVar_s(ltMsgPk,"enable7");
     enable8=ltMsgGetVar_s(ltMsgPk,"enable8");
     
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'enable1=%s' >> %s",enable1,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'enable2=%s' >> %s",enable2,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'enable3=%s' >> %s",enable3,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'enable4=%s' >> %s",enable4,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'enable5=%s' >> %s",enable5,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'enable6=%s' >> %s",enable6,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'enable7=%s' >> %s",enable7,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
     memset(caCmdCon,0,sizeof(caCmdCon));
		 sprintf(caCmdCon,"echo 'enable8=%s' >> %s",enable8,"/etc/msa/msa/VPS/settings");
		 system(caCmdCon);
		 
		 sprintf(caCmdCon,"echo '' > %s","/etc/msa/msa/start/vps.up");
		 system(caCmdCon);
		 //写配置脚本
		 if(strcmp(enable1,"1")==0){
			  //#/sbin/iptables -t nat -A PREROUTING -p tcp -d $addr0 --dport 20 -i eth0 -j DNAT --to $addr3:20  
				if(strcmp(pro2,"-1")==0){//all
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p udp  -d %s --dport %s -i eth%s -j DNAT --to %s' > %s",ip11,lan11,Lan1,ip1,"/etc/msa/msa/start/vps.up");
			   system(caCmdCon);
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p tcp  -d %s --dport %s -i eth%s -j DNAT --to %s' > %s",ip11,lan11,Lan1,ip1,"/etc/msa/msa/start/vps.up");
			  }else if(strcmp(pro2,"0")==0){//UDP
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p udp -d %s --dport %s -i eth%s -j DNAT --to %s' > %s",ip11,lan11,Lan1,ip1,"/etc/msa/msa/start/vps.up");
			  }else if(strcmp(pro2,"1")==0){//TCP
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p tcp -d %s --dport %s -i eth%s -j DNAT --to %s' > %s",ip11,lan11,Lan1,ip1,"/etc/msa/msa/start/vps.up");
			  }
			  system(caCmdCon);
		 }
		 if(strcmp(enable2,"1")==0){
			  //#/sbin/iptables -t nat -A PREROUTING -p tcp -d $addr0 --dport 20 -i eth0 -j DNAT --to $addr3:20  
				if(strcmp(pro2,"-1")==0){//all
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p udp  -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip22,lan22,Lan2,ip2,"/etc/msa/msa/start/vps.up");
			   system(caCmdCon);
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p tcp  -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip22,lan22,Lan2,ip2,"/etc/msa/msa/start/vps.up");
			  }else if(strcmp(pro2,"0")==0){//UDP
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p udp -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip22,lan22,Lan2,ip2,"/etc/msa/msa/start/vps.up");
			  }else if(strcmp(pro2,"1")==0){//TCP
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p tcp -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip22,lan22,Lan2,ip2,"/etc/msa/msa/start/vps.up");
			  }
			  system(caCmdCon);
		 }
		 if(strcmp(enable3,"1")==0){
			  //#/sbin/iptables -t nat -A PREROUTING -p tcp -d $addr0 --dport 20 -i eth0 -j DNAT --to $addr3:20  
				if(strcmp(pro3,"-1")==0){//all
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p udp  -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip33,lan33,Lan3,ip3,"/etc/msa/msa/start/vps.up");
			   system(caCmdCon);
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p tcp  -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip33,lan33,Lan3,ip3,"/etc/msa/msa/start/vps.up");
			  }else if(strcmp(pro3,"0")==0){//UDP
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p udp -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip33,lan33,Lan3,ip3,"/etc/msa/msa/start/vps.up");
			  }else if(strcmp(pro3,"1")==0){//TCP
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p tcp -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip33,lan33,Lan3,ip3,"/etc/msa/msa/start/vps.up");
			  }
			  system(caCmdCon);
		 }
		 if(strcmp(enable4,"1")==0){
			  //#/sbin/iptables -t nat -A PREROUTING -p tcp -d $addr0 --dport 20 -i eth0 -j DNAT --to $addr3:20  
				if(strcmp(pro4,"-1")==0){//all
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p udp  -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip44,lan44,Lan4,ip4,"/etc/msa/msa/start/vps.up");
			   system(caCmdCon);
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p tcp  -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip44,lan44,Lan4,ip4,"/etc/msa/msa/start/vps.up");
			  }else if(strcmp(pro4,"0")==0){//UDP
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p udp -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip44,lan44,Lan4,ip4,"/etc/msa/msa/start/vps.up");
			  }else if(strcmp(pro4,"1")==0){//TCP
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p tcp -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip44,lan44,Lan4,ip4,"/etc/msa/msa/start/vps.up");
			  }
			  system(caCmdCon);
		 }
		 if(strcmp(enable5,"1")==0){
			  //#/sbin/iptables -t nat -A PREROUTING -p tcp -d $addr0 --dport 20 -i eth0 -j DNAT --to $addr3:20  
				if(strcmp(pro5,"-1")==0){//all
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p udp  -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip55,lan55,Lan5,ip5,"/etc/msa/msa/start/vps.up");
			   system(caCmdCon);
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p tcp  -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip55,lan55,Lan5,ip5,"/etc/msa/msa/start/vps.up");
			  }else if(strcmp(pro5,"0")==0){//UDP
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p udp -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip55,lan55,Lan5,ip5,"/etc/msa/msa/start/vps.up");
			  }else if(strcmp(pro5,"1")==0){//TCP
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p tcp -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip55,lan55,Lan5,ip5,"/etc/msa/msa/start/vps.up");
			  }
			  system(caCmdCon);
		 }
		 if(strcmp(enable6,"1")==0){
			  //#/sbin/iptables -t nat -A PREROUTING -p tcp -d $addr0 --dport 20 -i eth0 -j DNAT --to $addr3:20  
				if(strcmp(pro6,"-1")==0){//all
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p udp  -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip66,lan66,Lan6,ip6,"/etc/msa/msa/start/vps.up");
			   system(caCmdCon);
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p tcp  -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip66,lan66,Lan6,ip6,"/etc/msa/msa/start/vps.up");
			  }else if(strcmp(pro6,"0")==0){//UDP
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p udp -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip66,lan66,Lan6,ip6,"/etc/msa/msa/start/vps.up");
			  }else if(strcmp(pro6,"1")==0){//TCP
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p tcp -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip66,lan66,Lan6,ip6,"/etc/msa/msa/start/vps.up");
			  }
			  system(caCmdCon);
		 }
		 if(strcmp(enable7,"1")==0){
			  //#/sbin/iptables -t nat -A PREROUTING -p tcp -d $addr0 --dport 20 -i eth0 -j DNAT --to $addr3:20  
				if(strcmp(pro7,"-1")==0){//all
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p udp  -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip77,lan77,Lan7,ip7,"/etc/msa/msa/start/vps.up");
			   system(caCmdCon);
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p tcp  -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip77,lan77,Lan7,ip7,"/etc/msa/msa/start/vps.up");
			  }else if(strcmp(pro7,"0")==0){//UDP
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p udp -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip77,lan77,Lan7,ip7,"/etc/msa/msa/start/vps.up");
			  }else if(strcmp(pro7,"1")==0){//TCP
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p tcp -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip77,lan77,Lan7,ip7,"/etc/msa/msa/start/vps.up");
			  }
			  system(caCmdCon);
		 }
		 if(strcmp(enable8,"1")==0){
			  //#/sbin/iptables -t nat -A PREROUTING -p tcp -d $addr0 --dport 20 -i eth0 -j DNAT --to $addr3:20  
				if(strcmp(pro8,"-1")==0){//all
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p udp  -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip88,lan88,Lan8,ip8,"/etc/msa/msa/start/vps.up");
			   system(caCmdCon);
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p tcp  -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip88,lan88,Lan8,ip8,"/etc/msa/msa/start/vps.up");
			   
			  }else if(strcmp(pro8,"0")==0){//UDP
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p udp -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip88,lan88,Lan8,ip8,"/etc/msa/msa/start/vps.up");
			  }else if(strcmp(pro8,"1")==0){//TCP
			   sprintf(caCmdCon,"echo 'iptables -t nat -I PREROUTING -p tcp -d %s --dport %s -i eth%s -j DNAT --to %s' >> %s",ip88,lan88,Lan8,ip8,"/etc/msa/msa/start/vps.up");
			  }
			  system(caCmdCon);
		 }
		 ///////system("chmod 755 %s",文件名);
		 vserver(confd,ltMsgPk,lt_MMHead);
		 return 0;
}
/*
*DDNS
*/
int ddnsset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
			char servername[24],username[24],userpass[24];
			char str[16];
      char *pFile="/etc/msa/msa/DDNS/settings";
      
     	ltDbHeadPtr dbPtr;
     	dbPtr=lt_dbinit();
	    lt_db_htmlpage(dbPtr,"utf-8");
    
	    
		  memset(servername,0,sizeof(servername));
		  bcCnfGetValue_s("/etc/msa/msa/DDNS/settings1","ddnsenable",servername);
			memset(str,0,sizeof(str));
			sprintf(str,"servername%s",servername);
			lt_dbput_rootvars(dbPtr,1,str,"selected");
    
	    memset(username,0,sizeof(username));
	    bcCnfGetValue_s(pFile,"szUserID",username);
	    lt_dbput_rootvars(dbPtr,1,"username",username);
		 
	    memset(userpass,0,sizeof(userpass));
	    bcCnfGetValue_s(pFile,"szUserPWD",userpass);
	    lt_dbput_rootvars(dbPtr,1,"userpass",userpass);
		   
	    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/nasDDNSPage.htm",dbPtr->head,0);
			ltMsgFree(ltMsgPk);
			lt_dbfree(dbPtr);
			return 0;
}
/*
*配置DDNS
*/
int ltDDNSok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char *servername,*username,*userpass;
	char caCmdCon[128];
	
	servername=ltMsgGetVar_s(ltMsgPk,"servername");
	username=ltMsgGetVar_s(ltMsgPk,"username");
	userpass=ltMsgGetVar_s(ltMsgPk,"userpass");
	
	if(strcmp(servername,"1")==0){
		memset(caCmdCon,0,sizeof(caCmdCon));
	  sprintf(caCmdCon,"echo 'ddnsenable=0' > %s","/etc/msa/msa/DDNS/settings1");
	  system(caCmdCon);
	}else{
		memset(caCmdCon,0,sizeof(caCmdCon));
	  sprintf(caCmdCon,"echo 'ddnsenable=1' > %s","/etc/msa/msa/DDNS/settings1");
	  system(caCmdCon);
	}
	
	system("chmod 755 /etc/msa/msa/DDNS/settings1");
	
	memset(caCmdCon,0,sizeof(caCmdCon));
  sprintf(caCmdCon,"echo '[settings]' > %s","/etc/msa/msa/DDNS/settings");
  system(caCmdCon);
  
  memset(caCmdCon,0,sizeof(caCmdCon));
  sprintf(caCmdCon,"echo 'szHost = PhLinux3.Oray.Net' > %s","/etc/msa/msa/DDNS/settings");
  system(caCmdCon);
	  
  memset(caCmdCon,0,sizeof(caCmdCon));
  sprintf(caCmdCon,"echo 'szUserID=%s' >> %s",username,"/etc/msa/msa/DDNS/settings");
  system(caCmdCon);
  
  memset(caCmdCon,0,sizeof(caCmdCon));
  sprintf(caCmdCon,"echo 'szUserPWD=%s' >> %s",userpass,"/etc/msa/msa/DDNS/settings");
  system(caCmdCon);
  
  memset(caCmdCon,0,sizeof(caCmdCon));
  sprintf(caCmdCon,"echo 'nicName=eth0' >> %s","/etc/msa/msa/DDNS/settings");
  system(caCmdCon);

  
  memset(caCmdCon,0,sizeof(caCmdCon));
  sprintf(caCmdCon,"echo '' >> %s","/etc/msa/msa/DDNS/settings");
  system(caCmdCon);
  
  memset(caCmdCon,0,sizeof(caCmdCon));
  sprintf(caCmdCon,"echo 'file = /var/log/phlinux.log' >> %s","/etc/msa/msa/DDNS/settings");
  system(caCmdCon);
  
  
  ddnsset(confd,ltMsgPk,lt_MMHead);
	return 0;
}


int msaLoginDdns(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char strP[1024];
	char buff[1024],str[1024],utf8str[2048];
	char *pppp;
 	int  iSocket;
 	struct sockaddr_in dest;
	struct   hostent   *hp   =   NULL; 

	iSocket = (-1);
	
	if((hp=gethostbyname( "ddns.oray.com"))!=NULL){ 
		memcpy(&(dest.sin_addr),hp-> h_addr,hp-> h_length); 
		dest.sin_family=hp-> h_addrtype; 
		//printf( "IP:!dest.sin_addr   =   %s\n ",   inet_ntoa(dest.sin_addr)); 
	}
  iSocket = lt_tcpconnect(inet_ntoa(dest.sin_addr),80); //80 or 443
	if(iSocket < 0){
		perror("oops:iSocket");
		return 0;
	}
	sprintf(strP,"%s:%s",ltMsgGetVar_s(ltMsgPk,"uname"),ltMsgGetVar_s(ltMsgPk,"password"));
	int outlen;
	char strCodeOut[1024];
	memset(strCodeOut,0,sizeof(strCodeOut));
	memset(buff,0,sizeof(buff));
	ltEncode64(strP,strlen(strP), strCodeOut, 1024, &outlen);

	memset(str,0,sizeof(str));
	sprintf(str,"GET /ph/update HTTP/1.0\r\nHost: ddns.oray.com\r\nAuthorization: Basic %s\r\nUser-Agent: msa\n\n",strCodeOut);

  write(iSocket,str,strlen(str));
  read(iSocket,buff,sizeof(buff));
  close(iSocket);
  pppp=strstr(buff,"\r\n\r\n");
  if(pppp){
  	*pppp='\0';
		pppp=pppp+4;
  }
  sprintf(buff,"%s",pppp);
  //printf("buff   %s \n",buff);
//  printf("pppp   %s \n",pppp);
  memset(utf8str,0,sizeof(utf8str));
  if(strcmp(buff,"good")==0){
  	strgb2utf8("更新成功，域名的IP地址已经更新。本次更新IP：",utf8str,strlen("更新成功，域名的IP地址已经更新。本次更新IP："));
  	sprintf(utf8str,"%s%s",utf8str,pppp);
  }
  if(strcmp(buff,"nochg")==0){
  	strgb2utf8("更新成功，但没有改变IP。上次更新IP：",utf8str,strlen("更新成功，但没有改变IP。上次更新IP："));
  	sprintf(utf8str,"%s%s",utf8str,pppp);
  }
  if(strcmp(buff,"notfqdn")==0){
  	strgb2utf8("未有激活花生壳的域名。",utf8str,strlen("未有激活花生壳的域名。"));
  }
  if(strcmp(buff,"nohost")==0){
  	strgb2utf8("域名不存在或未激活花生壳。",utf8str,strlen("域名不存在或未激活花生壳。"));
  }
  if(strcmp(buff,"abuse")==0){
  	strgb2utf8("请求频繁,请稍后再试。",utf8str,strlen("请求频繁,请稍后再试。"));
  }
  if(strcmp(buff,"badauth")==0){
  	strgb2utf8("验证失败，用户名或密码错误。",utf8str,strlen("验证失败，用户名或密码错误。"));
  }
  if(strcmp(buff,"badagent")==0){
  	strgb2utf8("请求的User-Agent不合法。",utf8str,strlen("请求的User-Agent不合法。"));
  }
  if(strcmp(buff,"!donator")==0){
  	strgb2utf8("此功能需要付费用户才能使用。",utf8str,strlen("此功能需要付费用户才能使用。"));
  }
  if(strcmp(buff,"911")==0){
  	strgb2utf8("系统错误。",utf8str,strlen("系统错误。"));
  }

	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,utf8str);
	ltMsgFree(ltMsgPk);
	return 0;
}

int ltdesktopset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){      
     	ltDbHeadPtr dbPtr;
     	dbPtr=lt_dbinit();
	    lt_db_htmlpage(dbPtr,"utf-8");
	    
		  char *dbName;
			dbName=_ltPubInfo->_dbname;
			
			lt_dbput_rootvars(dbPtr,1,"dbName",dbName);
	    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/desktop/desktop.html",dbPtr->head,0);
			ltMsgFree(ltMsgPk);
			lt_dbfree(dbPtr);
			return 0;
}

int netconnecttype(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){      
     	char mytype[16];
			ltDbHeadPtr dbPtr;
			dbPtr=lt_dbinit();
			lt_db_htmlpage(dbPtr,"utf-8");
			//记录级别
			memset(mytype,0,sizeof(mytype));
		  bcCnfGetValue_s("/etc/msa/msa/guide/type","type",mytype);
		  if(strcmp(mytype,"3")==0){
		     lt_dbput_rootvars(dbPtr,1,"type3radio","checked");    
		  }else if(strcmp(mytype,"6")==0){
		     lt_dbput_rootvars(dbPtr,1,"type6radio","checked");         
		  }else{
		  	 lt_dbput_rootvars(dbPtr,1,"type3radio"," ");
		  	 lt_dbput_rootvars(dbPtr,1,"type6radio"," ");
		  }
		  lt_dbput_rootvars(dbPtr,1,"type",mytype);
	    lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/disposition/netconnecttype.htm",dbPtr->head,0);
			ltMsgFree(ltMsgPk);
			lt_dbfree(dbPtr);
			return 0;
}

/*加载部署向导页面*/
int disposition(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	checkTimeOut(confd,ltMsgPk,lt_MMHead);
	ltDbHeadPtr dbPtr;
	dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
  
  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/disposition/netconnectconf.htm",dbPtr->head,0);        
	ltMsgFree(ltMsgPk); 
	lt_dbfree(dbPtr);
	return 0;
}

int sysnet3conf(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  char myinLanA[32],myoutLanA[32],myinLanB[32],myoutLanB[32];
  char mymanagerLan[32],mymanagerIPv4[32],mymanagerIPv6[32],myinmask4[32],myinmask6[32],myGateway4[32],myGateway6[32];
  char myLanchk1[32],myLanchk2[32],myLanchkM[32];
  char myipv4chk[10],myipv6chk[10];
  ////////////////  
  int   nownum;
  int   match;
  int   jjjj;
  char  mynownum[16];
  
  ltDbHeadPtr dbPtr;
  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	
  static char inbuf[4096];
  int   fd;
  fd=open("/proc/net/dev",O_RDONLY | O_NDELAY); 
	static stralloc line = {0};
	buffer ss;
	nownum=0;
	buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
	for (;;){
		if (getln(&ss,&line,&match,'\n') == -1){ break;}
		if (!match && !line.len) { break; }
		for(jjjj=0;jjjj<line.len;jjjj++){
			 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h'){
				 nownum++;		
		   }
	  }
	}
	sprintf(mynownum,"%d",nownum);
	lt_dbput_rootvars(dbPtr,1,"nownum",mynownum);
  //////////////// 
  memset(myLanchk1,0,sizeof(myLanchk1));
  bcCnfGetValue_s("/etc/msa/msa/guide/Lanchk1","Lanchk1",myLanchk1);
  if(strcmp(myLanchk1,"1")==0){
 	 lt_dbput_rootvars(dbPtr,1,"Lanchk1chk","checked");
  }else{
 	 lt_dbput_rootvars(dbPtr,1,"Lanchk1chk","");
  }
  
  memset(myLanchk2,0,sizeof(myLanchk2));
  bcCnfGetValue_s("/etc/msa/msa/guide/Lanchk2","Lanchk2",myLanchk2);
  if(strcmp(myLanchk2,"1")==0){
 	 lt_dbput_rootvars(dbPtr,1,"Lanchk2chk","checked");
  }else{
 	 lt_dbput_rootvars(dbPtr,1,"Lanchk2chk","");
  }
  
  memset(myLanchkM,0,sizeof(myLanchkM));
  bcCnfGetValue_s("/etc/msa/msa/guide/LanchkM","LanchkM",myLanchkM);
  if(strcmp(myLanchkM,"1")==0){
 	 lt_dbput_rootvars(dbPtr,1,"LanchkMchk","checked");
  }else{
 	 lt_dbput_rootvars(dbPtr,1,"LanchkMchk","");
  }
  
  ltTablePtr  tablePtr;
 	int iii;
  
  bcCnfGetValue_s("/etc/msa/msa/guide/inLanA","inLanA",myinLanA);
  tablePtr=lt_dbput_table(dbPtr,"Lanlist1");	
	for(iii=0;iii<nownum;iii++){
			if(atol(myinLanA)==iii){
				lt_dbput_recordvars(tablePtr,2,"vlanid",LT_TYPE_LONG,iii,"selvid",LT_TYPE_STRING,"selected");
			}else{
				lt_dbput_recordvars(tablePtr,1,"vlanid",LT_TYPE_LONG,iii);
			}
	}
  bcCnfGetValue_s("/etc/msa/msa/guide/outLanA","outLanA",myoutLanA);
 	tablePtr=lt_dbput_table(dbPtr,"Lanlist2");	
	for(iii=0;iii<nownum;iii++){
			if(atol(myoutLanA)==iii){
				lt_dbput_recordvars(tablePtr,2,"vlanid",LT_TYPE_LONG,iii,"selvid",LT_TYPE_STRING," selected ");
			}else{
				lt_dbput_recordvars(tablePtr,1,"vlanid",LT_TYPE_LONG,iii);
			}
	}

 	////////////////////////////////////////////////////////////////////
  bcCnfGetValue_s("/etc/msa/msa/guide/inLanB","inLanB",myinLanB);
 	tablePtr=lt_dbput_table(dbPtr,"Lanlist3");	
	for(iii=0;iii<nownum;iii++){
			if(atol(myinLanB)==iii){
				lt_dbput_recordvars(tablePtr,2,"vlanid",LT_TYPE_LONG,iii,"selvid",LT_TYPE_STRING," selected ");
			}else{
				lt_dbput_recordvars(tablePtr,1,"vlanid",LT_TYPE_LONG,iii);
			}
	}
  bcCnfGetValue_s("/etc/msa/msa/guide/outLanB","outLanB",myoutLanB);
 	tablePtr=lt_dbput_table(dbPtr,"Lanlist4");	
	for(iii=0;iii<nownum;iii++){
			if(atol(myoutLanB)==iii){
				lt_dbput_recordvars(tablePtr,2,"vlanid",LT_TYPE_LONG,iii,"selvid",LT_TYPE_STRING," selected ");
			}else{
				lt_dbput_recordvars(tablePtr,1,"vlanid",LT_TYPE_LONG,iii);
			}
	}
 	/////////////////////////////////////////////
  bcCnfGetValue_s("/etc/msa/msa/guide/managerLan","managerLan",mymanagerLan);
 	tablePtr=lt_dbput_table(dbPtr,"Lanlist9");	
	for(iii=0;iii<nownum;iii++){
			if(atol(mymanagerLan)==iii){
				lt_dbput_recordvars(tablePtr,2,"vlanid",LT_TYPE_LONG,iii,"selvid",LT_TYPE_STRING," selected ");
			}else{
				lt_dbput_recordvars(tablePtr,1,"vlanid",LT_TYPE_LONG,iii);
			}
	}
 	
 	/////////////////启动项
  memset(myipv4chk,0,sizeof(myipv4chk));
  bcCnfGetValue_s("/etc/msa/msa/guide/ipv4chk","ipv4chk",myipv4chk);
  if(strcmp(myipv4chk,"1")==0){
 	 lt_dbput_rootvars(dbPtr,1,"ipv4chk","checked");
  }else{
 	 lt_dbput_rootvars(dbPtr,1,"ipv4chk","");
  }
  memset(myipv6chk,0,sizeof(myipv6chk));
  bcCnfGetValue_s("/etc/msa/msa/guide/ipv6chk","ipv6chk",myipv6chk);
  if(strcmp(myipv6chk,"1")==0){
 	 lt_dbput_rootvars(dbPtr,1,"ipv6chk","checked");
  }else{
 	 lt_dbput_rootvars(dbPtr,1,"ipv6chk","");
  }
  
  memset(mymanagerIPv4,0,sizeof(mymanagerIPv4));
  bcCnfGetValue_s("/etc/msa/msa/guide/managerIPv4","managerIPv4",mymanagerIPv4);
  lt_dbput_rootvars(dbPtr,1,"managerIPv4",mymanagerIPv4);
 	
  memset(myinmask4,0,sizeof(myinmask4));
  bcCnfGetValue_s("/etc/msa/msa/guide/inmask4","inmask4",myinmask4);
 	lt_dbput_rootvars(dbPtr,1,"inmask4",myinmask4);
 	
  memset(myGateway4,0,sizeof(myGateway4));
  bcCnfGetValue_s("/etc/msa/msa/guide/Gateway4","Gateway4",myGateway4);
 	lt_dbput_rootvars(dbPtr,1,"Gateway4",myGateway4);
 	
  memset(mymanagerIPv6,0,sizeof(mymanagerIPv6));
  bcCnfGetValue_s("/etc/msa/msa/guide/managerIPv6","managerIPv6",mymanagerIPv6);
  lt_dbput_rootvars(dbPtr,1,"managerIPv6",mymanagerIPv6);
 	
  memset(myinmask6,0,sizeof(myinmask6));
  bcCnfGetValue_s("/etc/msa/msa/guide/inmask6","inmask6",myinmask6);
 	lt_dbput_rootvars(dbPtr,1,"inmask6",myinmask6);
 	
  memset(myGateway6,0,sizeof(myGateway6));
  bcCnfGetValue_s("/etc/msa/msa/guide/Gateway6","Gateway6",myGateway6);
 	lt_dbput_rootvars(dbPtr,1,"Gateway6",myGateway6);
 	
  memset(myinmask6,0,sizeof(myinmask6));
  bcCnfGetValue_s("/etc/msa/msa/guide/inmask6","inmask6",myinmask6);
 	lt_dbput_rootvars(dbPtr,1,"inmask6",myinmask6);
	
	lt_dbput_rootvars(dbPtr,1,"type","3");
  
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/disposition/sysnetconf3.htm",dbPtr->head,0); 
	ltMsgFree(ltMsgPk); 
	lt_dbfree(dbPtr);
	return 0;
}

int sysnet6conf(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	char myListen1Lan[32],myListen2Lan[32];
  char myListenchk1[32],myListenchk2[32];
  char mymanagerLan[64],mymanagerIPv4[64],mymanagerIPv6[64],myinmask4[64],myinmask6[64];
  char myipv4chk[10],myipv6chk[10];
  char myGateway4[32],myGateway6[32];
  int   nownum;
  int   match;
  int   jjjj;
  char  mynownum[16];
  int   iii;
  ltDbHeadPtr dbPtr;
  ltTablePtr  tablePtr;
  dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	
  static char inbuf[4096];
  int   fd;
  fd=open("/proc/net/dev",O_RDONLY | O_NDELAY); 
	static stralloc line = {0};
	buffer ss;
	nownum=0;
	buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
	for (;;){
		if (getln(&ss,&line,&match,'\n') == -1){ break;}
		if (!match && !line.len) { break; }
		for(jjjj=0;jjjj<line.len;jjjj++){
			 if(line.s[jjjj]=='e' && line.s[jjjj+1]=='t' && line.s[jjjj+2]=='h'){
				 nownum++;		
		   }
	  }
	}
	sprintf(mynownum,"%d",nownum);
	lt_dbput_rootvars(dbPtr,1,"nownum",mynownum);
  ///////////////
  
  bcCnfGetValue_s("/etc/msa/msa/guide3/Listen1Lan","Listen1Lan",myListen1Lan);
 	tablePtr=lt_dbput_table(dbPtr,"Lanlist1");	
	for(iii=0;iii<nownum;iii++){
			if(atol(myListen1Lan)==iii){
				lt_dbput_recordvars(tablePtr,2,"vlanid",LT_TYPE_LONG,iii,"selvid",LT_TYPE_STRING," selected ");
			}else{
				lt_dbput_recordvars(tablePtr,1,"vlanid",LT_TYPE_LONG,iii);
			}
	}
 	
  
  bcCnfGetValue_s("/etc/msa/msa/guide3/Listen2Lan","Listen2Lan",myListen2Lan);
 	tablePtr=lt_dbput_table(dbPtr,"Lanlist2");	
	for(iii=0;iii<nownum;iii++){
			if(atol(myListen2Lan)==iii){
				lt_dbput_recordvars(tablePtr,2,"vlanid",LT_TYPE_LONG,iii,"selvid",LT_TYPE_STRING," selected ");
			}else{
				lt_dbput_recordvars(tablePtr,1,"vlanid",LT_TYPE_LONG,iii);
			}
	}
 	/////////////////监控口
  memset(myListenchk1,0,sizeof(myListenchk1));
  bcCnfGetValue_s("/etc/msa/msa/guide3/Listenchk1","Listenchk1",myListenchk1);
  if(strcmp(myListenchk1,"1")==0){
 	 lt_dbput_rootvars(dbPtr,1,"Listenchk1chk","checked");
  }else{
 	 lt_dbput_rootvars(dbPtr,1,"Listenchk1chk","");
  }
 	
  memset(myListenchk2,0,sizeof(myListenchk2));
  bcCnfGetValue_s("/etc/msa/msa/guide3/Listenchk2","Listenchk2",myListenchk2);
  if(strcmp(myListenchk2,"1")==0){
 	 lt_dbput_rootvars(dbPtr,1,"Listenchk2chk","checked");
  }else{
 	 lt_dbput_rootvars(dbPtr,1,"Listenchk2chk","");
  }
  //管理口
  bcCnfGetValue_s("/etc/msa/msa/guide3/managerLan","managerLan",mymanagerLan);
 	tablePtr=lt_dbput_table(dbPtr,"Lanlist5");	
	for(iii=0;iii<nownum;iii++){
			if(atol(mymanagerLan)==iii){
				lt_dbput_recordvars(tablePtr,2,"vlanid",LT_TYPE_LONG,iii,"selvid",LT_TYPE_STRING," selected ");
			}else{
				lt_dbput_recordvars(tablePtr,1,"vlanid",LT_TYPE_LONG,iii);
			}
	}
 	/////////////////启动项
  memset(myipv4chk,0,sizeof(myipv4chk));
  bcCnfGetValue_s("/etc/msa/msa/guide3/ipv4chk","ipv4chk",myipv4chk);
  if(strcmp(myipv4chk,"1")==0){
 	 lt_dbput_rootvars(dbPtr,1,"ipv4chk","checked");
  }else{
 	 lt_dbput_rootvars(dbPtr,1,"ipv4chk","");
  }
  
  memset(myipv6chk,0,sizeof(myipv6chk));
  bcCnfGetValue_s("/etc/msa/msa/guide3/ipv6chk","ipv6chk",myipv6chk);
  if(strcmp(myipv6chk,"1")==0){
 	 lt_dbput_rootvars(dbPtr,1,"ipv6chk","checked");
  }else{
 	 lt_dbput_rootvars(dbPtr,1,"ipv6chk","");
  }
  
  memset(mymanagerIPv4,0,sizeof(mymanagerIPv4));
  bcCnfGetValue_s("/etc/msa/msa/guide3/managerIPv4","managerIPv4",mymanagerIPv4);
 	lt_dbput_rootvars(dbPtr,1,"managerIPv4",mymanagerIPv4);
 	
  memset(myinmask4,0,sizeof(myinmask4));
  bcCnfGetValue_s("/etc/msa/msa/guide3/inmask4","inmask4",myinmask4);
 	lt_dbput_rootvars(dbPtr,1,"inmask4",myinmask4);
 	
  memset(myGateway4,0,sizeof(myGateway4));
  bcCnfGetValue_s("/etc/msa/msa/guide3/Gateway4","Gateway4",myGateway4);
 	lt_dbput_rootvars(dbPtr,1,"Gateway4",myGateway4);
  
  memset(mymanagerIPv6,0,sizeof(mymanagerIPv6));
  bcCnfGetValue_s("/etc/msa/msa/guide3/managerIPv6","managerIPv6",mymanagerIPv6);
 	lt_dbput_rootvars(dbPtr,1,"managerIPv6",mymanagerIPv6);
 	
  memset(myinmask6,0,sizeof(myinmask6));
  bcCnfGetValue_s("/etc/msa/guide3/inmask6","inmask6",myinmask6);
 	lt_dbput_rootvars(dbPtr,1,"inmask6",myinmask6);
 	
  memset(myGateway6,0,sizeof(myGateway6));
  bcCnfGetValue_s("/etc/msa/msa/guide3/Gateway6","Gateway6",myGateway6);
 	lt_dbput_rootvars(dbPtr,1,"Gateway6",myGateway6);
 	
 	
	
	lt_dbput_rootvars(dbPtr,1,"type","6");

	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/disposition/sysnetconf6.htm",dbPtr->head,0); 
	ltMsgFree(ltMsgPk); 
	lt_dbfree(dbPtr);
	return 0;
}

int sysnetconfok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  int  actiontype;
  int   mondev=1;
	if(ltMsgGetVar_s(ltMsgPk,"actiontype")!=NULL){
		actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));
		char caCmdCon[128];
		if(actiontype==3){//二路网桥
			char *myLanchk1,*myLanchk2,*myLanchkM;//是否启动网桥
			  char *myinLanA,*myoutLanA;//网桥一：内网口 外网口    IPv4   子网掩码 IPv6
			  char *myinLanB,*myoutLanB;//网桥二：内网口 外网口    IPv4   子网掩码 IPv6
			  char *mymanagerIPv4,*mymanagerIPv6,*myinmask4,*myinmask6,*myGateway4,*myGateway6,*mymanagerLan;//管理口：IP 子网掩码 管理口
			  char *myipv4chk,*myipv6chk;
			  
			  myLanchk1=ltMsgGetVar_s(ltMsgPk,"Lanchk1");
			  myLanchk2=ltMsgGetVar_s(ltMsgPk,"Lanchk2");
			  myLanchkM=ltMsgGetVar_s(ltMsgPk,"LanchkM");
			  
			  myinLanA=ltMsgGetVar_s(ltMsgPk,"inLanA");
			  myoutLanA=ltMsgGetVar_s(ltMsgPk,"outLanA");
			  
			  myinLanB=ltMsgGetVar_s(ltMsgPk,"inLanB");
			  myoutLanB=ltMsgGetVar_s(ltMsgPk,"outLanB");
			  
			  mymanagerLan=ltMsgGetVar_s(ltMsgPk,"managerLan");
				mymanagerIPv4=ltMsgGetVar_s(ltMsgPk,"managerIPv4");
			  myinmask4=ltMsgGetVar_s(ltMsgPk,"inmask4");
				mymanagerIPv6=ltMsgGetVar_s(ltMsgPk,"managerIPv6");
			  myinmask6=ltMsgGetVar_s(ltMsgPk,"inmask6");
			  myGateway4=ltMsgGetVar_s(ltMsgPk,"Gateway4");
			  myGateway6=ltMsgGetVar_s(ltMsgPk,"Gateway6");
			  
			  myipv4chk=ltMsgGetVar_s(ltMsgPk,"ipv4chk");
			  myipv6chk=ltMsgGetVar_s(ltMsgPk,"ipv6chk");
			
				memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'type=3' > %s","/etc/msa/msa/guide/type");
				system(caCmdCon);
			  
			  memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'Lanchk1=%s' > %s",myLanchk1,"/etc/msa/msa/guide/Lanchk1");
				system(caCmdCon);
				if(strcmp(myLanchk1,"1")==0){
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'inLanA=%s' > %s",myinLanA,"/etc/msa/msa/guide/inLanA");
					system(caCmdCon);
	
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'outLanA=%s' > %s",myoutLanA,"/etc/msa/msa/guide/outLanA");
					system(caCmdCon);
				}else{
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'inLanA=%s' > %s","","/etc/msa/msa/guide/inLanA");
					system(caCmdCon);
	
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'outLanA=%s' > %s","","/etc/msa/msa/guide/outLanA");
					system(caCmdCon);
				}
			  
			  memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'Lanchk2=%s' > %s",myLanchk2,"/etc/msa/msa/guide/Lanchk2");
				system(caCmdCon);
				if(strcmp(myLanchk2,"1")==0){
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'inLanB=%s' > %s",myinLanB,"/etc/msa/msa/guide/inLanB");
					system(caCmdCon);
	
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'outLanB=%s' > %s",myoutLanB,"/etc/msa/msa/guide/outLanB");
					system(caCmdCon);
				}else{
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'inLanB=%s' > %s","","/etc/msa/msa/guide/inLanB");
					system(caCmdCon);
	
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'outLanB=%s' > %s","","/etc/msa/msa/guide/outLanB");
					system(caCmdCon);
				}
				
				memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'LanchkM=%s' > %s",myLanchkM,"/etc/msa/msa/guide/LanchkM");
				system(caCmdCon);
				if(strcmp(myLanchkM,"1")==0){
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'managerIPv4=%s' > %s",mymanagerIPv4,"/etc/msa/msa/guide/managerIPv4");
					system(caCmdCon);
	  
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'inmask4=%s' > %s",myinmask4,"/etc/msa/msa/guide/inmask4");
					system(caCmdCon);
	  
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'Gateway4=%s' > %s",myGateway4,"/etc/msa/msa/guide/Gateway4");
					system(caCmdCon);
	
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'managerLan=%s' > %s",mymanagerLan,"/etc/msa/msa/guide/managerLan");
					system(caCmdCon);
					
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'managerIPv6=%s' > %s",mymanagerIPv6,"/etc/msa/msa/guide/managerIPv6");
					system(caCmdCon);
	  
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'inmask6=%s' > %s",myinmask6,"/etc/msa/msa/guide/inmask6");
					system(caCmdCon);
	  
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'Gateway6=%s' > %s",myGateway6,"/etc/msa/msa/guide/Gateway6");
					system(caCmdCon);
					
					memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'type=3' > %s","/etc/msa/msa/guide/type");
					system(caCmdCon);
	  
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'ipv4chk=%s' > %s",myipv4chk,"/etc/msa/msa/guide/ipv4chk");
					system(caCmdCon);
	  
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'ipv6chk=%s' > %s",myipv6chk,"/etc/msa/msa/guide/ipv6chk");
					system(caCmdCon);
				}else{		      
					memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'managerIPv4=%s' > %s","","/etc/msa/msa/guide/managerIPv4");
					system(caCmdCon);
	  
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'inmask4=%s' > %s","","/etc/msa/msa/guide/inmask4");
					system(caCmdCon);
					
					memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'managerIPv6=%s' > %s","","/etc/msa/msa/guide/managerIPv6");
					system(caCmdCon);
	  
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'inmask6=%s' > %s","","/etc/msa/msa/guide/inmask6");
					system(caCmdCon);
	
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'managerLan=%s' > %s","","/etc/msa/msa/guide/managerLan");
					system(caCmdCon);
	  
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'ipv4chk=0' > %s","/etc/msa/msa/guide/ipv4chk");
					system(caCmdCon);
	  
		      memset(caCmdCon,0,sizeof(caCmdCon));
					sprintf(caCmdCon,"echo 'ipv6chk=0' > %s","/etc/msa/msa/guide/ipv6chk");
					system(caCmdCon);
				}
	  
			 //写启动脚本
			 system("echo '' > /etc/msa/msa/start/linkup");
	     system("/bin/echo 'dev1=0' > /etc/msa/msa/dev/dev1"); 
	     system("/bin/echo 'pdev1=0' > /etc/msa/msa/dev/pdev1"); 
       system("/bin/echo 'dev2=0' > /etc/msa/msa/dev/dev2"); 
       system("/bin/echo 'pdev2=1' > /etc/msa/msa/dev/pdev2"); 
       system("/bin/echo 'dev3=0' > /etc/msa/msa/dev/dev3"); 
       system("/bin/echo 'pdev3=2' > /etc/msa/msa/dev/pdev3");
       system("/bin/echo 'dev4=0' > /etc/msa/msa/dev/dev4"); 
       system("/bin/echo 'pdev4=2' > /etc/msa/msa/dev/pdev4");  
	     if(strcmp(myLanchk1,"1")==0){
           sprintf(caCmdCon,"echo 'ifconfig eth%s 0.0.0.0 promisc  up' >> /etc/msa/msa/start/linkup",myinLanA);              
           system(caCmdCon);
           sprintf(caCmdCon,"echo 'ifconfig eth%s 0.0.0.0 promisc  up' >> /etc/msa/msa/start/linkup",myoutLanA);           
           system(caCmdCon);
					 {
	             system("echo 'brctl addbr ns01' >> /etc/msa/msa/start/linkup");  
	             sprintf(caCmdCon,"echo 'brctl addif ns01 eth%s' >> /etc/msa/msa/start/linkup",myinLanA);  
	             system(caCmdCon);  
	             sprintf(caCmdCon,"echo 'brctl addif ns01 eth%s' >> /etc/msa/msa/start/linkup",myoutLanA);  
	             system(caCmdCon);  
	
              
               system("echo 'ifconfig ns01 0.0.0.0 up' >> /etc/msa/msa/start/linkup");  
               
           }
					 sprintf(caCmdCon,"echo 'dev%d=eth%s' > /etc/msa/msa/dev/dev%d",mondev,myinLanA,mondev);  
           system(caCmdCon); 
           mondev++;
	     }
	     if(strcmp(myLanchk2,"1")==0){
             sprintf(caCmdCon,"echo 'ifconfig eth%s 0.0.0.0 promisc  up' >> /etc/msa/msa/start/linkup",myinLanB);              
             system(caCmdCon);
             sprintf(caCmdCon,"echo 'ifconfig eth%s 0.0.0.0 promisc  up' >> /etc/msa/msa/start/linkup",myoutLanB);           
             system(caCmdCon);
						 {
               system("echo 'brctl addbr ns02' >> /etc/msa/msa/start/linkup");  
               sprintf(caCmdCon,"echo 'brctl addif ns02 eth%s' >> /etc/msa/msa/start/linkup",myinLanB);  
               system(caCmdCon);  
               sprintf(caCmdCon,"echo 'brctl addif ns02 eth%s' >> /etc/msa/msa/start/linkup",myoutLanB);  
							 system(caCmdCon); 
							 
               system("echo 'ifconfig ns02 0.0.0.0 up' >> /etc/msa/msa/start/linkup");  

 						 }
					   sprintf(caCmdCon,"echo 'dev%d=eth%s' > /etc/msa/msa/dev/dev%d",mondev,myinLanB,mondev);  
					   system(caCmdCon); 
					   mondev++;    
	     }        
       if(strcmp(myLanchkM,"1")==0){
       	if(strcmp(myipv4chk,"1")==0){
	         sprintf(caCmdCon,"echo 'ifconfig eth%s %s netmask %s  up' >> /etc/msa/msa/start/linkup",mymanagerLan,mymanagerIPv4,myinmask4);            
	         system(caCmdCon);
	         sprintf(caCmdCon,"echo 'route -A inet6 add default gw %s dev eth%s' >> /etc/msa/msa/start/linkup",myGateway4,mymanagerLan);            
	         system(caCmdCon);
	         
	         msaSetLocalIp(mymanagerIPv4);
       	}
       	if(strcmp(myipv6chk,"1")==0){
	         sprintf(caCmdCon,"echo 'ifconfig eth%s inet6 add  %s/%s' >> /etc/msa/msa/start/linkup",mymanagerLan,mymanagerIPv6,myinmask6);            
	         system(caCmdCon);
	         sprintf(caCmdCon,"echo 'route -A inet6 add default gw %s dev eth%s' >> /etc/msa/msa/start/linkup",myGateway6,mymanagerLan);            
	         system(caCmdCon);
	         
	         msaSetLocalIp(mymanagerIPv6);
       	}
         
       } 
				/////////////

			 ltMsgPk->msgpktype=1;
		   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"1");
	 	   ltMsgFree(ltMsgPk);			
		   return 0;
	  }else if(actiontype==6){//旁路部署
			char *myListenchk1,*myListenchk2;
			char *myListen1Lan,*myListen2Lan;
			char *mymanagerLan,*mymanagerIPv4,*mymanagerIPv6,*myinmask4,*myinmask6;
			char *myipv4chk,*myipv6chk;
			char *myGateway4,*myGateway6;
			
			myListenchk1=ltMsgGetVar_s(ltMsgPk,"Listenchk1");
			myListenchk2=ltMsgGetVar_s(ltMsgPk,"Listenchk2");
			
			myListen1Lan=ltMsgGetVar_s(ltMsgPk,"Listen1Lan");
			myListen2Lan=ltMsgGetVar_s(ltMsgPk,"Listen2Lan");
			
			mymanagerIPv4=ltMsgGetVar_s(ltMsgPk,"managerIPv4");
			mymanagerIPv6=ltMsgGetVar_s(ltMsgPk,"managerIPv6");
			mymanagerLan=ltMsgGetVar_s(ltMsgPk,"managerLan");
			myinmask4=ltMsgGetVar_s(ltMsgPk,"inmask4");
			myinmask6=ltMsgGetVar_s(ltMsgPk,"inmask6");
			myGateway4=ltMsgGetVar_s(ltMsgPk,"Gateway4");
			myGateway6=ltMsgGetVar_s(ltMsgPk,"Gateway6");
			
			myipv4chk=ltMsgGetVar_s(ltMsgPk,"ipv4chk");
			myipv6chk=ltMsgGetVar_s(ltMsgPk,"ipv6chk");
			
			memset(caCmdCon,0,sizeof(caCmdCon));
			sprintf(caCmdCon,"echo 'type=6' > %s","/etc/msa/msa/guide/type");
			system(caCmdCon);
			
			memset(caCmdCon,0,sizeof(caCmdCon));
			sprintf(caCmdCon,"echo 'Listenchk1=%s' > %s",myListenchk1,"/etc/msa/msa/guide3/Listenchk1");
			system(caCmdCon);
			
			memset(caCmdCon,0,sizeof(caCmdCon));
			sprintf(caCmdCon,"echo 'Listenchk2=%s' > %s",myListenchk2,"/etc/msa/msa/guide3/Listenchk2");
			system(caCmdCon);
			
			if(strcmp(myListenchk1,"1")==0){
				memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'Listenchk1=%s' > %s",myListenchk1,"/etc/msa/msa/guide3/Listenchk1");
				system(caCmdCon);
						
				memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'Listen1Lan=%s' > %s",myListen1Lan,"/etc/msa/msa/guide3/Listen1Lan");
				system(caCmdCon);
			}else{
				memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'Listenchk1=%s' > %s","","/etc/msa/msa/guide3/Listenchk1");
				system(caCmdCon);
						
				memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'Listen1Lan=%s' > %s","","/etc/msa/msa/guide3/Listen1Lan");
				system(caCmdCon);
			}
			
			if(strcmp(myListenchk2,"1")==0){
				memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'Listenchk2=%s' > %s",myListenchk2,"/etc/msa/msa/guide3/Listenchk2");
				system(caCmdCon);
						
				memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'Listen2Lan=%s' > %s",myListen2Lan,"/etc/msa/msa/guide3/Listen2Lan");
				system(caCmdCon);
			}else{
				memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'Listenchk2=%s' > %s","","/etc/msa/msa/guide3/Listenchk2");
				system(caCmdCon);
						
				memset(caCmdCon,0,sizeof(caCmdCon));
				sprintf(caCmdCon,"echo 'Listen2Lan=%s' > %s","","/etc/msa/msa/guide3/Listen2Lan");
				system(caCmdCon);
			}
			
			memset(caCmdCon,0,sizeof(caCmdCon));
			sprintf(caCmdCon,"echo 'managerLan=%s' > %s",mymanagerLan,"/etc/msa/msa/guide3/managerLan");
			system(caCmdCon);
			
			memset(caCmdCon,0,sizeof(caCmdCon));
			sprintf(caCmdCon,"echo 'managerIPv4=%s' > %s",mymanagerIPv4,"/etc/msa/msa/guide3/managerIPv4");
			system(caCmdCon);
			
			memset(caCmdCon,0,sizeof(caCmdCon));
			sprintf(caCmdCon,"echo 'managerIPv6=%s' > %s",mymanagerIPv6,"/etc/msa/msa/guide3/managerIPv6");
			system(caCmdCon);
			
			memset(caCmdCon,0,sizeof(caCmdCon));
			sprintf(caCmdCon,"echo 'inmask4=%s' > %s",myinmask4,"/etc/msa/msa/guide3/inmask4");
			system(caCmdCon);
			
			memset(caCmdCon,0,sizeof(caCmdCon));
			sprintf(caCmdCon,"echo 'inmask6=%s' > %s",myinmask6,"/etc/msa/msa/guide3/inmask6");
			system(caCmdCon);
			
			memset(caCmdCon,0,sizeof(caCmdCon));
			sprintf(caCmdCon,"echo 'Gateway4=%s' > %s",myGateway4,"/etc/msa/msa/guide3/Gateway4");
			system(caCmdCon);
			
			memset(caCmdCon,0,sizeof(caCmdCon));
			sprintf(caCmdCon,"echo 'Gateway6=%s' > %s",myGateway6,"/etc/msa/msa/guide3/Gateway6");
			system(caCmdCon);
			
			memset(caCmdCon,0,sizeof(caCmdCon));
			sprintf(caCmdCon,"echo 'ipv4chk=%s' > %s",myipv4chk,"/etc/msa/msa/guide3/ipv4chk");
			system(caCmdCon);
			
			memset(caCmdCon,0,sizeof(caCmdCon));
			sprintf(caCmdCon,"echo 'ipv6chk=%s' > %s",myipv6chk,"/etc/msa/msa/guide3/ipv6chk");
			system(caCmdCon);
			
			
	  
			 //写启动脚本
			 system("echo '' > /etc/msa/msa/start/linkup");
	     system("/bin/echo 'dev1=0' > /etc/msa/msa/dev/dev1"); 
	     system("/bin/echo 'pdev1=0' > /etc/msa/msa/dev/pdev1"); 
       system("/bin/echo 'dev2=0' > /etc/msa/msa/dev/dev2"); 
       system("/bin/echo 'pdev2=1' > /etc/msa/msa/dev/pdev2"); 
       system("/bin/echo 'dev3=0' > /etc/msa/msa/dev/dev3"); 
       system("/bin/echo 'pdev3=2' > /etc/msa/msa/dev/pdev3");
       system("/bin/echo 'dev4=0' > /etc/msa/msa/dev/dev4"); 
       system("/bin/echo 'pdev4=2' > /etc/msa/msa/dev/pdev4");  
	     if(strcmp(myListenchk1,"1")==0){
           sprintf(caCmdCon,"echo 'ifconfig eth%s 0.0.0.0 promisc  up' >> /etc/msa/msa/start/linkup",myListen1Lan);              
           system(caCmdCon);
					 {
	             system("echo 'brctl addbr ns01' >> /etc/msa/msa/start/linkup");  
	             sprintf(caCmdCon,"echo 'brctl addif ns01 eth%s' >> /etc/msa/msa/start/linkup",myListen1Lan);  
	             system(caCmdCon);
               system("echo 'ifconfig ns01 0.0.0.0 up' >> /etc/msa/msa/start/linkup");  
               
           }
					 sprintf(caCmdCon,"echo 'dev%d=eth%s' > /etc/msa/msa/dev/dev%d",mondev,myListen1Lan,mondev);  
           system(caCmdCon); 
           mondev++;
	     }
	     if(strcmp(myListenchk2,"1")==0){
           sprintf(caCmdCon,"echo 'ifconfig eth%s 0.0.0.0 promisc  up' >> /etc/msa/msa/start/linkup",myListen2Lan);              
           system(caCmdCon);
					 {
	             system("echo 'brctl addbr ns01' >> /etc/msa/msa/start/linkup");  
	             sprintf(caCmdCon,"echo 'brctl addif ns01 eth%s' >> /etc/msa/msa/start/linkup",myListen2Lan);  
	             system(caCmdCon);
               system("echo 'ifconfig ns01 0.0.0.0 up' >> /etc/msa/msa/start/linkup");  
               
           }
					 sprintf(caCmdCon,"echo 'dev%d=eth%s' > /etc/msa/msa/dev/dev%d",mondev,myListen2Lan,mondev);  
           system(caCmdCon); 
           mondev++;   
	     }  
       if(strcmp(myipv4chk,"1")==0){
	         sprintf(caCmdCon,"echo 'ifconfig eth%s %s netmask %s  up' >> /etc/msa/msa/start/linkup",mymanagerLan,mymanagerIPv4,myinmask4);            
	         system(caCmdCon);
	         
	         sprintf(caCmdCon,"echo 'route -A inet6 add default gw %s dev eth%s' >> /etc/msa/msa/start/linkup",myGateway4,mymanagerLan);            
	         system(caCmdCon);
	         
	         msaSetLocalIp(mymanagerIPv4);
       }
       if(strcmp(myipv6chk,"1")==0){
	         sprintf(caCmdCon,"echo 'ifconfig eth%s inet6 add  %s/%s' >> /etc/msa/msa/start/linkup",mymanagerLan,mymanagerIPv6,myinmask6);            
	         system(caCmdCon);
	         
	         sprintf(caCmdCon,"echo 'route -A inet6 add default gw %s dev eth%s' >> /etc/msa/msa/start/linkup",myGateway6,mymanagerLan);            
	         system(caCmdCon);
	         
	         msaSetLocalIp(mymanagerIPv6);
       }
			/////////////

			ltMsgPk->msgpktype=1;
		  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"1");
	 	  ltMsgFree(ltMsgPk);			
		  return 0;
		}
	}
   return 0;
}

//软件重新启动
int msaSysApplication(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
  system("killall -2 msawdog");
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"1");
	ltMsgFree(ltMsgPk);			
	return 0;
  
}


/* 初始化升级页面  */
int ltonlinecheck(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   ltDbHeadPtr dbPtr;
   char version [24];
	 char str[16];

   char *pFile="/etc/msa/msa/system/version";
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");

   memset(version ,0,sizeof(version ));
   bcCnfGetValue_s(pFile,"version",version );
	 memset(str,0,sizeof(str));
	 lt_dbput_rootvars(dbPtr,1,"version",version);

   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msa/htmlplt/manager/onlinecheck.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
}

int packagelist(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
   char caCmd[256];
   char caTmpp[512];
   stralloc strTemp;
   int k;
   char caFile[256];
   char cDate[128];
   char reportname[64];
   char leveldesc[128];
   int  fileproc;
   struct stat st;
   int len=0; //sum of file names 
   msasDir *fnames;
	 char filleSize[256];
printf("stype:%d\n",atoi(ltMsgGetVar_s(ltMsgPk,"stype")));
	 if(atoi(ltMsgGetVar_s(ltMsgPk,"stype"))==0){
	   sprintf(caFile,"%s","/app/updateservice");
	 }else if(atoi(ltMsgGetVar_s(ltMsgPk,"stype"))==1){
	   sprintf(caFile,"%s","/app/msa/update");
	 }
   fnames=mydirlist(caFile,&len);
  
   strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",len);
   stralloc_cats(&strTemp,caTmpp);
   k=0;
   
   int cadeflag;
	 if(ltMsgGetVar_s(ltMsgPk,"deflag")!=NULL){
		cadeflag=atoi(ltMsgGetVar_s(ltMsgPk,"deflag"));
		if(cadeflag==1){
			   char *filedir;
			   filedir=ltMsgGetVar_s(ltMsgPk,"del_id");
         if(filedir){
				   if(strlen(filedir)>1){
					  sprintf(caCmd,"rm -Rf %s/%s",caFile,filedir);
					  system(caCmd);
				   }else{
					  ltMsgPk->msgpktype=1;
					  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error! file isn't exist!</body></html>");            
				   }
			   }else{
				  ltMsgPk->msgpktype=1;
				  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"#cccccc\" >Error! file isn't exist!</body></html>");            
			   }
		}
		ltMsgPk->msgpktype=1;
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
		ltMsgFree(ltMsgPk);
		return 0;	   
	 }
   while(len--){
      fileproc=0;
      memset(reportname,0,sizeof(reportname));
      memset(cDate,0,sizeof(cDate));                                                             
      sprintf(caFile,"/app/updateservice/%s/info.ini",fnames[len].caDirName);
      bcCnfGetValue_s(caFile,"date",cDate);
      bcCnfGetValue_s(caFile,"name",reportname);
      bcCnfGetValue_s(caFile,"desc",leveldesc);
      
      if(k==0){
	       sprintf(caFile,"/app/updateservice/%s/proccess.ini",fnames[len].caDirName);
	       if(stat(caFile,&st) == 0){
	              fileproc=1;
	       }       
	       sprintf(caFile,"/app/updateservice/%s/package.tgz",fnames[len].caDirName);	       
	       if(stat(caFile,&st) == 0){
	       	memset(filleSize,0,sizeof(filleSize));
	       	    if(st.st_size>1024000){
	       	      sprintf(filleSize,"%0.3fM",st.st_size/1024000.00);
	       	    }else{
	       	      sprintf(filleSize,"%0.3fK",st.st_size/1024.00);
	       	    }
				      sprintf(caTmpp,"{\"packagename\":\"%s\",\"reportname\":\"%s\",\"reportstate\":\"%s\",\"size\":\"%s\",\"leveldesc\":\"%s\"}",fnames[len].caDirName,reportname,fileproc==1?"making":"successfully",filleSize,leveldesc);
		   }
		   stralloc_cats(&strTemp,caTmpp);
      }else{
	       sprintf(caFile,"/app/updateservice/%s/proccess.ini",fnames[len].caDirName);
	       if(stat(caFile,&st) == 0){
	                fileproc=1;
	       }       
	       sprintf(caFile,"/app/updateservice/%s/package.tgz",fnames[len].caDirName);
	       if(stat(caFile,&st) == 0){
	       memset(filleSize,0,sizeof(filleSize));
	       	    if(st.st_size>1024000){
	       	      sprintf(filleSize,"%0.3fM",st.st_size/1024000.00);
	       	    }else{
	       	      sprintf(filleSize,"%0.3fK",st.st_size/1024.00);
	       	    }
	       	    sprintf(caTmpp,",{\"packagename\":\"%s\",\"reportname\":\"%s\",\"reportstate\":\"%s\",\"size\":\"%s\",\"leveldesc\":\"%s\"}",fnames[len].caDirName,reportname,fileproc==1?"making":"successfully",filleSize,leveldesc);
	       }	
	       stralloc_cats(&strTemp,caTmpp);
      }
      k++;
   }
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	 ltMsgFree(ltMsgPk);
	 return 0;
}
void resetcatch_zbalarm(int signum){
   _exit(0);
}
//升级包生成
int naspackageok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  char   caCmd[512];
  char   *levelnum;
  char   *leveldesc;
  //char   *levelfile;
  int    haveip;
  char   caTempDir[512];
  char   caBack[256];
  char   sDate[36];
  char   sTime[36];
printf("test1................\n");  
  haveip=0;
  levelnum=ltMsgGetVar_s(ltMsgPk,"levelnum");
  leveldesc=ltMsgGetVar_s(ltMsgPk,"leveldesc");
  //levelfile=ltMsgGetVar_s(ltMsgPk,"levelfile"); 
printf("test2................\n");
  
  nasCvtStime(time(0),sDate,sTime);
  sprintf(caTempDir,"/app/updateservice/%s/",levelnum);///app/updateservice/    ltStrGetId()
  if(ltFileIsExist(caTempDir)!=1) {
    if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0){
        /// 生成目录错误
        return 0;
    }
  }
printf("test3................\n");
printf("caTempDir:%s................\n",caTempDir);
  sprintf(caBack,"echo 'date=%s-%s' > %sinfo.ini",sDate,sTime,caTempDir);
  system(caBack);//建立目录标志
  sprintf(caBack,"echo 'name=%s' >> %sinfo.ini",levelnum,caTempDir);
  system(caBack);//建立目录标志
  sprintf(caBack,"echo 'desc=%s' >> %sinfo.ini",leveldesc,caTempDir);
  system(caBack);//建立目录标志
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body>Success</body></html>");    
  sprintf(caBack,"echo '1' > %sproccess.ini",caTempDir);
  system(caBack);//建立目录正在处理标志 
printf("test4................\n");
  sprintf(caCmd,"rm  %sproccess.ini",caTempDir);
  system(caCmd);
  
  chdir(caTempDir);
	system("tar -cvzf package.tgz *");
	
  ltMsgFree(ltMsgPk); 
  return 0;
}
int ltupsysok(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
  char   caCmd[512];
  char   caTempDir[512];
  //char   caBack[256];
  char   version[32];
  char   mylevelnum[32]; 
  
  int len=0,jjj=0; //sum of file names 
  msasDir *fnames;
  char caFile[256];
  char str[32];
  
  bcCnfGetValue_s("/etc/msa/msa/system/version","version",version);

printf("version:%d\n",atoi(version));
	sprintf(caFile,"%s","/app/updateservice");
printf("caFile:%s\n",caFile);
  fnames=mydirlist(caFile,&len); 
  while(len--){
  	if(atoi(fnames[len].caDirName)>atoi(version)){
  		sprintf(mylevelnum,"%s",fnames[len].caDirName);
  		
		  sprintf(caTempDir,"/app/msa/update/%s/",mylevelnum);///app/msa/update    ltStrGetId()
		  if(ltFileIsExist(caTempDir)!=1){
		    if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0){
		        /// 生成目录错误
		        return 0;
		    }
		  }
		  
		  //拷贝文件  循环拷贝
		  memset(caCmd,0,sizeof(caCmd));
			sprintf(caCmd,"/bin/cp -r /app/updateservice/%s/* %s/",mylevelnum,caTempDir);
		  system(caCmd);
		  memset(caCmd,0,sizeof(caCmd));
		  sprintf(caCmd,"echo version='%s' > %s",mylevelnum,"/etc/msa/msa/system/version");
  		system(caCmd);
		  
		  jjj++;
  	}
	}
	if(jjj==0){
	  ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"2"); //无新版本   
	  ltMsgFree(ltMsgPk); 
	  return 0;
	}
  ltMsgPk->msgpktype=1;
  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"0");    
  ltMsgFree(ltMsgPk); 
  return 0;
}
