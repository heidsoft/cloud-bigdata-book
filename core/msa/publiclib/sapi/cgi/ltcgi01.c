/*  Cgi 操作有关的实用程序  */


#include <stdio.h>
#include <string.h>  
#include <stdlib.h>
#include <stdarg.h>
#include <sys/un.h>
#include "publiclib.h"
#include "ltmsg.h"
#include "ltcomm.h"
#include "ltcgi.h"
#include "libxml/tree.h"
#include "libxml/parser.h"
#include "ltplt.h"

#define LF 10
#define CR 13

#define MALLOC_BLOCK_SIZE	128
#define min(x,y)  (x:x<y)


char *myHeads="<html>\n\
<head>\n\
<title></title>\n\ 
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\">\n\
<script>\n\
function goto(){\n";

char *myEnds="} \n\
</script>\n\
<style type=\"text/css\">\n\
<!--\n\
.style2 {\n\
	font-size: 16px;\n\
	font-weight: bold;\n\
	font-style: italic;\n\
	color: #FF3333;\n\
}\n\
-->\n\
</style>\n\
</head>\n\
<body bgcolor=\"#f0f0f0\" leftmargin=\"0\" topmargin=\"0\" marginwidth=\"0\" marginheight=\"0\" oncontextmenu=\"return false\" onLoad=\"goto()\" >\n \
<p>&nbsp;</p>\n \
<p>&nbsp;";



char *myEnds1="</p>\n</body>\n</html>";


char *myHead="<html> \n \
<head> \n \
<meta http-equiv=\"Content-Type\" content=\"text/html; charset=gb2312\"> \n \
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
<div ID=\"floater\" style=\"left: 20px; top: 10px\"> \n \
<table border=\"1\" align=\"left\" cellspacing=\"0\" cellpadding=\"0\" bgcolor=\"#FFFFFF\" bordercolor=\"#000000\">\n \
<tr>\n \
<td width=\"30\"  > &nbsp;  </td>\n \
<td>\n \
<table width=\"200\" border=\"0\" align=\"left\" cellpadding=\"4\" cellspacing=\"0\">\n \
<tr> \n \
<td> \n \
<img src=\"/images/msg-s.png\">\n \
<p> <strong>System Message </strong> \n \
<hr>\n \
<p> \n \
<strong>";




char *myEnd1="</strong></p> \n \
<p> \n \
<span onClick=\"JavaScript:self.close();\" style=\"cursor: hand\" ><b>Close</b></span> &nbsp;&nbsp;  \n \
<span onClick=\"JavaScript:location.reload(true);\" style=\"cursor: hand\" ><b>Refresh</b></span>&nbsp;&nbsp;  \n \
<span onClick=\"javascript:history.back(1);\" style=\"cursor: hand\" ><b>Back</b></span>  \n \
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

/* 将一个字符串根据分隔附分解为两个字符串  
    line --- 输入---原字符串  输出---将终止符转换为 0
    返回----终止符后面部分的字符串
    
    This Function modified by Li Yunmimg in 2000/7/15 

*/
char *ltCgiMakeWord(char *line, char stop)
{
  int x = 0;
  for(x = 0; ((line[x]) && (line[x] != stop) && (line[x] != '=')); x++);
  if(line[x]) {
    line[x] = 0;
    x++;
  }
  return &line[x];
}


char *ltCgiFmakeWord(FILE *fp, char stop, int *length)
{
  int wsize = MALLOC_BLOCK_SIZE, ll = 0;
  char *word = (char *) malloc(sizeof(char) * (wsize + 1));

  while(1)
  {
    word[ll] = (char)fgetc(fp);
    if(ll == wsize)
    {
      word[ll+1] = '\0';
      wsize += MALLOC_BLOCK_SIZE;
      word = (char *)realloc(word,sizeof(char)*(wsize+1));
    }
    (*length)--;
    if((word[ll] == stop) || (feof(fp)) || (!(*length)))
    {
      if(word[ll] != stop) ll++;
      word[ll] = '\0';
      return word;
    }
    ll++;
  }
}



char ltCgiX2C(char *what)
{
  register char digit;
  digit = (what[0] >= 'A' ? ((what[0] & 0xdf) - 'A')+10 : (what[0] - '0'));
  digit *= 16;
  digit += (what[1] >= 'A' ? ((what[1] & 0xdf) - 'A')+10 : (what[1] - '0'));
  return(digit);
}

/* --------------------------------------------------------------------------- */

void ltCgiUnEscapeUrl(char *url)
{
  register int x,y;

  for(x = 0,y = 0; url[y]; x++, y++)
  {
    if ( (url[x] = url[y]) == '%')
    {
      url[x] = ltCgiX2C(&url[y+1]);
      y+=2;
    }
  }
  url[x] = '\0';
}



