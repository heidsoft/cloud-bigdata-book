#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <unistd.h>
#include "ltpublic.h"
#include "ltplt.h"

int main(){
	ltDbHeadPtr dbPtr;
	int iReturn;
	ltTablePtr tablePtr;

	dbPtr=lt_dbinit();
	lt_db_htmlpage(dbPtr,"gb2312");

	tablePtr=lt_dbput_table(dbPtr,"xxx");
	lt_dbput_recordvars(tablePtr,1,"xxx","111");
	lt_db_print(dbPtr);


	printf("xxxxxxxxxx\n");
	lt_dbfree(dbPtr);
	printf("xxxxxxxxxx\n");
	return 0;

}
