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
#include <sys/sem.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <gdfontmb.h>
#include "msas.h"

/*
*案件查询以及删除
*/
int msasAnjianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    char strBuf[1024];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char caTmpp[512];
    long lRowNum,lStartRec,lSumRec,lCount,lNum;
    int k;

    char *caAnjian_id;
    char *caAnjian_name;
    char *caAnjian_desc;
    char *caZerenren;
    char *caZerenrentel;
    char *caZerenrenmp;
    char *caZerenrenmail;
    char *caJingju;


    char caWhere[512];
    stralloc strTemp;
    char *exportaction;


    caAnjian_id=ltMsgGetVar_s(ltMsgPk,"anjian_id");  //案件Id
    caAnjian_name=ltMsgGetVar_s(ltMsgPk,"anjian_name");//案件名称
    caAnjian_desc=ltMsgGetVar_s(ltMsgPk,"anjian_desc");//案件详情
    caZerenren=ltMsgGetVar_s(ltMsgPk,"zerenren");//案件责任人
    caZerenrentel=ltMsgGetVar_s(ltMsgPk,"zerenrentel");//责任人电话
    caZerenrenmp=ltMsgGetVar_s(ltMsgPk,"zerenrenmp");//责任人手机
    caZerenrenmail=ltMsgGetVar_s(ltMsgPk,"zerenrenmail");//责任人邮箱
    caJingju=ltMsgGetVar_s(ltMsgPk,"jingju");//警局名称
    exportaction=ltMsgGetVar_s(ltMsgPk,"export");

  	//根据传的参数判断删除以及执行删除
	  int cadeflag;
		char sqlBuf[2048];
		if(ltMsgGetVar_s(ltMsgPk,"deflag")!=NULL)
		{
			cadeflag=atol(ltMsgGetVar_s(ltMsgPk,"deflag"));
			if(cadeflag==1)
			{
				 char *myAnjian_id;
				 myAnjian_id=ltMsgGetVar_s(ltMsgPk,"delAnjian_id");//树的Pid
				 sprintf(sqlBuf,"delete from AnJianList where anjian_id='%s'",myAnjian_id);
				 ltDbExecSql(G_DbCon,sqlBuf); 
			}
			 ltMsgPk->msgpktype=1;
		   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
		   ltMsgFree(ltMsgPk);
		   return 0;
		}
	  
	  
		if(!exportaction)
		{
			exportaction="";
		}
		memset(caWhere,0,sizeof(caWhere));
		sprintf(caWhere,"%s","1=1 ");
			
	  if(caAnjian_id && caAnjian_id[0] )
		{
			sprintf(caWhere,"%s and anjian_id like '%c%s%c'",caWhere,'%',caAnjian_id,'%');
		}	
	  if(caAnjian_name && caAnjian_name[0] )
		{
			sprintf(caWhere,"%s and anjian_name like '%c%s%c'",caWhere,'%',caAnjian_name,'%');
		}	
	  if(caAnjian_desc && caAnjian_desc[0] )
		{
			sprintf(caWhere,"%s and anjian_desc like '%c%s%c'",caWhere,'%',caAnjian_desc,'%');
		}	
	  if(caZerenren && caZerenren[0] )
		{
			sprintf(caWhere,"%s and zerenren like '%c%s%c'",caWhere,'%',caZerenren,'%');
		}	
	  if(caZerenrentel && caZerenrentel[0] )
		{
			sprintf(caWhere,"%s and zerenrentel like '%c%s%c'",caWhere,'%',caZerenrentel,'%');
		}	
	  if(caZerenrenmp && caZerenrenmp[0] )
		{
			sprintf(caWhere,"%s and zerenrenmp like '%c%s%c'",caWhere,'%',caZerenrenmp,'%');
		}	
	  if(caZerenrenmail && caZerenrenmail[0] )
		{
			sprintf(caWhere,"%s and zerenrenmail like '%c%s%c'",caWhere,'%',caZerenrenmail,'%');
		}
		if(caJingju && caJingju[0] )
		{
			sprintf(caWhere,"%s and jingju like '%c%s%c'",caWhere,'%',caJingju,'%');
		}

    lRowNum=20; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0; 
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
    if(ltMsgGetVar_s(ltMsgPk,"limit"))
	  {
      lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
    }
    if(ltMsgGetVar_s(ltMsgPk,"start"))
	  {
      lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
    }
    sprintf(strBuf,"select count(*) from AnJianList where %s ",caWhere);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
		{
		  tempRow= ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL)
		  {
			  lCount=atol(tempRow[0]);
		  }
	  }
    ltDbCloseCursor(tempCursor);
    strTemp.s=0;
	  sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);
    k=0;
    sprintf(strBuf,"select anjian_id,anjian_name,anjian_desc,zerenren,zerenrentel,zerenrenmp,zerenrenmail,jingju from AnJianList where %s limit %lu offset %lu ",caWhere,lRowNum,lStartRec);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
		{
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	   	 {                
	       if(k==0)
				{
	        sprintf(caTmpp,"{\"anjian_id\":\"%s\",\"anjian_name\":\"%s\",\"anjian_desc\":\"%s\",\"zerenren\":\"%s\",\"zerenrentel\":\"%s\",\"zerenrenmp\":\"%s\",\"zerenrenmail\":\"%s\",\"jingju\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7]);
	        stralloc_cats(&strTemp,caTmpp);     
				}
				else
				{
			    sprintf(caTmpp,",{\"anjian_id\":\"%s\",\"anjian_name\":\"%s\",\"anjian_desc\":\"%s\",\"zerenren\":\"%s\",\"zerenrentel\":\"%s\",\"zerenrenmp\":\"%s\",\"zerenrenmail\":\"%s\",\"jingju\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7]);
	        stralloc_cats(&strTemp,caTmpp);     
				}                         
				tempRow= ltDbFetchRow(tempCursor);
				k++;
		   }
       ltDbCloseCursor(tempCursor);
	   }
   stralloc_cats(&strTemp,"]}");
   stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
   ltMsgFree(ltMsgPk);
   return 0;
}


/*
*某案件策略
*/ 
int msasAlertAnJianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
printf("AAAAAAAAAAAAAAAA\n");
	//根据传的参数判断删除以及执行删除
	  int cadeflag;
		char strSql[2048];
		if(ltMsgGetVar_s(ltMsgPk,"deflag")!=NULL)
		{
			cadeflag=atol(ltMsgGetVar_s(ltMsgPk,"deflag"));
printf("cadeflag：%d\n",cadeflag);
			if(cadeflag==1)
			{
				 char *myMatching_id;
				 myMatching_id=ltMsgGetVar_s(ltMsgPk,"delMatching_id");
printf("myMatching_id：%s\n",myMatching_id);
				 sprintf(strSql,"delete from AlertList where Matching_Id=%s",myMatching_id);
				 ltDbExecSql(G_DbCon,strSql);
printf("strSql：%s\n",strSql);
			   ltMsgPk->msgpktype=1;
			   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,"0");
			   ltMsgFree(ltMsgPk);
			   return 0;
			}
		}
	 char *myanjian_id;
   ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   LT_DBROW   tempRow;
   myanjian_id=ltMsgGetVar_s(ltMsgPk,"Alertmyanjian_id");
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
	  char strBuf[1024];
    ltDbCursor *tempCursor;
    char caTmpp[512];
    long lRowNum,lStartRec,lSumRec,lCount,lNum;
    int k;
    
		 char *caRule_id;    //报警规则Id
	   char *caanjian_id;  //案件Id
	   char *caRule_name;  //报警规则名称
	   char *caRule_type;//规则类型
	   char *caKeyWord1;
	   char *caKeyWord2;
	   char *caKeyWord3;
	   char *caRule_endtime;//规则触发结束时间
	   char *caRule_areacode;//规则区域号码
	   char *caupdateflag;//更新标记
		 
		 char caWhere[512];
     stralloc strTemp;
      
     caRule_id=ltMsgGetVar_s(ltMsgPk,"Rule_id");  
     caanjian_id=ltMsgGetVar_s(ltMsgPk,"anjian_id");  
     caRule_name=ltMsgGetVar_s(ltMsgPk,"Rule_name"); 
     caRule_type=ltMsgGetVar_s(ltMsgPk,"Rule_type");  
     caKeyWord1=ltMsgGetVar_s(ltMsgPk,"KeyWord1");  
     caKeyWord2=ltMsgGetVar_s(ltMsgPk,"KeyWord2");  
     caKeyWord3=ltMsgGetVar_s(ltMsgPk,"KeyWord3"); 
     caRule_endtime=ltMsgGetVar_s(ltMsgPk,"Rule_endtime");
     caRule_areacode=ltMsgGetVar_s(ltMsgPk,"Rule_areacode");  
     caupdateflag=ltMsgGetVar_s(ltMsgPk,"updateflag");  
     
     memset(caWhere,0,sizeof(caWhere));
		 sprintf(caWhere,"%s","1=1");
		
		 if(caRule_id && caRule_id[0] )
		{
			sprintf(caWhere,"%s and Rule_id like '%c%s%c'",caWhere,'%',caRule_id,'%');
		}
		 if(caanjian_id && caanjian_id[0] )
		{
			sprintf(caWhere,"%s and anjian_id like '%c%s%c'",caWhere,'%',caanjian_id,'%');
		}
		 if(caRule_name && caRule_name[0] )
		{
			sprintf(caWhere,"%s and Rule_name like '%c%s%c'",caWhere,'%',caRule_name,'%');
		}
		 if(caRule_type && caRule_type[0] )
		{
			sprintf(caWhere,"%s and Rule_type like '%c%s%c'",caWhere,'%',caRule_type,'%');
		}
		 if(caKeyWord1 && caKeyWord1[0] )
		{
			sprintf(caWhere,"%s and KeyWord1 like '%c%s%c'",caWhere,'%',caKeyWord1,'%');
		}
		 if(caKeyWord2 && caKeyWord2[0] )
		{
			sprintf(caWhere,"%s and KeyWord2 like '%c%s%c'",caWhere,'%',caKeyWord2,'%');
		}
		 if(caKeyWord3 && caKeyWord3[0] )
		{
			sprintf(caWhere,"%s and KeyWord3 like '%c%s%c'",caWhere,'%',caKeyWord3,'%');
		}
		 if(caRule_endtime && caRule_endtime[0] )
		{
			sprintf(caWhere,"%s and Rule_endtime like '%c%s%c'",caWhere,'%',caRule_endtime,'%');
		}
		 if(caRule_areacode && caRule_areacode[0] )
		{
			sprintf(caWhere,"%s and Rule_areacode like '%c%s%c'",caWhere,'%',caRule_areacode,'%');
		}
		 if(caupdateflag && caupdateflag[0] )
		{
			sprintf(caWhere,"%s and updateflag like '%c%s%c'",caWhere,'%',caupdateflag,'%');
		}
	 //////////////////////////////////////////////////////////////////////////////////////
	  lRowNum=20; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0; 
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
    if(ltMsgGetVar_s(ltMsgPk,"limit"))
		{
	    lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	  }
    if(ltMsgGetVar_s(ltMsgPk,"start"))
		{
	    lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	  }
    sprintf(strBuf,"select count(*) from RuleList where %s and anjian_id='%s'",caWhere,myanjian_id);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
		{
		  tempRow= ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL)
		  {
			  lCount=atol(tempRow[0]);
		  }
	  }
    ltDbCloseCursor(tempCursor);
    strTemp.s=0;
	  sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);
    k=0;
    sprintf(strBuf,"select Rule_id,anjian_id,Rule_name,Rule_type,KeyWord1,KeyWord2,KeyWord3,Rule_endtime,Rule_areacode,updateflag from RuleList where %s and anjian_id='%s' limit %lu offset %lu ",caWhere,myanjian_id,lRowNum,lStartRec);
    printf("SQL  %s  \n",strBuf);
    tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {                
          if(k==0)
					{
            sprintf(caTmpp,"{\"Rule_id\":\"%s\",\"anjian_id\":\"%s\",\"Rule_name\":\"%s\",\"Rule_type\":\"%s\",\"KeyWord1\":\"%s\",\"KeyWord2\":\"%s\",\"KeyWord3\":\"%s\",\"Rule_endtime\":\"%s\",\"Rule_areacode\":\"%s\",\"updateflag\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9]);
            stralloc_cats(&strTemp,caTmpp);     
					}
					else
					{
				    sprintf(caTmpp,",{\"Rule_id\":\"%s\",\"anjian_id\":\"%s\",\"Rule_name\":\"%s\",\"Rule_type\":\"%s\",\"KeyWord1\":\"%s\",\"KeyWord2\":\"%s\",\"KeyWord3\":\"%s\",\"Rule_endtime\":\"%s\",\"Rule_areacode\":\"%s\",\"updateflag\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9]);
		        stralloc_cats(&strTemp,caTmpp);     
					}                         
					tempRow= ltDbFetchRow(tempCursor);
					k++;
		    }
       ltDbCloseCursor(tempCursor);      
    }
   stralloc_cats(&strTemp,"]}");
   stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
   ltMsgFree(ltMsgPk);
   return 0;
}

