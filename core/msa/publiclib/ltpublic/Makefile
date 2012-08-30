all: \
error.o buffer.o byte.o case.o fd.o getln.o open.o stralloc.o \
str.o str01.o bc_cnf.o ltdate.o ltfile.o fmt.o shm.o wait.o env.o alloc.o
	ar rv publiclib.a  buffer.o byte.o case.o fd.o \
	getln.o open.o stralloc.o str.o  \
	str01.o bc_cnf.o ltdate.o ltfile.o  fmt.o shm.o wait.o env.o alloc.o
	cp *.o ../lib
	cp *.h ../include

error.o: error.h error.c
	gcc -c  error.c

alloc.o: alloc.h alloc.c
	gcc -c  alloc.c

buffer.o: buffer.h buffer.c
	gcc -c  buffer.c

byte.o: byte.h byte.c
	gcc -c  byte.c

case.o: case.h case.c
	gcc -c  case.c

fd.o: fd.h fd.c
	gcc -c  fd.c

fmt.o: fmt.c fmt.h
	gcc -c  fmt.c

getln.o: getln.h getln.c
	gcc -c  getln.c

open.o: open.h open.c
	gcc -c  open.c

stralloc.o: stralloc.h stralloc.c
	gcc -c  stralloc.c

str.o: ltstr.h str.c
	gcc -c  str.c

str01.o: str01.c
	gcc -c str01.c

ltdate.o: ltdate.c
	gcc -c ltdate.c 

ltfile.o: ltfile.c
	gcc -c ltfile.c 

bc_cnf.o: bc_cnf.c
	gcc -c bc_cnf.c
	
shm.o: shm.c
	gcc -c shm.c

wait.o: wait.c
	gcc -c wait.c

env.o: env.c
	gcc -c env.c


install:
	cp *.o ../lib
	cp *.h ../include


