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
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#define extern 
#include <sys/io.h> 

#include "msas.h"

int errno;
/*整个系统的主进程*/
lt_shmHead   *lt_MMHead;

void die(int e)
{ 
	_exit(e); 
}


/*初始化系统参数、内存结构*/	
int ltStartApp(int sysActiveFlag){

	
	char *p;
	int  intMaxShmSize;
	int  intShmKey;
	

  
  /*读取最大共享内存数*/
	p=bcCnfGetValueS(pubconfdir,"kernel", "maxshmsize");
	if(p==NULL){
		fprintf(stderr,"Start applicatin error: maxshmsize is NULL!\n");
		return -1;	
	}
	intMaxShmSize=atol(p);

	/*读取系统shmKey*/
	p=bcCnfGetValueS(pubconfdir,"kernel", "shmkey");
	if(p==NULL){
		fprintf(stderr,"Start applicatin error: shmkey is NULL!\n");
		return -1;	
	} 
	intShmKey=atol(p);
	
	lt_MMHead=msascreateShmMem(intShmKey,intMaxShmSize);
  
	if(lt_MMHead){
		_ltPubInfo = (msasPubInfo *)lt_MMHead->ShmMemPtr;
    _ltPubInfo->sysActive=sysActiveFlag;
		return 0;
	}
	return -1;

}

	
void catch_hup(int signum)
{
	printf("system receive hup signal.\n");
	_ltPubInfo->lRunning=0;
	    
  /*运行自己的hup信号处理程序*/
  
  _ltPubInfo->lRunning=1;
  printf("mainsystem hup reset ok.\n");
	//exit(0);
}

void catch_int(int signum)
{
	printf("system receive int signal.\n");
	
	
}
void catch_alrm(int signum)
{
  /*系统时钟处理函数*/
  
  _exit(0);
}






int main(int argc,char **argv){

		int  sysActiveFlag;
		int  iReturn;
		int  iPid;
		char **binqqargs;
	
		/**/
		int msasLocalServerPid;
    int msasCommServerPid;
    int msasSqlServerPid;
    int msasTimeUpdatePid;
	
		/*系统信号识别*/
    signal(SIGHUP,catch_hup);
    signal(SIGINT,catch_int);
    signal(SIGCHLD, SIG_IGN);
	  signal(SIGINT,  SIG_IGN);
	  signal(SIGTERM, SIG_IGN);
  	signal(SIGPIPE, SIG_IGN);
    
    /*启动系统进程*/
    
    /*时间同步*/
    /*system("/etc/cron.daily/ntpdate &");*/
   
    sysActiveFlag=1; /*系统不用被激活*/
 
    if(ltStartApp(sysActiveFlag)<0){
				printf("starting application error!\n");
				return 0;
    }
    
    /*1>初始化共享内存参数*/
    printf("begin init pubinfo ok\n");
    iReturn=msasInitPubInfo(lt_MMHead);
    if(iReturn != 0) {
				printf("Init PubInfo error\n");
				closeShmMem(lt_MMHead);
        return -1;
    } 
    printf("Start init pubinfo ok\n");
    
    iReturn=msasInitHostListHash(lt_MMHead->ShmMemPtr);
    if(iReturn != 0) {
				printf("Init HostListInfoHash error\n");
				closeShmMem(lt_MMHead);
        return -1;
    } 
    printf("Start init HostListInfoHash ok\n");
  
	  /*启动临时文件处理程序*/
		printf("starting local server proccess...\n");
	  iPid=fork();
	  if(iPid==0){
			 chdir("/app/msa/bin");
			 binqqargs = (char **) malloc(2 * sizeof(char *));
			 binqqargs[0]= "msasLocalServer";
			 binqqargs[1]= 0;
			 execv(*binqqargs,binqqargs);
			 die(0);
	  }
	  if (iPid>0){
				msasLocalServerPid=iPid;
	  }
//    int iii;
//    msasPubInfo *ltPubInfo;
//    msasOnlineHost  tmpHost;
//    memset(&tmpHost,0,sizeof(msasOnlineHost));
//    ltPubInfo=lt_MMHead->ShmMemPtr;
//    tmpHost.ipadd=ntohl(inet_addr("192.168.0.100"));
//    sprintf(tmpHost.caName,"%s",ltPubInfo->pubMsgStr[231].conStr);
//    sprintf(tmpHost.caKey,"%s","1234-1234-1234-1234");
//
//    tmpHost.sysversion=2010080100;
//    tmpHost.cpunum=10;
//    tmpHost.memnum=20;           	  
//    tmpHost.sysdisknum=10;          
//    tmpHost.appdisknum=5;           
//    tmpHost.usernum=150;            
//    tmpHost.p2pnum=15;              
//    tmpHost.livernum=20;            
//    tmpHost.gamenum=10;             
//    tmpHost.soketnum=6;             
//    tmpHost.tcpnum=2000;            
//    tmpHost.udpnum=3000;            
//    tmpHost.httpnum=350;            
//    tmpHost.urlnum=200;             
//    tmpHost.bytenum=(unsigned long long)3000;           
//    tmpHost.bytenumu=(unsigned long long)1000;          
//    tmpHost.bytenumd=(unsigned long long)2000;   
//    for(iii=0;iii<MSAS_MAX_GROUPLEVEL;iii++){
//    	tmpHost.levelgroup[iii]=0;
//    }
//    tmpHost.levelgroup[0]=6;       
//   msasHashInsertHost(lt_MMHead->ShmMemPtr,tmpHost.ipadd, &tmpHost);
  
   /**/
   printf("starting comm server proccess...\n");
	 iPid=fork();
	 if(iPid==0){
			 chdir("/app/msa/bin");
			 binqqargs = (char **) malloc(2 * sizeof(char *));
			 binqqargs[0]= "msasComServer";
			 binqqargs[1]= 0;
			 execv(*binqqargs,binqqargs);
			 die(0);
	 }
	 if (iPid>0){
				msasCommServerPid=iPid;
	 }
	 
	 printf("starting sql proccess...\n");
	 iPid=fork();
	 if(iPid==0){
			 chdir("/app/msa/bin");
			 binqqargs = (char **) malloc(2 * sizeof(char *));
			 binqqargs[0]= "msasSqlProc";
			 binqqargs[1]= 0;
			 execv(*binqqargs,binqqargs);
			 die(0);
	 }
	 if (iPid>0){
				msasSqlServerPid=iPid;
	 }
	 
	 printf("starting Time proccess...\n");
	 iPid=fork();
	 if(iPid==0){
			 chdir("/app/msa/bin");
			 binqqargs = (char **) malloc(2 * sizeof(char *));
			 binqqargs[0]= "msasTimeUpdate";
			 binqqargs[1]= 0;
			 execv(*binqqargs,binqqargs);
			 die(0);
	 }
	 if (iPid>0){
				msasTimeUpdatePid=iPid;
	 }
			
			
   /*数据备份单独进程*/
   printf("system start success\n");
   while(1){
			sleep(30);
   }
   return 0;

}



