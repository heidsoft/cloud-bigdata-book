#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <dirent.h>
#include <stdarg.h>

#include "publiclib.h"
#include "dbmysql.h"
#include "ltdb.h"
#include "ltmsg.h"
#include "ltcgi.h"



main(int argc,char *argv[])
{

    ltCgiEntry *psEntries;
    int   totalVar;
    int   i;
    char  *pppp;
    char  gnmfile[256];
    char  caFileName[128];
    int   fd,lRead;
    char  buf[1024];
    
    memset(caFileName,0,sizeof(caFileName));
		memset(gnmfile,0,sizeof(gnmfile));

    
    psEntries = ltCgiRetrieveArgs(&totalVar);
    
    for(i=0;i < totalVar;i++)
    {
       if(strncmp(psEntries[i].name,"gnmfile",6) == 0)
       {    
          snprintf(gnmfile,254,"%s",psEntries[i].val);
       }           
    }
    
    if(strlen(gnmfile)<2){	
    	printf("Content-type: text/html\n\n");
	    printf("get file error\n");
			exit(0);
    }
    pppp=strrchr(gnmfile,'/');
    if(pppp){
    	sprintf(caFileName,"%s",pppp+1);
    }
    printf("Content-disposition: attachment; filename=\"%s\"\n",caFileName);
    printf("Content-type: application/octet-stream\n");
	  printf("Content-transfer-encoding:BINARY\n\n");
    fd=open(gnmfile,0);
	  if(fd<0){
			printf("Content-type: text/html\n\n");
	    printf("open file error\n");
			exit(0);
	  }	
	  lRead=read(fd,buf,sizeof(buf));
	  while(lRead > 0) {
	           fwrite(buf,lRead,1,stdout);
	           lRead=read(fd,buf,sizeof(buf));
	 }
   close(fd);                        
   exit(0);
}   
    
    
    
   
