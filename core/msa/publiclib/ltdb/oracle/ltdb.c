#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "lttype.h"
#include "dboracle.h"
#include "ltdb.h"

ltDbConn *ltDbConnect(char *pUser,char *pPassword,char *pService){
	
    ltDbConn     *tempDbConn=NULL;
    tempDbConn =(ltDbConn *) dbConnect(pUser,pPassword,pService);
    return (ltDbConn *)tempDbConn;

}

void  ltDbClose(ltDbConn *psConn){
	dbDisconnect(psConn);
}





/*光标操作*/
/*  Set Bind Values */
int _ltDbSetBindValue_o(ltDbConn *pConn,DB_CURSOR *pCursor,int iSumBind,va_list *ap)
{
    int i,iType,lMaxLen;
    OCIBind* pBind=NULL;
    char caVarName[32];
    static  int ind;
    long *lValue;
    char *pValue;
    double *dValue;
    int status;
    char *pName;
    for(i=0;i<iSumBind;i++) {
        pName = va_arg(*ap,char *);
        iType = va_arg(*ap,int);
        lMaxLen = va_arg(*ap,int);
        switch(iType) {
            case LT_TYPE_STRING:
                pValue = va_arg(*ap,char *);
                sprintf(caVarName,":%s",pName);
                status = OCIBindByName(pCursor->pStmt, &pBind, pCursor->pConn->pErr,
                        (text *) caVarName,strlen(caVarName), (ub1 *) pValue, lMaxLen, SQLT_STR, 
                        (dvoid *)&ind, (ub2 *) 0, (ub2) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT);
                if(status != OCI_SUCCESS) {
                    return ltDbErrorCode(pConn);
                }
                break;
            case LT_TYPE_SHORT:
            case LT_TYPE_LONG:
                lValue = va_arg(*ap,long *);
                sprintf(caVarName,":%s",pName);
                status = OCIBindByName(pCursor->pStmt, &pBind, pCursor->pConn->pErr,
                        (text *) caVarName,strlen(caVarName), (ub1 *) lValue, sizeof(long), SQLT_INT, 
                        (dvoid *)&ind, (ub2 *) 0, (ub2) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT);
                if(status != OCI_SUCCESS) {
                    return ltDbErrorCode(pConn);
                }
                break;
            case LT_TYPE_FLOAT:
                dValue = va_arg(*ap,double *);
                sprintf(caVarName,":%s",pName);
                status = OCIBindByName(pCursor->pStmt, &pBind, pCursor->pConn->pErr,
                        (text *) caVarName,strlen(caVarName), (ub1 *) dValue, sizeof(double), SQLT_FLT, 
                        (dvoid *)&ind, (ub2 *) 0, (ub2) 0, (ub4) 0, (ub4 *) 0, OCI_DEFAULT);
                if(status != OCI_SUCCESS) {
                    return ltDbErrorCode(pConn);
                }
                break;
            default:
                return -1;
        }
    }
    return 0;
}


DB_CURSOR *ltDbOpenSql0(ltDbConn *pConn,char *pSmt,int iSumBind,va_list ap)
{
    DB_CURSOR *lCursor;
    int iReturn,iStatus,status;
    if(pSmt == NULL) {
        return NULL;
    }
    lCursor=NULL;
    lCursor = dbOpen(pConn);
    if(lCursor == NULL) {
        return NULL;
    }
    iReturn = dbParse(lCursor, pSmt);
    if(iReturn == 0) {
	 if(lCursor){
         	dbClose(lCursor);
	 }
         return NULL;
    }else {
        if(iSumBind > 0) {
            status = _ltDbSetBindValue_o(pConn,lCursor,iSumBind,&ap);
            if(status != OCI_SUCCESS) {
               status = ltDbErrorCode(pConn);
               if(lCursor){
         		dbClose(lCursor);
	       }
               return NULL;
            }
        }
        iReturn = dbExec(lCursor);
        if(iReturn == 0) {
            if(lCursor){
         		dbClose(lCursor);
	     }
            return NULL;
        }
        else {
            return lCursor;
        }
    }
}

/* 打开游标，使用带占位符的参数  */

DB_CURSOR *ltDbOpenCursorB(ltDbConn *pConn,char *pSmt,int iSumBind,...)
{
    va_list ap;
    DB_CURSOR *ps;
    va_start(ap,iSumBind);
    ps = ltDbOpenSql0(pConn,pSmt,iSumBind,ap);
    va_end(ap);
    return ps ;
}