void ltCgiPlusToSpace(char *str)
{
  register int x;

  for(x=0; str[x]; x++)
    if(str[x] == '+') str[x] = ' ';
}



/* retrieve arguments from both FORM POST and
   GET inputs. For the GET input it should be of
   the form /cgi-bin/progname?name1=val1&name2=val2 */

ltCgiEntry *ltCgiRetrieveArgs(int *iSumVar)
{
    ltCgiEntry *psEntries;
    register int x,m=0;
    int iMaxVar;
    int cl;
    char *line, *line_ptr;
    char *envstr;

    iMaxVar = 32;
    psEntries = (ltCgiEntry *)malloc(sizeof(ltCgiEntry) * iMaxVar);
    if(psEntries == NULL) {
        *iSumVar = 0;
        return NULL;
    }
    {
        char *pstr;
        envstr=(char*)getenv("QUERY_STRING");
  
        if(envstr==NULL) {
            *iSumVar = 0;
            return NULL;
        }
        line = (char *)malloc(sizeof(char) * (strlen( envstr) + 1));
        strcpy(line, envstr);
        line_ptr = line;
        pstr = line;
        m = 0;
        x = 0;
        while (*line_ptr) {
            if ((*line_ptr) == '&' || (*line_ptr) == '+')
            {
                pstr[x] = '\0';
                if(m >= iMaxVar) {
		                psEntries = (ltCgiEntry *)realloc(psEntries,sizeof(ltCgiEntry) * (iMaxVar+32));
		                iMaxVar = iMaxVar + 32;
		            }
                psEntries[m].name = (char *)malloc(strlen(pstr) + 1);
                strcpy(psEntries[m].name,pstr);
                psEntries[m].val = ltCgiMakeWord(psEntries[m].name,'@');
                x = 0;
                m++;
                line_ptr++;
            }
            else
            {
                pstr[x++] = *line_ptr;
                line_ptr++;
            }
        }
        pstr[x] = '\0';
        psEntries[m].name = (char *)malloc(strlen(pstr) + 1);
        strcpy(psEntries[m].name,pstr);
        psEntries[m].val = ltCgiMakeWord(psEntries[m].name,'@');
        *iSumVar = m + 1;
        free(line);
    }
    envstr=(char *)getenv("REQUEST_METHOD");
    if (envstr != NULL &&(!strcmp(envstr,"POST")))
    {
        cl = atoi((char *)getenv("CONTENT_LENGTH"));
        if(cl > 8912000) return 0;
        for(x=m;cl && (!feof(stdin));x++)
        {
            if(x >= iMaxVar) {
                psEntries = (ltCgiEntry *)realloc(psEntries,sizeof(ltCgiEntry) * (iMaxVar+32));
                iMaxVar = iMaxVar + 32;
            }
            m=x;
            psEntries[x].name = ltCgiFmakeWord(stdin,'&',&cl);
            ltCgiPlusToSpace(psEntries[x].name);
            ltCgiUnEscapeUrl(psEntries[x].name);
            psEntries[x].val = ltCgiMakeWord(psEntries[x].name,'@');
        }
        *iSumVar = m+1; 
    }
    return psEntries;
}


/*分解cookie*/
ltCgiEntry *ltCgiRetrieveCookie(char *cpGetCookie,int *iSum)
{
	char *cpLine;
	char *cpLine_ptr, *cpPtr, *cpFst;
	register int x=0;
	int iMaxVar;
	ltCgiEntry *cookies;	
    iMaxVar = 32;

    cookies = malloc(32 * sizeof(ltCgiEntry));
    if(cookies == NULL) {
        *iSum = 0;
        return NULL;
    }
	cpLine = (char *)malloc(strlen(cpGetCookie)+2);
	sprintf(cpLine,"%s;",cpGetCookie);
	cpFst = cpLine;
	cpLine_ptr = cpLine;
	cpPtr = cpLine;
	while (*cpLine_ptr){
	      	if ((*cpLine_ptr) == ';'){
			*cpLine_ptr = '\0';
			while(*cpPtr){
				if(*cpPtr == '='){
					*cpPtr = ' ';
                    if(x >= iMaxVar) {
                        cookies = (ltCgiEntry *)realloc(cookies,sizeof(ltCgiEntry) * (iMaxVar+32));
                        iMaxVar = iMaxVar + 32;
                    }
                    for(;*cpLine == ' ';cpLine++);
					cookies[x].name = (char *)malloc(sizeof(char) * (strlen(cpLine) + 1));
                    strcpy(cookies[x].name,cpLine);
                    cookies[x].val = ltCgiMakeWord(cookies[x].name,' ');
					x++;
					cpLine_ptr ++;
					cpLine = cpLine_ptr;
					cpPtr = cpLine_ptr;
					break;
				}
				else
					cpPtr++;
			}
		}
		else{			
			cpLine_ptr++;
		}
	}
	free(cpFst);
	*iSum = x;

	return cookies;
}



