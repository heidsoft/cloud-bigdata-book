#ifndef __MSASDEF__
#define __MSASDEF__


#include <netinet/ether.h>
#include "pubfun.h"

/*部门层次级别*/
#define MSAS_MAX_GROUPLEVEL   5
/*全部部门总数*/
#define NAS_MAX_ALLGRPNUM     1000
/*系统函数参数*/
#define NAS_MAX_COREFUNNUM    500
#define NAS_MAX_FUNNUM        500
#define NAS_DEFAULT_FUNTIME   900

#define COMM_MAX_COREFUNNUM   50

/*管理员参数*/
#define MAX_ADMIN_SESSION            300
#define MAX_ADMIN_TIMEOUT            7200
#define MAX_ADMIN_TRYTIMEOUT         120
#define MAX_ADMIN_MAXRIGHT           100

#define MSAS_RIGHT_SYSADMIN          5

#define MSAS_DEFAULT_ONLINEHOSTSTR   "500"

/*系统服务一级服务数*/
#define NAS_MAX_SYSSRVNUM        50

/*自定义字符串*/
#define MAX_SYSTEM_MSG               1000
/*最长256*/
#define MAX_LANG_LIST_CONSIZE        256
#define CTYPEINDEXSTART         15
#define CTYPEINDEXEND           37
#define CUNTRYINDEXSTART        38
#define CUNTRYINDEXEND          230
#define NETIDINDEXSTART         260
#define NETIDINDEXEND           369
#define ALERTTYPENAME           232
#define ALERTTYPENAMEEND        242
#define ALERTTYPENAME0          243

/*即时滚动的报警数目*/
#define MAXALERTNOW           200

#define pubconfdir   "/etc/msa/msas"
#define datacenterdir   "/datacenter/msas"
#define proccessdir   "/datacenter/proccess"
#define _TMPDIR   "/datacenter/tmp"

#define SERV_PORT 8888

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
	char           lright[MAX_ADMIN_MAXRIGHT];     /* 权限       */
	char           caname[64];     /* 权限       */
  char            allgroup[NAS_MAX_ALLGRPNUM];   /* 管理部门   */
  unsigned int   trytimes;
	unsigned long  lasttime;   /*最后一次登陆时间 */
} nasAdmSession;

/*
0:chinese
1:en
*/
typedef struct conListType_s {
    char            conStr[MAX_LANG_LIST_CONSIZE];
} conListType;

/*
hash 基于分离链解决冲突的链表
freecon 单链表解决办法,最后一个的标记为next=-1
*/
typedef struct _shm_hash_link_onlinehosthead {
    unsigned int       bufptr;  /*从头开始的偏移量*/
    unsigned int       bufsize;  /*从头开始的偏移量*/ 
    unsigned int       conbufptr;  /*从头开始的偏移量*/ 
    unsigned int       bufnum;
    int                freecontenthead;
    int                freecontenttail;
    int                locksemid;
    int                nownum;
} shm_hash_link_onlinehosthead;

/*hash index 根据index分布*/
typedef struct _shm_hash_link_onlinehostindex {
    int           next; /*-1相当于NULL*/
} shm_hash_link_onlinehostindex;

typedef struct msasOnlineHostS_s {   
	  uint32   starttime;
	  uint32   lasttime;  
	  uint32   ipadd;                /* 通信的ip地址*/
    char     caName[128];          /* 名称      */
    char     caKey[32];            /* 序列号    */
    uint32   sysversion;           /* 系统版本  */
    uint32   cpunum;               /* cpu利用率 */
    uint32   memnum;           	   /* mem利用率 */
    uint32   sysdisknum;           /* 系统硬盘利用率 */
    uint32   appdisknum;           /* 应用硬盘利用率 */
    uint32   usernum;              /* 在线用户 */
    uint32   p2pnum;               /* p2p用户 */
    uint32   livernum;             /* live用户 */
    uint32   gamenum;              /* game用户 */
    uint32   soketnum;             /* 股票用户 */
    uint32   tcpnum;               /*tcp连接数*/
    uint32   udpnum;               /*udp连接数*/
    uint32   httpnum;              /* http连接数 */   
    uint32   urlnum;               /* 活动网址数*/
    uint64   bytenum;              /*出口速率*/ 
    uint64   bytenumu;             /*出口速率*/ 
    uint64   bytenumd;             /*出口速率*/ 
    uint16   levelgroup[MSAS_MAX_GROUPLEVEL]; 
} msasOnlineHostS;

