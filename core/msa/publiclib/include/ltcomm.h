#ifndef __LTOCOMDEF__
#define __LTOCOMDEF__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdarg.h>

#include "ltmsg.h"


#define LT_COM_MAXBUFFER    4096

#define LT_COM_TCPTIMEOUT   900

#define LT_LISTENQ 100




int  lt_tcpconnect(const char *ipHost,short nPort);
/*Æô¶¯tcp server*/
int lt_tcpserver(uint16 localport, int iQlen); 

int lt_timeoutread(int t, int fd, char *buf, int len);
int lt_timeoutwrite(int t,int fd,char *buf,int len) ;
int lt_saferead(int fd, char *buf, int len) ;
int lt_safewrite(int fd, char *buf, int len); 
int lt_TcpMsgDesSend(int iSock,ltMsgHead *psMsgHead,char *pKey);


int lt_SetMaxTcpBuf(long lMaxTcpBuf);
int lt_TcpMsgSend(int iSock,ltMsgHead *psMsgHead);
ltMsgHead * lt_TcpMsgRead(int iFd,int *errint);
int lt_TcpResponse(int iSock,ltMsgHead *psMsgHead0,short nVarNum,...);
ltMsgHead *lt_TcpMsgDesRead(int iSock,int *errint,char *pKey);
int lt_TcpDesResponse(int iSock,ltMsgHead *psMsgHead0,char *pKey,short nVarNum,...);



int lt_TcpMsgSCSend(int iSock,ltMsgHead *psMsgHead);
int lt_TcpSCResponse(int iSock,ltMsgHead *psMsgHead0,short nVarNum,...);
ltMsgHead *lt_TcpMsgSCRead(int iSock,int *errint);

#endif