/*
*某类策略的报警类型
*/
int msasRuleListAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	   /* 触发规则查询  */
     char strBuf[1024];
     ltDbCursor *tempCursor;
     LT_DBROW tempRow;     
     char caTmpp[512];
     long lRowNum,lStartRec,lSumRec,lCount,lNum;
     int k;
     
     
     char *myRule_id;
     myRule_id=ltMsgGetVar_s(ltMsgPk,"AlertmyRule_id");
     
	   char *caRule_id;    //报警规则Id
	   char *caanjian_id;  //案件Id
	   char *caRule_name;  //报警规则名称
	   char *caRule_Type;  //报警规则类型
	   char *caKeyWord1;
	   char *caKeyWord2;
	   char *caKeyWord3;
	   char *caRule_endtime;//规则触发结束时间
		 char *caRule_areacode;//规则区域号码
		 char *caupdateflag;//更新标记
	
	
	   char caWhere[512];
	   stralloc strTemp;
		 ltDbHeadPtr dbPtr;
     dbPtr=lt_dbinit();
     lt_db_htmlpage(dbPtr,"utf-8");

    caRule_id=ltMsgGetVar_s(ltMsgPk,"logid");  //报警规则Id
    caanjian_id=ltMsgGetVar_s(ltMsgPk,"gnmkey");//案件Id
    caRule_name=ltMsgGetVar_s(ltMsgPk,"sname");//报警规则名称
    caRule_Type=ltMsgGetVar_s(ltMsgPk,"User_name");//报警规则类型
    caKeyWord1=ltMsgGetVar_s(ltMsgPk,"KeyWord1");  
    caKeyWord2=ltMsgGetVar_s(ltMsgPk,"KeyWord2");  
    caKeyWord3=ltMsgGetVar_s(ltMsgPk,"KeyWord3"); 
    caRule_endtime=ltMsgGetVar_s(ltMsgPk,"Offline_time");//规则触发结束时间
    caRule_areacode=ltMsgGetVar_s(ltMsgPk,"Net_ending_ip");//规则区域号码
    caupdateflag=ltMsgGetVar_s(ltMsgPk,"updateflag");//更新标记
    
    
    
		memset(caWhere,0,sizeof(caWhere));
		sprintf(caWhere,"%s","1=1 ");
			
	  if(caRule_id && caRule_id[0] )
		{
			sprintf(caWhere,"%s and Rule_id like '%c%s%c'",caWhere,'%',caRule_id,'%');
		}
		 if(caanjian_id && caanjian_id[0] )
		{
			sprintf(caWhere,"%s and anjian_id like '%c%s%c'",caWhere,'%',caanjian_id,'%');
		}
		 if(caRule_name && caRule_name[0] )
		{
			sprintf(caWhere,"%s and Rule_name like '%c%s%c'",caWhere,'%',caRule_name,'%');
		}
		 if(caRule_Type && caRule_Type[0] )
		{
			sprintf(caWhere,"%s and Rule_type like '%c%s%c'",caWhere,'%',caRule_Type,'%');
		}
	  if(caKeyWord1 && caKeyWord1[0] )
		{
			sprintf(caWhere,"%s and KeyWord1 like '%c%s%c'",caWhere,'%',caKeyWord1,'%');
		}
		 if(caKeyWord2 && caKeyWord2[0] )
		{
			sprintf(caWhere,"%s and KeyWord2 like '%c%s%c'",caWhere,'%',caKeyWord2,'%');
		}
		 if(caKeyWord3 && caKeyWord3[0] )
		{
			sprintf(caWhere,"%s and KeyWord3 like '%c%s%c'",caWhere,'%',caKeyWord3,'%');
		}
		if(caRule_endtime && caRule_endtime[0] )
		{
			sprintf(caWhere,"%s and Rule_endtime like '%c%s%c'",caWhere,'%',caRule_endtime,'%');
		}
		 if(caRule_areacode && caRule_areacode[0] )
		{
			sprintf(caWhere,"%s and Rule_areacode like '%c%s%c'",caWhere,'%',caRule_areacode,'%');
		}
		 if(caupdateflag && caupdateflag[0] )
		{
			sprintf(caWhere,"%s and updateflag like '%c%s%c'",caWhere,'%',caupdateflag,'%');
		}
		
		
    lRowNum=20; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0; 
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
    if(ltMsgGetVar_s(ltMsgPk,"limit"))
		{
	    lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	  }
    if(ltMsgGetVar_s(ltMsgPk,"start"))
		{
	    lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	  }
    if(strcmp(myRule_id,"-1")==0){
	  	sprintf(strBuf,"select count(*) from RuleList where %s '",caWhere);
	  }else{
    	sprintf(strBuf,"select count(*) from RuleList where %s and Rule_id='%s'",caWhere,myRule_id);
    }
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
		{
		  tempRow= ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL)
		  {
			  lCount=atol(tempRow[0]);
		  }
	  }
    ltDbCloseCursor(tempCursor);
    strTemp.s=0;
	  sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);
    k=0;
    if(strcmp(myRule_id,"-1")==0){
    	sprintf(strBuf,"select Rule_id,anjian_id,Rule_name,Rule_type,KeyWord1,KeyWord2,KeyWord3,Rule_endtime,Rule_areacode,updateflag from RuleList where %s  limit %lu offset %lu ",caWhere,lRowNum,lStartRec);
	  }else{
	  	sprintf(strBuf,"select Rule_id,anjian_id,Rule_name,Rule_type,KeyWord1,KeyWord2,KeyWord3,Rule_endtime,Rule_areacode,updateflag from RuleList where %s and Rule_id='%s' limit %lu offset %lu ",caWhere,myRule_id,lRowNum,lStartRec);
	  }
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {                
          if(k==0)
					{
            sprintf(caTmpp,"{\"Rule_id\":\"%s\",\"anjian_id\":\"%s\",\"Rule_name\":\"%s\",\"Rule_type\":\"%s\",\"KeyWord1\":\"%s\",\"KeyWord2\":\"%s\",\"KeyWord3\":\"%s\",\"Rule_endtime\":\"%s\",\"Rule_areacode\":\"%s\",\"updateflag\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9]);
            stralloc_cats(&strTemp,caTmpp);     
					}
					else
					{
				    sprintf(caTmpp,",{\"Rule_id\":\"%s\",\"anjian_id\":\"%s\",\"Rule_name\":\"%s\",\"Rule_type\":\"%s\",\"KeyWord1\":\"%s\",\"KeyWord2\":\"%s\",\"KeyWord3\":\"%s\",\"Rule_endtime\":\"%s\",\"Rule_areacode\":\"%s\",\"updateflag\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9]);
		        stralloc_cats(&strTemp,caTmpp);     
					}                         
					tempRow= ltDbFetchRow(tempCursor);
					k++;
		    }
       ltDbCloseCursor(tempCursor);
     }
   stralloc_cats(&strTemp,"]}");
   stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
   ltMsgFree(ltMsgPk);
   return 0;
}

