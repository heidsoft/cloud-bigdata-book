lsnrctl status//查看状态
ORACLE_SID=clouddb//设置需要操作的实例
sqlplus /nolog //nolog是不登陆到数据库服务器的意思，如果没有/nolog参数，sqlplus会提示你输入用户名和密码 
sqlplus /nolog --运行sqlplus命令，进入sqlplus环境，nolog参数表示不登录； 
SQL> connect / as sysdba --以系统管理员(sysdba)的身份连接数据库；如果需要对数据库进行管理操作，那么需要以这种方式登录数据库，或者 
connect sys@service_name as sysdba，其中service_name 你配置的客户tnsname服务名． 
SQL> startup --如果数据库没有启动的话，经过上述步骤连接后，正常启动数据 
1：sqlplus '/ as sysdba ' 
和 
2：sqlplus /nolog 
SQL> connect / as sysdba 
1和2是一样的意思 
