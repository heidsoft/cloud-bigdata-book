all: ssllib.o
	ar rv ssllib.a  ssllib.o

ssllib.o: ssllib.h ssllib.c
	gcc -g -I/usr/include  -I/app/ns/v2008/publiclib/include -Wall    -c -o ssllib.o ssllib.c
	