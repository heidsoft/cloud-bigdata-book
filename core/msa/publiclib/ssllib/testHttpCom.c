#include "ssllib.h"
#include "ltmsg.h"
#include <openssl/err.h>


ltMsgHead *msaMsgHttpsCom(char *caUrl,ltMsgHead *psMsgHead,char *host,int port,int *err)
{
    
    ltMsgHead *psMsgHead0; 
    struct hostent *hp;
    struct sockaddr_in addr;
    int    sock;
    char   htmlHead[513];
    int    icount;
    char   *ppp,*ppp1,*ppp2;
    int    remsglen;
    char   caMsgLen[20];

    int    lMaxBytes;

    int    iii;
    SSL_CTX *ctx;
    SSL *ssl;
    BIO *sbio;
 
    lMaxBytes=psMsgHead->lBytes;
    
    *err=0;
    psMsgHead0=NULL;
    
    ctx=initialize_ctx(KEYFILE,PASSWORD);
    
    
    if(!(hp=gethostbyname(host))){
      *err=1;
      return psMsgHead0;
    }
    memset(&addr,0,sizeof(addr));
    addr.sin_addr=*(struct in_addr*)hp->h_addr_list[0];
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);

    if((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
printf("Couldn't create socket");
      *err=2;
      return psMsgHead0;
    }
    if(connect(sock,(struct sockaddr *)&addr,sizeof(addr))<0){
printf("Couldn't connect socket");
      *err=3;
      return psMsgHead0;
    }

    ssl=SSL_new(ctx);
    sbio=BIO_new_socket(sock,BIO_NOCLOSE);
    SSL_set_bio(ssl,sbio,sbio);

    if(SSL_connect(ssl)<=0){
    	//close(sock);
 printf("SSL connect error");
        SSL_shutdown(ssl);
		    SSL_free(ssl);
		    destroy_ctx(ctx);
        *err=3;
    	  return psMsgHead0;
    }
    
    sprintf(htmlHead,"POST %s HTTP/1.1\r\nAccept: text/*\r\n\
Content-Type: application/octet-stream\r\n\
User-Agent: BAOCHUANGAPP\r\n\
Host: %s\r\n\
Content-Length: %u\r\n\
Connection: Keep-Alive\r\n\
Cache-Control: no-cache\r\n\r\n",
caUrl,host,lMaxBytes);
printf("htmlHead:\r\n%s",htmlHead);
    icount=SSL_write(ssl,htmlHead,strlen(htmlHead));
    icount=psMsgHead->lBytes;
    ppp=(char *)psMsgHead;
    while(icount > 0) {
    	     if(icount>512){
	           SSL_write(ssl,ppp,512);
	           icount=icount-512;
	           ppp=ppp+512;
	         }else{
	           SSL_write(ssl,ppp,icount);
	           break;
	         }
	  }
	  
    //close(sock);

    memset(htmlHead,0,sizeof(htmlHead));
    icount=SSL_read(ssl,htmlHead,512);
    ppp=strstr(htmlHead,"\r\n\r\n");
		if(!ppp){
			  SSL_shutdown(ssl);
		    SSL_free(ssl);
		    destroy_ctx(ctx);
		    *err=4;
    	  return psMsgHead0;
		}
		ppp=ppp+4;
//printf("%s\r\n",htmlHead);
		ppp1=strstr(htmlHead,"Content-Type: application/octet-stream");   
    if(!ppp1){
printf("%s\r\n",ppp+4);
    	  SSL_shutdown(ssl);
		    SSL_free(ssl);
		    destroy_ctx(ctx);
		    *err=5;
    	  return psMsgHead0;
		}
    ppp1=strstr(htmlHead,"Content-Length: ");   
    if(!ppp1){
    	  SSL_shutdown(ssl);
		    SSL_free(ssl);
		    destroy_ctx(ctx);
		    *err=6;
    	  return psMsgHead0;
		}
		
	  ppp1=ppp1+16;
		
		memset(caMsgLen,0,20);
		ppp2=caMsgLen;
    for(iii=0;iii<20;iii++){
    	if(*ppp1=='\r' || *ppp1=='\n' ){  
    			break;
      }
      *ppp2=*ppp1;
      ppp1++;
      ppp2++;
    }
    remsglen=atol(caMsgLen);
    if(remsglen<0 || remsglen>4096000 ){
    	  SSL_shutdown(ssl);
		    SSL_free(ssl);
		    destroy_ctx(ctx);
    	  *err=7;
    	  return psMsgHead0;
    }
//printf("remsglen:%d\r\n",remsglen);
    psMsgHead0=(ltMsgHead *)malloc(remsglen); 
    if(!psMsgHead0){
    	  SSL_shutdown(ssl);
		    SSL_free(ssl);
		    destroy_ctx(ctx);
    	  *err=8;
    	  return psMsgHead0;
    }
    ppp1=ppp;
    iii=icount-(ppp1-htmlHead);
    memcpy((void *)psMsgHead0,ppp1,iii );
    ppp=(char *)psMsgHead0;
    ppp=ppp+iii;
    remsglen=remsglen- iii;
    while(remsglen>0){
      if(remsglen>512){
      	icount=SSL_read(ssl,ppp,512);
      	if(icount<512){
      		break;
      	}
      	remsglen=remsglen-512;
      	ppp=ppp+512;
      }else{
      	icount=SSL_read(ssl,ppp,remsglen);
      	break;
      }
   }
   SSL_shutdown(ssl);
   SSL_free(ssl);
   destroy_ctx(ctx);
   return psMsgHead0;
}


