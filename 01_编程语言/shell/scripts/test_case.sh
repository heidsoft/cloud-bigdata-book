#!/bin/bash
echo -n "请输入命令:"
read cmd
case $cmd in
	oneoaas_cmdb)
		echo "您输入的参数是$cmd"
		break;
	;;
	
	oneoaas_monitor)
		echo "您输入的参数是$cmd"	
		break;
	;;
	
	oneoaas_cd)
		echo "您输入的参数是$cmd"
		break;
	;;
	
	*)
		echo "您应该输入参数为(oneoaas_cmdb|oneoaas_monitor|oneoaas_cd)其中的一种"
	;;

esac
