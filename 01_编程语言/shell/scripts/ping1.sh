#!/bin/bash

ping -c 1 10.0.2.1
if [ $? == 0 ];then
	echo "到网关正常"
fi
