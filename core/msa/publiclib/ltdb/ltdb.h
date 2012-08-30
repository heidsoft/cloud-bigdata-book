#ifndef  _LT_DB_
#define _LT_DB_

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifdef  _LT_DB_MYSQL_ 

#include "mysql.h"
typedef MYSQL ltDbConn;
typedef MYSQL_ROW  LT_DBROW;
typedef MYSQL_RES  DB_CURSOR;
typedef char**	   LT_DBFIELD;
typedef struct{
	DB_CURSOR *db_cursor;
	LT_DBROW db_row;
	unsigned int num_field;
	LT_DBFIELD db_field;
} ltDbCursor;

#endif

#ifdef  _LT_DB_POSTGRESQL_ 

#include "libpq-fe.h"
typedef PGconn ltDbConn;
typedef char **LT_DBROW;
typedef char**	   LT_DBFIELD;
typedef struct{
  ltDbConn *db_conn;
	PGresult *db_cursor;
	LT_DBROW db_row;
	unsigned int num_field;
	LT_DBFIELD   db_field;
	char     cursorname[48];
	unsigned int nowline;
	unsigned int maxline;
} ltDbCursor;

#endif

#ifdef  _LT_DB_ORACLE_ 

#include "ltoci.h"
typedef DB_CONNECT ltDbConn;
typedef char **LT_DBROW;
//typedef MYSQL_RES  DB_CURSOR;
typedef char**	   LT_DBFIELD;
typedef struct{
	DB_CURSOR *db_cursor;
	LT_DBROW db_row;
	unsigned int num_field;
	LT_DBFIELD db_field;
} ltDbCursor;

#endif


/*数据库连接*/
ltDbConn *ltDbConnect(char *pUser,char *pPassword,char *pService);
/* 关闭数据库连接，并释放有关资源  */
void  ltDbClose(ltDbConn *psConn);



/* 打开游标, 使用带格式参数  */
//ltDbCursor *ltDbOpenCursor(ltDbConn *pConn,  char *pSmt,...);
ltDbCursor *ltDbOpenCursorS(ltDbConn *pConn,  char *pSmt);
ltDbCursor *ltDbOpenCursor(ltDbConn *pConn,  char *pSmt);
void ltDbCloseCursor(ltDbCursor *ltCursor);



/*光标的fetch处理*/
/*光标的fetch处理*/
unsigned  int ltNumField(ltDbCursor *ltCursor);
/*得到字段名*/
LT_DBFIELD  ltDbFetchField(ltDbCursor *ltCursor);
/*fetch并得到字段值*/
LT_DBROW  ltDbFetchRow(ltDbCursor *ltCursor);
/*type, len, arg*/
//int ltDbFetchInto(ltDbCursor *ltCursor,...);


/*单条记录操作*/
//LT_DBROW ltDbOneRow(ltDbConn *pConn,int *fieldnum,char *caSmt);
//void ltDbFreeRow(LT_DBROW dbRow);
///*type, len, arg*/
//int ltDbOneRowTo(ltDbConn *pConn,char *pSmt,...);


/*更新操作*/
int ltDbExecSql(ltDbConn *pConn,  char *pSmt);
//int ltDbExecSqlS(ltDbConn *pConn,  char *pSmt);
//unsigned int ltDbAffectRows(ltDbConn *pConn);

///*事务操作*/
//int ltDbCommit(ltDbConn *pConn);
//int ltDbRollback(ltDbConn *pConn);
//
//
//
///*错误处理*/
//unsigned int  ltDbErrorCode(ltDbConn *pConn);
//char *ltDbError(ltDbConn *pConn);

#endif
