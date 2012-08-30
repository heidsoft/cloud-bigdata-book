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
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <asm/types.h>
#include <dirent.h>
#include <stdarg.h>
#include <dlfcn.h>

#include "msa.h"

int ltsSysCheck(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){

	
	ltMsgPk->msgpktype=1;
	
	  //ltWebMsgErr(strGmsg[atol(langsel)][atol(stylesel)][4], confd,ltMsgPk);
	ltWebMsgErr("Can't find Function", confd,ltMsgPk);
	  
	ltMsgFree(ltMsgPk);

	return 0;

}

int ltsSysRunCheck(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){

	
	ltMsgPk->msgpktype=1;
	lt_TcpResponse(confd,ltMsgPk,2,"lthead",LT_TYPE_STRING,"Content-type: text/html\n","lthtml",LT_TYPE_STRING,"1");

	ltMsgFree(ltMsgPk);
	return 0;

}

int ltactivesys(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead){
	ltMsgPk->msgpktype=1;
	
	  //ltWebMsgErr(strGmsg[atol(langsel)][atol(stylesel)][4], confd,ltMsgPk);
	ltWebMsgErr("Please contact us to active your system", confd,ltMsgPk);
	  
	ltMsgFree(ltMsgPk);

	return 0;

}


int ltSetCoreFunListN(char *strFunName, int (*op)()){

	int i;
	for(i=0;i<NAS_MAX_COREFUNNUM;i++){
		if(_ltcorefunList[i].op==NULL){
			snprintf(_ltcorefunList[i].strFunName,63,"%s",strFunName);
			_ltcorefunList[i].op=op;
			break;
		}
	}
  return 0;
}


int ltCoreFunListInit(){
	
  unsigned int i;
	for(i=0;i<NAS_MAX_COREFUNNUM;i++){
		memset(_ltcorefunList[i].strFunName,0,64);
		_ltcorefunList[i].op=NULL;
	}
	i=0;
  snprintf(_ltcorefunList[i].strFunName,63,"%s","ltsSysCheck");
	_ltcorefunList[i].op=ltsSysCheck;
	i++;
	snprintf(_ltcorefunList[i].strFunName,63,"%s","ltsSysRunCheck");
	_ltcorefunList[i].op=ltsSysRunCheck;
	
	
	return 0;	
}