/*
*策略类型对话框  根据type查询   //没作用
*/
int msasShowRule(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	  int cadeflag;
		char strSql[2048];
		char tempstr[512];
		if(ltMsgGetVar_s(ltMsgPk,"deflag")!=NULL){
			cadeflag=atol(ltMsgGetVar_s(ltMsgPk,"deflag"));
			if(cadeflag==1){
				 char *myRule_id;
				 myRule_id=ltMsgGetVar_s(ltMsgPk,"delRule_id");
				 printf("myRule_id:%s\n",myRule_id);
				 sprintf(strSql,"delete from RuleList where Rule_id='%s'",myRule_id);
				 printf("strSql:%s\n",strSql);
				 ltDbExecSql(G_DbCon,strSql); 
				 sprintf(strSql,"delete from ClientCmd where cmdpath='%s' and cmdtype='RuleList'",myRule_id);
				 printf("strSql:%s\n",strSql);
				 ltDbExecSql(G_DbCon,strSql); 
			}
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");
	    ltMsgFree(ltMsgPk);
	    return 0;
	 }
	 char *myRule_type;
	 ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   myRule_type=ltMsgGetVar_s(ltMsgPk,"AlertmyRule_type");
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   if(atoi(myRule_type)==6){
   	memset(tempstr,0,sizeof(tempstr));
    strgb2utf8("浏览该URL",tempstr,strlen("浏览该URL"));
   	lt_dbput_rootvars(dbPtr,1,"btnName",tempstr);
   }else if(atoi(myRule_type)==7){
   	memset(tempstr,0,sizeof(tempstr));
    strgb2utf8("浏览该表单",tempstr,strlen("浏览该表单"));
   	lt_dbput_rootvars(dbPtr,1,"btnName",tempstr);
   }else if(atoi(myRule_type)==8){
   	memset(tempstr,0,sizeof(tempstr));
    strgb2utf8("浏览该邮件",tempstr,strlen("浏览该邮件"));
   	lt_dbput_rootvars(dbPtr,1,"btnName",tempstr);
   }
   lt_dbput_rootvars(dbPtr,1,"myRule_type",myRule_type);
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/rule/AlertList.html","Content-type: text/html; charset=utf-8\r\n",0);
   ltMsgFree(ltMsgPk); 
   return 0; 
}

/*
*某个策略的报警类型
*/
int msasAlertRuleLsit(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   char *myRule_id=NULL;

   LT_DBROW   tempRow;
	 char strBuf[1024];
   ltDbCursor *tempCursor;
   char caTmpp[512];
   long lRowNum,lStartRec,lSumRec,lCount,lNum;
   int k;
   stralloc strTemp;
   
   if(ltMsgGetVar_s(ltMsgPk,"AlertmyRule_id")){
     myRule_id=ltMsgGetVar_s(ltMsgPk,"AlertmyRule_id");
   }

      
	  lRowNum=20; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0; 
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
    if(ltMsgGetVar_s(ltMsgPk,"limit"))
		{
	    lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	  }
    if(ltMsgGetVar_s(ltMsgPk,"start"))
		{
	    lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	  }

	  	sprintf(strBuf,"select count(*) from AlertList where  rule_id='%s'",myRule_id);
	  
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
		{
		  tempRow= ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL)
		  {
			  lCount=atol(tempRow[0]);
		  }
	  }
    ltDbCloseCursor(tempCursor);
    strTemp.s=0;
printf("1111lCount:%ld\n",lCount);
    sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);
    k=0;

    sprintf(strBuf,"select a.Matching_id,a.gnmkey,sname,a.Rule_id,a.Rule_name,a.anjian_id,a.Matching_time,a.Net_ending_ip,a.Net_ending_code,a.Net_ending_mac,a.Destination_ip,a.Service_type,a.KeyWord1,a.KeyWord2,a.KeyWord3,a.Customer_name,a.Certificate_type,a.Certificate_code,a.Rule_type,a.contenturl,a.updateflag,b.service_name from AlertList a left join msasclientinfo b on a.gnmkey=b.service_code where a.Rule_id='%s' order by matching_time desc limit %lu offset %lu ",myRule_id,lRowNum,lStartRec);
printf("strBuf:%s\n",strBuf);    
    tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {                
          if(k==0)
					{
            sprintf(caTmpp,"{\"Matching_id\":\"%s\",\"gnmkey\":\"%s\",\"sname\":\"%s\",\"Rule_id\":\"%s\",\"Rule_name\":\"%s\",\"anjian_id\":\"%s\",\"Matching_time\":\"%s\",\"Net_ending_ip\":\"%s\",\"Net_ending_code\":\"%s\",\"Net_ending_mac\":\"%s\",\"Destination_ip\":\"%s\",\"Service_type\":\"%s\",\"KeyWord1\":\"%s\",\"KeyWord2\":\"%s\",\"KeyWord3\":\"%s\",\"Customer_name\":\"%s\",\"Certificate_type\":\"%s\",\"Certificate_code\":\"%s\",\"Rule_type\":\"%s\",\"contenturl\":\"%s\",\"updateflag\":\"%s\"}",tempRow[0],tempRow[1],tempRow[21],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14],tempRow[15],tempRow[16],tempRow[17],tempRow[18],tempRow[19],tempRow[20]);
	          stralloc_cats(&strTemp,caTmpp);     
					}
					else
					{
			      sprintf(caTmpp,",{\"Matching_id\":\"%s\",\"gnmkey\":\"%s\",\"sname\":\"%s\",\"Rule_id\":\"%s\",\"Rule_name\":\"%s\",\"anjian_id\":\"%s\",\"Matching_time\":\"%s\",\"Net_ending_ip\":\"%s\",\"Net_ending_code\":\"%s\",\"Net_ending_mac\":\"%s\",\"Destination_ip\":\"%s\",\"Service_type\":\"%s\",\"KeyWord1\":\"%s\",\"KeyWord2\":\"%s\",\"KeyWord3\":\"%s\",\"Customer_name\":\"%s\",\"Certificate_type\":\"%s\",\"Certificate_code\":\"%s\",\"Rule_type\":\"%s\",\"contenturl\":\"%s\",\"updateflag\":\"%s\"}",tempRow[0],tempRow[1],tempRow[21],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14],tempRow[15],tempRow[16],tempRow[17],tempRow[18],tempRow[19],tempRow[20]);
		        stralloc_cats(&strTemp,caTmpp);     
					}                         
					tempRow= ltDbFetchRow(tempCursor);
					k++;
		    }
       ltDbCloseCursor(tempCursor);      
    }
   stralloc_cats(&strTemp,"]}");
   stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
   ltMsgFree(ltMsgPk);
   return 0;
}

