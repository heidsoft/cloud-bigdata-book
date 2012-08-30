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
#include <dlfcn.h>

#include "msas.h"

int ltsSysCheck(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){

	
	ltMsgPk->msgpktype=1;
	
	  //ltWebMsgErr(strGmsg[atol(langsel)][atol(stylesel)][4], confd,ltMsgPk);
	ltWebMsgErr("Can't find Function", confd,ltMsgPk);
	  
	ltMsgFree(ltMsgPk);

	return 0;

}

int ltactivesys(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	ltMsgPk->msgpktype=1;
	
	  //ltWebMsgErr(strGmsg[atol(langsel)][atol(stylesel)][4], confd,ltMsgPk);
	ltWebMsgErr("Please contact us to active your system", confd,ltMsgPk);
	  
	ltMsgFree(ltMsgPk);

	return 0;

}


int ltSetCoreFunListN(char *strFunName, int (*op)()){

	int i;
	for(i=0;i<NAS_MAX_COREFUNNUM;i++){
		if(_ltcorefunList[i].op==NULL){
			snprintf(_ltcorefunList[i].strFunName,63,"%s",strFunName);
			_ltcorefunList[i].op=op;
			break;
		}
	}
		
  return 0;

}

int ltCoreFunListInit(){
	
  unsigned int i;
	for(i=0;i<NAS_MAX_COREFUNNUM;i++){
		memset(_ltcorefunList[i].strFunName,0,64);
		_ltcorefunList[i].op=NULL;
	}
	i=0;
  snprintf(_ltcorefunList[i].strFunName,63,"%s","ltsSysCheck");
	_ltcorefunList[i].op=ltsSysCheck;
	
	
	return 0;	
}


int setAllCoreFun(){
	
	  //模块注册
	//ltSetCoreFunListN("ErrorTest",ErrorTest);
	ltSetCoreFunListN("msasTestApp",msasTestApp);
 	ltSetCoreFunListN("msasTestApp1",msasTestApp1);
	ltSetCoreFunListN("msasAddAction",msasAddAction);
	ltSetCoreFunListN("msasShowRulePage",msasShowRulePage);
	ltSetCoreFunListN("msasAnjianAction",msasAnjianAction);
	ltSetCoreFunListN("msasAnjianTree",msasAnjianTree);
	ltSetCoreFunListN("msasShowAnJian",msasShowAnJian);
	ltSetCoreFunListN("msasAlertAnJianAction",msasAlertAnJianAction);
	ltSetCoreFunListN("msasRuleListAction",msasRuleListAction); 
	ltSetCoreFunListN("msasShowRule",msasShowRule); 
	ltSetCoreFunListN("msasRuleAnJianAction",msasRuleAnJianAction);
	ltSetCoreFunListN("msasAlertRuleAction",msasAlertRuleAction);
	ltSetCoreFunListN("msasRuleTypeAction",msasRuleTypeAction);
	ltSetCoreFunListN("msasShowanjianPage",msasShowanjianPage);
	ltSetCoreFunListN("msasAddanjianAction",msasAddanjianAction);
	ltSetCoreFunListN("msasUpdateanjianAction",msasUpdateanjianAction);
	/*注册自己的函数*/
	
  return 0;
}

int nasFindFun(unsigned long lFunCode){
	
//	int i;
	int l=-1;
	int r=_ltPubInfo->maxfuntonnum+1;
	/*for(i=0;i<r;i++){
		if(lFunCode==_ltfunList[i].lFunCode) return i;
	}*/
	
	while(l+1!=r){
		int i=(l+r)/2;
		if(lFunCode<_ltfunList[i].lFunCode) r=i;
		if(lFunCode==_ltfunList[i].lFunCode) return i;
		if(lFunCode>_ltfunList[i].lFunCode) l=i;
	}
	return -1;
}



