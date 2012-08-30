#ifndef LTFILE_H
#define LTFILE_H

/*得到一个文件的大小*/
long ltFileSize(char *pFile);
/*判断一个文件是否存在 存在返回1 不存返回-1*/
int ltFileIsExist(char *pFile);
/*文件栲陪*/
int  ltFileCopy(char *pSfile,char *pDfile);

/* 删除指定文件夹的内容  */
int ltDeleteDir(char *pDir,
                 unsigned long lStartTime,
                 unsigned long lStopTime);

/* 当当前目录下的文件全部删除后，将当前目录也删除  */
int ltDeleteDir_p(char *pDir,
                 unsigned long lStartTime,
                 unsigned long lStopTime);


/* 删除指定文件夹的内容  */
int ltRmDir(char *pDir);
/* 根据设备名取剩余磁盘空间  包含*/
int ltFsGetDiskSpaceD(char *pDev,long *lTotal,long *lUsed,long *lFree);

/*相等*/
int ltFsGetDiskSpaceF(char *pMount,long *lTotal,long *lUsed,long *lFree);


/* 将文件打包成一个 tar 格式  
   参数: pTarFile ---- 生成的文件, 建议以 .tar结尾
         pFlst    ---- 文件列表, 可以为多个文件，文件名之间以空格分隔
   返回值:
         0 --- 正确
         其它---错误
*/
int ltTarTxtFile(char *pTarFile,char *pFlst);
/* 将 Dos的文本文件格式转换为UNIX的文本文件格式  */
int ltTxtDos2Unix(char *pDosFile,char *pUnixFile);

/* 将UNIX格式的文本文件转换为Dos格式的文本文件 */
int ltTxtUnix2Dos(char *pUnixFile,char *pDosFile);
/* 将文件打包成一个 tar 格式  
   参数: pTarFile ---- 生成的文件, 建议以 .tar结尾
         pFlst    ---- 文件列表, 可以为多个文件，文件名之间以空格分隔
   返回值:
         0 --- 正确
         其它---错误
*/
int ltTarDirFile(char *pTarFile,char *pPath,char *pFlst);

/* 分离上传的文件名  */
int ltStrGetUploadName(char *pIn,char *pPath,char *pFile);
#endif


