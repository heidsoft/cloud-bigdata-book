prefix = /app/msa/
bindir = /app/msa/bin
libdir = /app/msa/msas/lib
srcdir = /app/msa/msas
sqldir = /app/sqldb/include
publicdir = /app/msa/publiclib

CFLAGS:= -O2 -Wall -Wunused  
CFLAGS +=  -I/usr/include 
CFLAGS +=  -I/app/msa/msas/include 
CFLAGS +=  -I/app/msa/publiclib/include 
CFLAGS +=  -I${sqldir}
CFLAGS +=  -I/usr/include/libxml2/ 
CFLAGS += -DNO_SHARED_LIBS=1

all: ${bindir}/msasMainProc 	\
  ${bindir}/msasLocalServer 
	
clean: 
	rm -f ${bindir}/msasMainProc 
	rm -f ${bindir}/msasLocalServer 
	rm -f ${bindir}/msasShutDown 
	rm -f ${bindir}/msasComServer
	rm -f ${bindir}/msasSqlProc
	rm -f ${bindir}/msasTimeUpdate

${bindir}/msasMainProc: ${srcdir}/mainproc/msasMainProc.c ${libdir}/srvlib.a ${libdir}/mainlib.a
	gcc   $(CFLAGS) -o ${bindir}/msasMainProc ${srcdir}/mainproc/msasMainProc.c \
	${libdir}/mainlib.a ${prefix}/publiclib/lib/publib.a \
	-L /app/sqldb/lib -lpq \
	-lm  -lpthread  -lfreetype -lm -lgd -ljpeg -lxml2  -liconv  -lssl -lcrypto

${bindir}/msasLocalServer: ${srcdir}/mainproc/msasLocalServer.c
	gcc   $(CFLAGS) -o ${bindir}/msasLocalServer ${srcdir}/mainproc/msasLocalServer.c \
	${libdir}/srvlib.a ${libdir}/mainlib.a  ${prefix}/publiclib/lib/publib.a \
	-L /app/sqldb/lib -lpq    \
	-lm  -lpthread  -lfreetype -lm -lgd -ljpeg -lxml2  -liconv  -lssl -lcrypto	

${bindir}/msasComServer: ${srcdir}/mainproc/msasComServer.c
	gcc   $(CFLAGS) -o ${bindir}/msasComServer ${srcdir}/mainproc/msasComServer.c \
	${libdir}/mainlib.a ${libdir}/srvlib.a  ${prefix}/publiclib/lib/publib.a \
	-L /app/sqldb/lib -lpq    \
	-lm  -lpthread  -lfreetype -lm -lgd -ljpeg -lxml2  -liconv  -lssl -lcrypto	

${bindir}/msasTimeUpdate: ${srcdir}/mainproc/msasTimeUpdate.c
	gcc   $(CFLAGS) -o ${bindir}/msasTimeUpdate ${srcdir}/mainproc/msasTimeUpdate.c \
	${libdir}/mainlib.a ${libdir}/srvlib.a  ${prefix}/publiclib/lib/publib.a \
	-L /app/sqldb/lib -lpq    \
	-lm  -lpthread  -lfreetype -lm -lgd -ljpeg -lxml2  -liconv  -lssl -lcrypto	

${bindir}/msasShutDown: ${srcdir}/mainproc/msasShutDown.c
	gcc   $(CFLAGS) -o ${bindir}/msasShutDown ${srcdir}/mainproc/msasShutDown.c \
	${prefix}/publiclib/lib/publib.a 


${bindir}/msasSqlProc: ${srcdir}/mainproc/msasSqlProc.c
	gcc   $(CFLAGS) -o ${bindir}/msasSqlProc ${srcdir}/mainproc/msasSqlProc.c \
	${libdir}/mainlib.a ${libdir}/srvlib.a  ${prefix}/publiclib/lib/publib.a \
	-L /app/sqldb/lib -lpq    \
	-lm  -lpthread  -lfreetype -lm -lgd -ljpeg -lxml2  -liconv  -lssl -lcrypto	

${bindir}/inportsql: ${srcdir}/mainproc/inportsql.c
	gcc   $(CFLAGS) -o ${bindir}/inportsql ${srcdir}/mainproc/inportsql.c \
	${libdir}/mainlib.a ${libdir}/srvlib.a  ${prefix}/publiclib/lib/publib.a \
	-L /app/sqldb/lib -lpq    \
	-lm  -lpthread  -lfreetype -lm -lgd -ljpeg -lxml2  -liconv  -lssl -lcrypto	
