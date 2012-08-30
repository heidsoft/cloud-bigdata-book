#include "ltmsg.h"
#include "ltcomm.h"
#include "ltplt.h"

ltDbHeadPtr lt_dbinit(){
        ltDbHeadPtr dbPtr;
        xmlNodePtr  tempRootNode;
				dbPtr=NULL;
        dbPtr=(ltDbHeadPtr)malloc(sizeof(ltDbHeadPtr));
        dbPtr->doc=xmlNewDoc("1.0");
				dbPtr->doc->encoding="UTF8";
				dbPtr->doc->charset=1;
        tempRootNode=xmlNewDocNode(dbPtr->doc, NULL, "ltdb", NULL);
        xmlDocSetRootElement(dbPtr->doc,tempRootNode);
        tempRootNode=xmlDocGetRootElement(dbPtr->doc);
        dbPtr->bodyPtr=xmlNewChild(tempRootNode, NULL, "ltbody", NULL);
				dbPtr->head=NULL;
        return dbPtr;
}
  
void lt_dbfree(ltDbHeadPtr dbPtr){
	
	if(dbPtr->head){
		free(dbPtr->head);
	}
	if(dbPtr){
		if(dbPtr->doc){
			xmlDtdPtr extSubset, intSubset;
			xmlDocPtr cur;
			cur=dbPtr->doc;
  		        if (cur->ids != NULL) xmlFreeIDTable((xmlIDTablePtr) cur->ids);
			    cur->ids = NULL;
			if (cur->refs != NULL) xmlFreeRefTable((xmlRefTablePtr) cur->refs);
			    cur->refs = NULL;
			free(cur);
		}
	free(dbPtr);
	}

}


int lt_dbput_head(ltDbHeadPtr dbPtr,char *value){
	if(dbPtr->head==NULL){
		dbPtr->head=(char *)malloc(strlen(value)+1);
		strcpy(dbPtr->head,value);
		return 0;
	}else{
		int iPosition;
		char *p;
		iPosition=strlen(dbPtr->head);
		dbPtr->head=(char *)realloc(dbPtr->head,iPosition+strlen(value)+1);
		p=dbPtr->head+iPosition;
		strcpy(p,value);
		return 0;
	}
 	return 0;
}

int lt_dbput_rootvar(ltDbHeadPtr dbPtr,char *name,char *value){
	xmlNodePtr tempNode;
	tempNode=xmlNewChild(dbPtr->bodyPtr, NULL, name, value);
 	return 0;
}

int lt_dbput_rootvars(ltDbHeadPtr dbPtr,int varnum,...){
	 va_list ap;
     char *pName;
     char *pValue;
	 int  xlen;
	 xmlNodePtr tempNode;
	 va_start(ap,varnum);
	 for(xlen=0;xlen<varnum;xlen++){
		pName=va_arg(ap,char*);
		pValue=va_arg(ap,char*);
		
		tempNode=xmlNewChild(dbPtr->bodyPtr, NULL, pName, pValue);
	 }
	 
	 return 0;
}


ltTablePtr lt_dbput_table(ltDbHeadPtr dbPtr,char *tablename){
	xmlNodePtr tempNode;
	char lttable[20];
	sprintf(lttable,"%s","lttable");
	tempNode=xmlNewChild(dbPtr->bodyPtr, NULL, lttable, NULL);
	xmlSetProp(tempNode,"name",tablename);
 	return tempNode;
}


  
ltTablePtr lt_dbput_subtable(ltTablePtr tablePtr,char *tablename){
	xmlNodePtr tempNode;
	char lttable[20];
	sprintf(lttable,"%s","lttable");
	tempNode=xmlNewChild(tablePtr, NULL, lttable, NULL);
	xmlSetProp(tempNode,"name",tablename);
 	return tempNode;
}
  
