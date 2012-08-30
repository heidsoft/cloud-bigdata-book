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


int msaslooparea(stralloc *strTemp,int level,int pid){
	
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	ltDbConn *Tmp_DbCon; /*各个包处理进程的数据库连接*/
	char *dbUser;
	char *dbPass;
	char *dbName;
	char sqlBuf[512];
	char tmpstr[512]; 
	int iHead;
	level++;
  dbName=_ltPubInfo->_dbname;
	dbUser=_ltPubInfo->_dbuser;
	dbPass=_ltPubInfo->_dbpass;
	Tmp_DbCon=ltDbConnect(dbUser,dbPass,dbName);
	if (Tmp_DbCon==NULL){
		return -1;
	}
	sprintf(sqlBuf,"select id,pid,name,mydesc,level,areacode from msasGroup where level=%d and pid=%d order by id asc ",level,pid );
	tempCursor=ltDbOpenCursor(Tmp_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		if(tempRow==NULL){
			sprintf(tmpstr,"%s",",leaf: true");
			stralloc_cats(strTemp,tmpstr);
			ltDbCloseCursor(tempCursor);
			ltDbClose(Tmp_DbCon);
	    return 0;
		}else{
		  sprintf(tmpstr,"%s",",children: [");
			stralloc_cats(strTemp,tmpstr);
	  }
		iHead=1;
		while(tempRow!=NULL){
			if(iHead==1){
				sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s'",tempRow[0],tempRow[5],tempRow[2]);
				iHead=0;
			}else{
			  sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s'",tempRow[0],tempRow[5],tempRow[2]);
		  }			
			stralloc_cats(strTemp,tmpstr);
			pid=atol(tempRow[0]);
			
			if(level<5){
				msaslooparea(strTemp,level,pid);
			}
			stralloc_cats(strTemp,"}");
			tempRow= ltDbFetchRow(tempCursor);

		}
		stralloc_cats(strTemp,"]");
		ltDbCloseCursor(tempCursor);
	}
	ltDbClose(Tmp_DbCon);

	return 0;
}


int msasGroupTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[512];
	char tmpstr[512];
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	int level,pid;
	stralloc strTemp;

	int iHead;
	memset(tmpstr,0,sizeof(tmpstr));

	pid=0;
	level=0;
	iHead=1;
	strTemp.s=0;
	//stralloc_cats(&strTemp,"\xef\xbb\xbf[");
	stralloc_cats(&strTemp,"[");
 	sprintf(sqlBuf,"select id,pid,name,mydesc,level,areacode from msasGroup where level=0 and pid=0 order by id asc " );
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			/*判断有没有子节点*/
			if(iHead==1){
				sprintf(tmpstr,"{id: '%s',qtip: '%s',text: '%s'",tempRow[0],tempRow[5],tempRow[2]);
				iHead=0;
			}else{
			  sprintf(tmpstr,",{id: '%s',qtip: '%s',text: '%s'",tempRow[0],tempRow[5],tempRow[2]);
		  }
		
		  stralloc_cats(&strTemp,tmpstr);	
		
			pid=atol(tempRow[0]);
						
			if(level<5){
				msaslooparea(&strTemp,level,pid);
			}
			stralloc_cats(&strTemp,"}");	
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


int msasGroupAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	int  actiontype;
	
	actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));
	if(actiontype==1){/*add*/
	
		long lCount;
		int tttid;
    lCount=0;
    for(tttid=1;tttid<NAS_MAX_ALLGRPNUM;tttid++){
    	
		        sprintf(sqlBuf,"select count(*) from msasGroup where id=%d ",tttid);
						tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
						if(tempCursor){
							tempRow= ltDbFetchRow(tempCursor);
							if(tempRow){
								if( atol(tempRow[0])==0){
									lCount=tttid;
									ltDbCloseCursor(tempCursor);
									break;
								}
							}
							ltDbCloseCursor(tempCursor);
						}
    }
    if(lCount>0){
				sprintf(sqlBuf,"insert into msasGroup values (%ld,%s,'%s','%s',%s,'%s')",
					lCount,ltMsgGetVar_s(ltMsgPk,"pid"),ltMsgGetVar_s(ltMsgPk,"name"),ltMsgGetVar_s(ltMsgPk,"mydesc"),
					ltMsgGetVar_s(ltMsgPk,"level"),ltMsgGetVar_s(ltMsgPk,"areacode"));
				//printf("sqlBuf:%s\n",sqlBuf);
				ltDbExecSql(G_DbCon,sqlBuf); 
	  }
	  ltMsgPk->msgpktype=1;
	  sprintf(sqlBuf,"%ld",lCount);
		lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,sqlBuf);
		ltMsgFree(ltMsgPk);
		return 0;
	}else if (actiontype==2){/*del*/
		int allnum;
		allnum=0;
		/*有子分组就不能删除*/
		sprintf(sqlBuf,"select count(*) from msasGroup where pid=%s ",ltMsgGetVar_s(ltMsgPk,"id"));
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow){
						allnum= atol(tempRow[0]);
				}
				ltDbCloseCursor(tempCursor);
		}
		if(allnum==0){				
			sprintf(sqlBuf,"delete from msasGroup where id=%s ",ltMsgGetVar_s(ltMsgPk,"id"));		
			ltDbExecSql(G_DbCon,sqlBuf);
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
			ltMsgFree(ltMsgPk);
			return 0;
		}
		
		ltMsgPk->msgpktype=1;
	  lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"1");
	  ltMsgFree(ltMsgPk);
		return 0;

	}else if (actiontype==3){/*update*/
		if(atol(ltMsgGetVar_s(ltMsgPk,"id"))!=0){
			sprintf(sqlBuf,"update msasGroup set name='%s',mydesc='%s',areacode='%s' where id=%s ",
			ltMsgGetVar_s(ltMsgPk,"name"),ltMsgGetVar_s(ltMsgPk,"mydesc"),ltMsgGetVar_s(ltMsgPk,"areacode"),
			ltMsgGetVar_s(ltMsgPk,"id"));		
			//printf("%s\n",sqlBuf);			
			ltDbExecSql(G_DbCon,sqlBuf); 
			ltMsgPk->msgpktype=1;
			lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"0");
			ltMsgFree(ltMsgPk);
			return 0;
		}
	}
	return 0;

}


int msasShowAddClient(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
		
  ltDbHeadPtr dbPtr; 
  dbPtr=lt_dbinit();
  lt_db_htmlpage(dbPtr,"utf-8");
  lt_dbput_rootvars(dbPtr,1,"id",ltMsgGetVar_s(ltMsgPk,"id"));
  ltMsgPk->msgpktype=1;
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/manager/msasterminfonew.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}