char *ltCgiMsgErr(char *errorMsg)
{

	  printf("Content-type: text/html\n\n");
	  printf(myHead);
    printf(errorMsg);
    printf(myEnd1);
	
	
}


int ltCgiFreeHead(ltFcgiHead *psCgiHead) 
{
    register int i;
    if(psCgiHead->iSumVar > 0){
        for(i=0;i<psCgiHead->iSumVar;i++) {
            free(psCgiHead->entries[i].name);
            free(psCgiHead->entries[i].val);
        }
        free(psCgiHead->entries);
    }
    if(psCgiHead->iSumCookies > 0){
        for(i=0;i<psCgiHead->iSumCookies;i++) {
            free(psCgiHead->cookies[i].name);
            free(psCgiHead->cookies[i].val);
        }
        free(psCgiHead->cookies);
    }
    
    free(psCgiHead);
    return 0;
}



int ltCgiConnect(ltCgiConfInfo *psCgiConf)
{
    register int i;
    char *ppp;
    int iSock;
    iSock = (-1);
    i = 0;
    ppp=strchr(psCgiConf->ipAddr,'.');
    if(ppp){		
		        iSock = lt_tcpconnect(psCgiConf->ipAddr,psCgiConf->nPort);
		        if(iSock > 0 ) {
		               return iSock;
		        }
		}else{
		
				struct sockaddr_un  sSin;
		    memset(&sSin,0,sizeof(struct sockaddr_un));
		    sSin.sun_family = AF_LOCAL;
		    strcpy(sSin.sun_path,psCgiConf->ipAddr);
		    if( (iSock=socket(AF_LOCAL, SOCK_STREAM, 0)) == (-1)) {
		        return (-1);
		    }
		    /* Connect the socket */
		    if ( connect(iSock,(struct sockaddr *)&sSin,sizeof(sSin)) < 0) {
		        close(iSock);
		        return (-1);
		    }
		
	  }
    return iSock;
}

//int ltCgiConnectlocal(ltCgiConfInfo *psCgiConf)
//{
//    
//    struct sockaddr_un  sSin;
//    int iSock;
//    memset(&sSin,0,sizeof(struct sockaddr_un));
//    sSin.sun_family = AF_LOCAL;
//    strcpy(sSin.sun_path,"/tmp/gnmserver");
//    if( (iSock=socket(AF_LOCAL, SOCK_STREAM, 0)) == (-1)) {
//        return (-1);
//    }
//    /* Connect the socket */
//    if ( connect(iSock,(struct sockaddr *)&sSin,sizeof(sSin)) < 0) {
//        close(iSock);
//        return (-1);
//    }
//    return iSock;
//   
//}



/* 释放变量链表  */
int ltCgiFreeEntries(ltCgiEntry *psEntries,int iSum) 
{
    register int i;
    if(psEntries == NULL) return 0;
    for(i=0;i<iSum;i++) {
        free(psEntries[i].name);
    }
    free(psEntries);
    return 0;
}

/* Send the Environment to Pronline */
int ltCgiSendEnvTo(ltMsgHead **psMsgHead)
{
    char *pEnv;
/* Send the Environment to Pronline */
  
    pEnv = (char *)getenv("SERVER_NAME");
    if(pEnv) {
            ltMsgAdd_s(psMsgHead,"srvname",pEnv);
    }
    pEnv = (char *)getenv("HTTP_USER_AGENT");
    if(pEnv) {
          ltMsgAdd_s(psMsgHead,"usragent",pEnv);
    }

    pEnv = (char *)getenv("SERVER_PORT");
    if(pEnv) {
            ltMsgAdd_s(psMsgHead,"srvport",pEnv);
    }
    pEnv = (char *)getenv("HTTP_REFERER");
    if(pEnv) {
            ltMsgAdd_s(psMsgHead,"httpref",pEnv);
   }
   pEnv = (char *)getenv("REMOTE_ADDR");
   if(pEnv) {
            ltMsgAdd_s(psMsgHead,"clientip",pEnv);
   }
   pEnv = (char *)getenv("REQUEST_URI");
   if(pEnv) {
            ltMsgAdd_s(psMsgHead,"requri",pEnv);
   }
   pEnv = (char *)getenv("AUTH_TYPE");
   if(pEnv) {
            ltMsgAdd_s(psMsgHead,"authtype",pEnv);
    }
   pEnv = (char *)getenv("REMOTE_USER");
   if(pEnv) {
            ltMsgAdd_s(psMsgHead,"clientusr",pEnv);
        }
   pEnv = (char *)getenv("REMOTE_PORT");
   if(pEnv) {
            ltMsgAdd_s(psMsgHead,"cltport",pEnv);
        }
   pEnv = (char *)getenv("REMOTE_IDENT");
   if(pEnv) {
            ltMsgAdd_s(psMsgHead,"cltident",pEnv);
        }
   
    return 0;
}




