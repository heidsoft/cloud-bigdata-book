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
#include <sys/sem.h>
#include "msas.h"


int msasShowXTZY(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    
    char  sysver[24];
    int   cpuinfo;
    int   rootdiskinfo;
    int   appdiskinfo;
    int   onlinehost;
    int   onlineuser;
    int   onlinep2p;
    int   onelinelive;
    int   onlinegame;
    int   onlinestock;
    int   allmem;
    int   freemem;
    int   mempp;
    unsigned long long bytenum;
    unsigned long long bytemax;
    unsigned long byteavg;
    char     strbytenum[32];
    char     strbytemax[32];
    char     strbyteavg[32];
    int      allhostnum;
    
    char  allAlertNum[32];
		char  rAlertNum[32];
		char  iAlertNum[32];
		char  kAlertNum[32];
		unsigned int lTime;
		char  sDate[32];
		char  infoFile[256];
    
    char  caPlt[2056];
    char  caPltutf8[2056];
    static struct sembuf lock={0,-1,SEM_UNDO};
	  static struct sembuf unlock={0,1,SEM_UNDO|IPC_NOWAIT};
    
    mempp=20;
    memset(sysver,0,sizeof(sysver));
    sprintf(sysver,"%s",bcCnfGetValueS(pubconfdir,"kernel","version"));
    cpuinfo=0;
    {
									  	jiffy_counts_t jif, prev_jif;
									    FILE* fp;
									    sleep(1);
									    fp = fopen("/proc/stat", "r");
											fscanf(fp, "cpu  %lld %lld %lld %lld %lld %lld %lld %lld",
													&prev_jif.usr,&prev_jif.nic,&prev_jif.sys,&prev_jif.idle,
													&prev_jif.iowait,&prev_jif.irq,&prev_jif.softirq,&prev_jif.steal);
											fclose(fp);
											prev_jif.total = prev_jif.usr + prev_jif.nic + prev_jif.sys + prev_jif.idle
													+ prev_jif.iowait + prev_jif.irq + prev_jif.softirq + prev_jif.steal;
											/* procps 2.x does not count iowait as busy time */
											prev_jif.busy = prev_jif.total - prev_jif.idle - prev_jif.iowait;
											
											sleep(3);
											fp = fopen("/proc/stat", "r");
											fscanf(fp, "cpu  %lld %lld %lld %lld %lld %lld %lld %lld",
													&jif.usr,&jif.nic,&jif.sys,&jif.idle,
													&jif.iowait,&jif.irq,&jif.softirq,&jif.steal) ;
											fclose(fp);
											jif.total = jif.usr + jif.nic + jif.sys + jif.idle
													+ jif.iowait + jif.irq + jif.softirq + jif.steal;
											/* procps 2.x does not count iowait as busy time */
											jif.busy = jif.total - jif.idle - jif.iowait;
									
											cpuinfo=(100*(jif.busy-prev_jif.busy))/(jif.total-prev_jif.total)+1;
											if(cpuinfo>20){
												cpuinfo=cpuinfo/1.5;
											}
			}	
		
			rootdiskinfo=0;
			appdiskinfo=0;
			{
				    FILE *fp;
				    char *p;
				    char caBuffer[132];
				    char caDev[32];
				    char caRate[32];
				    char caMount[32];
				    unsigned long long lTotal,lUsed,lFree;
				    fp = popen("/sbin/df -k","r");
				    if(fp) {
				        p = fgets(caBuffer,131,fp);
				        p = fgets(caBuffer,131,fp);
				        while(p) {
				        	  //printf("p:%s\n",p);
				            sscanf(caBuffer,"%s %lld %lld %lld %s %s",
				                caDev,&lTotal,&lUsed,&lFree,caRate,caMount);
				            if(strcmp(caMount,"/")==0) {
				            	  rootdiskinfo=(lUsed*100)/lTotal;
				            }
				            if(strcmp(caMount,"/app")==0) {
												appdiskinfo=(lUsed*100)/lTotal;
												break;
				            }
				            p = fgets(caBuffer,132,fp);
				        }
				        pclose(fp);
				  }
				
			}
		
			{
				 FILE *fp;
				 char *ppppp;
				 
				 int  iii;
				 int  jjj;
				 char *p;
				 char caBuffer[256];
				 freemem=0;
				 allmem=0;
				 unsigned long long xxxmem;
				 
				 system("cat /proc/meminfo > /app/ns/bin/mymem");
         fp=fopen("/app/ns/bin/mymem","r");
         memset(caBuffer,0,256);
         p=fgets(caBuffer,255,fp);
			   if(p) {
			   		if(strncmp(caBuffer,"MemTotal:",9)==0 ){
			   			ppppp=caBuffer;
			   			iii=strlen(ppppp);
			   			for(jjj=0;jjj<iii;jjj++){
			   				if(caBuffer[jjj]!=' ' && caBuffer[jjj]!='k' && caBuffer[jjj]!='B' ){
			   			  	*ppppp=caBuffer[jjj];
			   			  	ppppp++;
			   			  }
			   		  }
			   		  *ppppp='\0';
			   		  ppppp=caBuffer;
			   		  ppppp=ppppp+9;
			   		  allmem=atol(ppppp)/1000;
			   		}
			   }
			    memset(caBuffer,0,256);
         p=fgets(caBuffer,255,fp);
			   if(p) {
			   		if(strncmp(caBuffer,"MemFree:",8)==0 ){
			   			ppppp=caBuffer;
			   			iii=strlen(ppppp);
			   			for(jjj=0;jjj<iii;jjj++){
			   				if(caBuffer[jjj]!=' ' && caBuffer[jjj]!='k' && caBuffer[jjj]!='B' ){
			   			  	*ppppp=caBuffer[jjj];
			   			  	ppppp++;
			   			  }
			   		  }
			   		  *ppppp='\0';
			   		  ppppp=caBuffer;
			   		  ppppp=ppppp+8;
			   		  freemem=atol(ppppp)/1000;
			   		}
			   }
			   xxxmem=(allmem-freemem)*100;
				 mempp=xxxmem/allmem;
				 if(mempp<0 ){
				   mempp=20;	
				 }
				 if(mempp>85 ){
				   mempp=85;	
				 }
			}
				
			onlinehost=0;
			semop(_ltPubInfo->_onlinehosthead.locksemid,&lock,1);
			onlinehost=_ltPubInfo->_onlinehosthead.nownum;
			semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
			onlineuser=0;
      onlinep2p=0;
      onelinelive=0;
      onlinegame=0;
      onlinestock=0;
      bytenum=(unsigned long long)0;
      bytemax=(unsigned long long)0;
      
      
      {
						char *p;
					  int  iii;
					  int indexWalker;
					  shm_hash_link_onlinehostindex *_gnm_hash_index;
						shm_hash_link_onlinehost      *_gnm_hash_content;
						shm_hash_link_onlinehost *walker;
						msasOnlineHost  *nasHashItemInfo;

					  p=(char *)_ltPubInfo;
						_gnm_hash_index=(shm_hash_link_onlinehostindex *)(p+sizeof(msasPubInfo));
					  _gnm_hash_content=(shm_hash_link_onlinehost *)(p+sizeof(msasPubInfo)+_ltPubInfo->_onlinehosthead.conbufptr);
					    
					  for(iii=0;iii<_ltPubInfo->_onlinehosthead.bufnum;iii++){
					  	 semop(_ltPubInfo->_onlinehosthead.locksemid,&lock,1); 
					   	 indexWalker=_gnm_hash_index[iii].next;
					   	 if(indexWalker==-1){
					   	 	semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
					   	 	continue;
					   	 }
					   	 semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
							 walker = &_gnm_hash_content[indexWalker];
							 for (; ; ) {
										  nasHashItemInfo=(msasOnlineHost *)(&walker->item);
										  onlineuser=onlineuser+nasHashItemInfo->usernum;
										  onlinep2p=onlinep2p+nasHashItemInfo->p2pnum;
                      onelinelive=onelinelive+nasHashItemInfo->livernum;
                      onlinegame=onlinegame+nasHashItemInfo->gamenum;
                      onlinestock=onlinestock+nasHashItemInfo->soketnum;	                     
                      bytenum=(unsigned long long)(bytenum+(unsigned long long)nasHashItemInfo->bytenum);	
                      if(nasHashItemInfo->bytenum>bytemax){
												bytemax=nasHashItemInfo->bytenum;
											}										
											semop(_ltPubInfo->_onlinehosthead.locksemid,&lock,1); 
											if(walker->next == -1){
												semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
												break;
											}
											indexWalker = walker->next;
											semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
											walker = &_gnm_hash_content[indexWalker];
							 }
					  }
			}
      if(onlinehost==0){			
				byteavg=bytenum/1;
			}else{
				byteavg=bytenum/onlinehost;
			}
			bytenum=bytenum*8;
			byteavg=byteavg*8;
			bytemax=bytemax*8;
			memset(strbytenum,0,sizeof(strbytenum));
			memset(strbytemax,0,sizeof(strbytemax));
			memset(strbyteavg,0,sizeof(strbyteavg));
			if(bytenum>1024000000){
				sprintf(strbytenum,"%0.3fG",bytenum/1024000000.000);
			}else if(bytenum>1024000){
				sprintf(strbytenum,"%0.2fM",bytenum/1024000.00);
			}else if(bytenum>1024){
				sprintf(strbytenum,"%0.2fK",bytenum/1024.00);
			}else{
			 	sprintf(strbytenum,"%lld",bytenum);
		  }
		  
		  if(bytemax>1024000000){
				sprintf(strbytemax,"%0.3fG",bytemax/1024000000.000);
			}else if(bytemax>1024000){
				sprintf(strbytemax,"%0.2fM",bytemax/1024000.00);
			}else if(bytemax>1024){
				sprintf(strbytemax,"%0.2fK",bytemax/1024.00);
			}else{
			 	sprintf(strbytemax,"%lld",bytemax);
		  }
		  
		  if(byteavg>1024000000){
				sprintf(strbyteavg,"%0.3fG",byteavg/1024000000.000);
			}else if(byteavg>1024000){
				sprintf(strbyteavg,"%0.2fM",byteavg/1024000.00);
			}else if(byteavg>1024){
				sprintf(strbyteavg,"%0.2fK",byteavg/1024.00);
			}else{
			 	sprintf(strbyteavg,"%ld",byteavg);
		  }
	  
		  {
		  	 ltDbCursor *tempCursor;
		     LT_DBROW tempRow;
		     char sqlBuf[1024];
		     allhostnum=0;
		  	 sprintf(sqlBuf,"select count(*) from  msasclientinfo");
			   tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
			   tempRow= ltDbFetchRow(tempCursor);
			   if(tempRow!=NULL){
			       allhostnum=atol(tempRow[0]);
			    }
			    ltDbCloseCursor(tempCursor);
		  }
		  if(allhostnum==0){
		    allhostnum=1;
		  }
		  
		  lTime = time(0);
      memset(sDate,0,sizeof(sDate));
		  nasTimeGetDate(sDate,lTime);
		  memset(allAlertNum,0,sizeof(allAlertNum));
		  memset(rAlertNum,0,sizeof(rAlertNum));
		  memset(iAlertNum,0,sizeof(iAlertNum));
		  memset(kAlertNum,0,sizeof(kAlertNum));
		  
//		  if(ltFileIsExist(infoFile)!=1) {
//		  	sprintf(allAlertNum,"%s","0");
//		  	sprintf(rAlertNum,"%s","0");
//		  	sprintf(iAlertNum,"%s","0");
//		  	sprintf(kAlertNum,"%s","0");
//		  }else{
//			  bcCnfGetValue_s(infoFile,"allcount",allAlertNum);
//			  bcCnfGetValue_s(infoFile,"rnum",rAlertNum);
//	      bcCnfGetValue_s(infoFile,"inum",iAlertNum);
//	      bcCnfGetValue_s(infoFile,"knum",kAlertNum);
//			}
			
			{
		  	 ltDbCursor *tempCursor;
		     LT_DBROW tempRow;
		     char sqlBuf[1024];
		  	 sprintf(sqlBuf,"select count(*) from  AlertList");
			   tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
			   tempRow= ltDbFetchRow(tempCursor);
			   if(tempRow!=NULL){
			       sprintf(allAlertNum,"%s",tempRow[0]);
			   }
			   ltDbCloseCursor(tempCursor);
			    
			   
			   struct tm *tm1;
         tm1 = localtime(&lTime);
         sprintf(sDate,"%04d%02d%02d",tm1->tm_year + 1900,tm1->tm_mon+1,tm1->tm_mday);
     
			   sprintf(sqlBuf,"select count(*) from  AlertList where substring(Matching_time,1,8)='%s'",sDate);
			   tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
			   tempRow= ltDbFetchRow(tempCursor);
			   if(tempRow!=NULL){
			       sprintf(rAlertNum,"%s",tempRow[0]);
			   }
			   ltDbCloseCursor(tempCursor); 
			    
		  }

			  	
      memset(caPlt,0,sizeof(caPlt));
      sprintf(caPlt,"[['系统版本','%s','版本号'],\
['系统状态','cpu:<font color=#00dd00><b>%d\x25</b></font>  memory:<font color=#00dd00><b>%d\x25</b></font> 系统空间利用率:<font color=#00dd00><b>%d\x25</b></font> 应用空间利用率:<font color=#00dd00><b>%d\x25</b></font>',' '],\
['联网单位','在线联网单位<b>%d</b> 联网单位:<b>%d</b> 在线率:<b>%d\x25</b>',' '],\
['当日报警','当日报警总数:<font color=#dd0000><b>%s</b></font>  全部报警总数:<font color=#dd0000><b>%s</b></font>',' '],\
['当前人员','总人员:<b>%d</b> P2P使用人员:<b>%d</b> 网络电视人员:<b>%d</b> 使用游戏人员:<b>%d</b> 股票软件人员:<b>%d</b>','个'],\
['出口带宽','平均出口带宽:<b>%s</b> 总出口带宽:<b>%s</b> 最高出口带宽:<b>%s</b>','单位:bits']]",
sysver,cpuinfo,mempp,rootdiskinfo,appdiskinfo,
onlinehost,allhostnum,onlinehost*100/allhostnum,
rAlertNum,allAlertNum,
onlineuser,onlinep2p,
onelinelive,onlinegame,onlinestock,
strbyteavg,strbytenum,strbytemax);


    memset(caPltutf8,0,sizeof(caPltutf8));
    strgb2utf8(caPlt,caPltutf8,strlen(caPlt));
  
    ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,caPltutf8);
    ltMsgFree(ltMsgPk);
    
    return 0;
}