int lt_dbput_recordvars(ltTablePtr tablePtr,int varnum,...){

	 va_list ap;
   char *pName;
   char *pValue;
   char intValue[40];
	 int  valType;
   int xlen;
   xmlNodePtr tempNode;
	 xmlNodePtr rsNode;
   va_start(ap,varnum);
	 if(varnum>0){
		rsNode=xmlNewChild(tablePtr,NULL,"ltrecord",NULL);
	 }
         for(xlen=0;xlen<varnum;xlen++){
                pName=va_arg(ap,char*);
		valType=va_arg(ap,int);
		if (valType==LT_TYPE_LONG){
				
				sprintf(intValue,"%d",va_arg(ap,long));
				tempNode=xmlNewChild(rsNode, NULL, pName, intValue);
		}else if(valType==LT_TYPE_LONGLONG){
				
				sprintf(intValue,"%lld",va_arg(ap,long long));
				
				tempNode=xmlNewChild(rsNode, NULL, pName, intValue);
		}else if(valType==LT_TYPE_STRING){ 
				
		                pValue=va_arg(ap,char*);
		                tempNode=xmlNewChild(rsNode, NULL, pName, pValue);
		}
        }
        return 0;

}  
    
ltRecordPtr lt_dbput_record(ltTablePtr tablePtr){
        xmlNodePtr tempNode;
        tempNode=xmlNewChild(tablePtr, NULL, "ltrecord", NULL);
        return tempNode;
}
  
	
int lt_dbput_recordvar(ltRecordPtr rsPtr,char *name,char *value){
	xmlNodePtr tempNode;
	tempNode=xmlNewChild(rsPtr, NULL, name, value);
 	return 0;
}
  
char *lt_db_print(ltDbHeadPtr dbPtr){
	xmlChar *mem;
	int size;
	xmlDocDumpMemory(dbPtr->doc, &mem, &size);
	printf("ltdbcontent:\n%s\n",mem);
	return mem;
 } 

char *lt_db_mem(ltDbHeadPtr dbPtr){
	xmlChar *mem;
	int size;
	xmlDocDumpMemory(dbPtr->doc, &mem, &size);
	return mem;
}

int lt_db_response(int iSock,ltMsgHead *psMsgHead0,ltDbHeadPtr dbPtr){
	xmlChar *mem;
        int size;
	int iReturn;
	//char *p;
        xmlDocDumpMemoryEnc(dbPtr->doc, &mem, &size,"UTF8");
	/*p=(char *)malloc(size+10);
	memset(p,0,size+10);
	memcpy(p,mem,30);
	memcpy(p+30,"GB2312",6);
	memcpy(p+36,mem+34,size-34); */  
	
	psMsgHead0->msgpktype=0;
	iReturn=lt_TcpResponse(iSock,psMsgHead0,1,"ltdbcontent",LT_TYPE_STRING,mem);
        free(mem);
	return iReturn;
}



xmlNodePtr lt_dbget_body(xmlDocPtr doc){
	xmlNodePtr node;
	xmlNodePtr tempRootNode;
        tempRootNode=xmlDocGetRootElement(doc);
	
	for(node=tempRootNode->children;node!=NULL;node=node->next){
	    if(node->type==1){	
		if( strcmp(node->name,"ltbody")==0 ){
			return node;
			break;
		}
	   }
	}
	return NULL;
}

xmlNodePtr lt_dbget_table(xmlNodePtr tableParent,char  *tableName){
        xmlNodePtr node;
        for(node=tableParent->children;node!=NULL;node=node->next){
            if(node->type==1){
	        if( strcmp(node->name,"lttable")==0 ){
                        if(xmlGetProp(node,"name")!=NULL){
                                if(strcmp(tableName,xmlGetProp(node,"name"))==0){
                                        return node;
                                        break;
                                }
                        }
                }
	    }
        }
        return NULL;
}

char *lt_dbget_Var(xmlNodePtr nodePtr,char *varName){

	xmlNodePtr childNode;
	
	for(childNode=nodePtr->children;childNode!=NULL;childNode=childNode->next){
	   if(childNode->type==1){	
		if( strcmp(childNode->name,varName)==0 ){
			return xmlNodeGetContent(childNode);	
		}
	   }
	}			
		
	return NULL;
}



