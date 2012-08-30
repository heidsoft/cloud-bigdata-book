/*lt通信协议的实现*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdarg.h>

#include "ltcomm.h"

long   LTCOMMAXTCPBUF = LT_COM_MAXBUFFER;
long   LTMSGPKGONESIZE=1024;


/* Send a Tcp Message */
int lt_SetMaxTcpBuf(long lMaxTcpBuf)
{
    LTCOMMAXTCPBUF = lMaxTcpBuf;
    return 0;
}



int lt_TcpMsgSend(int iSock,ltMsgHead *psMsgHead)
{
    
    
    ltMsgHead sMsgHead0;    
    long iReturn;
    /*初始化*/
    ltMsgHton(psMsgHead);
    
    /*发送操作*/
	  /*发送消息头*/
    iReturn=lt_safewrite(iSock,(char *)psMsgHead,sizeof(ltMsgHead));  
    if(iReturn!=sizeof(ltMsgHead)){
            return (-1);
    }
	  /*得到消息头的反馈*/
	  iReturn=lt_saferead(iSock,(char *)&sMsgHead0,sizeof(ltMsgHead));
	  if(iReturn != sizeof(ltMsgHead)) {
             return (-2);
    }
	  //ltMsgNtoh(&sMsgHead0);	
	  /*判断返回的消息包*/
	  iReturn=ltMsgIsValidCode(ntohl(sMsgHead0.lCode)) ;
	  if(iReturn==0){
		    return (-3);
    }  
	  if(ntohl(sMsgHead0.lMsgId)!=ntohl(psMsgHead->lMsgId)){
					return (-3);
    }      
	  if(ntohl(sMsgHead0.comFlag)!=100){
			if(ntohl(sMsgHead0.comFlag)==11){
				return -11;/*too big*/
			}else if (ntohl(sMsgHead0.comFlag)==12){
				return -12;
			}else{
				return -13;
			}
    }
   	if(ntohl(psMsgHead->lBytes)>sizeof(ltMsgHead)){
		/*循环发送消息体，每4096个为单位*/
		char *sendP; /*发送msg包的当前位置*/
		int  sendLen; /*本次发送的长度*/
		int  leaveLen; 	/*剩余全部发送的长度*/	

		sendP=(char *)psMsgHead+sizeof(ltMsgHead);
		leaveLen=ntohl(psMsgHead->lBytes)-sizeof(ltMsgHead);
		if(leaveLen<=LTMSGPKGONESIZE){
			sendLen=leaveLen;
			leaveLen=0;
		}else{
			sendLen=LTMSGPKGONESIZE;
			leaveLen=leaveLen-LTMSGPKGONESIZE;
		}		

		/*发送消息体*/
		while(sendLen>0){
			iReturn=lt_safewrite(iSock,(char *)sendP,sendLen);  	
			if(iReturn!=sendLen){
		            return (-4);
		  }
		        /*得到消息体的反馈*/
			iReturn=lt_saferead(iSock,(char *)&sMsgHead0,sizeof(ltMsgHead));
			if(iReturn != sizeof(ltMsgHead)) {
		             return (-5);
		  }
			/*判断返回的消息包*/
			iReturn=ltMsgIsValidCode(ntohl(sMsgHead0.lCode)) ;
			if(iReturn==0){
				return (-6);
		  }  
			if(ntohl(sMsgHead0.lMsgId)!=ntohl(psMsgHead->lMsgId)){
				return (-7);
		  }      
			if(ntohl(sMsgHead0.comFlag)!=200){
				return -200;
		  }
		        
			sendP=sendP+sendLen;
			if(leaveLen>0){
				if(leaveLen<=LTMSGPKGONESIZE){
					sendLen=leaveLen;
					leaveLen=0;
				}else{
					sendLen=LTMSGPKGONESIZE;
					leaveLen=leaveLen-LTMSGPKGONESIZE;
				}	
			}else{
				sendLen=0;
				leaveLen=0;
			}
		}
	}
  return 0;
}