int msasZaixianInfo(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{

    stralloc strTemp;
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char sqlBuf[1024];
    int  zaixiantype;
    
  	strTemp.s=0;
    struct msasConCount_s {
        char          caItem[12];
        unsigned int  num;
    } sCount[33];
    
    unsigned int lMaxNum;
    int      i;
    int      j,k;

    char caTemp[512];
    memset((char *)&sCount,0,33*sizeof(struct msasConCount_s));
    
    zaixiantype=atol(ltMsgGetVar_s(ltMsgPk,"zaixiantype"));
    tempCursor=NULL;
    lMaxNum = 1;
    i=0;
    if(zaixiantype==1 || zaixiantype==2){
			    sprintf(sqlBuf,"select sdate,stime,onlinehost,usernum from msassysinfolog  order by rtime desc limit 31 ");
			    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
			    tempRow= ltDbFetchRow(tempCursor);
			    while(tempRow!=NULL){
			        sprintf(sCount[i].caItem,"%s",tempRow[1]);
			        if(zaixiantype==1){
			        	sCount[i].num = atol(tempRow[2]);
			        }else{
			          sCount[i].num = atol(tempRow[3]);
			        }
			        if(lMaxNum < sCount[i].num ) {
			           lMaxNum = sCount[i].num ;
			        }
			        i++;
			        tempRow= ltDbFetchRow(tempCursor);
			    }
			    ltDbCloseCursor(tempCursor);
			   
			    /* 结果输出  */ 
			  	sprintf(caTemp,"{\"totalCount\":\"%d\",\"Results\":[",i);
			  	stralloc_cats(&strTemp,caTemp);	
			    k=0;
			    for(j=0;j<i  ;j++){
			     	  if(k==0){
			     	  	 sprintf(caTemp,"{\"date\":\"%s\",\"Value\":\"%d\"}",sCount[j].caItem,sCount[j].num);
			     	     stralloc_cats(&strTemp,caTemp);	
			     	  }else{
			     	  	 sprintf(caTemp,",{\"date\":\"%s\",\"Value\":\"%d\"}",sCount[j].caItem,sCount[j].num);
			     	     stralloc_cats(&strTemp,caTemp);
			     	  }
			     	  k++;
			    }
			  stralloc_cats(&strTemp,"]}");
			  stralloc_0(&strTemp);
  }else if(zaixiantype==3){
  	      sprintf(caTemp,"{\"totalCount\":\"%d\",\"Results\":[",31);
			  	stralloc_cats(&strTemp,caTemp);	
			  	k=0;
  	      sprintf(sqlBuf,"select sdate,stime,onlinehost,usernum,p2pnum,livernum,gamenum,soketnum from msassysinfolog  order by rtime desc limit 31 ");
			    tempCursor = ltDbOpenCursor(G_DbCon,sqlBuf);
			    tempRow= ltDbFetchRow(tempCursor);
			    while(tempRow!=NULL){
			        if(k==0){
			     	  	 sprintf(caTemp,"{\"date\":\"%s Hour:%s\",\"host\":\"%s\",\"Value\":\"%s\",\"Valuep\":\"%s\",\"Valuet\":\"%s\",\"Valueg\":\"%s\",\"Values\":\"%s\"}",
			     	  	 tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7]);
			     	     stralloc_cats(&strTemp,caTemp);	
			     	  }else{
			     	  	  sprintf(caTemp,",{\"date\":\"%s Hour:%s\",\"host\":\"%s\",\"Value\":\"%s\",\"Valuep\":\"%s\",\"Valuet\":\"%s\",\"Valueg\":\"%s\",\"Values\":\"%s\"}",
			     	  	  tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7]);
			     	     stralloc_cats(&strTemp,caTemp);	
			     	  }
			     	  k++;
			        tempRow= ltDbFetchRow(tempCursor);
			    }
			    ltDbCloseCursor(tempCursor);
			    stralloc_cats(&strTemp,"]}");
			    stralloc_0(&strTemp);	
  }
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s); 
	ltMsgFree(ltMsgPk);
	return 0;
		
}

