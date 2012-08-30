#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <gdfontmb.h>
#include "msas.h"


/*±¨¸æ1-->4º¯Êı´¦Àí*/
int msasreportproc(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{	
	/*²ÎÊı¶¨Òå*/
	char caLabel[256];
	char caSdate[32],caEdate[32],caSdate1[32],caTime1[32],email[32],serve[32];
	char *reportname;
	char caTempDir[256];
	char caTempFile[256];
	long lTime;
 	unsigned int time1,time2,Max,reporttype;
		
 	char sqlBuf[1024],strBuf[1024];
	int srcfd,j,srcfd1,i;
	char caItem[16],rate[32];
	char caTemp[18192];
	char caTempDataDir[256];
	char caTempDataDir1[256];
 	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	ltDbHeadPtr dbPtr;
	ltTablePtr  tablePtr;
	ltTablePtr  tablePtr1;
	
	float temprate,sumnum,tempnum;
	ltDbConn *tempDbCon;
  char *dbUser;
  char *dbPass;
	char *dbName;
 	ltDbCursor *tempCursor1;
 	LT_DBROW tempRow1;
	
	printf("========START==========\n");
	/*»ñÈ¡±¨¸æÀàĞÍ*/
	reporttype=atol(ltMsgGetVar_s(ltMsgPk,"reporttype"));
	/*¹«¹²Ö´ĞĞ´úÂëÇøÓò£º»ñÈ¡Ö´ĞĞÒ³Ãæ£¨±íÏÖ²ã£©¶ÔÓ¦µÄ²ÎÊı£¬²¢´æ´¢Êı¾İ*/
	if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
			sprintf(caSdate,"%s","");
    }else{
    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
    }
    
    if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
			sprintf(caEdate,"%s","");
    }else{
    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
    }
    
    if( ltMsgGetVar_s(ltMsgPk,"email")==NULL){
			sprintf(caEdate,"%s","");
    }else{
    		sprintf(email,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
    }
			
    reportname=ltMsgGetVar_s(ltMsgPk,"reportname");
    if(reportname==NULL){
    	reportname="********";
    }
    
    time1 = nasCvtLongTime(caSdate,"00:00:00");
	  time2 = nasCvtLongTime(caEdate,"23:59:59");
	  lTime = time(0);
	  nasCvtStime(lTime,caSdate1,caTime1);
	  caTime1[5]=0;	
	  
	   sprintf(caTempDir,"%s/report/%d/%lu/",datacenterdir,reporttype,ltStrGetId());
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            /// Éú³É¸Ã±¨¸æÄ¿Â¼´íÎó 
            ltMsgPk->msgpktype=1;
						lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >Error:±¨¸æÉú³É´íÎó,ÎŞ·¨½¨Á¢±¨¸æÄ¿Â¼!<br><a href=\"javascript:history.back(1);\" >Éú³ÉĞÂ±¨¸æ?</a></body></html>");		
						ltMsgFree(ltMsgPk);
            return 0;
        }
   }
       
    sprintf(caLabel,"/bin/echo 'sname=%s' > %sinfo.ini",reportname,caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼±êÖ¾*/
    sprintf(caLabel,"/bin/echo 'email=%s' >> %sinfo.ini",email,caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼±êÖ¾*/
    sprintf(caLabel,"/bin/echo 'sdate=%s/%s' >> %sinfo.ini",caSdate1,caTime1,caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼±êÖ¾*/
    sprintf(caLabel,"/bin/echo 'edate=%s' >> %sinfo.ini",caEdate,caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼±êÖ¾*/
    sprintf(caLabel,"/bin/echo 'scon=%s--%s' >> %sinfo.ini",caEdate,caSdate,caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼±êÖ¾*/

    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼ÕıÔÚ´¦Àí±êÖ¾*/
	  sprintf(caLabel,"/bin/cp /app/msa/msas/htmlplt/reportplt/left-1.jpg  %s",caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼ÕıÔÚ´¦Àí±êÖ¾*/
	  sprintf(caLabel,"/bin/cp /app/msa/msas/htmlplt/reportplt/left-2.jpg  %s",caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼ÕıÔÚ´¦Àí±êÖ¾*/
	  sprintf(caLabel,"/bin/cp /app/msa/msas/htmlplt/reportplt/left-3.jpg  %s",caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼ÕıÔÚ´¦Àí±êÖ¾*/
	  sprintf(caLabel,"/bin/cp /app/msa/msas/htmlplt/reportplt/left-4.jpg  %s",caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼ÕıÔÚ´¦Àí±êÖ¾*/
	  sprintf(caLabel,"/bin/cp /app/msa/msas/htmlplt/reportplt/banner.jpg  %s",caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼ÕıÔÚ´¦Àí±êÖ¾*/
	  sprintf(caLabel,"/bin/cp /app/msa/msas/htmlplt/reportplt/right-top-d.jpg  %s",caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼ÕıÔÚ´¦Àí±êÖ¾*/	
  
  /*ÅĞ¶Ï±¨¸æÀàĞÍ£¬Ñ¡Ôñ±¨¸æsqlÖ´ĞĞÖÖÀà£¬ÒÔ¼°Í¼Æ¬Éú³ÉµÄÀàĞÍ*/	
	if(reporttype==1){
		sprintf(caTempDataDir,"%sdata",caTempDir);     
    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd == (-1)) {
	  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","Êı¾İÎÄ¼şĞ´´íÎó,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
		      system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
		    	ltMsgFree(ltMsgPk);
		        return 0;
		}		
		   /*    Ä£°å¿ªÊ¼     */ 
    dbPtr=lt_dbinit();
    tablePtr=lt_dbput_table(dbPtr,"list");
		Max = 100;
    sprintf(sqlBuf,"select stime,cast(avg(cpunum) as dec(18,2)) as cpunum,cast(avg(memnum) as dec(18,2)) as memnum,cast(avg(sysdisknum) as dec(18,2)) as sysdisknum,cast(avg(appdisknum) as dec(18,2)) as appdisknum,cast(avg(usernum) as dec(18,2)) as usernum from msassysinfolog where rtime >= %u and rtime <=%u group by stime order by stime asc",time2,time1);
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
				j=0;
				tempRow= ltDbFetchRow(tempCursor);
                while(tempRow!=NULL){
								 sprintf(caItem,"%s:00",tempRow[0]);
                 lt_dbput_recordvars(tablePtr,6,
                 "stime",LT_TYPE_STRING,tempRow[0],
                 "cpunum",LT_TYPE_STRING,tempRow[1],
								 "memnum",LT_TYPE_STRING,tempRow[2],
                 "sysdisknum",LT_TYPE_STRING,tempRow[3],
                 "appdisknum",LT_TYPE_STRING,tempRow[4],
                 "usernum",LT_TYPE_STRING,tempRow[5]);
				 
				 if( (j%3)==0 ){
			          sprintf(caTemp,"%s,%llu,%llu,%llu,\r\n",
			        	caItem,
						  	atoll(tempRow[1]),
								atoll(tempRow[2]),
								atoll(tempRow[3]));
			          write(srcfd, caTemp, strlen(caTemp));	
			      }else{
			      	sprintf(caTemp,"%s,%llu,%llu,%llu, \r\n",
			        	"",			        	
						atoll(tempRow[1]),
						atoll(tempRow[2]),
						atoll(tempRow[3]));
			        write(srcfd, caTemp, strlen(caTemp));	
			      }
			      j++;
				 
               tempRow=ltDbFetchRow(tempCursor);
                }
         ltDbCloseCursor(tempCursor);
    }   
    sprintf(caTempFile,"%soutflow1.gif",caTempDir);
    sprintf(caTemp,"/app/msa/msas/htmlplt/Scripts/outflowpltline.pl   %s %s %u ",caTempDataDir,caTempFile,Max);
    system(caTemp);
    lt_dbput_rootvars(dbPtr,1,"file1","outflow1.gif");
	  /* Ä£°å2*/
		sprintf(caTempDataDir1,"%sdata1",caTempDir);     
    srcfd1 = open(caTempDataDir1,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd == (-1)) {
	  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","Êı¾İÎÄ¼şĞ´´íÎó,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
		      system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
		    	ltMsgFree(ltMsgPk);
		        return 0;
		}	
    tablePtr1=lt_dbput_table(dbPtr,"list1");
    sprintf(sqlBuf,"select sdate,cast(avg(cpunum) as dec(18,2)) as cpunum,cast(avg(memnum) as dec(18,2)) as memnum,cast(avg(sysdisknum) as dec(18,2)) as sysdisknum,cast(avg(appdisknum) as dec(18,2)) as appdisknum,cast(avg(usernum) as dec(18,2)) as usernum from msassysinfolog where rtime >=%u and rtime <=%u group by sdate order by sdate asc",time2,time1);
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
				j=0;
				tempRow= ltDbFetchRow(tempCursor);
                while(tempRow!=NULL){				
				sprintf(caItem,"%s",tempRow[0]);
                 lt_dbput_recordvars(tablePtr1,6,
                 "stime",LT_TYPE_STRING,tempRow[0],
                 "cpunum",LT_TYPE_STRING,tempRow[1],
				 				 "memnum",LT_TYPE_STRING,tempRow[2],
                 "sysdisknum",LT_TYPE_STRING,tempRow[3],
                 "appdisknum",LT_TYPE_STRING,tempRow[4],
                 "usernum",LT_TYPE_STRING,tempRow[5]);
				 
				 if( (j%3)==0 ){
			          sprintf(caTemp,"%s,%llu,%llu,%llu,\r\n",
			        	caItem,
						atoll(tempRow[1]),
						atoll(tempRow[2]),
						atoll(tempRow[3]));
			          write(srcfd1, caTemp, strlen(caTemp));	
			      }else{
			      	sprintf(caTemp,"%s,%llu,%llu,%llu, \r\n",
			        	"",			        	
						atoll(tempRow[1]),
						atoll(tempRow[2]),
						atoll(tempRow[3]));
			        write(srcfd1, caTemp, strlen(caTemp));	
			      }
			      j++;
            tempRow=ltDbFetchRow(tempCursor);
           }
         ltDbCloseCursor(tempCursor);
    }
	
	
    lt_dbput_rootvars(dbPtr,1,"reportbdate",caSdate1);	
	  sprintf(caTempFile,"%soutflow2.gif",caTempDir);
    sprintf(caTemp,"/app/msa/msas/htmlplt/Scripts/outflowpltline.pl   %s %s %u ",caTempDataDir1,caTempFile,Max);
    system(caTemp);
    lt_dbput_rootvars(dbPtr,1,"file2","outflow2.gif");
    char  *lt_page_content;
    int   iFd;
    lt_page_content=ltPltFileParse("/app/msa/msas/htmlplt/reportplt/sysinforeport.htm",dbPtr->doc,0);
    if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","ÎŞ±¨¸æÊı¾İ,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
            system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
    }else{
        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");/*Éú³Éhtml¸ñÊ½±¨¸æ*/
        sprintf(caLabel,"%sindex.htm",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
       close(iFd);
    }
     ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >±¨¸æÕıÔÚÉú³ÉÖĞ,Çëµ½±¨¸æÁĞ±íÖĞ²ì¿´Éú³É×´Ì¬»òÕßÏÂÔØ±¨¸æ<br><a href=\"/main/srvreportlist.htm\" >²ì¿´ĞÂ±¨¸æ?</a></body></html>");		
     //pdf¸ñÊ½±¨¸æÉú³É´úÂë
     sprintf(caLabel,"/app/msa/msas/htmlplt/Scripts/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
     printf("change to mht :: %s\n",caLabel);
     system(caLabel);
     chdir(caTempDir);
	   system("tar -cvzf report.tgz *");
	   sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	   system(caLabel);
	   sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	   system(caLabel);
	   ltMsgFree(ltMsgPk);
     return 0;
   }
	
	if(reporttype==2){
		sprintf(caTempDataDir,"%sdata",caTempDir);     
    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd == (-1)) {
	  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","Êı¾İÎÄ¼şĞ´´íÎó,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
		      system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
		    	ltMsgFree(ltMsgPk);
		        return 0;
		}	
		 /*    Ä£°å¿ªÊ¼     */ 
    dbPtr=lt_dbinit();
    tablePtr=lt_dbput_table(dbPtr,"list");
		Max = 100;
    sprintf(sqlBuf,"select gnmkey,cast(avg(cpunum) as dec(18,2)) as cpunum,cast(avg(memnum) as dec(18,2)) as memnum,cast(avg(sysdisknum) as dec(18,2)) as usernum,cast(avg(tcpnum) as dec(18,2)) as tcpnum,cast(avg(udpnum) as dec(18,2)) as udpnum,cast(avg(urlnum) as dec(18,2)) as urlnum,cast(avg(bytenum) as dec(18,2)) as bytenum,cast(avg(bytenumu) as dec(18,2)) as bytenumu,cast(avg(bytenumd) as dec(18,2)) as bytenumd from msastimeinfolog where rtime >=%u and rtime <=%u group by gnmkey order by gnmkey asc",time2,time1);
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
				tempRow=ltDbFetchRow(tempCursor);
                while(tempRow!=NULL){				
								 sprintf(caItem,"%s",tempRow[0]);
                 lt_dbput_recordvars(tablePtr,10,
                 "stime",LT_TYPE_STRING,tempRow[0],
                 "cpunum",LT_TYPE_STRING,tempRow[1],
				 				 "memnum",LT_TYPE_STRING,tempRow[2],
                 "usernum",LT_TYPE_STRING,tempRow[3],
                 "tcpnum",LT_TYPE_STRING,tempRow[4],
                 "udpnum",LT_TYPE_STRING,tempRow[5],
				 				 "urlnum",LT_TYPE_STRING,tempRow[6],
								 "bytenum",LT_TYPE_STRING,tempRow[7],
								 "bytenumu",LT_TYPE_STRING,tempRow[8],
				 				 "bytenumd",LT_TYPE_STRING,tempRow[9]);
				 
			  sprintf(caTemp,"%s,%llu,%llu,%llu,\r\n",caItem,atoll(tempRow[1]),atoll(tempRow[2]),atoll(tempRow[3]));
			  write(srcfd, caTemp, strlen(caTemp));					 
        tempRow=ltDbFetchRow(tempCursor);
       }
        ltDbCloseCursor(tempCursor);
    }
    lt_dbput_rootvars(dbPtr,1,"reportbdate",caSdate1);	
		sprintf(caTempFile,"%soutflow2.gif",caTempDir);
    sprintf(caTemp,"/app/msa/msas/htmlplt/Scripts/outflowpltline.pl   %s %s %u ",caTempDataDir,caTempFile,Max);
    system(caTemp);
    lt_dbput_rootvars(dbPtr,1,"file1","outflow2.gif");
    char  *lt_page_content;
    int   iFd;
    lt_page_content=ltPltFileParse("/app/msa/msas/htmlplt/reportplt/timeinforeport.htm",dbPtr->doc,0);
    if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","ÎŞ±¨¸æÊı¾İ,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
            system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
    }else{
        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");/*Éú³Éhtml¸ñÊ½±¨¸æ*/
        sprintf(caLabel,"%sindex.htm",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
       close(iFd); 
    }
     ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >±¨¸æÕıÔÚÉú³ÉÖĞ,Çëµ½±¨¸æÁĞ±íÖĞ²ì¿´Éú³É×´Ì¬»òÕßÏÂÔØ±¨¸æ<br><a href=\"/main/srvreportlist.htm\" >²ì¿´ĞÂ±¨¸æ?</a></body></html>");		
	   //pdf¸ñÊ½±¨¸æÉú³É´úÂë
     sprintf(caLabel,"/app/msa/msas/htmlplt/Scripts/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
     printf("change to mht :: %s\n",caLabel);
     system(caLabel);
     chdir(caTempDir);
  	 system("tar -cvzf report.tgz *");
	   sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	   system(caLabel);
	   sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	   system(caLabel);	
	   ltMsgFree(ltMsgPk);
     return 0;
  }
  
  if(reporttype==3){	
  	sprintf(caTempDataDir,"%sdata",caTempDir);     
    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd == (-1)) {
	  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","Êı¾İÎÄ¼şĞ´´íÎó,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
		      system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
		    	ltMsgFree(ltMsgPk);
		        return 0;
		} 	
  	 /*    Ä£°å¿ªÊ¼     */ 
    dbPtr=lt_dbinit();
    tablePtr=lt_dbput_table(dbPtr,"list");
		Max = 100;
    sprintf(sqlBuf,"select gnmkey,cast(avg(usernum) as dec(18,0)) from msastimeinfolog where rtime >= %u and rtime <=%u group by gnmkey order by gnmkey asc",time2,time1); 
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
				tempRow=ltDbFetchRow(tempCursor);
                while(tempRow!=NULL){				
								sprintf(caItem,"%s",tempRow[0]);
								tempnum=(atol(tempRow[1]));
									/*¶ş´Î²éÑ¯*/								  
    							dbName=_ltPubInfo->_dbname;
	  							dbUser=_ltPubInfo->_dbuser;
	  							dbPass=_ltPubInfo->_dbpass;
    							tempDbCon=ltDbConnect(dbUser,dbPass,dbName);
    							if (tempDbCon==NULL){
										ltMsgPk->msgpktype=1;
										lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >ÏµÍ³Êı¾İÁ¬½ÓÎŞ·¨´ò¿ª<br><a href=\"javascript:history.back(1);\" >·µ»Ø</a></body></html>");		
										ltMsgFree(ltMsgPk);
										return 0;}     							
										sprintf(strBuf,"select usernum from msasclientinfo where gnmkey='%s'",tempRow[0]);
								 		tempCursor1=ltDbOpenCursor(tempDbCon,strBuf);
									if(tempCursor1!=NULL){
													tempRow1= ltDbFetchRow(tempCursor1);
													while(tempRow1!=NULL){												
       												sumnum=(atol(tempRow1[0]));
       												temprate=tempnum/sumnum*100;
       												sprintf(rate,"%4.2f",temprate);
       												lt_dbput_recordvars(tablePtr,4,
                       			  "gnmkey",LT_TYPE_STRING,caItem,
                       			  "tempnum",LT_TYPE_STRING,tempRow[1],
                       			  "sumnum",LT_TYPE_STRING,tempRow1[0],
                        				"rate",LT_TYPE_STRING,rate);
                        			sprintf(caTemp,"%s,%f,\r\n",
       			        					caItem,
       												temprate);
       			          				write(srcfd, caTemp, strlen(caTemp));	
                 							tempRow1= ltDbFetchRow(tempCursor1);
													}	
													ltDbCloseCursor(tempCursor1);						 																																																
									}
									ltDbClose(tempDbCon); /*¹Ø±ÕÊı¾İ¿â*/	
									/*¶ş´Î²éÑ¯½áÊø*/					          	      
                tempRow=ltDbFetchRow(tempCursor);
                }
         ltDbCloseCursor(tempCursor);
    }
     lt_dbput_rootvars(dbPtr,1,"reportbdate",caSdate1);
	   sprintf(caTempFile,"%soutflow2.gif",caTempDir);
     sprintf(caTemp,"/app/msa/msas/htmlplt/Scripts/outflowpltline2.pl   %s %s %u ",caTempDataDir,caTempFile,Max);
     system(caTemp);
     lt_dbput_rootvars(dbPtr,1,"file1","outflow2.gif");
     char  *lt_page_content;
     int   iFd;
     lt_page_content=ltPltFileParse("/app/msa/msas/htmlplt/reportplt/timeinforeport.htm",dbPtr->doc,0);
     if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","ÎŞ±¨¸æÊı¾İ,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
            system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
     }else{
        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");/*Éú³Éhtml¸ñÊ½±¨¸æ*/
        sprintf(caLabel,"%sindex.htm",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
       close(iFd);
     }
  	 ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >±¨¸æÕıÔÚÉú³ÉÖĞ,Çëµ½±¨¸æÁĞ±íÖĞ²ì¿´Éú³É×´Ì¬»òÕßÏÂÔØ±¨¸æ<br><a href=\"/main/srvreportlist.htm\" >²ì¿´ĞÂ±¨¸æ?</a></body></html>");		
     //pdf¸ñÊ½±¨¸æÉú³É´úÂëÂÔ
     sprintf(caLabel,"/app/msa/msas/htmlplt/Scripts/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
     printf("change to mht :: %s\n",caLabel);
     system(caLabel);
     chdir(caTempDir);
	   system("tar -cvzf report.tgz *");
	   sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	   system(caLabel);
	   sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	   system(caLabel);	
	   ltMsgFree(ltMsgPk);
     return 0;	
  	}
  	 	
  if(reporttype==4){
  	sprintf(caTempDataDir,"%sdata",caTempDir);     
    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd == (-1)) {
	  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","Êı¾İÎÄ¼şĞ´´íÎó,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
		      system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
		    	ltMsgFree(ltMsgPk);
		        return 0;
		} 	
  	/*Ä£°å¿ªÊ¼*/  
    dbPtr=lt_dbinit();
    tablePtr=lt_dbput_table(dbPtr,"list");
		Max = 10000;
		
    sprintf(sqlBuf,"select sum(srvflow0) as srvflow0,sum(srvflow1) as srvflow1,sum(srvflow2) as srvflow2,sum(srvflow3) as srvflow3,sum(srvflow4) as srvflow4,sum(srvflow5) as srvflow5,sum(srvflow6) as srvflow6,sum(srvflow7) as srvflow7,sum(srvflow8) as srvflow8,sum(srvflow9) as srvflow9,sum(srvflow10) as srvflow10,sum(srvflow11) as srvflow11,sum(srvflow12) as srvflow12,sum(srvflow13) as srvflow13,sum(srvflow14) as srvflow14,sum(srvflow15) as srvflow15,sum(srvflow16) as srvflow16,sum(srvflow17) as srvflow17,sum(srvflow18) as srvflow18,sum(srvflow19) as srvflow19,sum(srvflow20) as srvflow20,sum(srvflow21) as srvflow21,sum(srvflow22) as srvflow22,sum(srvflow23) as srvflow23,sum(srvflow24) as srvflow24,sum(srvflow25) as srvflow25,sum(srvflow26) as srvflow26,sum(srvflow27) as srvflow27,sum(srvflow28) as srvflow28,sum(srvflow29) as srvflow29 from msastimeinfolog where rtime >=%u and rtime <=%u group by stime order by stime asc",time2,time1);
    printf("%s",sqlBuf);
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
          for(i=0;i<30;i++){
                sprintf(serve,"srvflow%d",i);
                 lt_dbput_recordvars(tablePtr,2,
                 "serve",LT_TYPE_STRING,serve,
                 "flow",LT_TYPE_STRING,tempRow[i]);                 
                 sprintf(caTemp,"%s,%s \r\n",serve,tempRow[i]);
       			     write(srcfd, caTemp, strlen(caTemp));			 
             }                
         ltDbCloseCursor(tempCursor);
    }
    lt_dbput_rootvars(dbPtr,1,"reportbdate",caSdate1);	
    sprintf(caTempFile,"%soutflow1.gif",caTempDir);
    sprintf(caTemp,"/app/msa/msas/htmlplt/Scripts/onlinesrv1_s.pl   %s %s %u ",caTempDataDir,caTempFile,Max);
    system(caTemp);
    lt_dbput_rootvars(dbPtr,1,"file1","outflow1.gif");
    char  *lt_page_content;
    int   iFd;
    lt_page_content=ltPltFileParse("/app/msa/msas/htmlplt/reportplt/timeinforeport.htm",dbPtr->doc,0);
    if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","ÎŞ±¨¸æÊı¾İ,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
            system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
    }else{
        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");/*Éú³Éhtml¸ñÊ½±¨¸æ*/
        sprintf(caLabel,"%sindex.htm",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
        close(iFd);     
     }	
     
     ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >±¨¸æÕıÔÚÉú³ÉÖĞ,Çëµ½±¨¸æÁĞ±íÖĞ²ì¿´Éú³É×´Ì¬»òÕßÏÂÔØ±¨¸æ<br><a href=\"/main/srvreportlist.htm\" >²ì¿´ĞÂ±¨¸æ?</a></body></html>");		
     //pdf¸ñÊ½±¨¸æÉú³É´úÂëÂÔ
     sprintf(caLabel,"/app/msa/msas/htmlplt/Scripts/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
     printf("change to mht :: %s\n",caLabel);
     system(caLabel);
     chdir(caTempDir);
   	 system("tar -cvzf report.tgz *");
	   sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	   system(caLabel);
	   sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	   system(caLabel);	
	   ltMsgFree(ltMsgPk);
     return 0;
  }
	return 0; 
} 