ltMsgHead * lt_TcpMsgRead(int iFd,int *errint)
{
    

    ltMsgHead sMsgHead0; 
    ltMsgHead *psMsgHead0;   
    long iReturn;
    /*初始化*/
    
	/*读消息头*/
	iReturn=lt_saferead(iFd,(char *)&sMsgHead0,sizeof(ltMsgHead));
	if(iReturn != sizeof(ltMsgHead)) {
             	*errint=-1;
							return NULL;
        }
	/*判断读到的消息包*/
	iReturn=ltMsgIsValidCode(ntohl(sMsgHead0.lCode)) ;
	if(iReturn==0){
					sMsgHead0.comFlag=htonl(12);
       		iReturn=lt_safewrite(iFd,(char *)&sMsgHead0,sizeof(ltMsgHead));  
       		if(iReturn!=sizeof(ltMsgHead)){
								*errint=-2;
            		return NULL;
        	}
							*errint=-3;
            	return NULL;
        }else if(ntohl(sMsgHead0.lBytes)>LTCOMMAXTCPBUF){
					sMsgHead0.comFlag=htonl(11);	
					/*发送反馈消息头*/
       		iReturn=lt_safewrite(iFd,(char *)&sMsgHead0,sizeof(ltMsgHead));  
       		if(iReturn!=sizeof(ltMsgHead)){
            		*errint=-2;
            		return NULL;
        	}
							*errint=-4;
            	return NULL;		
	}else{
		sMsgHead0.comFlag=htonl(100);
		psMsgHead0=(ltMsgHead *)malloc(ntohl(sMsgHead0.lBytes));

		if(psMsgHead0==NULL){
			*errint=-5;
            		return NULL;	
		}
		/*发送反馈消息头*/
		memcpy((char *)psMsgHead0,(char *)&sMsgHead0,sizeof(ltMsgHead));
    iReturn=lt_safewrite(iFd,(char *)&sMsgHead0,sizeof(ltMsgHead)); 
    if(iReturn!=sizeof(ltMsgHead)){
								free(psMsgHead0);
            		*errint=-2;
            		return NULL;
     }
	}

	if(ntohl(psMsgHead0->lBytes)>sizeof(ltMsgHead)){	
		
		/*循环读消息体，每4096个为单位*/
		char *revP; /*接收msg包的当前位置*/
		int  revLen; /*本次接收的长度*/
		int  leaveLen; 	/*剩余全部接收的长度*/	

		revP=(char *)psMsgHead0+sizeof(ltMsgHead);
		leaveLen=ntohl(psMsgHead0->lBytes)-sizeof(ltMsgHead);
		if(leaveLen<=LTMSGPKGONESIZE ){
			revLen=leaveLen;
			leaveLen=0;
		}else{
			revLen=LTMSGPKGONESIZE;
			leaveLen=leaveLen-LTMSGPKGONESIZE;
		}		

		/*接收消息体*/
		while(revLen>0){
			iReturn=lt_saferead(iFd,(char *)revP,revLen);
			if( iReturn != revLen ) {
			        sMsgHead0.comFlag=htonl(12);
			    		iReturn=lt_safewrite(iFd,(char *)&sMsgHead0,sizeof(ltMsgHead));  
		       		if(iReturn!=sizeof(ltMsgHead)){
										free(psMsgHead0);
		            		*errint=-2;
		            		return NULL;
		        	}
							free(psMsgHead0);
							*errint=-6;
							return NULL;
		  }else{
		        	sMsgHead0.comFlag=htonl(200);
							/*发送反馈消息头*/
			       	iReturn=lt_safewrite(iFd,(char *)&sMsgHead0,sizeof(ltMsgHead));  
			       	if(iReturn!=sizeof(ltMsgHead)){
								free(psMsgHead0);
				        *errint=-2;
				        return NULL;
			        }
			}
			revP=revP+revLen;
			if(leaveLen>0){
				if(leaveLen<=LTMSGPKGONESIZE ){
					revLen=leaveLen;
					leaveLen=0;
				}else{
					revLen=LTMSGPKGONESIZE;
					leaveLen=leaveLen-LTMSGPKGONESIZE;
				}	
			}else{
				revLen=0;
				leaveLen=0;
			}
		}
	
	}
	*errint=0;
	ltMsgNtoh(psMsgHead0);
	psMsgHead0->lMaxBytes=psMsgHead0->lBytes;
	return psMsgHead0;
}