int setAllCoreFun(){
	
	
  /*srv-admin.c*/
	ltSetCoreFunListN("msaShowAdminLogon",msaShowAdminLogon);
	ltSetCoreFunListN("msaAdminLogon",msaAdminLogon);
	ltSetCoreFunListN("msaAdminLogout",msaAdminLogout);
	ltSetCoreFunListN("msaShowTopview",msaShowTopview);
	ltSetCoreFunListN("checkTimeOut",checkTimeOut);
	ltSetCoreFunListN("checkoutShowpage",checkoutShowpage);
	
	ltSetCoreFunListN("groupmanager",groupmanager);
	ltSetCoreFunListN("torolemanager",torolemanager);
	ltSetCoreFunListN("roleedit",roleedit);
	ltSetCoreFunListN("roleupdate",roleupdate);
	ltSetCoreFunListN("toroleadd",toroleadd);
	ltSetCoreFunListN("roleadd",roleadd);
	ltSetCoreFunListN("toadminadd",toadminadd);
	ltSetCoreFunListN("adminadd",adminadd);
	ltSetCoreFunListN("ltadminlist",ltadminlist);
	ltSetCoreFunListN("adminlist",adminlist);
	ltSetCoreFunListN("adminedit",adminedit);
	ltSetCoreFunListN("adminupdate",adminupdate);
	ltSetCoreFunListN("sysconfshow",sysconfshow);
	ltSetCoreFunListN("sysconfset",sysconfset);
	/*srv-manager.c*/
    ltSetCoreFunListN("msaAdminUserList",msaAdminUserList);
	ltSetCoreFunListN("msaAdminAction",msaAdminAction);
	ltSetCoreFunListN("msaAdmuserList",msaAdmuserList);
	ltSetCoreFunListN("msaCheckOutAdmin",msaAdminAction);
	ltSetCoreFunListN("msaAdmlog",msaAdmlog);
	ltSetCoreFunListN("msaSysReset",msaSysReset);
	ltSetCoreFunListN("msaSysShutdown",msaSysShutdown);
	
	ltSetCoreFunListN("ltnetareaaction",ltnetareaaction);
	ltSetCoreFunListN("ltnetarealist",ltnetarealist);
	ltSetCoreFunListN("ltapplynetArea",ltapplynetArea);
	
	ltSetCoreFunListN("ltportinfolist",ltportinfolist);
	ltSetCoreFunListN("ltportinfoaction",ltportinfoaction);
	ltSetCoreFunListN("ltapplyportinfo",ltapplyportinfo);
	
	ltSetCoreFunListN("ltappFunctionok",ltappFunctionok);
	ltSetCoreFunListN("ltappFunction",ltappFunction);
    ltSetCoreFunListN("msamailGroup",msamailGroup);
	ltSetCoreFunListN("ltmailfilteraction",ltmailfilteraction);
	ltSetCoreFunListN("ltmailfilterlist",ltmailfilterlist);
	ltSetCoreFunListN("msaShowMail",msaShowMail);
	ltSetCoreFunListN("msaformGroup",msaformGroup);
	ltSetCoreFunListN("ltformfilteraction",ltformfilteraction);
    ltSetCoreFunListN("ltformfilterlist",ltformfilterlist);
	ltSetCoreFunListN("msaShowForm",msaShowForm);
	
	ltSetCoreFunListN("msaformOneGroup",msaformOneGroup);
	ltSetCoreFunListN("ltformOnefilteraction",ltformOnefilteraction);
    ltSetCoreFunListN("ltformOnefilterlist",ltformOnefilterlist);
	ltSetCoreFunListN("msaShowFormOne",msaShowFormOne);

	ltSetCoreFunListN("ltschinfolist",ltschinfolist);
    ltSetCoreFunListN("ltaddsch",ltaddsch);
	ltSetCoreFunListN("ltupdatesch",ltupdatesch);
	ltSetCoreFunListN("ltdelsch",ltdelsch);
	ltSetCoreFunListN("ltjjrinfolist",ltjjrinfolist);
	ltSetCoreFunListN("ltaddjjr",ltaddjjr);
	ltSetCoreFunListN("ltupdatejjr",ltupdatejjr);
  ltSetCoreFunListN("ltdeljjr",ltdeljjr);
  ltSetCoreFunListN("lttimesetting",lttimesetting);
	ltSetCoreFunListN("lttimeok",lttimeok);
	ltSetCoreFunListN("lttimeset",lttimeset);
	ltSetCoreFunListN("ltonlinecheck",ltonlinecheck);
	ltSetCoreFunListN("msaRout",msaRout);
	ltSetCoreFunListN("ltformlevelok",ltformlevelok);
	ltSetCoreFunListN("ltformlevel",ltformlevel);
	ltSetCoreFunListN("ltmaillevelok",ltmaillevelok);
    ltSetCoreFunListN("ltmaillevel",ltmaillevel);
    ltSetCoreFunListN("creatMsakey",creatMsakey);
  
    ltSetCoreFunListN("addsysconf",addsysconf);
    ltSetCoreFunListN("policytemplateJsonStore",policytemplateJsonStore);
    ltSetCoreFunListN("setsysconf",setsysconf);
    ltSetCoreFunListN("viewsysconf",viewsysconf);
    ltSetCoreFunListN("defaultfile",defaultfile);
  
	//srv-rule.c
	
	ltSetCoreFunListN("msaRuleSrvTree",msaRuleSrvTree);
	ltSetCoreFunListN("msaUpPrio",msaUpPrio);
	ltSetCoreFunListN("msaDownPrio",msaDownPrio);
	ltSetCoreFunListN("showServicePolicy",showServicePolicy);
	ltSetCoreFunListN("showEditRuleSrv",showEditRuleSrv);
	ltSetCoreFunListN("msaEditRuleSrv",msaEditRuleSrv);
	ltSetCoreFunListN("addRuleSrv",addRuleSrv);
	ltSetCoreFunListN("showRuleSrv",showRuleSrv);
	ltSetCoreFunListN("showService",showService);
    ltSetCoreFunListN("addService",addService);
	ltSetCoreFunListN("showEditService",showEditService);
	ltSetCoreFunListN("msaEditSrvRule",msaEditSrvRule);
	
	ltSetCoreFunListN("msaSrvRuleList",msaSrvRuleList);
	ltSetCoreFunListN("msaSrvRulesrvList",msaSrvRulesrvList);
	ltSetCoreFunListN("msaEditWebpost",msaEditWebpost);
	ltSetCoreFunListN("msaEditWeburl",msaEditWeburl);
	ltSetCoreFunListN("msaEditip",msaEditip);
	
	ltSetCoreFunListN("ShowSelectUserTree",ShowSelectUserTree);
	ltSetCoreFunListN("ruleGroupUserlist",ruleGroupUserlist);
	ltSetCoreFunListN("serviceUserlist",serviceUserlist);
	ltSetCoreFunListN("addruleuser",addruleuser);
	
	ltSetCoreFunListN("ShowUrlSort",ShowUrlSort);
	ltSetCoreFunListN("ShowAddress",ShowAddress);
    //ltSetCoreFunListN("editkeyword",editkeyword);
	
	ltSetCoreFunListN("msaltServiceTree",msaltServiceTree);
	//ltSetCoreFunListN("msatopSrvTreeView",msatopSrvTreeView);
	//ltSetCoreFunListN("msaSrvTree",msaSrvTree);
	ltSetCoreFunListN("lturlsort",lturlsort);
	ltSetCoreFunListN("lturlsortlist",lturlsortlist);
	ltSetCoreFunListN("ltupdateurlsort",ltupdateurlsort);
	ltSetCoreFunListN("ltupdateurlType",ltupdateurlType);
	ltSetCoreFunListN("ltsaveurlsort",ltsaveurlsort);
	ltSetCoreFunListN("msaUpdateType",msaUpdateType);
	ltSetCoreFunListN("msaUpdateIP",msaUpdateIP);
	ltSetCoreFunListN("msaUpdateArea",msaUpdateArea);
	ltSetCoreFunListN("ltreseturlsort",ltreseturlsort);
	ltSetCoreFunListN("msaOneServiceTree",msaOneServiceTree);
	ltSetCoreFunListN("msaTwoServiceTree",msaTwoServiceTree);
	ltSetCoreFunListN("msaSMSsend",msaSMSsend);
	
	ltSetCoreFunListN("msaalertLink",msaalertLink);
	ltSetCoreFunListN("msaalertset",msaalertset);

	//srv-ruleSimp.c
  ltSetCoreFunListN("showServiceSimp",showServiceSimp);
  ltSetCoreFunListN("showRuleSrvSimp",showRuleSrvSimp);
  ltSetCoreFunListN("showEditServiceSimp",showEditServiceSimp);
  ltSetCoreFunListN("showUserPolicy",showUserPolicy);
  ltSetCoreFunListN("showPolicyTemplate",showPolicyTemplate);
  /*srv-userpolicy.c*/
	ltSetCoreFunListN("msaListUserPolicy",msaListUserPolicy);
  ltSetCoreFunListN("msaUpdatePrio",msaUpdatePrio);
  ltSetCoreFunListN("msaAddPrio",msaAddPrio);
  ltSetCoreFunListN("msaEditPrio",msaEditPrio);
  ltSetCoreFunListN("showPolicyApply",showPolicyApply);
  
  ltSetCoreFunListN("msaGroupToPageTreeView",msaGroupToPageTreeView);
  ltSetCoreFunListN("msaPiLiangAppPolicy",msaPiLiangAppPolicy);
  ltSetCoreFunListN("msaPolicyToPageTree",msaPolicyToPageTree);
  ltSetCoreFunListN("msaServiceToPageTree",msaServiceToPageTree);
  /*srv-nowview.c*/
  ltSetCoreFunListN("msaNowSrvList",msaNowSrvList);
  ltSetCoreFunListN("msashowxtzy",msashowxtzy);
  
  ltSetCoreFunListN("msaNowGroupList",msaNowGroupList);
  ltSetCoreFunListN("msaGroupTreeView",msaGroupTreeView);
  
  ltSetCoreFunListN("msaNowQosList",msaNowQosList);
  ltSetCoreFunListN("msaQosDetailList",msaQosDetailList);
  
  ltSetCoreFunListN("msaNowSrvruleList",msaNowSrvruleList);
  ltSetCoreFunListN("msaToSrvRule",msaToSrvRule);
  ltSetCoreFunListN("msaRuleHtml",msaRuleHtml);
  
  ltSetCoreFunListN("msaNowPathList",msaNowPathList);
  ltSetCoreFunListN("msaPathDetail",msaPathDetail);
  ltSetCoreFunListN("msaPathDetailList",msaPathDetailList);
  
  ltSetCoreFunListN("msa24qushi",msa24qushi);
  
  ltSetCoreFunListN("msaGroupDetail",msaGroupDetail);
  ltSetCoreFunListN("msaGroupDetailList",msaGroupDetailList);
  
 
  /*srv-nowconnect.c*/
  ltSetCoreFunListN("ltconquery",ltconquery);
  ltSetCoreFunListN("msaNowConectInfo",msaNowConectInfo);
  
  /*srv-nowviewuser.c*/
  ltSetCoreFunListN("msaShowOnlineUser",msaShowOnlineUser);
  ltSetCoreFunListN("msaonlineuser",msaonlineuser);

  ltSetCoreFunListN("msaUsersrvPage",msaUsersrvPage);
  ltSetCoreFunListN("msaUserSrvList",msaUserSrvList);
    /*部署*/
  ltSetCoreFunListN("packagelist",packagelist);
  ltSetCoreFunListN("naspackageok",naspackageok);
  ltSetCoreFunListN("ltupsysok",ltupsysok);
  ltSetCoreFunListN("netconnecttype",netconnecttype);
	ltSetCoreFunListN("disposition",disposition);
	ltSetCoreFunListN("sysnet3conf",sysnet3conf);
	ltSetCoreFunListN("sysnet6conf",sysnet6conf);
	ltSetCoreFunListN("sysnetconfok",sysnetconfok);
	/*srv-user.c*/
	ltSetCoreFunListN("msaTermInfoView",msaTermInfoView);
  ltSetCoreFunListN("msaShowAddClient",msaShowAddClient);
  ltSetCoreFunListN("msaTerminfoAdd",msaTerminfoAdd);
  ltSetCoreFunListN("msaTerminfoUp",msaTerminfoUp);

  ltSetCoreFunListN("msaUpdatePwd",msaUpdatePwd);
  ltSetCoreFunListN("msaUpdateGroup",msaUpdateGroup);
  ltSetCoreFunListN("msaGroupInport",msaGroupInport);
  ltSetCoreFunListN("msaServiceTree",msaServiceTree);
  /*srv-group.c*/
	ltSetCoreFunListN("msaGroupAction",msaGroupAction);
	ltSetCoreFunListN("msaPolicyTree",msaPolicyTree);
	ltSetCoreFunListN("msaPolicyAction",msaPolicyAction);
	ltSetCoreFunListN("msaShowClientList",msaShowClientList);
	ltSetCoreFunListN("msaClientList",msaClientList);
	ltSetCoreFunListN("msaGroupTree",msaGroupTree);
	/*srv-report.c*/
	ltSetCoreFunListN("msasortreportlink",msasortreportlink);
	ltSetCoreFunListN("msaReportShowPage",msaReportShowPage);
	ltSetCoreFunListN("msasortreport",msasortreport);
	ltSetCoreFunListN("msaReportJsonStore",msaReportJsonStore);
	ltSetCoreFunListN("msaDataCenetrDownLoad",msaDataCenetrDownLoad);
	ltSetCoreFunListN("msaSummaryReport",msaSummaryReport);
	ltSetCoreFunListN("msacompareFlowAjaxSubmit",msacompareFlowAjaxSubmit);
	/*srv-data-data.c*/
	ltSetCoreFunListN("msaShowAccessList",msaShowAccessList);
	ltSetCoreFunListN("msaAccessList",msaAccessList);
	ltSetCoreFunListN("msaAlertList",msaAlertList);
	ltSetCoreFunListN("msaAccountList",msaAccountList);
  return 0;
}