/*在线人员*/
static int nasSortBufBy1(const void *p1,const void *p2)
{
    msasOnlineHostS *ps1,*ps2;
    ps1 = (msasOnlineHostS *)p1;
    ps2 = (msasOnlineHostS *)p2;    
    return ps2->usernum-ps1->usernum;
}
/*名称*/
static int nasSortBufBy2(const void *p1,const void *p2)
{
    msasOnlineHostS *ps1,*ps2;
    ps1 = (msasOnlineHostS *)p1;
    ps2 = (msasOnlineHostS *)p2;    
    return strcmp(ps2->caName ,ps1->caName);
}
/*tcp*/
static int nasSortBufBy3(const void *p1,const void *p2)
{
    msasOnlineHostS *ps1,*ps2;
    ps1 = (msasOnlineHostS *)p1;
    ps2 = (msasOnlineHostS *)p2;    
    return ps2->tcpnum-ps1->tcpnum;
}
/*udp*/
static int nasSortBufBy4(const void *p1,const void *p2)
{
    msasOnlineHostS *ps1,*ps2;
    ps1 = (msasOnlineHostS *)p1;
    ps2 = (msasOnlineHostS *)p2;    
    return ps2->udpnum-ps1->udpnum;
}
/*出口速率排序*/
static int nasSortBufBy5(const void *p1,const void *p2)
{
    msasOnlineHostS *ps1,*ps2;
    ps1 = (msasOnlineHostS *)p1;
    ps2 = (msasOnlineHostS *)p2;    
    return ps2->bytenum-ps1->bytenum;
}
/*CPU利用率排序*/
static int nasSortBufBy6(const void *p1,const void *p2)
{
    msasOnlineHostS *ps1,*ps2;
    ps1 = (msasOnlineHostS *)p1;
    ps2 = (msasOnlineHostS *)p2;    
    return ps2->cpunum-ps1->cpunum;
}
/*硬盘利用率排序*/
static int nasSortBufBy7(const void *p1,const void *p2)
{
    msasOnlineHostS *ps1,*ps2;
    ps1 = (msasOnlineHostS *)p1;
    ps2 = (msasOnlineHostS *)p2;    
    return ps2->appdisknum-ps1->appdisknum;
}


