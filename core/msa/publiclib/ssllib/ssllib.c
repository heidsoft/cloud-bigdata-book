#include "ssllib.h"
#include "ltmsg.h"
#include <openssl/err.h>


BIO *bio_err=0;
static char *pass;
static int password_cb(char *buf,int num,
  int rwflag,void *userdata);
static void sigpipe_handle(int x);

/* A simple error and exit routine*/
int err_exit(string)
  char *string;
  {
    fprintf(stderr,"%s\n",string);
    exit(0);
  }

/* Print SSL errors and exit*/
int berr_exit(string)
  char *string;
  {
    BIO_printf(bio_err,"%s\n",string);
    ERR_print_errors(bio_err);
    exit(0);
  }

/*The password code is not thread safe*/
static int password_cb(char *buf,int num,
  int rwflag,void *userdata)
  {
    if(num<strlen(pass)+1)
      return(0);

    strcpy(buf,pass);
    return(strlen(pass));
  }

static void sigpipe_handle(int x){
}

SSL_CTX *initialize_ctx(keyfile,password)
  char *keyfile;
  char *password;
  {
    SSL_METHOD *meth;
    SSL_CTX *ctx;
    
    if(!bio_err){
      /* Global system initialization*/
      SSL_library_init();
      SSL_load_error_strings();
      
      /* An error write context */
      bio_err=BIO_new_fp(stderr,BIO_NOCLOSE);
    }

    /* Set up a SIGPIPE handler */
    signal(SIGPIPE,sigpipe_handle);
    
    /* Create our context*/
    meth=SSLv23_method();
    ctx=SSL_CTX_new(meth);

    /* Load our keys and certificates*/
    if(!(SSL_CTX_use_certificate_chain_file(ctx,
      keyfile)))
      berr_exit("Can't read certificate file");

    pass=password;
    SSL_CTX_set_default_passwd_cb(ctx,
      password_cb);
    if(!(SSL_CTX_use_PrivateKey_file(ctx,
      keyfile,SSL_FILETYPE_PEM)))
      berr_exit("Can't read key file");

    /* Load the CAs we trust*/
    if(!(SSL_CTX_load_verify_locations(ctx,
      CA_LIST,0)))
      berr_exit("Can't read CA list");
#if (OPENSSL_VERSION_NUMBER < 0x00905100L)
    SSL_CTX_set_verify_depth(ctx,1);
#endif
    
    return ctx;
  }
     
void destroy_ctx(ctx)
  SSL_CTX *ctx;
  {
    SSL_CTX_free(ctx);
  }