int nasFindFun(unsigned long lFunCode){
	
//	int i;
	int l=-1;
	int r=_ltPubInfo->maxfuntonnum+1;
	/*for(i=0;i<r;i++){
		if(lFunCode==_ltfunList[i].lFunCode) return i;
	}*/
	
	while(l+1!=r){
		int i=(l+r)/2;
		if(lFunCode<_ltfunList[i].lFunCode) r=i;
		if(lFunCode==_ltfunList[i].lFunCode) return i;
		if(lFunCode>_ltfunList[i].lFunCode) l=i;
	}
	return -1;
}



int ltSetFunListN(char *strFunName, int (*op)()){

	//int i;
	//for(i=0;i<NAS_MAX_FUNNUM;i++){
		  //if(_ltfunList[_ltPubInfo->maxfuntonnum].lFunCode==0){
					_ltfunList[_ltPubInfo->maxfuntonnum].lFunCode=ltMd5Code(strFunName,strlen(strFunName),"LT");
					_ltfunList[_ltPubInfo->maxfuntonnum].funFlag=0;
					sprintf(_ltfunList[_ltPubInfo->maxfuntonnum].strFunName,"%s",strFunName);
					sprintf(_ltfunList[_ltPubInfo->maxfuntonnum].strFunUrl,"%s","");
					_ltfunList[_ltPubInfo->maxfuntonnum].maxruntime=900;
					_ltfunList[_ltPubInfo->maxfuntonnum].activeflag=1;
					_ltfunList[_ltPubInfo->maxfuntonnum].rightflag=0;
					_ltfunList[_ltPubInfo->maxfuntonnum].op=op;
					_ltPubInfo->maxfuntonnum++;
		//			break;
		//	}
	//}
  return 0;

}


int ltSetFunListS(char *strFunName, int (*op)()){

	//int i;
	//for(i=0;i<NAS_MAX_FUNNUM;i++){
		  //if(_ltfunList[_ltPubInfo->maxfuntonnum].lFunCode==0){
					_ltfunList[_ltPubInfo->maxfuntonnum].lFunCode=ltMd5Code(strFunName,strlen(strFunName),"LT");
					_ltfunList[_ltPubInfo->maxfuntonnum].funFlag=0;
					sprintf(_ltfunList[_ltPubInfo->maxfuntonnum].strFunName,"%s",strFunName);
					sprintf(_ltfunList[_ltPubInfo->maxfuntonnum].strFunUrl,"%s","");
					_ltfunList[_ltPubInfo->maxfuntonnum].maxruntime=3600;
					_ltfunList[_ltPubInfo->maxfuntonnum].activeflag=0;
					_ltfunList[_ltPubInfo->maxfuntonnum].rightflag=0;
					_ltfunList[_ltPubInfo->maxfuntonnum].op=op;
					_ltPubInfo->maxfuntonnum++;
		//			break;
		//	}
	//}
  return 0;

}


int ltSetFunListNA(char *strFunName, int (*op)(),unsigned int maxruntime){

	//int i;
	//for(i=0;i<NAS_MAX_FUNNUM;i++){
		  //if(_ltfunList[_ltPubInfo->maxfuntonnum].lFunCode==0){
					_ltfunList[_ltPubInfo->maxfuntonnum].lFunCode=ltMd5Code(strFunName,strlen(strFunName),"LT");
					_ltfunList[_ltPubInfo->maxfuntonnum].funFlag=0;
					sprintf(_ltfunList[_ltPubInfo->maxfuntonnum].strFunName,"%s",strFunName);
					sprintf(_ltfunList[_ltPubInfo->maxfuntonnum].strFunUrl,"%s","");
					_ltfunList[_ltPubInfo->maxfuntonnum].maxruntime=maxruntime;
					_ltfunList[_ltPubInfo->maxfuntonnum].activeflag=1;
					_ltfunList[_ltPubInfo->maxfuntonnum].rightflag=0;
					_ltfunList[_ltPubInfo->maxfuntonnum].op=op;
					_ltPubInfo->maxfuntonnum++;
		//			break;
		//	}
	//}
  return 0;

}

