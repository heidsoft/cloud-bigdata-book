#include <stdio.h>
#include <stdlib.h>
#include "dbmysql.h"
#include "ltdb.h"


main(){
	ltDbConn *tempCon;
	tempCon=ltDbConnect("nas","naspass","nas@localhost");
	if(tempCon!=NULL){
		printf("ok\n");
	}
	



}

