prefix = /app/msa/
bindir = /app/msa/bin
libdir = /app/msa/msas2/lib
srcdir = /app/msa/msas2
sqldir = /app/sqldb/include
publicdir = /app/msa/publiclib

CFLAGS:= -O2 -nostartfiles   -Wall -Wunused  
CFLAGS +=  -I/usr/include 
CFLAGS +=  -I/app/msa/msas2/include 
CFLAGS +=  -I/app/msa/publiclib/include 
CFLAGS +=  -I${sqldir}
CFLAGS +=  -I/usr/include/libxml2/ 
CFLAGS += -DNO_SHARED_LIBS=1

all: ${bindir}/msasMainProc2 	\
	${bindir}/msasLocalServer2 \
	${bindir}/msasShutDown2 
	
clean: 
	rm -f ${bindir}/msasMainProc2
	rm -f ${bindir}/msasLocalServer2 
	rm -f ${bindir}/msasShutDown2 

${bindir}/msasMainProc2: ${srcdir}/mainproc/msasMainProc.c ${libdir}/srvlib.a ${libdir}/mainlib.a
	gcc   $(CFLAGS) -o ${bindir}/msasMainProc2 ${srcdir}/mainproc/msasMainProc.c \
	/usr/lib/crt1.o /usr/lib/crti.o /usr/lib/crtn.o \
	${libdir}/mainlib.a ${prefix}/publiclib/lib/publib.a \
	-L /app/sqldb/lib -lpq \
	-lm  -lpthread  -lfreetype -lm -lgd -ljpeg -lxml2  -liconv  -lssl -lcrypto

${bindir}/msasLocalServer2: ${srcdir}/mainproc/msasLocalServer.c
	gcc   $(CFLAGS) -o ${bindir}/msasLocalServer2 ${srcdir}/mainproc/msasLocalServer.c \
	/usr/lib/crt1.o /usr/lib/crti.o /usr/lib/crtn.o \
	${libdir}/srvlib.a ${libdir}/mainlib.a  ${prefix}/publiclib/lib/publib.a \
	-L /app/sqldb/lib -lpq   \
	-lm  -lpthread  -lfreetype -lm -lgd -ljpeg -lxml2  -liconv  -lssl -lcrypto	

${bindir}/msasShutDown2: ${srcdir}/mainproc/msasShutDown.c
	gcc   $(CFLAGS) -o ${bindir}/msasShutDown2 ${srcdir}/mainproc/msasShutDown.c \
	/usr/lib/crt1.o /usr/lib/crti.o /usr/lib/crtn.o \
	${prefix}/publiclib/lib/publib.a 