int ltFunListInit(char *funconffile){
	
  xmlDocPtr doc;
	xmlNodePtr  node;
	xmlNodePtr  childnode;
  xmlNodePtr  tempRootNode;
	char *nodeMaxTime;
	char *nodeName;
	char *nodeDesc;
	char *nodeUrl;
	char *nodeType;
	char *nodeActive;
	char *nodeRightCheck;
	uint32 lFunCode;
	unsigned int i;
	unsigned int j;
	funList tmpFunList;


	for(i=0;i<NAS_MAX_FUNNUM;i++){
			_ltfunList[i].lFunCode=0;
			_ltfunList[i].funFlag=0;
			sprintf(_ltfunList[i].strFunName,"%s","");
			sprintf(_ltfunList[i].strFunUrl,"%s","");
			_ltfunList[i].maxruntime=NAS_DEFAULT_FUNTIME;
			_ltfunList[i].op=NULL;
	}
	
			_ltfunList[0].lFunCode=ltMd5Code("ltsSysCheck",strlen("ltsSysCheck"),"LT");;
			_ltfunList[0].funFlag=0;
			sprintf(_ltfunList[0].strFunName,"%s","ltsSysCheck");
			sprintf(_ltfunList[0].strFunUrl,"%s","");
			_ltfunList[0].maxruntime=60;
			_ltfunList[0].op=ltsSysCheck;
			
       _ltfunList[1].lFunCode=ltMd5Code("ltsSysRunCheck",strlen("ltsSysRunCheck"),"LT");;
			_ltfunList[1].funFlag=0;
			sprintf(_ltfunList[1].strFunName,"%s","ltsSysRunCheck");
			sprintf(_ltfunList[1].strFunUrl,"%s","");
			_ltfunList[1].maxruntime=60;
			_ltfunList[1].op=ltsSysRunCheck;

			
			
//	doc=xmlParseFile(funconffile); 
//	if(doc==NULL){
//		return -1;
//	}
	i=2;
//	tempRootNode=xmlDocGetRootElement(doc);
//	for(node = tempRootNode->children; node != NULL; node = node->next) {
//	  if(node->name!=NULL){
//	  	/*begin core fun*/
//			if(!case_diffs(node->name,"corefun")){
//				for(childnode=node->children;childnode!=NULL;childnode=childnode->next){
//					nodeType=childnode->name;
//					nodeName=xmlGetProp(childnode,"name");
//					nodeMaxTime=xmlGetProp(childnode,"maxtime");
//					nodeDesc=xmlGetProp(childnode,"desc");
//					nodeUrl=xmlGetProp(childnode,"url");
//					nodeActive=xmlGetProp(childnode,"activeflag");
//					nodeRightCheck=xmlGetProp(childnode,"rightflag");
//					if(nodeType!=NULL && nodeName!=NULL && nodeMaxTime!=NULL && nodeDesc!=NULL && nodeUrl!=NULL && nodeActive!=NULL && nodeRightCheck!=NULL ){
//							for(j=0;j<NAS_MAX_COREFUNNUM;j++){
//								if(strcmp(_ltcorefunList[j].strFunName,nodeName)==0){
//									  lFunCode=ltMd5Code(nodeName,strlen(nodeName),"LT");
//										_ltfunList[i].lFunCode=lFunCode;
//										_ltfunList[i].funFlag=0;
//										snprintf(_ltfunList[i].strFunName,63,"%s",nodeName);
//										sprintf(_ltfunList[i].strFunUrl,"%s","");
//										_ltfunList[i].maxruntime=atol(nodeMaxTime);
//										_ltfunList[i].rightflag=atol(nodeRightCheck);
//										_ltfunList[i].activeflag=atol(nodeActive);
//										_ltfunList[i].op=_ltcorefunList[j].op;
//										i++;
//										break;
//								}
//							}	
//					}
//				}
//			}
//			/*end core fun*/
//			/*begin sofun*/
//			if(!case_diffs(node->name,"sofun")){
//				for(childnode=node->children;childnode!=NULL;childnode=childnode->next){
//					nodeType=childnode->name;
//					nodeName=xmlGetProp(childnode,"name");
//					nodeMaxTime=xmlGetProp(childnode,"maxtime");
//					nodeDesc=xmlGetProp(childnode,"desc");
//					nodeUrl=xmlGetProp(childnode,"url");
//					nodeActive=xmlGetProp(childnode,"activeflag");
//					nodeRightCheck=xmlGetProp(childnode,"rightflag");
//					if(nodeType!=NULL && nodeName!=NULL && nodeMaxTime!=NULL && nodeDesc!=NULL && nodeUrl!=NULL && nodeActive!=NULL && nodeRightCheck!=NULL ){
//									  lFunCode=ltMd5Code(nodeName,strlen(nodeName),"LT");
//										_ltfunList[i].lFunCode=lFunCode;
//										_ltfunList[i].funFlag=1;
//										snprintf(_ltfunList[i].strFunName,63,"%s",nodeName);
//										snprintf(_ltfunList[i].strFunUrl,127,"%s",nodeUrl);
//										_ltfunList[i].maxruntime=atol(nodeMaxTime);
//										_ltfunList[i].rightflag=atol(nodeRightCheck);
//										_ltfunList[i].activeflag=atol(nodeActive);
//										_ltfunList[i].op=NULL;
//										i++;
//					}
//				}
//			}
//			/*end sofun*/
//			/*begin appfun*/
//			if(!case_diffs(node->name,"appfun")){
//				for(childnode=node->children;childnode!=NULL;childnode=childnode->next){
//					nodeType=childnode->name;
//					nodeName=xmlGetProp(childnode,"name");
//					nodeMaxTime=xmlGetProp(childnode,"maxtime");
//					nodeDesc=xmlGetProp(childnode,"desc");
//					nodeUrl=xmlGetProp(childnode,"url");
//					nodeActive=xmlGetProp(childnode,"activeflag");
//					nodeRightCheck=xmlGetProp(childnode,"rightflag");
//					if(nodeType!=NULL && nodeName!=NULL && nodeMaxTime!=NULL && nodeDesc!=NULL && nodeUrl!=NULL && nodeActive!=NULL && nodeRightCheck!=NULL ){
//									  lFunCode=ltMd5Code(nodeName,strlen(nodeName),"LT");
//										_ltfunList[i].lFunCode=lFunCode;
//										_ltfunList[i].funFlag=3;
//										snprintf(_ltfunList[i].strFunName,63,"%s",nodeName);
//										snprintf(_ltfunList[i].strFunUrl,127,"%s",nodeUrl);
//										_ltfunList[i].maxruntime=atol(nodeMaxTime);
//										_ltfunList[i].rightflag=atol(nodeRightCheck);
//										_ltfunList[i].activeflag=atol(nodeActive);
//										_ltfunList[i].op=NULL;
//										i++;
//					}
//				}
//			}
//			/*end appfun*/
//		}				
//  }
  
  _ltPubInfo->maxfuntonnum=i;
  /*排序 按照lFunCode升序排列*/
	/*srv-admin.c*/  
  ltSetFunListN("msaShowAdminLogon",msaShowAdminLogon);
	ltSetFunListN("msaAdminLogon",msaAdminLogon);
	ltSetFunListN("msaAdminLogout",msaAdminLogout);
	ltSetFunListN("msaShowTopview",msaShowTopview);
	ltSetFunListN("checkTimeOut",checkTimeOut);
	ltSetFunListN("checkoutShowpage",checkoutShowpage);
	
	ltSetFunListN("groupmanager",groupmanager);
	ltSetFunListN("torolemanager",torolemanager);
	ltSetFunListN("roleedit",roleedit);
	ltSetFunListN("roleupdate",roleupdate);
	ltSetFunListN("toroleadd",toroleadd);
	ltSetFunListN("roleadd",roleadd);
	ltSetFunListN("toadminadd",toadminadd);
	ltSetFunListN("adminadd",adminadd);
	ltSetFunListN("ltadminlist",ltadminlist);
	ltSetFunListN("adminlist",adminlist);
	ltSetFunListN("adminedit",adminedit);
	ltSetFunListN("adminupdate",adminupdate);
	ltSetFunListN("sysconfshow",sysconfshow);
	ltSetFunListN("sysconfset",sysconfset);
	/*srv-mamanger.c*/
	ltSetFunListN("msaAdminUserList",msaAdminUserList);
	ltSetFunListN("msaAdminAction",msaAdminAction);
	ltSetFunListN("msaAdmuserList",msaAdmuserList);
	ltSetFunListN("msaCheckOutAdmin",msaAdminAction);
	ltSetFunListN("msaAdmlog",msaAdmlog);
	ltSetFunListN("msaSysReset",msaSysReset);
	ltSetFunListN("msaSysShutdown",msaSysShutdown);
  
  ltSetFunListN("ltnetareaaction",ltnetareaaction);
	ltSetFunListN("ltnetarealist",ltnetarealist);
	ltSetFunListN("ltapplynetArea",ltapplynetArea);
	
	ltSetFunListN("ltportinfolist",ltportinfolist);
	ltSetFunListN("ltportinfoaction",ltportinfoaction);
	ltSetFunListN("ltapplyportinfo",ltapplyportinfo);
	ltSetFunListN("ltappFunctionok",ltappFunctionok);
	ltSetFunListN("ltappFunction",ltappFunction);
  ltSetFunListN("msamailGroup",msamailGroup);
	ltSetFunListN("ltmailfilteraction",ltmailfilteraction);
	ltSetFunListN("ltmailfilterlist",ltmailfilterlist);
	ltSetFunListN("msaShowMail",msaShowMail);
	ltSetFunListN("msaformGroup",msaformGroup);
	ltSetFunListN("ltformfilteraction",ltformfilteraction);
  ltSetFunListN("ltformfilterlist",ltformfilterlist);
	ltSetFunListN("msaShowForm",msaShowForm);
	
	ltSetFunListN("msaformOneGroup",msaformOneGroup);
	ltSetFunListN("ltformOnefilteraction",ltformOnefilteraction);
  ltSetFunListN("ltformOnefilterlist",ltformOnefilterlist);
	ltSetFunListN("msaShowFormOne",msaShowFormOne);
	
	ltSetFunListN("ltschinfolist",ltschinfolist);
  ltSetFunListN("ltaddsch",ltaddsch);
	ltSetFunListN("ltupdatesch",ltupdatesch);
	ltSetFunListN("ltdelsch",ltdelsch);
	ltSetFunListN("ltjjrinfolist",ltjjrinfolist);
	ltSetFunListN("ltaddjjr",ltaddjjr);
	ltSetFunListN("ltupdatejjr",ltupdatejjr);
  ltSetFunListN("ltdeljjr",ltdeljjr);
  ltSetFunListN("lttimesetting",lttimesetting);
	ltSetFunListN("lttimeok",lttimeok);
	ltSetFunListN("lttimeset",lttimeset);
	ltSetFunListN("ltonlinecheck",ltonlinecheck);
	ltSetFunListN("msaRout",msaRout);
	ltSetFunListN("ltformlevelok",ltformlevelok);
	ltSetFunListN("ltformlevel",ltformlevel);
	ltSetFunListN("ltmaillevelok",ltmaillevelok);
  ltSetFunListN("ltmaillevel",ltmaillevel);
  ltSetFunListN("creatMsakey",creatMsakey);
  
  ltSetFunListN("addsysconf",addsysconf);
  ltSetFunListN("policytemplateJsonStore",policytemplateJsonStore);
  ltSetFunListN("setsysconf",setsysconf);
  ltSetFunListN("viewsysconf",viewsysconf);
  ltSetFunListN("defaultfile",defaultfile);
  
  //srv-rule.c
	
	ltSetFunListN("msaRuleSrvTree",msaRuleSrvTree);
	ltSetFunListN("msaUpPrio",msaUpPrio);
	ltSetFunListN("msaDownPrio",msaDownPrio);
	ltSetFunListN("showServicePolicy",showServicePolicy);
	ltSetFunListN("showEditRuleSrv",showEditRuleSrv);
	ltSetFunListN("msaEditRuleSrv",msaEditRuleSrv);
	ltSetFunListN("addRuleSrv",addRuleSrv);
	ltSetFunListN("showRuleSrv",showRuleSrv);
	ltSetFunListN("showService",showService);
  ltSetFunListN("addService",addService);
	ltSetFunListN("showEditService",showEditService);
	ltSetFunListN("msaEditSrvRule",msaEditSrvRule);
	
	ltSetFunListN("msaSrvRuleList",msaSrvRuleList);
	ltSetFunListN("msaSrvRulesrvList",msaSrvRulesrvList);
	ltSetFunListN("msaEditWebpost",msaEditWebpost);
	ltSetFunListN("msaEditWeburl",msaEditWeburl);
	ltSetFunListN("msaEditip",msaEditip);
	
	ltSetFunListN("ShowSelectUserTree",ShowSelectUserTree);
	ltSetFunListN("ruleGroupUserlist",ruleGroupUserlist);
	ltSetFunListN("serviceUserlist",serviceUserlist);
	ltSetFunListN("addruleuser",addruleuser);
	
	ltSetFunListN("ShowUrlSort",ShowUrlSort);
	ltSetFunListN("ShowAddress",ShowAddress);
  //ltSetFunListN("editkeyword",editkeyword);
	
	ltSetFunListN("msaltServiceTree",msaltServiceTree);
	//ltSetFunListN("msatopSrvTreeView",msatopSrvTreeView);
	//ltSetFunListN("msaSrvTree",msaSrvTree);
	ltSetFunListN("lturlsort",lturlsort);
	ltSetFunListN("lturlsortlist",lturlsortlist);
	ltSetFunListN("ltupdateurlsort",ltupdateurlsort);
	ltSetFunListN("ltupdateurlType",ltupdateurlType);
	ltSetFunListN("ltsaveurlsort",ltsaveurlsort);
	ltSetFunListN("msaUpdateType",msaUpdateType);
	ltSetFunListN("msaUpdateIP",msaUpdateIP);
	ltSetFunListN("msaUpdateArea",msaUpdateArea);
	ltSetFunListN("ltreseturlsort",ltreseturlsort);
	ltSetFunListN("msaOneServiceTree",msaOneServiceTree);
	ltSetFunListN("msaTwoServiceTree",msaTwoServiceTree);
	ltSetFunListN("msaSMSsend",msaSMSsend);
	
	ltSetFunListN("msaalertLink",msaalertLink);
	ltSetFunListN("msaalertset",msaalertset);
	//srv-ruleSimp.c
  ltSetFunListN("showServiceSimp",showServiceSimp);
  ltSetFunListN("showRuleSrvSimp",showRuleSrvSimp);
  ltSetFunListN("showEditServiceSimp",showEditServiceSimp);
  ltSetFunListN("showUserPolicy",showUserPolicy);
  ltSetFunListN("showPolicyTemplate",showPolicyTemplate);
  /*srv-userpolicy.c*/
	ltSetFunListN("msaListUserPolicy",msaListUserPolicy);
  ltSetFunListN("msaUpdatePrio",msaUpdatePrio);
  ltSetFunListN("msaAddPrio",msaAddPrio);
  ltSetFunListN("msaEditPrio",msaEditPrio);
  ltSetFunListN("showPolicyApply",showPolicyApply);
  ltSetFunListN("msaGroupToPageTreeView",msaGroupToPageTreeView);
  ltSetFunListN("msaPiLiangAppPolicy",msaPiLiangAppPolicy);
  ltSetFunListN("msaPolicyToPageTree",msaPolicyToPageTree);
  ltSetFunListN("msaServiceToPageTree",msaServiceToPageTree);
  /*srv-nowview.c*/
  ltSetFunListN("msaNowSrvList",msaNowSrvList);
  ltSetFunListN("msashowxtzy",msashowxtzy);
  
  ltSetFunListN("msaNowGroupList",msaNowGroupList);
  ltSetFunListN("msaGroupTreeView",msaGroupTreeView);
  
  ltSetFunListN("msaNowQosList",msaNowQosList);
  ltSetFunListN("msaQosDetailList",msaQosDetailList);
    
  
  ltSetFunListN("msaNowSrvruleList",msaNowSrvruleList);
  ltSetFunListN("msaToSrvRule",msaToSrvRule);
  ltSetFunListN("msaRuleHtml",msaRuleHtml);
  
  ltSetFunListN("msaNowPathList",msaNowPathList);
  ltSetFunListN("msaPathDetail",msaPathDetail);
  ltSetFunListN("msaPathDetailList",msaPathDetailList);
  
  ltSetFunListN("msa24qushi",msa24qushi);
  
  ltSetFunListN("msaGroupDetail",msaGroupDetail);
  ltSetFunListN("msaGroupDetailList",msaGroupDetailList);
  
  /*srv-nowconnect.c*/
  ltSetFunListN("ltconquery",ltconquery);
  ltSetFunListN("msaNowConectInfo",msaNowConectInfo);
  
  /*srv-nowviewuser.c*/
  ltSetFunListN("msaShowOnlineUser",msaShowOnlineUser);
  ltSetFunListN("msaonlineuser",msaonlineuser);

  ltSetFunListN("msaUsersrvPage",msaUsersrvPage);
  ltSetFunListN("msaUserSrvList",msaUserSrvList);
  /*部署*/
  ltSetFunListN("packagelist",packagelist);
  ltSetFunListN("naspackageok",naspackageok);
  ltSetFunListN("ltupsysok",ltupsysok);
  ltSetFunListN("netconnecttype",netconnecttype);
	ltSetFunListN("disposition",disposition);
	ltSetFunListN("sysnet3conf",sysnet3conf);
	ltSetFunListN("sysnet6conf",sysnet6conf);
	ltSetFunListN("sysnetconfok",sysnetconfok);
  /*srv-user.c*/
	ltSetFunListN("msaTermInfoView",msaTermInfoView);
  ltSetFunListN("msaShowAddClient",msaShowAddClient);
  ltSetFunListN("msaTerminfoAdd",msaTerminfoAdd);
  ltSetFunListN("msaTerminfoUp",msaTerminfoUp);

  ltSetFunListN("msaUpdatePwd",msaUpdatePwd);
  ltSetFunListN("msaUpdateGroup",msaUpdateGroup);
  ltSetFunListN("msaGroupInport",msaGroupInport);
  ltSetFunListN("msaServiceTree",msaServiceTree);
  /*srv-group.c*/
	ltSetFunListN("msaGroupAction",msaGroupAction);
	ltSetFunListN("msaPolicyTree",msaPolicyTree);
	ltSetFunListN("msaPolicyAction",msaPolicyAction);
	ltSetFunListN("msaShowClientList",msaShowClientList);
	ltSetFunListN("msaClientList",msaClientList);
	ltSetFunListN("msaGroupTree",msaGroupTree);
	
	/*srv-report.c*/
	ltSetFunListN("msasortreportlink",msasortreportlink);
	ltSetFunListN("msaReportShowPage",msaReportShowPage);
	ltSetFunListN("msasortreport",msasortreport);
	ltSetFunListN("msaReportJsonStore",msaReportJsonStore);
	ltSetFunListN("msaDataCenetrDownLoad",msaDataCenetrDownLoad);
	ltSetFunListN("msaSummaryReport",msaSummaryReport);
	ltSetFunListN("msacompareFlowAjaxSubmit",msacompareFlowAjaxSubmit);
	/*srv-data-data.c*/
	ltSetFunListN("msaShowAccessList",msaShowAccessList);
	ltSetFunListN("msaAccessList",msaAccessList);
	ltSetFunListN("msaAlertList",msaAlertList);
	ltSetFunListN("msaAccountList",msaAccountList);
  i=_ltPubInfo->maxfuntonnum;
  //printf("iiiiii:%d\n",i);
  //qsort(_ltfunList,i,sizeof(funList),nasSortFunList);
  
	//xmlFreeDoc(doc);
	
	for(i=0;i<_ltPubInfo->maxfuntonnum-1;i++){
		for(j=_ltPubInfo->maxfuntonnum-1;j>i;j--){
			if(	_ltfunList[j].lFunCode< _ltfunList[j-1].lFunCode){
				memcpy(&tmpFunList,&_ltfunList[j],sizeof(funList));
				memcpy(&_ltfunList[j],&_ltfunList[j-1],sizeof(funList));
				memcpy(&_ltfunList[j-1],&tmpFunList,sizeof(funList));
			}
		}
	}

	
//	for(i=0;i<_ltPubInfo->maxfuntonnum;i++){
//		printf("%d lFunCode==%u %s\n",i,_ltfunList[i].lFunCode, _ltfunList[i].strFunName);
//	}
	
	
	
	return 0;	
}