int msasTerminfoAdd(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    
    char sqlBuf[2048];
    char caDate[32];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    int  nowgid;
		int  nowglevel;
		int  pgid[MSAS_MAX_GROUPLEVEL];
	  int  iii;
    char areacode[32];
    int  newgid;
    char servicecode[32];
    
   
    memset(areacode,0,sizeof(areacode));
    for(iii=0;iii<MSAS_MAX_GROUPLEVEL;iii++){
  		pgid[iii]=0;
  	}
		nowglevel=0;
		nowgid=atol(ltMsgGetVar_s(ltMsgPk,"id"));
    sprintf(sqlBuf,"select pid,level,areacode from msasGroup where id=%d ",nowgid);

		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow){
					nowglevel=atol(tempRow[1]);	
					sprintf(areacode,"%s",tempRow[2]);			
			}
			ltDbCloseCursor(tempCursor);
		}
		if(nowglevel>=MSAS_MAX_GROUPLEVEL){
			ltMsgPk->msgpktype=1;
		  ltWebMsgErr(_ltPubInfo->pubMsgStr[7].conStr, confd,ltMsgPk);
		  ltMsgFree(ltMsgPk);
			return 0;
		}
	
  
		for(iii=nowglevel;iii>-1;iii--){
	  	sprintf(sqlBuf,"select pid from msasGroup where id=%d and level=%d ",nowgid,iii);
			tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
			if(tempCursor){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow){
						pgid[iii]=nowgid;		
						nowgid=atol(tempRow[0]);			
				}
				ltDbCloseCursor(tempCursor);
			}
	  }
 	  
	  /*生成序号,先cunt(*) maxid */
	  newgid=0;
	  sprintf(sqlBuf,"select max(UnitNo)+1 from msasclientinfo ");
		tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
		if(tempCursor){
			tempRow= ltDbFetchRow(tempCursor);
			if(tempRow){
						newgid=atol(tempRow[0]);			
			}
			ltDbCloseCursor(tempCursor);
		}
	  /*生成servicecode,根据areacode 序号等*/
	  sprintf(servicecode,"%s%08d",areacode,newgid);
	  
    sprintf(caDate,"%s",ltTimeFormat("%Y%m%d%H:%M:%S",time(0)));    
     		
		sprintf(sqlBuf,"insert into msasclientinfo (gnmkey,UnitNo,groupid0,groupid1,groupid2,groupid3,groupid4, \
Service_code,Service_name,Address,Zip,Principal,Principal_id, \  
Principal_tel,Principal_email,yingyearea,Practitioner_number,Ending_number, \
Infor_man,Infor_man_tel,Infor_man_id,Infor_man_email, \
Ip,Net_type,clienttype,producer_code,Status,Net_monitor_department,Net_monitor_man,Net_monitor_man_tel, \
onlinehost,usernum,startday,endday,lasttime, \
lastappupdatev,lastappeupdate,updateflag,intervalfig,cmdupdateflag, \
cmdurl,alarmlog,trlog,onlinelog,identitylog,conlog,statlog,uploadtype,uploadtimeval,uploadip,Remark,vpnserver,vpnaddress,vpnpass) \
values ( '%s',%d,%d,%d,%d,%d,%d,'%s','%s','%s','%s','%s','%s','%s','%s',%s,%s,%s,'%s','%s','%s','%s','%s',\
'%s','%s','%s','%s','%s','%s','%s', \
%s,%s,'%s','%s','%s','%s','%s',%s,%s,%s,'%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s',%s,'%s','%s') ",
ltMsgGetVar_s(ltMsgPk,"gnmkey"),
newgid,pgid[0],pgid[1],pgid[2],pgid[3],pgid[4],
servicecode,
ltMsgGetVar_s(ltMsgPk,"clientname"),ltMsgGetVar_s(ltMsgPk,"clientadd"),
ltMsgGetVar_s(ltMsgPk,"Zip"),ltMsgGetVar_s(ltMsgPk,"zherenren"),
ltMsgGetVar_s(ltMsgPk,"zherenrenid"),ltMsgGetVar_s(ltMsgPk,"zherenrentel"),
ltMsgGetVar_s(ltMsgPk,"zherenrenemail"),ltMsgGetVar_s(ltMsgPk,"yingyearea"),
ltMsgGetVar_s(ltMsgPk,"congyenum"),ltMsgGetVar_s(ltMsgPk,"termnum"),
ltMsgGetVar_s(ltMsgPk,"secname"),ltMsgGetVar_s(ltMsgPk,"sectel"),
ltMsgGetVar_s(ltMsgPk,"secid"),ltMsgGetVar_s(ltMsgPk,"secemail"),"",
ltMsgGetVar_s(ltMsgPk,"connecttype"),ltMsgGetVar_s(ltMsgPk,"clienttype"),
ltMsgGetVar_s(ltMsgPk,"producer_code"),ltMsgGetVar_s(ltMsgPk,"clientstatus"),
ltMsgGetVar_s(ltMsgPk,"Net_monitor_department"),ltMsgGetVar_s(ltMsgPk,"Net_monitor_man"),ltMsgGetVar_s(ltMsgPk,"Net_monitor_man_tel"),
"0",ltMsgGetVar_s(ltMsgPk,"usernum"),ltMsgGetVar_s(ltMsgPk,"startday"),ltMsgGetVar_s(ltMsgPk,"endday"),"",
ltMsgGetVar_s(ltMsgPk,"lastappupdatev"),"",ltMsgGetVar_s(ltMsgPk,"updateflag")?"1":"0",
ltMsgGetVar_s(ltMsgPk,"intervalfig"),"0","",
ltMsgGetVar_s(ltMsgPk,"alarmlog")?"Y":"N",ltMsgGetVar_s(ltMsgPk,"trlog")?"Y":"N",
ltMsgGetVar_s(ltMsgPk,"onlinelog")?"Y":"N",ltMsgGetVar_s(ltMsgPk,"identitylog")?"Y":"N",
ltMsgGetVar_s(ltMsgPk,"conlog")?"Y":"N",ltMsgGetVar_s(ltMsgPk,"statlog")?"Y":"N",
ltMsgGetVar_s(ltMsgPk,"uploadtype"),ltMsgGetVar_s(ltMsgPk,"uploadtimeval"),
ltMsgGetVar_s(ltMsgPk,"uploadcmd"),ltMsgGetVar_s(ltMsgPk,"note"),
ltMsgGetVar_s(ltMsgPk,"vpnserver")?"1":"0",ltMsgGetVar_s(ltMsgPk,"vpnaddress"),ltMsgGetVar_s(ltMsgPk,"vpnpass"));


ltDbExecSql(G_DbCon,sqlBuf);

if(ltMsgGetVar_s(ltMsgPk,"vpnserver")){
	if(strlen(ltMsgGetVar_s(ltMsgPk,"vpnaddress"))>0){
		char caIpadd[32];
		unsigned int iIp;
		struct in_addr s;
		
		sprintf(caIpadd,"%s",ltMsgGetVar_s(ltMsgPk,"vpnaddress"));
		iIp=ntohl(inet_addr(caIpadd));
		iIp++;
		s.s_addr = htonl(iIp);
		sprintf(sqlBuf,"echo 'ifconfig-push %s %s'>/app/vpnserver/client/%s",caIpadd,inet_ntoa(s),servicecode); 
		system(sqlBuf);
	}
}

 

 msasTermInfoView( confd,ltMsgPk,lt_MMHead);
 return 0;
 
}


