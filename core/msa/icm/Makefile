prefix = /app/msa/
bindir = /app/msa/bin
libdir = /app/msa/icm/lib
srcdir = /app/msa/icm
sqldir = /app/sqldb/include
publicdir = /app/msa/publiclib

CFLAGS:= -O2 -Wall -Wunused  
CFLAGS +=  -I/usr/include 
CFLAGS +=  -I/app/msa/icm/include 
CFLAGS +=  -I/app/msa/publiclib/include 
CFLAGS +=  -I${sqldir}
CFLAGS +=  -I/usr/include/libxml2/ 
CFLAGS += -DNO_SHARED_LIBS=1

all: ${bindir}/icmLocalServer
	
clean: 
	rm -f ${bindir}/icmLocalServer 


${bindir}/icmLocalServer: ${srcdir}/mainproc/icmLocalServer.c
	gcc   $(CFLAGS) -o ${bindir}/icmLocalServer ${srcdir}/mainproc/icmLocalServer.c \
	${libdir}/srvlib.a ${libdir}/mainlib.a  ${prefix}/publiclib/lib/publib.a \
	-L /app/sqldb/lib -lpq    \
	-lm  -lpthread  -lfreetype -lm -lgd -ljpeg -lxml2  -liconv  -lssl -lcrypto	