void resetcatch_hup(int signum)
{
	 
	 return;
}




void resetcatch_alarm(int signum)
{
	_exit(0);
	
}

void die(int e)
{ 
	_exit(e); 
}



/*系统启动的时候运行,初始化*/
int on_app(lt_shmHead *lt_mmHead){

  char caFunFile[256];
  sprintf(caFunFile,"%s/kernel/function",_pubconfdir);

	ltCoreFunListInit();
	setAllCoreFun();

	ltFunListInit(caFunFile);
	

	return 0;
}

int on_proc(lt_shmHead *lt_mmHead){
	

//	char *dbUser;
//  char *dbPass;
//  char *dbName; 
//  dbName=_ltPubInfo->_dbname;
//	dbUser=_ltPubInfo->_dbuser;
//	dbPass=_ltPubInfo->_dbpass;
//	G_DbCon=ltDbConnect(dbUser,dbPass,dbName);
//	if(G_DbCon!=NULL){
//		//printf("db connect ok\n");
//	}else{
//		fprintf(stderr,"db connect error\n");
//	}

	return 0;
}


int on_proc_stop(lt_shmHead *lt_mmHead){
	
	//ltDbClose(G_DbCon);
	//printf("db close ok\n");
	return 0;
}




int ltTcpDoSRequest(lt_shmHead *lt_MMHead,int confd)
{
    int i;
    int errint;
    uint32  lFunCode;
    ltMsgHead *psMsgHead;
    char **binqqargs;

    int  (*op)();   
     

    
    psMsgHead = (ltMsgHead *)lt_TcpMsgRead(confd,&errint);
    if(psMsgHead == NULL) {
            return 0;
    }
    /*msg包的其他处理*/

    psMsgHead->lMaxBytes = psMsgHead->lBytes;

    lFunCode=0;
    lFunCode=psMsgHead->lFunCode;
    /*later will be hash table and more funtion control*/
    /*serach funtion*/
    //ltMsgPrintMsg(psMsgHead);

    i=nasFindFun(lFunCode);  
    if(i==-1){
    	  ltsSysCheck(confd,psMsgHead,lt_MMHead);
    		close(confd);
    		return 0;
    }
    if(_ltfunList[i].maxruntime>0){
    	alarm(_ltfunList[i].maxruntime);
    }else{
    	alarm(900);
    }


    /*检查权限问题*/
    
//    if(_ltfunList[i].rightflag!=0){
//    	if(checkRight(confd,psMsgHead,_ltfunList[i].rightflag,lt_MMHead)==-1){
//    		close(confd);
//				return -1;
//			}
//    }
    
    
    if(_ltfunList[i].funFlag==0){
    		op=_ltfunList[i].op;
				op(confd,psMsgHead,lt_MMHead);
				close(confd);
    		return 0;
    }else if(_ltfunList[i].funFlag==1){
    	void *handle;

		  handle = dlopen (_ltfunList[i].strFunUrl, RTLD_LAZY);

		  op = dlsym (handle, _ltfunList[i].strFunName);

		  op(confd,psMsgHead,lt_MMHead);

			close(confd);

		  dlclose (handle);
		  return 0;
    }else if(_ltfunList[i].funFlag==3){/*app*/
    	int pim[2];
		  int pie[2];
		
		  if (pipe(pim) == -1) return -1;
		  if (pipe(pie) == -1) { close(pim[0]); close(pim[1]); return -1; }
		 
		  switch(vfork()) {
		    case -1:
		      close(pim[0]); close(pim[1]);
		      close(pie[0]); close(pie[1]);
		      return -1;
		    case 0:/*child process*/
		      close(pim[1]);
		      close(pie[0]);
		      /*
		      pim[0] read; 
		      pie[1] write
		      */
		      if (fd_move(0,pim[0]) == -1) _exit(120);
		      if (fd_move(1,pie[1]) == -1) _exit(120);
		      binqqargs = (char **) malloc(2 * sizeof(char *));
		      binqqargs[0]=_ltfunList[i].strFunUrl;
		      binqqargs[1]= 0;
		      execv(*binqqargs,binqqargs);
		      _exit(120);
		  }
		  /*
		      pim[1] write; 
		      pie[0] read
		   */
		  close(pim[0]);
		  close(pie[1]);
		  //fd_move(confd,pie[0]);
		  lt_TcpMsgSend(pim[1],psMsgHead);
		  psMsgHead = (ltMsgHead *)lt_TcpMsgRead(confd,&errint);
    	if(psMsgHead != NULL) {
            lt_TcpMsgSend(pim[1],psMsgHead);
    	}
		  close(confd);
		  return 0;
    }else{
    		ltsSysCheck(confd,psMsgHead,lt_MMHead);
    		close(confd);
    		return 0;
    }

    close(confd);
    return 0;
}