int msasHostList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	char caTmpp[2048];
	msasOnlineHostS  *pOnlineHost;
	char *caSear;
	int  orderby;
	int  lRowNum,lStartRec,lCount;
  int  i,k;
  int  iIndex;
  stralloc strTemp;
  int  grouplevel,groupid;
  char strByteNum[32];
  char strByteNumD[32];
  char strByteNumU[32];
  static struct sembuf lock={0,-1,SEM_UNDO};
	static struct sembuf unlock={0,1,SEM_UNDO|IPC_NOWAIT};
	char ip[32],ipmask[32],caFile[256];
	unsigned int vpnips,vpnipmask;
	int  isvpn;
	
	
	sprintf(caFile,"/etc/msa/msas/vpnserver/settings"); 
  memset(ip,0,sizeof(ip));
  memset(ipmask,0,sizeof(ipmask));

  bcCnfGetValue_s(caFile,"ip",ip);  
  bcCnfGetValue_s(caFile,"ipmask",ipmask);
  //vpnips=ntohl(inet_addr(caIp));
  vpnips=ntohl(inet_addr(ip));
  vpnipmask=ntohl(inet_addr(ipmask));
	
	grouplevel=0;
	groupid=-1;
	
	if(ltMsgGetVar_s(ltMsgPk,"groupid")!=NULL){
		groupid=atol(ltMsgGetVar_s(ltMsgPk,"groupid"));
	}
	if(ltMsgGetVar_s(ltMsgPk,"grouplevel")!=NULL){
		grouplevel=atol(ltMsgGetVar_s(ltMsgPk,"grouplevel"));
	}
	if(grouplevel>=MSAS_MAX_GROUPLEVEL){
		grouplevel=0;
	}
	  
	caSear=ltMsgGetVar_s(ltMsgPk,"cond");
	if(!caSear){
		caSear="";
  }
  orderby=0;
	if(ltMsgGetVar_s(ltMsgPk,"orderby")!=NULL){
		orderby=atol(ltMsgGetVar_s(ltMsgPk,"orderby"));
	}
  lRowNum=50; /*每页的行数*/
  lCount=0; 
  lStartRec=0;
  if(ltMsgGetVar_s(ltMsgPk,"limit")){
    		lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
  }
  if(ltMsgGetVar_s(ltMsgPk,"start")){
    		lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
  }
	

  semop(_ltPubInfo->_onlinehosthead.locksemid,&lock,1);
	lCount=_ltPubInfo->_onlinehosthead.nownum;
  semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
  
  strTemp.s=0;
  
  if(lCount==0){
  	sprintf(caTmpp,"%s","{\"totalCount\":\"0\",\"Results\":[]}");
  	stralloc_cats(&strTemp,caTmpp);
  	ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	  ltMsgFree(ltMsgPk);
	  return 0;
  }
   
  sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",lCount);
  stralloc_cats(&strTemp,caTmpp);
  
  pOnlineHost=(msasOnlineHostS  *)malloc(sizeof(msasOnlineHostS)*(lCount+1));
  if(!pOnlineHost){
  	sprintf(caTmpp,"%s","{\"totalCount\":\"0\",\"Results\":[]}");
  	ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	  ltMsgFree(ltMsgPk);
	  return 0;
  }
  
  if(pOnlineHost){
  	memset(pOnlineHost,0,sizeof(msasOnlineHostS)*(lCount+1));
  }
	iIndex=0;
  /*拷贝得到列表*/
  {
						char *p;
					  int  iii;
					  int indexWalker;
					  shm_hash_link_onlinehostindex *_gnm_hash_index;
						shm_hash_link_onlinehost      *_gnm_hash_content;
						shm_hash_link_onlinehost *walker;
						msasOnlineHost  *nasHashItemInfo;

					  p=(char *)_ltPubInfo;
						_gnm_hash_index=(shm_hash_link_onlinehostindex *)(p+sizeof(msasPubInfo));
					  _gnm_hash_content=(shm_hash_link_onlinehost *)(p+sizeof(msasPubInfo)+_ltPubInfo->_onlinehosthead.conbufptr);
					    
					  for(iii=0;iii<_ltPubInfo->_onlinehosthead.bufnum;iii++){
					  	 semop(_ltPubInfo->_onlinehosthead.locksemid,&lock,1); 
					   	 indexWalker=_gnm_hash_index[iii].next;
					   	 if(indexWalker==-1){
					   	 	semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
					   	 	continue;
					   	 }
					   	 semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
							 walker = &_gnm_hash_content[indexWalker];
							 for (; ; ) {
										  nasHashItemInfo=&walker->item;
										  if( groupid==-1 ) {/*符合条件*/
										    if( ltStrIsSpaces(caSear) || strstr(nasHashItemInfo->caName,caSear) ) {
													memcpy(&pOnlineHost[iIndex],nasHashItemInfo,sizeof(msasOnlineHostS));
													iIndex++;
													if(iIndex>=lCount){
														break;
												  }
												}
										  }else	{
										  	if( (ltStrIsSpaces(caSear) || strstr(nasHashItemInfo->caName,caSear))
										    && nasHashItemInfo->levelgroup[grouplevel]==groupid ) {/*符合条件*/
													memcpy(&pOnlineHost[iIndex],nasHashItemInfo,sizeof(msasOnlineHostS));
													iIndex++;
													if(iIndex>=lCount){
														break;
												  }
										    }				
										  }							
											semop(_ltPubInfo->_onlinehosthead.locksemid,&lock,1); 
											if(walker->next == -1){
												semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
												break;
											}
											indexWalker = walker->next;
											semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
											walker = &_gnm_hash_content[indexWalker];
							 }
					  }
	}
  	
  if(orderby==1){
  	qsort(pOnlineHost,iIndex,sizeof(msasOnlineHostS),nasSortBufBy1);
	}else if(orderby==2){
  	qsort(pOnlineHost,iIndex,sizeof(msasOnlineHostS),nasSortBufBy2);
	}else if(orderby==3){
  	qsort(pOnlineHost,iIndex,sizeof(msasOnlineHostS),nasSortBufBy3);
	}else if(orderby==4){
  	qsort(pOnlineHost,iIndex,sizeof(msasOnlineHostS),nasSortBufBy4);
	}else if(orderby==5){
  	qsort(pOnlineHost,iIndex,sizeof(msasOnlineHostS),nasSortBufBy5);
	}else if(orderby==6){
  	qsort(pOnlineHost,iIndex,sizeof(msasOnlineHostS),nasSortBufBy6);
	}else if(orderby==7){
  	qsort(pOnlineHost,iIndex,sizeof(msasOnlineHostS),nasSortBufBy7);
	}
 
	i=0;
	strTemp.s=0;
  sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",iIndex);
  stralloc_cats(&strTemp,caTmpp);
  k=0;
	for(i=lStartRec;i<iIndex;i++){
		struct in_addr s;
		s.s_addr = htonl(pOnlineHost[i].ipadd);
		
    memset(strByteNum,0,sizeof(strByteNum));
    memset(strByteNumD,0,sizeof(strByteNumD));
    memset(strByteNumU,0,sizeof(strByteNumU));
    if(pOnlineHost[i].bytenum>1024000){
				sprintf(strByteNum,"%0.3fG",pOnlineHost[i].bytenum/1024000.000);
	  }else if(pOnlineHost[i].bytenum>1024){
				sprintf(strByteNum,"%0.2fM",pOnlineHost[i].bytenum/1024.00);
	  }else{
			 	sprintf(strByteNum,"%ldK",pOnlineHost[i].bytenum);
		}
		
		if(pOnlineHost[i].bytenumu>1024000){
				sprintf(strByteNumU,"%0.3fG",pOnlineHost[i].bytenumu/1024000.000);
	  }else if(pOnlineHost[i].bytenumu>1024){
				sprintf(strByteNumU,"%0.2fM",pOnlineHost[i].bytenumu/1024.00);
	  }else{
			 	sprintf(strByteNumU,"%ldK",pOnlineHost[i].bytenumu);
		}
		
		if(pOnlineHost[i].bytenumd>1024000){
				sprintf(strByteNumD,"%0.3fG",pOnlineHost[i].bytenumd/1024000.000);
	  }else if(pOnlineHost[i].bytenumd>1024){
				sprintf(strByteNumD,"%0.2fM",pOnlineHost[i].bytenumd/1024.00);
	  }else{
			 	sprintf(strByteNumD,"%ldK",pOnlineHost[i].bytenumd);
		}
    isvpn=0;
    if( (vpnips & vpnipmask) == (pOnlineHost[i].ipadd & vpnipmask) ){
    	isvpn=1;
    }
    
		if(k==0){
	     sprintf(caTmpp,"{\"name\":\"%s\",\"syskey\":\"%s\",\"sysversion\":\"%d\",\"cpup\":\"%d\",\"diskp\":\"%d%c\",\"Value\":\"%d\",\"tcpnum\":\"%d\",\"udpnum\":\"%d\",\"flowinfo\":\"%s\",\"upflowinfo\":\"%s\",\"downflowinfo\":\"%s\",\"isvpn\":\"%d\",\"ipadd\":\"%s\"}",
	     pOnlineHost[i].caName,pOnlineHost[i].caKey,pOnlineHost[i].sysversion,pOnlineHost[i].cpunum,
	     pOnlineHost[i].appdisknum,'%',pOnlineHost[i].usernum,pOnlineHost[i].tcpnum,pOnlineHost[i].udpnum,strByteNum,strByteNumU,strByteNumD,isvpn,inet_ntoa(s));
	     stralloc_cats(&strTemp,caTmpp);	
	  }else{
	     sprintf(caTmpp,",{\"name\":\"%s\",\"syskey\":\"%s\",\"sysversion\":\"%d\",\"cpup\":\"%d\",\"diskp\":\"%d%c\",\"Value\":\"%d\",\"tcpnum\":\"%d\",\"udpnum\":\"%d\",\"flowinfo\":\"%s\",\"upflowinfo\":\"%s\",\"downflowinfo\":\"%s\",\"isvpn\":\"%d\",\"ipadd\":\"%s\"}",
	     pOnlineHost[i].caName,pOnlineHost[i].caKey,pOnlineHost[i].sysversion,pOnlineHost[i].cpunum,
	     pOnlineHost[i].appdisknum,'%',pOnlineHost[i].usernum,pOnlineHost[i].tcpnum,pOnlineHost[i].udpnum,strByteNum,strByteNumU,strByteNumD,isvpn,inet_ntoa(s));
	     stralloc_cats(&strTemp,caTmpp);
	  }			
		k++;
		if(k>=lRowNum){
			break;
		}
	}
	stralloc_cats(&strTemp,"]}");
	stralloc_0(&strTemp);

  free(pOnlineHost);
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
    
	ltMsgFree(ltMsgPk);
	return 0;
	
}