/* Read from the keyboard and write to the server
   Read from the server and write to the keyboard

   we use select() to multiplex
*/
void read_write(ssl,sock)
  SSL *ssl;
  {
    int width;
    int r,c2sl=0,c2s_offset=0;
    int read_blocked_on_write=0,write_blocked_on_read=0,read_blocked=0;
    fd_set readfds,writefds;
    int shutdown_wait=0;
    char c2s[BUFSIZZ],s2c[BUFSIZZ];
    int ofcmode;
    
    /*First we make the socket nonblocking*/
    ofcmode=fcntl(sock,F_GETFL,0);
    ofcmode|=O_NDELAY;
    if(fcntl(sock,F_SETFL,ofcmode))
      err_exit("Couldn't make socket nonblocking");
    

    width=sock+1;
    
    while(1){
      FD_ZERO(&readfds);
      FD_ZERO(&writefds);

      FD_SET(sock,&readfds);

      /* If we're waiting for a read on the socket don't
         try to write to the server */
      if(!write_blocked_on_read){
        /* If we have data in the write queue don't try to
           read from stdin */
        if(c2sl || read_blocked_on_write)
          FD_SET(sock,&writefds);
        else
          FD_SET(fileno(stdin),&readfds);
      }
      
      r=select(width,&readfds,&writefds,0,0);
      if(r==0)
        continue;

      /* Now check if there's data to read */
      if((FD_ISSET(sock,&readfds) && !write_blocked_on_read) ||
        (read_blocked_on_write && FD_ISSET(sock,&writefds))){
        do {
          read_blocked_on_write=0;
          read_blocked=0;
          
          r=SSL_read(ssl,s2c,BUFSIZZ);
          
          switch(SSL_get_error(ssl,r)){
            case SSL_ERROR_NONE:
              /* Note: this call could block, which blocks the
                 entire application. It's arguable this is the
                 right behavior since this is essentially a terminal
                 client. However, in some other applications you
                 would have to prevent this condition */
              fwrite(s2c,1,r,stdout);
              break;
            case SSL_ERROR_ZERO_RETURN:
              /* End of data */
              if(!shutdown_wait)
                SSL_shutdown(ssl);
              goto end;
              break;
            case SSL_ERROR_WANT_READ:
              read_blocked=1;
              break;
              
              /* We get a WANT_WRITE if we're
                 trying to rehandshake and we block on
                 a write during that rehandshake.

                 We need to wait on the socket to be 
                 writeable but reinitiate the read
                 when it is */
            case SSL_ERROR_WANT_WRITE:
              read_blocked_on_write=1;
              break;
            default:
              berr_exit("SSL read problem");
          }

          /* We need a check for read_blocked here because
             SSL_pending() doesn't work properly during the
             handshake. This check prevents a busy-wait
             loop around SSL_read() */
        } while (SSL_pending(ssl) && !read_blocked);
      }
      
      /* Check for input on the console*/
      if(FD_ISSET(fileno(stdin),&readfds)){
        c2sl=read(fileno(stdin),c2s,BUFSIZZ);
        if(c2sl==0){
          shutdown_wait=1;
          if(SSL_shutdown(ssl))
            return;
        }
        c2s_offset=0;
      }

      /* If the socket is writeable... */
      if((FD_ISSET(sock,&writefds) && c2sl) ||
        (write_blocked_on_read && FD_ISSET(sock,&readfds))) {
        write_blocked_on_read=0;

        /* Try to write */
        r=SSL_write(ssl,c2s+c2s_offset,c2sl);
          
        switch(SSL_get_error(ssl,r)){
          /* We wrote something*/
          case SSL_ERROR_NONE:
            c2sl-=r;
            c2s_offset+=r;
            break;
              
            /* We would have blocked */
          case SSL_ERROR_WANT_WRITE:
            break;

            /* We get a WANT_READ if we're
               trying to rehandshake and we block on
               write during the current connection.
               
               We need to wait on the socket to be readable
               but reinitiate our write when it is */
          case SSL_ERROR_WANT_READ:
            write_blocked_on_read=1;
            break;
              
              /* Some other error */
          default:	      
            berr_exit("SSL write problem");
        }
      }
    }
      
  end:
    SSL_free(ssl);
    close(sock);
    return;
  }


int ssltcp_connect(host,port)
  char *host;
  int port;
  {
    struct hostent *hp;
    struct sockaddr_in addr;
    int sock;
    
    if(!(hp=gethostbyname(host)))
      berr_exit("Couldn't resolve host");
    memset(&addr,0,sizeof(addr));
    addr.sin_addr=*(struct in_addr*)
      hp->h_addr_list[0];
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);

    if((sock=socket(AF_INET,SOCK_STREAM,
      IPPROTO_TCP))<0)
      err_exit("Couldn't create socket");
    if(connect(sock,(struct sockaddr *)&addr,
      sizeof(addr))<0)
      err_exit("Couldn't connect socket");
    
    return sock;
  }

/* Check that the common name matches the
   host name*/
void check_cert(ssl,host)
  SSL *ssl;
  char *host;
  {
    X509 *peer;
    char peer_CN[256];
    
    if(SSL_get_verify_result(ssl)!=X509_V_OK)
      berr_exit("Certificate doesn't verify");

    /*Check the cert chain. The chain length
      is automatically checked by OpenSSL when
      we set the verify depth in the ctx */

    /*Check the common name*/
    peer=SSL_get_peer_certificate(ssl);
    X509_NAME_get_text_by_NID
      (X509_get_subject_name(peer),
      NID_commonName, peer_CN, 256);
    if(strcasecmp(peer_CN,host))
    err_exit
      ("Common name doesn't match host name");
  }