/* 打开游标, 使用带格式参数  */
ltDbCursor *ltDbOpenCursor(ltDbConn *pConn,  char *pSmt,...)
{
    ltDbCursor *tempCursor;
    DB_CURSOR *dbcursor;
    int i;
    va_list ap;
    char caSmt[4096];
    va_start(ap,pSmt);
    vsprintf(caSmt,pSmt,ap);
    va_end(ap);
    
    tempCursor=(ltDbCursor *)malloc(sizeof(ltDbCursor));
    if(tempCursor==NULL){
	return NULL;
    }
    tempCursor->db_row=NULL;
    tempCursor->db_field=NULL;
    tempCursor->num_field=0;
    
    dbcursor=ltDbOpenCursorB(pConn,caSmt,0);
    if(dbcursor!=NULL){
	    tempCursor->db_cursor= dbcursor;
	    tempCursor->num_field=dbcursor->nDef;
	    tempCursor->db_field=(LT_DBFIELD)malloc(tempCursor->num_field*sizeof(char *));
	    for(i = 0; i < tempCursor->num_field; i++)
	    {
	     	   unsigned int namelen;
		   namelen=dbcursor->pDef[i].namel;
         	   tempCursor->db_field[i] =(char *)malloc(namelen+1);
		   memcpy(tempCursor->db_field[i],dbcursor->pDef[i].name,namelen);
		   tempCursor->db_field[i][namelen]=0;
	    }
            return tempCursor;
    }else{
	free(tempCursor);
	return NULL;
    }

}

void ltDbCloseCursor(ltDbCursor *ltCursor)
{
    int iTemp;
    if(ltCursor) {
	/*释放字段名*/
	if(ltCursor->db_field){
		if(ltCursor->num_field>0){
			for(iTemp=0;iTemp<ltCursor->num_field;iTemp++){
				if(ltCursor->db_field[iTemp]){
					free(ltCursor->db_field[iTemp]);
				}
			}
		}
		free(ltCursor->db_field);
		ltCursor->db_field=NULL;
	}
	/*释放字段值*/
	if(ltCursor->db_row){
		if(ltCursor->num_field>0){
			for(iTemp=0;iTemp<ltCursor->num_field;iTemp++){
				if(ltCursor->db_row[iTemp]){
					free(ltCursor->db_row[iTemp]);
				}
			}
		}
		free(ltCursor->db_row);
		ltCursor->db_row=NULL;
	}
	/*释放数据库光标*/
	if(ltCursor->db_cursor){
           	dbClose(ltCursor->db_cursor);
	}
	/*释放指针*/
	free(ltCursor);
    }
}



/*********************/
/**********************/
/*当前光标字段操作*/
/*光标的fetch处理*/
unsigned  int ltNumField(ltDbCursor *ltCursor){
	if(ltCursor){
		return ltCursor->num_field;
	}
	return 0;
}


/*得到字段名*/
LT_DBFIELD  ltDbFetchField(ltDbCursor *ltCursor){
	if(ltCursor==NULL){
		return NULL;
	}
	return ltCursor->db_field;
}


/*fetch并得到字段值*/
LT_DBROW  ltDbFetchRow(ltDbCursor *ltCursor){
    LT_DBROW tmp;
    int iTemp;
    unsigned int fieldnum;
    int iReturn;
    iReturn = dbFetch(ltCursor->db_cursor);
    if(iReturn == 0) {
	 return NULL;
    }
    /*释放字段值*/
    if(ltCursor->db_row){
		if(ltCursor->num_field>0){
			for(iTemp=0;iTemp<ltCursor->num_field;iTemp++){
				if(ltCursor->db_row[iTemp]){
					free(ltCursor->db_row[iTemp]);
				}
			}
		}
		free(ltCursor->db_row);
		ltCursor->db_row=NULL;
    }
	
    fieldnum=ltCursor->db_cursor->nDef;
    tmp=(LT_DBROW)malloc(fieldnum*sizeof(char *));

    for(iReturn=0;iReturn<fieldnum;iReturn++){
	
    	tmp[iReturn] =(char *)malloc(ltCursor->db_cursor->pDef[iReturn].bufl+1);
	if(tmp[iReturn]==NULL){
		return NULL;
	}
	memcpy(tmp[iReturn],ltCursor->db_cursor->pDef[iReturn].buf,ltCursor->db_cursor->pDef[iReturn].bufl);
        tmp[ltCursor->db_cursor->pDef[iReturn].bufl]=0;
   }

   ltCursor->db_row=tmp;	  
   return tmp;
}