int msasShowClientSrv(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{

	ltDbHeadPtr dbPtr;
	
	dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
	
	lt_dbput_rootvars(dbPtr,1,"ipadd",ltMsgGetVar_s(ltMsgPk,"ipadd"));
  
	ltMsgPk->msgpktype=1;
  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/nowview/clientservicebar.htm",dbPtr->head,0);
  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);
  return 0;
		
}

int msasSrvInfo(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{

    stralloc strTemp;
    int      icount;
    int      i,k;
    char     caTemp[512];
    char     *ipadd;
    
    strTemp.s=0;
    icount=0;
    
    ipadd=ltMsgGetVar_s(ltMsgPk,"ipadd");
    if(!ipadd){
			    for(i=0;i<NAS_MAX_SYSSRVNUM;i++){
			    	if(_ltPubInfo->syssrvflow[i]>0){
			    		icount++;
			      }
			    }
				  /* 结果输出  */ 
				  sprintf(caTemp,"{\"totalCount\":\"%d\",\"Results\":[",icount);
					stralloc_cats(&strTemp,caTemp);	
					k=0;
					for(i=0;i<NAS_MAX_SYSSRVNUM;i++){
			    	if(_ltPubInfo->syssrvflow[i]>0){
			    		if(k==0){
						  	 sprintf(caTemp,"{\"sid\":\"%d\",\"sname\":\"%s\",\"sflow\":\"%lld\"}",i,_ltPubInfo->topSrvName[i].srvname,_ltPubInfo->syssrvflow[i]);
						     stralloc_cats(&strTemp,caTemp);	
						  }else{
						  	sprintf(caTemp,",{\"sid\":\"%d\",\"sname\":\"%s\",\"sflow\":\"%lld\"}",i,_ltPubInfo->topSrvName[i].srvname,_ltPubInfo->syssrvflow[i]);
						     stralloc_cats(&strTemp,caTemp);	
						  }
						  k++;
			      }
			    }
		}else{
			unsigned int lIp;
			msasOnlineHost *tempHostInfo;
			
			lIp=ntohl(inet_addr(ipadd));
			tempHostInfo=msasHashLookUpHost(_ltPubInfo,lIp);
			if(tempHostInfo){
				 for(i=0;i<NAS_MAX_SYSSRVNUM;i++){
			    	if(tempHostInfo->srvflow[i]>0){
			    		icount++;
			      }
			    }
				  /* 结果输出  */ 
				  sprintf(caTemp,"{\"totalCount\":\"%d\",\"Results\":[",icount);
					stralloc_cats(&strTemp,caTemp);	
					k=0;
					for(i=0;i<NAS_MAX_SYSSRVNUM;i++){
			    	if(_ltPubInfo->syssrvflow[i]>0){
			    		if(k==0){
						  	 sprintf(caTemp,"{\"sid\":\"%d\",\"sname\":\"%s\",\"sflow\":\"%lld\"}",i,_ltPubInfo->topSrvName[i].srvname,tempHostInfo->srvflow[i]);
						     stralloc_cats(&strTemp,caTemp);	
						  }else{
						  	sprintf(caTemp,",{\"sid\":\"%d\",\"sname\":\"%s\",\"sflow\":\"%lld\"}",i,_ltPubInfo->topSrvName[i].srvname,tempHostInfo->srvflow[i]);
						     stralloc_cats(&strTemp,caTemp);	
						  }
						  k++;
			      }
			    }
		  }
		}
	  stralloc_cats(&strTemp,"]}");
		stralloc_0(&strTemp);
 
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s); 
	ltMsgFree(ltMsgPk);
	return 0;
		
}