/*
*某策略类型的报警类型
*/
int msasAlertRuleAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   char *myRule_type=NULL;
   char my_keyword[20];
   LT_DBROW   tempRow;
	 char strBuf[1024];
   ltDbCursor *tempCursor;
   char caTmpp[512];
   long lRowNum,lStartRec,lSumRec,lCount,lNum;
   int k;
   stralloc strTemp;
   
   if(ltMsgGetVar_s(ltMsgPk,"AlertmyRule_type")){
     myRule_type=ltMsgGetVar_s(ltMsgPk,"AlertmyRule_type");
   }
   if(ltMsgGetVar_s(ltMsgPk,"keyword")){
     sprintf(my_keyword,"%s",ltMsgGetVar_s(ltMsgPk,"keyword"));
   }
//printf("my_keyword:%s\n",my_keyword);    
	  lRowNum=20; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0; 
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
//printf("LIMIT:%s\n",ltMsgGetVar_s(ltMsgPk,"limit")); 
    if(ltMsgGetVar_s(ltMsgPk,"limit"))
		{
	    lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	  }
    if(ltMsgGetVar_s(ltMsgPk,"start"))
		{
	    lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	  }
	  if(strcmp(myRule_type,"-1")==0){
			sprintf(strBuf,"select count(*) from AlertList");
		}
	  else if(strcmp(my_keyword,"")==0){
	  	sprintf(strBuf,"select count(*) from AlertList where  rule_type='%s'",myRule_type);
		}else{
			sprintf(strBuf,"select count(*) from AlertList where  rule_type='%s' and keyword1='%s'",myRule_type,my_keyword);
		}
printf("AAAstrBuf:%s\n",strBuf);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
		{
		  tempRow= ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL)
		  {
			  lCount=atol(tempRow[0]);
		  }
	  }
    ltDbCloseCursor(tempCursor);
    strTemp.s=0;
	  printf("lCount:%ld\n",lCount);
    sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);
    k=0;
	  if(strcmp(myRule_type,"-1")==0){
			sprintf(strBuf,"select Matching_id,a.gnmkey,sname,Rule_id,Rule_name,anjian_id,Matching_time,Net_ending_ip,Net_ending_code,Net_ending_mac,Destination_ip,Service_type,KeyWord1,KeyWord2,KeyWord3,Customer_name,Certificate_type,Certificate_code,Rule_type,contenturl,a.updateflag,b.service_name from AlertList a left join msasclientinfo b on a.gnmkey=b.service_code order by Matching_time desc limit %lu offset %lu",lRowNum,lStartRec);
		}
	  else if(strcmp(my_keyword,"")==0){
    	sprintf(strBuf,"select Matching_id,a.gnmkey,sname,Rule_id,Rule_name,anjian_id,Matching_time,Net_ending_ip,Net_ending_code,Net_ending_mac,Destination_ip,Service_type,KeyWord1,KeyWord2,KeyWord3,Customer_name,Certificate_type,Certificate_code,Rule_type,contenturl,a.updateflag,b.service_name from AlertList a left join msasclientinfo b on a.gnmkey=b.service_code where rule_type='%s' order by matching_time desc limit %lu offset %lu",myRule_type,lRowNum,lStartRec);
    }else{
    	sprintf(strBuf,"select Matching_id,a.gnmkey,sname,Rule_id,Rule_name,anjian_id,Matching_time,Net_ending_ip,Net_ending_code,Net_ending_mac,Destination_ip,Service_type,KeyWord1,KeyWord2,KeyWord3,Customer_name,Certificate_type,Certificate_code,Rule_type,contenturl,a.updateflag,b.service_name from AlertList a left join msasclientinfo b on a.gnmkey=b.service_code where rule_type='%s' and keyword1='%s' order by matching_time desc limit %lu offset %lu",myRule_type,my_keyword,lRowNum,lStartRec);
    }
    printf("SQL   %s \n",strBuf);
    tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {                
          if(k==0)
					{
            sprintf(caTmpp,"{\"Matching_id\":\"%s\",\"gnmkey\":\"%s\",\"sname\":\"%s\",\"Rule_id\":\"%s\",\"Rule_name\":\"%s\",\"anjian_id\":\"%s\",\"Matching_time\":\"%s\",\"Net_ending_ip\":\"%s\",\"Net_ending_code\":\"%s\",\"Net_ending_mac\":\"%s\",\"Destination_ip\":\"%s\",\"Service_type\":\"%s\",\"KeyWord1\":\"%s\",\"KeyWord2\":\"%s\",\"KeyWord3\":\"%s\",\"Customer_name\":\"%s\",\"Certificate_type\":\"%s\",\"Certificate_code\":\"%s\",\"Rule_type\":\"%s\",\"contenturl\":\"%s\",\"updateflag\":\"%s\",\"service_name\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14],tempRow[15],tempRow[16],tempRow[17],tempRow[18],tempRow[19],tempRow[20],tempRow[21]);
	          stralloc_cats(&strTemp,caTmpp);     
					}
					else
					{
			      sprintf(caTmpp,",{\"Matching_id\":\"%s\",\"gnmkey\":\"%s\",\"sname\":\"%s\",\"Rule_id\":\"%s\",\"Rule_name\":\"%s\",\"anjian_id\":\"%s\",\"Matching_time\":\"%s\",\"Net_ending_ip\":\"%s\",\"Net_ending_code\":\"%s\",\"Net_ending_mac\":\"%s\",\"Destination_ip\":\"%s\",\"Service_type\":\"%s\",\"KeyWord1\":\"%s\",\"KeyWord2\":\"%s\",\"KeyWord3\":\"%s\",\"Customer_name\":\"%s\",\"Certificate_type\":\"%s\",\"Certificate_code\":\"%s\",\"Rule_type\":\"%s\",\"contenturl\":\"%s\",\"updateflag\":\"%s\",\"service_name\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14],tempRow[15],tempRow[16],tempRow[17],tempRow[18],tempRow[19],tempRow[20],tempRow[21]);
		        stralloc_cats(&strTemp,caTmpp);     
					}                         
					tempRow= ltDbFetchRow(tempCursor);
					k++;
		    }
       ltDbCloseCursor(tempCursor);      
    }
   stralloc_cats(&strTemp,"]}");
   stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
   ltMsgFree(ltMsgPk);
   return 0;
}

/*
*某案件报警类型
*/ 
int msasRuleAnJianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   char *myanjian_id;
   ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   LT_DBROW   tempRow;
   myanjian_id=ltMsgGetVar_s(ltMsgPk,"Alertmyanjian_id");
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
	  char strBuf[1024];
    ltDbCursor *tempCursor;
    char caTmpp[512];
    long lRowNum,lStartRec,lSumRec,lCount,lNum;
    int k;
    stralloc strTemp;
    
	  lRowNum=20; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0; 
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
    if(ltMsgGetVar_s(ltMsgPk,"limit"))
		{
	    lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	  }
    if(ltMsgGetVar_s(ltMsgPk,"start"))
		{
	    lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	  }
    sprintf(strBuf,"select count(*) from AlertList where anjian_id='%s'",myanjian_id);
	  tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
		{
		  tempRow= ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL)
		  {
			  lCount=atol(tempRow[0]);
		  }
	  }
    ltDbCloseCursor(tempCursor);
    strTemp.s=0;
	  sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);
    k=0;
    sprintf(strBuf,"select Matching_id,a.gnmkey,sname,Rule_id,anjian_id,Rule_name,Matching_time,Net_ending_ip,Net_ending_code,Net_ending_mac,Destination_ip,Service_type,KeyWord1,KeyWord2,KeyWord3,Customer_name,Certificate_type,Certificate_code,Rule_type,contenturl,a.updateflag,service_name from AlertList a left join msasclientinfo b on a.gnmkey=b.service_code where anjian_id='%s' order by matching_time desc limit %lu offset %lu",myanjian_id,lRowNum,lStartRec);