void ltCatch_fatal(int sig)
{
    //fprintf(stderr,"exit on signal (%d)\n", sig);
    _exit(0);
}


void sig_chld(int sig){
	pid_t pid;
	int stat;
	while( (pid=waitpid(-1,&stat,WNOHANG))>0){
		//printf("child:%d terminates\n",pid);
	}
	return;
}
void sig_int(int sig){
	
	return;
}


int lt_localserver(char *servername)
/*
 * Arguments:
 *      qlen      - maximum server request queue length (Only in TCP */
{   
  int	iSock;	/* socket descriptor and socket type	*/
  struct sockaddr_un  sSin; 
  struct timeval testtime; 
  int    iQlen;  
  char   strCmd[256];
  
  iQlen=LT_LISTENQ;

printf("start lt_localserver\n");
  
  memset(&sSin,0,sizeof(struct sockaddr_un));
  iSock = socket(AF_LOCAL, SOCK_STREAM, 0);
  unlink(servername);
  if (iSock == -1){
    return -1;
  }
  //setsockopt(iSock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  sSin.sun_family = AF_LOCAL;
  strcpy(sSin.sun_path,servername);
  //sSin.sun_port = htons(localport);
  //sSin.sin_addr.s_addr = htonl(INADDR_ANY); 

  if (bind(iSock,(struct sockaddr *)&sSin, sizeof(sSin)) < 0) {
        close(iSock);
        return (-1);
   }
    testtime.tv_sec=600000; 
		testtime.tv_usec=0;
		setsockopt(iSock,SOL_SOCKET,SO_RCVTIMEO,&testtime,sizeof(testtime));
		testtime.tv_sec = 600000; 
		testtime.tv_usec=0;
		setsockopt(iSock,SOL_SOCKET,SO_SNDTIMEO,&testtime,sizeof(testtime));

  if (listen(iSock,iQlen) == -1){
    	close(iSock);
	return -1;
  }
  sprintf(strCmd,"chmod 777  %s",servername);
  system(strCmd);
  //ndelay_off(iSock);
  
  return iSock;

}

