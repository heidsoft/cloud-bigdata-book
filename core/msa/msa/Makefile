prefix = /app/msa/
bindir = /app/msa/bin
libdir = /app/msa/msa/lib
srcdir = /app/msa/msa
sqldir = /app/sqldb/include
publicdir = /app/msa/publiclib

CFLAGS:= -O2 -Wall -Wunused  
CFLAGS +=  -I/usr/include 
CFLAGS +=  -I/app/msa/msa/include 
CFLAGS +=  -I/app/msa/publiclib/include 
CFLAGS +=  -I${sqldir}
CFLAGS +=  -I/usr/include/libxml2/ 
CFLAGS += -DNO_SHARED_LIBS=1

all: ${bindir}/msaLocalServer
	
	
clean: 
	rm -f ${bindir}/msaLocalServer 

	

${bindir}/msaLocalServer: ${srcdir}/mainproc/msaLocalServer.c
	gcc   $(CFLAGS) -o ${bindir}/msaLocalServer ${srcdir}/mainproc/msaLocalServer.c \
	${libdir}/srvlib.a ${libdir}/mainlib.a  ${prefix}/publiclib/lib/publib.a \
	-L /app/sqldb/lib -lpq    \
	-lm  -lpthread  -lm -lgd -lxml2  -liconv  -lssl -lldap