int lt_TcpResponse(int iSock,ltMsgHead *psMsgHead0,short nVarNum,...)
{
    char *pVar;
    char *pValue;
    short nType,nValue;
    long  lValue,lMsgCode,lBytes;
    register int i,iReturn;
    ltMsgHead *psMsgHead;
    va_list ap;
    lMsgCode = psMsgHead0->lCode ;
    va_start(ap, nVarNum);
    psMsgHead = ltMsgInit(lMsgCode,psMsgHead0->lFunCode,psMsgHead0->lMsgId,512);
    if(psMsgHead == (ltMsgHead *)NULL){ 
        return (-1);
    }
    psMsgHead->sid=psMsgHead0->sid;
    psMsgHead->msgpktype=psMsgHead0->msgpktype;

    for(i=0;i<nVarNum;i++) {
        pVar = va_arg(ap,char *);
        nType = va_arg(ap,int);
        switch (nType) {
            case LT_TYPE_SHORT:
                nValue = va_arg(ap,int);
                ltMsgAdd_n(&psMsgHead,pVar,nValue);
                break;
            case LT_TYPE_LONG:
                lValue = va_arg(ap,long);
                ltMsgAdd_l(&psMsgHead,pVar,lValue);
                break;
            case LT_TYPE_STRING:
                pValue = va_arg(ap,char *);
                ltMsgAdd_s(&psMsgHead,pVar,pValue);
                break;
            case LT_TYPE_STRUCT:
                pValue = va_arg(ap,char *);
                lBytes = va_arg(ap,long);
                ltMsgAdd_v(&psMsgHead,pVar,pValue,lBytes);
                break;

            default:
                pValue = va_arg(ap,char *);
                break;
                
        }
    }
    va_end(ap);
    iReturn = lt_TcpMsgSend(iSock,psMsgHead);
    ltMsgFree(psMsgHead);
    return iReturn;
}


int lt_TcpDesResponse(int iSock,ltMsgHead *psMsgHead0,char *pKey,short nVarNum,...)
{
    char *pVar;
    char *pValue;
    short nType,nValue;
    long  lValue,lMsgCode,lBytes;
    register int i,iReturn;
    ltMsgHead *psMsgHead;
    va_list ap;
    lMsgCode = psMsgHead0->lCode ;
    va_start(ap, nVarNum);
    psMsgHead = ltMsgInit(lMsgCode,psMsgHead0->lFunCode,psMsgHead0->lMsgId,512);
    if(psMsgHead == (ltMsgHead *)NULL){ 
        return (-1);
    }
    psMsgHead->sid=psMsgHead0->sid;
    psMsgHead->msgpktype=psMsgHead0->msgpktype;

    for(i=0;i<nVarNum;i++) {
        pVar = va_arg(ap,char *);
        nType = va_arg(ap,int);
        switch (nType) {
            case LT_TYPE_SHORT:
                nValue = va_arg(ap,int);
                ltMsgAdd_n(&psMsgHead,pVar,nValue);
                break;
            case LT_TYPE_LONG:
                lValue = va_arg(ap,long);
                ltMsgAdd_l(&psMsgHead,pVar,lValue);
                break;
            case LT_TYPE_STRING:
                pValue = va_arg(ap,char *);
                ltMsgAdd_s(&psMsgHead,pVar,pValue);
                break;
            case LT_TYPE_STRUCT:
                pValue = va_arg(ap,char *);
                lBytes = va_arg(ap,long);
                ltMsgAdd_v(&psMsgHead,pVar,pValue,lBytes);
                break;

            default:
                pValue = va_arg(ap,char *);
                break;
                
        }
    }
    va_end(ap);
    iReturn = lt_TcpMsgDesSend(iSock,psMsgHead,pKey);
    ltMsgFree(psMsgHead);
    return iReturn;
}


