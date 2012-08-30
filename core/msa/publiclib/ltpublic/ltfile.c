/* 有关文件操作的函数  
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include "ltfile.h"



long ltFileSize(char *pFile)
{
    struct stat buf;
    int iReturn;
    iReturn = stat(pFile, &buf);
    if(iReturn < 0) return iReturn;
    else {
        return buf.st_size;
    }
}


int ltFileIsExist(char *pFile)
{
    int iReturn;
    iReturn = access(pFile,F_OK);
    if(iReturn == 0) {
        return 1;
    }
    else {
        return (-1);
    }
}


/* 文件复制  */
int  ltFileCopy(char *pSfile,char *pDfile)
{
	struct stat stStat;
    int iReturn;

    iReturn = stat(pSfile, &stStat);
	if(iReturn != (-1) && S_ISREG(stStat.st_mode)) {
	    int srcfd, destfd;
		char buf[4096];
		int count;

		srcfd = open(pSfile, O_RDONLY);
		if(srcfd == (-1)) {
		    return (-1);
		}
		destfd = open(pDfile, O_WRONLY | O_CREAT | O_TRUNC, stStat.st_mode);
		if(destfd == (-1)) {
		    close(srcfd);
		    return (-1);
		}
		/* 复制文件 */
		while( (count = read(srcfd, buf, 4096)) > 0) 
			write(destfd, buf, count);
			/* 复制完成，关闭文件 */
		close(srcfd);
		close(destfd);
		return 0;
	}
	else {
	    return (-1);
	}
}

/* 删除指定文件夹的内容  */
int ltDeleteDir(char *pDir,
                 unsigned long lStartTime,
                 unsigned long lStopTime)
{
    char    caFileName[256];
    struct  stat st,st1;
   	struct dirent *d;
    DIR *dir;
    int iCount;
    int iReturn;
    iCount=0;
  	iReturn=stat(pDir,&st);
    if(iReturn !=0) { /* 目录不存在  */
        return 0;
    }
    if(S_ISDIR(st.st_mode) && (dir = opendir(pDir))) {
        while (d = readdir(dir)) {
		    if (d->d_name[0] == '.') continue;
			sprintf(caFileName, "%s/%s", pDir, d->d_name);
			if (stat(caFileName,&st1) == 0) {
			    if(S_ISREG(st1.st_mode)) {
			        if(st1.st_mtime <= lStopTime  &&
			                    st1.st_mtime >= lStartTime) {
			            iReturn = remove(caFileName);
			            iCount++;
			        }
			    }
			    else if(S_ISDIR(st1.st_mode)) {
			        iCount = iCount + ltDeleteDir(caFileName,lStartTime,lStopTime);
			        
      			}
			}
        }
        closedir(dir);
    }
    return iCount;
}


/* 当当前目录下的文件全部删除后，将当前目录也删除  */
int ltDeleteDir_p(char *pDir,
                 unsigned long lStartTime,
                 unsigned long lStopTime)
{
    char    caFileName[256];
    struct  stat st,st1;
   	struct dirent *d;
    DIR *dir;
    int iCount,iCount0;
    int iReturn;
    iCount=0;
    iCount0 = 0;
  	iReturn=stat(pDir,&st);
    if(iReturn !=0) { /* 目录不存在  */
        return 0;
    }
    if(S_ISDIR(st.st_mode) && (dir = opendir(pDir))) {
        while (d = readdir(dir)) {
		    if (d->d_name[0] == '.') continue;
			sprintf(caFileName, "%s/%s", pDir, d->d_name);
			if (stat(caFileName,&st1) == 0) {
			    if(S_ISREG(st1.st_mode)) {
			        if(st1.st_mtime <= lStopTime  &&
			                    st1.st_mtime >= lStartTime) {
			            iReturn = remove(caFileName);
			            iCount++;
			        }
			    }
			    else if(S_ISDIR(st1.st_mode)) {
			        iReturn = ltDeleteDir_p(caFileName,lStartTime,lStopTime);
			        if(iReturn == 1) {
			            rmdir(caFileName);
			            iCount++;
			        }
      			}
      			iCount0++;
			}
        }
        closedir(dir);
    }
    if(iCount == iCount0) {
        return 1;
    }
    else {
        return 0;
    }
}

/* 删除指定文件夹的内容  */
int ltRmDir(char *pDir)
{
    char    caFileName[256];
    struct  stat st,st1;
   	struct dirent *d;
    DIR *dir;
    int iReturn;

  	iReturn=stat(pDir,&st);
    if(iReturn !=0) { /* 目录不存在  */
        return 0;
    }
    if(S_ISDIR(st.st_mode) && (dir = opendir(pDir))) {
        while (d = readdir(dir)) {
		    if (d->d_name[0] == '.') continue;
			sprintf(caFileName, "%s/%s", pDir, d->d_name);
			if (stat(caFileName,&st1) == 0) {
			    if(S_ISREG(st1.st_mode)) {
 	                iReturn = remove(caFileName);
			    }
			    else if(S_ISDIR(st1.st_mode)) {
			        iReturn = ltRmDir(caFileName);
      			}
			}
        }
       closedir(dir);
    }
    rmdir(pDir);
    return 0;
}