typedef struct msasOnlineHostHour_s { 
	  char     chours; 
	  uint16   hourtimes;
    uint32   cpunum;               /* cpu利用率 */
    uint32   memnum;           	   /* mem利用率 */
    uint32   sysdisknum;           /* 系统硬盘利用率 */
    uint32   appdisknum;           /* 应用硬盘利用率 */
    uint32   usernum;              /* 在线用户 */
    uint32   p2pnum;               /* p2p用户 */
    uint32   livernum;             /* live用户 */
    uint32   gamenum;              /* game用户 */
    uint32   soketnum;             /* 股票用户 */
    uint32   tcpnum;               /*tcp连接数*/
    uint32   udpnum;               /*udp连接数*/
    uint32   httpnum;              /* http连接数 */   
    uint32   urlnum;               /* 活动网址数*/
    uint64   bytenum;              /*出口速率*/ 
    uint64   bytenumu;             /*出口速率*/ 
    uint64   bytenumd;             /*出口速率*/
    unsigned long long srvflow[NAS_MAX_SYSSRVNUM];
} msasOnlineHostHour;

typedef struct msasOnlineHost_s { 
	  uint32   starttime;
	  uint32   lasttime;  
	  uint32   ipadd;                /* 通信的ip地址*/
    char     caName[128];          /* 名称      */
    char     caKey[32];            /* 序列号    */
    uint32   sysversion;           /* 系统版本  */
    uint32   cpunum;               /* cpu利用率 */
    uint32   memnum;           	   /* mem利用率 */
    uint32   sysdisknum;           /* 系统硬盘利用率 */
    uint32   appdisknum;           /* 应用硬盘利用率 */
    uint32   usernum;              /* 在线用户 */
    uint32   p2pnum;               /* p2p用户 */
    uint32   livernum;             /* live用户 */
    uint32   gamenum;              /* game用户 */
    uint32   soketnum;             /* 股票用户 */
    uint32   tcpnum;               /*tcp连接数*/
    uint32   udpnum;               /*udp连接数*/
    uint32   httpnum;              /* http连接数 */   
    uint32   urlnum;               /* 活动网址数*/
    uint64   bytenum;              /*出口速率*/ 
    uint64   bytenumu;             /*出口速率*/ 
    uint64   bytenumd;             /*出口速率*/ 
    uint16   levelgroup[MSAS_MAX_GROUPLEVEL];        /*5级别分组*/
    unsigned long long srvflow[NAS_MAX_SYSSRVNUM];
    uint8    cmdupdateflag;
    msasOnlineHostHour hourinfo;
    uint16   intervalfig;
    uint8    alarmlog;    /*上报报警以及拦截日志*/
    uint8    trlog;       /*上报轨迹记录*/
    uint8    onlinelog;   /*上报上网络访问记录*/
    uint8    identitylog; /*上报身份记录*/
    uint8    conlog;      /*上报内容记录*/
    uint8    statlog;      /*上报统计记录*/       
    uint16   uploadtimeval;       /*上传时间间隔*/
    uint8    vpnflag; 
    uint8    havecmd;       
} msasOnlineHost;

/*hash内容队列,根据空余插入*/
typedef struct _shm_hash_link_onlinehost {
    int           next;
    msasOnlineHost   item;
} shm_hash_link_onlinehost;

typedef struct jiffy_counts_t {
	unsigned long long usr,nic,sys,idle,iowait,irq,softirq,steal;
	unsigned long long total;
	unsigned long long busy;
} jiffy_counts_t;

typedef struct nasTopSrv_s {
	int           srvid;
	char          srvname[48];
}nasTopSrv;

typedef struct AlertList_s {
	int           Matching_id;
	char          gnmkey[32];
	char          srvname[128];
	char          Rule_id[32];
  char          anjian_id[32];
  char          Rule_name[64];
  char          Matching_time[32];
  char          Net_ending_ip[32];
  char          Net_ending_code[32];
  char          Net_ending_mac[16];
  char          Destination_ip[32];
  char          Service_type[16];
  char          KeyWord1[64];
  char          KeyWord2[32];
  char          KeyWord3[24];
  char          Customer_name[32];
  char          Certificate_type[12];
  char          Certificate_code[24];
  int           Rule_type;
  char          contenturl[128];
}AlertList;



