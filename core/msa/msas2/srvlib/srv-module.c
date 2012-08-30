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

int msasreportproc(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{	
		
	char caLabel[256];
	char caSdate[32],caEdate[32],caSdate1[32],caTime1[32],email[32];
	char *reportname;
	char caTempDir[256];
	char caTempFile[256];
	long lTime;
 	unsigned int time1,time2,Max;
		
 	char sqlBuf[1024];
	int srcfd,j,srcfd1;
	char caItem[16];
	char caTemp[18192];
	char caTempDataDir[256];
	char caTempDataDir1[256];
 	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	ltDbHeadPtr dbPtr;
	ltTablePtr  tablePtr;
	ltTablePtr  tablePtr1;

    
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
	  sprintf(caTempDir,"/app/msadatacenter/report/21/%lu/",ltStrGetId());
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            /// 生成该报告目录错误 
            ltMsgPk->msgpktype=1;
						lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >Error:报告生成错误,无法建立报告目录!<br><a href=\"javascript:history.back(1);\" >生成新报告?</a></body></html>");		
						ltMsgFree(ltMsgPk);
            return 0;
        }
   }
       
    sprintf(caLabel,"/bin/echo 'sname=%s' > %sinfo.ini",reportname,caTempDir);

    system(caLabel);/*建立目录标志*/
    
    sprintf(caLabel,"/bin/echo 'email=%s' >> %sinfo.ini",email,caTempDir);

    system(caLabel);/*建立目录标志*/
    
    sprintf(caLabel,"/bin/echo 'sdate=%s' >> %sinfo.ini",caSdate,caTempDir);

    system(caLabel);/*建立目录标志*/
    
    sprintf(caLabel,"/bin/echo 'edate=%s' >> %sinfo.ini",caEdate,caTempDir);

    system(caLabel);/*建立目录标志*/

    sprintf(caLabel,"/bin/echo 'tdate=%s/%s' >> %sinfo.ini",caSdate1,caTime1,caTempDir);

    system(caLabel);/*建立目录标志*/
 
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);

    system(caLabel);/*建立目录正在处理标志*/

	 sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report/left-1.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report/left-2.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report/left-3.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report/left-4.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report/banner.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report/right-top-d.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/

		sprintf(caTempDataDir,"%sdata",caTempDir);     
    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd == (-1)) {
	  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据文件写错误,无法生成报告",caTempDir);
		      system(caLabel);/*建立目录和正在处理标志*/
		    	ltMsgFree(ltMsgPk);
		        return 0;
		}

	   /*    模板开始     */ 
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
    sprintf(caTemp,"/app/msa/msas2/cgi/outflowpltline.pl   %s %s %u ",caTempDataDir,caTempFile,Max);
    system(caTemp);
    lt_dbput_rootvars(dbPtr,1,"file1","outflow1.gif");
	/* 模板2*/
		sprintf(caTempDataDir1,"%sdata1",caTempDir);     
    srcfd1 = open(caTempDataDir1,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd == (-1)) {
	  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据文件写错误,无法生成报告",caTempDir);
		      system(caLabel);/*建立目录和正在处理标志*/
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
    sprintf(caTemp,"/app/msa/msas2/cgi/outflowpltline.pl   %s %s %u ",caTempDataDir1,caTempFile,Max);
    system(caTemp);
    lt_dbput_rootvars(dbPtr,1,"file2","outflow2.gif");
	
  //ltMsgPk->msgpktype=1;
  //lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas2/model/report/sysinforeport.htm",dbPtr->head,0);
  char  *lt_page_content;
  int   iFd;
   lt_page_content=ltPltFileParse("/app/msa/msas2/model/report/sysinforeport.htm",dbPtr->doc,0);
   if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
            system(caLabel);/*建立目录和正在处理标志*/
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
      /*生成html格式报告*/
  }
  

   
    
	   ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >报告正在生成中,请到报告列表中察看生成状态或者下载报告<br><a href=\"/main/srvreportlist.htm\" >察看新报告?</a></body></html>");		

	  //system("/app/ns/java/html2fo/html2fo  index.htm index.fo ");
	  //chdir("/app/ns/java/fop-0.20.5");
	//  system("/app/ns/java/jdk1.6.0_01/jre/bin/java -classpath /app/ns/java/fop-0.20.5/:/app/ns/java/fop-0.20.5/lib/xml-apis.jar:/app/ns/java/fop-0.20.5/lib/xercesImpl-2.2.1.jar:/app/ns/java/fop-0.20.5/lib/xalan-2.4.1.jar:/app/ns/java/fop-0.20.5/lib/batik.jar:/app/ns/java/fop-0.20.5/lib/avalon-framework-cvs-20020806.jar:/app/ns/java/fop-0.20.5/build/fop.jar   myfop index.fo index.pdf");
	 //system(caLabel);
