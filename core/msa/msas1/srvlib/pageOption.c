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

//此函数作用为：ajax调用生成报告
/*生成报告一函数处理*/
int msasreportproc1(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{   
    long lTime;
    char caLabel[256];
    char caSdate[32],caEdate[32],caSdate1[32],caTime1[32];
    char *reportname;

    int  iFd;
    char caTempDir[256];
    char email[25];
    int srcfd;
    unsigned int number=0;
    char caTempFile[256];
    char caTempDataDir[256];
    char caTemp[18192];
    
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;
    char sqlBuf[1024];
    char *lt_page_content;

    printf("-----------------------------------------sdate\n");

    if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){//传入客户端参数sdate
    sprintf(caSdate,"%s","");
      }else{
           sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
           }
           
    printf("------------------------------------------edate\n");
    
    if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
    sprintf(caEdate,"%s","");
      }else{
            sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
           }

    printf("-------------------------------------------reportname\n");
         
    reportname=ltMsgGetVar_s(ltMsgPk,"reportname");
    if(reportname==NULL){
        reportname="********";
     }
    
    printf("-----------------caSdate---------------------caEdate\n"); 
    printf("%s\n",caSdate);
    printf("%s\n",caEdate); 
  
   // time1 = nasCvtLongTime(caSdate,"00:00:00");
   // time2 = nasCvtLongTime(caEdate,"23:59:59");
      
   /* unsigned int time_1,time_2;
    time_1 = nasCvtLongTime("10-10-29","00:00:00");
    time_2 = nasCvtLongTime("10-09-29","23:59:59"); 
    printf("%d\n",time1);
    printf("%d\n",time2);
    printf("%d\n",time_1);
    printf("%d\n",time_2);
    */
    printf("-----------------------------------time\n");
      
    lTime = time(0);
	  nasCvtStime(lTime,caSdate1,caTime1);
	  caTime1[5]=0;
     
    sprintf(caTempDir,"/app/msadatacenter/report/1/%lu/",ltStrGetId());
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            //生成该报告目录错误 
            ltMsgPk->msgpktype=1;
                        lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >Error:报告生成错误,无法建立报告目录!<br><a href=\"javascript:history.back(1);\" >生成新报告?</a></body></html>");     
                        ltMsgFree(ltMsgPk);
            return 0;
        }

   }  
    
    printf("---------sname---------sdate----------sname--------suser\n");

    sprintf(caLabel,"/bin/echo 'sname=%s' > %sinfo.ini",reportname,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'sdate=%s' >> %sinfo.ini",caSdate,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'edate=%s' >> %sinfo.ini",caEdate,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'sysdate=%s/%s' >> %sinfo.ini",caSdate1,caTime1,caTempDir);
    system(caLabel);/*建立目录标志*/
    
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);

    printf("55555555555555555\n");

    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/right-top.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/banner.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/up-1.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/up-2.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/up-3.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    
    sprintf(caTempDataDir,"%sdata",caTempDir);     
    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd == (-1)) {
	  	        sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据文件写错误,无法生成报告",caTempDir);
		        system(caLabel);/*建立目录和正在处理标志*/
		    	ltMsgFree(ltMsgPk);
		        return 0;
		}
    
     
    /*运用网页模板生成报表*/
   
    printf("444444444444444444444444444444444444444444444\n");

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
    
  number=number*1.3;
  printf("%d",number);
  lt_dbput_rootvars(dbPtr,1,"reportbdate",caSdate1);

  sprintf(caTempFile,"%soutflow1.gif",caTempDir);
  sprintf(caTemp,"/app/msa/msas1/cgi/outflowplt.pl   %s %s %d ",caTempDataDir,caTempFile,number);
  system(caTemp);
  lt_dbput_rootvars(dbPtr,1,"file1","outflow1.gif");

      
  ltMsgPk->msgpktype=1;
  
  lt_page_content=ltPltFileParse("/app/msa/msas/htmlplt/test1/moldboard/model.htm",dbPtr->doc,0);
   if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
            system(caLabel);/*建立目录和正在处理标志*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
  }else{
        lt_page_content=strstr(lt_page_content,"<html");
        sprintf(caLabel,"%sindex.htm",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
       close(iFd);
      /*生成html格式报告*/
  }

    printf("5555555555555555555555555555555555555\n");
    
    ltMsgPk->msgpktype=1;
 
    printf("6666666666666666666666666666666666666\n");

    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >报告正在生成中,请到报告列表中察看生成状态或者下载报告<br><a href=\"/main/srvreportlist.htm\" >察看新报告?</a></body></html>");

    printf("77777777777777777777777777777777777777\n");        

      //system("/app/ns/java/html2fo/html2fo  index.htm index.fo ");
      //chdir("/app/ns/java/fop-0.20.5");
    //  system("/app/ns/java/jdk1.6.0_01/jre/bin/java -classpath /app/ns/java/fop-0.20.5/:/app/ns/java/fop-0.20.5/lib/xml-apis.jar:/app/ns/java/fop-0.20.5/lib/xercesImpl-2.2.1.jar:/app/ns/java/fop-0.20.5/lib/xalan-2.4.1.jar:/app/ns/java/fop-0.20.5/lib/batik.jar:/app/ns/java/fop-0.20.5/lib/avalon-framework-cvs-20020806.jar:/app/ns/java/fop-0.20.5/build/fop.jar   myfop index.fo index.pdf");
     //system(caLabel);
    sprintf(caLabel,"/app/ns/bin/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
    printf("change to mht :: %s\n",caLabel);
    system(caLabel);
    system("tar -cvzf report.tgz *");
    sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
    system(caLabel);
    sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
    system(caLabel);   
    ltMsgFree(ltMsgPk);
 
    return 0;   
} 

