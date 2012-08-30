SHELL=/bin/sh
DBDIR = /usr/local/mysql
DBINCLUDE = -I$(DBDIR)/include
DBLIB = $(DBDIR)/lib/libmysqlclient.a
LTLIB =/app/ltServer/lib/ltlib.a
LTDB=/app/ltServer/lib/ltmysql.a
LTINCLUDE= -I/app/ltServer/include
test1:
	gcc -o test1 test1.c $(DBINCLUDE) $(LTINCLUDE) $(LTLIB) $(LTDB) $(DBLIB) 