int msasShowHostList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{

	ltDbHeadPtr dbPtr;
	
	dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
	
	if(ltMsgGetVar_s(ltMsgPk,"groupid")!=NULL){
		lt_dbput_rootvars(dbPtr,1,"groupid",ltMsgGetVar_s(ltMsgPk,"groupid"));
	}else{
	  lt_dbput_rootvars(dbPtr,1,"groupid","-1");
  }
	if(ltMsgGetVar_s(ltMsgPk,"grouplevel")!=NULL){
		lt_dbput_rootvars(dbPtr,1,"grouplevel",ltMsgGetVar_s(ltMsgPk,"grouplevel"));
	}else{
	  lt_dbput_rootvars(dbPtr,1,"grouplevel","0");
  }
	
	ltMsgPk->msgpktype=1;
  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/nowview/hostlistinfo.htm",dbPtr->head,0);
  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);
  return 0;
		
}

int msasGetAlertNow(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	int  iiii,jjjj;
	unsigned int alertListIndex;
  stralloc strTemp;
  char     caTemp[1024];  
  char     srvname[128];
  char     Rule_name[64];
  char     Net_ending_ip[32];
  char     Net_ending_mac[16];
  char     Rule_type[64]; 
  char     strBuf[1024]; 
  ltDbCursor *tempCursor;
  LT_DBROW tempRow;
    
  strTemp.s=0;
  sprintf(strBuf,"select Matching_id,a.gnmkey,sname,Rule_id,Rule_name,anjian_id,Matching_time,Net_ending_ip,Net_ending_code,Net_ending_mac,Destination_ip,Service_type,KeyWord1,KeyWord2,KeyWord3,Customer_name,Certificate_type,Certificate_code,Rule_type,contenturl,a.updateflag,b.service_name from AlertList a left join msasclientinfo b on a.gnmkey=b.service_code  order by matching_time desc limit 50");
	tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
     	sprintf(caTemp,"<a href='/msas/main.xp?Fun=msasShKeyList+type=%s'><b>%s</b>[%s]  %s:%s %s:%s %s:%s<br></a>\r\n",
			tempRow[11],tempRow[6],tempRow[2],_ltPubInfo->pubMsgStr[ALERTTYPENAME0+2].conStr,tempRow[4],
			_ltPubInfo->pubMsgStr[ALERTTYPENAME0+3].conStr,tempRow[7],_ltPubInfo->pubMsgStr[ALERTTYPENAME0+4].conStr,tempRow[9]);
			stralloc_cats(&strTemp,caTemp);	
      tempRow= ltDbFetchRow(tempCursor);
    }
    ltDbCloseCursor(tempCursor);
  }
