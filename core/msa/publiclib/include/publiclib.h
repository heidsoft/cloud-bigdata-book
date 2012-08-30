#ifndef _PUBLICLIB_
#define _PUBLICLIB_

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
#include <sys/io.h> 
#include <sys/mman.h>


typedef struct pub_ShmMem_S {
	unsigned int 	shmKey;
	int 					shmid;
	unsigned int 	intMaxShmSize;
	void  *ShmMemPtr;
} lt_shmHead;

#include "error.h"
#include "buffer.h"
#include "byte.h"
#include "case.h"
#include "fd.h"
#include "getln.h"
#include "open.h"
#include "ltstr.h"
#include "stralloc.h"
#include "ltfile.h"
#include "ltdate.h"
#include "lttype.h"
#include "fmt.h"
#include "wait.h"
#include "env.h"
#include "alloc.h"


int bcCnfGetValueL(char *confdir,char *path,char *pVarName);
int bcCnfGetValue_l(char *pFile,char *pVarName);
char *bcCnfGetValue_s(char *pFile,char *pVarName,char *pValue);
char *bcCnfGetValueS(char *confdir,char *path,char *pVarName);
int ltCnfLookVar(char *pHand,char *pVarName,char *pVarValue);
int bcProcGetValue_s(char *pFile,char *pVarName,char *pVarValue);

int closeShmMem(lt_shmHead *lt_MMHead);
lt_shmHead *openShmMem(unsigned int intShmKey,unsigned int intMaxShmSize);
lt_shmHead *createShmMem(unsigned int intShmKey,unsigned int intMaxShmSize);

#endif