//int msaHttpMsgInit(msahttpBuf *psHttpMsgHead, char *caFunName,char *caHost,char *caUrl,unsigned long  lMaxBytes)
//{
//    char         *ppp;
//    char         FunBuffer[255];
//    
//    if(lMaxBytes<256){
//    	lMaxBytes=256;
//    }
//    memset((char *)psHttpMsgHead,0,sizeof(msahttpBuf));
//    
//    if( (ppp = (char *)malloc(lMaxBytes)) == NULL) {
//        return -1;
//    }
//    
//    snprintf(psHttpMsgHead->caHost,64,"%s",caHost);
//    snprintf(psHttpMsgHead->caUrl,128,"%s",caUrl);
//    snprintf(psHttpMsgHead->caFun,64,"%s",caFun);
//    snprintf(psHttpMsgHead->caboundary,64,"----baochuang%u%d",time(0),getpid());
//    psHttpMsgHead->iLen=0;
//    psHttpMsgHead->iMaxBytes=iMaxBytes;
//    psHttpMsgHead->pBuffer=ppp;
//    
//    sprintf(FunBuffer,"--%s\r\nContent-Disposition: form-data; name=\"Fun\"\r\n\r\n%s\r\n",
//    					psHttpMsgHead->caboundary,psHttpMsgHead->caFun);
//    
//    sprintf(psHttpMsgHead->pBuffer,"%s",FunBuffer);
//    psHttpMsgHead->iLen=strlen(FunBuffer);
//
//    return 0;
//}
//
//
//
//int msaHttpMsgAdd_s(msahttpBuf *psHttpMsgHead,char *pVarName,char *pVarValue)
//{
//    unsigned int  lLen;
//    char *ppp;
//    char tmpBuffer[255];
//    
//    ppp=psHttpMsgHead->pBuffer;
//    lLen = strlen(pVarValue) + 256;
//    
//    if(psHttpMsgHead->lMaxBytes - psHttpMsgHead->iLen < lLen ) {
//        ppp = (char *)realloc(ppp, psHttpMsgHead->iLen + lLen);
//        if(ppp == (char *)NULL) {
//            return (-1);
//        }
//        else {
//           psHttpMsgHead->pBuffer=ppp;
//           psHttpMsgHead->lMaxBytes = psHttpMsgHead->iLen + lLen;
//        }
//    }
//    ppp = (char *)ppp + psHttpMsgHead->iLen;
//    
//    sprintf(tmpBuffer,"--%s\r\nContent-Disposition: form-data; name=\"%s\"\r\n\r\n",psHttpMsgHead->caboundary,pVarName);
//    sprintf(ppp,"%s%s\r\n",tmpBuffer,pVarValue);
//    psHttpMsgHead->iLen=psHttpMsgHead->iLen+strlen(tmpBuffer)+strlen(pVarValue)+2;
//    
//    return 0;
//}
//
//int msaHttpMsgAdd_v(msahttpBuf *psHttpMsgHead,char *pVarName,char *pVarValue,unsigned long myLen)
//{
//    unsigned int  lLen,vLen;
//    char *ppp;
//    char tmpBuffer[255];
//    
//    ppp=psHttpMsgHead->pBuffer;
//    lLen = myLen + 256;
//    
//    if(psHttpMsgHead->lMaxBytes - psHttpMsgHead->iLen < lLen ) {
//        ppp = (char *)realloc(ppp, psHttpMsgHead->iLen + lLen);
//        if(ppp == (char *)NULL) {
//            return (-1);
//        }
//        else {
//           psHttpMsgHead->pBuffer=ppp;
//           psHttpMsgHead->lMaxBytes = psHttpMsgHead->iLen + lLen;
//        }
//    }
//    ppp = (char *)ppp + psHttpMsgHead->iLen;
//    
//    sprintf(tmpBuffer,"--%s\r\nContent-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\nContent-Type: application/octet-stream\r\n\r\n",psHttpMsgHead->caboundary,pVarName,pVarName);
//    vLen=strlen(tmpBuffer);
//    memcpy(ppp,tmpBuffer,vLen);
//    memcpy(ppp+vLen,pVarValue,myLen);
//    memcpy(ppp+vLen+myLen,"\r\n",2);
//    psHttpMsgHead->iLen=psHttpMsgHead->iLen+vLen+myLen+2;
//    return 0;
//}
//
//
//ltMsgHead *msaHttpMsgCom(msahttpBuf *psHttpMsgHead,char *host,int port,int *err)
//{
//    
//    ltMsgHead *psMsgHead0; 
//    struct hostent *hp;
//    struct sockaddr_in addr;
//    int    sock;
//    char   htmlHead[513];
//    int    icount;
//    char   *ppp,*ppp1,*ppp2;
//    int    remsglen;
//    char   caMsgLen[20];
//    
//    
//    *err=0;
//    psMsgHead0=NULL;
//    if(!(hp=gethostbyname(host))){
//      *err=1;
//      return psMsgHead0;
//    }
//    memset(&addr,0,sizeof(addr));
//    addr.sin_addr=*(struct in_addr*)hp->h_addr_list[0];
//    addr.sin_family=AF_INET;
//    addr.sin_port=htons(port);
//
//    if((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
//      //printf("Couldn't create socket");
//      *err=2;
//      return psMsgHead0;
//    }
//    if(connect(sock,(struct sockaddr *)&addr,sizeof(addr))<0){
//      //printf("Couldn't connect socket");
//      *err=3;
//      return psMsgHead0;
//    }
//    sprintf(htmlHead,"POST %s HTTP/1.1\r\nAccept: text/*\r\n\
//Content-Type: multipart/form-data; boundary=%s\r\n\
//User-Agent: BAOCHUANGAPP\r\n\
//Host: %s\r\n\
//Content-Length: %u\r\n\
//Connection: Keep-Alive\r\n\
//Cache-Control: no-cache\r\n\r\n",
//psHttpMsgHead->caUrl,psHttpMsgHead->caboundary,psHttpMsgHead->caHost,psHttpMsgHead->iLen+strlen(psHttpMsgHead->caboundary)+4);
//    icount=write(sock,htmlHead,strlen(htmlHead));
//    icount=psHttpMsgHead->iLen;
//    ppp=psHttpMsgHead->pBuffer;
//    if(!ppp){
//    	//close(sock);
//    	*err=4;
//    	return psMsgHead0;
//    }
//    while(icount > 0) {
//    	     if(icount>512){
//	           write(sock,ppp,512);
//	           icount=icount-512;
//	           ppp=ppp+512;
//	         }else{
//	           write(sock,ppp,icount);
//	           break;
//	         }
//	  }
//	  sprintf(htmlHead,"--%s--",psHttpMsgHead->caboundary);
//	  icount=write(sock,htmlHead,strlen(htmlHead));
//    //close(sock);
//
//    memset(htmlHead,0,sizeof(htmlHead));
//    icount=read(sock,htmlHead,512);
//    ppp=strstr(htmlHead,"\r\n\r\n");
//		if(!ppp){
//		    *err=5;
//    	  return psMsgHead0;
//		}
//    ppp1=strstr(htmlHead,"Content-Length:");   
//    if(!ppp1){
//		    *err=6;
//    	  return psMsgHead0;
//		}
//		if(*ppp1==' '){
//			ppp1=ppp1+1;
//		}
//		ppp1=ppp1+1;
//		memset(caMsgLen,0,20);
//		ppp2=caMsgLen;
//    for(iii=0;iii<20;iii++){
//    	if(*ppp1=='\r' || *ppp1=='\n' ){  
//    			break;
//      }
//      *ppp2=*ppp1;
//      ppp1++;
//      ppp2++;
//    }
//    remsglen=atol(caMsgLen);
//    if(remsglen<0 || remsglen>4096000 ){
//    	  *err=7;
//    	  return psMsgHead0;
//    }
//    psMsgHead0=(ltMsgHead *)malloc(remsglen); 
//    if(!psMsgHead0){
//    	  *err=8;
//    	  return psMsgHead0;
//    }
//    ppp=(char *)psMsgHead0;
//    while(remsglen>0){
//      if(remsglen>512){
//      	icount=read(sock,ppp,512);
//      	if(icount<1){
//      		break;
//      	}
//      	remsglen=remsglen-512;
//      	ppp=ppp+512;
//      }else{
//      	icount=read(sock,ppp,remsglen);
//      	if(icount<1){
//      		break;
//      	}
//      }
//   }
//   return psMsgHead0;
//}


