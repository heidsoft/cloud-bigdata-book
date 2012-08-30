prefix = /app/msa/
bindir = /app/msa/bin
libdir = /app/msa/msa/lib
srcdir = /app/msa/msa
sqldir = /app/sqldb/include
publicdir = /app/msa/publiclib

CFLAGS:= -O2 -nostartfiles   -Wall -Wunused  -Iinclude -I${publicdir}/include -I${srcdir}/include \
-I${sqldir} -I/usr/include/libxml2/ -DNO_SHARED_LIBS=1

all: srv-admin.o srv-users.o srv-group.o \
srv-bushu.o srv-rule.o srv-ruleSimp.o srv-data-data.o  srv-report.o srv-manager.o srv-userpolicy.o
	ar rv ${libdir}/srvlib.a  *.o
clean:
	rm -f  ${libdir}/srvlib.a 


srv-admin.o: srv-admin.c
	gcc -c  srv-admin.c  $(CFLAGS)

srv-users.o: srv-users.c
	gcc -c  srv-users.c  $(CFLAGS)

srv-group.o: srv-group.c
	gcc -c  srv-group.c  $(CFLAGS)

srv-bushu.o: srv-bushu.c
	gcc -c  srv-bushu.c  $(CFLAGS)

srv-rule.o: srv-rule.c
	gcc -c  srv-rule.c  $(CFLAGS)

srv-ruleSimp.o: srv-ruleSimp.c
	gcc -c  srv-ruleSimp.c  $(CFLAGS)

srv-data-data.o: srv-data-data.c
	gcc -c  srv-data-data.c  $(CFLAGS)

srv-manager.o: srv-manager.c
	gcc -c  srv-manager.c  $(CFLAGS)

srv-report.o: srv-report.c
	gcc -c  srv-report.c  $(CFLAGS)

srv-nowview.o: srv-nowview.c
	gcc -c  srv-nowview.c  $(CFLAGS)

srv-nowconect.o: srv-nowconect.c
	gcc -c  srv-nowconect.c  $(CFLAGS)

srv-nowfun.o: srv-nowfun.c
	gcc -c  srv-nowfun.c  $(CFLAGS)

srv-nowviewuser.o: srv-nowviewuser.c
	gcc -c  srv-nowviewuser.c  $(CFLAGS)

srv-userpolicy.o: srv-userpolicy.c
	gcc -c  srv-userpolicy.c  $(CFLAGS)