//      sprintf(caLabel,"/app/ns/bin/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
//      printf("change to mht :: %s\n",caLabel);
//      system(caLabel);
//	  system("tar -cvzf report.tgz *");
	  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	  system(caLabel);
//	
	  ltMsgFree(ltMsgPk);
 

      return 0;
	 
} 

/*  

场所状态报表   
		msastimeinfolog
	
		*/

int msasreportproc1(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{	
		
	char caLabel[256];
	char caSdate[32],caEdate[32],caSdate1[32],caTime1[32],email[32];
	char *reportname;
	char caTempDir[256];
	char caTempFile[256];
	long lTime;
 	unsigned int time1,time2,Max;
		
 	char sqlBuf[1024];
	int srcfd;
	char caItem[16];
	char caTemp[18192];
	char caTempDataDir[256];
 	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	ltDbHeadPtr dbPtr;
	ltTablePtr  tablePtr;

    
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
	  sprintf(caTempDir,"/app/msadatacenter/report/22/%lu/",ltStrGetId());
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            /// 生成该报告目录错误 
            ltMsgPk->msgpktype=1;
						lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >Error:报告生成错误,无法建立报告目录!<br><a href=\"javascript:history.back(1);\" >生成新报告?</a></body></html>");		
						ltMsgFree(ltMsgPk);
            return 0;
        }
   }     
    sprintf(caLabel,"/bin/echo 'sname=%s' > %sinfo.ini",reportname,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'sdate=%s' >> %sinfo.ini",caSdate,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'edate=%s' >> %sinfo.ini",caEdate,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'email=%s' >> %sinfo.ini",email,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'tdate=%s/%s' >> %sinfo.ini",caSdate1,caTime1,caTempDir);
    system(caLabel);/*建立目录标志*/ 
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	 sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_1/left-1.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
 	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_1/left-2.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_1/left-3.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_1/left-4.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_1/banner.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_1/right-top-d.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/

		sprintf(caTempDataDir,"%sdata",caTempDir);     
    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd == (-1)) {
	  	        sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据文件写错误,无法生成报告",caTempDir);
		        system(caLabel);/*建立目录和正在处理标志*/
		    	ltMsgFree(ltMsgPk);
		        return 0;
		}

	   /*    模板开始     */ 
    dbPtr=lt_dbinit();
    tablePtr=lt_dbput_table(dbPtr,"list");
		Max = 100;
    sprintf(sqlBuf,"select gnmkey,cast(avg(cpunum) as dec(18,2)) as cpunum,cast(avg(memnum) as dec(18,2)) as memnum,cast(avg(sysdisknum) as dec(18,2)) as usernum,cast(avg(tcpnum) as dec(18,2)) as tcpnum,cast(avg(udpnum) as dec(18,2)) as udpnum,cast(avg(urlnum) as dec(18,2)) as urlnum,cast(avg(bytenum) as dec(18,2)) as bytenum,cast(avg(bytenumu) as dec(18,2)) as bytenumu,cast(avg(bytenumd) as dec(18,2)) as bytenumd from msastimeinfolog where rtime >=%u and rtime <=%u group by gnmkey order by gnmkey asc",time2,time1);
    
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
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
				 
			          sprintf(caTemp,"%s,%llu,%llu,%llu,\r\n",
			        	caItem,
						atoll(tempRow[1]),
						atoll(tempRow[2]),
						atoll(tempRow[3]));
			          write(srcfd, caTemp, strlen(caTemp));	
			      				 
               tempRow=ltDbFetchRow(tempCursor);
                }
         ltDbCloseCursor(tempCursor);
    }
     lt_dbput_rootvars(dbPtr,1,"reportbdate",caSdate1);	
		sprintf(caTempFile,"%soutflow2.gif",caTempDir);
    sprintf(caTemp,"/app/msa/msas2/cgi/outflowpltline.pl   %s %s %u ",caTempDataDir,caTempFile,Max);
    system(caTemp);
    lt_dbput_rootvars(dbPtr,1,"file1","outflow2.gif");
	
  //ltMsgPk->msgpktype=1;
  //lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas2/model/report/sysinforeport.htm",dbPtr->head,0);
  char  *lt_page_content;
  int   iFd;
   lt_page_content=ltPltFileParse("/app/msa/msas2/model/report_1/timeinforeport.htm",dbPtr->doc,0);
   if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
            system(caLabel);/*建立目录和正在处理标志*/
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
      /*生成html格式报告*/
  }
  

   
    
    ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >报告正在生成中,请到报告列表中察看生成状态或者下载报告<br><a href=\"/main/srvreportlist.htm\" >察看新报告?</a></body></html>");		

	  //system("/app/ns/java/html2fo/html2fo  index.htm index.fo ");
	  //chdir("/app/ns/java/fop-0.20.5");
	//  system("/app/ns/java/jdk1.6.0_01/jre/bin/java -classpath /app/ns/java/fop-0.20.5/:/app/ns/java/fop-0.20.5/lib/xml-apis.jar:/app/ns/java/fop-0.20.5/lib/xercesImpl-2.2.1.jar:/app/ns/java/fop-0.20.5/lib/xalan-2.4.1.jar:/app/ns/java/fop-0.20.5/lib/batik.jar:/app/ns/java/fop-0.20.5/lib/avalon-framework-cvs-20020806.jar:/app/ns/java/fop-0.20.5/build/fop.jar   myfop index.fo index.pdf");
	 //system(caLabel);