printf("strBufstrBuf:%s\n",strBuf);
    tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {                
	        if(k==0){
						sprintf(caTmpp,"{\"Matching_id\":\"%s\",\"gnmkey\":\"%s\",\"sname\":\"%s\",\"Rule_id\":\"%s\",\"anjian_id\":\"%s\",\"Rule_name\":\"%s\",\"Matching_time\":\"%s\",\"Net_ending_ip\":\"%s\",\"Net_ending_code\":\"%s\",\"Net_ending_mac\":\"%s\",\"Destination_ip\":\"%s\",\"Service_type\":\"%s\",\"KeyWord1\":\"%s\",\"KeyWord2\":\"%s\",\"KeyWord3\":\"%s\",\"Customer_name\":\"%s\",\"Certificate_type\":\"%s\",\"Certificate_code\":\"%s\",\"Rule_type\":\"%s\",\"contenturl\":\"%s\",\"updateflag\":\"%s\"}",tempRow[0],tempRow[1],tempRow[21],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14],tempRow[15],tempRow[16],tempRow[17],tempRow[18],tempRow[19],tempRow[20]);
						stralloc_cats(&strTemp,caTmpp);     
					}else{
		        sprintf(caTmpp,",{\"Matching_id\":\"%s\",\"gnmkey\":\"%s\",\"sname\":\"%s\",\"Rule_id\":\"%s\",\"anjian_id\":\"%s\",\"Rule_name\":\"%s\",\"Matching_time\":\"%s\",\"Net_ending_ip\":\"%s\",\"Net_ending_code\":\"%s\",\"Net_ending_mac\":\"%s\",\"Destination_ip\":\"%s\",\"Service_type\":\"%s\",\"KeyWord1\":\"%s\",\"KeyWord2\":\"%s\",\"KeyWord3\":\"%s\",\"Customer_name\":\"%s\",\"Certificate_type\":\"%s\",\"Certificate_code\":\"%s\",\"Rule_type\":\"%s\",\"contenturl\":\"%s\",\"updateflag\":\"%s\"}",tempRow[0],tempRow[1],tempRow[21],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10],tempRow[11],tempRow[12],tempRow[13],tempRow[14],tempRow[15],tempRow[16],tempRow[17],tempRow[18],tempRow[19],tempRow[20]);
	          stralloc_cats(&strTemp,caTmpp);     
		      }                         
					tempRow= ltDbFetchRow(tempCursor);
					k++;
		   }
       ltDbCloseCursor(tempCursor);      
    }
   stralloc_cats(&strTemp,"]}");
   stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
   ltMsgFree(ltMsgPk);
   return 0;
}

/*
**根据策略类型查询策略
*/
int msasRuleTypeAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	 char *myRule_type;
   ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   LT_DBROW   tempRow;
   myRule_type=ltMsgGetVar_s(ltMsgPk,"AlertmyRule_type");
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
	  char strBuf[1024];
    ltDbCursor *tempCursor;
    char caTmpp[512];
    long lRowNum,lStartRec,lSumRec,lCount,lNum;
    int k;
      
	  char *caRule_id;
    char *caanjian_id;
    char *caRule_name;
    char *caRule_type;
    char *caKeyWord1;
    char *caKeyWord2;
    char *caKeyWord3;
    char *caRule_endtime;
    char *caRule_areacode;
    char *caupdateflag;
		 
		 char caWhere[512];
     stralloc strTemp;
      
    caRule_id=ltMsgGetVar_s(ltMsgPk,"Rule_id");
    caanjian_id=ltMsgGetVar_s(ltMsgPk,"anjian_id");
    caRule_name=ltMsgGetVar_s(ltMsgPk,"Rule_name");
    caRule_type=ltMsgGetVar_s(ltMsgPk,"Rule_type");
    caKeyWord1=ltMsgGetVar_s(ltMsgPk,"KeyWord1");
    caKeyWord2=ltMsgGetVar_s(ltMsgPk,"KeyWord2");
    caKeyWord3=ltMsgGetVar_s(ltMsgPk,"KeyWord3");
    caRule_endtime=ltMsgGetVar_s(ltMsgPk,"Rule_endtime");
    caRule_areacode=ltMsgGetVar_s(ltMsgPk,"Rule_areacode");
    caupdateflag=ltMsgGetVar_s(ltMsgPk,"updateflag");  
     
     memset(caWhere,0,sizeof(caWhere));
		 sprintf(caWhere,"%s","1=1");
		
		memset(caWhere,0,sizeof(caWhere));
		sprintf(caWhere,"%s","1=1");
			
	  if(caRule_id && caRule_id[0] )
		{
			sprintf(caWhere,"%s and Rule_id like '%c%s%c'",caWhere,'%',caRule_id,'%');
		}
		if(caanjian_id && caanjian_id[0] )
		{
			sprintf(caWhere,"%s and anjian_id like '%c%s%c'",caWhere,'%',caanjian_id,'%');
		}
		if(caRule_name && caRule_name[0] )
		{
			sprintf(caWhere,"%s and Rule_name like '%c%s%c'",caWhere,'%',caRule_name,'%');
		}
		if(caRule_type && caRule_type[0] )
		{
			sprintf(caWhere,"%s and Rule_type like '%c%s%c'",caWhere,'%',caRule_type,'%');
		}
		if(caKeyWord1 && caKeyWord1[0] )
		{
			sprintf(caWhere,"%s and KeyWord1 like '%c%s%c'",caWhere,'%',caKeyWord1,'%');
		}
		if(caKeyWord2 && caKeyWord2[0] )
		{
			sprintf(caWhere,"%s and KeyWord2 like '%c%s%c'",caWhere,'%',caKeyWord2,'%');
		}
		if(caKeyWord3 && caKeyWord3[0] )
		{
			sprintf(caWhere,"%s and KeyWord3 like '%c%s%c'",caWhere,'%',caKeyWord3,'%');
		}
		if(caRule_endtime && caRule_endtime[0] )
		{
			sprintf(caWhere,"%s and Rule_endtime like '%c%s%c'",caWhere,'%',caRule_endtime,'%');
		}
		if(caRule_areacode && caRule_areacode[0] )
		{
			sprintf(caWhere,"%s and Rule_areacode like '%c%s%c'",caWhere,'%',caRule_areacode,'%');
		}
		if(caupdateflag && caupdateflag[0] )
		{
			sprintf(caWhere,"%s and updateflag like '%c%s%c'",caWhere,'%',caupdateflag,'%');
		}
	  lRowNum=20; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0; 
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
    if(ltMsgGetVar_s(ltMsgPk,"limit"))
		{
	    lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
	  }
    if(ltMsgGetVar_s(ltMsgPk,"start"))
		{
	    lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
	  }
    if(strcmp(myRule_type,"-1")==0){
    	sprintf(strBuf,"select count(*) from RuleList where %s ",caWhere);
    }else{
    	sprintf(strBuf,"select count(*) from RuleList where %s and Rule_type='%s'",caWhere,myRule_type);
    }
	  //printf("CCC查询行数strBuf:%s\n",strBuf);
    tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
		{
		  tempRow= ltDbFetchRow(tempCursor);
		  if(tempRow!=NULL)
		  {
			  lCount=atol(tempRow[0]);
		  }
	  }
    ltDbCloseCursor(tempCursor);
    strTemp.s=0;
	  //printf("lCount:%ld\n",lCount);
    sprintf(caTmpp,"{\"totalCount\":\"%ld\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);
    k=0;
    if(strcmp(myRule_type,"-1")==0){
    	sprintf(strBuf,"select Rule_id,anjianList.anjian_id,anjianList.anjian_name,Rule_name,Rule_type,KeyWord1,KeyWord2,KeyWord3,Rule_endtime,Rule_areacode,updateflag from anjianList,RuleList where anjianList.anjian_id=RuleList.anjian_id and %s  limit %lu offset %lu ",caWhere,lRowNum,lStartRec);
    }else{
    	sprintf(strBuf,"select Rule_id,anjianList.anjian_id,anjianList.anjian_name,Rule_name,Rule_type,KeyWord1,KeyWord2,KeyWord3,Rule_endtime,Rule_areacode,updateflag from anjianList,RuleList where anjianList.anjian_id=RuleList.anjian_id and %s and Rule_type='%s' limit %lu offset %lu ",caWhere,myRule_type,lRowNum,lStartRec);
    }
    tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL)
	  {
       tempRow=ltDbFetchRow(tempCursor);
       while(tempRow!=NULL)
	     {                
          if(k==0)
					{
            sprintf(caTmpp,"{\"Rule_id\":\"%s\",\"anjian_id\":\"%s\",\"anjian_name\":\"%s\",\"Rule_name\":\"%s\",\"Rule_type\":\"%s\",\"KeyWord1\":\"%s\",\"KeyWord2\":\"%s\",\"KeyWord3\":\"%s\",\"Rule_endtime\":\"%s\",\"Rule_areacode\":\"%s\",\"updateflag\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10]);
	          stralloc_cats(&strTemp,caTmpp);     
					}
					else
					{
				    sprintf(caTmpp,",{\"Rule_id\":\"%s\",\"anjian_id\":\"%s\",\"anjian_name\":\"%s\",\"Rule_name\":\"%s\",\"Rule_type\":\"%s\",\"KeyWord1\":\"%s\",\"KeyWord2\":\"%s\",\"KeyWord3\":\"%s\",\"Rule_endtime\":\"%s\",\"Rule_areacode\":\"%s\",\"updateflag\":\"%s\"}",tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8],tempRow[9],tempRow[10]);
		        stralloc_cats(&strTemp,caTmpp);     
					}                         
					tempRow= ltDbFetchRow(tempCursor);
					k++;
		    }
       ltDbCloseCursor(tempCursor);      
    }
   stralloc_cats(&strTemp,"]}");
   stralloc_0(&strTemp);
   ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
   ltMsgFree(ltMsgPk);
   return 0;
}

