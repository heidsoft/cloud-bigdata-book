prefix = /app/msa/
bindir = /app/msa/bin
libdir = /app/msa/msas/lib
srcdir = /app/msa/msas
mysqldir = /app/ns/mysql
publicdir = /app/msa/publiclib
sharefubdir = /app/msa/bin/sharefun

CFLAGS:= -O2 -nostartfiles   -Wall -Wunused  -Iinclude -I${publicdir}/include -I${srcdir}/include \
-I${mysqldir}/include/mysql -I/usr/include/libxml2/ -DNO_SHARED_LIBS=1


all: ${sharefubdir}/sharefun-test.so
	
clean: 
	rm -f  ${sharefubdir}/sharefun-test.so
		
${sharefubdir}/sharefun-test.so: sharefun-test.c
	gcc -shared -lc  -o ${sharefubdir}/sharefun-test.so sharefun-test.c  $(CFLAGS)