/* 根据设备名取剩余磁盘空间  */
int ltFsGetDiskSpaceD(char *pDev,long *lTotal,long *lUsed,long *lFree)
{
    FILE *fp;
    char *p;
    char caBuffer[132];
    char caDev[32];
    char caRate[32];
    char caMount[32];
    fp = popen("df -k","r");
    if(fp) {
        p = fgets(caBuffer,131,fp);
        p = fgets(caBuffer,131,fp);
        while(p) {
            sscanf(caBuffer,"%s %d %d %d %s %s",
                caDev,lTotal,lUsed,lFree,caRate,caMount);
            if(p=strstr(caDev,pDev)) {
                pclose(fp);
                return 0;
            }
            p = fgets(caBuffer,132,fp);
        }
        pclose(fp);
    }
    return (-1);
}



int ltFsGetDiskSpaceF(char *pMount,long *lTotal,long *lUsed,long *lFree)
{
    FILE *fp;
    char *p;
    char caBuffer[132];
    char caDev[32];
    char caRate[32];
    char caMount[32];
    fp = popen("df -k","r");
    if(fp) {
        p = fgets(caBuffer,131,fp);
        p = fgets(caBuffer,131,fp);
        while(p) {
            sscanf(caBuffer,"%s %d %d %d %s %s",
                caDev,lTotal,lUsed,lFree,caRate,caMount);
            if(strcmp(caMount,pMount)==0) {
                pclose(fp);
                return 0;
            }
            p = fgets(caBuffer,132,fp);
        }
        pclose(fp);
    }
    return (-1);
}



/* 将文件打包成一个 tar 格式  
   参数: pTarFile ---- 生成的文件, 建议以 .tar结尾
         pFlst    ---- 文件列表, 可以为多个文件，文件名之间以空格分隔
   返回值:
         0 --- 正确
         其它---错误
*/
int ltTarTxtFile(char *pTarFile,char *pFlst)
{
    int iReturn;
    char *pCommand;   
    pCommand = malloc(strlen(pTarFile) + strlen(pFlst) + 32);
    if(pCommand == NULL) {
        return (-1);
    }
    sprintf(pCommand,"tar cf %s  %s",pTarFile,pFlst);
    iReturn = system(pCommand);
    free(pCommand);
    return iReturn;
}

/* 将 Dos的文本文件格式转换为UNIX的文本文件格式  */
int ltTxtDos2Unix(char *pDosFile,char *pUnixFile)
{
    FILE *fp;
    FILE *fpo;
    int iChar;
    fp = fopen(pDosFile,"r+b");
    if(fp == NULL) {
        return (-1);
    }
    fpo = fopen(pUnixFile,"w");
    if(fpo == NULL) {
        return (-1);
    }
    while( (iChar = fgetc(fp)) != EOF) {
        if(iChar != 13) {
            fputc(iChar,fpo);
        }
    }
    fclose(fpo); 
    fclose(fp);
    return 0;
}           

/* 将UNIX格式的文本文件转换为Dos格式的文本文件 */
int ltTxtUnix2Dos(char *pUnixFile,char *pDosFile)
{
    FILE *fp;
    FILE *fpo;
    int iChar;
    fp = fopen(pUnixFile,"r+b");
    if(fp == NULL) {
        return (-1);
    }
    fpo = fopen(pDosFile,"w");
    if(fpo == NULL) {
        return (-1);
    }
    while( (iChar = fgetc(fp)) != EOF) {
        if(iChar != 13) {
            if(iChar == 10) {
                fputc(13,fpo);
                fputc(iChar,fpo);
            }
            else {
                fputc(iChar,fpo);
            }
        }
    }
    fclose(fpo); 
    fclose(fp);
    return 0;
}  
         
/* 将文件打包成一个 tar 格式  
   参数: pTarFile ---- 生成的文件, 建议以 .tar结尾
         pFlst    ---- 文件列表, 可以为多个文件，文件名之间以空格分隔
   返回值:
         0 --- 正确
         其它---错误
*/
int ltTarDirFile(char *pTarFile,char *pPath,char *pFlst)
{
    int iReturn;
    char *pCommand;   
    pCommand = malloc(strlen(pTarFile) + strlen(pPath) + strlen(pFlst) + 32);
    if(pCommand == NULL) {
        return (-1);
    }
    sprintf(pCommand,"tar cf %s -C %s  %s",pTarFile,pPath,pFlst);
    iReturn = system(pCommand);
    free(pCommand);
    return iReturn;
}


/* 分离文件名  */
int ltStrGetUploadName(char *pIn,char *pPath,char *pFile)
{
    char *p,*p1;
    register int i,l;
    p = strstr(pIn,";");
    if(p&&*p == ';') {
        p1 = p+1;
        p = strstr(p+1,";");
        if(p&&*p == ';') {
            l = p - p1;
            if(l > 0) {
                p--;
                for(i=l;i>0 && *p!='/';p--,i--);
                if(i>0) {
                    if(pPath) {
                        memcpy(pPath,p1,i);
                        pPath[i] = 0;
                    }
                    memcpy(pFile,p+1,l-i);
                    pFile[l-i] = 0;
                    return 0;
                }
                else {
                    if(pPath) {
                        pPath[0]=0;
                    }
                    memcpy(pFile,p+1,l);
                    pFile[l] = 0;
                    return 0;
                }
            }
            else {
                if(pPath) {
                    pPath[0]=0;
                }
                pFile[0]=0;
                return 0;
            }
        }
    }
    if(pPath) {
        pPath[0]=0;
    }
    pFile[0]=0;
    return (-1);
}