/*    Do CGI   */

int ltCgiDo(ltCgiConfInfo *psCgiConf,char *curPath)
{
    ltCgiEntry *psEntries,*psCookies;
    char  *pcSetCookie,*p;
    int   totalVar,totalCookie;
    unsigned long  lFunCode;
    ltMsgHead *MsgHead,*getMsgHead;
    int i,iResult,iServer;
    int iSocket;
    int errint;
    int deflan;
    deflan=-1;
     
    lFunCode=0;
    
  
    psEntries = ltCgiRetrieveArgs(&totalVar);

    pcSetCookie = (char *)getenv("HTTP_COOKIE");
    if(pcSetCookie != NULL)
    {
			psCookies = ltCgiRetrieveCookie(pcSetCookie,&totalCookie);
		    }else {
			totalCookie = 0;
			psCookies = NULL;
     }
    
    MsgHead = ltMsgInit(psCgiConf->lMsgCode,0, ltStrGetId(),1024);
    if (MsgHead == NULL ) {
        ltCgiMsgErr("<H1><b>Create Msg package error!</b></H1><br><br>Memory overload!");
        exit(0);
    }
  
    for(i=0;i < totalVar;i++)
    {
       if(strncmp(psEntries[i].name,"Fun",3) == 0)
       {    
           lFunCode = (int)ltMd5Code(psEntries[i].val,strlen(psEntries[i].val),"LT");
           MsgHead->lFunCode = lFunCode;
       }           
       else if(psEntries[i].name[0] != 0) {
           ltMsgAdd_s(&MsgHead,psEntries[i].name,psEntries[i].val);
       }
    }

    ltCgiFreeEntries(psEntries,totalVar);
    if(lFunCode == 0) {
       ltCgiMsgErr("<H1><b>Function set error!</b></H1><br><br>");
       free(MsgHead);
       return 0;
    }

    for (i=0;i<totalCookie;i++) {
		 	if(strcmp(psCookies[i].name,"sessionid")==0){
				MsgHead->sid=atol(psCookies[i].val);
			}else if(strcmp(psCookies[i].name,"ltdeflan")==0){
				deflan=atol(psCookies[i].val);
		       		ltMsgAdd_s(&MsgHead,psCookies[i].name,psCookies[i].val);
			}else{
		       		ltMsgAdd_s(&MsgHead,psCookies[i].name,psCookies[i].val);
			}
    }

    ltCgiFreeEntries(psCookies,totalCookie);
    
    /* Send the Environment to Pronline */
    ltCgiSendEnvTo(&MsgHead);
    
     
     iSocket = ltCgiConnect(psCgiConf);
 
     if(iSocket == -1) {
        ltCgiMsgErr("系统正在数据整理中,稍候...<br><img src=\"/images/loading.gif\"><br>");
        free(MsgHead);
        return 0;
    }
    else {

        iResult = lt_TcpMsgSend(iSocket,MsgHead);
				free(MsgHead);
        if(iResult == -1)
        {
            ltCgiMsgErr("Message Send Error!\n");
        }
        else {
        	  int readtime;
        	  readtime=0;
        	
            getMsgHead = lt_TcpMsgRead(iSocket,&errint);
            while(getMsgHead == NULL && errint !=0)
            {

               getMsgHead = lt_TcpMsgRead(iSocket,&errint);
               readtime++;
               
               sleep(4);
               if(readtime>=4){
               	break;
               }
	    			}	
            if(getMsgHead == NULL && errint !=0)
            {
               ltCgiMsgErr("<b>系统没有返回数据，可能没有数据或参数不对!</b><br><br>");
	    			}	
            else {
	       if(getMsgHead->msgpktype==0){/*xml*/
	               /*if ( (p = ltMsgGetVar_s(getMsgHead,"ltdbcontent"))!=NULL)
	               {
			   char *lt_page_content;
			   char *lt_head;
	   		   char *lt_charset;
			   xmlDocPtr ltDocPtr;
			   
		 	   ltDocPtr=xmlParseMemory(p,strlen(p));
	                   lt_page_content=ltPltFileParse(curPath,ltDocPtr);
		           lt_head=lt_dbget_head(ltDocPtr);
			   lt_charset=lt_dbget_charset(ltDocPtr);
			   
	  		   printf("%s",lt_head);
			  
			   printf("%s",lt_page_content);
			   xmlFreeDoc(ltDocPtr);
			   free(lt_page_content);
	     
	               }*/
		}
		else if( getMsgHead->msgpktype==1){/*html*/
			//printf("Content-type: text/html\n\n");
			//ltMsgPrintMsg(getMsgHead);
     			//return 0;
			if( (p = ltMsgGetVar_s(getMsgHead,"lthead"))!=NULL){
				 printf("%s\n",p);
			}else{
				 printf("Content-type: text/html\n");
			}
			if( (p = ltMsgGetVar_s(getMsgHead,"lthtml"))!=NULL){
				int plen;
				char *pTemp;
				int l;
				int iRead;
				plen=strlen(p);
				pTemp = malloc(plen + 1);
    				strcpy(pTemp,p);
    				pTemp[plen] = 0;
				
				l = 0;
				    while(l < plen) {
       					iRead = printf("%s",pTemp + l);
       					if(iRead == 0) iRead = 1;
       					l = l + iRead;
    				}
    				printf("\n");
    				free(pTemp);
			}
		}else if( getMsgHead->msgpktype==3){/*html*/
			//printf("Content-type: text/html\n\n");
			//ltMsgPrintMsg(getMsgHead);
     			//return 0;
			if( (p = ltMsgGetVar_s(getMsgHead,"lthead"))!=NULL){
				 printf("%s\n",p);
			}else{
				 printf("Content-type: text/html\n");
			}
			if( (p = ltMsgGetVar_s(getMsgHead,"lthtml"))!=NULL){
				int plen;
				char *pTemp;
				int l;
				int iRead;
				plen=strlen(p);
				pTemp = malloc(plen + 1);
    				strcpy(pTemp,p);
    				pTemp[plen] = 0;
				
				l = 0;
				    while(l < plen) {
       					iRead = printf("%s",pTemp + l);
       					if(iRead == 0) iRead = 1;
       					l = l + iRead;
    				}
    				//printf("\n");
    				free(pTemp);
			}
		}
		else if(getMsgHead->msgpktype==2){
			   char caFileName[128];
	                   char buf[4096];
	                   int  lSize;
    			  
	                   p = ltMsgGetVar_s(getMsgHead,"filename");
	                   if(p) {
	                       strcpy(caFileName,p);
	                   }
	                   else {
	                       strcpy(caFileName,"unknown.dat");
	                   }
	                   printf("Content-disposition: attachment; filename=\"%s\"\n",caFileName);
	                   if( (p = ltMsgGetVar_s(getMsgHead,"contenttype"))!=NULL){
			   	printf("Content-type: %s\n",p);
			   }else{
				printf("Content-type: application/octet-stream\n");
			   }
	                   printf("Content-transfer-encoding:BINARY\n\n");

	                   lSize =lt_saferead(iSocket,buf,sizeof(buf));
	                   while(lSize > 0) {
	                       fwrite(buf,lSize,1,stdout);
	                       lSize = lt_saferead(iSocket,buf,sizeof(buf));
	                   }
		}
               free(getMsgHead);
            }
        }
       close(iSocket);
     }
  	 
        //ltMsgPrintMsg(MsgHead);
	
        return 0;
}




