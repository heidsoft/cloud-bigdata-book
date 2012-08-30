
#include <stdio.h>
#include <signal.h>
#include <syslog.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

#include "libxml/tree.h"
#include "libxml/parser.h"
#include "ltpublic.h" 
#include "ltmsg.h"
#include "ltcgi.h"

extern char version[];
char msntauth_version[] = "Msntauth v2.0.3 (C) 2 Sep 2001 Stellar-X Antonino Iannella.\nModified by the Squid HTTP Proxy team 26 Jun 2002";

/* Main program for simple authentication.
 * Reads the denied user file. Sets alarm timer.
 * Scans and checks for Squid input, and attempts to validate the user.
 */

int
main(int argc, char **argv)
{
    char username[256];
    char password[256];
    char wstr[256];
    int err = 0;
    ltCgiConfInfo   sCgiConf; /* ÅäÖÃÐÅÏ¢   */
    ltCgiConfInfo   *psCgiConf; 
    
    openlog("msnt_auth", LOG_PID, LOG_USER);
    setbuf(stdout, NULL);
    
    memset((char *)&sCgiConf,0,sizeof(ltCgiConfInfo));
    sprintf(sCgiConf.upLoadPath,"/tmp/upload/");
    sCgiConf.lTimeOut = 60;
    sCgiConf.lMaxMsgPkg = 0;
    if( ltGetCgiValue("/app/ns/app/xp.conf",&sCgiConf)!=0){
			 syslog(LOG_WARNING, "webproxy nt auth error");
       return 1;
		}
		lt_SetMaxTcpBuf(sCgiConf.lMaxMsgPkg);
		psCgiConf=&sCgiConf;

    while (1) {
				int n;
				/* Read whole line from standard input. Terminate on break. */
				memset(wstr, '\0', sizeof(wstr));
				if (fgets(wstr, 255, stdin) == NULL)
				    break;
				/* ignore this line if we didn't get the end-of-line marker */
				if (NULL == strchr(wstr, '\n')) {
				    err = 1;
				    continue;
				}
				if (err) {
				    syslog(LOG_WARNING, "oversized message");
				    goto error;
				}

				/*
				 * extract username and password.
				 * XXX is sscanf() safe?
				 */
				username[0] = '\0';
				password[0] = '\0';
				n = sscanf(wstr, "%s %[^\n]", username, password);
				if (2 != n) {
				    puts("ERR");
				    continue;
				}
				/* Check for invalid or blank entries */
				if ((username[0] == '\0') || (password[0] == '\0')) {
				    puts("ERR");
				    continue;
				}
			
				rfc1738_unescape(username);
				rfc1738_unescape(password);

				/*
				 * Check if user is explicitly denied or allowed.
				 * If user passes both checks, they can be authenticated.
				 */
			MsgHead = ltMsgInit(psCgiConf->lMsgCode,0, ltStrGetId(),1024);
	    if (MsgHead == NULL ) {
	       puts("ERR");
				 continue;
	    }
	    lFunCode = (int)ltMd5Code("naswebproxycheck",strlen("naswebproxycheck"),"LT");
	    MsgHead->lFunCode = lFunCode;
	    ltMsgAdd_s(&MsgHead,"username",username);
	    ltMsgAdd_s(&MsgHead,"password",password);
	   
	    iSocket = ltCgiConnectlocal(psCgiConf);
	    if(iSocket == -1) {
	        
	        syslog(LOG_WARNING, "System is busy for key issue,try 30 second later!");
	        puts("ERR");
	        free(MsgHead);
				  continue;
	    }
	    else {

        iResult = lt_TcpMsgSend(iSocket,MsgHead);
				free(MsgHead);
        if(iResult == -1)
        {
          syslog(LOG_WARNING, "Message Send Error!!");
	        puts("ERR");
	        close(iSocket);
				  continue;
        }
        else {
        	  int readtime;
        	  readtime=0;
        	
            getMsgHead = lt_TcpMsgRead(iSocket,&errint);
            while(getMsgHead == NULL && errint !=0)
            {
               getMsgHead = lt_TcpMsgRead(iSocket,&errint);
               readtime++;
               
               sleep(4);
               if(readtime>=4){
               	break;
               }
	    			}	
            if(getMsgHead == NULL && errint !=0)
            {
                syslog(LOG_WARNING, "Message receive Error!!");
				        puts("ERR");
				        close(iSocket);
							  continue;
	    			}	
            else {
								p = ltMsgGetVar_s(getMsgHead,"lthtml");
								free(getMsgHead);
								if( p!=NULL){
									if(strstr(p,"Success"){
										puts("OK");
									}else{
									  syslog(LOG_INFO, "'%s' denied", username);
				    				puts("ERR");
								  }
								}else{
								   syslog(LOG_INFO, "'%s' denied", username);
				    			 puts("ERR");
							  }
					   }
          }
          close(iSocket);
       }

     }
				 

    return 0;
}
