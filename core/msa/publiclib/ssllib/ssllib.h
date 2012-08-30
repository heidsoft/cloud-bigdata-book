#ifndef _client_h
#define _client_h

//typedef struct httpBuf_S {
//        char     caHost[64];
//        char     caUrl[128];
//        char     caFun[32];
//        char     caboundary[64];
//        unsigned int iLen;                  /* 当前字符串长度          */
//        unsigned int iMaxBytes;             /* 目前分配空间的最大长度  */
//        char     *pBuffer;                  /* 数据缓冲区              */
//} msahttpBuf;


#define KEYFILE "client.pem"
#define PASSWORD "password"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include <openssl/ssl.h>

#define CA_LIST "root.pem"
#define HOST	"localhost"
#define RANDOM  "random.pem"
#define PORT	4433
#define BUFSIZZ 1024

extern BIO *bio_err;
int berr_exit (char *string);
int err_exit(char *string);

SSL_CTX *initialize_ctx(char *keyfile, char *password);
void destroy_ctx(SSL_CTX *ctx);

#ifndef ALLOW_OLD_VERSIONS
#if (OPENSSL_VERSION_NUMBER < 0x00905100L)
#error "Must use OpenSSL 0.9.6 or later"
#endif
#endif

void read_write(SSL *ssl,int sock);
int ssltcp_connect(char *host,int port);
void check_cert(SSL *ssl,char *host);

//ltMsgHead *msaMsgHttpsCom(char *caUrl,ltMsgHead *psMsgHead,char *host,int port,int *err);
//
//ltMsgHead *msaMsgHttpCom(char *caUrl,ltMsgHead *psMsgHead,char *host,int port,int *err);

#endif