/*    Do CGI   */

int ltCgiDoSlow(ltCgiConfInfo *psCgiConf,char *curPath)
{
    ltCgiEntry *psEntries;
    int   totalVar;
    int i;
    char strLocation[2056];
    
    
    
    memset(strLocation,0,sizeof(strLocation));
    psEntries = ltCgiRetrieveArgs(&totalVar);
    printf("Content-type: text/html\n\n");
    printf(myHeads);
    sprintf(strLocation,"self.location.href=\"/main/main.xp?");
    for(i=0;i < totalVar;i++)
    {
               
       if( psEntries[i].name[0] != 0 ) {
       	   sprintf(strLocation,"%s%s=%s+",strLocation,psEntries[i].name,psEntries[i].val);
       }
    }
    sprintf(strLocation,"%s\"\n",strLocation);
    ltCgiFreeEntries(psEntries,totalVar);
    printf(strLocation);
    printf(myEnds);
    printf("<img src=\"/images/loading.gif\"><br>");
    printf(myEnds1);
    return 0;
    
}






/*    Do CGI   */

int ltCgiDoMuti(ltCgiConfInfo *psCgiConf,char *curPath)
{
    ltCgiEntry *psEntries,*psCookies;
    char  *pcSetCookie,*p;
    int   totalVar,totalCookie;
    unsigned long  lFunCode;
    ltMsgHead *MsgHead,*getMsgHead;
    int i,iResult,iServer;
    int iSocket;
    int errint;
    int deflan;
    deflan=-1;
     
    lFunCode=0;
    psEntries = ltCgiRetrieveArgs(&totalVar);

    pcSetCookie = (char *)getenv("HTTP_COOKIE");
    if(pcSetCookie != NULL)
    {
	psCookies = ltCgiRetrieveCookie(pcSetCookie,&totalCookie);
    }else {
	totalCookie = 0;
	psCookies = NULL;
     }
    
    MsgHead = ltMsgInit(psCgiConf->lMsgCode,0, ltStrGetId(),1024);
    if (MsgHead == NULL ) {
        ltCgiMsgErr("<H1><b>Create Msg package error!</b></H1><br><br>Memory overload!");
        exit(0);
    }
  
    for(i=0;i < totalVar;i++)
    {
       if(strncmp(psEntries[i].name,"Fun",3) == 0)
       {    
           lFunCode = (int)ltMd5Code(psEntries[i].val,strlen(psEntries[i].val),"LT");
           MsgHead->lFunCode = lFunCode;
       }           
       else if(psEntries[i].name[0] != 0) {
           ltMsgAdd_s(&MsgHead,psEntries[i].name,psEntries[i].val);
       }
    }

    ltCgiFreeEntries(psEntries,totalVar);
    if(lFunCode == 0) {
      ltCgiMsgErr("<H1><b>Function set error!</b></H1><br><br>");
       free(MsgHead);
       return 0;
    }

    for (i=0;i<totalCookie;i++) {
 	if(strcmp(psCookies[i].name,"sessionid")==0){
		MsgHead->sid=atol(psCookies[i].val);
	}else if(strcmp(psCookies[i].name,"ltdeflan")==0){
		deflan=atol(psCookies[i].val);
       		ltMsgAdd_s(&MsgHead,psCookies[i].name,psCookies[i].val);
	}else{
       		ltMsgAdd_s(&MsgHead,psCookies[i].name,psCookies[i].val);
	}
    }

    ltCgiFreeEntries(psCookies,totalCookie);
    
    /* Send the Environment to Pronline */
    ltCgiSendEnvTo(&MsgHead);
    
     
     iSocket = ltCgiConnect(psCgiConf);
 
     if(iSocket == -1) {
        ltCgiMsgErr("<H2><b>系统暂时不让连接，可能在处理关键业务，30秒钟后重试!<br>System is busy for key issue,try 30 second later!</b></H2><br><br>");
        free(MsgHead);
        return 0;
    }
    else {
        iResult = lt_TcpMsgSend(iSocket,MsgHead);
	free(MsgHead);
        if(iResult == -1)
        {
            ltCgiMsgErr("Message Send Error!\n");
        }
        else {
            getMsgHead = lt_TcpMsgRead(iSocket,&errint);
            if(getMsgHead == NULL && errint !=0)
            {
               ltCgiMsgErr("<H1><b>系统没有返回数据，可能没有数据或参数不对!</b></H1><br><br>");
	    }	
            else {
	       if(getMsgHead->msgpktype==0){/*xml*/
	               /*if ( (p = ltMsgGetVar_s(getMsgHead,"ltdbcontent"))!=NULL)
	               {
			   char *lt_page_content;
			   char *lt_head;
	   		   char *lt_charset;
			   xmlDocPtr ltDocPtr;
			   
		 	   ltDocPtr=xmlParseMemory(p,strlen(p));
	                   lt_page_content=ltPltFileParse(curPath,ltDocPtr);
		           lt_head=lt_dbget_head(ltDocPtr);
			   lt_charset=lt_dbget_charset(ltDocPtr);
			   
	  		   printf("%s",lt_head);
			  
			   printf("%s",lt_page_content);
			   xmlFreeDoc(ltDocPtr);
			   free(lt_page_content);
	     
	               }*/
		}
		else if( getMsgHead->msgpktype==1){/*html*/
			//printf("Content-type: text/html\n\n");
			//ltMsgPrintMsg(getMsgHead);
     			//return 0;
		
			//printf("Content-type: text/html; charset=big5\n");
			if( (p = ltMsgGetVar_s(getMsgHead,"lthead"))!=NULL){
				 
				  printf("%s",p);
				  printf("Content-type: text/html; charset=big5\n\n");
				  
			}else{
				 printf("Content-type: text/html; charset=big5\n\n");
			}

			
			if( (p = ltMsgGetVar_s(getMsgHead,"lthtml"))!=NULL){
				int plen;
				char *pTemp;
				int l;
				int iRead;
				plen=strlen(p);
				pTemp = malloc(plen + 1);
    				strcpy(pTemp,p);
    				pTemp[plen] = 0;
				
				l = 0;
				    while(l < plen) {
       					iRead = printf("%s",pTemp + l);
       					if(iRead == 0) iRead = 1;
       					l = l + iRead;
    				}
    				printf("\n");
    				free(pTemp);
			}
		}
		else if(getMsgHead->msgpktype==2){
			   						char caFileName[128];
	                   char buf[4096];
	                   int  lSize;
    			  
	                   p = ltMsgGetVar_s(getMsgHead,"filename");
	                   if(p) {
	                       strcpy(caFileName,p);
	                   }
	                   else {
	                       strcpy(caFileName,"unknown.dat");
	                   }
	                   printf("Content-disposition: attachment; filename=\"%s\"\n",caFileName);
	                   if( (p = ltMsgGetVar_s(getMsgHead,"contenttype"))!=NULL){
									   		printf("Content-type: %s\n",p);
									   }else{
												printf("Content-type: application/octet-stream\n");
									   }
	                   printf("Content-transfer-encoding:BINARY\n\n");

	                   lSize =lt_saferead(iSocket,buf,sizeof(buf));
	                   while(lSize > 0) {
	                       fwrite(buf,lSize,1,stdout);
	                       lSize = lt_saferead(iSocket,buf,sizeof(buf));
	                   }
		}
               free(getMsgHead);
            }
        }
       close(iSocket);
     }
  	 
        //ltMsgPrintMsg(MsgHead);
	
        return 0;
}






