#ifndef __MSASDEF__
#define __MSASDEF__


#include <netinet/ether.h>
#include "pubfun.h"

#define NAS_MAX_COREFUNNUM    500
#define NAS_MAX_FUNNUM        500
#define NAS_DEFAULT_FUNTIME   900

char *pubconfdir;

/*注册函数库*/
typedef struct corefunList_s{
	char strFunName[64]; /*函数名称*/
  int (*op)();
} corefunList;



typedef struct funList_s{
	unsigned int lFunCode;  /*代码*/
	unsigned int funFlag;   /*0 直接调用 1 dll动态调用 2java类 3应用程序调用*/
	char strNode[32]; /*函数说明*/
	char strFunName[64]; /*函数名称*/
	char strFunUrl[128]; /*dll调用的url*/
	unsigned int  maxruntime;    /*该函数最大处理时间*/
	unsigned int  activeflag;    /*激活相关性 0与激活无关 1与激活相关 */
	unsigned int  rightflag;    /*权限相关性  0与权限无关 1与权限相关  */
  int (*op)();
} funList;

corefunList  _ltcorefunList[NAS_MAX_COREFUNNUM];
funList      _ltfunList[NAS_MAX_FUNNUM];
int          _maxfuntonnum;

typedef struct nasAdmSession_s {
	unsigned long  lSip;       /* 源IP地址     */
	unsigned long  lSid;       /* session id  4位数字  */
	char           lright[100];     /* 权限       */
	char           caname[64];     /* 权限       */
  int            allgroup;   /* 管理部门   */
  unsigned int   trytimes;
	unsigned long  lasttime;   /*最后一次登陆时间 */
} nasAdmSession;

#define MAX_LANG_LIST_CONSIZE        256
#define MAX_SYSTEM_MSG               500
/*每句话最多512字节*/
/*
0:chinese
1:en
*/
typedef struct conListType_s {
    char            conStr[MAX_LANG_LIST_CONSIZE];
} conListType;


typedef struct msasPubInfo_s {
	  unsigned int  lRunning;
	  char          _dbname[128];
	  char          _dbuser[128];
	  char          _dbpass[128];
	  int           sysActive;
	  int           maxfuntonnum;
	  nasAdmSession      admSession[20];
	  conListType        pubMsgStr[MAX_SYSTEM_MSG];
	  int           defaultLan;    /*默认语言*/      
}msasPubInfo;

msasPubInfo  *_ltPubInfo;

ltDbConn     *G_DbCon; /*各个包处理进程的数据库连接*/

lt_shmHead *msascreateShmMem(unsigned int intShmKey,unsigned int intMaxShmSize);
lt_shmHead *msasopenShmMem(unsigned int intShmKey,unsigned int intMaxShmSize);
int msascloseShmMem(lt_shmHead *lt_MMHead);

/*nasPubUtil.c*/
int msasInitPubInfo(lt_shmHead *lt_MMHead);
int ltWebMsgErr(char *errorMsg,int confd,ltMsgHead *ltMsgPk);
int msasInitPubVar(lt_shmHead *lt_MMHead);

/*srv-test.c*/
//int ErrorTest(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasTestApp(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasTestApp1(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

/*Example.c*/
int msasAddAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowRulePage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAnjianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAnjianTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowAnJian(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAlertAnJianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasRuleListAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowRule(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasRuleAnJianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAlertRuleAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasRuleTypeAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowanjianPage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAddanjianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasUpdateanjianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
   

/*srv-test2.c*/
#endif


