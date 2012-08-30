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

#include "msas.h"


void resetcatch_alarm(int signum)
{
	 _exit(0);
}

void die(int e)
{ 
	_exit(e); 
}

void catch_int(int signum)
{
 return;
}

int msassqlProcessOneFold()
{  
    
    struct dirent *d;
    struct stat st;
    DIR *dir;	
    char caFile1[256];
    //int iReturn;
    int  iIndex;
    int iPid,iPidW,iStatus;
    char *dbUser;
	  char *dbPass;
		char *dbName;
		dbName=_ltPubInfo->_dbname;
	  dbUser=_ltPubInfo->_dbuser;
	  dbPass=_ltPubInfo->_dbpass;
    iIndex=0;
    dir = opendir("/datacenter/proccess/msassql" );
    if ( dir ) {
    	  
        while ( d = readdir(dir) ) {
            if (d->d_name[0] == '.') continue;
            //if (d->d_name[0] == '..') continue;
		    		sprintf(caFile1, "/datacenter/proccess/msassql/%s",  d->d_name);
						if (stat(caFile1,&st) == 0 && S_ISREG(st.st_mode)) {
					    /* 当文件在30秒以内没有变动的情况下才移动  */
					    if(st.st_ctime < (time(0) - 900)) {
					    	unlink(caFile1);
					    	continue;
					    }
			        iIndex++;
					    if(iIndex>500){
								unlink(caFile1);
								continue;
							} 
			        if(  st.st_ctime < (time(0) - 10 )  ) {
			    	   
			    	   iPid = fork();	
							 if(iPid == 0) {  
										    static char inbuf[2048];
												static stralloc line = {0};
										    buffer ss;
										    int fd;
										    int match;
										    //int iPid;
										    G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
												if (G_DbCon==NULL){
														printf("inportsql: can't open db\n");
														_exit(0);
											  }			    
										    fd = open_read(caFile1);
										    if (fd == -1)
										    {
										        ltDbClose(G_DbCon);
										        _exit(0);
										    }
			    					    signal (SIGALRM, resetcatch_alarm);
												alarm (180);		
		                    buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
												for (;;)
												{
													   if (getln(&ss,&line,&match,'\n') == -1) break;
													   if (!match && !line.len) { close(fd); _exit(0); }
													   //striptrailingwhitespace(&line);
													   if (!stralloc_0(&line)) break;
														 ltDbExecSql(G_DbCon,line.s); 
												}
												close(fd);
												ltDbClose(G_DbCon);
												_exit(0);
							   }
							   iPidW = wait(&iStatus);
							   unlink(caFile1);
				     }
			    }
		    }
        closedir (dir);
    }
    return 0;
}

void sig_chld(int sig){
	pid_t pid;
	int stat;
	while( (pid=waitpid(-1,&stat,WNOHANG))>0){
		//printf("child:%d terminates\n",pid);
	}
	return;
}

int main(int argc,char **argv){
		
	
	char *p;
	int  intMaxShmSize;
	int  intShmKey;
  msasPubInfo *ltPubInfo;
  lt_shmHead   *lt_MMHead;
  char *dbUser;
  char *dbPass;
  char *dbName; 
  
  //int iPid;
//  int tmpi;
   
  signal(SIGHUP,SIG_IGN);
  signal(SIGINT,SIG_IGN);
  signal(SIGPIPE,SIG_IGN);
  signal(SIGCHLD,SIG_IGN);
  signal(SIGTERM,SIG_IGN);

  /*读取最大共享内存数*/
	p=bcCnfGetValueS(pubconfdir,"kernel", "maxshmsize");
	if(p==NULL){
		fprintf(stderr,"Start applicatin error: maxshmsize is NULL!\n");
		die(51);
	}
	intMaxShmSize=atol(p);

	/*读取系统shmKey*/
	p=bcCnfGetValueS(pubconfdir,"kernel", "shmkey");
	if(p==NULL){
		fprintf(stderr,"Start applicatin error: shmkey is NULL!\n");
		die(52);
	} 
	intShmKey=atol(p);
	
	lt_MMHead=msasopenShmMem(intShmKey,intMaxShmSize);
	if(lt_MMHead==NULL){
		fprintf(stderr,"can open share memory!\n");
		die(53);
	}
  
  ltPubInfo=(msasPubInfo  *)lt_MMHead->ShmMemPtr;
	if(ltPubInfo == NULL) {
	 	  fprintf(stderr,"can open share memory pubinfo!\n");
	    die(54);
	}
	 
  msasInitPubVar(lt_MMHead);
   
  dbName=ltPubInfo->_dbname;
	dbUser=ltPubInfo->_dbuser;
	dbPass=ltPubInfo->_dbpass;

  while(1){
	     	msassqlProcessOneFold();	
				sleep(20);
  } 
  return 0;
}


