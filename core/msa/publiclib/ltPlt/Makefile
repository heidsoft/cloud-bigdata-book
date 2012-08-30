all: ltpltdb.o ltplt.o
	ar rv ltPlt.a  ltplt.o ltpltdb.o
	cp *.o     ../lib/
	cp *.h     ../include

ltpltdb.o: ltplt.h ltpltdb.c
	gcc -I../include/ -I/usr/include/libxml2/ -c  ltpltdb.c 

ltplt.o: ltplt.h ltplt.c
	gcc -I../include/ -I/usr/include/libxml2/   -c ltplt.c 

install:
	cp *.o     ../lib/
	cp *.h     ../include

clear:
	rm -f *.a
	rm -f *.o
