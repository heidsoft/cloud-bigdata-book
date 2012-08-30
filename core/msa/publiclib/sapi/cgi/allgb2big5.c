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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "lt.h"
#include "ltcgi.h"


int main(int argc,char **argv){

	  struct dirent *d;
    struct stat st;
    char caFile1[256];
	  DIR *dir;	
	  
	  struct stat buf;
    char *pTemp;
    char *p;
    int iFd1;
    int iRead;
	  
    dir = opendir(argv[1]);
    if ( dir  ) {
        while ( d = readdir(dir) ) {
            if ( strcmp(d->d_name,".")==0) continue;
	    			if ( strcmp(d->d_name,"..")==0) continue;
	    						sprintf(caFile1, "%s/%s", argv[1], d->d_name);
	    						iFd1 = open(caFile1,O_RDONLY);
    
							    fstat(iFd1,&buf);
							    pTemp = malloc(buf.st_size + 1);
							    iRead=read(iFd1,pTemp,buf.st_size);
							    pTemp[buf.st_size] = 0;
							    
							    ltCvtGbToBig5Head(pTemp,buf.st_size);
							    ltCvtGbToBig5(pTemp,buf.st_size);
							    close(iFd1);
							    
							    iFd1 = open(caFile1,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
							    write(iFd1,pTemp,buf.st_size);
							    close(iFd1);
							    free(pTemp);
				}
		}  
									

}



