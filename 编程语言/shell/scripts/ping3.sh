#!/bin/bash
#author:bin.liu
#ping 测试判断，简写方法
#ping -c 表示ping 几次
#&& 表示与
#/dev/null 黑洞
ping -c 1 10.0.2.3 && echo "到网关10.0.2.3正常" || echo "到网关10.0.2.3不通"
ping -c 1 10.0.2.1 > /dev/null && echo "到网关10.0.2.1正常" || echo "到网关10.0.2.1不通"
