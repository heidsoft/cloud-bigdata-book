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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>

#include "msas.h"



static char inbuf[2048];
static stralloc line = {0};

void catch_alarm (int sig)
{
  _exit(0);
}


int main(int argc,char **argv)
{


    buffer ss;
    int fd;
    int match;
    int iPid;
     
    fd = open_read(argv[1]);
    if (fd == -1)
    {
        printf("open user error\n");
        return 0;
    }



 buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));
 
 
 
 for (;;)
  {
	   if (getln(&ss,&line,&match,'\n') == -1) break;
	   if (!match && !line.len) { close(fd); return 1; }
	   if (!stralloc_0(&line)) break;
	   printf("line: %s\n",line.s);
	   iPid = fork();
		 if(iPid == 0) {
						    
      					signal (SIGALRM, catch_alarm);
								alarm (60);
								char dbUser[128];
						    char dbPass[128];
						    char dbName[128];
						    sprintf(dbName,"%s",bcCnfGetValueS(pubconfdir,"kernel","dbname"));
						    sprintf(dbUser,"%s",bcCnfGetValueS(pubconfdir,"kernel","dbuser"));
						    sprintf(dbPass,"%s",bcCnfGetValueS(pubconfdir,"kernel","dbpass"));
						    G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
						    if (G_DbCon==NULL){
										printf("can't open db\n");
										_exit(0);
						    }
						    ltDbExecSql(G_DbCon,line.s);
						    
							  ltDbClose(G_DbCon);
							  _exit(0);
		 }
		 wait(NULL);
	   //sleep(1);
  }
  close(fd);
  return 0;
}

