# 安装文件下载地址
http://archive.cloudera.com/cm5/cm/5/
http://archive.cloudera.com/cm5/redhat/7/x86_64/cm/5.12.0/repodata/

# 虚拟机快速入门
https://www.cloudera.com/documentation/enterprise/5-9-x/topics/cloudera_quickstart_vm.html


# 手动安装mysql 数据库

hive
create database hive DEFAULT CHARSET utf8 COLLATE utf8_general_ci;
activity monitor
create database amon DEFAULT CHARSET utf8 COLLATE utf8_general_ci;

grant all privileges on *.* to 'root'@'cdh1' identified by '123456' with grant option;
flush privileges;

# 初始化数据

[root@cdh1 cmf]# schema/scm_prepare_database.sh mysql cm -hlocalhost -uroot -p123456 --scm-host localhost scm scm scm
JAVA_HOME=/usr/local/share/jdk1.8.0_144
Verifying that we can write to /opt/cm-5.12.1/etc/cloudera-scm-server
Creating SCM configuration file in /opt/cm-5.12.1/etc/cloudera-scm-server
Executing:  /usr/local/share/jdk1.8.0_144/bin/java -cp /usr/share/java/mysql-connector-java.jar:/usr/share/java/oracle-connector-java.jar:/opt/cm-5.12.1/share/cmf/schema/../lib/* com.cloudera.enterprise.dbutil.DbCommandExecutor /opt/cm-5.12.1/etc/cloudera-scm-server/db.properties com.cloudera.cmf.db.
[                          main] DbCommandExecutor              INFO  Successfully connected to database.
All done, your SCM database is configured correctly!

# 添加用户

useradd --system --home=/opt/opt/cm-5.12.1/run/cloudera-scm-server/ --no-create-home --shell=/bin/false --comment "Cloudera SCM User" cloudera-scm


# 安装pip

yum -y install epel-release
yum install python-pip
