#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <unistd.h>
#include "ltpublic.h"
#include "ltplt.h"

int main(){

char *p;
char xml[1000];
int iFd;
int iReturn;
struct stat  sStatBuf;
xmlDocPtr ltDocPtr;


memset(xml,0,1000);
 iFd = open("/app/ltServer/ltPlt/sample/test.xml",O_RDONLY);
 if(iFd < 0) {
      return 0;
 }
    
  iReturn = fstat(iFd,&sStatBuf);
    if(iReturn == (-1)) {
         close(iFd);
        return 0;
    }
   iReturn = read(iFd,xml,sStatBuf.st_size);
    close(iFd);
    
		   
ltDocPtr=xmlParseMemory(xml,strlen(xml));

p=ltPltFileParse("/app/ltServer/ltPlt/sample/test.xp",ltDocPtr);
printf("xp:\n%s\n",p);
free(p);
xmlFreeDoc(ltDocPtr);
return 0;

}