int ltCgiQQDo(ltCgiConfInfo *psCgiConf,char *curPath)
{
    ltCgiEntry *psEntries,*psCookies;
    char  *pcSetCookie,*p;
    int   totalVar,totalCookie;
    unsigned long  lFunCode;
    ltMsgHead *MsgHead;
    int i,iResult,iServer;
    int iSocket;
    int errint;
    int deflan;
    deflan=-1;
     
    lFunCode=0;
    
 
   
    MsgHead = ltMsgInit(psCgiConf->lMsgCode,0, ltStrGetId(),1024);
    if (MsgHead == NULL ) {
        ltCgiMsgErr("<H1><b>Create Msg package error!</b></H1><br><br>Memory overload!");
        exit(0);
    }
    lFunCode = (int)ltMd5Code("nasqqsave",strlen("nasqqsave"),"LT");
    MsgHead->lFunCode = lFunCode;
  
    char *envstr;

    envstr=(char *)getenv("REQUEST_METHOD");
    
    if (envstr != NULL &&(!strcmp(envstr,"POST")))
    {
        int cl;
        int tempiii;
        char  *msgbuf;
        
        cl = atoi((char *)getenv("CONTENT_LENGTH"));
        if(cl > 8912000) return 0;
        msgbuf=(char *)malloc(cl+1);
        if(!msgbuf){
        	 ltCgiMsgErr("<H1><b>msgbuf malloc error!</b></H1><br><br>");
		       free(MsgHead);
		       return 0;
        }
        tempiii=0;
        for(tempiii=0;tempiii<cl && (!feof(stdin));tempiii++)
        {
            msgbuf[tempiii]= (char)fgetc(stdin);
            
        }
        msgbuf[cl]='\0';
        ltMsgAdd_s(&MsgHead,"qqmsgbuffer",msgbuf);
    }
   // ltMsgAdd_s(&MsgHead,"qqmsgbuffer","09-08-03 10:30:00\t21212-21221\teqewqewqewqewq\r\n");
    char *pEnv ;
    pEnv = (char *)getenv("REMOTE_ADDR");
    if(pEnv) {
            ltMsgAdd_s(&MsgHead,"clientip",pEnv);
    }

 
     iSocket = ltCgiConnect(psCgiConf);
 
     if(iSocket == -1) {
        ltCgiMsgErr("系统正在数据整理中,稍候...<br><img src=\"/images/loading.gif\"><br>");
        free(MsgHead);
        return 0;
    }
    else {

        iResult = lt_TcpMsgSend(iSocket,MsgHead);
				free(MsgHead);
        if(iResult == -1)
        {
            ltCgiMsgErr("Message Send Error!\n");
        }
      
        	  
       close(iSocket);
     }
  	 
        //ltMsgPrintMsg(MsgHead);
	
        return 0;
}