/*启动tcp服务*/

int ltLocalServer(char *confFile){
	
	char *p;


	int  iReturn;
	/*读取配置信息并判断，如果不对返回错误*/
	if(confFile==NULL){
		fprintf(stderr,"Start tcp server error: the conf file is NULL!\n");
		return -1;	
	}

	/*设置消息包验证号*/
	p=bcCnfGetValueS(confFile,"kernel", "msgcheck");
	if(p==NULL){
		/*消息包验证号*/
		ltMsgSetValidCode(0);
	}else{
		ltMsgSetValidCode(atol(p));
	} 
	

	/*设置最大消息包*/
	p=bcCnfGetValueS(confFile,"kernel", "maxmsgpk");
	if(p==NULL){
		/*设置最大消息包*/
		lt_SetMaxTcpBuf(4096);
	}else{
		//ltMsgSetValidCode(atol(p));
		lt_SetMaxTcpBuf(atol(p));
	} 	
	
	
	p=bcCnfGetValueS(confFile,"kernel", "servername");
	if(p==NULL){
		/*设置最大消息包*/
		p="/tmp/msa";
	}	
	
    	//iSock =lt_tcpserver(8000, 20);

	iReturn=lt_localserver(p);
  if(iReturn<0){
		return (-1);
  }

	return iReturn;
}