int ltSetFunListN(char *strFunName, int (*op)()){

	//int i;
	//for(i=0;i<NAS_MAX_FUNNUM;i++){
		  //if(_ltfunList[_ltPubInfo->maxfuntonnum].lFunCode==0){
					_ltfunList[_ltPubInfo->maxfuntonnum].lFunCode=ltMd5Code(strFunName,strlen(strFunName),"LT");;
					_ltfunList[_ltPubInfo->maxfuntonnum].funFlag=0;
					sprintf(_ltfunList[_ltPubInfo->maxfuntonnum].strFunName,"%s",strFunName);
					sprintf(_ltfunList[_ltPubInfo->maxfuntonnum].strFunUrl,"%s","");
					_ltfunList[_ltPubInfo->maxfuntonnum].maxruntime=1200;
					_ltfunList[_ltPubInfo->maxfuntonnum].activeflag=1;
					_ltfunList[_ltPubInfo->maxfuntonnum].rightflag=0;
					_ltfunList[_ltPubInfo->maxfuntonnum].op=op;
					_ltPubInfo->maxfuntonnum++;
		//			break;
		//	}
	//}
  return 0;

}
int ltFunListInit(char *funconffile){
	
  xmlDocPtr doc;
	xmlNodePtr  node;
	xmlNodePtr  childnode;
  xmlNodePtr  tempRootNode;
	char *nodeMaxTime;
	char *nodeName;
	char *nodeDesc;
	char *nodeUrl;
	char *nodeType;
	char *nodeActive;
	char *nodeRightCheck;
	uint32 lFunCode;
	unsigned int i;
	unsigned int j;
	funList tmpFunList;

	
	for(i=0;i<NAS_MAX_FUNNUM;i++){
			_ltfunList[i].lFunCode=0;
			_ltfunList[i].funFlag=0;
			sprintf(_ltfunList[i].strFunName,"%s","");
			sprintf(_ltfunList[i].strFunUrl,"%s","");
			_ltfunList[i].maxruntime=NAS_DEFAULT_FUNTIME;
			_ltfunList[i].op=NULL;
	}
	
			_ltfunList[0].lFunCode=ltMd5Code("ltsSysCheck",strlen("ltsSysCheck"),"LT");;
			_ltfunList[0].funFlag=0;
			sprintf(_ltfunList[0].strFunName,"%s","ltsSysCheck");
			sprintf(_ltfunList[0].strFunUrl,"%s","");
			_ltfunList[0].maxruntime=60;
			_ltfunList[0].op=ltsSysCheck;
			

			
			
			
	doc=xmlParseFile(funconffile); 
	if(doc==NULL){
		return -1;
	}
	i=1;
	tempRootNode=xmlDocGetRootElement(doc);
	for(node = tempRootNode->children; node != NULL; node = node->next) {
	  if(node->name!=NULL){
	  	/*begin core fun*/
			if(!case_diffs(node->name,"corefun")){
				for(childnode=node->children;childnode!=NULL;childnode=childnode->next){
					nodeType=childnode->name;
					nodeName=xmlGetProp(childnode,"name");
					nodeMaxTime=xmlGetProp(childnode,"maxtime");
					nodeDesc=xmlGetProp(childnode,"desc");
					nodeUrl=xmlGetProp(childnode,"url");
					nodeActive=xmlGetProp(childnode,"activeflag");
					nodeRightCheck=xmlGetProp(childnode,"rightflag");
					if(nodeType!=NULL && nodeName!=NULL && nodeMaxTime!=NULL && nodeDesc!=NULL && nodeUrl!=NULL && nodeActive!=NULL && nodeRightCheck!=NULL ){
							for(j=0;j<NAS_MAX_COREFUNNUM;j++){
								if(strcmp(_ltcorefunList[j].strFunName,nodeName)==0){
									  lFunCode=ltMd5Code(nodeName,strlen(nodeName),"LT");
										_ltfunList[i].lFunCode=lFunCode;
										_ltfunList[i].funFlag=0;
										snprintf(_ltfunList[i].strFunName,63,"%s",nodeName);
										sprintf(_ltfunList[i].strFunUrl,"%s","");
										_ltfunList[i].maxruntime=atol(nodeMaxTime);
										_ltfunList[i].rightflag=atol(nodeRightCheck);
										_ltfunList[i].activeflag=atol(nodeActive);
										_ltfunList[i].op=_ltcorefunList[j].op;
										i++;
										break;
								}
							}	
					}
				}
			}
			/*end core fun*/
			/*begin sofun*/
			if(!case_diffs(node->name,"sofun")){
				for(childnode=node->children;childnode!=NULL;childnode=childnode->next){
					nodeType=childnode->name;
					nodeName=xmlGetProp(childnode,"name");
					nodeMaxTime=xmlGetProp(childnode,"maxtime");
					nodeDesc=xmlGetProp(childnode,"desc");
					nodeUrl=xmlGetProp(childnode,"url");
					nodeActive=xmlGetProp(childnode,"activeflag");
					nodeRightCheck=xmlGetProp(childnode,"rightflag");
					if(nodeType!=NULL && nodeName!=NULL && nodeMaxTime!=NULL && nodeDesc!=NULL && nodeUrl!=NULL && nodeActive!=NULL && nodeRightCheck!=NULL ){
									  lFunCode=ltMd5Code(nodeName,strlen(nodeName),"LT");
										_ltfunList[i].lFunCode=lFunCode;
										_ltfunList[i].funFlag=1;
										snprintf(_ltfunList[i].strFunName,63,"%s",nodeName);
										snprintf(_ltfunList[i].strFunUrl,127,"%s",nodeUrl);
										_ltfunList[i].maxruntime=atol(nodeMaxTime);
										_ltfunList[i].rightflag=atol(nodeRightCheck);
										_ltfunList[i].activeflag=atol(nodeActive);
										_ltfunList[i].op=NULL;
										i++;
					}
				}
			}
			/*end sofun*/
			/*begin appfun*/
			if(!case_diffs(node->name,"appfun")){
				for(childnode=node->children;childnode!=NULL;childnode=childnode->next){
					nodeType=childnode->name;
					nodeName=xmlGetProp(childnode,"name");
					nodeMaxTime=xmlGetProp(childnode,"maxtime");
					nodeDesc=xmlGetProp(childnode,"desc");
					nodeUrl=xmlGetProp(childnode,"url");
					nodeActive=xmlGetProp(childnode,"activeflag");
					nodeRightCheck=xmlGetProp(childnode,"rightflag");
					if(nodeType!=NULL && nodeName!=NULL && nodeMaxTime!=NULL && nodeDesc!=NULL && nodeUrl!=NULL && nodeActive!=NULL && nodeRightCheck!=NULL ){
									  lFunCode=ltMd5Code(nodeName,strlen(nodeName),"LT");
										_ltfunList[i].lFunCode=lFunCode;
										_ltfunList[i].funFlag=3;
										snprintf(_ltfunList[i].strFunName,63,"%s",nodeName);
										snprintf(_ltfunList[i].strFunUrl,127,"%s",nodeUrl);
										_ltfunList[i].maxruntime=atol(nodeMaxTime);
										_ltfunList[i].rightflag=atol(nodeRightCheck);
										_ltfunList[i].activeflag=atol(nodeActive);
										_ltfunList[i].op=NULL;
										i++;
					}
				}
			}
			/*end appfun*/
		}				
  }
  
  _ltPubInfo->maxfuntonnum=i;
  /*排序 按照lFunCode升序排列*/
  

  //模块注册
  //ltSetFunListN("ErrorTest",ErrorTest);
  ltSetFunListN("msasTestApp",msasTestApp);
  ltSetFunListN("msasTestApp1",msasTestApp1);
  ltSetFunListN("msasAddAction",msasAddAction);
  ltSetFunListN("msasShowRulePage",msasShowRulePage);
  ltSetFunListN("msasAnjianAction",msasAnjianAction);
  ltSetFunListN("msasAnjianTree",msasAnjianTree);    
  ltSetFunListN("msasShowAnJian",msasShowAnJian);   
  ltSetFunListN("msasAlertAnJianAction",msasAlertAnJianAction);
  ltSetFunListN("msasRuleListAction",msasRuleListAction);
  ltSetFunListN("msasShowRule",msasShowRule);
  ltSetFunListN("msasRuleAnJianAction",msasRuleAnJianAction);
  ltSetFunListN("msasAlertRuleAction",msasAlertRuleAction); 
  ltSetFunListN("msasRuleTypeAction",msasRuleTypeAction);  
  ltSetFunListN("msasShowanjianPage",msasShowanjianPage);  
  ltSetFunListN("msasAddanjianAction",msasAddanjianAction);
  ltSetFunListN("msasUpdateanjianAction",msasUpdateanjianAction);    
  i=_ltPubInfo->maxfuntonnum;
  //printf("iiiiii:%d\n",i);
  //qsort(_ltfunList,i,sizeof(funList),nasSortFunList);
  
	xmlFreeDoc(doc);
	
	
	for(i=0;i<_ltPubInfo->maxfuntonnum-1;i++){
		for(j=_ltPubInfo->maxfuntonnum-1;j>i;j--){
			if(	_ltfunList[j].lFunCode< _ltfunList[j-1].lFunCode){
				memcpy(&tmpFunList,&_ltfunList[j],sizeof(funList));
				memcpy(&_ltfunList[j],&_ltfunList[j-1],sizeof(funList));
				memcpy(&_ltfunList[j-1],&tmpFunList,sizeof(funList));
			}
		}
	}

	
//	for(i=0;i<_ltPubInfo->maxfuntonnum;i++){
//		printf("%d lFunCode==%u %s\n",i,_ltfunList[i].lFunCode, _ltfunList[i].strFunName);
//	}
	
	
	
	return 0;	
}



