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


typedef struct nasDir_s {
    char   caDirName[128];
    char   caDirPath[128];
    int    dirTime; 
    int    dirsize;       
} nasDir;

nasDir* mydirlist(char* dir,int* len)
{
 struct dirent** namelist;
 nasDir *ret=NULL;
 int n,rindex,lindex; //index of ret and index of namelist
 struct stat buf;

 char path[1024];


 n = scandir(dir, &namelist, 0, alphasort); //number of items in namelist
 if (n < 0){
               return NULL;
 }else{
       n-=2; //ignore dot and dot-dot
      if(n<=0)
      {
       *len=0;
        return NULL;
      }
    
      rindex=n-1;
      lindex=n+1;
      n+=2;
      *len=rindex+1;

      ret=(nasDir*)malloc((*len)*sizeof(nasDir)); 

      for(;rindex>=0;lindex--)
      {
       if(strcmp(namelist[lindex]->d_name,".")==0 || strcmp(namelist[lindex]->d_name,"..")==0)
       {
        continue;
       }
     
       sprintf(path,"%s/%s",dir,namelist[lindex]->d_name);
       stat(path,&buf);

        snprintf(ret[rindex].caDirName,128,"%s",namelist[lindex]->d_name);
        snprintf(ret[rindex].caDirPath,128,"%s",path);
        ret[rindex].dirTime=buf.st_mtime;
        ret[rindex].dirsize=buf.st_size;
        rindex--;

      }
      while(n--) //frees memory blocks
      {
                   free(namelist[n]);
      }
      free(namelist);

      return ret;
     }
     return NULL;
}

int ccmp(nasDir* a,nasDir* b)
{
 int myret;
 myret=a->dirTime-b->dirTime;
 if(myret!=0){
 		return myret;	
 }
 return strcmp(a->caDirName,b->caDirName);
}


main(int argc,char *argv[])
{

    ltCgiEntry *psEntries;
    int   totalVar;
    int   i;
    ltDbConn *G_DbCon; /*各个包处理进程的数据库连接*/
    char dbName[64];
    char dbUser[64];
    char dbPass[64];
    unsigned long long endsrv;
    char strEndDrv[64];
    char myXml[4096];
    
    char gnmkey[32];
    long long  gnmver;
    int  len;
    nasDir *fnames;
    
    char sqlBuf[1024];
  	ltDbCursor *tempCursor;
	  LT_DBROW tempRow;

    
    sprintf(dbName,"%s","gc@localhost");
    sprintf(dbUser,"%s","root");
    sprintf(dbPass,"%s","");
   
  
		
		memset(myXml,0,sizeof(myXml));
		memset(gnmkey,0,sizeof(gnmkey));

    gnmver=0;
    

    psEntries = ltCgiRetrieveArgs(&totalVar);
    
    for(i=0;i < totalVar;i++)
    {
       if(strncmp(psEntries[i].name,"gnmkey",6) == 0)
       {    
          snprintf(gnmkey,31,"%s",psEntries[i].val);
       }
       if(strncmp(psEntries[i].name,"gnmver",6) == 0)
       {    
          gnmver=atoll(psEntries[i].val);
       }              
    }
    
    if(strlen(gnmkey)<10 || gnmver==0 ){	
    	printf("Content-type: text/html\n\n");
	    printf("get gnmkey or gnmver error\n");
			return ;
    }
    G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if(G_DbCon==NULL){
			printf("Content-type: text/html\n\n");
	    printf("db connect error\n");
			return ;
		}

    sprintf(strEndDrv,"%s","0");
    endsrv=0;
    sprintf(sqlBuf,"select status,agent,euname,mtype,startday,sendday from invoice  where mindex='%s' order by id desc ",gnmkey );
    tempCursor=ltDbOpenCursorS(G_DbCon,sqlBuf);
	  if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					 sprintf(myXml,"%s\n",tempRow[0]);
					 sprintf(myXml,"%s%s\n",myXml,tempRow[1]);
					 sprintf(myXml,"%s%s\n",myXml,tempRow[2]);
					 sprintf(myXml,"%s%s\n",myXml,tempRow[3]);
					 sprintf(myXml,"%s%s\n",myXml,tempRow[4]);
					 sprintf(myXml,"%s%s\n",myXml,tempRow[5]);
				}else{
			    printf("Content-type: text/html\n\n");
			    printf("get client info error\n");
					return ;
			  }
			  sprintf(strEndDrv,"%s24",tempRow[5]);
			  endsrv=atoll(strEndDrv);
				ltDbCloseCursor(tempCursor);
	  }else{
	    printf("Content-type: text/html\n\n");
	    printf("get client info error\n");
			return ;
	  }
    
    
     fnames=mydirlist("/app/updatedir",&len);
		 if(fnames){
		 	qsort(fnames,len,sizeof(nasDir),ccmp); //给文件名按字母排序
		 }
    
     while(len--) //free all allocated memory blocks
     {
           if( (gnmver < atoll(fnames[len].caDirName)) && (atoll(fnames[len].caDirName) < endsrv) ){
              sprintf(myXml,"%s%s(%dBtye)\n",myXml,fnames[len].caDirName,fnames[len].dirsize);
           }
    }
    

		if(fnames){
	 		free(fnames);
	  }
	  
	  printf("Content-type: text/html\r\n\r\n");
	  printf(myXml);
    ltDbClose(G_DbCon);
    exit(0);
}   
    
    
    
   