//ltMsgHead *msaMsgHttpCom(char *caUrl,ltMsgHead *psMsgHead,char *host,int port,int *err)
//{
//    
//    ltMsgHead *psMsgHead0; 
//    struct hostent *hp;
//    struct sockaddr_in addr;
//    int    sock;
//    char   htmlHead[513];
//    int    icount;
//    char   *ppp,*ppp1,*ppp2;
//    int    remsglen;
//    char   caMsgLen[20];
//    char   caboundary[64];
//    int    lMaxBytes;
//    char   tmpBuffer[255];
//    int    iii;
//    snprintf(caboundary,64,"----baochuang%u%d",time(0),getpid());
//    
//    sprintf(tmpBuffer,"--%s\r\nContent-Disposition: form-data; name=\"MSGBUF\"; filename=\"MSGBUF\"\r\nContent-Type: application/octet-stream\r\n\r\n",caboundary);
//    lMaxBytes=atol(tmpBuffer)+psMsgHead->lBytes+strlen(caboundary)+6;
//    
//    *err=0;
//    psMsgHead0=NULL;
//    if(!(hp=gethostbyname(host))){
//      *err=1;
//      return psMsgHead0;
//    }
//    memset(&addr,0,sizeof(addr));
//    addr.sin_addr=*(struct in_addr*)hp->h_addr_list[0];
//    addr.sin_family=AF_INET;
//    addr.sin_port=htons(port);
//
//    if((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
//      //printf("Couldn't create socket");
//      *err=2;
//      return psMsgHead0;
//    }
//    if(connect(sock,(struct sockaddr *)&addr,sizeof(addr))<0){
//      //printf("Couldn't connect socket");
//      *err=3;
//      return psMsgHead0;
//    }
//    sprintf(htmlHead,"POST %s HTTP/1.1\r\nAccept: text/*\r\n\
//Content-Type: multipart/form-data; boundary=%s\r\n\
//User-Agent: BAOCHUANGAPP\r\n\
//Host: %s\r\n\
//Content-Length: %u\r\n\
//Connection: Keep-Alive\r\n\
//Cache-Control: no-cache\r\n\r\n",
//caUrl,caboundary,host,lMaxBytes);
//    icount=write(sock,htmlHead,strlen(htmlHead));
//    icount=write(sock,tmpBuffer,strlen(tmpBuffer));
//    icount=psMsgHead->lBytes;
//    ppp=(char *)psMsgHead;
//    while(icount > 0) {
//    	     if(icount>512){
//	           write(sock,ppp,512);
//	           icount=icount-512;
//	           ppp=ppp+512;
//	         }else{
//	           write(sock,ppp,icount);
//	           break;
//	         }
//	  }
//	  sprintf(tmpBuffer,"\r\n--%s--",caboundary);
//	  icount=write(sock,tmpBuffer,strlen(tmpBuffer));
//    //close(sock);
//
//    memset(htmlHead,0,sizeof(htmlHead));
//    icount=read(sock,htmlHead,512);
//    ppp=strstr(htmlHead,"\r\n\r\n");
//		if(!ppp){
//		    *err=4;
//    	  return psMsgHead0;
//		}
//		ppp1=strstr(htmlHead,"Content-type: application/octet-stream");   
//    if(!ppp1){
//		    *err=5;
//    	  return psMsgHead0;
//		}
//    ppp1=strstr(htmlHead,"Content-Length:");   
//    if(!ppp1){
//		    *err=6;
//    	  return psMsgHead0;
//		}
//		if(*ppp1==' '){
//			ppp1=ppp1+1;
//		}
//		ppp1=ppp1+1;
//		memset(caMsgLen,0,20);
//		ppp2=caMsgLen;
//    for(iii=0;iii<20;iii++){
//    	if(*ppp1=='\r' || *ppp1=='\n' ){  
//    			break;
//      }
//      *ppp2=*ppp1;
//      ppp1++;
//      ppp2++;
//    }
//    remsglen=atol(caMsgLen);
//    if(remsglen<0 || remsglen>4096000 ){
//    	  *err=7;
//    	  return psMsgHead0;
//    }
//    psMsgHead0=(ltMsgHead *)malloc(remsglen); 
//    if(!psMsgHead0){
//    	  *err=8;
//    	  return psMsgHead0;
//    }
//    ppp=(char *)psMsgHead0;
//    while(remsglen>0){
//      if(remsglen>512){
//      	icount=read(sock,ppp,512);
//      	if(icount<1){
//      		break;
//      	}
//      	remsglen=remsglen-512;
//      	ppp=ppp+512;
//      }else{
//      	icount=read(sock,ppp,remsglen);
//      	if(icount<1){
//      		break;
//      	}
//      }
//   }
//   return psMsgHead0;
//}