/*
**案件动态树
*/
int msasAnjianTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	   char sqlBuf[512];
	   char tmpstr[512];
	   ltDbCursor *tempCursor;
	   LT_DBROW tempRow;
	   stralloc strTemp;
	   int iHead;
	   memset(tmpstr,0,sizeof(tmpstr));
	 
	   iHead=1;
	   strTemp.s=0;
	   stralloc_cats(&strTemp,"[");
	   sprintf(sqlBuf,"select anjian_id,anjian_name,anjian_desc,zerenren,zerenrentel,zerenrenmp,zerenrenmail,jingju from AnJianList order by anjian_id asc");
	   tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	   if(tempCursor!=NULL)
	   	{
	       tempRow= ltDbFetchRow(tempCursor);
	       while(tempRow!=NULL)
			   {
						/*判断有没有子节点*/
						if(iHead==1)
						{
							 sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s',leaf: true}",tempRow[0],tempRow[2],tempRow[1]);//qtip:鼠标悬浮时显示提示信息
							 iHead=0;
						}
						else
						{
							 sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s',leaf: true}",tempRow[0],tempRow[2],tempRow[1]);
						}
						stralloc_cats(&strTemp,tmpstr);       
						tempRow= ltDbFetchRow(tempCursor);
	        }
	        ltDbCloseCursor(tempCursor);
	    }
	   stralloc_cats(&strTemp,"]");
	   stralloc_0(&strTemp);
	   ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
	   ltMsgFree(ltMsgPk);
	   return 0;
}
/*
*案件详情对话框  根据Id查询
*/
int msasShowAnJian(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
   char *myanjian_id;
	// int  countnum;
	// char caStr[2048];
	 char sqlBuf[2048];
   ltDbHeadPtr dbPtr;
   dbPtr=lt_dbinit();
   ltDbCursor *tempCursor;
   LT_DBROW   tempRow;
   myanjian_id=ltMsgGetVar_s(ltMsgPk,"myanjian_id");
   dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   if(myanjian_id==NULL){/*new*/
   }else{/*update*/
		sprintf(sqlBuf,"select anjian_name,anjian_desc,zerenren,zerenrentel,zerenrenmp,zerenrenmail,jingju from AnJianList where anjian_id='%s'",myanjian_id);
			 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);  
			 if(tempCursor!=NULL)
			 {
					 tempRow= ltDbFetchRow(tempCursor);
					 if(tempRow!=NULL)
					 	{
						 lt_dbput_rootvars(dbPtr,1,"anjian_id",myanjian_id);
						 lt_dbput_rootvars(dbPtr,1,"anjian_name",tempRow[0]);
						 lt_dbput_rootvars(dbPtr,1,"anjian_desc",tempRow[1]);
						 lt_dbput_rootvars(dbPtr,1,"zerenren",tempRow[2]);
						 lt_dbput_rootvars(dbPtr,1,"zerenrentel",tempRow[3]);
						 lt_dbput_rootvars(dbPtr,1,"zerenrenmp",tempRow[4]);
						 lt_dbput_rootvars(dbPtr,1,"zerenrenmail",tempRow[5]);
						 lt_dbput_rootvars(dbPtr,1,"jingju",tempRow[6]); 
						 lt_dbput_rootvars(dbPtr,1,"myanjian_id",myanjian_id);
					 }
				 ltDbCloseCursor(tempCursor);
			 }
   }
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/rule/AnJianList.html","Content-type: text/html; charset=utf-8\r\n",0);
   ltMsgFree(ltMsgPk); 
   return 0; 
}

//调用添加案件模板
int msasShowanjianPage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
	
    char *myanjian_id;
    
	  char sqlBuf[2048];
    ltDbHeadPtr dbPtr;
    ltDbCursor *tempCursor;
    LT_DBROW   tempRow;
    dbPtr=lt_dbinit();

    myanjian_id=ltMsgGetVar_s(ltMsgPk,"myanjian_id");
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    if(myanjian_id==NULL){/*new*/
  	
	   }else{/*update*/
			sprintf(sqlBuf,"select anjian_name,anjian_desc,zerenren,zerenrentel,zerenrenmp,zerenrenmail,jingju from anjianList where anjian_id='%s'",myanjian_id);
				 tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);   
				  if(tempCursor!=NULL){
						 tempRow= ltDbFetchRow(tempCursor);
						 if(tempRow!=NULL){
							 lt_dbput_rootvars(dbPtr,1,"anjian_name",tempRow[0]);
							 lt_dbput_rootvars(dbPtr,1,"anjian_desc",tempRow[1]);
							 lt_dbput_rootvars(dbPtr,1,"zerenren",tempRow[2]);
							 lt_dbput_rootvars(dbPtr,1,"zerenrentel",tempRow[3]);
							 lt_dbput_rootvars(dbPtr,1,"zerenrenmp",tempRow[4]);
							 lt_dbput_rootvars(dbPtr,1,"zerenrenmail",tempRow[5]);
							 lt_dbput_rootvars(dbPtr,1,"jingju",tempRow[6]); 
							 lt_dbput_rootvars(dbPtr,1,"myanjian_id",myanjian_id);  
						 }
					 ltDbCloseCursor(tempCursor);
				 }
	   }
	  ltMsgPk->msgpktype=1;
	  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/rule/anjiannewbk.html","Content-type: text/html; charset=utf-8\r\n",0);
	  ltMsgFree(ltMsgPk);
	  return 0;
}


/*
*添加案件
*/
int msasAddanjianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    char *caanjian_name;
    char *caanjian_desc;
    char *cazerenren;
    char *cazerenrentel;
    char *cazerenrenmp;
    char *cazerenrenmail;
    char *cajingju;
    
    char  maxAnjian_Id[256];
		sprintf(maxAnjian_Id,"%ld%d",time(0),getpid());
    
	  char caStr[2048];
	  char sqlBuf[2048];
    ltDbHeadPtr dbPtr;
    dbPtr=lt_dbinit();
    
	 caanjian_name=ltMsgGetVar_s(ltMsgPk,"anjian_name");
     if(!caanjian_name){
         caanjian_name="";
	 }
    caanjian_desc=ltMsgGetVar_s(ltMsgPk,"anjian_desc");
     if(!caanjian_desc){
         caanjian_desc="";
	 }
    cazerenren=ltMsgGetVar_s(ltMsgPk,"zerenren");
     if(!cazerenren){
         cazerenren="";
	 }
    cazerenrentel=ltMsgGetVar_s(ltMsgPk,"zerenrentel");
     if(!cazerenrentel){
         cazerenrentel="";
	 }
    cazerenrenmp=ltMsgGetVar_s(ltMsgPk,"zerenrenmp");
     if(!cazerenrenmp){
         cazerenrenmp="";
	 }
    cazerenrenmail=ltMsgGetVar_s(ltMsgPk,"zerenrenmail");
     if(!cazerenrenmail){
         cazerenrenmail="";
	 }
    cajingju=ltMsgGetVar_s(ltMsgPk,"jingju");
     if(!cajingju){
         cajingju="";
	 }
    

     sprintf(sqlBuf,"insert into anjianList(anjian_id,anjian_name,anjian_desc,zerenren,zerenrentel,zerenrenmp,zerenrenmail,jingju)values ('%s','%s','%s','%s','%s','%s','%s','%s')",maxAnjian_Id,caanjian_name,caanjian_desc,cazerenren,cazerenrentel,cazerenrenmp,cazerenrenmail,cajingju);
     //printf("sqlBuf:%s\n",sqlBuf);   
	   ltDbExecSql(G_DbCon,sqlBuf); 
     memset(caStr,0,sizeof(caStr));
  	 ltMsgPk->msgpktype=1;
     lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");   
	   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/rule/anjiannewbk.html",dbPtr->head,0);
     ltMsgFree(ltMsgPk);
    return 0;
}


