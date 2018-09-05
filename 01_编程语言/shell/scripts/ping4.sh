read "测试ping "  input
echo $input
echo $?
piog -c 1 10.0.2.1 >/dev/null  

if [ $? == 0 ];then
	echo "到网关正常"
	echo "ping -c 10.0.2.1"
fi