int msasTermInfoView(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    
    
    char strBuf[2048];
    char strGroupPath[512];
    char strtmp[128];
    ltDbHeadPtr dbPtr;
    ltDbCursor  *tempCursor;
    LT_DBROW    tempRow;
    char        *gnmkey;
    
    ltDbCursor *tempCursor1;
		LT_DBROW tempRow1;
		ltDbConn *Tmp_DbCon; /*各个包处理进程的数据库连接*/
		char *dbUser;
		char *dbPass;
		char *dbName;
		int   iii;
		
	  dbName=_ltPubInfo->_dbname;
		dbUser=_ltPubInfo->_dbuser;
		dbPass=_ltPubInfo->_dbpass;
		Tmp_DbCon=ltDbConnect(dbUser,dbPass,dbName);
		if (Tmp_DbCon==NULL){
			ltMsgPk->msgpktype=1;
		  ltWebMsgErr(_ltPubInfo->pubMsgStr[6].conStr, confd,ltMsgPk);
		  ltMsgFree(ltMsgPk);
			return 0;
		}
    
    dbPtr=lt_dbinit();
    lt_db_htmlpage(dbPtr,"utf-8");
    
    gnmkey=ltMsgGetVar_s(ltMsgPk,"gnmkey");
    memset(strGroupPath,0,sizeof(strGroupPath));
    sprintf(strBuf,"select gnmkey,unitno,groupid0,groupid1,groupid2,groupid3,groupid4,service_code,\
service_name,address,zip,principal,principal_id,principal_tel,principal_email,yingyearea,\
practitioner_number,ending_number,infor_man,infor_man_tel,infor_man_id,infor_man_email,ip,\
net_type,clienttype,producer_code,status,net_monitor_department,net_monitor_man,net_monitor_man_tel,\
onlinehost,usernum,startday,endday,lasttime,lastappupdatev,lastappeupdate,updateflag,intervalfig,\
cmdupdateflag,cmdurl,alarmlog,trlog,onlinelog,identitylog,conlog,statlog,uploadtype,uploadtimeval,\
uploadip,registname,registuser,registadd,registtel,registmail,registtime,myservice_code,\
verifycode,remark,vpnserver,vpnaddress,vpnpass from msasclientinfo where gnmkey='%s' ",gnmkey);
     
//printf("sql:%s\n", strBuf);    
   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	if(tempRow!=NULL){
            /*读取分组路径*/
            for(iii=2;iii<7;iii++){
		            if(iii==2){
		            	sprintf(strBuf,"select name from msasGroup where id=%s",tempRow[iii]);
		            }else{
		              sprintf(strBuf,"select name from msasGroup where id=%s and level>0",tempRow[iii]);
		            }
		            tempCursor1=ltDbOpenCursor(Tmp_DbCon,strBuf);
							  if(tempCursor1!=NULL){
								      tempRow1= ltDbFetchRow(tempCursor1);
							       	if(tempRow1!=NULL){
							       		if(strlen(strGroupPath)>0){
							       			sprintf(strGroupPath,"%s-%s",strGroupPath,tempRow1[0]);
							       		}else{
							       		  sprintf(strGroupPath,"%s",tempRow1[0]);
							       	  }
							       	}
							       ltDbCloseCursor(tempCursor1);	
							  }
						} 
            
						if( strcmp(tempRow[23],"01")==0 ){
							lt_dbput_rootvars(dbPtr,1,"nsel1"," selected ");
						}else if( strcmp(tempRow[16],"02")==0 ){
							lt_dbput_rootvars(dbPtr,1,"nsel2"," selected ");
						}else if( strcmp(tempRow[16],"03")==0 ){
							lt_dbput_rootvars(dbPtr,1,"nsel3"," selected ");
						}else if( strcmp(tempRow[16],"04")==0 ){
							lt_dbput_rootvars(dbPtr,1,"nsel4"," selected ");
						}else if( strcmp(tempRow[16],"05")==0 ){
							lt_dbput_rootvars(dbPtr,1,"nsel5"," selected ");
						}else if( strcmp(tempRow[16],"06")==0 ){
							lt_dbput_rootvars(dbPtr,1,"nsel6"," selected ");
						}else if( strcmp(tempRow[16],"07")==0 ){
							lt_dbput_rootvars(dbPtr,1,"nsel7"," selected ");
						}else if( strcmp(tempRow[16],"08")==0 ){
							lt_dbput_rootvars(dbPtr,1,"nsel8"," selected ");
						}else {
							lt_dbput_rootvars(dbPtr,1,"nsel99"," selected ");
						}
						
						sprintf(strtmp,"csel%s",tempRow[24]);
						lt_dbput_rootvars(dbPtr,1,strtmp," selected ");
						
										
						if( strcmp(tempRow[25],"01")==0 ){
							lt_dbput_rootvars(dbPtr,1,"psel1"," selected ");
						}else if( strcmp(tempRow[18],"02")==0 ){
							lt_dbput_rootvars(dbPtr,1,"psel2"," selected ");
						}else if( strcmp(tempRow[18],"03")==0 ){
							lt_dbput_rootvars(dbPtr,1,"psel3"," selected ");
						}else if( strcmp(tempRow[18],"04")==0 ){
							lt_dbput_rootvars(dbPtr,1,"psel4"," selected ");
						}else if( strcmp(tempRow[18],"05")==0 ){
							lt_dbput_rootvars(dbPtr,1,"psel5"," selected ");
						}else if( strcmp(tempRow[18],"06")==0 ){
							lt_dbput_rootvars(dbPtr,1,"psel6"," selected ");
						}else if( strcmp(tempRow[18],"07")==0 ){
							lt_dbput_rootvars(dbPtr,1,"psel7"," selected ");
						}else if( strcmp(tempRow[18],"08")==0 ){
							lt_dbput_rootvars(dbPtr,1,"psel8"," selected ");
						}else if( strcmp(tempRow[18],"09")==0 ){
							lt_dbput_rootvars(dbPtr,1,"psel9"," selected ");
						}else {
							lt_dbput_rootvars(dbPtr,1,"psel99"," selected ");
						}
						
						sprintf(strtmp,"ssel%s",tempRow[26]);
						lt_dbput_rootvars(dbPtr,1,strtmp," selected ");
						
						if( strcmp(tempRow[31],"50")==0 ){
							lt_dbput_rootvars(dbPtr,1,"usel1"," selected ");
						}else if( strcmp(tempRow[31],"100")==0 ){
							lt_dbput_rootvars(dbPtr,1,"usel2"," selected ");
						}else if( strcmp(tempRow[31],"200")==0 ){
							lt_dbput_rootvars(dbPtr,1,"usel3"," selected ");
						}else if( strcmp(tempRow[31],"300")==0 ){
							lt_dbput_rootvars(dbPtr,1,"usel4"," selected ");
						}else if( strcmp(tempRow[31],"500")==0 ){
							lt_dbput_rootvars(dbPtr,1,"usel5"," selected ");
						}else if( strcmp(tempRow[31],"800")==0 ){
							lt_dbput_rootvars(dbPtr,1,"usel6"," selected ");
						}else if( strcmp(tempRow[31],"1000")==0 ){
							lt_dbput_rootvars(dbPtr,1,"usel7"," selected ");
						}else if( strcmp(tempRow[31],"200")==0 ){
							lt_dbput_rootvars(dbPtr,1,"usel8"," selected ");
						}else if( strcmp(tempRow[31],"3000")==0 ){
							lt_dbput_rootvars(dbPtr,1,"usel9"," selected ");
						}else if( strcmp(tempRow[31],"5000")==0 ){
							lt_dbput_rootvars(dbPtr,1,"usel10"," selected ");
						}else if( strcmp(tempRow[31],"8000")==0 ){
							lt_dbput_rootvars(dbPtr,1,"usel11"," selected ");
						}else if( strcmp(tempRow[31],"10000")==0 ){
							lt_dbput_rootvars(dbPtr,1,"usel12"," selected ");
						}else if( strcmp(tempRow[31],"20000")==0 ){
							lt_dbput_rootvars(dbPtr,1,"usel13"," selected ");
						}else if( strcmp(tempRow[31],"50000")==0 ){
							lt_dbput_rootvars(dbPtr,1,"usel14"," selected ");
						}else if( strcmp(tempRow[31],"100000")==0 ){
							lt_dbput_rootvars(dbPtr,1,"usel15"," selected ");
						}
						
						if( strcmp(tempRow[47],"0")==0 ){
							lt_dbput_rootvars(dbPtr,1,"upsel0"," selected ");
						}else if( strcmp(tempRow[47],"1")==0 ){
							lt_dbput_rootvars(dbPtr,1,"upsel1"," selected ");
						}
						
						lt_dbput_rootvars(dbPtr,44,
						  "gnmkey",tempRow[0],
							"group",strGroupPath,
							"clientid",tempRow[7],
							"clientname",tempRow[8],
							"clientadd",tempRow[9],
							"zherenren",tempRow[11],
							"zherenrenid",tempRow[12],
							"zherenrentel",tempRow[13],
							"zherenrenemail",tempRow[14],
							"yingyearea",tempRow[15],
							"congyenum",tempRow[16],
							"termnum",tempRow[17],
							"secname",tempRow[18],
							"sectel",tempRow[19],
							"secid",tempRow[20],
							"secemail",tempRow[21],
							"iplist",tempRow[22],
							"lasttime",tempRow[34],
							"intervalfig",tempRow[38],
							"alarmlog",tempRow[41][0]=='Y'?"checked":"",
							"trlog",tempRow[42][0]=='Y'?"checked":"",
							"onlinelog",tempRow[43][0]=='Y'?"checked":"",
							"identitylog",tempRow[44][0]=='Y'?"checked":"",
							"conlog",tempRow[45][0]=='Y'?"checked":"",
							"daystat",tempRow[46][0]=='Y'?"checked":"",
							"startday",tempRow[32],
							"endday",tempRow[33],
							"updateflag",tempRow[37][0]=='1'?"checked":"",
							"lastappupdatev",tempRow[35],
							"registname",tempRow[50],
							"registuser",tempRow[51],
							"registadd", tempRow[52],
							"registtel",tempRow[53],
							"registmail",tempRow[54],
							"Net_monitor_department", tempRow[27],
							"Net_monitor_man",tempRow[28],
							"Net_monitor_man_tel",tempRow[29],
							"UnitNo",tempRow[1],
							"uploadtimeval",tempRow[48],
							"uploadcmd",tempRow[49],
							"Zip",tempRow[10],
							"vpnserverchk",tempRow[59][0]=='1'?"checked":"",
							"vpnaddress",tempRow[60],
							"vpnpass",tempRow[61]
							);
	      }
        ltDbCloseCursor(tempCursor);
        
        
   }
   ltDbClose(Tmp_DbCon);
 /*全局变量*/
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/manager/msasterminfodetail.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
   
}



