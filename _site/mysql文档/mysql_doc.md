#测试mariadb是否能运行cmdb
```
安装：yum install mariadb mariadb-server
开启服务：sudo systemctl start mariadb
开机启动：sudo systemctl enable mariadb
初始化：mysql_secure_installation

grant all on *.* to oneoaas@'%' identified by '123456';
grant all privileges on *.* to oneoaas@'%' identified by '123456' with grant option;
grant all privileges on *.* to root@'10.0.2.146' identified by '123456' with grant option;


```