//      sprintf(caLabel,"/app/ns/bin/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
//      printf("change to mht :: %s\n",caLabel);
//      system(caLabel);
//	  system("tar -cvzf report.tgz *");
	  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	  system(caLabel);
//	
	  ltMsgFree(ltMsgPk);
 

      return 0;
	 
} 


int msasreportproc2(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{	
		
	char caLabel[256];
  char caSdate[32],caEdate[32],caSdate1[32],caTime1[32],email[32];
  char *reportname;
  char caTempDir[256];
  char caTempFile[256];
	long lTime;
 	unsigned int time1,time2,Max;
 	char sqlBuf[1024],strBuf[1024];
	int srcfd;
	float temprate,sumnum,tempnum;
	char caItem[16],rate[32];
	char caTemp[18192];
	char caTempDataDir[256];
 	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	ltDbHeadPtr dbPtr;
	ltTablePtr  tablePtr;
	
	ltDbConn *tempDbCon;
  char *dbUser;
  char *dbPass;
	char *dbName;
 	ltDbCursor *tempCursor1;
 	LT_DBROW tempRow1;

    
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
	  sprintf(caTempDir,"/app/msadatacenter/report/23/%lu/",ltStrGetId());
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            /// 生成该报告目录错误 
            ltMsgPk->msgpktype=1;
						lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >Error:报告生成错误,无法建立报告目录!<br><a href=\"javascript:history.back(1);\" >生成新报告?</a></body></html>");		
						ltMsgFree(ltMsgPk);
            return 0;
        }
   }
       
    sprintf(caLabel,"/bin/echo 'sname=%s' > %sinfo.ini",reportname,caTempDir);

    system(caLabel);/*建立目录标志*/

    sprintf(caLabel,"/bin/echo 'sdate=%s' >> %sinfo.ini",caSdate,caTempDir);

    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'edate=%s' >> %sinfo.ini",caEdate,caTempDir);

    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'email=%s' >> %sinfo.ini",email,caTempDir);

    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'tdate=%s/%s' >> %sinfo.ini",caSdate1,caTime1,caTempDir);

    system(caLabel);/*建立目录标志*/
 
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);

    system(caLabel);/*建立目录正在处理标志*/

	 sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_2/left-1.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_2/left-2.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_2/left-3.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_2/left-4.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_2/banner.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_2/right-top-d.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/

		sprintf(caTempDataDir,"%sdata",caTempDir);     
    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd == (-1)) {
	  	        sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据文件写错误,无法生成报告",caTempDir);
		        system(caLabel);/*建立目录和正在处理标志*/
		    	ltMsgFree(ltMsgPk);
		        return 0;
		}

	   /*    模板开始     */ 
    dbPtr=lt_dbinit();
    tablePtr=lt_dbput_table(dbPtr,"list");
		Max = 100;
    sprintf(sqlBuf,"select gnmkey,cast(avg(usernum) as dec(18,0)) from msastimeinfolog where rtime >= %u and rtime <=%u group by gnmkey order by gnmkey asc",time2,time1);
    
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
    if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
                while(tempRow!=NULL){				
								sprintf(caItem,"%s",tempRow[0]);
								tempnum=(atol(tempRow[1]));
									/*二次查询*/								  
    							dbName=_ltPubInfo->_dbname;
	  							dbUser=_ltPubInfo->_dbuser;
	  							dbPass=_ltPubInfo->_dbpass;
    							tempDbCon=ltDbConnect(dbUser,dbPass,dbName);
    							if (tempDbCon==NULL){
										ltMsgPk->msgpktype=1;
										lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >系统数据连接无法打开<br><a href=\"javascript:history.back(1);\" >返回</a></body></html>");		
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
									ltDbClose(tempDbCon); /*关闭数据库*/	
									/*二次查询结束*/					          	      
                tempRow=ltDbFetchRow(tempCursor);
                }
         ltDbCloseCursor(tempCursor);
    }
     lt_dbput_rootvars(dbPtr,1,"reportbdate",caSdate1);
	   sprintf(caTempFile,"%soutflow2.gif",caTempDir);
     sprintf(caTemp,"/app/msa/msas2/cgi/outflowpltline2.pl   %s %s %u ",caTempDataDir,caTempFile,Max);
     system(caTemp);
     lt_dbput_rootvars(dbPtr,1,"file1","outflow2.gif");
	
  //ltMsgPk->msgpktype=1;
  //lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas2/model/report/sysinforeport.htm",dbPtr->head,0);
   char  *lt_page_content;
   int   iFd;
   lt_page_content=ltPltFileParse("/app/msa/msas2/model/report_2/timeinforeport.htm",dbPtr->doc,0);
   if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
            system(caLabel);/*建立目录和正在处理标志*/
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
      /*生成html格式报告*/
  }
  

   
    
    ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >报告正在生成中,请到报告列表中察看生成状态或者下载报告<br><a href=\"/main/srvreportlist.htm\" >察看新报告?</a></body></html>");		

	  //system("/app/ns/java/html2fo/html2fo  index.htm index.fo ");
	  //chdir("/app/ns/java/fop-0.20.5");
	//  system("/app/ns/java/jdk1.6.0_01/jre/bin/java -classpath /app/ns/java/fop-0.20.5/:/app/ns/java/fop-0.20.5/lib/xml-apis.jar:/app/ns/java/fop-0.20.5/lib/xercesImpl-2.2.1.jar:/app/ns/java/fop-0.20.5/lib/xalan-2.4.1.jar:/app/ns/java/fop-0.20.5/lib/batik.jar:/app/ns/java/fop-0.20.5/lib/avalon-framework-cvs-20020806.jar:/app/ns/java/fop-0.20.5/build/fop.jar   myfop index.fo index.pdf");
	 //system(caLabel);
