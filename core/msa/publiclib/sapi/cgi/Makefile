mysqldir = /app/ns/mysql

all:ltcgi.a xp xpbig5  upload  msauth.cgi qqMsg httpMsgProxy baochuanginstallmsa

clean:
	rm -f upload 
	rm -f xp
	rm -f qqMsg 
	rm -f *.o
	rm -f *.a
	rm -f gb2big5
	rm -f xpbig5
	rm -f xpslow
	rm -f msauth.cgi
	rm -f httpMsgProxy
	rm -f baochuanginstallmsa

ltcgi.a:ltcgi01.o ltcgi02.o 
	ar rv ltcgi.a ltcgi01.o ltcgi02.o

ltcgi01.o:ltcgi.h ltcgi01.c
	gcc -c -g ltcgi01.c -I../../include/  -I/usr/include/libxml2/ 

ltcgi02.o:ltcgi.h ltcgi02.c
	gcc -c -g ltcgi02.c -I../../include/  -I/usr/include/libxml2/

xp: ltcgi.h xp.c ltcgi.a
	gcc   -o xp xp.c   -I../../include/  -I/usr/include/libxml2/  ltcgi.a ../../lib/publib.a -lxml2 

qqMsg: ltcgi.h qqMsg.c ltcgi.a
	gcc   -o qqMsg qqMsg.c   -I../../include/  -I/usr/include/libxml2/  ltcgi.a ../../lib/publib.a -lxml2 

httpMsgProxy: ltcgi.h httpMsgProxy.c ltcgi.a
	gcc   -o httpMsgProxy httpMsgProxy.c   -I../../include/  -I/usr/include/libxml2/  ltcgi.a ../../lib/publib.a -lxml2 

msauth.cgi: ltcgi.h msauth.c ltcgi.a
	gcc   -o msauth.cgi msauth.c   -I../../include/  -I/usr/include/libxml2/  ltcgi.a ../../lib/publib.a -lxml2
		
xpslow: ltcgi.h xpslow.c ltcgi.a
	gcc   -o xpslow xpslow.c   -I../../include/  -I/usr/include/libxml2/ ltcgi.a ../../lib/publib.a -lxml2 

xpbig5: ltcgi.h xpbig5.c ltcgi.a
	gcc  -o xpbig5 xpbig5.c   -I../../include/  -I/usr/include/libxml2/  ltcgi.a ../../lib/publib.a -lxml2
upload: ltcgi.h upload.c ltcgi.a
	gcc  -o upload upload.c  -I../../include/  -I/usr/include/libxml2/  ltcgi.a ../../lib/publib.a -lxml2
gb2big5: gb2312tobig5.c ltcgi.a
	gcc   -o gb2big5 gb2312tobig5.c   -I../../include/  -I/usr/include/libxml2/  ltcgi.a ../../lib/publib.a  -lxml2 
allgb2big5: allgb2big5.c ltcgi.a
	gcc   -o allgb2big5 allgb2big5.c   -I../../include/  -I/usr/include/libxml2/  ltcgi.a ../../lib/publib.a  -lxml2
tobig5head: tobig5head.c ltcgi.a
	gcc   -o tobig5head tobig5head.c   -I../../include/  -I/usr/include/libxml2/  ltcgi.a ../../lib/publib.a  -lxml2

updatecgi: ltcgi.h updatecgi.c ltcgi.a  
	gcc   -o updatecgi updatecgi.c   -I${mysqldir}/include/mysql  -I../../include/  -I/usr/include/libxml2/  ltcgi.a ../../lib/publib.a ${mysqldir}/lib/mysql/libmysqlclient.a  -lxml2 

downloadcgi: ltcgi.h downloadcgi.c ltcgi.a  
	gcc   -o downloadcgi downloadcgi.c   -I${mysqldir}/include/mysql  -I../../include/  -I/usr/include/libxml2/  ltcgi.a ../../lib/publib.a ${mysqldir}/lib/mysql/libmysqlclient.a  -lxml2 

updateurl: updateurl.c 
	gcc   -o updateurl updateurl.c

baochuanginstallmsa: baochuanginstallmsa.c 
	gcc   -o baochuanginstallmsa baochuanginstallmsa.c  -I../../include/  -I/usr/include/libxml2/  ltcgi.a ../../lib/publib.a -lxml2 