/* 网站访问查询  */
int msasTermInfoUpdate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    
    ltDbCursor  *tempCursor;
    LT_DBROW    tempRow;
    char strBuf[2048];
    char *gnmkey;
    /*升级终端管理，先按照gnmkey查找然后更新或新建*/

    gnmkey=ltMsgGetVar_s(ltMsgPk,"gnmkey");
    sprintf(strBuf,"update  msasclientinfo set Service_name='%s',Address='%s',Principal='%s', \
Principal_id='%s',Principal_tel='%s',Principal_email='%s',yingyearea=%s,Practitioner_number=%s,Ending_number=%s,Infor_man='%s', \
Infor_man_tel='%s',Infor_man_id='%s',Infor_man_email='%s',Ip='%s',Net_type='%s',clienttype='%s',producer_code='%s',Status='%s',  \
usernum=%s,intervalfig=%s,alarmlog='%s',trlog='%s',onlinelog='%s',identitylog='%s',conlog='%s',statlog='%s', \
startday='%s',endday='%s',updateflag='%s',lastappupdatev='%s',Net_monitor_department='%s', Net_monitor_man='%s',Net_monitor_man_tel='%s', \
uploadtype='%s',uploadtimeval='%s',Zip='%s',vpnserver=%s,vpnaddress='%s',vpnpass='%s' where gnmkey='%s' ",
ltMsgGetVar_s(ltMsgPk,"clientname"),
ltMsgGetVar_s(ltMsgPk,"clientadd"),
ltMsgGetVar_s(ltMsgPk,"zherenren"),

ltMsgGetVar_s(ltMsgPk,"zherenrenid"),
ltMsgGetVar_s(ltMsgPk,"zherenrentel"),
ltMsgGetVar_s(ltMsgPk,"zherenrenemail"),
ltMsgGetVar_s(ltMsgPk,"yingyearea"),
ltMsgGetVar_s(ltMsgPk,"congyenum"),
ltMsgGetVar_s(ltMsgPk,"termnum"),
ltMsgGetVar_s(ltMsgPk,"secname"),

ltMsgGetVar_s(ltMsgPk,"sectel"),
ltMsgGetVar_s(ltMsgPk,"secid"),
ltMsgGetVar_s(ltMsgPk,"secemail"),
ltMsgGetVar_s(ltMsgPk,"iplist"),
ltMsgGetVar_s(ltMsgPk,"connecttype"),
ltMsgGetVar_s(ltMsgPk,"clienttype"),
ltMsgGetVar_s(ltMsgPk,"producer_code"),
ltMsgGetVar_s(ltMsgPk,"clientstatus"),

ltMsgGetVar_s(ltMsgPk,"usernum"),
ltMsgGetVar_s(ltMsgPk,"intervalfig"),
ltMsgGetVar_s(ltMsgPk,"alarmlog")?"Y":"N",
ltMsgGetVar_s(ltMsgPk,"trlog")?"Y":"N",
ltMsgGetVar_s(ltMsgPk,"onlinelog")?"Y":"N",
ltMsgGetVar_s(ltMsgPk,"identitylog")?"Y":"N",
ltMsgGetVar_s(ltMsgPk,"conlog")?"Y":"N",
ltMsgGetVar_s(ltMsgPk,"statlog")?"Y":"N",

ltMsgGetVar_s(ltMsgPk,"startday"),
ltMsgGetVar_s(ltMsgPk,"endday"),
ltMsgGetVar_s(ltMsgPk,"updateflag")?"1":"0",
ltMsgGetVar_s(ltMsgPk,"lastappupdatev"),

ltMsgGetVar_s(ltMsgPk,"Net_monitor_department"),
ltMsgGetVar_s(ltMsgPk,"Net_monitor_man"),
ltMsgGetVar_s(ltMsgPk,"Net_monitor_man_tel"),
ltMsgGetVar_s(ltMsgPk,"uploadtype"),
ltMsgGetVar_s(ltMsgPk,"uploadtimeval"),
ltMsgGetVar_s(ltMsgPk,"Zip"),
ltMsgGetVar_s(ltMsgPk,"vpnserver")?"1":"0",
ltMsgGetVar_s(ltMsgPk,"vpnaddress"),
ltMsgGetVar_s(ltMsgPk,"vpnpass"),
gnmkey);
//printf("strBuf:%s\n",strBuf);
ltDbExecSql(G_DbCon,strBuf);
if(ltMsgGetVar_s(ltMsgPk,"vpnserver")){
	if(strlen(ltMsgGetVar_s(ltMsgPk,"vpnaddress"))>0){
		char caIpadd[32];
		char servicecode[64];
		unsigned int iIp;
		struct in_addr s;
		memset(servicecode,0,sizeof(servicecode));
		sprintf(strBuf,"select service_code from msasclientinfo where gnmkey='%s' ",gnmkey);  
    tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
    if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	if(tempRow!=NULL){
       		sprintf(servicecode,"%s",tempRow[0]);
       	}
       	ltDbCloseCursor(tempCursor);	
    } 
    if(strlen(servicecode)>0){		
			sprintf(caIpadd,"%s",ltMsgGetVar_s(ltMsgPk,"vpnaddress"));
			iIp=ntohl(inet_addr(caIpadd));
			iIp++;
			s.s_addr = htonl(iIp);
			sprintf(strBuf,"echo 'ifconfig-push %s %s'>/app/vpnserver/client/%s",caIpadd,inet_ntoa(s),servicecode); 
			system(strBuf);
	  }
	}
} 
   msasTermInfoView( confd,ltMsgPk,lt_MMHead);
  	
   return 0;
 
}

int msasShowClientList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	 char *gid;
	 char *gnmlevel;
	 int  level;
   ltDbHeadPtr dbPtr;
   char  strlevel[32];
   
   gnmlevel=ltMsgGetVar_s(ltMsgPk,"level");
   gid=ltMsgGetVar_s(ltMsgPk,"gid");
	 level=0;
	 if(!gnmlevel){
	 		gnmlevel="0";	
	 }
	 if(!gid){
	 		gid="";	
	 }
	 level=atol(gnmlevel);
	 if(level<0 || level>=MSAS_MAX_GROUPLEVEL ){
	 	 level=0;
	 }
   sprintf(strlevel,"%d",level);
	 
	 dbPtr=lt_dbinit();
   lt_db_htmlpage(dbPtr,"utf-8");
   
   lt_dbput_rootvars(dbPtr,1,"gid",gid);
   lt_dbput_rootvars(dbPtr,1,"level",strlevel);
   
   ltMsgPk->msgpktype=1;
   lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/manager/clientlistinfo.htm",dbPtr->head,0);
   ltMsgFree(ltMsgPk);
   lt_dbfree(dbPtr);
   return 0;
	 
}
	
