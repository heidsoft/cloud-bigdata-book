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
#include <iconv.h>
#include "msas.h"

lt_shmHead *msascreateShmMem(unsigned int intShmKey,unsigned int intMaxShmSize){

    int shmid;
    struct shmid_ds shmbuf;
    void    *shmMem;
    lt_shmHead *lt_MMHead;
    unsigned int   sysShmMax;
    FILE  *fp;
    char  caTemp[1024];
    int   iSize;
    /*
    cat /proc/cmdline
    bigphysarea=30000
    cat /proc/sys/kernel/shmmax
    */
    
    memset(caTemp,0,sizeof(caTemp));
    fp=fopen("/proc/sys/kernel/shmmax","r");
    if(fp==NULL){
            printf("can't get /proc/sys/kernel/shmmax\n");
            sysShmMax=0;	
    }
    fgets(caTemp,sizeof(caTemp),fp);
    fclose(fp);
    caTemp[1023]='\0';
    for(iSize=0;iSize<1024;iSize++){
        if(caTemp[iSize]=='\0' || caTemp[iSize]==' ' || caTemp[iSize]=='\r' || caTemp[iSize]=='\n' ){
             caTemp[iSize]='\0';
             break;
    	}
    }
    sysShmMax=(unsigned int)atol(caTemp);
    

    
    if(intMaxShmSize>sysShmMax){
        fprintf(stderr,"shmsize is too big or /proc/sys/kernel/shmmax too small!\n");
        return NULL;
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

lt_shmHead *msasopenShmMem(unsigned int intShmKey,unsigned int intMaxShmSize){


    int shmid;

    void    *shmMem;
    lt_shmHead *lt_MMHead;
    
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


int msascloseShmMem(lt_shmHead *lt_MMHead){
  shmdt((void *)lt_MMHead->ShmMemPtr);
  shmctl(lt_MMHead->shmid, IPC_RMID, NULL);
  free(lt_MMHead);
    return 0;
}

char *myHead="<html> \n \
<head> \n \
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"> \n \
<style type=\"text/css\"> <!--\n \
#floater {position: absolute;left: 500;top: 146;width: 125;visibility: visible;z-index: 10;}\n \
--> \n \
BODY{font-family:verdana,sans-serif}PRE{font-family:sans-serif}\n \
.handme {  cursor: hand}\n \
.bord4 {  border: #999999; border-style: solid; border-top-width: 1px; border-right-width: 1px; border-bottom-width: 1px; border-left-width: 1px}\n \
table {  font-size: 9pt}\n \
</style> \n \
<SCRIPT language=JavaScript> \n \
function alltrack() \n \
{ \n \
location.reload(true); \n \
} \n \
function track() \n \
{ \n \
var timeID= setTimeout(\"alltrack()\",25000); \n \
} \n \
</SCRIPT> \n \
<title>Message</title> \n \
</head> \n \
<body bgcolor=\"#f0f0f0\" onLoad=\"track()\" oncontextmenu=\"return false\"   style=\"SCROLLBAR-FACE-COLOR: #f0f0f0;   OVERFLOW: scroll; SCROLLBAR-SHADOW-COLOR: #f0f0f0; SCROLLBAR-3DLIGHT-COLOR: #f0f0f0; SCROLLBAR-ARROW-COLOR: #f0f0f0; SCROLLBAR-DARKSHADOW-COLOR: #cccccc;\"  > \n \
<div ID=\"floater\" style=\"left: 50px; top: 30px\"> \n \
<table border=\"1\" align=\"left\" cellspacing=\"0\" cellpadding=\"0\" bgcolor=\"#FFFFFF\" bordercolor=\"#000000\">\n \
<tr>\n \
<td width=\"150\"  > &nbsp;  </td>\n \
<td>\n \
<table width=\"450\" border=\"0\" align=\"left\" cellpadding=\"4\" cellspacing=\"0\">\n \
<tr> \n \
<td> \n \
<img src=\"/images/msg.png\">\n \
<p> <strong>System Message </strong> \n \
<hr>\n \
<p> \n \
<strong>";

char *myEnd1="</strong></p> \n \
<p> \n \
<span onClick=\"JavaScript:self.close();\" style=\"cursor: hand\" >¹ر֒³Ħ</span>   \n \
<span onClick=\"JavaScript:location.reload(true);\" style=\"cursor: hand\" >ˢтҳĦ</span>  \n \
<span onClick=\"javascript:history.back(1);\" style=\"cursor: hand\" >ǰһҳĦ</span>  \n \
</p> \n \
</td></tr></table></td></tr></table></div>  \n \
<script LANGUAGE=\"JavaScript\">\n \
self.onError=null; \n \
currentX = currentY = 0;\n \
whichIt = null;\n \
lastScrollX = 0; lastScrollY = 0;\n \
NS = (document.layers) ? 1 : 0;\n \
IE = (document.all) ? 1: 0;\n \
<!-- STALKER CODE --> \n \
function heartBeat() {\n \
if(IE) { diffY = document.body.scrollTop; diffX = document.body.scrollLeft; }\n \
if(NS) { diffY = self.pageYOffset; diffX = self.pageXOffset; }\n \
if(diffY != lastScrollY) {\n \
percent = .1 * (diffY - lastScrollY);\n \
if(percent >  0) percent = Math.ceil(percent);\n \
else percent = Math.floor(percent);\n \
if(IE) document.all.floater.style.pixelTop += percent;\n \
if(NS) document.floater.top += percent;\n \
lastScrollY = lastScrollY + percent; }\n \
if(diffX != lastScrollX) {\n \
percent = .1 * (diffX - lastScrollX);\n \
if(percent >  0) percent = Math.ceil(percent);\n \
else percent = Math.floor(percent);\n \
if(IE) document.all.floater.style.pixelLeft += percent;\n \
if(NS) document.floater.left += percent;\n \
lastScrollX = lastScrollX + percent;}}\n \
<!-- /STALKER CODE --> \n \
<!-- DRAG DROP CODE --> \n \
function checkFocus(x,y) {\n \
stalkerx = document.floater.pageX;\n \
stalkery = document.floater.pageY;\n \
stalkerwidth = document.floater.clip.width;\n \
stalkerheight = document.floater.clip.height;\n \
if( (x >  stalkerx && x <(stalkerx+stalkerwidth)) && (y >  stalkery && y <(stalkery+stalkerheight))) return true;\n \
else return false; }\n \
function grabIt(e) {\n \
if(IE) {whichIt = event.srcElement;\n \
while (whichIt.id.indexOf(\"floater\") == -1) {\n \
whichIt = whichIt.parentElement;\n \
if (whichIt == null) { return true; } }\n \
whichIt.style.pixelLeft = whichIt.offsetLeft;\n \
whichIt.style.pixelTop = whichIt.offsetTop;\n \
currentX = (event.clientX + document.body.scrollLeft);\n \
currentY = (event.clientY + document.body.scrollTop); } else {\n \
window.captureEvents(Event.MOUSEMOVE);\n \
if(checkFocus (e.pageX,e.pageY)) {\n \
whichIt = document.floater;\n \
StalkerTouchedX = e.pageX-document.floater.pageX;\n \
StalkerTouchedY = e.pageY-document.floater.pageY; } }\n \
return true;}\n \
function moveIt(e) {\n \
if (whichIt == null) { return false; }\n \
if(IE) {\n \
newX = (event.clientX + document.body.scrollLeft);\n \
newY = (event.clientY + document.body.scrollTop);\n \
distanceX = (newX - currentX); distanceY = (newY - currentY);\n \
currentX = newX; currentY = newY;\n \
whichIt.style.pixelLeft += distanceX;\n \
whichIt.style.pixelTop += distanceY;\n \
if(whichIt.style.pixelTop <document.body.scrollTop) whichIt.style.pixelTop = document.body.scrollTop;\n \
if(whichIt.style.pixelLeft <document.body.scrollLeft) whichIt.style.pixelLeft = document.body.scrollLeft;\n \
if(whichIt.style.pixelLeft >  document.body.offsetWidth - document.body.scrollLeft - whichIt.style.pixelWidth - 20) whichIt.style.pixelLeft = document.body.offsetWidth - whichIt.style.pixelWidth - 20;\n \
if(whichIt.style.pixelTop >  document.body.offsetHeight + document.body.scrollTop - whichIt.style.pixelHeight - 5) whichIt.style.pixelTop = document.body.offsetHeight + document.body.scrollTop - whichIt.style.pixelHeight - 5;\n \
event.returnValue = false;\n \
} else {\n \
whichIt.moveTo(e.pageX-StalkerTouchedX,e.pageY-StalkerTouchedY);\n \
if(whichIt.left <0+self.pageXOffset) whichIt.left = 0+self.pageXOffset;\n \
if(whichIt.top <0+self.pageYOffset) whichIt.top = 0+self.pageYOffset;\n \
if( (whichIt.left + whichIt.clip.width) >= (window.innerWidth+self.pageXOffset-17)) whichIt.left = ((window.innerWidth+self.pageXOffset)-whichIt.clip.width)-17;\n \
if( (whichIt.top + whichIt.clip.height) >= (window.innerHeight+self.pageYOffset-17)) whichIt.top = ((window.innerHeight+self.pageYOffset)-whichIt.clip.height)-17;\n \
return false; } return false; }\n \
function dropIt() {\n \
whichIt = null;\n \
if(NS) window.releaseEvents (Event.MOUSEMOVE);\n \
return true; }\n \
<!-- DRAG DROP CODE --> \n \
if(NS) {\n \
window.captureEvents(Event.MOUSEUP|Event.MOUSEDOWN);\n \
window.onmousedown = grabIt;\n \
window.onmousemove = moveIt;\n \
window.onmouseup = dropIt; }\n \
if(IE) {\n \
document.onmousedown = grabIt;\n \
document.onmousemove = moveIt;\n \
document.onmouseup = dropIt; }\n \
if(NS || IE) action = window.setInterval(\"heartBeat()\",1);\n \
</script> \n \
</body> \n \
</html>\n";

int ltWebMsgErr(char *errorMsg,int confd,ltMsgHead *ltMsgPk)
{
        char *errmsg;
        int  strsize;
        strsize=strlen(myHead)+strlen(errorMsg)+strlen(myEnd1)+10;
        errmsg=malloc(strsize);
        memset(errmsg,0,strsize);
        sprintf(errmsg,"%s%s%s",myHead,errorMsg,myEnd1);
    	
        ltMsgPk->msgpktype=1;
        lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,errmsg);		
        free(errmsg);

        return 0;
}

void static striptrailingwhitespace(stralloc *sa)
{
 while (sa->len > 0)
   switch(sa->s[sa->len - 1])
    {
     case '\n':  case '\r':
       --sa->len;
       break;
     default:
       return;
    }
}

int msasInitPubInfo(lt_shmHead *lt_MMHead)
{
    char dbUser[128];
    char dbPass[128];
    char dbName[128];
    int  i,k;
    buffer ss;
    int fd;
    int match;
    char caTempDir[256];
    static char inbuf[4096];
    static stralloc line = {0};
    
    sprintf(dbName,"%s",bcCnfGetValueS(pubconfdir,"kernel","dbname"));
    sprintf(dbUser,"%s",bcCnfGetValueS(pubconfdir,"kernel","dbuser"));
    sprintf(dbPass,"%s",bcCnfGetValueS(pubconfdir,"kernel","dbpass"));
    
    memset(lt_MMHead->ShmMemPtr,0,lt_MMHead->intMaxShmSize);
    
    _ltPubInfo = (msasPubInfo *)lt_MMHead->ShmMemPtr;
    if(_ltPubInfo == NULL) {
          printf("init memory error!\n");
        return (-1);
    }
    
      snprintf(_ltPubInfo->_dbname,127,"%s",dbName);
      snprintf(_ltPubInfo->_dbuser,127,"%s",dbUser);
      snprintf(_ltPubInfo->_dbpass,127,"%s",dbPass);
      
      //_ltPubInfo->defaultLan=atol(bcCnfGetValueS(pubconfdir,"kernel", "defaultlan"));
      
     
   for(i=0;i<MAX_SYSTEM_MSG;i++){
             memset(_ltPubInfo->pubMsgStr[i].conStr,0,MAX_LANG_LIST_CONSIZE);
   }

  {
      sprintf(caTempDir,"%s","/app/msa/bin/MSASLANGSTR2");
      //printf("caTempDir:%s\n",caTempDir);
        if(ltFileIsExist(caTempDir)!=1) {
          return 0; 
      }
          fd = open_read(caTempDir);
          if (fd == -1)
          {
                return 0; 
          }
          buffer_init(&ss,read,fd,inbuf,sizeof(inbuf));

        for(k=0;k<MAX_SYSTEM_MSG;k++){
      			
                 if (getln(&ss,&line,&match,'\n') == -1) {
                          close(fd);
                          return 0; 
        		 }
                     if (!match && !line.len) { 
                     	   close(fd);
                     	   return 0; 
        			 }
                     striptrailingwhitespace(&line);
                     if (!stralloc_0(&line)) {
                     	    close(fd);
                     		  return 0; 
        			 }
//                   if(line.len>=(MAX_LANG_LIST_CONSIZE-2)){
//                          line.len=MAX_LANG_LIST_CONSIZE-2;
//                          stralloc_0(&line);
//          		 }
                 snprintf(_ltPubInfo->pubMsgStr[k].conStr,MAX_LANG_LIST_CONSIZE-1,"%s",line.s);
      	}
  }
      
  return 0; 
    
}


int msasInitPubVar(lt_shmHead *lt_MMHead){
    
    _ltPubInfo = (msasPubInfo *)lt_MMHead->ShmMemPtr;
    return 0;
}
nasDir* mydirlist(char* dir,int* len)
{
 struct dirent** namelist;
 nasDir *ret=NULL;
 int n,rindex,lindex; //index of ret and index of namelist
 struct stat buf;

 char path[1024];


 n = scandir(dir, &namelist, 0, alphasort); //number of items in namelist
 if (n < 0){
               printf("scandir failed\n");
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
     //  printf("%s\n",path);
//       if(stat(path,&buf)<0)
//       {
//        printf("failed to acquire the state of the file.\n");
//        return NULL;
//       }

       //if(S_ISDIR(buf.st_mode))
       //{
        snprintf(ret[rindex].caDirName,128,"%s",namelist[lindex]->d_name);
        snprintf(ret[rindex].caDirPath,128,"%s",path);
        ret[rindex].dirTime=buf.st_mtime;
        rindex--;
       //}
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


int ccmpn(nasDir* a,nasDir* b)  

{

   int myret;

   myret=a->dirTime-b->dirTime;

   if(myret!=0){
        return myret;	
   }

   return strcmp(a->caDirName,b->caDirName);

} 



int msasReportList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){

  stralloc strTemp;
  char caTmpp[2048];
  //int  iii;
  int  allnum;
  char caMyDir[256];
  char *caDelFile;
  int  fileproc;
  struct stat st;
  int len=0; //sum of file names 
  nasDir *fnames;
  int    listtype;
  char   *manageruser;
  int    jjj;
  char   sname[256];
  char   sdate[256];
  char   suser[256];
  char   stype[256];
  char   searchcon[256];
  int    reporttype;
  char   caFile1[256];

  reporttype=atol(ltMsgGetVar_s(ltMsgPk,"reporttype"));
  manageruser=ltMsgGetVar_s(ltMsgPk,"manageruser");
  caDelFile=ltMsgGetVar_s(ltMsgPk,"delfile");
  sprintf(caMyDir,"/app/msadatacenter/report/%d",reporttype);
  
  if(caDelFile){ 
      char caCmd[256];
      if(strlen(caDelFile)>1){
          sprintf(caCmd,"rm -Rf %s/%s",caMyDir,caDelFile);
          system(caCmd);
      }
      sleep(1);
  }
  fnames=mydirlist(caMyDir,&len);
  if(fnames){
        qsort(fnames,len,sizeof(nasDir),ccmpn); //给文件名按字母排序
  }
  allnum=0;
  
 strTemp.s=0;
 sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",len);
 printf("%s\n",len);
 stralloc_cats(&strTemp,caTmpp);
 jjj=0;
 while(len--)
 { 		
              jjj++;
              memset(sname,0,sizeof(sname));
              memset(sDate,0,sizeof(sdate));
              memset(suser,0,sizeof(suser));
              memset(stype,0,sizeof(stype));
              memset(searchcon,0,sizeof(searchcon));
          
              fileproc=0;
              sprintf(caFile1,"%s/%s/proccess.ini",caMyDir,fnames[len].caDirName);
              
              if(stat(caFile1,&st) == 0){ 
                    fileproc=1; 
              }
              sprintf(caFile1,"%s/%s/info.ini",caMyDir,fnames[len].caDirName);
              bcCnfGetValue_s(caFile1,"sname",sname);
              bcCnfGetValue_s(caFile1,"sdate",sDate);
              bcCnfGetValue_s(caFile1,"suser",suser);
              bcCnfGetValue_s(caFile1,"stype",stype);
              bcCnfGetValue_s(caFile1,"scon",searchcon);
              memset(caTmpp,0,sizeof(caTmpp));

              if(jjj==1){
                     sprintf(caTmpp,"{\"sname\":\"%s\",\"sdate\":\"%s\",\"suser\":\"%s\",\"scon\":\"%s\",\"myurl\":\"%s\",\"stype\":\"%s\",\"fproc\":\"%s\"}",
                     sname,sDate,suser,searchcon,fnames[len].caDirName,stype,fileproc==1?"Search is being generated or encountered an error":"Search successful"); 
                     stralloc_cats(&strTemp,caTmpp);                        
              }else{
                sprintf(caTmpp,",{\"sname\":\"%s\",\"sdate\":\"%s\",\"suser\":\"%s\",\"scon\":\"%s\",\"myurl\":\"%s\",\"stype\":\"%s\",\"fproc\":\"%s\"}",
                sname,sDate,suser,searchcon,fnames[len].caDirName,stype,fileproc==1?"Search is being generated or encountered an error":"Search successful");  
                stralloc_cats(&strTemp,caTmpp);                                  
              }
              printf("5555555555\n");  
 }
 printf("11111111\n"); 
 stralloc_cats(&strTemp,"]}");
 printf("2222222\n"); 
 stralloc_0(&strTemp); 
 printf("3333333\n"); 
 ltMsgPk->msgpktype=1;
 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json; charset=utf-8\n","lthtml",LT_TYPE_STRING,strTemp.s);  
 ltMsgFree(ltMsgPk);
 return 0;

}

unsigned long nasCvtLongTime(char *caSdate,char *caTime)
{
    struct tm *tm;
    char caTemp[16],*p;
    unsigned long lTime;   
    lTime = time(0);
    tm =localtime(&lTime);
    p = ltStrGetWord(caSdate,caTemp,6,"/");
    if(*p == '/' && !ltStrIsSpaces(caTemp)) {
        tm->tm_year = atol(caTemp) - 1900;
        p = ltStrGetWord(p+1,caTemp,6,"/");
        if(*p == '/' && !ltStrIsSpaces(caTemp)) {
            tm->tm_mon = atol(caTemp) - 1;
            p++;
            if(!ltStrIsSpaces(p)) {
                tm->tm_mday = atol(p);
            }
        }
    }else{    
        p = ltStrGetWord(caSdate,caTemp,4,"-");
                       if(*p == '-' && !ltStrIsSpaces(caTemp)) {
                           tm->tm_year = 2000+atol(caTemp) - 1900;
                           p = ltStrGetWord(p+1,caTemp,6,"-");
                           if(*p == '-' && !ltStrIsSpaces(caTemp)) {
                               tm->tm_mon = atol(caTemp) - 1;
                               p++;
                               if(!ltStrIsSpaces(p)) {
                                   tm->tm_mday = atol(p);
                               }
                           }
                       }
       }   
    if(!ltStrIsSpaces(caTime)) {
        p = ltStrGetWord(caTime,caTemp,6,":");
        if(*p == ':' && !ltStrIsSpaces(caTemp)) {
            tm->tm_hour = atol(caTemp);
            p = ltStrGetWord(p+1,caTemp,16,":");
            if(*p == ':' && !ltStrIsSpaces(caTemp)) {
                tm->tm_min = atol(caTemp);
                p++;
                if(!ltStrIsSpaces(p)) {
                    tm->tm_sec = atol(p);
                }
            }
        }
    }
    lTime = mktime(tm);
    return lTime;
}

int nasCvtStime(unsigned long lTime,char *caDate,char *caTime)

{

    struct tm *tm1;

    tm1 = localtime(&lTime);

    sprintf(caDate,"%04d/%02d/%02d",tm1->tm_year + 1900,tm1->tm_mon+1,tm1->tm_mday);

    sprintf(caTime,"%02d:%02d:%02d",tm1->tm_hour,tm1->tm_min,tm1->tm_sec);

    return 0;

}