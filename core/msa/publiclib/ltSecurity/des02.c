#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include "syslog.h"
#define DesStepBytes   1024
int ltDesEncryptFile(char *pInFile,char *pOutFile,char *pKey)
{
    int  iFd,iFdo,iReturn,iFileSize,iTotalBytes;
    struct stat  sStatBuf;
    char caBuffer[1056];
    char caOutBuf[1056];
    char caMd5[16];
    int  iLen,iWrite;
    memset(caMd5,0,16);
    iFd = open(pInFile,O_RDONLY);
    if(iFd < 0) {
        //ltSysLog(" File %s Open Error ",pInFile);
        return (-1);
    }
    iReturn = fstat(iFd,&sStatBuf);
    if(iReturn == (-1)) {
        //ltSysLog(" fStat Error ");
        close(iFd);
        return (-1);
    }
    iFileSize = sStatBuf.st_size;
    iTotalBytes = iFileSize;
    iFdo = open(pOutFile, O_WRONLY | O_CREAT | O_TRUNC, sStatBuf.st_mode);
    if(iFdo < 0) {
        //ltSysLog(" File %s open error \n",pOutFile);
        close(iFd);
    }
    iReturn = 1;
    write(iFdo,caMd5,16);
    write(iFdo,&iFileSize,sizeof(long));
    while(iReturn > 0 && iTotalBytes > 0) {
        iReturn = read(iFd,caBuffer,DesStepBytes);
        if(iReturn > 0) {
            iTotalBytes = iTotalBytes - iReturn;
            iLen = ltDesEncrypt(caBuffer,iReturn,caOutBuf,pKey);
            iWrite = write(iFdo,caOutBuf,iLen);
            if(iWrite != iLen) {
                //ltSysLog(" File %s write Error ",pOutFile);
                close(iFd);
                close(iFdo);
                return (-1);
            }
            memcpy(caBuffer+iReturn,caMd5,16);
            ltMd5(caBuffer,iReturn+16,caMd5);
        }
    }
    lseek(iFdo,0,SEEK_SET);
    write(iFdo,caMd5,16);
    close(iFd);
    close(iFdo);
}

/* Return 0--正确   1--文件不正确  */
int ltDesDencryptFile(char *pInFile,char *pOutFile,char *pKey)
{
    int  iFd,iFdo,iTotalBytes,iReturn,iFileSize;
    struct stat  sStatBuf;
    int iReadBytes;
    int iFileSize0,iSum;
    char caBuffer[1056];
    char caOutBuf[1056];
    char caMd5[16];
    char caMd51[16];
    int  iLen,iWrite;
    memset(caMd5,0,16);
    iFd = open(pInFile,O_RDONLY);
    if(iFd < 0) {
        //ltSysLog(" File %s Open Error ",pInFile);
        return (-1);
    }
    iReturn = fstat(iFd,&sStatBuf);
    if(iReturn == (-1)) {
        //ltSysLog(" fStat Error ");
        close(iFd);
        return (-1);
    }
    iFdo = open(pOutFile, O_WRONLY | O_CREAT | O_TRUNC, sStatBuf.st_mode);
    if(iFdo < 0) {
        //ltSysLog(" File %s open error \n",pOutFile);
        close(iFd);
    }
    iReturn = 1;
    read(iFd,caMd51,16);
    read(iFd,&iFileSize0,sizeof(long));
    iFileSize = sStatBuf.st_size;
    iTotalBytes = iFileSize-20;
    iSum = 0;
    while(iReturn > 0 && iTotalBytes > 0) {
        iReturn = read(iFd,caBuffer,DesStepBytes);
        if(iReturn > 0) {
            iTotalBytes = iTotalBytes - iReturn;
            iLen = ltDesDecrypt(caBuffer,iReturn,caOutBuf,pKey);
            if(iSum > iFileSize0 - iLen) {
                iLen = iFileSize0 - iSum;
            }
            iWrite = write(iFdo,caOutBuf,iLen);
            if(iWrite != iLen) {
                //ltSysLog(" File %s write Error ",pOutFile);
                close(iFd);
                close(iFdo);
                return (-1);
            }
            iSum = iSum + iWrite;
            memcpy(caOutBuf+iLen,caMd5,16);
            ltMd5(caOutBuf,iLen+16,caMd5);
        }
    }
    close(iFd);
    close(iFdo);
    if(memcmp(caMd51,caMd5,16) == 0) {
        return 0;
    }
    else {
        return 1;
    }
}

