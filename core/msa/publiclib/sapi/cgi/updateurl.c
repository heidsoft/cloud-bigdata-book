#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <dirent.h>
#include <stdarg.h>


main(int argc,char *argv[])
{

    char *envstr;

    envstr=(char *)getenv("REQUEST_METHOD");
    
    if (envstr != NULL &&(!strcmp(envstr,"POST")))
    {
        int   cl;
        int   tempiii;
        char  *msgbuf;
        int   fd;
        char  fn[256];
        
        cl = atoi((char *)getenv("CONTENT_LENGTH"));
        if(cl > 8912000) return 0;
        msgbuf=(char *)malloc(cl+1);
        if(!msgbuf){
		       return 0;
        }
        memset(msgbuf,0,cl+1);
        tempiii=0;
        for(tempiii=0;tempiii<cl && (!feof(stdin));tempiii++)
        {
            msgbuf[tempiii]= (char)fgetc(stdin);
        }
        msgbuf[cl]='\0';
        sprintf(fn,"/postdir/01/16/jake.xue@baochuang.com.cn/Inbox/new/%d",time(0));
        fd=open(fn,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0600); 
        if(fd>0){
        	char caMsg[1024];
        	char timebuf[40];
        	struct tm *mytm;
	        time_t mytime;
	        memset(caMsg,0,sizeof(caMsg));
        	strcpy(caMsg,"Content-Type: text/plain\nContent-Transfer-Encoding: 8bit\nMIME-Version: 1.0\n");
			    mytime=time(NULL);
		    	mytm=localtime(&mytime);
			    strftime(timebuf,40,"%a, %d %b %Y %H:%M:%S %z",mytm);
			    sprintf(caMsg,"%sDate:%s\nFrom: urlupload@baochuang.com.cn\nTo: jake.xue@baochuang.com.cn\nSubject: urlupload\n\n",caMsg,timebuf);
        	write(fd,caMsg,strlen(caMsg));
        	write(fd,msgbuf,tempiii);
        	close(fd);
        } 
    }
	  
	  printf("Content-type: text/html\r\n\r\n");
	  printf("ok");
    exit(0);
}   
    
    
    
   