int ltDbFetchInto(ltDbCursor *ltCursor,...)
{

    char   *p,*pValue;
    long   *lValue;
    short  *nValue;
    double *dValue;
    char   caTemp[50];
    int i,j,iType,iStart,l,l1,iLen,iReturn;
    
    LT_DBROW  tempRow;
    int tempLen;
    va_list args;
    va_start(args,ltCursor);
    tempRow=ltDbFetchRow(ltCursor);
    if(tempRow==NULL){
	return -1;
    }
    for(j=0;j<ltCursor->num_field;j++) {
        iType = va_arg(args,int);
        iLen  = va_arg(args,int);
        switch(iType) {
            case LT_TYPE_STRING:
                pValue = va_arg(args,char *);
	        tempLen=strlen(tempRow[j]);
                if(iLen < tempLen || iLen==tempLen) {
                    memcpy(pValue,tempRow[j],iLen);
                    for(i=iLen-1;i>=0&&pValue[i]==' ';i--);
                }
                else {
                    memcpy(pValue,tempRow[j],tempLen);
                    for(i=tempLen;i>=0 && pValue[i]==' ';i--);
                }
                pValue[i+1]=0;
                break;
            case LT_TYPE_LONG:
		 tempLen=strlen(tempRow[j]);
                 lValue = va_arg(args,long *);
                 l1 = min(49,tempLen);
                 memcpy(caTemp,tempRow[j],l1);
                 caTemp[l1]=0;
                 *lValue = atol(caTemp);
                 break;
	    case LT_TYPE_LONGLONG:
                 tempLen=strlen(tempRow[j]);
                 lValue = va_arg(args,int64 *);
                 l1 = min(49,tempLen);
                 memcpy(caTemp,tempRow[j],l1);
                 caTemp[l1]=0;
                 *lValue = atoll(caTemp);
                 break;

            case LT_TYPE_SHORT:
		 tempLen=strlen(tempRow[j]);
                 nValue = va_arg(args,short *);
                 l1 = min(tempLen,49);
                 memcpy(caTemp,tempRow[j],l1);
                 caTemp[l1]=0;
                 *nValue = (short)atol(caTemp);
                 break;
            case LT_TYPE_FLOAT:
		 tempLen=strlen(tempRow[j]);
                 dValue = va_arg(args,double *);
                 l1 = min(tempLen,49);
                 memcpy(caTemp,tempRow[j],l1);
                 caTemp[l1]=0;
                 *dValue = (double)atof(caTemp);
                 break;
            default:
		 tempLen=strlen(tempRow[j]);
                 pValue = va_arg(args,char *);
                 l1 = min(iLen,tempLen);
                 memcpy(pValue,tempRow[j],l1);
                 pValue[l1]=0;
                 break;
        }
    }
    va_end(args);
    return  0;
}



/*单条记录操作*/
LT_DBROW ltDbOneRow(ltDbConn *pConn,int *fieldnum,char *pSmt,...)
{
    LT_DBROW tempRow;
    DB_CURSOR *dbcursor;
    int iReturn;
    va_list ap;
    char caSmt[4096];
    int i;

    va_start(ap,pSmt);
    vsprintf(caSmt,pSmt,ap);
    va_end(ap);
    tempRow=NULL;

    dbcursor=ltDbOpenCursorB(pConn,caSmt,0);
    if(dbcursor==NULL){
	return NULL;
    }
    iReturn = dbFetch(dbcursor);
    if(iReturn == 0) {
        dbClose(dbcursor);
        return NULL;
    }
    
    *fieldnum=dbcursor->nDef;
    tempRow=(LT_DBROW)malloc((*fieldnum)*sizeof(char *));
    if(tempRow==NULL){
	 dbClose(dbcursor);
         return NULL;	
    }
    for(iReturn=0;iReturn<(*fieldnum);iReturn++){
    	tempRow[iReturn] =(char *)malloc(dbcursor->pDef[iReturn].bufl+1);
	if(tempRow[iReturn]==NULL){
		dbClose(dbcursor);
		return NULL;
	}
	memcpy(tempRow[iReturn],dbcursor->pDef[iReturn].buf,dbcursor->pDef[iReturn].bufl);
        tempRow[dbcursor->pDef[iReturn].bufl]=0;
   }
   dbClose(dbcursor);
   return tempRow;
}


void ltDbFreeRow(LT_DBROW dbRow,int fieldnum){
	int iTemp;
	if(dbRow){
		if(fieldnum>0){
			for(iTemp=0;iTemp<fieldnum;iTemp++){
				if(dbRow[iTemp]){
					free(dbRow[iTemp]);
				}
			}
		}
		free(dbRow);
		dbRow=NULL;
    }
}





