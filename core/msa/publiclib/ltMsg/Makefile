all: ltmsg01.o ltmsg02.o 
	ar rv ltmsgpk.a  ltmsg01.o ltmsg02.o 
	cp *.o  ../lib/
	cp ltmsg.h ../include 

ltmsg01.o: ltmsg01.c
	gcc -c  ltmsg01.c  -I../include

ltmsg02.o: ltmsg02.c
	gcc -c  ltmsg02.c  -I../include

install:
	cp *.o  ../lib/
	cp ltmsg.h ../include