int  isOnlineHost(char *gnmkey){
	  
	  
    static struct sembuf lock={0,-1,SEM_UNDO};
	  static struct sembuf unlock={0,1,SEM_UNDO|IPC_NOWAIT};

	  
		char *p;
		int  iii;
		int indexWalker;
		shm_hash_link_onlinehostindex *_gnm_hash_index;
		shm_hash_link_onlinehost      *_gnm_hash_content;
		shm_hash_link_onlinehost *walker;
		msasOnlineHost  *nasHashItemInfo;

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
						  nasHashItemInfo=&walker->item;
						  if(strcmp(nasHashItemInfo->caKey,gnmkey)==0){
						  	return 1;
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
	
	  return 0;
}

int msasClientList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	 char *delflag;
	 char *gid;
	 char *gnmlevel;
	 int  level;
   char sqlStr[1024];
   char sqlStr1[1024];
   char caTmpp[1024];
   stralloc strTemp;
   int  allcount;
   ltDbCursor  *tempCursor;
   LT_DBROW    tempRow;
   int  k;
   gnmlevel=ltMsgGetVar_s(ltMsgPk,"level");
   gid=ltMsgGetVar_s(ltMsgPk,"gid");
	 
	 delflag=ltMsgGetVar_s(ltMsgPk,"delflag");
	 if(delflag){
	 		char *gnmkey=ltMsgGetVar_s(ltMsgPk,"gnmkey");

			sprintf(sqlStr,"select service_code from msasclientinfo where gnmkey='%s' ",gnmkey);  
			tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
			if(tempCursor!=NULL){
				      tempRow= ltDbFetchRow(tempCursor);
			       	if(tempRow!=NULL){
			       		sprintf(sqlStr,"/app/vpnserver/client/%s",tempRow[0]); 
			       		unlink(sqlStr);
			       	}
			       	ltDbCloseCursor(tempCursor);	
			} 
	 		sprintf(sqlStr,"delete from msasclientinfo where gnmkey='%s'",gnmkey);
	 		ltDbExecSql(G_DbCon,sqlStr);
	 }
	 level=0;
	 if(!gnmlevel){
	 		gnmlevel="0";	
	 }
	 if(!gid){
	 		gid="";	
	 }
	 level=atol(gnmlevel);
	 if(level<0 || level>=MSAS_MAX_GROUPLEVEL ){
	 	 level=0;
	 }
	 memset(sqlStr,0,sizeof(sqlStr));
	 if(strlen(gid)<1){
	 		sprintf(sqlStr,"%s","select count(*) from msasclientinfo");
	 		sprintf(sqlStr1,"%s","select gnmkey,Service_code,Service_name,Address,Principal,Principal_tel,usernum,startday,endday from msasclientinfo");
	 }else{
	    sprintf(sqlStr,"select count(*) from msasclientinfo where groupid%d=%s",level,gid);
	    sprintf(sqlStr1,"select gnmkey,Service_code,Service_name,Address,Principal,Principal_tel,usernum,startday,endday from msasclientinfo where groupid%d=%s",level,gid);
	 }
   allcount=0;
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	if(tempRow!=NULL){
       		allcount=atol(tempRow[0]);
       	}
       	ltDbCloseCursor(tempCursor);
   }   
   strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
   k=0;
   tempCursor=ltDbOpenCursor(G_DbCon,sqlStr1);
   if(tempCursor!=NULL){
	      tempRow= ltDbFetchRow(tempCursor);
       	while(tempRow!=NULL){
       		int  isOnline;
       		isOnline=0;
       		isOnline=isOnlineHost(tempRow[0]);
       		if(k==0){
	     	  	 sprintf(caTmpp,"{\"isonline\":\"%d\",\"gnmkey\":\"%s\",\"Service_code\":\"%s\",\"Service_name\":\"%s\",\"Address\":\"%s\",\"Principal\":\"%s\",\"Principal_tel\":\"%s\",\"usernum\":\"%s\",\"startday\":\"%s\",\"endday\":\"%s\"}",isOnline,tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8]);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }else{
	     	  	  sprintf(caTmpp,",{\"isonline\":\"%d\",\"gnmkey\":\"%s\",\"Service_code\":\"%s\",\"Service_name\":\"%s\",\"Address\":\"%s\",\"Principal\":\"%s\",\"Principal_tel\":\"%s\",\"usernum\":\"%s\",\"startday\":\"%s\",\"endday\":\"%s\"}",isOnline,tempRow[0],tempRow[1],tempRow[2],tempRow[3],tempRow[4],tempRow[5],tempRow[6],tempRow[7],tempRow[8]);
	     	     stralloc_cats(&strTemp,caTmpp);
	     	  }
	     	  k++;
       		tempRow= ltDbFetchRow(tempCursor);
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



int msasAdminUserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	int i;
	char caSel[32];
	char sqlBuf[1024];
	ltDbHeadPtr dbPtr;
	ltTablePtr tablePtr;
	ltDbCursor *tempCursor;
	LT_DBROW tempRow;
	char     caRight[128];
	long inum;
	char caTime[40];
  char *strUserName;
  strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
  if(!strUserName){
  	strUserName="";
  }
  
	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	
	lt_dbput_rootvars(dbPtr,1,"checkall"," checked ");
	tablePtr=lt_dbput_table(dbPtr,"list");

  sprintf(sqlBuf,"select name,lright,lasttime,bindip from msasadmuser ");
	inum=0;
	tempCursor=ltDbOpenCursor(G_DbCon,sqlBuf);
	if(tempCursor!=NULL){
		tempRow= ltDbFetchRow(tempCursor);
		while(tempRow!=NULL){
			inum++;
			nasTimeFormat(caTime,atol(tempRow[2]));
			lt_dbput_recordvars(tablePtr,3,
				"num",LT_TYPE_LONG,inum,
				"logname",LT_TYPE_STRING,tempRow[0],
				"lasttime",LT_TYPE_STRING,caTime);
				
			if(strcmp(strUserName,tempRow[0])==0){				
				lt_dbput_rootvars(dbPtr,1,"logname",tempRow[0]);
				memset(caRight,0,sizeof(caRight));
				sprintf(caRight,"%s",tempRow[1]);
				for(i=0;i<5;i++){
					sprintf(caSel,"chsel%d",i+1);
					if(caRight[i]=='1'){
						lt_dbput_rootvars(dbPtr,1,caSel," checked ");
					}
				}
				lt_dbput_rootvars(dbPtr,1,"bindip",tempRow[3]);
			}
			tempRow= ltDbFetchRow(tempCursor);
		}
		ltDbCloseCursor(tempCursor);
	}
	
	lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/manager/adminuser.htm",dbPtr->head,0);
	ltMsgFree(ltMsgPk);
	lt_dbfree(dbPtr);
	return 0;

}

int ltPassIsOk(char *InStr)
{
   register int i;
   int   findD;
   int   findS;
   findD=0;
   findS=0;
   for(i=0;InStr[i]!=0;i++) {
     if(InStr[i]>'9'||InStr[i]<'0') {
         findS=1;
     }else{
       findD=1;
     }
   }
   if(findD && findS){
   			return 1;
   }
   return 0;
}


int msasAdminAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	char sqlBuf[1024];
	char *strUserName;
	char *strPassword;
  char newPass[50];
	char sRight[100];

	int  i;
	char caSel[20];
	char sIp[24];
	char caMsg[256];
	int  passisok;
	char *bindip;
  int  actiontype;
  
  actiontype=0;
	passisok=0;
	memset(sIp,0,sizeof(sIp));
	memset(newPass,0,sizeof(newPass));
	memset(sRight,0,sizeof(sRight));

	actiontype=atol(ltMsgGetVar_s(ltMsgPk,"actiontype"));

	if(actiontype==1){/*add*/
			strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
			strPassword=ltMsgGetVar_s(ltMsgPk,"pass");
			bindip=ltMsgGetVar_s(ltMsgPk,"bindip");
			
			if(!bindip){
				bindip="";
			}
			if(strlen(strPassword)<8){
				ltMsgPk->msgpktype=1;
				ltWebMsgErr(_ltPubInfo->pubMsgStr[8].conStr, confd,ltMsgPk);
				ltMsgFree(ltMsgPk);
				return 0;
			}
			
			passisok=ltPassIsOk(strPassword);
			if(passisok==0){
				ltMsgPk->msgpktype=1;
				ltWebMsgErr(_ltPubInfo->pubMsgStr[8].conStr, confd,ltMsgPk);
				ltMsgFree(ltMsgPk);
				return 0;
			}
			
			ltMd5Encrypto(strPassword, strlen(strPassword),newPass);
			ltMsgGetSomeNVar(ltMsgPk,1,"clientip",LT_TYPE_STRING, 19, sIp);
			memset(sRight,0,sizeof(sRight));
			memset(sRight,'0',27);
			for(i=0;i<5;i++){
				sprintf(caSel,"chr%d",i+1);
				if(ltMsgGetVar_s(ltMsgPk,caSel)!=NULL){
					sRight[i]='1';
				}
			}
			sprintf(sqlBuf,"insert into msasadmuser values ('%s','%s','%s','',%ld,'%s')",
					strUserName,
					newPass,
					sRight,
					time(0),bindip);

			ltDbExecSql(G_DbCon,sqlBuf); 
			
			sprintf(caMsg,"add new user %s success.",strUserName);
			logWrite(ltMsgPk,caMsg,5);
	}else if(actiontype==2){/*del*/
			strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
			sprintf(sqlBuf,"delete from msasadmuser where name='%s' ",	strUserName);
			ltDbExecSql(G_DbCon,sqlBuf); 
			//char caMsg[256];
			sprintf(caMsg,"delete %s success.",strUserName);
			logWrite(ltMsgPk,caMsg,5);
	}else if(actiontype==3){/*update indfo*/
			memset(sRight,0,sizeof(sRight));
			strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
			memset(sRight,'0',5);	
			for(i=0;i<5;i++){
				sprintf(caSel,"chr%d",i+1);
				if(ltMsgGetVar_s(ltMsgPk,caSel)!=NULL){
					sRight[i]='1';
				}
			}
			sprintf(sqlBuf,"update msasadmuser set lright='%s' where name='%s' ",sRight,strUserName);
		//printf("%s\n",sqlBuf);
			ltDbExecSql(G_DbCon,sqlBuf); 
			//char caMsg[256];
			sprintf(caMsg,"update %s right success.",strUserName);
			logWrite(ltMsgPk,caMsg,2);
	}else if(actiontype==4){/*change password indfo*/
			memset(newPass,0,sizeof(newPass));
			strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
			strPassword=ltMsgGetVar_s(ltMsgPk,"pass");
			passisok=0;
		  passisok=ltPassIsOk(strPassword);
			if(passisok==0){
				ltMsgPk->msgpktype=1;
				ltWebMsgErr(_ltPubInfo->pubMsgStr[8].conStr, confd,ltMsgPk);
				ltMsgFree(ltMsgPk);
				return 0;
			}
			ltMd5Encrypto(strPassword, strlen(strPassword),newPass);
			sprintf(sqlBuf,"update msasadmuser set password='%s' where name='%s' ",newPass,strUserName);
					//printf("%s\n",sqlBuf);
			ltDbExecSql(G_DbCon,sqlBuf); 
			sprintf(caMsg,"change %s password success.",strUserName);
			logWrite(ltMsgPk,caMsg,2);
	}else if(actiontype==5){/*change ip*/
			strUserName=ltMsgGetVar_s(ltMsgPk,"logname");
			bindip=ltMsgGetVar_s(ltMsgPk,"bindip");
			sprintf(sqlBuf,"update msasadmuser set bindip='%s'where name='%s' ",bindip,strUserName);
		  //printf("%s\n",sqlBuf);
			ltDbExecSql(G_DbCon,sqlBuf); 
			//char caMsg[256];
			sprintf(caMsg,"update %s ip success.",strUserName);
			logWrite(ltMsgPk,caMsg,2);
	}
	
	msasAdminUserList(confd,ltMsgPk,lt_MMHead);
	return 0;

}


int msasAdmuserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	
	
   stralloc strTemp;
   int  allcount;
   int  i,k;
   char caTmpp[512];
   
   allcount=0;
   for(i=0;i<20;i++){
  	if( (_ltPubInfo->admSession[i].lSip>0) && ((time(0)-_ltPubInfo->admSession[i].lasttime)<7200)  ){
  			allcount++;   
  	}
  }
   
   strTemp.s=0;
   sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",allcount);
   stralloc_cats(&strTemp,caTmpp);
   k=0;

   for(i=0;i<20;i++){
  	if( (_ltPubInfo->admSession[i].lSip>0) && ((time(0)-_ltPubInfo->admSession[i].lasttime) < 7200)  ){
  			struct in_addr s;
  			char   xStatus[24];
  			s.s_addr = htonl(_ltPubInfo->admSession[i].lSip);
  			if(strcmp(_ltPubInfo->admSession[i].lright,"0000000000000000000000000000000000000000000")==0){
		  		sprintf(xStatus,"%s","Trying");
		  	}else{
		  		sprintf(xStatus,"%s","Success");
		    }
  			if(k==0){
	     	  	 sprintf(caTmpp,"{\"xindex\":\"%d\",\"ip\":\"%s\",\"lasttime\":\"%s\",\"trytime\":\"%d\",\"status\":\"%s\"}",
	     	  	 i,inet_ntoa(s),ltTimeFormat("%Y-%m-%d %H:%M:%S",_ltPubInfo->admSession[i].lasttime),_ltPubInfo->admSession[i].trytimes,xStatus );
	     	     stralloc_cats(&strTemp,caTmpp);
	     	}else{
	     	  	 sprintf(caTmpp,",{\"xindex\":\"%d\",\"ip\":\"%s\",\"lasttime\":\"%s\",\"trytime\":\"%d\",\"status\":\"%s\"}",
	     	  	 i,inet_ntoa(s),ltTimeFormat("%Y-%m-%d %H:%M:%S",_ltPubInfo->admSession[i].lasttime),_ltPubInfo->admSession[i].trytimes,xStatus );
	     	     stralloc_cats(&strTemp,caTmpp);
	     	}
	     	k++; 
  	}
  }
       		
       
   stralloc_cats(&strTemp,"]}");
	 stralloc_0(&strTemp);
	 
   ltMsgPk->msgpktype=1;
	 lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: application/json\n","lthtml",LT_TYPE_STRING,strTemp.s);
    
	 ltMsgFree(ltMsgPk);
	 return 0;
}

int msasCheckOutAdmin(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	

	int  iindex;
	char strIndex[12];

	memset(strIndex,0,sizeof(strIndex));
	
  ltMsgGetSomeNVar(ltMsgPk,1,"xindex", LT_TYPE_STRING, 12, strIndex);
	iindex=atol(strIndex);
	
	if(iindex<20 && iindex>-1 ){
	     _ltPubInfo->admSession[iindex].lSip=0;
			_ltPubInfo->admSession[iindex].trytimes=0;
		  _ltPubInfo->admSession[iindex].lSid=0;
		  sprintf(_ltPubInfo->admSession[iindex].lright,"%s","0000000000000000000000000000000000000000000");		   
		  
  }
	
  ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/plain\n","lthtml",LT_TYPE_STRING,"SUCCESS");
    
	ltMsgFree(ltMsgPk);
	return 0;

}