int ltDbOneRowTo(ltDbConn *pConn,char *pSmt,...)
{


    DB_CURSOR *pRes;
    LT_DBROW  tempRow;
    int tempLen;
    int fieldnum;
    va_list args;

    char   *p,*pValue;
    long   *lValue;
    short  *nValue;
    double *dValue;
    char   caTemp[50];
    int j;
    int iType,iLen,i,l1;

    va_start(args,pSmt);
    tempRow=ltDbOneRow(pConn,&fieldnum,pSmt);
    if(tempRow==NULL){
	return -1;
    }
    for(j=0;j<fieldnum;j++) {
        iType = va_arg(args,int);
        iLen  = va_arg(args,int);
        switch(iType) {
            case LT_TYPE_STRING:
                pValue = va_arg(args,char *);
	        tempLen=strlen(tempRow[j]);
                if(iLen < tempLen || iLen==tempLen) {
                    memcpy(pValue,tempRow[j],iLen);
                    for(i=iLen-1;i>=0&&pValue[i]==' ';i--){
			;
		    }
                }
                else {
                    memcpy(pValue,tempRow[j],tempLen);
                    for(i=tempLen;i>=0 && pValue[i]==' ';i--){
			;
		    }
                }
                pValue[i+1]=0;
                break;
            case LT_TYPE_LONG:
		 tempLen=strlen(tempRow[j]);
                 lValue = va_arg(args,long *);
                 l1 = min(49,tempLen);
                 memcpy(caTemp,tempRow[j],l1);
                 caTemp[l1]=0;
                 *lValue = atol(caTemp);
                 break;
            case LT_TYPE_SHORT:
		 tempLen=strlen(tempRow[j]);
                 nValue = va_arg(args,short *);
                 l1 = min(tempLen,49);
                 memcpy(caTemp,tempRow[j],l1);
                 caTemp[l1]=0;
                 *nValue = (short)atol(caTemp);
                 break;
            case LT_TYPE_FLOAT:
		 tempLen=strlen(tempRow[j]);
                 dValue = va_arg(args,double *);
                 l1 = min(tempLen,49);
                 memcpy(caTemp,tempRow[j],l1);
                 caTemp[l1]=0;
                 *dValue = (double)atof(caTemp);
                 break;
            default:
		 tempLen=strlen(tempRow[j]);
                 pValue = va_arg(args,char *);
                 l1 = min(iLen,tempLen);
                 memcpy(pValue,tempRow[j],l1);
                 pValue[l1]=0;
                 break;
        }
    }
    va_end(args);
    ltDbFreeRow(tempRow,fieldnum);
    return  0;
}


/* 执行 SQL语句  */
int ltDbExecSql0(ltDbConn *pConn,char *pSmt,int iSumBind,...)
{
    DB_CURSOR *psCur;
    int status;
    va_list ap;
    psCur = dbOpen(pConn);
    if(psCur == NULL) {
        return -1;
    }
    status = OCIStmtPrepare(psCur->pStmt, psCur->pConn->pErr, pSmt,
        (ub4)strlen(pSmt),
        (ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT);
    if(status != OCI_SUCCESS) {
        status = ltDbErrorCode(pConn);
        dbClose(psCur);
        return status;
    }
    va_start(ap,iSumBind);
    status = _ltDbSetBindValue_o(pConn,psCur,iSumBind,&ap);
    va_end(ap);
    if(status != 0) {
        status = ltDbErrorCode(pConn);
        dbClose(psCur);
        return status;
    }
    
    status = OCIStmtExecute(psCur->pConn->pSvc, psCur->pStmt, psCur->pConn->pErr,
        1, (ub4)0, (OCISnapshot*)0, (OCISnapshot*)0, OCI_DEFAULT);
    if(status == OCI_SUCCESS) {
    /* 网文件中写日志  */
       
        dbClose(psCur);
        return 0;
    }
    else {
        status = ltDbErrorCode(pConn);
        dbClose(psCur);
        return status;
    }
}


/*更新操作*/
int ltDbExecSql(ltDbConn *pConn,  char *pSmt,...){
	va_list ap;
	char caSmt[4096];
	va_start(ap,pSmt);
	vsprintf(caSmt,pSmt,ap);
	va_end(ap);
	return ltDbExecSql0(pConn,caSmt,0);
}

unsigned int ltDbAffectRows(ltDbConn *pConn){
	return 0;
}

/*事务操作*/
int ltDbCommit(ltDbConn *pConn)
{
     return dbCommit(pConn);
}

int ltDbRollback(ltDbConn *pConn)
{
    return dbRollback(pConn);
}

/*错误处理*/
unsigned int  ltDbErrorCode(ltDbConn *pConn){
    ub4 errcode;
    text msgBuf[512];
    OCIErrorGet(pConn->pErr, (ub4) 1, (text *) NULL, &errcode,
                    msgBuf, (ub4) sizeof(msgBuf), OCI_HTYPE_ERROR);
    return errcode;
}

char *ltDbError(ltDbConn *pConn){
    ub4 errcode;
    static char msgBuf[512];
    OCIErrorGet(pConn->pErr, (ub4) 1, (text *) NULL, &errcode,
                    msgBuf, (ub4) sizeof(msgBuf), OCI_HTYPE_ERROR);
    return msgBuf;
}

