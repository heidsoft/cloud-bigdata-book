
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "ltmsg.h"
#include "ltcomm.h"


int  lt_tcpconnect(const char *ipHost,short nPort){
    struct sockaddr_in  sSin;
    int iSock;
    memset(&sSin,0,sizeof(struct sockaddr_in));
    sSin.sin_family = AF_INET;
    sSin.sin_port = htons(nPort);
    sSin.sin_addr.s_addr = inet_addr(ipHost) ;
    if( (iSock=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == (-1)) {
        return (-1);
    }
    /* Connect the socket */
    if ( connect(iSock,(struct sockaddr *)&sSin,sizeof(struct sockaddr_in)) < 0) {
        close(iSock);
        return (-1);
    }
    return iSock;
}

int lt_tcpserver(uint16 localport, int iQlen)
/*
 * Arguments:
 *      qlen      - maximum server request queue length (Only in TCP */
{   
  int	iSock,iReturn;	/* socket descriptor and socket type	*/
  struct sockaddr_in  sSin; 
  int on=1;
  memset(&sSin,0,sizeof(struct sockaddr_in));
  iSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);;
  if (iSock == -1){
    return -1;
  }
  setsockopt(iSock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  sSin.sin_family = AF_INET;
  sSin.sin_port = htons(localport);
  sSin.sin_addr.s_addr = htonl(INADDR_ANY); 

  if (bind(iSock,(struct sockaddr *)&sSin, sizeof(struct sockaddr_in)) < 0) {
        close(iSock);
        return (-1);
   }
  if (listen(iSock,iQlen) == -1){
    	close(iSock);
	return -1;
  }
  //ndelay_off(iSock);

  return iSock;

}




lt_timeoutread(int t, int fd, char *buf, int len)
{
  fd_set rfds;
  struct timeval tv;

  tv.tv_sec = t;
  tv.tv_usec = 0;

  FD_ZERO(&rfds);
  FD_SET(fd,&rfds);

  if (select(fd + 1,&rfds,(fd_set *) 0,(fd_set *) 0,&tv) == -1) return -1;
  if (FD_ISSET(fd,&rfds)) return read(fd,buf,len);

  return -1;
}

int lt_saferead(int fd, char *buf, int len)
{
  int timeout = LT_COM_TCPTIMEOUT ;
  int r;
  r = lt_timeoutread(timeout,fd,buf,len);
  return r;
}

int lt_timeoutwrite(int t,int fd,char *buf,int len)
{
	
  fd_set wfds;
  struct timeval tv;
  tv.tv_sec = t;
  tv.tv_usec = 0;
  FD_ZERO(&wfds);
  FD_SET(fd,&wfds);
  if (select(fd + 1,(fd_set *) 0,&wfds,(fd_set *) 0,&tv) == -1) return -1;
  if (FD_ISSET(fd,&wfds)) return write(fd,buf,len);
  return -1;
  
}

int lt_safewrite(int fd, char *buf, int len)
{
  int timeout = LT_COM_TCPTIMEOUT ;
  int r;
  r = lt_timeoutwrite(timeout,fd,buf,len);
  return r;
}


