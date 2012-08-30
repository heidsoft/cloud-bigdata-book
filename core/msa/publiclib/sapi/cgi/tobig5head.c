#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "lt.h"
#include "ltcgi.h"

main(int argc,char *argv[])
{
    struct stat buf;
    char *pTemp;
    char *p;
    int iFd1,iReturn;
    int iRead,l;
    int i;

    p = getenv("PATH_TRANSLATED");
    printf("Content-type: text/html; charset=big5\n\n");
    if(p == NULL) {
        printf(" 很抱歉，页面不存在 \n");
        exit(0);
    }

    iFd1 = open(p,O_RDONLY);
    if(iFd1 == (-1)) {
        printf(" File %s Open Error \n",argv[1]);
        exit(0);
    }
    fstat(iFd1,&buf);
    pTemp = malloc(buf.st_size + 1);
    iRead=read(iFd1,pTemp,buf.st_size);
    pTemp[buf.st_size] = 0;
    close(iFd1);
    l = 0;
   
    while(l < buf.st_size) {
       iRead = printf("%s",pTemp + l);
       if(iRead == 0) iRead = 1;
       l = l + iRead;
    }
    printf("\n");
    free(pTemp);
    exit(0);
}   
    
    
    
   