/*场所报警统计报告 生成报告二函数处理*/
int msasreportproc2(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{   
    long lTime;
    char caLabel[256];
    char caSdate[32],caEdate[32],caSdate1[32],caTime1[32];
    char *reportname;

    int  iFd;
    char caTempDir[256];
    char email[25];
    int srcfd;
    unsigned int lMaxBytes;
    char caTempFile[256];
    char caTempDataDir[256];
    char caTemp[18192];
    
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;
    char sqlBuf[1024];
    char  *lt_page_content;

    printf("11111111111111111111111111111111111\n");

    if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){//传入客户端参数sdate
    sprintf(caSdate,"%s","");
      }else{
           sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
           }
           
    printf("1111111111111111111111111111111111111\n");
    
    if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
    sprintf(caEdate,"%s","");
      }else{
            sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
           }

    printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
         
    reportname=ltMsgGetVar_s(ltMsgPk,"reportname");
    if(reportname==NULL){
        reportname="********";
     }
    
    printf("22222222222222222222222222222222222222\n"); 
    printf("%s\n",caSdate);
    printf("%s\n",caEdate); 
    printf("3333333333333333333333333333333333333333333\n");
      
    lTime = time(0);
	  nasCvtStime(lTime,caSdate1,caTime1);//将日期 和 时间 分开
	  caTime1[5]=0;
     
    sprintf(caTempDir,"/app/msadatacenter/report/2/%lu/",ltStrGetId());
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            //生成该报告目录错误 
            ltMsgPk->msgpktype=1;
                        lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >Error:报告生成错误,无法建立报告目录!<br><a href=\"javascript:history.back(1);\" >生成新报告?</a></body></html>");     
                        ltMsgFree(ltMsgPk);
            return 0;
        }

   }  
    
    printf("44444444444444444444444444444444444444444\n");

    sprintf(caLabel,"/bin/echo 'sname=%s' > %sinfo.ini",reportname,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'sdate=%s' >> %sinfo.ini",caSdate,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'edate=%s' >> %sinfo.ini",caEdate,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'sysdate=%s/%s' >> %sinfo.ini",caSdate1,caTime1,caTempDir);
    system(caLabel);/*建立目录标志*/
 
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);

    printf("55555555555555555\n");

    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/right-top.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/banner.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/up-1.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/up-2.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/up-3.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    
    sprintf(caTempDataDir,"%sdata",caTempDir);     
    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
    printf("%d\n",srcfd);
	  if(srcfd == (-1)) {
	  	 sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据文件写错误,无法生成报告",caTempDir);
		   system(caLabel);/*建立目录和正在处理标志*/
		   ltMsgFree(ltMsgPk);
		   return 0;
		}
      
    /*运用网页模板生成报表*/
   
    printf("44444444444444444444\n");

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
    lMaxBytes =10;
    sprintf(sqlBuf,"select gnmkey,count(*) as number from AlertList where Matching_time>='%s' and Matching_time<='%s' group by gnmkey;",caEdate,caSdate);
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
		       
		   /*二次查询*/
			 ltDbConn *tempDbCon;
    	 char *dbUser; char *dbPass; char *dbName; char strBuf[1024];
    	 ltDbCursor *tempCursor1; 
    	 LT_DBROW tempRow1;
    							dbName=_ltPubInfo->_dbname;
	  							dbUser=_ltPubInfo->_dbuser;
	  							dbPass=_ltPubInfo->_dbpass;
    							tempDbCon=ltDbConnect(dbUser,dbPass,dbName);
    							if (tempDbCon==NULL){
										ltMsgPk->msgpktype=1;
										lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >系统数据连接无法打开<br><a href=\"javascript:history.back(1);\" >返回</a></body></html>");		
										ltMsgFree(ltMsgPk);
										return 0;}     							
										sprintf(strBuf,"select sname from alertlist where gnmkey in('%s')",tempRow[0]);
								 		tempCursor1=ltDbOpenCursor(tempDbCon,strBuf);
									if(tempCursor1!=NULL){
													tempRow1= ltDbFetchRow(tempCursor1);												    																								 																																																
									}
									ltDbClose(tempDbCon); /*关闭数据库*/	
									/*二次查询结束*/							
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
  sprintf(caTemp,"/app/msa/msas1/cgi/outflowplt.pl   %s %s %d ",caTempDataDir,caTempFile,lMaxBytes);
  system(caTemp);
  lt_dbput_rootvars(dbPtr,1,"file1","outflow1.gif");

      
  ltMsgPk->msgpktype=1;
  
  lt_page_content=ltPltFileParse("/app/msa/msas/htmlplt/test1/moldboard/gnmkey_model.htm",dbPtr->doc,0);
   if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
            system(caLabel);/*建立目录和正在处理标志*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
  }else{
        lt_page_content=strstr(lt_page_content,"<html");
        sprintf(caLabel,"%sindex.htm",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
       close(iFd);
      /*生成html格式报告*/
  }

    printf("55555555555555555555\n");
    
    ltMsgPk->msgpktype=1;
 
    printf("6666666666666666666\n");

    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >报告正在生成中,请到报告列表中察看生成状态或者下载报告<br><a href=\"/main/srvreportlist.htm\" >察看新报告?</a></body></html>");

   printf("777777777777777777777777\n");        

      //system("/app/ns/java/html2fo/html2fo  index.htm index.fo ");
      //chdir("/app/ns/java/fop-0.20.5");
    //  system("/app/ns/java/jdk1.6.0_01/jre/bin/java -classpath /app/ns/java/fop-0.20.5/:/app/ns/java/fop-0.20.5/lib/xml-apis.jar:/app/ns/java/fop-0.20.5/lib/xercesImpl-2.2.1.jar:/app/ns/java/fop-0.20.5/lib/xalan-2.4.1.jar:/app/ns/java/fop-0.20.5/lib/batik.jar:/app/ns/java/fop-0.20.5/lib/avalon-framework-cvs-20020806.jar:/app/ns/java/fop-0.20.5/build/fop.jar   myfop index.fo index.pdf");
     //system(caLabel);
    sprintf(caLabel,"/app/ns/bin/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
    printf("change to mht :: %s\n",caLabel);
    system(caLabel);
      system("tar -cvzf report.tgz *");
      sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
      system(caLabel);
      sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
      system(caLabel);
    
      ltMsgFree(ltMsgPk);
 

      return 0;
     
}