int ltCgiDoHttpFun(ltCgiConfInfo *psCgiConf,char *curPath)
{

    ltMsgHead *MsgHead,*getMsgHead;
    int i,iResult,iServer;
    int iSocket;
    int errint;
    char *envstr;
    char *p;
        
    MsgHead = NULL;

    envstr=(char *)getenv("REQUEST_METHOD");
    
    if (envstr != NULL &&(!strcmp(envstr,"POST")))
    {
        int cl;
        int tempiii;
        char  *msgbuf;
        
        cl = atoi((char *)getenv("CONTENT_LENGTH"));
        if(cl > 8912000) return 0;
        msgbuf=(char *)malloc(cl+1);
        if(!msgbuf){
        	 printf("Content-type: text/html\r\n\r\n");
        	 printf("msgbuf malloc error!\r\n");
					 exit(0);
        }
        tempiii=0;
        for(tempiii=0;tempiii<cl && (!feof(stdin));tempiii++)
        {
            msgbuf[tempiii]= (char)fgetc(stdin);
        }
        msgbuf[cl]='\0';
        MsgHead=(ltMsgHead *)msgbuf;
    }

 
     if (MsgHead == NULL ) {
    	              printf("Content-type: text/html\r\n\r\n");
						        printf("Create Msg package error!Memory overload2!\r\n");
						        exit(0);
		}
//		else{
//		  printf("Content-type: text/html\r\n\r\n");
//		  ltMsgAllRecord(MsgHead,(char *)NULL,ltMsgPrint);
//		   exit(0);
//	  }
		
    if(MsgHead->lFunCode == (int)ltMd5Code("testreply",strlen("testreply"),"LT")){
    	     
						      
						      
						      int  lSize;
					    		getMsgHead=	MsgHead;  
						      
//						      printf("Content-type: text/html\r\n\r\n");
//  		  					printf("%d\n",getMsgHead->lBytes);
//  		   					exit(0);
						      
						      //printf("Content-Length: %d\r\n",getMsgHead->lBytes);
						      printf("Content-disposition: attachment; filename=\"baochuanghttpproxy\"\r\n"); 
									printf("Content-Type: application/octet-stream\r\n");
									printf("Content-transfer-encoding:BINARY\r\n\r\n");
					        
					        
					        p=(char *)getMsgHead;
					        lSize =getMsgHead->lBytes;
						      while(lSize > 0) {
						          if(lSize>512){
						          	fwrite(p,512,1,stdout);
						          	p=p+512;
						          	lSize=lSize-512;
						          }else{
						            fwrite(p,lSize,1,stdout);
						            break;
						          }
						      }
							  
                free(getMsgHead); 
                exit(0);   	
    }
    

    iSocket = ltCgiConnect(psCgiConf);
 
     if(iSocket == -1) {
     	  printf("Content-type: text/html\r\n\r\n");
        printf("系统无法连接\r\n");
        free(MsgHead);
        exit(0);  
    }
    else {

        iResult = lt_TcpMsgSend(iSocket,MsgHead);
				free(MsgHead);
        if(iResult == -1)
        {
            printf("Content-type: text/html\r\n\r\n");
            printf("Message Send Error!\r\n");
            close(iSocket);
            exit(0);  
        }
        else {
        	  int readtime;
        	  readtime=0;
        	
            getMsgHead = lt_TcpMsgRead(iSocket,&errint);
            while(getMsgHead == NULL && errint !=0)
            {

               getMsgHead = lt_TcpMsgRead(iSocket,&errint);
               readtime++;
               
               sleep(4);
               if(readtime>=4){
               	break;
               }
	    			}	
            if(getMsgHead == NULL && errint !=0)
            {
               printf("Content-type: text/html\r\n\r\n");
               printf("系统没有返回数据，可能没有数据或参数不对!\r\n");
               close(iSocket);
               exit(0); 
	    			}	
            else {

						      int  lSize;
					    			  
						      printf("Content-disposition: attachment; filename=\"baochuanghttpproxy\"\r\n"); 
									printf("Content-Type: application/octet-stream\r\n");
									printf("Content-transfer-encoding:BINARY\r\n\r\n");
					        
					        p=(char *)getMsgHead;
					        lSize =getMsgHead->lBytes;
						      while(lSize > 0) {
						          if(lSize>512){
						          	fwrite(p,512,1,stdout);
						          	p=p+512;
						          	lSize=lSize-512;
						          }else{
						            fwrite(p,lSize,1,stdout);
						            break;
						          }
						      }
						   
               free(getMsgHead);
               close(iSocket);
               exit(0); 
            }
        }
       
     }
  	
     exit(0); 
	
     return 0;
}

