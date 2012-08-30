#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "lttype.h"
#include "dbmysql.h"
#include "ltdb.h"


ltDbConn *ltDbConnect(char *pUser,char *pPassword,char *pService){
	MYSQL *       tempHandle;
	char caHostname[128];
  char caDbname[128];
  if(pService == NULL) {
       	 	strcpy(caHostname,"127.0.0.1");
        	strcpy(caDbname,pUser);
  }else {
        	char *p;
        	p = strstr(pService,"@");
        	if(*p == '@') {
           		 strcpy(caHostname,p+1);
           		 memset(caDbname,0,sizeof(caDbname));
            		memcpy(caDbname,pService,p-pService);
        	}
  }
  tempHandle = mysql_init(NULL);
	if(tempHandle == NULL) {
		return NULL;
	}
  if(mysql_real_connect(tempHandle,
                          caHostname,
                          pUser,
                          pPassword,
                          caDbname,
                          0,
                          NULL,
                          0) == NULL) {
      
       		return NULL;
   }
   return tempHandle;	

   	
}

void  ltDbClose(ltDbConn *psConn){
	mysql_close(psConn); 
}


/* 打开游标, 使用带格式参数  */
/*ltDbCursor *ltDbOpenCursor(ltDbConn *pConn,  char *pSmt,...)
{
    ltDbCursor *tempCursor;
    va_list ap;
    char caSmt[4096];
    int i;
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
    if(mysql_query(pConn, caSmt) == 0){
	    MYSQL_FIELD *fields;
	    tempCursor->db_cursor= mysql_use_result(pConn);
	    tempCursor->num_field=mysql_num_fields(tempCursor->db_cursor);
	    tempCursor->db_field=(LT_DBFIELD)malloc(tempCursor->num_field*sizeof(char *));
	    fields = mysql_fetch_fields(tempCursor->db_cursor);
	    for(i = 0; i < tempCursor->num_field; i++)
	    {
	     	   unsigned int namelen;
		   		 namelen=strlen(fields[i].name);
         	 tempCursor->db_field[i] =(char *)malloc(namelen+1);
		   		 memcpy(tempCursor->db_field[i],fields[i].name,namelen);
		   		 tempCursor->db_field[i][namelen]=0;
	    }
      return tempCursor;
    }else{
			free(tempCursor);
			return NULL;
    }
}
*/



ltDbCursor *ltDbOpenCursor(ltDbConn *pConn,  char *caSmt)
{
    ltDbCursor *tempCursor;
    int i;
    tempCursor=(ltDbCursor *)malloc(sizeof(ltDbCursor));
    if(tempCursor==NULL){
			return NULL;
    }
    tempCursor->db_row=NULL;
    tempCursor->db_field=NULL;
    tempCursor->num_field=0;
    if(mysql_query(pConn, caSmt) == 0){
	    MYSQL_FIELD *fields;
	    tempCursor->db_cursor= mysql_use_result(pConn);
	    tempCursor->num_field=mysql_num_fields(tempCursor->db_cursor);
	    tempCursor->db_field=(LT_DBFIELD)malloc(tempCursor->num_field*sizeof(char *));
	    fields = mysql_fetch_fields(tempCursor->db_cursor);
	    for(i = 0; i < tempCursor->num_field; i++)
	    {
	     	  unsigned int namelen;
		   		namelen=strlen(fields[i].name);
         	tempCursor->db_field[i] =(char *)malloc(namelen+1);
		   		memcpy(tempCursor->db_field[i],fields[i].name,namelen);
		   		tempCursor->db_field[i][namelen]=0;
	    }
      return tempCursor;
    }else{
				free(tempCursor);
				return NULL;
    }
}

ltDbCursor *ltDbOpenCursorS(ltDbConn *pConn,  char *caSmt)
{
	return ltDbOpenCursor(pConn, caSmt);
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
			/*释放数据库光标*/
			if(ltCursor->db_cursor){
		      mysql_free_result(ltCursor->db_cursor);
			}
			/*释放指针*/
			free(ltCursor);
    }
}



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
	
	if(ltCursor==NULL){
		return NULL;
	}
	ltCursor->db_row=(LT_DBROW)mysql_fetch_row((MYSQL_RES *)ltCursor->db_cursor);
	return ltCursor->db_row;
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
                 lValue = va_arg(args,long long *);
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
LT_DBROW ltDbOneRow(ltDbConn *pConn,int *fieldnum,char *caSmt)
{
    LT_DBROW tempRow1;
    LT_DBROW tempRow;
    int temLen;
    DB_CURSOR *pRes;
    int iReturn;
    va_list ap;
    //char caSmt[4096];
    int i;
    //va_start(ap,pSmt);
    //vsprintf(caSmt,pSmt,ap);
    //va_end(ap);
    tempRow1=NULL;
    tempRow=NULL;
    if(mysql_query(pConn, caSmt) == 0){
	   pRes= mysql_use_result(pConn);
     tempRow1 = mysql_fetch_row(pRes);
	   *fieldnum=mysql_num_fields(pRes);
	   tempRow=(LT_DBROW)malloc((*fieldnum)*sizeof(char *));
     if(tempRow==NULL){
								mysql_free_result(pRes);
                return NULL;
     }
	   for(iReturn=0;iReturn<*fieldnum;iReturn++){
				temLen=strlen(tempRow1[iReturn]);
	    	tempRow[iReturn] =(char *)malloc(temLen+1);
				if(tempRow[iReturn]==NULL){
					free(tempRow);
					mysql_free_result(pRes);
					return NULL;
				}
				memcpy(tempRow[iReturn],tempRow1[iReturn],temLen);
	      tempRow[temLen]=0;
	   }

	   	mysql_free_result(pRes);	
    }
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
    int i,j,iType,iStart,l,l1,iLen,iReturn;

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
                 lValue = va_arg(args,long long *);
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
    ltDbFreeRow(tempRow,fieldnum);
    return  0;
}




/*更新操作*/
/*int ltDbExecSql(ltDbConn *pConn,  char *pSmt,...){
	va_list ap;
	char caSmt[4096];
	va_start(ap,pSmt);
	vsprintf(caSmt,pSmt,ap);
	va_end(ap);
	return mysql_query(pConn, caSmt) ;
}*/
int ltDbExecSql(ltDbConn *pConn,  char *pSmt){

	return mysql_query(pConn, pSmt) ;
}
int ltDbExecSqlS(ltDbConn *pConn,  char *pSmt){

	return mysql_query(pConn, pSmt) ;
}

unsigned int ltDbAffectRows(ltDbConn *pConn){
	return mysql_affected_rows(pConn);
}

/*事务操作*/
int ltDbCommit(ltDbConn *pConn)
{
	return 0;
}

int ltDbRollback(ltDbConn *pConn)
{

    return 0;
}



/*错误处理*/
unsigned int  ltDbErrorCode(ltDbConn *pConn){
	return mysql_errno(pConn) ;
}

char *ltDbError(ltDbConn *pConn){
	return mysql_error(pConn); 
}