/*生成报告三函数处理*/
int msasreportproc3(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{   
    long lTime;
    char caLabel[256];
    char caSdate[32],caEdate[32],caSdate1[32],caTime1[32];
    char *reportname;

    int  iFd;
    char caTempDir[256];
    char email[25];
    int srcfd;
    //unsigned int time1,time2;
    unsigned int lMaxBytes;
    char caTempFile[256];
    char caTempDataDir[256];
    char caTemp[18192];
    
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;
    char sqlBuf[1024];
    char  *lt_page_content;

    printf("11111111111111111\n");

    if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){//传入客户端参数sdate
    sprintf(caSdate,"%s","");
      }else{
           sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
           }
           
    printf("11111111111111111\n");
    
    if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
    sprintf(caEdate,"%s","");
      }else{
            sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
           }

    printf("aaaaaaaaaaaaaa\n");
         
    reportname=ltMsgGetVar_s(ltMsgPk,"reportname");
    if(reportname==NULL){
        reportname="********";
     }
    
    printf("222222222222222222\n"); 
    printf("%s\n",caSdate);
    printf("%s\n",caEdate); 
  
   // time1 = nasCvtLongTime(caSdate,"00:00:00");
   // time2 = nasCvtLongTime(caEdate,"23:59:59");
      
   /*unsigned int time_1,time_2;
    time_1 = nasCvtLongTime("10-10-29","00:00:00");
    time_2 = nasCvtLongTime("10-09-29","23:59:59"); 
    printf("%d\n",time1);
    printf("%d\n",time2);
    printf("%d\n",time_1);
    printf("%d\n",time_2);
    */
    printf("3333333333333333333\n");
      
    lTime = time(0);//获取当前系统时间
	  nasCvtStime(lTime,caSdate1,caTime1);
	  caTime1[5]=0;
     
    sprintf(caTempDir,"/app/msadatacenter/report/3/%lu/",ltStrGetId());
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            //生成该报告目录错误 
            ltMsgPk->msgpktype=1;
                        lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >Error:报告生成错误,无法建立报告目录!<br><a href=\"javascript:history.back(1);\" >生成新报告?</a></body></html>");     
                        ltMsgFree(ltMsgPk);
            return 0;
        }

   }  
    
    printf("44444444444444444444444\n");

    sprintf(caLabel,"/bin/echo 'sname=%s' > %sinfo.ini",reportname,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'sdate=%s' >> %sinfo.ini",caSdate,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'edate=%s' >> %sinfo.ini",caEdate,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'sysdate=%s/%s' >> %sinfo.ini",caSdate1,caTime1,caTempDir);
    system(caLabel);/*建立目录标志*/
 
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);

    printf("55555555555555555\n");

    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/right-top.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/banner.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/up-1.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/up-2.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/up-3.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    
    sprintf(caTempDataDir,"%sdata",caTempDir);     
    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd == (-1)) {
	  	        sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据文件写错误,无法生成报告",caTempDir);
		        system(caLabel);/*建立目录和正在处理标志*/
		    	ltMsgFree(ltMsgPk);
		        return 0;
		}
    
     
    /*运用网页模板生成报表*/
   
    printf("44444444444444444444\n");

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
  sprintf(caTemp,"/app/msa/msas1/cgi/outflowplt.pl   %s %s %d ",caTempDataDir,caTempFile,lMaxBytes);
  system(caTemp);
  lt_dbput_rootvars(dbPtr,1,"file1","outflow1.gif");

      
  ltMsgPk->msgpktype=1;
  
  lt_page_content=ltPltFileParse("/app/msa/msas/htmlplt/test1/moldboard/Alarm_model.htm",dbPtr->doc,0);
   if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
            system(caLabel);/*建立目录和正在处理标志*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
  }else{
        lt_page_content=strstr(lt_page_content,"<html");
        sprintf(caLabel,"%sindex.htm",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
       close(iFd);
      /*生成html格式报告*/
  }

    printf("55555555555555555555\n");
    
    ltMsgPk->msgpktype=1;
 
    printf("6666666666666666666\n");

    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >报告正在生成中,请到报告列表中察看生成状态或者下载报告<br><a href=\"/main/srvreportlist.htm\" >察看新报告?</a></body></html>");

   printf("777777777777777777777777\n");        

      //system("/app/ns/java/html2fo/html2fo  index.htm index.fo ");
      //chdir("/app/ns/java/fop-0.20.5");
    //  system("/app/ns/java/jdk1.6.0_01/jre/bin/java -classpath /app/ns/java/fop-0.20.5/:/app/ns/java/fop-0.20.5/lib/xml-apis.jar:/app/ns/java/fop-0.20.5/lib/xercesImpl-2.2.1.jar:/app/ns/java/fop-0.20.5/lib/xalan-2.4.1.jar:/app/ns/java/fop-0.20.5/lib/batik.jar:/app/ns/java/fop-0.20.5/lib/avalon-framework-cvs-20020806.jar:/app/ns/java/fop-0.20.5/build/fop.jar   myfop index.fo index.pdf");
     //system(caLabel);
    sprintf(caLabel,"/app/ns/bin/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
    printf("change to mht :: %s\n",caLabel);
    system(caLabel);
    system("tar -cvzf report.tgz *");
    sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
    system(caLabel);
    sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
    system(caLabel);
    ltMsgFree(ltMsgPk);
    return 0;
}