int main(){

//  int  err;
//	ltMsgHead *MsgHead,*getMsgHead;
//	MsgHead = ltMsgInit(100, 0, 1000,4096);
//	if(MsgHead){
//	  MsgHead->lFunCode = (int)ltMd5Code("testreply",strlen("testreply"),"LT");
//	  ltMsgAdd_s(&MsgHead,"test","test1111");
//	  ltMsgAdd_s(&MsgHead,"222","22222222");
//	  ltMsgAdd_s(&MsgHead,"333","33333333");
//	  ltMsgPrintMsg(MsgHead);
//	  getMsgHead=msaMsgHttpCom("/cgi-bin/httpMsgProxy",MsgHead,"192.168.0.199",80,&err);
//	  if(getMsgHead){
//	    ltMsgPrintMsg(getMsgHead);
//	    free(getMsgHead);
//	  }
//	  free(MsgHead);
//	}
//	int  err;
//	ltMsgHead *MsgHead,*getMsgHead;
//	MsgHead = ltMsgInit(888888, 0, 0,4096);
//	if(MsgHead){
//	  MsgHead->lFunCode = (int)ltMd5Code("ltshowlogonpage",strlen("ltshowlogonpage"),"LT");
//	  ltMsgAdd_s(&MsgHead,"clientip","192.168.0.88");
//	  ltMsgPrintMsg(MsgHead);
//	  getMsgHead=msaMsgHttpCom("/cgi-bin/httpMsgProxy",MsgHead,"192.168.0.199",80,&err);
//	  if(getMsgHead){
//	    ltMsgPrintMsg(getMsgHead);
//	    free(getMsgHead);
//	  }
//	  free(MsgHead);
//	}
	
	int  err;
	ltMsgHead *MsgHead,*getMsgHead;
	MsgHead = ltMsgInit(888888, 0, 0,4096);
	if(MsgHead){
	  MsgHead->lFunCode = (int)ltMd5Code("ltshowlogonpage",strlen("ltshowlogonpage"),"LT");
	  ltMsgAdd_s(&MsgHead,"clientip","192.168.0.88");
	  ltMsgPrintMsg(MsgHead);
	  getMsgHead=msaMsgHttpsCom("/cgi-bin/httpMsgProxy",MsgHead,"192.168.0.199",443,&err);
	  if(getMsgHead){
	    ltMsgPrintMsg(getMsgHead);
	    free(getMsgHead);
	  }
	  free(MsgHead);
	}
	
}

