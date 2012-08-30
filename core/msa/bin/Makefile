prefix = /app/msa/
bindir = /app/msa/bin
libdir = /app/msa/msa/lib
srcdir = /app/msa/msa
sqldir = /app/sqldb/include
publicdir = /app/msa/publiclib

CFLAGS:= -O2 -nostartfiles   -Wall -Wunused  -Iinclude -I${publicdir}/include -I${srcdir}/include \
-I${sqldir} -I/usr/include/libxml2/ -DNO_SHARED_LIBS=1

all: srv-admin.o  srv-manager.o srv-group.o srv-nowview.o srv-nowconect.o srv-nowviewuser.o \
srv-nowtools.o srv-nowfun.o srv-users.o srv-adusers.o srv-userexport.o srv-userimport.o srv-userpolicy.o \
srv-manager.o srv-sysclean.o srv-bushu.o srv-rule.o srv-url.o srv-auth.o srv-qos.o srv-firewall.o \
srv-route.o srv-keyword.o srv-rule-vid.o srv-rule-sec.o srv-data.o   \
srv-srvline.o  srv-rule-time.o  \
srv-data-data.o  srv-data-conkeysech.o srv-data-form.o srv-data-time.o  srv-data-people.o \
srv-report-linereport.o srv-report-srvline.o srv-report-webline.o srv-report-userline.o \
srv-report-pageoption.o srv-data-convert.o  srv-service-tree.o  srv-report-sortreport.o srv-dailyreportcall.o \
srv-conFun.o srv-vpn.o srv-exportpackage.o  srv-bypass.o srv-onlinecloud.o srv-report-comparereport.o \
srv-report-weblinereport.o srv-setworktime.o  srv-setworktime.o srv-register.o
	ar rv ${libdir}/srvlib.a  *.o 
clean: 
	rm -f  ${libdir}/srvlib.a *.o
	
srv-admin.o: srv-admin.c
	gcc -c  srv-admin.c  $(CFLAGS)

srv-manager.o: srv-manager.c
	gcc -c  srv-manager.c  $(CFLAGS)

srv-group.o: srv-group.c
	gcc -c  srv-group.c  $(CFLAGS)

srv-nowview.o: srv-nowview.c
	gcc -c  srv-nowview.c  $(CFLAGS)

srv-nowconect.o: srv-nowconect.c
	gcc -c  srv-nowconect.c  $(CFLAGS)

srv-nowviewuser.o: srv-nowviewuser.c
	gcc -c  srv-nowviewuser.c  $(CFLAGS)
	
srv-nowtools.o: srv-nowtools.c
	gcc -c  srv-nowtools.c  $(CFLAGS)

srv-nowfun.o: srv-nowfun.c
	gcc -c  srv-nowfun.c  $(CFLAGS)	

srv-users.o: srv-users.c
	gcc -c  srv-users.c  $(CFLAGS)	

srv-adusers.o: srv-adusers.c
	gcc -c  srv-adusers.c  $(CFLAGS)	

srv-userexport.o: srv-userexport.c
	gcc -c  srv-userexport.c  $(CFLAGS)	
	
srv-userimport.o: srv-userimport.c
	gcc -c  srv-userimport.c  $(CFLAGS)	
	
srv-userpolicy.o: srv-userpolicy.c
	gcc -c  srv-userpolicy.c  $(CFLAGS)	
	
srv-sysclean.o: srv-sysclean.c
	gcc -c  srv-sysclean.c  $(CFLAGS)	

srv-bushu.o: srv-bushu.c
	gcc -c  srv-bushu.c  $(CFLAGS)	

srv-rule.o: srv-rule.c
	gcc -c  srv-rule.c  $(CFLAGS)	

srv-url.o: srv-url.c
	gcc -c  srv-url.c  $(CFLAGS)	
	
srv-auth.o: srv-auth.c
	gcc -c  srv-auth.c  $(CFLAGS)	

srv-qos.o: srv-qos.c
	gcc -c  srv-qos.c  $(CFLAGS)

srv-firewall.o: srv-firewall.c
	gcc -c  srv-firewall.c  $(CFLAGS)	
	
srv-route.o: srv-route.c
	gcc -c  srv-route.c  $(CFLAGS)	

srv-keyword.o: srv-keyword.c
	gcc -c  srv-keyword.c  $(CFLAGS)
	
srv-rule-vid.o: srv-rule-vid.c
	gcc -c  srv-rule-vid.c  $(CFLAGS)	

srv-rule-sec.o: srv-rule-sec.c
	gcc -c  srv-rule-sec.c  $(CFLAGS)	

srv-rule-time.o: srv-rule-time.c
	gcc -c  srv-rule-time.c  $(CFLAGS)	

srv-data-data.o: srv-data-data.c
	gcc -c  srv-data-data.c  $(CFLAGS)

srv-data-conkeysech.o: srv-data-conkeysech.c
	gcc -c  srv-data-conkeysech.c  $(CFLAGS)

srv-data-form.o: srv-data-form.c
	gcc -c  srv-data-form.c  $(CFLAGS)
	
srv-data-time.o: srv-data-time.c
	gcc -c  srv-data-time.c  $(CFLAGS)

srv-data-people.o: srv-data-people.c
	gcc -c  srv-data-people.c  $(CFLAGS)

srv-data-convert.o: srv-data-convert.c
	gcc -c  srv-data-convert.c  $(CFLAGS)

srv-service-tree.o: srv-service-tree.c
	gcc -c  srv-service-tree.c  $(CFLAGS)

srv-report-linereport.o: srv-report-linereport.c
	gcc -c  srv-report-linereport.c  $(CFLAGS)

srv-report-srvline.o: srv-report-srvline.c
	gcc -c  srv-report-srvline.c  $(CFLAGS)

srv-report-webline.o: srv-report-webline.c
	gcc -c  srv-report-webline.c  $(CFLAGS)

srv-report-userline.o: srv-report-userline.c
	gcc -c  srv-report-userline.c  $(CFLAGS)	
	
srv-report-pageoption.o: srv-report-pageoption.c
	gcc -c  srv-report-pageoption.c  $(CFLAGS)	

srv-report-sortreport.o: srv-report-sortreport.c
	gcc -c  srv-report-sortreport.c  $(CFLAGS)

srv-dailyreportcall.o: srv-dailyreportcall.c
	gcc -c  srv-dailyreportcall.c  $(CFLAGS)

srv-conFun.o: srv-conFun.c
	gcc -c  srv-conFun.c  $(CFLAGS)

srv-vpn.o: srv-vpn.c
	gcc -c  srv-vpn.c  $(CFLAGS)

srv-exportpackage.o: srv-exportpackage.c
	gcc -c  srv-exportpackage.c  $(CFLAGS)

srv-bypass.o: srv-bypass.c
	gcc -c  srv-bypass.c  $(CFLAGS)

srv-onlinecloud.o: srv-onlinecloud.c
	gcc -c  srv-onlinecloud.c  $(CFLAGS)

srv-report-comparereport.o: srv-report-comparereport.c
	gcc -c  srv-report-comparereport.c  $(CFLAGS)

srv-report-weblinereport.o: srv-report-weblinereport.c
	gcc -c  srv-report-weblinereport.c  $(CFLAGS)
	
srv-setworktime.o: srv-setworktime.c
	gcc -c  srv-setworktime.c  $(CFLAGS)

srv-register.o: srv-register.c
	gcc -c  srv-register.c  $(CFLAGS)
	
	