/*生成场所流量报表函数处理*/

int msasreportproc4(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{   
    long lTime;
    char caLabel[256];
    char caSdate[32],caEdate[32],caSdate1[32],caTime1[32];
    char *reportname;

    int  iFd;
    char caTempDir[256];
    char email[25];
    int srcfd;
    unsigned int lMaxBytes;
    char caTempFile[256];
    char caTempDataDir[256];
    char caTemp[18192];
    
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;
    char sqlBuf[1024];
    char  *lt_page_content;

    printf("11111111111111111111111111111111111\n");

    if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){//传入客户端参数sdate
    sprintf(caSdate,"%s","");
      }else{
           sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
           }
           
    printf("1111111111111111111111111111111111111\n");
    
    if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
    sprintf(caEdate,"%s","");
      }else{
            sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
           }

    printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
         
    reportname=ltMsgGetVar_s(ltMsgPk,"reportname");
    if(reportname==NULL){
        reportname="********";
     }
    
    printf("22222222222222222222222222222222222222\n"); 
    printf("%s\n",caSdate);
    printf("%s\n",caEdate); 
  
   // time1 = nasCvtLongTime(caSdate,"00:00:00");
   // time2 = nasCvtLongTime(caEdate,"23:59:59");
      
   /* unsigned int time_1,time_2;
    time_1 = nasCvtLongTime("10-10-29","00:00:00");
    time_2 = nasCvtLongTime("10-09-29","23:59:59"); 
    printf("%d\n",time1);
    printf("%d\n",time2);
    printf("%d\n",time_1);
    printf("%d\n",time_2);
    */
    printf("3333333333333333333333333333333333333333333\n");
      
    lTime = time(0);
	  nasCvtStime(lTime,caSdate1,caTime1);
	  caTime1[5]=0;
     
    sprintf(caTempDir,"/app/msadatacenter/report/4/%lu/",ltStrGetId());
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            //生成该报告目录错误 
            ltMsgPk->msgpktype=1;
                        lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >Error:报告生成错误,无法建立报告目录!<br><a href=\"javascript:history.back(1);\" >生成新报告?</a></body></html>");     
                        ltMsgFree(ltMsgPk);
            return 0;
        }

   }  
    
    printf("44444444444444444444444444444444444444444\n");

   sprintf(caLabel,"/bin/echo 'sname=%s' > %sinfo.ini",reportname,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'sdate=%s' >> %sinfo.ini",caSdate,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'edate=%s' >> %sinfo.ini",caEdate,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'sysdate=%s/%s' >> %sinfo.ini",caSdate1,caTime1,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);

    printf("55555555555555555\n");

    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/right-top.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/banner.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/up-1.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/up-2.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/up-3.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    
    sprintf(caTempDataDir,"%sdata",caTempDir);     
    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
    printf("%d\n",srcfd);
	  if(srcfd == (-1)) {
	  	 sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据文件写错误,无法生成报告",caTempDir);
		   system(caLabel);/*建立目录和正在处理标志*/
		   ltMsgFree(ltMsgPk);
		   return 0;
		}
    
     
    /*运用网页模板生成报表*/
   
    printf("44444444444444444444\n");

    //char useracoount[32];
    
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
		   /*二次查询*/
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
					lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >系统数据连接无法打开<br><a href=\"javascript:history.back(1);\" >返回</a></body></html>");		
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
					   								
				printf("aaaaaaaaaaaaaaaaaaa*\n");	
										
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
           printf("bbbbbbbbbbbbbbbb\n");     		   
		       sprintf(caTemp,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\r\n",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],
		       tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],
		       tempRow[12],tempRow[13],tempRow[14],tempRow[15],tempRow[16],tempRow[17],
		       tempRow[18],tempRow[19],tempRow[20],tempRow[21],tempRow[22],tempRow[23],
		       tempRow[24],tempRow[25],tempRow[26],tempRow[27],tempRow[28],tempRow[29],
		       tempRow[30],tempRow1[1]
		       );
		       
		       printf("bbbbbbbbbbbbbbbb\n");       
			     write(srcfd, caTemp, strlen(caTemp));		
           tempRow=ltDbFetchRow(tempCursor);
           
           ltDbCloseCursor(tempCursor1);	
           ltDbClose(tempDbCon); /*关闭数据库*/
           }        
         ltDbCloseCursor(tempCursor);
    }
   
  printf("cccccccccccccccccccccc\n");  
  lt_dbput_rootvars(dbPtr,1,"reportbdate",caSdate1);

  printf("ddddddddddddddddddddddddd\n"); 
  sprintf(caTempFile,"%soutflow1.gif",caTempDir);
  sprintf(caTemp,"/app/msa/msas1/cgi/outflowpltline.pl   %s %s %u ",caTempDataDir,caTempFile,lMaxBytes);
  system(caTemp);
  lt_dbput_rootvars(dbPtr,1,"file1","outflow1.gif");

      
  ltMsgPk->msgpktype=1;
  
  lt_page_content=ltPltFileParse("/app/msa/msas/htmlplt/test1/moldboard/placeflow.htm",dbPtr->doc,0);
   if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
            system(caLabel);/*建立目录和正在处理标志*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
  }else{
        lt_page_content=strstr(lt_page_content,"<html");
        sprintf(caLabel,"%sindex.htm",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
       close(iFd);
      /*生成html格式报告*/
  }

    printf("55555555555555555555\n");
    
    ltMsgPk->msgpktype=1;
 
    printf("6666666666666666666\n");

    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >报告正在生成中,请到报告列表中察看生成状态或者下载报告<br><a href=\"/main/srvreportlist.htm\" >察看新报告?</a></body></html>");

   printf("777777777777777777777777\n");        

      //system("/app/ns/java/html2fo/html2fo  index.htm index.fo ");
      //chdir("/app/ns/java/fop-0.20.5");
    //  system("/app/ns/java/jdk1.6.0_01/jre/bin/java -classpath /app/ns/java/fop-0.20.5/:/app/ns/java/fop-0.20.5/lib/xml-apis.jar:/app/ns/java/fop-0.20.5/lib/xercesImpl-2.2.1.jar:/app/ns/java/fop-0.20.5/lib/xalan-2.4.1.jar:/app/ns/java/fop-0.20.5/lib/batik.jar:/app/ns/java/fop-0.20.5/lib/avalon-framework-cvs-20020806.jar:/app/ns/java/fop-0.20.5/build/fop.jar   myfop index.fo index.pdf");
     //system(caLabel);
    sprintf(caLabel,"/app/ns/bin/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
    printf("change to mht :: %s\n",caLabel);
    system(caLabel);
      system("tar -cvzf report.tgz *");
      sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
      system(caLabel);
      sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
      system(caLabel);
    
      ltMsgFree(ltMsgPk);
 

      return 0;
     
}