/*±¨¸æ5-->9º¯Êı´¦Àí*/
int msasreportproc1(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  //±äÁ¿²ÎÊı¶¨Òå
	  long lTime;
    char caLabel[256];
    char caSdate[32],caEdate[32],caSdate1[32],caTime1[32];
    char *reportname;

    int  iFd;
    char caTempDir[256];
    char email[25];
    int srcfd;
    unsigned int lMaxBytes,reporttype,number=0;
    char caTempFile[256];
    char caTempDataDir[256];
    char caTemp[18192];
    
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;
    char sqlBuf[1024];
    char  *lt_page_content;
    
    printf("========START==========\n");
	
	  /*»ñÈ¡±¨¸æÀàĞÍ*/
	  reporttype=atol(ltMsgGetVar_s(ltMsgPk,"reporttype"));
	 /*¹«¹²Ö´ĞĞ´úÂëÇøÓò£º»ñÈ¡Ö´ĞĞÒ³Ãæ£¨±íÏÖ²ã£©¶ÔÓ¦µÄ²ÎÊı£¬²¢´æ´¢Êı¾İ*/
	  if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){
			sprintf(caSdate,"%s","");
    }else{
    		sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
    }
    
    if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
			sprintf(caEdate,"%s","");
    }else{
    		sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
    }
    
    if( ltMsgGetVar_s(ltMsgPk,"email")==NULL){
			sprintf(caEdate,"%s","");
    }else{
    		sprintf(email,"%s",ltMsgGetVar_s(ltMsgPk,"email"));
    }
			
    reportname=ltMsgGetVar_s(ltMsgPk,"reportname");
    if(reportname==NULL){
    	reportname="********";
    }
    
	  lTime = time(0);
	  nasCvtStime(lTime,caSdate1,caTime1);//½«µ±Ç°Ê±¼ä»®·ÖÎªÈÕÆÚºÍ
	  caTime1[5]=0;	
	  
	  sprintf(caTempDir,"%s/report/%d/%lu/",datacenterdir,reporttype,ltStrGetId());
    if(ltFileIsExist(caTempDir)!=1){
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            /// Éú³É¸Ã±¨¸æÄ¿Â¼´íÎó 
            ltMsgPk->msgpktype=1;
						lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >Error:±¨¸æÉú³É´íÎó,ÎŞ·¨½¨Á¢±¨¸æÄ¿Â¼!<br><a href=\"javascript:history.back(1);\" >Éú³ÉĞÂ±¨¸æ?</a></body></html>");		
						ltMsgFree(ltMsgPk);
            return 0;
        }
    }
       
    sprintf(caLabel,"/bin/echo 'sname=%s' > %sinfo.ini",reportname,caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼±êÖ¾*/
    sprintf(caLabel,"/bin/echo 'email=%s' >> %sinfo.ini",email,caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼±êÖ¾*/
    sprintf(caLabel,"/bin/echo 'sdate=%s/%s' >> %sinfo.ini",caSdate1,caTime1,caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼±êÖ¾*/
    sprintf(caLabel,"/bin/echo 'edate=%s' >> %sinfo.ini",caEdate,caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼±êÖ¾*/
    sprintf(caLabel,"/bin/echo 'scon=%s--%s' >> %sinfo.ini",caEdate,caSdate,caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼±êÖ¾*/

    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼ÕıÔÚ´¦Àí±êÖ¾*/
	  sprintf(caLabel,"/bin/cp /app/msa/msas/htmlplt/reportplt/left-1.jpg  %s",caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼ÕıÔÚ´¦Àí±êÖ¾*/
	  sprintf(caLabel,"/bin/cp /app/msa/msas/htmlplt/reportplt/left-2.jpg  %s",caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼ÕıÔÚ´¦Àí±êÖ¾*/
	  sprintf(caLabel,"/bin/cp /app/msa/msas/htmlplt/reportplt/left-3.jpg  %s",caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼ÕıÔÚ´¦Àí±êÖ¾*/
	  sprintf(caLabel,"/bin/cp /app/msa/msas/htmlplt/reportplt/left-4.jpg  %s",caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼ÕıÔÚ´¦Àí±êÖ¾*/
	  sprintf(caLabel,"/bin/cp /app/msa/msas/htmlplt/reportplt/banner.jpg  %s",caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼ÕıÔÚ´¦Àí±êÖ¾*/
	  sprintf(caLabel,"/bin/cp /app/msa/msas/htmlplt/reportplt/right-top-d.jpg  %s",caTempDir);
    system(caLabel);/*½¨Á¢Ä¿Â¼ÕıÔÚ´¦Àí±êÖ¾*/

    if(reporttype==6){
	    sprintf(caTempDataDir,"%sdata",caTempDir);     
	    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
		  if(srcfd == (-1)) {
		  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","Êı¾İÎÄ¼şĞ´´íÎó,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
			      system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
			    	ltMsgFree(ltMsgPk);
			        return 0;
		}
		
		/*ÔËÓÃÍøÒ³Ä£°åÉú³É±¨±í*/  
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");  
    
    tablePtr=lt_dbput_table(dbPtr,"list");
    sprintf(sqlBuf,"select sdate, sum(srvflow0+ srvflow1+ srvflow2+ srvflow3+ srvflow4+ srvflow5+   srvflow6+ srvflow7+ srvflow8+ srvflow9+ srvflow10+ srvflow11+ srvflow12+   srvflow13+ srvflow14+ srvflow15+ srvflow16+ srvflow17+ srvflow18+ srvflow19+   srvflow20+ srvflow21+ srvflow22+ srvflow23+ srvflow24+ srvflow25+ srvflow26+   srvflow27+ srvflow28+   srvflow29)as flow from msastimeinfolog where sdate>='%s' and sdate<='%s' group by sdate",caEdate,caSdate);
    printf("%s\n",sqlBuf);
    
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){    	  
				tempRow=ltDbFetchRow(tempCursor);
				             
       while(tempRow!=NULL){  
       	if(number<=atol(tempRow[1])){
       		number=atol(tempRow[1]); 
       		}  
       	        
		   lt_dbput_recordvars(tablePtr,2,
                 "sdate",LT_TYPE_STRING,tempRow[0],
                 "flow",LT_TYPE_STRING,tempRow[1]);         
		   
		       sprintf(caTemp,"%s,%s\r\n",tempRow[0],tempRow[1]);
			     write(srcfd, caTemp, strlen(caTemp));		
           tempRow=ltDbFetchRow(tempCursor);
           }
         ltDbCloseCursor(tempCursor);
    }
    
  lt_dbput_rootvars(dbPtr,1,"reportbdate",caSdate1);

  sprintf(caTempFile,"%soutflow1.gif",caTempDir);
  sprintf(caTemp,"/app/msa/msas/htmlplt/Scripts/flowpltline.pl   %s %s %d ",caTempDataDir,caTempFile,number);
  system(caTemp);
  lt_dbput_rootvars(dbPtr,1,"file1","outflow1.gif");

      
  ltMsgPk->msgpktype=1;
  
  lt_page_content=ltPltFileParse("/app/msa/msas/htmlplt/reportplt/everydayflow.htm",dbPtr->doc,0);
  if(lt_page_content==NULL){
        sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","ÎŞ±¨¸æÊı¾İ,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
        system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
        ltMsgFree(ltMsgPk);
        lt_dbfree(dbPtr);
        return 0;
  }else{
        lt_page_content=strstr(lt_page_content,"<!DOCTYPE"); /*Éú³Éhtml¸ñÊ½±¨¸æ*/
        sprintf(caLabel,"%sindex.htm",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0){
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
       close(iFd);
  }

    ltMsgPk->msgpktype=1;
    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >±¨¸æÕıÔÚÉú³ÉÖĞ,Çëµ½±¨¸æÁĞ±íÖĞ²ì¿´Éú³É×´Ì¬»òÕßÏÂÔØ±¨¸æ<br><a href=\"/main/srvreportlist.htm\" >²ì¿´ĞÂ±¨¸æ?</a></body></html>");       
    //pdf¸ñÊ½±¨¸æÉú³É´úÂë
    sprintf(caLabel,"/app/ns/bin/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
    printf("change to mht :: %s\n",caLabel);
    system(caLabel);
    chdir(caTempDir);
    system("tar -cvzf report.tgz *");
    sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
    system(caLabel);
    sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
    system(caLabel);   
    ltMsgFree(ltMsgPk);
    return 0;   
   }
   
   if(reporttype==7){	
	   	sprintf(caTempDataDir,"%sdata",caTempDir);     
	    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
		  if(srcfd == (-1)) {
		  	 sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","Êı¾İÎÄ¼şĞ´´íÎó,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
			   system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
			   ltMsgFree(ltMsgPk);
			   return 0;
		}	
   	/*ÔËÓÃÍøÒ³Ä£°åÉú³É±¨±í*/
    char useracoount[32];
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");  
    sprintf(useracoount,"%s","0");
    sprintf(sqlBuf,"%s","select count(*) from AlertLis");
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
      tempRow= ltDbFetchRow(tempCursor);
      if(tempRow!=NULL){
        sprintf(useracoount,"%s",tempRow[0]);
      }
      ltDbCloseCursor(tempCursor);
    }

    tablePtr=lt_dbput_table(dbPtr,"list");
    sprintf(sqlBuf,"select substring(Matching_time,1,10) as myday,count(*) as number from AlertList where Matching_time>='%s' and Matching_time<='%s' group by myday",caEdate,caSdate);
    printf("%s\n",sqlBuf);
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){    	  
			 tempRow=ltDbFetchRow(tempCursor);	             
       while(tempRow!=NULL){    
       	  number=atol(tempRow[1]);       
		      lt_dbput_recordvars(tablePtr,2,
                 "Alarm_date",LT_TYPE_STRING,tempRow[0],
                 "Alarming_number",LT_TYPE_STRING,tempRow[1]);         	   
		       sprintf(caTemp,"%s,%s\r\n",tempRow[0],tempRow[1]);
			     write(srcfd, caTemp, strlen(caTemp));		
           tempRow=ltDbFetchRow(tempCursor);
           }
         ltDbCloseCursor(tempCursor);
    }   
  lt_dbput_rootvars(dbPtr,1,"reportbdate",caSdate1);
  sprintf(caTempFile,"%soutflow1.gif",caTempDir);
  sprintf(caTemp,"/app/msa/msas/htmlplt/Scripts/flowplt.pl   %s %s %d ",caTempDataDir,caTempFile,number);
  system(caTemp);
  lt_dbput_rootvars(dbPtr,1,"file1","outflow1.gif");    
  ltMsgPk->msgpktype=1;
  lt_page_content=ltPltFileParse("/app/msa/msas/htmlplt/reportplt/model.htm",dbPtr->doc,0);
   if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","ÎŞ±¨¸æÊı¾İ,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
            system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
   }else{
        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
        sprintf(caLabel,"%sindex.htm",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
       close(iFd);
      /*Éú³Éhtml¸ñÊ½±¨¸æ*/
   }
    ltMsgPk->msgpktype=1;
    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >±¨¸æÕıÔÚÉú³ÉÖĞ,Çëµ½±¨¸æÁĞ±íÖĞ²ì¿´Éú³É×´Ì¬»òÕßÏÂÔØ±¨¸æ<br><a href=\"/main/srvreportlist.htm\" >²ì¿´ĞÂ±¨¸æ?</a></body></html>");      
    //pdf
    sprintf(caLabel,"/app/ns/bin/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
    printf("change to mht :: %s\n",caLabel);
    system(caLabel);
    chdir(caTempDir);
    system("tar -cvzf report.tgz *");
    sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
    system(caLabel);
    sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
    system(caLabel);   
    ltMsgFree(ltMsgPk);
    return 0;   
   	}
   	
   if(reporttype==8){
	   	sprintf(caTempDataDir,"%sdata",caTempDir);     
	    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
		  if(srcfd == (-1)) {
		  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","Êı¾İÎÄ¼şĞ´´íÎó,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
			      system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
			    	ltMsgFree(ltMsgPk);
			      return 0;
		}
   		   		
    /*ÔËÓÃÍøÒ³Ä£°åÉú³É±¨±í*/
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");  
    tablePtr=lt_dbput_table(dbPtr,"list");
    lMaxBytes =10;
    sprintf(sqlBuf,"select gnmkey,count(*) as number from AlertList where Matching_time>='%s' and Matching_time<='%s' group by gnmkey;",caEdate,caSdate);
    printf("%s\n",sqlBuf);
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){    	  
				tempRow=ltDbFetchRow(tempCursor);			             
        while(tempRow!=NULL){             
        if(lMaxBytes<=(atol(tempRow[1]))){
		           lMaxBytes = atol(tempRow[1]);
		           return 0;
		        }
		       
		   /*¶ş´Î²éÑ¯*/
			 ltDbConn *tempDbCon;
    	 char *dbUser;
    	 char *dbPass; 
    	 char *dbName; 
    	 char strBuf[1024];
    	 ltDbCursor *tempCursor1; 
    	 LT_DBROW tempRow1;
    							dbName=_ltPubInfo->_dbname;
	  							dbUser=_ltPubInfo->_dbuser;
	  							dbPass=_ltPubInfo->_dbpass;
    							tempDbCon=ltDbConnect(dbUser,dbPass,dbName);
    							if (tempDbCon==NULL){
										ltMsgPk->msgpktype=1;
										lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >ÏµÍ³Êı¾İÁ¬½ÓÎŞ·¨´ò¿ª<br><a href=\"javascript:history.back(1);\" >·µ»Ø</a></body></html>");		
										ltMsgFree(ltMsgPk);
										return 0;
										}     							
										sprintf(strBuf,"select sname from alertlist where gnmkey in('%s')",tempRow[0]);
								 		tempCursor1=ltDbOpenCursor(tempDbCon,strBuf);
									if(tempCursor1!=NULL){
										tempRow1= ltDbFetchRow(tempCursor1);												    																								 																																																
									}
									ltDbClose(tempDbCon); /*¹Ø±ÕÊı¾İ¿â*/	
									/*¶ş´Î²éÑ¯½áÊø*/							
                lt_dbput_recordvars(tablePtr,3,
                 "gnmkey",LT_TYPE_STRING,tempRow[0],
                 "sname",LT_TYPE_STRING,tempRow1[0],
                 "Alarming_number",LT_TYPE_STRING,tempRow[1]);         		   
		       sprintf(caTemp,"%s,%s,%s\r\n",tempRow[0],tempRow1[0],tempRow[1]);		             
			     write(srcfd, caTemp, strlen(caTemp));		
           tempRow=ltDbFetchRow(tempCursor);           
           ltDbCloseCursor(tempCursor1);	
           }        
         ltDbCloseCursor(tempCursor);
    }
    
	  lt_dbput_rootvars(dbPtr,1,"reportbdate",caSdate1);
	  sprintf(caTempFile,"%soutflow1.gif",caTempDir);
	  sprintf(caTemp,"/app/msa/msas/htmlplt/Scripts/flowplt.pl   %s %s %d ",caTempDataDir,caTempFile,lMaxBytes);
	  system(caTemp);
	  lt_dbput_rootvars(dbPtr,1,"file1","outflow1.gif");    
	  ltMsgPk->msgpktype=1;
	  lt_page_content=ltPltFileParse("/app/msa/msas/htmlplt/reportplt/gnmkey_model.htm",dbPtr->doc,0);
	  if(lt_page_content==NULL){
	            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","ÎŞ±¨¸æÊı¾İ,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
	            system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
	            ltMsgFree(ltMsgPk);
	            lt_dbfree(dbPtr);
	            return 0;
	  }else{
	        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");
	        sprintf(caLabel,"%sindex.htm",caTempDir);
	        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
	        if(iFd >0) {
	            write(iFd,lt_page_content,strlen(lt_page_content));
	        }
	       close(iFd);
	      /*Éú³Éhtml¸ñÊ½±¨¸æ*/
	  }   
	   ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >±¨¸æÕıÔÚÉú³ÉÖĞ,Çëµ½±¨¸æÁĞ±íÖĞ²ì¿´Éú³É×´Ì¬»òÕßÏÂÔØ±¨¸æ<br><a href=\"/main/srvreportlist.htm\" >²ì¿´ĞÂ±¨¸æ?</a></body></html>");
	   //pdf
	   sprintf(caLabel,"/app/ns/bin/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
	   printf("change to mht :: %s\n",caLabel);
	   system(caLabel);
	   chdir(caTempDir);
	   system("tar -cvzf report.tgz *");
	   sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	   system(caLabel);
	   sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	   system(caLabel);
	   ltMsgFree(ltMsgPk);
	   return 0;				
	   }
   		
    if(reporttype==9){
		   	sprintf(caTempDataDir,"%sdata",caTempDir);     
		    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
			  if(srcfd == (-1)) {
			  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","Êı¾İÎÄ¼şĞ´´íÎó,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
				      system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
				    	ltMsgFree(ltMsgPk);
				        return 0;
		}
   			
   	/*ÔËÓÃÍøÒ³Ä£°åÉú³É±¨±í*/
    char useracoount[32];
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");  
    sprintf(useracoount,"%s","0");
    sprintf(sqlBuf,"%s","select count(*) from AlertLis");
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
      tempRow= ltDbFetchRow(tempCursor);
      if(tempRow!=NULL){
        sprintf(useracoount,"%s",tempRow[0]);
      }
      ltDbCloseCursor(tempCursor);
    }

    tablePtr=lt_dbput_table(dbPtr,"list");
    lMaxBytes = 10;
    sprintf(sqlBuf,"select rule_name,count(*) as number from AlertList where Matching_time>='%s' and Matching_time<='%s' group by rule_name",caEdate,caSdate);
    printf("%s\n",sqlBuf);
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){    	  
			 tempRow=ltDbFetchRow(tempCursor);            
       while(tempRow!=NULL){               
       if(lMaxBytes<=(atol(tempRow[1]))){
		         lMaxBytes = atol(tempRow[1]);
		         printf("%d\n",lMaxBytes);
		         printf("*****************\n");
		   }  
		   lt_dbput_recordvars(tablePtr,2,
                 "rule",LT_TYPE_STRING,tempRow[0],
                 "Alarming_number",LT_TYPE_STRING,tempRow[1]);           
		       sprintf(caTemp,"%s,%s\r\n",tempRow[0],tempRow[1]);
			     write(srcfd, caTemp, strlen(caTemp));		
           tempRow=ltDbFetchRow(tempCursor);
           }
         ltDbCloseCursor(tempCursor);
    }
    
  lt_dbput_rootvars(dbPtr,1,"reportbdate",caSdate1);
  sprintf(caTempFile,"%soutflow1.gif",caTempDir);
  sprintf(caTemp,"/app/msa/msas/htmlplt/Scripts/flowplt.pl   %s %s %d ",caTempDataDir,caTempFile,lMaxBytes);
  system(caTemp);
  lt_dbput_rootvars(dbPtr,1,"file1","outflow1.gif"); 
  ltMsgPk->msgpktype=1;
  lt_page_content=ltPltFileParse("/app/msa/msas/htmlplt/reportplt/Alarm_model.htm",dbPtr->doc,0);
   if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","ÎŞ±¨¸æÊı¾İ,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
            system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
  }else{
        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");/*Éú³Éhtml¸ñÊ½±¨¸æ*/
        sprintf(caLabel,"%sindex.htm",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
       close(iFd);  
  }
    ltMsgPk->msgpktype=1;
    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >±¨¸æÕıÔÚÉú³ÉÖĞ,Çëµ½±¨¸æÁĞ±íÖĞ²ì¿´Éú³É×´Ì¬»òÕßÏÂÔØ±¨¸æ<br><a href=\"/main/srvreportlist.htm\" >²ì¿´ĞÂ±¨¸æ?</a></body></html>");      
    //pdf¸ñÊ½±¨¸æÉú³É´úÂë
    sprintf(caLabel,"/app/ns/bin/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
    printf("change to mht :: %s\n",caLabel);
    system(caLabel);
    chdir(caTempDir);
    system("tar -cvzf report.tgz *");
    sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
    system(caLabel);
    sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
    system(caLabel);
    ltMsgFree(ltMsgPk);
    return 0; 			
  }
   			
   	if(reporttype==5){
   	/*³¡ËùÁ÷Á¿±¨±í*/		
   	sprintf(caTempDataDir,"%sdata",caTempDir);     
    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd == (-1)) {
	  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","Êı¾İÎÄ¼şĞ´´íÎó,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
		      system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
		    	ltMsgFree(ltMsgPk);
		        return 0;
		}			
   	/*ÔËÓÃÍøÒ³Ä£°åÉú³É±¨±í*/
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");  
    
    tablePtr=lt_dbput_table(dbPtr,"list");
    sprintf(sqlBuf,"select gnmkey, srvflow0, srvflow1, srvflow2, srvflow3, srvflow4, srvflow5,  srvflow6, srvflow7, srvflow8, srvflow9, srvflow10, srvflow11, srvflow12,  srvflow13, srvflow14, srvflow15, srvflow16, srvflow17, srvflow18, srvflow19,  srvflow20, srvflow21, srvflow22, srvflow23, srvflow24, srvflow25, srvflow26,  srvflow27, srvflow28,  srvflow29 from msastimeinfolog where sdate>='%s' and  sdate<='%s';",caEdate,caSdate);
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    printf("%s\n",sqlBuf);
    lMaxBytes=100;
    if(tempCursor!=NULL){    	  
			tempRow=ltDbFetchRow(tempCursor);			             
      while(tempRow!=NULL){       	             
		   /*¶ş´Î²éÑ¯*/
			 ltDbConn *tempDbCon;
    	 char *dbUser;
    	 char *dbPass;
    	 char *dbName; 
    	 char strBuf[1024];
    	 ltDbCursor *tempCursor1;
    	 LT_DBROW tempRow1;
    	 dbName=_ltPubInfo->_dbname;
	  	 dbUser=_ltPubInfo->_dbuser;
	  	 dbPass=_ltPubInfo->_dbpass;
    	 tempDbCon=ltDbConnect(dbUser,dbPass,dbName);
    	 if(tempDbCon==NULL){
					ltMsgPk->msgpktype=1;
					lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >ÏµÍ³Êı¾İÁ¬½ÓÎŞ·¨´ò¿ª<br><a href=\"javascript:history.back(1);\" >·µ»Ø</a></body></html>");		
					ltMsgFree(ltMsgPk);
					return 0;
					}     							
					sprintf(strBuf,"select gnmkey,sum(srvflow0+srvflow1+srvflow2+srvflow3+srvflow4+srvflow5+srvflow6+ srvflow7+ srvflow8+ srvflow9+ srvflow10+ srvflow11+ srvflow12+srvflow13+srvflow14+srvflow15+srvflow16+srvflow17+srvflow18+srvflow19+srvflow20+srvflow21+srvflow22+srvflow23+srvflow24+srvflow25+srvflow26+srvflow27+srvflow28+srvflow29)as place from msastimeinfolog where gnmkey='%s' group by gnmkey",tempRow[0]);
					tempCursor1=ltDbOpenCursor(tempDbCon,strBuf);
					if(tempCursor1!=NULL){
						  tempRow1= ltDbFetchRow(tempCursor1);		
							if(lMaxBytes<=(atol(tempRow1[1]))){
			           lMaxBytes = atol(tempRow1[1]);
			           printf("%d\n",lMaxBytes);
			        }     			    																								 																																																
						}					
          lt_dbput_recordvars(tablePtr,32,
          "place_gnmkey",LT_TYPE_STRING,tempRow[0],
          "service1",LT_TYPE_STRING,tempRow[1],
          "service2",LT_TYPE_STRING,tempRow[2],
          "service3",LT_TYPE_STRING,tempRow[3],
          "service4",LT_TYPE_STRING,tempRow[4],
          "service5",LT_TYPE_STRING,tempRow[5],
          "service6",LT_TYPE_STRING,tempRow[6],
          "service7",LT_TYPE_STRING,tempRow[7],
          "service8",LT_TYPE_STRING,tempRow[8],
          "service9",LT_TYPE_STRING,tempRow[9],
          "service10",LT_TYPE_STRING,tempRow[10],
          "service11",LT_TYPE_STRING,tempRow[11],
          "service12",LT_TYPE_STRING,tempRow[12],
          "service13",LT_TYPE_STRING,tempRow[13],
          "service14",LT_TYPE_STRING,tempRow[14],
          "service15",LT_TYPE_STRING,tempRow[15],
          "service16",LT_TYPE_STRING,tempRow[16],
          "service17",LT_TYPE_STRING,tempRow[17],
          "service18",LT_TYPE_STRING,tempRow[18],
          "service19",LT_TYPE_STRING,tempRow[19],
          "service20",LT_TYPE_STRING,tempRow[20],
          "service21",LT_TYPE_STRING,tempRow[21],
          "service22",LT_TYPE_STRING,tempRow[22],
          "service23",LT_TYPE_STRING,tempRow[23],
          "service24",LT_TYPE_STRING,tempRow[24],
          "service25",LT_TYPE_STRING,tempRow[25],
          "service26",LT_TYPE_STRING,tempRow[26],
          "service27",LT_TYPE_STRING,tempRow[27],
          "service28",LT_TYPE_STRING,tempRow[28],
          "service29",LT_TYPE_STRING,tempRow[29],
          "service30",LT_TYPE_STRING,tempRow[30],
          "overall",LT_TYPE_STRING,tempRow1[1]
        );        		   
		    sprintf(caTemp,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\r\n",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],
		    tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],
		    tempRow[12],tempRow[13],tempRow[14],tempRow[15],tempRow[16],tempRow[17],
		    tempRow[18],tempRow[19],tempRow[20],tempRow[21],tempRow[22],tempRow[23],
		    tempRow[24],tempRow[25],tempRow[26],tempRow[27],tempRow[28],tempRow[29],
		    tempRow[30],tempRow1[1]
		    );       
			  write(srcfd, caTemp, strlen(caTemp));		
        tempRow=ltDbFetchRow(tempCursor); 
        ltDbCloseCursor(tempCursor1);	
        ltDbClose(tempDbCon); /*¹Ø±ÕÊı¾İ¿â*/
        }        
       ltDbCloseCursor(tempCursor);
    } 
  lt_dbput_rootvars(dbPtr,1,"reportbdate",caSdate1);
  sprintf(caTempFile,"%soutflow1.gif",caTempDir);
  sprintf(caTemp,"/app/msa/msas/htmlplt/Scripts/flowpltline.pl   %s %s %u ",caTempDataDir,caTempFile,lMaxBytes);
  system(caTemp);
  lt_dbput_rootvars(dbPtr,1,"file1","outflow1.gif");   
  ltMsgPk->msgpktype=1;
  lt_page_content=ltPltFileParse("/app/msa/msas/htmlplt/reportplt/placeflow.htm",dbPtr->doc,0);
  if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","ÎŞ±¨¸æÊı¾İ,ÎŞ·¨Éú³É±¨¸æ",caTempDir);
            system(caLabel);/*½¨Á¢Ä¿Â¼ºÍÕıÔÚ´¦Àí±êÖ¾*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
    }else{
        lt_page_content=strstr(lt_page_content,"<!DOCTYPE");/*Éú³Éhtml¸ñÊ½±¨¸æ*/
        sprintf(caLabel,"%sindex.htm",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
       close(iFd);
    } 
    ltMsgPk->msgpktype=1;
    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >±¨¸æÕıÔÚÉú³ÉÖĞ,Çëµ½±¨¸æÁĞ±íÖĞ²ì¿´Éú³É×´Ì¬»òÕßÏÂÔØ±¨¸æ<br><a href=\"/main/srvreportlist.htm\" >²ì¿´ĞÂ±¨¸æ?</a></body></html>");      
    //pdf¸ñÊ½±¨¸æÉú³É´úÂë
    sprintf(caLabel,"/app/ns/bin/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
    printf("change to mht :: %s\n",caLabel);
    system(caLabel);
    chdir(caTempDir);
    system("tar -cvzf report.tgz *");
    sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
    system(caLabel);
    sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
    system(caLabel);
    ltMsgFree(ltMsgPk);
    return 0;
   	}	
    return 0;
}

¨¸æÁĞ±íÖĞ²ì¿´Éú³É×´Ì¬»òÕßÏÂÔØ±¨¸æ<br><a href=\"/main/srvreportlist.htm\" >²ì¿´ĞÂ±¨¸æ?</a></body></html>");      
    //pdf¸ñÊ½±¨¸æÉú³É´úÂë
    sprintf(caLabel,"/app/ns/bin/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
    printf("change to mht :: %s\n",caLabel);
    system(caLabel);
    chdir(caTempDir);
    system("tar -cvzf report.tgz *");
    sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
    system(caLabel);
    sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
    system(caLabel);
    ltMsgFree(ltMsgPk);
    return 0;
   	}	
    return 0;
}

