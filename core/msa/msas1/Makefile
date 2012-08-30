prefix = /app/msa/
bindir = /app/msa/bin
libdir = /app/msa/msas1/lib
srcdir = /app/msa/msas1
sqldir = /app/sqldb/include
publicdir = /app/msa/publiclib

CFLAGS:= -O2 -nostartfiles   -Wall -Wunused  
CFLAGS +=  -I/usr/include 
CFLAGS +=  -I/app/msa/msas1/include 
CFLAGS +=  -I/app/msa/publiclib/include 
CFLAGS +=  -I${sqldir}
CFLAGS +=  -I/usr/include/libxml2/ 
CFLAGS += -DNO_SHARED_LIBS=1

all: ${bindir}/msasMainProc1 	\
	${bindir}/msasLocalServer1 \
	${bindir}/msasShutDown1 
	
clean: 
	rm -f ${bindir}/msasMainProc1 
	rm -f ${bindir}/msasLocalServer1 
	rm -f ${bindir}/msasShutDown1 

${bindir}/msasMainProc1: ${srcdir}/mainproc/msasMainProc.c ${libdir}/srvlib.a ${libdir}/mainlib.a
	gcc   $(CFLAGS) -o ${bindir}/msasMainProc1 ${srcdir}/mainproc/msasMainProc.c \
	/usr/lib/crt1.o /usr/lib/crti.o /usr/lib/crtn.o \
	${libdir}/mainlib.a ${prefix}/publiclib/lib/publib.a \
	-L /app/sqldb/lib -lpq \
	-lm  -lpthread  -lfreetype -lm -lgd -ljpeg -lxml2  -liconv  -lssl -lcrypto

${bindir}/msasLocalServer1: ${srcdir}/mainproc/msasLocalServer.c
	gcc   $(CFLAGS) -o ${bindir}/msasLocalServer1 ${srcdir}/mainproc/msasLocalServer.c \
	/usr/lib/crt1.o /usr/lib/crti.o /usr/lib/crtn.o \
	${libdir}/mainlib.a ${libdir}/srvlib.a  ${prefix}/publiclib/lib/publib.a \
	-L /app/sqldb/lib -lpq     \
	-lm  -lpthread  -lfreetype -lm -lgd -ljpeg -lxml2  -liconv  -lssl -lcrypto	

${bindir}/msasShutDown1: ${srcdir}/mainproc/msasShutDown.c
	gcc   $(CFLAGS) -o ${bindir}/msasShutDown1 ${srcdir}/mainproc/msasShutDown.c \
	/usr/lib/crt1.o /usr/lib/crti.o /usr/lib/crtn.o \
	${prefix}/publiclib/lib/publib.a 