int lt_TcpMsgDesSend(int iSock,ltMsgHead *psMsgHead,char *pKey)
{
    int iReturn;
    iReturn=ltMsgDesEncrypt(&psMsgHead,pKey);
    if(iReturn<0){
    	return -1;
    }
    return lt_TcpMsgSend(iSock,psMsgHead);
}


ltMsgHead *lt_TcpMsgDesRead(int iSock,int *errint,char *pKey)
{

     ltMsgHead *psMsgHead;
		 psMsgHead = (ltMsgHead *)lt_TcpMsgRead(iSock,errint);
		 if(psMsgHead == NULL) {
					return NULL;
		 }
		 if(ltMsgDesDecrypt(&psMsgHead,pKey)<0){
		 	*errint=-1;
    	return NULL;
    }
    return psMsgHead;

}  



int lt_TcpSCResponse(int iSock,ltMsgHead *psMsgHead0,short nVarNum,...)
{
    char *pVar;
    char *pValue;
    short nType,nValue;
    long  lValue,lMsgCode,lBytes;
    register int i,iReturn;
    ltMsgHead *psMsgHead;
    va_list ap;
    lMsgCode = psMsgHead0->lCode ;
    va_start(ap, nVarNum);
    psMsgHead = ltMsgInit(lMsgCode,psMsgHead0->lFunCode,psMsgHead0->lMsgId,512);
    if(psMsgHead == (ltMsgHead *)NULL){ 
        return (-1);
    }
    psMsgHead->sid=psMsgHead0->sid;
    psMsgHead->msgpktype=psMsgHead0->msgpktype;

    for(i=0;i<nVarNum;i++) {
        pVar = va_arg(ap,char *);
        nType = va_arg(ap,int);
        switch (nType) {
            case LT_TYPE_SHORT:
                nValue = va_arg(ap,int);
                ltMsgAdd_n(&psMsgHead,pVar,nValue);
                break;
            case LT_TYPE_LONG:
                lValue = va_arg(ap,long);
                ltMsgAdd_l(&psMsgHead,pVar,lValue);
                break;
            case LT_TYPE_STRING:
                pValue = va_arg(ap,char *);
                ltMsgAdd_s(&psMsgHead,pVar,pValue);
                break;
            case LT_TYPE_STRUCT:
                pValue = va_arg(ap,char *);
                lBytes = va_arg(ap,long);
                ltMsgAdd_v(&psMsgHead,pVar,pValue,lBytes);
                break;

            default:
                pValue = va_arg(ap,char *);
                break;
                
        }
    }
    va_end(ap);
    iReturn = lt_TcpMsgSCSend(iSock,psMsgHead);
    ltMsgFree(psMsgHead);
    return iReturn;
}


int lt_TcpMsgSCSend(int iSock,ltMsgHead *psMsgHead)
{
    int iReturn;
    iReturn=ltMsgSEncrypt(psMsgHead);
    if(iReturn<0){
    	return -1;
    }
    return lt_TcpMsgSend(iSock,psMsgHead);
}


ltMsgHead *lt_TcpMsgSCRead(int iSock,int *errint)
{

     ltMsgHead *psMsgHead;
		 psMsgHead = (ltMsgHead *)lt_TcpMsgRead(iSock,errint);
		 if(psMsgHead == NULL) {
					return NULL;
		 }
		 if(ltMsgSDecrypt(psMsgHead)<0){
		 	*errint=-300;
    	return NULL;
    }
    return psMsgHead;

}  

