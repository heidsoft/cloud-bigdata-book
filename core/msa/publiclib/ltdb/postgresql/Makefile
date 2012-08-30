SHELL=/bin/sh
DBDIR = /app/sqldb
DBINCLUDE = -I$(DBDIR)/include/
DBLIB = -L$(DBDIR)/lib -lpq

all: ltdb.o
	ar rv dbpostgresql.a ltdb.o
	cp ltdb.o 					../../lib/
	cp ../ltdb.h 				../../include/
	cp ../dbpostgresql.h 		../../include/


ltdb.o: ltdb.c
	gcc -c ltdb.c -I../ -I../../include  $(DBINCLUDE)
	
install:
	cp ltdb.o 					../../lib/
	cp ../ltdb.h 				../../include/
	cp ../dbpostgresql.h 		../../include/