//  strTemp.s=0;
//	alertListIndex=_ltPubInfo->AlertListIndex;
//	for(iiii=alertListIndex;iiii>-1;iiii--){
//		if(strlen(_ltPubInfo->_AlertList[iiii].srvname)>0){
//			sprintf(srvname,"%s",_ltPubInfo->_AlertList[iiii].srvname);
//			sprintf(Rule_name,"%s",_ltPubInfo->_AlertList[iiii].Rule_name);
//			sprintf(Net_ending_ip,"%s",_ltPubInfo->_AlertList[iiii].Net_ending_ip);
//			printf(Net_ending_mac,"%s",_ltPubInfo->_AlertList[iiii].Net_ending_mac);
//			jjjj=iiii+ALERTTYPENAME-1;
//			if(jjjj>ALERTTYPENAME && jjjj<(ALERTTYPENAMEEND+1)){
//				sprintf(Rule_type,"%s",_ltPubInfo->pubMsgStr[jjjj].conStr);
//			}else{
//				sprintf(Rule_type,"%s",_ltPubInfo->pubMsgStr[ALERTTYPENAME0].conStr);
//			}
//			sprintf(caTemp,"<b>%s</b>[%s:%s  %s:%s %s:%s %s:%s<b><br>\r\n",
//			Rule_type,_ltPubInfo->pubMsgStr[ALERTTYPENAME0+1].conStr,srvname,_ltPubInfo->pubMsgStr[ALERTTYPENAME0+2].conStr,Rule_name,
//			_ltPubInfo->pubMsgStr[ALERTTYPENAME0+3].conStr,Net_ending_ip,_ltPubInfo->pubMsgStr[ALERTTYPENAME0+4].conStr,Net_ending_mac);
//			stralloc_cats(&strTemp,caTemp);	
//		}
//	}
//	for(iiii=MAXALERTNOW-1;iiii>alertListIndex;iiii--){
//		if(strlen(_ltPubInfo->_AlertList[iiii].srvname)>0){
//			sprintf(srvname,"%s",_ltPubInfo->_AlertList[iiii].srvname);
//			sprintf(Rule_name,"%s",_ltPubInfo->_AlertList[iiii].Rule_name);
//			sprintf(Net_ending_ip,"%s",_ltPubInfo->_AlertList[iiii].Net_ending_ip);
//			printf(Net_ending_mac,"%s",_ltPubInfo->_AlertList[iiii].Net_ending_mac);
//			jjjj=iiii+ALERTTYPENAME-1;
//			if(jjjj>ALERTTYPENAME && jjjj<(ALERTTYPENAMEEND+1)){
//				sprintf(Rule_type,"%s",_ltPubInfo->pubMsgStr[jjjj].conStr);
//			}else{
//				sprintf(Rule_type,"%s",_ltPubInfo->pubMsgStr[ALERTTYPENAME0].conStr);
//			}
//			sprintf(caTemp,"<b>%s</b>[%s:%s  %s:%s %s:%s %s:%s<b><br>\r\n",
//			Rule_type,_ltPubInfo->pubMsgStr[ALERTTYPENAME0+1].conStr,srvname,_ltPubInfo->pubMsgStr[ALERTTYPENAME0+2].conStr,Rule_name,
//			_ltPubInfo->pubMsgStr[ALERTTYPENAME0+3].conStr,Net_ending_ip,_ltPubInfo->pubMsgStr[ALERTTYPENAME0+4].conStr,Net_ending_mac);
//			stralloc_cats(&strTemp,caTemp);	
//		}
//	}
	
	stralloc_0(&strTemp);
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,strTemp.s); 
	ltMsgFree(ltMsgPk);
	return 0;
		
}







