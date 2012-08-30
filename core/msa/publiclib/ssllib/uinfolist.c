/* A simple HTTPS client

   It connects to the server, makes an HTTP
   request and waits for the response
*/
#include "ssllib.h"


static char *REQUEST_TEMPLATE=
   "GET /cgi-bin/updatecgi?gnmkey=%s+gnmver=%s HTTP/1.0\r\nUser-Agent:"
   "GNM\r\nHost: %s:%d\r\n\r\n";

static char *host=HOST;
static int port=PORT;
static int require_server_auth=1;

static int http_request(ssl)
  SSL *ssl;
{
    char *request=0;
    char buf[BUFSIZZ];
    int r;
    int len, request_len;
    
    char gnmkey[24];
    char gnmver[64];
    FILE *fp;
    char *gnmverp;
    char *pppp;
    int  haveprint;

    memset(gnmkey,0,sizeof(gnmkey));
    fp=fopen("/etc/nsmkey","r");
    if(fp==NULL){
			err_exit("can't get serial code\n");
			return 0; 	
    }
    fgets(gnmkey,sizeof(gnmkey),fp);
    gnmkey[19]='\0';
    fclose(fp);
    
    memset(gnmver,0,sizeof(gnmver));
    fp=fopen("/etc/gnm/conf/system/version","r");
    if(fp==NULL) {
        err_exit("Error, Can't open the version file \n");
        return (-1);
    }
    fgets(gnmver,64,fp);
    gnmverp=gnmver;
    gnmverp=gnmverp+8;
    while(gnmverp) {
        if(*gnmverp==' ' || *gnmverp=='\r' || *gnmverp=='\n' ){
        	*gnmverp='\0';
        	break;
        }
        gnmverp++;
    }
    gnmverp=gnmver;
    gnmverp=gnmverp+8;
    
    /* Now construct our HTTP request */
    request_len=strlen(REQUEST_TEMPLATE)+ strlen(gnmkey) + strlen(gnmverp) +
      strlen(host)+6;
    if(!(request=(char *)malloc(request_len)))
      err_exit("Couldn't allocate request");
    snprintf(request,request_len,REQUEST_TEMPLATE,
      gnmkey,gnmverp,host,port);

    /* Find the exact request_len */
    request_len=strlen(request);

    r=SSL_write(ssl,request,request_len);
    switch(SSL_get_error(ssl,r)){      
      case SSL_ERROR_NONE:
        if(request_len!=r)
          err_exit("Incomplete write!");
        break;
        default:
          berr_exit("SSL write problem");
    }
    
    /* Now read the server's response, assuming
       that it's terminated by a close */
    haveprint=0;
    while(1){
      r=SSL_read(ssl,buf,BUFSIZZ);
      switch(SSL_get_error(ssl,r)){
        case SSL_ERROR_NONE:
          len=r;
          break;
        case SSL_ERROR_ZERO_RETURN:
          goto shutdown;
        case SSL_ERROR_SYSCALL:
          goto done;
        default:
          berr_exit("SSL read problem");
      }
      if(haveprint==0){
		      pppp=strstr(buf,"\r\n\r\n");
		      if(pppp){
		      	haveprint=1;
		      	fwrite(pppp+4,1,len-(pppp+4-buf),stdout);
		      }
		  }else{
		    fwrite(buf,1,len,stdout);
		  }
   }
    
  shutdown:
    r=SSL_shutdown(ssl);
    switch(r){
      case 1:
        break; /* Success */
      case 0:
      case -1:
      default:
        berr_exit("Shutdown failed");
    }
    
  done:
    SSL_free(ssl);
    free(request);
    return(0);
}
    
int main(argc,argv)
  int argc;
  char **argv;
  {
    SSL_CTX *ctx;
    SSL *ssl;
    BIO *sbio;
    int sock;
    extern char *optarg;

    host="www.baochuang.com.cn";
    port=443;
    require_server_auth=0;
    /* Build our SSL context*/
    ctx=initialize_ctx(KEYFILE,PASSWORD);

    /* Connect the TCP socket*/
    sock=ssltcp_connect(host,port);

    /* Connect the SSL socket */
    ssl=SSL_new(ctx);
    sbio=BIO_new_socket(sock,BIO_NOCLOSE);
    SSL_set_bio(ssl,sbio,sbio);

    if(SSL_connect(ssl)<=0)
      berr_exit("SSL connect error");
    if(require_server_auth)
      check_cert(ssl,host);
 
    /* Now make our HTTP request */
    http_request(ssl);

    /* Shutdown the socket */
    destroy_ctx(ctx);
    close(sock);

    exit(0);
  }

