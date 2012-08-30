prefix = /app/msa/
bindir = /app/msa/bin
libdir = /app/msa/msas3/lib
srcdir = /app/msa/msas3
sqldir = /app/sqldb/include
publicdir = /app/msa/publiclib

CFLAGS:= -O2 -nostartfiles   -Wall -Wunused  
CFLAGS +=  -I/usr/include 
CFLAGS +=  -I/app/msa/msas3/include 
CFLAGS +=  -I/app/msa/publiclib/include 
CFLAGS +=  -I${sqldir}
CFLAGS +=  -I/usr/include/libxml2/ 
CFLAGS += -DNO_SHARED_LIBS=1

all: ${bindir}/msasMainProc3 	\
  ${bindir}/msasLocalServer3 \
	${bindir}/msasShutDown3 
	
clean: 
	rm -f ${bindir}/msasMainProc3 
	rm -f ${bindir}/msasLocalServer3 
	rm -f ${bindir}/msasShutDown3 

${bindir}/msasMainProc3: ${srcdir}/mainproc/msasMainProc.c ${libdir}/srvlib.a ${libdir}/mainlib.a
	gcc   $(CFLAGS) -o ${bindir}/msasMainProc3 ${srcdir}/mainproc/msasMainProc.c \
	/usr/lib/crt1.o /usr/lib/crti.o /usr/lib/crtn.o \
	${libdir}/mainlib.a ${prefix}/publiclib/lib/publib.a \
	-L /app/sqldb/lib -lpq \
	-lm  -lpthread  -lfreetype -lm -lgd -ljpeg -lxml2  -liconv  -lssl -lcrypto

${bindir}/msasLocalServer3: ${srcdir}/mainproc/msasLocalServer.c
	gcc   $(CFLAGS) -o ${bindir}/msasLocalServer3 ${srcdir}/mainproc/msasLocalServer.c \
	/usr/lib/crt1.o /usr/lib/crti.o /usr/lib/crtn.o \
	${libdir}/mainlib.a ${libdir}/srvlib.a  ${prefix}/publiclib/lib/publib.a \
	-L /app/sqldb/lib -lpq    \
	-lm  -lpthread  -lfreetype -lm -lgd -ljpeg -lxml2  -liconv  -lssl -lcrypto	

${bindir}/msasShutDown3: ${srcdir}/mainproc/msasShutDown.c
	gcc   $(CFLAGS) -o ${bindir}/msasShutDown3 ${srcdir}/mainproc/msasShutDown.c \
	/usr/lib/crt1.o /usr/lib/crti.o /usr/lib/crtn.o \
	${prefix}/publiclib/lib/publib.a 