/*每日流量报表：函数处理*/

int msasreportproc5(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{   
    long lTime;
    char caLabel[256];
    char caSdate[32],caEdate[32],caSdate1[32],caTime1[32];
    char *reportname;

    int  iFd;
    char caTempDir[256];
    char email[25];
    int srcfd;
    unsigned int number=1000;
    char caTempFile[256];
    char caTempDataDir[256];
    char caTemp[18192];
    
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    ltDbHeadPtr dbPtr;
    ltTablePtr  tablePtr;
    char sqlBuf[1024];
    char *lt_page_content;

    printf("5555555555555555555555555555555555555555555sdate\n");

    if( ltMsgGetVar_s(ltMsgPk,"sdate")==NULL){//传入客户端参数sdate
    sprintf(caSdate,"%s","");
      }else{
           sprintf(caSdate,"%s",ltMsgGetVar_s(ltMsgPk,"sdate"));
           }
           
    printf("------------------------------------------edate\n");
    
    if( ltMsgGetVar_s(ltMsgPk,"edate")==NULL){
    sprintf(caEdate,"%s","");
      }else{
            sprintf(caEdate,"%s",ltMsgGetVar_s(ltMsgPk,"edate"));
           }

    printf("-------------------------------------------reportname\n");
         
    reportname=ltMsgGetVar_s(ltMsgPk,"reportname");
    if(reportname==NULL){
        reportname="********";
     }
    
    printf("-----------------caSdate---------------------caEdate\n"); 
    printf("%s\n",caSdate);
    printf("%s\n",caEdate); 
  
   // time1 = nasCvtLongTime(caSdate,"00:00:00");
   // time2 = nasCvtLongTime(caEdate,"23:59:59");
      
   /* unsigned int time_1,time_2;
    time_1 = nasCvtLongTime("10-10-29","00:00:00");
    time_2 = nasCvtLongTime("10-09-29","23:59:59"); 
    printf("%d\n",time1);
    printf("%d\n",time2);
    printf("%d\n",time_1);
    printf("%d\n",time_2);
    */
    printf("-----------------------------------time\n");
      
    lTime = time(0);
	  nasCvtStime(lTime,caSdate1,caTime1);
	  caTime1[5]=0;
     
    sprintf(caTempDir,"/app/msadatacenter/report/5/%lu/",ltStrGetId());
    if(ltFileIsExist(caTempDir)!=1) {
        if(mkdir(caTempDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)!=0) {
            //生成该报告目录错误 
            ltMsgPk->msgpktype=1;
                        lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >Error:报告生成错误,无法建立报告目录!<br><a href=\"javascript:history.back(1);\" >生成新报告?</a></body></html>");     
                        ltMsgFree(ltMsgPk);
            return 0;
        }

   }  
    
    printf("---------sname---------sdate----------sname--------suser\n");

    sprintf(caLabel,"/bin/echo 'sname=%s' > %sinfo.ini",reportname,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'sdate=%s' >> %sinfo.ini",caSdate,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'edate=%s' >> %sinfo.ini",caEdate,caTempDir);
    system(caLabel);/*建立目录标志*/
    sprintf(caLabel,"/bin/echo 'sysdate=%s/%s' >> %sinfo.ini",caSdate1,caTime1,caTempDir);
    system(caLabel);/*建立目录标志*/
    
    sprintf(caLabel,"/bin/echo '1' > %sproccess.ini",caTempDir);

    printf("55555555555555555\n");

    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/right-top.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/banner.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/up-1.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/up-2.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    sprintf(caLabel,"/bin/cp /app/ns/html/images/up-3.jpg  %s",caTempDir);
    system(caLabel);/*建立目录正在处理标志*/
    
    sprintf(caTempDataDir,"%sdata",caTempDir);     
    srcfd = open(caTempDataDir,O_WRONLY | O_CREAT | O_TRUNC);
	  if(srcfd == (-1)) {
	  	        sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","数据文件写错误,无法生成报告",caTempDir);
		        system(caLabel);/*建立目录和正在处理标志*/
		    	ltMsgFree(ltMsgPk);
		        return 0;
		}
    
     
    /*运用网页模板生成报表*/
   
    printf("444444444444444444444444444444444444444444444\n");

    
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
  sprintf(caTemp,"/app/msa/msas1/cgi/outflowpltline.pl   %s %s %d ",caTempDataDir,caTempFile,number);
  system(caTemp);
  lt_dbput_rootvars(dbPtr,1,"file1","outflow1.gif");

      
  ltMsgPk->msgpktype=1;
  
  lt_page_content=ltPltFileParse("/app/msa/msas/htmlplt/test1/moldboard/everydayflow.htm",dbPtr->doc,0);
   if(lt_page_content==NULL){
            sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无报告数据,无法生成报告",caTempDir);
            system(caLabel);/*建立目录和正在处理标志*/
            ltMsgFree(ltMsgPk);
            lt_dbfree(dbPtr);
            return 0;
  }else{
        lt_page_content=strstr(lt_page_content,"<html");
        sprintf(caLabel,"%sindex.htm",caTempDir);
        iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
        if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));
        }
       close(iFd);
      /*生成html格式报告*/
  }

    printf("5555555555555-------555555555555555555\n");
    
    ltMsgPk->msgpktype=1;
 
    printf("66666666666---------66666666666666666\n");

    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"<html><body bgcolor=\"white\" >报告正在生成中,请到报告列表中察看生成状态或者下载报告<br><a href=\"/main/srvreportlist.htm\" >察看新报告?</a></body></html>");

    printf("777777777777---------77777777777777777\n");        

      //system("/app/ns/java/html2fo/html2fo  index.htm index.fo ");
      //chdir("/app/ns/java/fop-0.20.5");
    //  system("/app/ns/java/jdk1.6.0_01/jre/bin/java -classpath /app/ns/java/fop-0.20.5/:/app/ns/java/fop-0.20.5/lib/xml-apis.jar:/app/ns/java/fop-0.20.5/lib/xercesImpl-2.2.1.jar:/app/ns/java/fop-0.20.5/lib/xalan-2.4.1.jar:/app/ns/java/fop-0.20.5/lib/batik.jar:/app/ns/java/fop-0.20.5/lib/avalon-framework-cvs-20020806.jar:/app/ns/java/fop-0.20.5/build/fop.jar   myfop index.fo index.pdf");
     //system(caLabel);
    sprintf(caLabel,"/app/ns/bin/html2mail  %s index.htm %sindex.mht report %s",caTempDir,caTempDir,email);
    printf("change to mht :: %s\n",caLabel);
    system(caLabel);
    system("tar -cvzf report.tgz *");
    sprintf(caLabel,"/bin/rm  %sproccess.ini",caTempDir);
    system(caLabel);
    sprintf(caLabel,"/bin/rm  %serrmsg.ini",caTempDir);
    system(caLabel);   
    ltMsgFree(ltMsgPk);
 
    return 0;   
} 