void resetcatch_hup(int signum)
{
	 
	 return;
}




void resetcatch_alarm(int signum)
{
	_exit(0);
	
}

void die(int e)
{ 
	_exit(e); 
}



/*系统启动的时候运行,初始化*/
int on_app(lt_shmHead *lt_mmHead){

  char caFunFile[256];
  sprintf(caFunFile,"%s/kernel/function",pubconfdir);
	
	ltCoreFunListInit();
	setAllCoreFun();
	ltFunListInit(caFunFile);
	

	

	return 0;
}

int on_proc(lt_shmHead *lt_mmHead){
	

	char *dbUser;
  char *dbPass;
  char *dbName; 
  dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	
	G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if(G_DbCon!=NULL){
		//printf("db connect ok\n");
	}else{
		fprintf(stderr,"db connect error\n");
	}

	return 0;
}


int on_proc_stop(lt_shmHead *lt_mmHead){
	
	ltDbClose(G_DbCon);
	//printf("db close ok\n");
	return 0;
}




int ltTcpDoSRequest(lt_shmHead *lt_MMHead,int confd)
{
    int i;
    int errint;
    uint32  lFunCode;
    ltMsgHead *psMsgHead;
    char **binqqargs;

    int  (*op)();   
     

    
    psMsgHead = (ltMsgHead *)lt_TcpMsgRead(confd,&errint);
    if(psMsgHead == NULL) {
            return 0;
    }
    /*msg包的其他处理*/

    psMsgHead->lMaxBytes = psMsgHead->lBytes;

    lFunCode=0;
    lFunCode=psMsgHead->lFunCode;
    /*later will be hash table and more funtion control*/
    /*serach funtion*/
    //ltMsgPrintMsg(psMsgHead);

    i=nasFindFun(lFunCode);  
    if(i==-1){
    	  ltsSysCheck(confd,psMsgHead,lt_MMHead);
    		close(confd);
    		return 0;
    }
    if(_ltfunList[i].maxruntime>0){
    	alarm(_ltfunList[i].maxruntime);
    }
    /*检查激活问题*/
    //printf("1111111111:%d\n",_ltPubInfo->sysActive);
//    if(_ltPubInfo->sysActive==0){
//    
//    	if(_ltfunList[i].activeflag==1){
//    		ltactivesys(confd,psMsgHead,lt_MMHead);
//    		close(confd);
//    		return 0;
//    	}
//    }   
    /*检查权限问题*/
    
//    if(_ltfunList[i].rightflag!=0){
//    	if(checkRight(confd,psMsgHead,_ltfunList[i].rightflag,lt_MMHead)==-1){
//    		close(confd);
//				return -1;
//			}
//    }
    
    
    if(_ltfunList[i].funFlag==0){
    		op=_ltfunList[i].op;
				op(confd,psMsgHead,lt_MMHead);
				close(confd);
    		return 0;
    }else if(_ltfunList[i].funFlag==1){
    	void *handle;

		  handle = dlopen (_ltfunList[i].strFunUrl, RTLD_LAZY);

		  op = dlsym (handle, _ltfunList[i].strFunName);

		  op(confd,psMsgHead,lt_MMHead);

			close(confd);

		  dlclose (handle);
		  return 0;
    }else if(_ltfunList[i].funFlag==3){/*app*/
    	int pim[2];
		  int pie[2];
		
		  if (pipe(pim) == -1) return -1;
		  if (pipe(pie) == -1) { close(pim[0]); close(pim[1]); return -1; }
		 
		  switch(vfork()) {
		    case -1:
		      close(pim[0]); close(pim[1]);
		      close(pie[0]); close(pie[1]);
		      return -1;
		    case 0:/*child process*/
		      close(pim[1]);
		      close(pie[0]);
		      /*
		      pim[0] read; 
		      pie[1] write
		      */
		      if (fd_move(0,pim[0]) == -1) _exit(120);
		      if (fd_move(1,pie[1]) == -1) _exit(120);
		      binqqargs = (char **) malloc(2 * sizeof(char *));
		      binqqargs[0]=_ltfunList[i].strFunUrl;
		      binqqargs[1]= 0;
		      execv(*binqqargs,binqqargs);
		      _exit(120);
		  }
		  /*
		      pim[1] write; 
		      pie[0] read
		   */
		  close(pim[0]);
		  close(pie[1]);
		  //fd_move(confd,pie[0]);
		  lt_TcpMsgSend(pim[1],psMsgHead);
		  psMsgHead = (ltMsgHead *)lt_TcpMsgRead(confd,&errint);
    	if(psMsgHead != NULL) {
            lt_TcpMsgSend(pim[1],psMsgHead);
    	}
		  close(confd);
		  return 0;
    }else{
    		ltsSysCheck(confd,psMsgHead,lt_MMHead);
    		close(confd);
    		return 0;
    }

    close(confd);
    return 0;
}

