all: md501.o  des01.o des02.o sha1.o b64.o 
	ar rv ltSecurity.a  md501.o   des01.o des02.o sha1.o b64.o
	cp *.o           ../lib/
	cp ltsecurity.h  ../include

md501.o: md501.c
	gcc -c  md501.c  -I../include

des01.o: des01.c
	gcc -c  des01.c -I../include

des02.o: des02.c
	gcc -c  des02.c -I../include
	
sha1.o: sha1.c sha1.h
	gcc -c  sha1.c 
	
b64.o: b64.c 
	gcc -c  b64.c

install:
	cp *.o           ../lib/
	cp ltsecurity.h  ../include