/* 网站访问查询  */
int msasAdmlog(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead)
{
    char strBuf[1024];
    ltDbCursor *tempCursor;
    LT_DBROW tempRow;
    char caTmpp[512];
    
    long lRowNum,lStartRec,lSumRec,lCount,lNum;
    int k;

    char *caName;
    char *caSubject;
    char *caloglevel;
    char *cadtype;
    char *casdate;
    char *caedate;
    char *casdate3;
    char *castime;
    char *caetime;
    char caWhere[512];
    stralloc strTemp;
    char *exportaction;
    
    caName=ltMsgGetVar_s(ltMsgPk,"name");
    caSubject=ltMsgGetVar_s(ltMsgPk,"subject");
    caloglevel=ltMsgGetVar_s(ltMsgPk,"loglevel");
    cadtype=ltMsgGetVar_s(ltMsgPk,"dtype");
    casdate=ltMsgGetVar_s(ltMsgPk,"sdate");
    caedate=ltMsgGetVar_s(ltMsgPk,"edate");
    casdate3=ltMsgGetVar_s(ltMsgPk,"sdate3");
    castime=ltMsgGetVar_s(ltMsgPk,"stime");
    caetime=ltMsgGetVar_s(ltMsgPk,"etime");
    exportaction=ltMsgGetVar_s(ltMsgPk,"export");
    if(!exportaction){
    	exportaction="";
    }
		memset(caWhere,0,sizeof(caWhere));
		sprintf(caWhere,"%s"," 1=1 ");
		
    if(caName && caName[0] ){
    	sprintf(caWhere,"%s and name like '%c%s%c'",caWhere,'%',caName,'%');
    }
    if(caSubject && caSubject[0] ){
    	sprintf(caWhere,"%s and subject like '%c%s%c'",caWhere,'%',caSubject,'%');
    }
    if(caloglevel && caloglevel[0] && caloglevel[0]!='0' ){
    	sprintf(caWhere,"%s and  loglevel = '%s'",caWhere,caloglevel);
    }
    if(cadtype==NULL){
    	cadtype="0";
    }
    if( cadtype[0]=='1' ){
    	if(casdate && casdate[0] ){
	    	sprintf(caWhere,"%s and  sdate >= '%s 00:00:00'",caWhere,casdate);
	    }
    	if(caedate && caedate[0] ){
	    	sprintf(caWhere,"%s and  sdate <= '%s 00:00:00'",caWhere,caedate);
	    }
    }else if( cadtype[0]=='2' ){
    	if(casdate3 && casdate3[0] && castime && castime[0] ){
	    	sprintf(caWhere,"%s and  sdate >= '%s %s'",caWhere,casdate3,castime);
	    }
    	if(casdate3 && casdate3[0] && caetime && caetime[0] ){
	    	sprintf(caWhere,"%s and  sdate <= '%s %s'",caWhere,casdate3,caetime);
	    }
    }
    if(strcmp(exportaction,"1")==0){/*导出到excel*/
    	  char caFileName[128];
//	    char caHead[1024];
//	    sprintf(caHead,"%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n",
//	    "Pragma: public",
//	    "Expires: 0",
//	    "Cache-Control: must-revalidate, post-check=0, pre-check=0",
//	    "Content-Type: application/force-download",
//	    "Content-type: application/vnd.ms-excel",
//	    "Content-Disposition:attachment;filename=export.csv"); 
       sprintf(caFileName,"admlogexport-%d.csv",time(0));
	     k=0;
	     ltMsgPk->msgpktype=2;
			 lt_TcpResponse(confd,ltMsgPk,2,"filename",LT_TYPE_STRING,caFileName,"contenttype","Application/MS-DOWNLOAD");
		   sprintf(strBuf,"select name,sdate,loglevel,subject from msasadmlog where %s ",caWhere);
		   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		   if(tempCursor!=NULL){
					tempRow= ltDbFetchRow(tempCursor);
				  while(tempRow!=NULL){		
			     	sprintf(caTmpp,"%s\t%s\t%s\t%s\r\n",tempRow[0],tempRow[1],tempRow[2],tempRow[3]);	
			     	write(confd,caTmpp,strlen(caTmpp));
						//printf("caTmpp:%s\n",caTmpp);
						tempRow= ltDbFetchRow(tempCursor);
						k++;
						if(k>50000){
							break;
						}
					}
		     ltDbCloseCursor(tempCursor);
		   }
       return 0;
    }
    lRowNum=50; /*每页的行数*/
    lSumRec=0;/*总行数*/
    lCount=0; 
    lStartRec=0;
    lNum=0; /*该页剩余的行数*/
    if(ltMsgGetVar_s(ltMsgPk,"limit")){
    	  //printf("%s\n",ltMsgGetVar_s(ltMsgPk,"limit"));
    		lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"limit"));
    }
    if(ltMsgGetVar_s(ltMsgPk,"start")){
    	  //printf("%s\n",ltMsgGetVar_s(ltMsgPk,"start"));
    		lStartRec=atol(ltMsgGetVar_s(ltMsgPk,"start"));
    }
    sprintf(strBuf,"select count(*) from msasadmlog where %s ",caWhere);

		tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
		if(tempCursor!=NULL){
				tempRow= ltDbFetchRow(tempCursor);
				if(tempRow!=NULL){
					lCount=atol(tempRow[0]);
				}
		}
		ltDbCloseCursor(tempCursor);
   
    strTemp.s=0;
    sprintf(caTmpp,"{\"totalCount\":\"%d\",\"Results\":[",lCount);
    stralloc_cats(&strTemp,caTmpp);

   k=0;
   sprintf(strBuf,"select name,sdate,loglevel,subject from msasadmlog where %s limit %lu offset %lu ",caWhere,lRowNum,lStartRec);