char *lt_dbget_LoopVar(xmlNodePtr parNode,char *tablename,char *varName,int index){
	
	xmlNodePtr tempNode;
	xmlNodePtr childNode;
	xmlNodePtr childNode1;
	xmlNodePtr childNode2;
	int tempIndex=1;
	
	if(index<1) {
		return NULL;
	}

	for(childNode=parNode->children;childNode!=NULL;childNode=childNode->next){
	    if(childNode->type==1){	
		if( strcmp(childNode->name,"lttable")==0 && xmlGetProp(childNode,"name")!=NULL ){
			if(strcmp(tablename,xmlGetProp(childNode,"name"))==0){
				for(childNode1=childNode->children;childNode1!=NULL;childNode1=childNode1->next){
				   if(childNode1->type==1){
					if( strcmp(childNode1->name,"ltrecord")==0 ){
						if(tempIndex==index){
							for(childNode2=childNode1->children;childNode2!=NULL;childNode2=childNode2->next){
							    if(childNode2->type==1){
								if( strcmp(childNode2->name,varName)==0 ){
									return xmlNodeGetContent(childNode2);
									break;	
								}
							    }
							}
							break;
						}
						else{
							tempIndex++;	
						}
					}
				   }
				}
				break;
			}			
		}
	    }
	}
	return NULL;
}


int lt_dbget_LoopNum(xmlNodePtr parNode,char *tablename){
	
	xmlNodePtr tempNode;
	xmlNodePtr childNode;
	xmlNodePtr childNode1;
	xmlNodePtr childNode2;
	int tempIndex=0;
	
	for(childNode=parNode->children;childNode!=NULL;childNode=childNode->next){
	    if(childNode->type==1){	
		if( strcmp(childNode->name,"lttable")==0 && xmlGetProp(childNode,"name")!=NULL ){
			if(strcmp(tablename,xmlGetProp(childNode,"name"))==0){
				for(childNode1=childNode->children;childNode1!=NULL;childNode1=childNode1->next){
				   if(childNode1->type==1){
					if( strcmp(childNode1->name,"ltrecord")==0 ){
						tempIndex++;
					}
				   }
				}
				break;
			}			
		}
	    }
	}
	return tempIndex;
}

int lt_db_htmlpage(ltDbHeadPtr dbPtr,char *charset){
	char xHead[200];
	sprintf(xHead,"Content-type: text/html; %s\n",charset);
	lt_dbput_head(dbPtr,xHead);
	return 0;	
}


int lt_db_setcookie(ltDbHeadPtr dbPtr,const char *name, const char *value){
	char *xHead;
	xHead=(char *) malloc(40+strlen(name)+strlen(value));
	sprintf(xHead,"Set-Cookie: %s=%s; path=/\n", name, value);
	lt_dbput_head(dbPtr,xHead);
	free(xHead);
	return 0;
}

int lt_db_setcookieA(ltDbHeadPtr dbPtr,const char *name, const char *value,
        char *pPath,char *pDomain,char *pExpire)
{
    	char *xHead;
	xHead=(char *) malloc(256+strlen(name)+strlen(value));
	sprintf(xHead,"Set-Cookie: %s=%s", name, value);
	if(pPath) {
        	strcat(xHead,";path=");
        	strcat(xHead,pPath);
    	}
	if(pDomain) {
        	strcat(xHead,";domain=");
        	strcat(xHead,pDomain);
    	}
	if(pExpire) {
        	strcat(xHead,";expires=");
        	strcat(xHead,pExpire);
    	}
	strcat(xHead,"\n");
	lt_dbput_head(dbPtr,xHead);
	free(xHead);
	return 0;
 } 


int lt_db_delcookie(ltDbHeadPtr dbPtr,const char *name)
{
    char *xHead;
    char caTime[64];
    struct tm *lt;
    time_t     lTime;
    	xHead=(char *) malloc(100+strlen(name));
	sprintf(xHead,"Set-Cookie: %s=%s", name,"end");
	strcat(xHead,";expires=");
    	time(&lTime);
    	lTime = lTime - 60;
    	lt = gmtime(&lTime);
    	strftime(caTime,63,"%A,%d-%b-%g %H:%M:%S GMT",lt);
    	strcat(xHead,caTime);
    	strcat(xHead,"\n");
	lt_dbput_head(dbPtr,xHead);
	free(xHead);
	return 0;
}
    
   

