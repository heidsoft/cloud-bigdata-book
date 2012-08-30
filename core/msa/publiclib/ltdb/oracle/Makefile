SHELL=/bin/sh
ORACLE_HOME=/home/oracle/app
DBINCLUDE = -I/app/ltServer/include   -I$(ORACLE_HOME)/rdbms/demo  -I$(ORACLE_HOME)/plsql/public  -I$(ORACLE_HOME)/network/public

ltoracle.a: ltoci.o ltdb.o
	ar rv ltoracle.a  ltoci.o ltdb.o

ltoci.o: 
	gcc -c ltoci.c  $(DBINCLUDE)
ltdb.o: 
	gcc -c ltdb.c  $(DBINCLUDE)

