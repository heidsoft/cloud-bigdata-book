prefix = /app/msa/
bindir = /app/msa/bin
libdir = /app/msa/msas/lib
srcdir = /app/msa/msas
sqldir = /app/sqldb/include
publicdir = /app/msa/publiclib

CFLAGS:= -O2 -nostartfiles   -Wall -Wunused  -Iinclude -I${publicdir}/include -I${srcdir}/include \
-I${sqldir} -I/usr/include/libxml2/ -DNO_SHARED_LIBS=1

all: srv-admin.o srv-now.o srv-group.o srv-data.o srv-rule.o srv-report.o srv-msarule.o
	ar rv ${libdir}/srvlib.a  *.o

clean: 
	rm -f  ${libdir}/srvlib.a *.o
	
srv-admin.o: srv-admin.c
	gcc -c  srv-admin.c  $(CFLAGS)

srv-now.o: srv-now.c
	gcc -c  srv-now.c  $(CFLAGS)

srv-group.o: srv-group.c
	gcc -c  srv-group.c  $(CFLAGS)

srv-data.o: srv-data.c
	gcc -c  srv-data.c  $(CFLAGS)

srv-rule.o: srv-rule.c
	gcc -c  srv-rule.c  $(CFLAGS)

srv-msarule.o: srv-msarule.c
	gcc -c  srv-msarule.c  $(CFLAGS)

srv-report.o: srv-report.c
	gcc -c  srv-report.c  $(CFLAGS)