ltMsgHead *msaMsgHttpCom(char *caUrl,ltMsgHead *psMsgHead,char *host,int port,int *err)
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
 
    lMaxBytes=psMsgHead->lBytes;
    
    *err=0;
    psMsgHead0=NULL;
    if(!(hp=gethostbyname(host))){
      *err=1;
      return psMsgHead0;
    }
    memset(&addr,0,sizeof(addr));
    addr.sin_addr=*(struct in_addr*)hp->h_addr_list[0];
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);

    if((sock=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP))<0){
      //printf("Couldn't create socket");
      *err=2;
      return psMsgHead0;
    }
    if(connect(sock,(struct sockaddr *)&addr,sizeof(addr))<0){
      //printf("Couldn't connect socket");
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
//printf("htmlHead:\r\n%s",htmlHead);
    icount=write(sock,htmlHead,strlen(htmlHead));
    icount=psMsgHead->lBytes;
    ppp=(char *)psMsgHead;
    while(icount > 0) {
    	     if(icount>512){
	           write(sock,ppp,512);
	           icount=icount-512;
	           ppp=ppp+512;
	         }else{
	           write(sock,ppp,icount);
	           break;
	         }
	  }
	  
    //close(sock);

    memset(htmlHead,0,sizeof(htmlHead));
    icount=read(sock,htmlHead,512);
    ppp=strstr(htmlHead,"\r\n\r\n");
		if(!ppp){
		    *err=4;
    	  return psMsgHead0;
		}
		ppp=ppp+4;
//printf("%s\r\n",htmlHead);
		ppp1=strstr(htmlHead,"Content-Type: application/octet-stream");   
    if(!ppp1){
    	  printf("%s\r\n",ppp+4);
		    *err=5;
    	  return psMsgHead0;
		}
    ppp1=strstr(htmlHead,"Content-Length: ");   
    if(!ppp1){
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
    	  *err=7;
    	  return psMsgHead0;
    }
//printf("remsglen:%d\r\n",remsglen);
    psMsgHead0=(ltMsgHead *)malloc(remsglen); 
    if(!psMsgHead0){
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
      	icount=lt_saferead(sock,ppp,512);
      	if(icount<512){
      		break;
      	}
      	remsglen=remsglen-512;
      	ppp=ppp+512;
      }else{
      	icount=lt_saferead(sock,ppp,remsglen);
      	break;
      }
   }
   return psMsgHead0;
}






//int msaHttpMsgFree(msahttpBuf *psHttpMsgHead,char *host,int port)
//{
//	psHttpMsgHead->iLen=0;
//	if(psHttpMsgHead->pBuffer){
//		free(psHttpMsgHead->pBuffer);
//	}
//	
//}