//      sprintf(caLabel,"/app/ns/bin/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
//      printf("change to mht :: %s\n",caLabel);
//      system(caLabel);
//	  system("tar -cvzf report.tgz *");
	  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	  system(caLabel);
//	
	  ltMsgFree(ltMsgPk);
 

      return 0;
	 
}  
/*
	服务汇总报表

*/
int msasreportproc3(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{	
	
	char caLabel[256];
	char caSdate[32],caEdate[32],caSdate1[32],caTime1[32],email[32],serve[32];
	char *reportname;
	char caTempDir[256];
	char caTempFile[256];
	long lTime;
 	unsigned int time1,time2,Max;
		
 	char sqlBuf[2048];
	int srcfd,i;
	char caTemp[18192];
	char caTempDataDir[256];
 	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	ltDbHeadPtr dbPtr;
	ltTablePtr  tablePtr;

    
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
	  sprintf(caTempDir,"/app/msadatacenter/report/24/%lu/",ltStrGetId());
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            /// 生成该报告目录错误 
            ltMsgPk->msgpktype=1;
						lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >Error:报告生成错误,无法建立报告目录!<br><a href=\"javascript:history.back(1);\" >生成新报告?</a></body></html>");		
						ltMsgFree(ltMsgPk);
            return 0;
        }
   }
      
    sprintf(caLabel,"/bin/echo 'sname=%s' > %sinfo.ini",reportname,caTempDir);

    system(caLabel);/*建立目录标志*/
    
    sprintf(caLabel,"/bin/echo 'email=%s' >> %sinfo.ini",email,caTempDir);

    system(caLabel);/*建立目录标志*/
    
    sprintf(caLabel,"/bin/echo 'sdate=%s' >> %sinfo.ini",caSdate,caTempDir);

    system(caLabel);/*建立目录标志*/
    
    sprintf(caLabel,"/bin/echo 'edate=%s' >> %sinfo.ini",caEdate,caTempDir);

    system(caLabel);/*建立目录标志*/

    sprintf(caLabel,"/bin/echo 'tdate=%s/%s' >> %sinfo.ini",caSdate1,caTime1,caTempDir);

    system(caLabel);/*建立目录标志*/
 
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);

    system(caLabel);/*建立目录正在处理标志*/

	 sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_3/left-1.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_3/left-2.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_3/left-3.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_3/left-4.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_3/banner.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
	sprintf(caLabel,"/bin/cp /app/msa/msas2/model/report_3/right-top-d.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/

		sprintf(caTempDataDir,"%sdata",caTempDir);     

    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd == (-1)) {
	  	    sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据文件写错误,无法生成报告",caTempDir);
		      system(caLabel);/*建立目录和正在处理标志*/
		    	ltMsgFree(ltMsgPk);
		        return 0;
		}

	   /*    模板开始     */ 
	   
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
    sprintf(caTemp,"/app/msa/msas2/cgi/onlinesrv1_s.pl   %s %s %u ",caTempDataDir,caTempFile,Max);
    system(caTemp);
    lt_dbput_rootvars(dbPtr,1,"file1","outflow1.gif");
	
  //ltMsgPk->msgpktype=1;
  //lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas2/model/report/sysinforeport.htm",dbPtr->head,0);
  char  *lt_page_content;
  int   iFd;
   lt_page_content=ltPltFileParse("/app/msa/msas2/model/report_3/timeinforeport.htm",dbPtr->doc,0);
   if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
            system(caLabel);/*建立目录和正在处理标志*/
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
      /*生成html格式报告*/
  }
      
	   ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >报告正在生成中,请到报告列表中察看生成状态或者下载报告<br><a href=\"/main/srvreportlist.htm\" >察看新报告?</a></body></html>");		

	  //system("/app/ns/java/html2fo/html2fo  index.htm index.fo ");
	  //chdir("/app/ns/java/fop-0.20.5");
	//  system("/app/ns/java/jdk1.6.0_01/jre/bin/java -classpath /app/ns/java/fop-0.20.5/:/app/ns/java/fop-0.20.5/lib/xml-apis.jar:/app/ns/java/fop-0.20.5/lib/xercesImpl-2.2.1.jar:/app/ns/java/fop-0.20.5/lib/xalan-2.4.1.jar:/app/ns/java/fop-0.20.5/lib/batik.jar:/app/ns/java/fop-0.20.5/lib/avalon-framework-cvs-20020806.jar:/app/ns/java/fop-0.20.5/build/fop.jar   myfop index.fo index.pdf");
	 // system(caLabel);
//    sprintf(caLabel,"/app/ns/bin/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
//    printf("change to mht :: %s\n",caLabel);
//    system(caLabel);
//	  system("tar -cvzf report.tgz *");
	  sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
	  system(caLabel);
	  sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
	  system(caLabel);
//	
	  ltMsgFree(ltMsgPk);
      return 0;
	 
}
