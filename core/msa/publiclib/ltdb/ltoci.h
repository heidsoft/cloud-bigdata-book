#ifndef __OCI_H
#define __OCI_H

#include <oci.h>
#define CNM_MAX         32          /*max length of column name*/
#define COL_MAX         255         /*max count of columns*/

#define SQL_CMD_UNKNOWN     1       /*unknown command*/
#define SQL_CMD_SELECT      2       /*select command*/
#define SQL_CMD_INSERT      3       /*insert command*/
#define SQL_CMD_UPDATE      4       /*update command*/
#define SQL_CMD_DELETE      5       /*delete command*/
    
typedef struct
{    
    text *cbuf; /*column name*/
    sb4 cbufl;          /*length of the column name*/
    sb2 dbsize;         /*maximum size*/
    sb4 dsize;          /*maximum display size*/
    sb2 dbtype;         /*internal datatype code*/
    sb2 prec;           /*precision of numeric items*/
    sb2 scale;          /*scale of numerics*/
    sb2 nullok;         /*whether null values are permitted in the column*/    
} DB_DESCR;             /*describe the column info for select-list*/    

/*typedef OCIDescribe DB_DESCR;*/
    
typedef struct
{
    ub1* name;          /*A pointer to select-list name*/
    sword namel;        /*length of the select-list name*/
    ub1* buf;           /*A pointer to the output variable*/
    sword bufl;         /*length of the variable*/
    sword ftype;        /*The external datatype to which the select-list item is to be converted*/
    sb2 ind;            /*indicates the result of fetching select-list item*/
    ub2 rlen;           /*length of returned data*/
    ub2 rcode;          /*column return code after the fetch*/
} DB_DEFINE;            /*define output variables for select-list*/
       
typedef struct
{    
    OCIEnv *pEnv;
    OCIServer *pSrv;
    OCISession *pSession;
    OCISvcCtx *pSvc;
    OCIError *pErr;
} DB_CONNECT;
    
typedef struct
{        
    char *pSql;
    OCIStmt *pStmt;
    DB_CONNECT *pConn;
    DB_DEFINE* pDef;
    int nDef;
    int bDescribed;
} DB_CURSOR;


/*---------------------------------------------------------------
  函数名称：dbConnect()
  功能说明：连接数据库
  参数说明：[in]uid     -- 数据库用户名
            [in]passwd  -- 数据库密码
            [in]service -- 数据库服务名
  返回值：  不成功  -- NULL
            成功    -- DB_CONNECT结构指针
---------------------------------------------------------------*/
DB_CONNECT* dbConnect(char* uid, char* passwd, char* service);

/*---------------------------------------------------------------
  函数名称：dbDisconnect()
  功能说明：断开数据库连接，释放pConn指向的buffer
  参数说明：[in]pConn   -- DB_CONNECT结构指针
  返回值：  不成功  -- 0
            成功    -- 1
---------------------------------------------------------------*/
int dbDisconnect(DB_CONNECT* pConn);

/*---------------------------------------------------------------
  函数名称：dbOpen()
  功能说明：打开数据库游标
  参数说明：[in]pConn   -- DB_CONNECT结构指针
  返回值：  不成功  -- NULL
            成功    -- DB_CURSOR结构指针
---------------------------------------------------------------*/
DB_CURSOR* dbOpen(DB_CONNECT* pConn);

/*---------------------------------------------------------------
  函数名称：dbClose()
  功能说明：关闭数据库游标
  参数说明：[in]pCursor -- DB_CURSOR结构指针
  返回值：  不成功  -- 0
            成功    -- 1
---------------------------------------------------------------*/
int dbClose(DB_CURSOR* pCursor);

/*---------------------------------------------------------------
  函数名称：dbParse()
  功能说明：解析SQL语句
  参数说明：[in]pCursor -- DB_CURSOR结构指针
            [in]sqlstmt -- SQL语句字符串，以'\0'结尾
  返回值：  不成功  -- 0
            成功    -- 1
---------------------------------------------------------------*/
int dbParse(DB_CURSOR* pCursor, char* sqlstmt);

/*---------------------------------------------------------------
  函数名称：dbBind()
  功能说明：绑定SQL语句中的变量与应用程序中的变量
  参数说明：[in]pCursor -- DB_CURSOR结构指针
            [in]sqlvar  -- SQL语句中的变量名，以'\0'结尾
            [in]progv   -- 程序中的变量
  返回值：  不成功  -- 0
            成功    -- 1
---------------------------------------------------------------*/
int dbBind(DB_CURSOR* pCursor, char* sqlvar, char* progv);

