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

void ltCatch_fatal(int sig)
{
    printf("exit on signal (%d)\n", sig);
    exit(0);
}

void sig_chld(int sig){
	pid_t pid;
	int stat;
	while( (pid=waitpid(-1,&stat,WNOHANG))>0){
		//printf("child:%d terminates\n",pid);
	}
	return;
}
void sig_int(int sig){
	
	return;
}

int srvrep1(int fd){
	
	  write(fd,"HTTP/1.0 407 Proxy Authentication Required\r\n",strlen("HTTP/1.0 407 Proxy Authentication Required\r\n"));
	  write(fd,"Server: squid/2.5.STABLE12-20060206\r\n",strlen("Server: squid/2.5.STABLE12-20060206\r\n"));
		write(fd,"Mime-Version: 1.0\r\n",strlen("Mime-Version: 1.0\r\n"));
		write(fd,"Content-Type: text/html\r\n",strlen("Content-Type: text/html\r\n"));
		write(fd,"Content-Length: 11\r\n",strlen("Content-Length: 9\r\n"));
		write(fd,"X-Squid-Error: ERR_CACHE_ACCESS_DENIED 0\r\n",strlen("X-Squid-Error: ERR_CACHE_ACCESS_DENIED 0\r\n"));
		write(fd,"Proxy-Authenticate: NTLM\r\n",strlen("Proxy-Authenticate: NTLM\r\n"));
		write(fd,"X-Cache: MISS from BAOCHUANG\r\n",strlen("X-Cache: MISS from BAOCHUANG\r\n"));
		write(fd,"Proxy-Connection: close\r\n",strlen("Proxy-Connection: close\r\n"));
		write(fd,"\r\n\r\n",4);
		write(fd,"123456789\r\n",11);			
	
	return 0;
}

int main(int argc,char *argv[])
{
    
	int	iSock,iReturn;	/* socket descriptor and socket type	*/
	char buf[8096];
  struct sockaddr_in  sSin; 
  int on=1;
  memset(&sSin,0,sizeof(struct sockaddr_in));
  iSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);;
  if (iSock == -1){
    return -1;
  }
  setsockopt(iSock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  sSin.sin_family = AF_INET;
  sSin.sin_port = htons(8000);
  sSin.sin_addr.s_addr = htonl(INADDR_ANY); 

  if (bind(iSock,(struct sockaddr *)&sSin, sizeof(struct sockaddr_in)) < 0) {
        close(iSock);
        return (-1);
   }
  if (listen(iSock,100) == -1){
    	close(iSock);
			return -1;
  }
	
	signal(SIGCHLD, sig_chld);
	signal(SIGINT, sig_int);
	signal(SIGTERM, sig_int);
	signal(SIGPIPE, SIG_IGN);
	
	for(;;){
	
		struct sockaddr_in sa;
		int dummy = sizeof(sa);
		int fd; 
		fd = accept(iSock,(struct sockaddr *) &sa,&dummy);
		if (fd == -1) continue;
	
		switch(fork()) {
		      case 0:
		        close(iSock);
		        signal(SIGINT, ltCatch_fatal);
						signal(SIGQUIT, ltCatch_fatal);
						signal(SIGILL, ltCatch_fatal);
						signal(SIGTRAP, ltCatch_fatal);
						signal(SIGIOT, ltCatch_fatal);
						signal(SIGFPE, ltCatch_fatal);
						signal(SIGTERM, ltCatch_fatal);
						signal(SIGALRM, ltCatch_fatal);
						alarm(1200);
		        srvrep1(fd);
		        
		        memset(buf,0,sizeof(buf));
		        read(fd,buf,8096);
		        printf("%s]n",buf);
		        exit(0);
		      case -1:
		        printf("main proc start error: can't start on_app_start\n");
		}
		close(fd);
	}
	close(iSock);
	

}   
    
    
    
   