void ltCatch_fatal(int sig)
{
    fprintf(stderr,"exit on signal (%d)\n", sig);
    _exit(0);
}


void sig_chld(int sig){
	pid_t pid;
	int stat;
	while( (pid=waitpid(-1,&stat,WNOHANG))>0){
		//printf("child:%d terminates\n",pid);
	}
	return;
}
void sig_int(int sig){
	
	return;
}


int lt_localserver(char *servername)
/*
 * Arguments:
 *      qlen      - maximum server request queue length (Only in TCP */
{   
  int	iSock;	/* socket descriptor and socket type	*/
  struct sockaddr_un  sSin; 
  struct timeval testtime; 
  int    iQlen;  
  char   strCmd[256];
  
  iQlen=LT_LISTENQ;
  
  memset(&sSin,0,sizeof(struct sockaddr_un));
  iSock = socket(AF_LOCAL, SOCK_STREAM, 0);
  unlink(servername);
  if (iSock == -1){
    return -1;
  }
  //setsockopt(iSock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  sSin.sun_family = AF_LOCAL;
  strcpy(sSin.sun_path,servername);
  //sSin.sun_port = htons(localport);
  //sSin.sin_addr.s_addr = htonl(INADDR_ANY); 

  if (bind(iSock,(struct sockaddr *)&sSin, sizeof(sSin)) < 0) {
        close(iSock);
        return (-1);
   }
    testtime.tv_sec=600000; 
		testtime.tv_usec=0;
		setsockopt(iSock,SOL_SOCKET,SO_RCVTIMEO,&testtime,sizeof(testtime));
		testtime.tv_sec = 600000; 
		testtime.tv_usec=0;
		setsockopt(iSock,SOL_SOCKET,SO_SNDTIMEO,&testtime,sizeof(testtime));

  if (listen(iSock,iQlen) == -1){
    	close(iSock);
	return -1;
  }
  sprintf(strCmd,"chmod 777  %s",servername);
  system(strCmd);
  //ndelay_off(iSock);
  
  return iSock;

}