/*
*修改案件
*/
int msasUpdateanjianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{

	  char *myanjian_id;
    char *caanjian_name;
    char *caanjian_desc;
    char *cazerenren;
    char *cazerenrentel;
    char *cazerenrenmp;
    char *cazerenrenmail;
    char *cajingju;

	  char caStr[2048];
	  char sqlBuf[2048];
    ltDbHeadPtr dbPtr; 
    dbPtr=lt_dbinit();

		myanjian_id=ltMsgGetVar_s(ltMsgPk,"anjian_id");
		caanjian_name=ltMsgGetVar_s(ltMsgPk,"anjian_name");
	     if(!caanjian_name){
	         caanjian_name="";
		 }
		caanjian_desc=ltMsgGetVar_s(ltMsgPk,"anjian_desc");
	     if(!caanjian_desc){
	         caanjian_desc="";
		 }
		cazerenren=ltMsgGetVar_s(ltMsgPk,"zerenren");
	     if(!cazerenren){
	         cazerenren="";
		 }
		cazerenrentel=ltMsgGetVar_s(ltMsgPk,"zerenrentel");
	     if(!cazerenrentel){
	         cazerenrentel="";
		 }
		cazerenrenmp=ltMsgGetVar_s(ltMsgPk,"zerenrenmp");
	     if(!cazerenrenmp){
	         cazerenrenmp="";
		 }
		cazerenrenmail=ltMsgGetVar_s(ltMsgPk,"zerenrenmail");
	     if(!cazerenrenmail){
	         cazerenrenmail="";
		 }
		cajingju=ltMsgGetVar_s(ltMsgPk,"jingju");
	     if(!cajingju){
	         cajingju="";
		 }
   sprintf(sqlBuf,"update anjianList set anjian_name='%s',anjian_desc='%s',zerenren='%s',zerenrentel='%s',zerenrenmp='%s',zerenrenmail='%s',jingju='%s' where anjian_id='%s'",caanjian_name,caanjian_desc,cazerenren,cazerenrentel,cazerenrenmp,cazerenrenmail,cajingju,myanjian_id);
   printf("sqlBuf:%s\n",sqlBuf);   
   ltDbExecSql(G_DbCon,sqlBuf); 
   memset(caStr,0,sizeof(caStr));
   printf("caStr:%d\n",sizeof(caStr));   
   ltMsgPk->msgpktype=1;
   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,"0");   
   ltMsgFree(ltMsgPk);
    return 0;
}

/*
*查看案件
*/
int msasAddAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{	
 	  char sqlBuf[512];
    ltDbHeadPtr dbPtr;	
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;    
    ltTablePtr tablePtr;	 	 
	  dbPtr=lt_dbinit();
    tablePtr=lt_dbput_table(dbPtr,"list");
	  lt_db_htmlpage(dbPtr,"utf-8");
	  sprintf(sqlBuf,"select anjian_id,anjian_name from AnJianList");
    tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	  if(tempCursor!=NULL){
			 tempRow= ltDbFetchRow(tempCursor);
			 while(tempRow!=NULL){
				lt_dbput_recordvars(tablePtr,2,"id",LT_TYPE_STRING,tempRow[0],"name",LT_TYPE_STRING,tempRow[1]);
				tempRow= ltDbFetchRow(tempCursor);
			 }
		  ltDbCloseCursor(tempCursor);
	  }	   
  	ltMsgPk->msgpktype=1;    
		lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/rule/nasnewbk.htm",dbPtr->head,0);
    ltMsgFree(ltMsgPk);
    return 0;
}


