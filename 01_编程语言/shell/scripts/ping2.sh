#!/bin/bash
#author:bin.liu
#shell 练习
ping -c 1 10.0.2.3
if [ $? == 0 ];then
	echo "到网关正常"
else
	echo "到网关不通"
fi