int lt_db_nocache(ltDbHeadPtr dbPtr){
	char *xHead;
	xHead=(char *) malloc(50);
	sprintf(xHead,"%s%s", "Cache-Control: no-store\n","Pragma: no-cache\n");
	lt_dbput_head(dbPtr,xHead);
	free(xHead);
	return 0;
}

int lt_db_nocachemsie(ltDbHeadPtr dbPtr){
	char *xHead;
	xHead=(char *) malloc(50);
	sprintf(xHead,"%s%s", "Cache-Control: private\n","Pragma: private\n");
	lt_dbput_head(dbPtr,xHead);
	free(xHead);
	return 0;
}

int lt_db_redirect(int iSock,ltMsgHead *psMsgHead0,const char *buf){
	char *xHead;
	xHead=(char *) malloc(100+strlen(buf)*2);
	sprintf(xHead,"Status: 303 Moved\nCache-Control: private\n","Pragma: private\nURI: %s\nLocation: %s\n\n", buf,buf);
	psMsgHead0->msgpktype=1;
	lt_TcpResponse(iSock,psMsgHead0,2,"lthead",LT_TYPE_STRING,xHead,"lthtml",LT_TYPE_STRING,"\n\n");
	free(xHead);
	return 0;
}


int lt_html_response(int iSock,ltMsgHead *psMsgHead0,char *htmlHead,char *htmlContent){
	int iReturn;
	psMsgHead0->msgpktype=1;
	iReturn=lt_TcpResponse(iSock,psMsgHead0,2,"lthead",LT_TYPE_STRING,htmlHead,"lthtml",LT_TYPE_STRING,htmlContent);
	
	return iReturn;
}

int lt_dbput_html(int iSock,ltMsgHead *msgPk,xmlDocPtr xDoc,char *pltPageUrl,char *head,int chSet){

    int size;
	int iReturn;
	char *lt_page_content;
	char *lt_head;
	char *lt_charset;
	char *ppp;
	int  iii;
        
	lt_page_content=ltPltFileParse(pltPageUrl,xDoc,chSet);
	if(lt_page_content==NULL){
		return -1;
	}
	iii=0;
	ppp=lt_page_content;
	while(*ppp!='<'){
		ppp++;
		iii++;
		if(iii>20){
			break;
		}
	}
	

//	ppp=strrchr(lt_page_content,'>');
//	if(ppp){
//		*(ppp+1)='\0';
//	}
	
	if(head==NULL){
		lt_head="Content-type: text/html; charset=GB2312\n";
	}else{
		lt_head=head;
	}
	
	msgPk->msgpktype=1;
	iReturn=lt_TcpResponse(iSock,msgPk,2,"lthead",LT_TYPE_STRING,lt_head,"lthtml",LT_TYPE_STRING,ppp);
	free(lt_page_content);
	return iReturn;
}



int lt_web_download(int iSock,ltMsgHead *psMsgHead0,char *fileurl,char *filename,char *contype){
	char buf[4096];
	int fd,lRead;
  if(filename){
		ltMsgAdd_s(&psMsgHead0,"filename",filename);
	}
	if(contype){
		ltMsgAdd_s(&psMsgHead0,"contenttype",contype);
	}
	fd=open(fileurl,0);
	if(fd<0){
		return -1;	
	}	
	lRead=read(fd,buf,sizeof(buf));
	while(lRead > 0) {
	           write(iSock,buf,lRead);
	           lRead=read(fd,buf,sizeof(buf));
	}
	ltMsgFree(psMsgHead0);
	return 0;
}

int lt_web_downloadA(int iSock,ltMsgHead *psMsgHead0,char *con,int consize,char *filename,char *contype){
	return 0;
}




