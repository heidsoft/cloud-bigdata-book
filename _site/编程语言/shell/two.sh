#!/bin/sh
myvar="Hi there"

echo $myvar
echo "$myvar"
echo 'myvar'
echo \$myvar

echo Enter some text
read myvar

echo '$myvar' now equals $myvar
exit 0

#$HOME 当前用户的登陆子目录
#$PATH 以冒号分隔的用来搜索命令的子目录清单
#PS1  命令行提示符，通常是“$” 字符
#PS2  辅助提示符，用来提示后续输入，通常是“>” 字符
#$0  shell脚本程序的名字
#$#  传递到脚本程序的参数个数
#$$  该shell 脚本程序的进程ID，脚本程序一般会使用它来创建独一无二的临时文件，
#比如/tmp/tmpfile_$$
