all: ltcomm01.o ltcomm02.o
	cp *.o      ../lib/
	cp ltcomm.h ../include
	
ltcomm01.o: ltcomm01.c
	gcc -c  ltcomm01.c  -I../include

ltcomm02.o: ltcomm02.c
	gcc -c  ltcomm02.c -I../include

install:
	cp *.o      ../lib/
	cp ltcomm.h ../include
