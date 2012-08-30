prefix = /app/msa/
bindir = /app/msa/bin
libdir = /app/msa/icm/lib
srcdir = /app/msa/icm
sqldir = /app/sqldb/include
publicdir = /app/msa/publiclib

CFLAGS:= -O2 -nostartfiles   -Wall -Wunused  -Iinclude -I${publicdir}/include -I${srcdir}/include \
-I${sqldir} -I/usr/include/libxml2/ -DNO_SHARED_LIBS=1

all: srv-group.o srv-manager.o
	ar rv ${libdir}/srvlib.a  *.o

clean: 
	rm -f  ${libdir}/srvlib.a  srv-manager.o
	
srv-manager.o: srv-manager.c
	gcc -c  srv-manager.c  $(CFLAGS)
	