/*---------------------------------------------------------------
  函数名称：dbDescribe()
  功能说明：取得SQL语句中select-list的属性
  参数说明：[in]pCursor -- DB_CURSOR结构指针
            [in]pos     -- select-list的序号，从1开始计数
            [in/out]pDescr  -- DB_DESCR结构指针
  返回值：  不成功  -- 0
            成功    -- 1
---------------------------------------------------------------*/
int dbDescribe(DB_CURSOR* pCursor, int pos, DB_DESCR* pDescr);

/*---------------------------------------------------------------
  函数名称：dbFetch()
  功能说明：取得游标所在位置的记录，并将游标移动至下一条记录
  参数说明：[in/out]pCursor -- DB_CURSOR结构指针            
  返回值：  不成功  -- 0
            成功    -- 1
---------------------------------------------------------------*/
int dbFetch(DB_CURSOR* pCursor);

/*---------------------------------------------------------------
  函数名称：dbFieldName()
  功能说明：取得select-list的名称
  参数说明：[in]pCursor -- DB_CURSOR结构指针
            [in]iPos    -- select-list的序号，从1开始计数
            [in/out]pName -- 指向存放select-list值的buffer
            [in/out]pLen -- 指向buffer长度，函数返回后指向实际长度
  返回值：  不成功  -- 0
            成功    -- 1
---------------------------------------------------------------*/
int dbFieldName(DB_CURSOR* pCursor, int iPos, char* pName, int* pLen);

/*---------------------------------------------------------------
  函数名称：dbFieldValue()
  功能说明：取得select-list的值
  参数说明：[in]pCursor -- DB_CURSOR结构指针
            [in]iPos    -- select-list的序号，从1开始计数
            [in/out]pVal -- 指向存放select-list值的buffer
            [in/out]pLen -- 指向buffer长度，函数返回后指向实际长度
  返回值：  不成功  -- 0
            成功    -- 1
---------------------------------------------------------------*/
int dbFieldValue(DB_CURSOR* pCursor, int iPos, char* pVal, int* pLen);

/*---------------------------------------------------------------
  函数名称：dbExec()
  功能说明：exec sql statement and define select-list。
  参数说明：[in][out]pCursor  -- DB_CURSOR结构指针
  返回值：  不成功  -- 0
            成功    -- 1
---------------------------------------------------------------*/
int dbExec(DB_CURSOR* pCursor);

/*---------------------------------------------------------------
  函数名称：dbCommit()
  功能说明：提交对数据库的更新。
  参数说明：[in]pConn   -- DB_CONNECT结构指针            
  返回值：  不成功  -- 0
            成功    -- 1
---------------------------------------------------------------*/
int dbCommit(DB_CONNECT* pConn);

/*---------------------------------------------------------------
  函数名称：dbRollback()
  功能说明：取消自上次commit之后对数据库的更新。
  参数说明：[in]pConn   -- DB_CONNECT结构指针            
  返回值：  不成功  -- 0
            成功    -- 1
---------------------------------------------------------------*/
int dbRollback(DB_CONNECT* pConn);

/*---------------------------------------------------------------
  函数名称：dbPrintError()
  功能说明：显示数据库操作错误信息。
  参数说明：[in]pCursor  -- DB_CURSOR结构指针
  返回值：  无返回
---------------------------------------------------------------*/
void dbPrintError(DB_CONNECT* pConn);

/*---------------------------------------------------------------
  函数名称：dbGetLastError()
  功能说明：取得数据库操作错误号。
  参数说明：[in]pCursor  -- DB_CURSOR结构指针
  返回值：  错误号
---------------------------------------------------------------*/
int dbGetLastError(DB_CONNECT* pConn);

/*---------------------------------------------------------------
  函数名称：dbSqlCommandType()
  功能说明：取得Sql操作类型。
  参数说明：[in]sqlstmt  -- Sql语句指针
  返回值：  
            SQL_CMD_UNKNOWN -- 未知
            SQL_CMD_SELECT -- select
            SQL_CMD_INSERT -- insert
            SQL_CMD_UPDATE -- update
            SQL_CMD_DELETE -- delete
---------------------------------------------------------------*/
int dbSqlCommandType(char* sqlstmt);

#endif
