先转一个


//Test1.c:

#include <stdio.h>
#include <mysql.h>

int main(int argc, char *argv[])
{
 int i;
 char *names[3];
 names[0] = "haha";
 names[1] = "xixi";
 names[2] = "wuwu";

 char execsql[1024];

 MYSQL mysql_conn;

 if(mysql_init(&mysql_conn) != NULL){
  if(mysql_real_connect(&mysql_conn, "localhost", "root", "xiaodan", "test", MYSQL_PORT, NULL, 0) != NULL)
  {
   printf("Good connection!\n");
   sprintf(execsql, "create database one_db");
   mysql_real_query(&mysql_conn, execsql, strlen(execsql));

//   if (!mysql_create_db(&mysql_conn, "one_db")){
   i = mysql_affected_rows(&mysql_conn);
   if (i<=0){
    printf("Can not create database one_db \n");
   }
   else{
    mysql_select_db(&mysql_conn, "one_db");
    sprintf(execsql, "create table girls(name char(10), age int(8))");
    mysql_real_query(&mysql_conn, execsql, strlen(execsql));
    mysql_select_db(&mysql_conn, "one_db");
    for(i=0; i<3; i++){
     sprintf(execsql,"insert into girls values('%s', %d)", names[i], 19+i);
     printf("%s \n", execsql);
     mysql_query(&mysql_conn, execsql);
    }
   }
  }else{
   printf("Connection Failed!\n");
  }
 }else{
  printf("Initialization Failed.\n");
  return -1;
 }

 mysql_close(&mysql_conn);
 return 0;
}

编译命令：gcc -g -o test1   -I/usr/include/mysql test1.c -L/usr/lib/mysql -lmysqlclient -lz

//Test2.c

#include <stdio.h>

//#include <dmalloc.h>

#include <mysql.h>

int main(int argc, char **argv) {

    MYSQL mysql_conn; /* Connection handle */

    MYSQL_RES *mysql_result; /* Result handle */

    MYSQL_ROW mysql_row; /* Row data */

    int f1, f2, num_row, num_col;

    if (mysql_init(&mysql_conn) != NULL) {

        if (mysql_real_connect(&mysql_conn, "localhost", "root", "xiaodan", "one_db", MYSQL_PORT, NULL, 0) != NULL) {

            if (mysql_query(&mysql_conn, "select * from girls") == 0) {

    /*  以上我们执行select语句，查询表中所以记录*/

                mysql_result = mysql_store_result(&mysql_conn); // get the result from the executing select query

                num_row = mysql_num_rows(mysql_result); /* Get the no. of row */

                num_col = mysql_num_fields(mysql_result); /* Get the no. of column */

                for (f1 = 0; f1 < num_row; f1++) {

//                  for (f2 = 0; f2 < num_col; f2++) {

                        mysql_row = mysql_fetch_row(mysql_result); /* Fetch one by one */

                        printf("[Row %d, Col 0] ==> [%s]\n", f1, mysql_row[0]);
   printf("[Row %d, Col 1] ==> [%s]\n", f1, mysql_row[1]);

//                  }

                }

            } else {

                (void) printf("Query fails\n");

            }

        } else {

            (void) printf("Connection fails\n");

        }

    } else {

        (void) printf("Initialization fails\n");

    }

    mysql_free_result(mysql_result);

    mysql_close(&mysql_conn);

    return 0;

}

编译命令：gcc -g -o test2   -I/usr/include/mysql test2.c -L/usr/lib/mysql -lmysqlclient -lz

注意：使用rpm包安装的数据库没有用于开发的头文件和库，需要下载MySQL-devel的rpm开发包

 

 下面的是我自己写的一个

 


/*
*
*file name: mysql.c
*
*/
#include <stdio.h>
#include <stdlib.h>
#include "mysql.h"  

int main(void)
{
  const char *host = "localhost";
  const char *user = "root";
  const char *pass = "";
  const char *db   = "mysql";
 /* 定义mysql变量 */
     MYSQL mysql;
     MYSQL_RES *rs;
     MYSQL_ROW row;
     mysql_init(&mysql); /* 初始化 */
/* 连接数据库 */
if (!mysql_real_connect(&mysql, host, user, pass, db, 0, NULL, 0))
 { 
          printf("%s", mysql_error(&mysql));
       
 }
  char *sql = "select host,user from user order by rand()";
  if (mysql_query(&mysql, sql)!=0) 
  { /* 查询 */
          printf( "%s", mysql_errno(&mysql), mysql_error(&mysql));
         
   }
   rs = mysql_store_result(&mysql); /* 获取查询结果 */
   while ((row = mysql_fetch_row(rs))) 
   {      /* 获取每一行记录 */
          printf( "%s---%s", row[0], row[1]);
   }
   mysql_free_result(rs); /* 释放结果集 */
   mysql_close(&mysql); /* 关闭连接 */
   return 1;
}
复制代码
编译：$gcc -g -o mysql   -I/usr/local/mysql/include/mysql mysql.c -L/usr/locla/mysql/lib/mysql -lmysqlclient -lz
如果运行的时候报libmysqlclient.so.16 找不到错误，运行下面命令。
$ln -s /usr/local/mysql/lib/mysql/libmysqlclient.so.16 /lib/libmysqlclient.so.16 


如果运行程序：

 $gcc -g -o mysql   -I/usr/local/mysql/include/mysql mysql.c -L/usr/locla/mysql/lib/mysql -lmysqlclient -lz 会在当前目录生成 mysql文件

./mysql运行

或者

/path-to -mysql/mysql 
g++ -g -o post   -I/usr/local/mysql/include/mysql post.cpp -L/usr/locla/mysql/lib/mysql -lmysqlclient -lz 



 
