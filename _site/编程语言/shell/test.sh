#!/bin/bash

ls ./
if [ $? -eq 0 ]; then
 echo "执行成功"
else
 echo "执行失败"
fi

echo $?
result=`pwd`
echo $result