/*启动tcp服务*/

int ltLocalServer(char *confFile){
	
	char *p;


	int  iReturn;
	/*读取配置信息并判断，如果不对返回错误*/
	if(confFile==NULL){
		fprintf(stderr,"Start tcp server error: the conf file is NULL!\n");
		return -1;	
	}

	/*设置消息包验证号*/
	p=bcCnfGetValueS(confFile,"kernel", "msgcheck");
	if(p==NULL){
		/*消息包验证号*/
		ltMsgSetValidCode(0);
	}else{
		ltMsgSetValidCode(atol(p));
	} 
	

	/*设置最大消息包*/
	p=bcCnfGetValueS(confFile,"kernel", "maxmsgpk");
	if(p==NULL){
		/*设置最大消息包*/
		lt_SetMaxTcpBuf(4096);
	}else{
		//ltMsgSetValidCode(atol(p));
		lt_SetMaxTcpBuf(atol(p));
	} 	
	
	
	p=bcCnfGetValueS(confFile,"kernel", "servername");
	if(p==NULL){
		/*设置最大消息包*/
		p="/tmp/msascom3";
	}	
	
    	//iSock =lt_tcpserver(8000, 20);

	iReturn=lt_localserver(p);
  if(iReturn<0){
		return (-1);
  }

	return iReturn;
}