printf("strBuf:%s\n",strBuf);
   tempCursor=ltDbOpenCursor(G_DbCon,strBuf);
   if(tempCursor!=NULL){
			tempRow= ltDbFetchRow(tempCursor);
		  while(tempRow!=NULL){		
		  	 if(k==0){
	     	  	sprintf(caTmpp,"{\"name\":\"%s\",\"date\":\"%s\",\"level\":\"%s\",\"action\":\"%s\"}",
	     	  	 tempRow[0],tempRow[1],tempRow[2],tempRow[3]);
	     	 		stralloc_cats(&strTemp,caTmpp);	
	     	 }else{
	     	  	sprintf(caTmpp,",{\"name\":\"%s\",\"date\":\"%s\",\"level\":\"%s\",\"action\":\"%s\"}",
	     	  	 tempRow[0],tempRow[1],tempRow[2],tempRow[3]);
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

int vpnonlineuser(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){

  ltDbHeadPtr dbPtr;  
  char caFile1[256];
	char caLabel[256]; 
	char *lt_page_content,*lt_page_content2;
	char caTempDir[256];
  int  iFd;
	char *caCurpg; 
  char caTotlePage[20],sCurPage[20];
  long lCurPage;
  long lRowNum,lCount,lTotPg;
	char allcount[24];
 
	char inbuf[65568];
	char linebuf[65568];
	int  iCount;
	int  fd;
	int  inum;
	int  lastnum;
	int  bnum;
	int  endnum;
	int  nownum;
  
	char *pppp;
	char seps[]=",";
	char *token;
	char flowInfo1[900000];
	char flowInfodata1[30000];
	char *pp1;
	int iIndex=0;
	int xIndex=0;
	int totalnum; 

	
  dbPtr=lt_dbinit();  
  lt_db_htmlpage(dbPtr,"utf-8");

 
  sprintf(caTempDir,"/app/vpnserver/");
  lt_page_content=ltPltFileParse("/app/vpnserver/sbin/vpn-status.log",dbPtr->doc,0); 
  if(lt_page_content==NULL){
			sprintf(caLabel,"/bin/echo '%s' > %serrmsg.ini","无数据,无法生成文件",caTempDir);
    	system(caLabel);//建立目录和正在处理标志 
    	ltMsgFree(ltMsgPk);
    	lt_dbfree(dbPtr);
      return 0;
	 }else{ 
	     lt_page_content=strstr(lt_page_content,"Virtual");
		   lt_page_content2=strstr(lt_page_content,"GLOBAL");
	     sprintf(caLabel,"%sonlineuser.xml",caTempDir);
			
		   iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); printf("%s\n",caLabel);
		  		printf("%d\n",iFd);
       if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content)-strlen(lt_page_content2));    
       }
       close(iFd);
  }

  sprintf(caFile1,"/app/vpnserver/onlineuser.xml");   
  fd=open(caFile1,O_RDONLY | O_NDELAY); 
  if(fd<1){
        ltMsgPk->msgpktype=1;
	 	    lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"");
		    ltMsgFree(ltMsgPk); 
		    lt_dbfree(dbPtr);
		    return 0;
  }
  totalnum=0;
  iCount=read(fd,inbuf,65568); 
  while(iCount){ 
     for(inum=0;inum<iCount;inum++){
     		if(inbuf[inum]=='\n'){
     		   totalnum++; 
     		}
     }
     iCount=read(fd,inbuf,65568);
  } 
  close(fd);
  sprintf(allcount,"%d",totalnum);
  lRowNum=50;
   
 
  if(ltMsgGetVar_s(ltMsgPk,"pagenum")){
		lRowNum=atol(ltMsgGetVar_s(ltMsgPk,"pagenum"));//每页行
	}
	 

  lCount=atol(allcount)-1;   //总行数
  printf("allcount:%d\n",lCount);
 
  caCurpg=ltMsgGetVar_s(ltMsgPk,"CurPg"); 
  if(caCurpg==NULL){
			lCurPage = 1;
  }else if( ltStrIsSpaces(caCurpg) ) {
        	lCurPage = 1;
  }else {
       		lCurPage = atol(caCurpg);
  } 
 
                
	bnum=(lCurPage-1)*lRowNum;
	if(lCurPage==1){
		bnum=1;
	}
  endnum=bnum+lRowNum; 
  lTotPg = (lCount - 1) / lRowNum + 1;
	sprintf(caTotlePage,"%ld",lTotPg);
	sprintf(sCurPage,"%ld",lCurPage);
  printf("bnum:::%d;endnum:::%d\n",bnum,endnum);    
  memset(linebuf,0,sizeof(linebuf));
  memset(flowInfo1,0,sizeof(flowInfo1));
  memset(flowInfodata1,0,sizeof(flowInfodata1));
  pppp=linebuf;
	
  fd=open(caFile1,O_RDONLY | O_NDELAY); 
  if(fd<1){
    ltMsgPk->msgpktype=1;
	 	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,"");
		ltMsgFree(ltMsgPk); 
		lt_dbfree(dbPtr);
		return 0;
  }
  nownum=0;
  iCount=read(fd,inbuf,65568); 
  pp1=flowInfo1;
  pp1=strcat(pp1,"[");    
  while(iCount){
     lastnum=0;
     for(inum=1;inum<iCount;inum++){
     		if(inbuf[inum]=='\n'){
     		   nownum++;
     		   if(nownum>bnum){
     		     memcpy(pppp,&inbuf[lastnum],inum-lastnum-1);
				     xIndex++; 
				     token=strtok(linebuf, seps); 
			 
 				     if(xIndex==1){ 
                    	pp1=strcat(pp1,"[");  
                     }else{
                		pp1=strcat(pp1,",[");  
                     }

		  			 while(token!=NULL){
			  	 			 iIndex++;   
			  	 			 if(iIndex==1){
			          		 	sprintf(flowInfodata1,"'%s'",token);
			          			pp1=strcat(pp1,flowInfodata1);
			          
			        	      }else{
					 			    sprintf(flowInfodata1,",'%s' ",token);
					  			    pp1=strcat(pp1,flowInfodata1); 
							   } //printf("aa:%s\n",pp1);
  	             token  = strtok( NULL, seps);
              } 
                pp1=strcat(pp1,"]");  
                iIndex=0;
     		   }
     		   if(nownum==endnum){
     		   	 break;
     		   }
     		   lastnum=inum+1;
     		}	
     		memset(linebuf,0,sizeof(linebuf));
     }
     if(nownum>=endnum){
     		   	break;
     }
     iCount=read(fd,inbuf,8192);
  } 
  close(fd);
   
  pp1=strcat(pp1,"]"); 
  //printf("%s\n",flowInfo1);

  ltMsgPk->msgpktype=3; 
 	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html; charset=utf-8\n","lthtml",LT_TYPE_STRING,flowInfo1); 
	ltMsgFree(ltMsgPk); 
	lt_dbfree(dbPtr);  
	return 0;
 	   
	
}

int tovpnsetting(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	ltDbHeadPtr dbPtr; 
	char caFile[256];
 	char port[128];
	char ip[32];
	char ipmask[32];

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"utf-8");
	sprintf(caFile,"/etc/msa/msas/vpnserver/settings"); 
  memset(port,0,sizeof(port));
  memset(ip,0,sizeof(ip));
  memset(ipmask,0,sizeof(ipmask));
  

  bcCnfGetValue_s(caFile,"port",port);  
  bcCnfGetValue_s(caFile,"ip",ip);  
  bcCnfGetValue_s(caFile,"ipmask",ipmask);
	

  lt_dbput_rootvars(dbPtr,1,"port",port);  
	
	lt_dbput_rootvars(dbPtr,1,"ip",ip);
	lt_dbput_rootvars(dbPtr,1,"ipmask",ipmask); 
	
  lt_dbput_html(confd,ltMsgPk,dbPtr->doc,"/app/msa/msas/htmlplt/manager/vpnsetting.htm",dbPtr->head,0);
   
	ltMsgFree(ltMsgPk);
 
	lt_dbfree(dbPtr);
 
	return 0;

}

int vpnsetting(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
 
  char caLabel[256]; 
  char localfile[256];
  ltDbHeadPtr dbPtr;
  char *lt_page_content;
  int  iFd;

  char portlabel[256];
  char severlabel[256];
  char addresspool[512];

 	char port[128];
	
	char ip[32];
	char ipmask[32];
	

  memset(port,0,sizeof(port));
  memset(ip,0,sizeof(ip));
  memset(ipmask,0,sizeof(ipmask));
  
  
  if(ltMsgGetVar_s(ltMsgPk,"port")){
  	sprintf(port,"%s",ltMsgGetVar_s(ltMsgPk,"port"));
  }
  if(ltMsgGetVar_s(ltMsgPk,"ip")){
  	sprintf(ip,"%s",ltMsgGetVar_s(ltMsgPk,"ip"));
  }
  if(ltMsgGetVar_s(ltMsgPk,"ipmask")){
  	sprintf(ipmask,"%s",ltMsgGetVar_s(ltMsgPk,"ipmask"));
  }
  
  memset(portlabel,0,sizeof(portlabel));
  memset(severlabel,0,sizeof(severlabel));
  memset(addresspool,0,sizeof(addresspool));
 

  
  sprintf(portlabel,"port %s",port); 
  sprintf(addresspool,"server %s %s",ip,ipmask); 

  
 sprintf(localfile,"/etc/msa/msas/vpnserver/settings");
    
 sprintf(caLabel,"/bin/echo 'port=%s' > %s",port,localfile); 
 system(caLabel);  
 sprintf(caLabel,"/bin/echo 'ip=%s' >> %s",ip,localfile); 
 system(caLabel);  
 sprintf(caLabel,"/bin/echo 'ipmask=%s' >> %s",ipmask,localfile); 
 system(caLabel);
 
	dbPtr=lt_dbinit();
  lt_dbput_rootvars(dbPtr,1,"portlabel",portlabel);  
	lt_dbput_rootvars(dbPtr,1,"addresspool",addresspool);

  lt_page_content=ltPltFileParse("/app/msa/msas/htmlplt/manager/vpnserverplt.htm",dbPtr->doc,0); 
  if(lt_page_content==NULL){
    		ltMsgFree(ltMsgPk);
    		lt_dbfree(dbPtr);
        return 0;
	 }else{ 
	       lt_page_content=strstr(lt_page_content,"#");
	       sprintf(caLabel,"/app/vpnserver/sbin/server.conf");
		     iFd = open(caLabel,O_WRONLY | O_NDELAY | O_TRUNC | O_CREAT,0644); 
         if(iFd >0) {
            write(iFd,lt_page_content,strlen(lt_page_content));    
         }
         close(iFd);
  }
  tovpnsetting(confd,ltMsgPk,lt_MMHead);
  ltMsgFree(ltMsgPk);
  lt_dbfree(dbPtr);
  return 0;
}