typedef struct msasPubInfo_s {
	  unsigned int  lRunning;
	  char          _dbname[128];
	  char          _dbuser[128];
	  char          _dbpass[128];
	  int           sysActive;
	  int           maxfuntonnum;
	  int           msasGeneralHour;
	  nasAdmSession      admSession[MAX_ADMIN_SESSION];
	  conListType        pubMsgStr[MAX_SYSTEM_MSG];
	  int                defaultLan;    /*默认语言*/  
	  shm_hash_link_onlinehosthead  _onlinehosthead;   
	  unsigned long long syssrvflow[NAS_MAX_SYSSRVNUM];
	  nasTopSrv          topSrvName[NAS_MAX_SYSSRVNUM]; 
	  AlertList          _AlertList[MAXALERTNOW];
	  unsigned  int      AlertListIndex;
}msasPubInfo;

msasPubInfo  *_ltPubInfo;

ltDbConn     *G_DbCon; /*各个包处理进程的数据库连接*/

typedef struct msasDir_s {
    char   caDirName[128];
    char   caDirPath[256];
    int    dirTime;    
} msasDir;


lt_shmHead *msascreateShmMem(unsigned int intShmKey,unsigned int intMaxShmSize);
lt_shmHead *msasopenShmMem(unsigned int intShmKey,unsigned int intMaxShmSize);
int msascloseShmMem(lt_shmHead *lt_MMHead);

/*nasPubUtil.c*/
int msasInitPubInfo(lt_shmHead *lt_MMHead);
int ltWebMsgErr(char *errorMsg,int confd,ltMsgHead *ltMsgPk);
int msasInitPubVar(lt_shmHead *lt_MMHead);
int logWrite(ltMsgHead *ltMsgPk,char *msg,int msgtype);
int nasTimeFormat(char *pFormat,long lTime);
int checkRight(int confd,ltMsgHead *ltMsgPk,int right,lt_shmHead *lt_MMHead);
int strgb2utf8(const char *cpInput,char *cpOut,int iInputLen);
int msasGetName(char *caKey,int start,int end,char *caGetName);
int dirtimecmp(msasDir* a,msasDir* b);
msasDir* mydirlist(char* dir,int* len);
int msasDataCenetrDownLoad(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int nasTimeGetDate(char *cDate,long lTime);
unsigned long nasCvtLongTime(char *caSdate,char *caTime);
int msasReportList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int nasTimeLongFormat(char *pFormat,long lTime);
int nasCvtStime(unsigned long lTime,char *caDate,char *caTime);
int msasTestApp(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

/*srv-admin.c*/
int msasShowAdminLogon(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAdminLogon(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAdminLogout(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msaShowTopview(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

int msasSysReset(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasSysShutdown(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

int msasPrintHashHost(msasPubInfo  *ltPubInfo);
int msasDeleteHost(msasPubInfo  *ltPubInfo,unsigned long lip);
msasOnlineHost *msasHashLookUpHost(msasPubInfo  *ltPubInfo,unsigned long lip);
msasOnlineHost * msasHashInsertHost(msasPubInfo  *ltPubInfo,unsigned long k, msasOnlineHost *item);
int msasInitHostListHash(msasPubInfo  *ltPubInfo);
int msasPrintFreeHashHost(msasPubInfo  *ltPubInfo);

/*srv-group.c*/
int msasGroupTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasGroupAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowAddClient(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasTerminfoAdd(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasTermInfoUpdate(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasTermInfoView(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowClientList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasClientList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAdminUserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAdminAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasCheckOutAdmin(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAdmuserList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAdmlog(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int vpnonlineuser(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int tovpnsetting(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int vpnsetting(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

/*srv-now.c*/
int msasShowXTZY(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasZaixianInfo(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasHostList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasSrvInfo(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowHostList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasGetAlertNow(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowClientSrv(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

/*srv-data.c*/
int msasRealIdList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasNetIdList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasNetIdTrackList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasClientOnOff(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowAccessList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAccessList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasSearchTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowAccessSearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasIpbackSearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasNewSearchList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAccesssCmdSet(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasYunSearchList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowYunSearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasYunSearchCMDList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasYunSearchDel(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasSearchShowList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasGetInfoByKey(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasSearchShowListt(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShKeySearch(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShKeyList(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

int msasShowjson(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowYunSearchPage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

/*srv-rule.c*/
int msasAnjianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAlertAnJianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasRuleListAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowRule(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAlertRuleAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasRuleAnJianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasRuleTypeAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAnjianTree(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowAnJian(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowanjianPage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAddanjianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasUpdateanjianAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAddAction(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasShowRulePage(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasAlertRuleLsit(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);

/*srv-report*/
int msasreportproc(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);
int msasreportproc1(int confd,ltMsgHead *ltMsgPk,lt_shmHead *lt_MMHead);



#endif