int lt_start_simple_server(lt_shmHead *lt_MMHead,char *confFile,int (*on_app_start)(),int (*on_proc_start)(),int (*on_proc_stop)() ){
	int iReturn;
	
	/*初始化主进程列表*/
	int ltServerSock;
	
	if(on_app_start!=NULL){
			iReturn=on_app_start(lt_MMHead); /*一般用来注册函数*/
			if(iReturn<0){
				fprintf(stderr,"error: can't start on_app_start\n");
				exit(-1);
			}	
	}
	signal(SIGCHLD, sig_chld);
	signal(SIGINT,  sig_int);
	signal(SIGTERM, sig_int);
	signal(SIGPIPE, SIG_IGN);
	
	iReturn=ltLocalServer(confFile);
	if(iReturn<0){
		fprintf(stderr,"start local server error!\n");
		exit(0);
	}
	fprintf(stderr,"start local server ok!\n");
	//printf("start tcp server ok \n");
	/*系统主侦听端口*/
	ltServerSock=iReturn;
	
	for(;;){
	
		struct sockaddr_in sa;
		int dummy = sizeof(sa);
		int fd; 
		fd = accept(ltServerSock,(struct sockaddr *) &sa,&dummy);
		if (fd == -1) continue;
	
		switch(fork()) {
		      case 0:
		        close(ltServerSock);
		        signal(SIGINT, ltCatch_fatal);
						signal(SIGQUIT, ltCatch_fatal);
						signal(SIGILL, ltCatch_fatal);
						signal(SIGTRAP, ltCatch_fatal);
						signal(SIGIOT, ltCatch_fatal);
						signal(SIGFPE, ltCatch_fatal);
						signal(SIGTERM, ltCatch_fatal);
						signal(SIGALRM, ltCatch_fatal);
						alarm(600);
		        if( on_proc_start!=NULL){
						   	iReturn = on_proc_start(lt_MMHead);
						    if(iReturn < 0) {
						        fprintf(stderr," Process %d Init Error \n",getpid());
						        _exit(0);
						    }
			 		 }
						ltTcpDoSRequest(lt_MMHead,fd);
						if( on_proc_stop!=NULL){
							on_proc_stop(lt_MMHead);
						}
		        _exit(0);
		      case -1:
		        fprintf(stderr,"main proc start error: can't start on_app_start\n");
		}
		close(fd);
	}
	
	return 0;
	
}



/*
1> open shm
2> init pub
3> update activesys
4> on_app_start
   11>init core funtion
   12>update core function
   13>init function list
5> start local server
6> proccess request
   
   
*/
int main(int argc,char **argv){
		
	
	char *p;
	int  intMaxShmSize;
	int  intShmKey;
  msasPubInfo *ltPubInfo;
  lt_shmHead   *lt_MMHead;

  
  signal(SIGHUP,resetcatch_hup);
  signal(SIGCHLD, SIG_IGN);
	signal(SIGINT,  SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
  
  pubconfdir=NULL;
  pubconfdir= env_get("RUNCONFDIR");
  if(!pubconfdir){
  	pubconfdir="/etc/msa/msas3";
  }
  
  /*读取最大共享内存数*/
	p=bcCnfGetValueS(pubconfdir,"kernel", "maxshmsize");
	if(p==NULL){
		fprintf(stderr,"Start applicatin error: maxshmsize is NULL!\n");
		die(51);
	}
	intMaxShmSize=atol(p);


	/*读取系统shmKey*/
	p=bcCnfGetValueS(pubconfdir,"kernel", "shmkey");
	if(p==NULL){
		fprintf(stderr,"Start applicatin error: shmkey is NULL!\n");
		die(52);
	} 
	intShmKey=atol(p);
	
	lt_MMHead=msasopenShmMem(intShmKey,intMaxShmSize);
	if(lt_MMHead==NULL){
		fprintf(stderr,"can open share memory!\n");
		die(53);
	}
  
   ltPubInfo=(msasPubInfo  *)lt_MMHead->ShmMemPtr;
	 if(ltPubInfo == NULL) {
	 	  fprintf(stderr,"can open share memory pubinfo!\n");
	    die(54);
	 }
	 
	
   msasInitPubVar(lt_MMHead);
	 


  /*加入其他执行程序*/

  lt_start_simple_server(lt_MMHead,pubconfdir,on_app,on_proc,on_proc_stop);
    
  return 0;
}

