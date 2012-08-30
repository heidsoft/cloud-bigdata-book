
#include <stdio.h>
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

#include "msas.h"


int main(int argc,char *argv[])
{
    
    char  caCmd[256];

	  
	  /*读取最大共享内存数*/
   
    system("killall -9 msasLocalServer");
    system("killall -9 msasSqlProc");
    system("killall -9 msasComServer");
    system("killall -9 msasTimeUpdate");
    system("killall -9 msasMainProc");
     
    sprintf(caCmd,"ipcrm -M %s",bcCnfGetValueS(pubconfdir,"kernel", "shmkey"));
    
    
    system(caCmd);
	
    return 0;
}


