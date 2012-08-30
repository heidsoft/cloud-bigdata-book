#include "publiclib.h"


lt_shmHead *openBigBootMem(unsigned int intMaxShmSize){

	int shmid;
	struct shmid_ds shmbuf;
	void 	*shmMem;
	lt_shmHead *lt_MMHead;
	
	char *su1_2; 
	char addr[9]; 
  int fd_procaddr; 
  unsigned long ADDR; 
  fd_procaddr = open("/proc/bigphysarea",O_RDONLY); 
  read(fd_procaddr,addr,9); 
  ADDR = atol(addr);
  int fd;
  fd=open("/dev/mem",O_RDWR);
  shmMem = mmap(0,intMaxShmSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, ADDR);
  lt_MMHead=(lt_shmHead *)malloc(sizeof(lt_shmHead));
  lt_MMHead->shmKey=0;
  lt_MMHead->intMaxShmSize=intMaxShmSize;
  lt_MMHead->shmid=0;
  lt_MMHead->ShmMemPtr=shmMem;
	return lt_MMHead;

}

int closeBigBootMem(lt_shmHead *lt_MMHead){

  munmap( (void *)lt_MMHead->ShmMemPtr,lt_MMHead->intMaxShmSize);
  //shmctl(lt_MMHead->shmid, IPC_RMID, NULL);
  free(lt_MMHead);
	return 0;

}

lt_shmHead *createShmMem(unsigned int intShmKey,unsigned int intMaxShmSize){

	
	int shmid;
	struct shmid_ds shmbuf;
	void 	*shmMem;
	lt_shmHead *lt_MMHead;
	char  *p;
	int   intShmType;
	
	
	p=bcCnfGetValueS("/etc/gnm/conf","Shm", "shmtype");
	if(p==NULL){
		fprintf(stderr,"Start applicatin error: shmtype is NULL!\n");
		return NULL;	
	}
	intShmType=atol(p);
	
	if(intShmType==1){
    lt_MMHead=openBigBootMem(intMaxShmSize);
    return lt_MMHead;
  }
	
	if ((shmid = shmget(intShmKey, intMaxShmSize, (0666|IPC_CREAT))) == -1){
        fprintf(stderr, "failed to acquire shared memory segment\r\n");
        return NULL;
  }
  
  
        
  if ((shmMem = (void *)shmat(shmid, NULL, 0)) == ((void *)-1)){
        fprintf(stderr,"failed to attach shared memory");
         return NULL;
  }
  memset(shmMem,0,intMaxShmSize);
  if (shmctl(shmid, IPC_STAT, &shmbuf) == -1){
        fprintf(stderr, "failed to get status of shared memory");
         return NULL;
  }
  shmbuf.shm_perm.uid = getuid();
  shmbuf.shm_perm.gid = getgid();
  if (shmctl(shmid, IPC_SET, &shmbuf) == -1){
        fprintf(stderr, "failed to set status of shared memory");
        return NULL;	
  }
  
  lt_MMHead=(lt_shmHead *)malloc(sizeof(lt_shmHead));
  lt_MMHead->shmKey=intShmKey;
  lt_MMHead->intMaxShmSize=intMaxShmSize;
  lt_MMHead->shmid=shmid;
  lt_MMHead->ShmMemPtr=shmMem;
	return lt_MMHead;

}


lt_shmHead *openShmMem(unsigned int intShmKey,unsigned int intMaxShmSize){


	int shmid;
	struct shmid_ds shmbuf;
	void 	*shmMem;
	lt_shmHead *lt_MMHead;
	char  *p;
	int   intShmType;
	
	
	p=bcCnfGetValueS("/etc/gnm/conf","Shm", "shmtype");
	if(p==NULL){
		fprintf(stderr,"Start applicatin error: shmtype is NULL!\n");
		return NULL;	
	}
	intShmType=atol(p);
	
	if(intShmType==1){
    lt_MMHead=openBigBootMem(intMaxShmSize);
    return lt_MMHead;
  }
	
	shmid = shmget(intShmKey, 0, 0);
	if (shmid == -1){
        fprintf(stderr, "failed to open shared memory segment\r\n");
        return NULL;	
  }
        
  if ((shmMem = (void *)shmat(shmid, NULL, 0)) == ((void *)-1)){
        fprintf(stderr,"failed to attach shared memory");
        return NULL;
  }
  
  
  lt_MMHead=(lt_shmHead *)malloc(sizeof(lt_shmHead));
  lt_MMHead->shmKey=intShmKey;
  lt_MMHead->intMaxShmSize=intMaxShmSize;
  lt_MMHead->shmid=shmid;
  lt_MMHead->ShmMemPtr=shmMem;
	return lt_MMHead;

}


int closeShmMem(lt_shmHead *lt_MMHead){
  char  *p;
	int   intShmType;
	
	
	p=bcCnfGetValueS("/etc/gnm/conf","Shm", "shmtype");
	if(p==NULL){
		fprintf(stderr,"Start applicatin error: shmtype is NULL!\n");
		return 0;	
	}
	intShmType=atol(p);
	
	if(intShmType==1){
    closeBigBootMem(lt_MMHead);
    return 0;
  }
  shmdt((void *)lt_MMHead->ShmMemPtr);
  shmctl(lt_MMHead->shmid, IPC_RMID, NULL);
  free(lt_MMHead);
	return 0;

}

int rmShmMem(lt_shmHead *lt_MMHead){

  shmdt((void *)lt_MMHead->ShmMemPtr);
  shmctl(lt_MMHead->shmid, IPC_RMID, NULL);
  free(lt_MMHead);
	return 0;

}


