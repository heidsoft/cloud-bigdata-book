#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

#include "libxml/tree.h"
#include "libxml/parser.h"
#include "publiclib.h" 
#include "ltmsg.h"
#include "ltcgi.h"



int ltGetCgiValue(char *filename,ltCgiConfInfo *psCgiConf){
	xmlDocPtr doc;
	xmlNodePtr  node;
	xmlNodePtr  childnode;
  xmlNodePtr  tempRootNode;
	char *nodeValue;
	char *nodeName;
	char *nodeType;
	char *tempValue;
	doc=xmlParseFile(filename); 
	if(doc==NULL){
		return -1;
	}
	tempRootNode=xmlDocGetRootElement(doc);
	for(node = tempRootNode->children; node != NULL; node = node->next) {
		if(node->type==1){	
			
			if(!case_diffs(node->name,"server")){
				for(childnode=node->children;childnode!=NULL;childnode=childnode->next){
				 if(childnode->type==1){
         if(!case_diffs(childnode->name,"parameter")){
					nodeName=xmlGetProp(childnode,"name");
					nodeValue=xmlGetProp(childnode,"value");
					if(!case_diffs(nodeName,"ipaddr")){
						snprintf(psCgiConf->ipAddr,sizeof(psCgiConf->ipAddr),"%s",	nodeValue);
					}else if(!case_diffs(nodeName,"port")){
						psCgiConf->nPort=atol(nodeValue);
					}else if(!case_diffs(nodeName,"timeout")){
						psCgiConf->lTimeOut=atol(nodeValue);	
				        }else if(!case_diffs(nodeName,"uploadpath")){
						sprintf(psCgiConf->upLoadPath,"%s",	nodeValue);
					}else if(!case_diffs(nodeName,"msgcode")){
						psCgiConf->lMsgCode=atol(nodeValue);
					}else if(!case_diffs(nodeName,"maxmsg")){
						psCgiConf->lMaxMsgPkg=atol(nodeValue);
					}else{
						xmlFreeDoc(doc);
						return -1;
					}
				}
				}
				}
			}else{
				xmlFreeDoc(doc);
				return -1;
			}
		break;
		}
  }
	xmlFreeDoc(doc);
	return 0;
} 



main(int argc,char *argv[])
{
  
   
    ltCgiConfInfo   sCgiConf;               /* 配置信息   */
    char *clientip;
    char *authtype;
    char *clientusr; 
    char *pathInfo1;

    /*得到各个路径*/
    clientip = getenv("REMOTE_ADDR");
    authtype=getenv("AUTH_TYPE");
    clientusr=getenv("REMOTE_USER");
    pathInfo1 = getenv("PATH_TRANSLATED");

   /*得到配置信息*/
   
    /* 读配置文件  */
    memset((char *)&sCgiConf,0,sizeof(ltCgiConfInfo));
    sprintf(sCgiConf.upLoadPath,"/tmp/upload/");
    sCgiConf.lTimeOut = 60;
    sCgiConf.lMaxMsgPkg = 0;
    
    if( ltGetCgiValue("/app/msa/msa/htmlplt/xp.conf",&sCgiConf)!=0){
								printf("Content-type:text/html\n\n");
				    		printf("Internal server error: 服务器配置错误，配置文件错误\n");
			          exit(0);
		}
		
//		 printf("Content-type:text/html\n\n");
//    ltCgiEntry *psEntries;
//    int   totalVar;
//    int   i;
//    psEntries = ltCgiRetrieveArgs(&totalVar);
//    for (i=0;i<totalVar;i++) {
//		      printf("name:%s value:%s\n",psEntries[i].name,psEntries[i].val);
//    }
//    
////   
////	  printf("REMOTE_USER:%s \n",(char*)getenv("QUERY_STRING"));
//		exit(0);
//			          
    lt_SetMaxTcpBuf(sCgiConf.lMaxMsgPkg);
    
    ltCgiDo(&sCgiConf,pathInfo1);

    exit(0);
}   
    
    
    
   
