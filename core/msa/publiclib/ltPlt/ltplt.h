#ifndef _LT_PLT_
#define _LT_PLT_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "libxml/tree.h"
#include "libxml/parser.h"
#include "ltmsg.h"

struct ltDb{
		xmlDocPtr doc;	
        xmlNodePtr bodyPtr;
		char       *head;
};

typedef struct ltDb* ltDbHeadPtr;   
typedef xmlNodePtr ltRecordPtr;
typedef xmlNodePtr ltTablePtr;

/*初始化数据模版指针*/
ltDbHeadPtr lt_dbinit();

/*增加head内容*/
int lt_dbput_head(ltDbHeadPtr dbPtr,char *value);

/*增加子节点变量*/
int lt_dbput_rootvar(ltDbHeadPtr dbPtr,char *name,char *value);

/*一次增加多个子节点变量*/
int lt_dbput_rootvars(ltDbHeadPtr dbPtr,int varnum,...);

/*打印整个dbhead结构*/
char *lt_db_print(ltDbHeadPtr dbPtr);

/*得到整个dbhead指针*/
char *lt_db_mem(ltDbHeadPtr dbPtr);

/*回复dbhead内容模版*/
int lt_db_response(int iSock,ltMsgHead *psMsgHead0,ltDbHeadPtr dbPtr);

/*得到dbhead头*/
char *lt_dbget_head(xmlDocPtr doc);
/*得到charset头*/
char *lt_dbget_charset(xmlDocPtr doc);
/*得到dbhead根变量值*/
char *lt_dbget_Var(xmlNodePtr nodePtr,char *varName);

/*得到循环变量值*/
char *lt_dbget_LoopVar(xmlNodePtr node,char *tablename,char *varName,int index);

/*设置cookie变量*/
int lt_db_setcookie(ltDbHeadPtr dbPtr,const char *name, const char *value);
/*设置cookie变量1*/
int lt_db_setcookieA(ltDbHeadPtr dbPtr,const char *name, const char *value,
        char *pPath,char *pDomain,char *pExpire);
/*删除cookie变量*/
int lt_db_delcookie(ltDbHeadPtr dbPtr,const char *name);

/*设置web页无缓存*/
int lt_db_nocache(ltDbHeadPtr dbPtr);
int lt_web_nocachemsie(ltDbHeadPtr dbPtr);

/*在根上加一个table*/ 
ltTablePtr lt_dbput_table(ltDbHeadPtr dbPtr,char *tablename);

/*在table上加一条完整的record值*/
int lt_dbput_recordvars(ltTablePtr tablePtr,int varnum,...);

/*在table上加一个table*/ 
ltTablePtr lt_dbput_subtable(ltTablePtr tablePtr,char *tablename);

/*在table下加record*/ 
ltRecordPtr lt_dbput_record(ltTablePtr tablePtr);

/*在record下加值*/
int lt_dbput_recordvar(ltRecordPtr rsPtr,char *name,char *value);

xmlNodePtr lt_dbget_body(xmlDocPtr doc);
xmlNodePtr lt_dbget_table(xmlNodePtr tableParent,char  *tableName);
int lt_dbget_LoopNum(xmlNodePtr parNode,char *tablename);

/*pFile 模版指针 ltDbBuf xml模版内容*/
char *ltPltFileParse(char *pFile,xmlDocPtr ltDocPtr,int chSet);
/* 将模板转换为 HTML字符串
       pIn      ---- 模板内容的字符串
       ltDbBuf ---- 模板数据xml结构
   返回:
       生成的 HTML字符串

 */
char *ltPltMemParse(char *pIn,xmlDocPtr ltDocPtr);

int lt_db_htmlpage(ltDbHeadPtr dbPtr,char *charset);

int lt_html_response(int iSock,ltMsgHead *psMsgHead0,char *htmlHead,char *htmlContent);
int lt_download(int iSock,ltMsgHead *psMsgHead0,char *fileurl,char *filename,char *contype);
int lt_downloadA(int iSock,ltMsgHead *psMsgHead0,char *con,int consize,char *filename,char *contype);
int lt_db_redirect(int iSock,ltMsgHead *psMsgHead0,const char *buf);
int lt_dbput_html(int iSock,ltMsgHead *msgPk,xmlDocPtr xDoc,char *pltPageUrl,char *head,int chSet);
void lt_dbfree(ltDbHeadPtr dbPtr);

#endif