int lt_start_simple_server(lt_shmHead *lt_MMHead,lt_shmHead   *kernel_MMHead,char *confFile,int (*on_app_start)(),int (*on_proc_start)(),int (*on_proc_stop)() ){
	int iReturn;
	
	/*初始化主进程列表*/
	int ltServerSock;
	
	if(on_app_start!=NULL){
			iReturn=on_app_start(lt_MMHead); /*一般用来注册函数*/
			if(iReturn<0){
				fprintf(stderr,"error: can't start on_app_start\n");
				exit(-1);
			}	
	}
	signal(SIGCHLD, SIG_IGN);
	signal(SIGINT,  sig_int);
	signal(SIGTERM, sig_int);
	signal(SIGPIPE, SIG_IGN);

	
	iReturn=ltLocalServer(confFile);
	if(iReturn<0){
		fprintf(stderr,"start local server error!\n");
		exit(0);
	}
	fprintf(stderr,"start local server ok!\n");
	//printf("start tcp server ok \n");
	/*系统主侦听端口*/
	ltServerSock=iReturn;
	
	for(;;){
	
		struct sockaddr_in sa;
		int dummy = sizeof(sa);
		int fd; 
		fd = accept(ltServerSock,(struct sockaddr *) &sa,&dummy);
		if (fd == -1) continue;
	
		switch(fork()) {
		      case 0:
		        close(ltServerSock);
		        signal(SIGINT, ltCatch_fatal);
						signal(SIGQUIT, ltCatch_fatal);
						signal(SIGILL, ltCatch_fatal);
						signal(SIGTRAP, ltCatch_fatal);
						signal(SIGIOT, ltCatch_fatal);
						signal(SIGFPE, ltCatch_fatal);
						signal(SIGTERM, ltCatch_fatal);
						signal(SIGALRM, ltCatch_fatal);
						//alarm(900);
						msaInitPubVar(lt_MMHead,kernel_MMHead);
						msaOpenSemLock();
		        if( on_proc_start!=NULL){
						   	iReturn = on_proc_start(lt_MMHead);
						    if(iReturn < 0) {
						        fprintf(stderr," Process %d Init Error \n",getpid());
						        _exit(0);
						    }
			 		  }
			 		 
						ltTcpDoSRequest(lt_MMHead,fd);
						if( on_proc_stop!=NULL){
							on_proc_stop(lt_MMHead);
						}
						//msaDelSemLock();
		        _exit(0);
		      case -1:
		        fprintf(stderr,"main proc start error: can't start on_app_start\n");
		}
		close(fd);
	}
	
	return 0;
	
}



/*
1> open shm
2> init pub
3> update activesys
4> on_app_start
   11>init core funtion
   12>update core function
   13>init function list
5> start local server
6> proccess request
   
   
*/
int main(int argc,char **argv){
		
	
	char *p;
	int  intMaxShmSize;
	int  intShmKey;
  msaPubInfo *ltPubInfo;
  lt_shmHead   *lt_MMHead;
  lt_shmHead   *kernel_MMHead;
  
  
   /* 父进程退出,程序进入后台运行 */
  if(fork()!=0) exit(1);
  
  if(setsid()<0)exit(1);/* 创建一个新的会议组 */ 
   
  /* 子进程退出,孙进程没有控制终端了 */  
  if(fork()!=0) exit(1);
  
  signal(SIGHUP,resetcatch_hup);
  signal(SIGCHLD, SIG_IGN);
	signal(SIGINT,  SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
  

  
  /*读取最大共享内存数*/
	p=bcCnfGetValueS(_pubconfdir,"kernel", "maxshmsize");
	if(p==NULL){
		fprintf(stderr,"Start applicatin error: maxshmsize is NULL!\n");
		die(51);
	}
	intMaxShmSize=atol(p);


	/*读取系统shmKey*/
	p=bcCnfGetValueS(_pubconfdir,"kernel", "shmkey");
	if(p==NULL){
		fprintf(stderr,"Start applicatin error: shmkey is NULL!\n");
		die(52);
	} 
	intShmKey=atol(p);
	
	lt_MMHead=msaopenShmMem(intShmKey,intMaxShmSize);
	if(lt_MMHead==NULL){
		fprintf(stderr,"can open share memory!\n");
		die(53);
	}
  
   ltPubInfo=(msaPubInfo  *)lt_MMHead->ShmMemPtr;
	 if(ltPubInfo == NULL) {
	 	  fprintf(stderr,"can open share memory pubinfo!\n");
	    die(54);
	 }
	 
	
	p=bcCnfGetValueS(_pubconfdir,"kernel", "maxkernelshmsize");
	if(p==NULL){
			fprintf(stderr,"Start applicatin error: maxkernelshmsize is NULL!\n");
			return -1;	
	}
	intMaxShmSize=atol(p);
	kernel_MMHead=msaopenBigBootMem(intMaxShmSize);
	if(!kernel_MMHead){
			closeShmMem(lt_MMHead);
			return -1;
	}
	_bcKernelHead=(msaKernelHead *)kernel_MMHead->ShmMemPtr;
		
	
   msaInitPubVar(lt_MMHead,kernel_MMHead);
	 

  /*加入其他执行程序*/

  lt_start_simple_server(lt_MMHead,kernel_MMHead,_pubconfdir,on_app,on_proc,on_proc_stop);
    
  return 0;
}