/*
*添加策略
*/
int msasShowRulePage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{		
		char Rule_id[32],anjian_id[32],Rule_name[32],KeyWord1[256],KeyWord2[256],KeyWord3[256],KeyWord4[256],KeyWord5[256],KeyWord6[256],KeyWord7[256],KeyWord8[256],KeyWord9[256],Rule_endtime[32],Rule_areacode[32],updateflag[32],flag[8];
		char sqlBuf[2048];
		unsigned int Rule_type;
    if( ltMsgGetVar_s(ltMsgPk,"anjian_id")==NULL){
			sprintf(anjian_id,"%s","");
    }else{
    	sprintf(anjian_id,"%s",ltMsgGetVar_s(ltMsgPk,"anjian_id"));
    }
    if( ltMsgGetVar_s(ltMsgPk,"Rule_name")==NULL){
			sprintf(Rule_name,"%s","");
    }else{
    	sprintf(Rule_name,"%s",ltMsgGetVar_s(ltMsgPk,"Rule_name"));
    }
    if( ltMsgGetVar_s(ltMsgPk,"Rule_type")==NULL){
			Rule_type=10;
    }else{
    	Rule_type=atol(ltMsgGetVar_s(ltMsgPk,"Rule_type"));
    }
    
    if( ltMsgGetVar_s(ltMsgPk,"KeyWord1")==NULL){
			sprintf(KeyWord1,"%s","");
    }else{
    		sprintf(KeyWord1,"%s",ltMsgGetVar_s(ltMsgPk,"KeyWord1"));
    }
    if( ltMsgGetVar_s(ltMsgPk,"KeyWord2")==NULL){
			sprintf(KeyWord2,"%s","");
    }else{
    		sprintf(KeyWord2,"%s",ltMsgGetVar_s(ltMsgPk,"KeyWord2"));
    }
    
    if( ltMsgGetVar_s(ltMsgPk,"KeyWord3")==NULL){
			sprintf(KeyWord3,"%s","");
    }else{
    		sprintf(KeyWord3,"%s",ltMsgGetVar_s(ltMsgPk,"KeyWord3"));
    }
    
    if( ltMsgGetVar_s(ltMsgPk,"KeyWord4")==NULL){
			sprintf(KeyWord4,"%s","");
    }else{
    		sprintf(KeyWord4,"%s",ltMsgGetVar_s(ltMsgPk,"KeyWord4"));
    }
    if( ltMsgGetVar_s(ltMsgPk,"KeyWord5")==NULL){
			sprintf(KeyWord5,"%s","");
    }else{
    		sprintf(KeyWord5,"%s",ltMsgGetVar_s(ltMsgPk,"KeyWord5"));
    }
    if( ltMsgGetVar_s(ltMsgPk,"KeyWord6")==NULL){
			sprintf(KeyWord6,"%s","");
    }else{
    		sprintf(KeyWord6,"%s",ltMsgGetVar_s(ltMsgPk,"KeyWord6"));
    }
    if( ltMsgGetVar_s(ltMsgPk,"KeyWord7")==NULL){
			sprintf(KeyWord7,"%s","");
    }else{
    		sprintf(KeyWord7,"%s",ltMsgGetVar_s(ltMsgPk,"KeyWord7"));
    }
    if( ltMsgGetVar_s(ltMsgPk,"KeyWord8")==NULL){
			sprintf(KeyWord8,"%s","");
    }else{
    		sprintf(KeyWord8,"%s",ltMsgGetVar_s(ltMsgPk,"KeyWord8"));
    }
    
    if( ltMsgGetVar_s(ltMsgPk,"KeyWord9")==NULL){
			sprintf(KeyWord9,"%s","");
    }else{
    		sprintf(KeyWord9,"%s",ltMsgGetVar_s(ltMsgPk,"KeyWord9"));
    }
    
    if( ltMsgGetVar_s(ltMsgPk,"Rule_endtime")==NULL){
			sprintf(Rule_endtime,"%s","");
    }else{
    		sprintf(Rule_endtime,"%s",ltMsgGetVar_s(ltMsgPk,"Rule_endtime"));
    }
    
    if( ltMsgGetVar_s(ltMsgPk,"Rule_areacode")==NULL){
			sprintf(Rule_areacode,"%s","");
    }else{
    		sprintf(Rule_areacode,"%s",ltMsgGetVar_s(ltMsgPk,"Rule_areacode"));
    }
    
    if( ltMsgGetVar_s(ltMsgPk,"updateflag")==NULL){
			sprintf(updateflag,"%d",0);
    }else{
    		sprintf(updateflag,"%s",ltMsgGetVar_s(ltMsgPk,"updateflag"));
    }
    sprintf(Rule_id,"%ld%d",time(0),getpid());
    sprintf(flag,"%s","1");
    	if(Rule_type==0){
    			sprintf(flag,"%s","0");
					sprintf(sqlBuf,"insert into RuleList values ('%s','%s','%s',%u,'%s','%s','%s','%s','%s',%s)",Rule_id,anjian_id,Rule_name,Rule_type,KeyWord1,KeyWord2,KeyWord3,Rule_endtime,Rule_areacode,updateflag);
					ltDbExecSql(G_DbCon,sqlBuf);
				}	
			else if(Rule_type<6){
					sprintf(flag,"%s","0");
					sprintf(sqlBuf,"insert into RuleList values ('%s','%s','%s',%u,'%s','','','%s','%s',%s)",Rule_id,anjian_id,Rule_name,Rule_type,KeyWord4,Rule_endtime,Rule_areacode,updateflag);
					ltDbExecSql(G_DbCon,sqlBuf);
				}
		  else if(Rule_type==6){
					sprintf(flag,"%s","0");
					sprintf(sqlBuf,"insert into RuleList values ('%s','%s','%s',%u,'%s','','','%s','%s',%s)",Rule_id,anjian_id,Rule_name,Rule_type,KeyWord5,Rule_endtime,Rule_areacode,updateflag);
					ltDbExecSql(G_DbCon,sqlBuf);
				}
			else if(Rule_type==7){
					sprintf(flag,"%s","0");
					sprintf(sqlBuf,"insert into RuleList values ('%s','%s','%s',%u,'%s','','','%s','%s',%s)",Rule_id,anjian_id,Rule_name,Rule_type,KeyWord6,Rule_endtime,Rule_areacode,updateflag);
					ltDbExecSql(G_DbCon,sqlBuf);
			}
			else if(Rule_type==8){
					sprintf(flag,"%s","0");
					sprintf(sqlBuf,"insert into RuleList values ('%s','%s','%s',%u,'%s','%s','%s','%s','%s',%s)",Rule_id,anjian_id,Rule_name,Rule_type,KeyWord7,KeyWord8,KeyWord9,Rule_endtime,Rule_areacode,updateflag);
					ltDbExecSql(G_DbCon,sqlBuf);
			}
			else if(Rule_type==9){
					sprintf(flag,"%s","0");
					sprintf(sqlBuf,"insert into RuleList values ('%s','%s','%s',%u,'%s','','','%s','%s',%s)",Rule_id,anjian_id,Rule_name,Rule_type,KeyWord4,Rule_endtime,Rule_areacode,updateflag);
					ltDbExecSql(G_DbCon,sqlBuf);
			}
//			else if(Rule_type==7){
//					sprintf(flag,"%s","0");
//					sprintf(sqlBuf,"insert into RuleList values ('%s','%s','%s',%u,'%s','','','%s','%s',%s)",Rule_id,anjian_id,Rule_name,Rule_type,KeyWord6,Rule_endtime,Rule_areacode,updateflag);					
//					ltDbExecSql(G_DbCon,sqlBuf);
//				}
//			else if(Rule_type==8){
//					sprintf(flag,"%s","0");
//					sprintf(sqlBuf,"insert into RuleList values ('%s','%s','%s',%u,'%s','','','%s','%s',%s)",Rule_id,anjian_id,Rule_name,Rule_type,KeyWord6,Rule_endtime,Rule_areacode,updateflag);					
//					ltDbExecSql(G_DbCon,sqlBuf);
//				}
			
			//更新布控内容
			char *dbUser;
			char *dbPass;
		  char *dbName; 
		  int  nowglevel;
		  char cmdtime[32];
		  char *gid;
		  char cmdno[128];
		  
		  ltDbCursor *tempCursor;
      LT_DBROW tempRow;
      int   jjj;
		  ltDbConn *G_DbConTemp;
			dbName=_ltPubInfo->_dbname;
			dbUser=_ltPubInfo->_dbuser;
			dbPass=_ltPubInfo->_dbpass;
			gid=ltMsgGetVar_s(ltMsgPk,"gid");
			if(!gid){
				 ltMsgPk->msgpktype=1;
			   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,flag);
			   ltMsgFree(ltMsgPk);
			   return 0;
			}
			G_DbConTemp=ltDbConnect(dbUser,dbPass,dbName);
			if(G_DbConTemp==NULL){
				 ltMsgPk->msgpktype=1;
			   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,flag);
			   ltMsgFree(ltMsgPk);
			   return 0;
			}
			memset(cmdtime,0,sizeof(cmdtime));
	    nasTimeLongFormat(cmdtime,time(0));
			nowglevel=0;
		  sprintf(sqlBuf,"select pid,level,name from msasGroup where id=%s ",gid);

			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor){
					tempRow= ltDbFetchRow(tempCursor);
					if(tempRow){
							nowglevel=atol(tempRow[1]);	
					}
					ltDbCloseCursor(tempCursor);  
			}
		  jjj=0;
		  sprintf(sqlBuf,"select gnmkey,Service_name from msasclientinfo where groupid%d=%s ",nowglevel,gid);  
	  
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor){
					tempRow= ltDbFetchRow(tempCursor);
					while(tempRow){
						  jjj++;
						  sprintf(cmdno,"%ld%d%d",time(0),getpid(),jjj);

							sprintf(sqlBuf,"insert into ClientCmd (cmdno,gnmkey,cmdpath,cmdtype,cmdtime,cmdptime,cmduploadtime,updateflag ) values ('%s','%s','%s','RuleList','%s','','','0') ",cmdno,tempRow[0],Rule_id,cmdtime);					  
			    
					    ltDbExecSql(G_DbConTemp,sqlBuf);  
					    /*
					    update online
					    */ 
					    {
										char *p;
									  int  iii;
									  int indexWalker;
									  shm_hash_link_onlinehostindex *_gnm_hash_index;
										shm_hash_link_onlinehost      *_gnm_hash_content;
										shm_hash_link_onlinehost *walker;
										msasOnlineHost  *nasHashItemInfo;
				            static struct sembuf lock={0,-1,SEM_UNDO};
			  						static struct sembuf unlock={0,1,SEM_UNDO|IPC_NOWAIT};
			  
									  p=(char *)_ltPubInfo;
										_gnm_hash_index=(shm_hash_link_onlinehostindex *)(p+sizeof(msasPubInfo));
									  _gnm_hash_content=(shm_hash_link_onlinehost *)(p+sizeof(msasPubInfo)+_ltPubInfo->_onlinehosthead.conbufptr);
									    
									  for(iii=0;iii<_ltPubInfo->_onlinehosthead.bufnum;iii++){
									  	 semop(_ltPubInfo->_onlinehosthead.locksemid,&lock,1); 
									   	 indexWalker=_gnm_hash_index[iii].next;
									   	 if(indexWalker==-1){
									   	 	semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
									   	 	continue;
									   	 }
									   	 semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
											 walker = &_gnm_hash_content[indexWalker];
											 for (; ; ) {
														  nasHashItemInfo=(msasOnlineHost *)(&walker->item);
														  if(strcmp(nasHashItemInfo->caKey,tempRow[0])==0){
														  	nasHashItemInfo->havecmd=1;
														  }
															semop(_ltPubInfo->_onlinehosthead.locksemid,&lock,1); 
															if(walker->next == -1){
																semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
																break;
															}
															indexWalker = walker->next;
															semop(_ltPubInfo->_onlinehosthead.locksemid,&unlock,1);
															walker = &_gnm_hash_content[indexWalker];
											 }
									  }
							}
						  tempRow= ltDbFetchRow(tempCursor);
					}
					ltDbCloseCursor(tempCursor); 
			}
			ltDbClose(G_DbConTemp);
			
	   ltMsgPk->msgpktype=1;
	   lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\r\n","lthtml",LT_TYPE_STRING,flag);
	   ltMsgFree(ltMsgPk);
	   return 0;
}
