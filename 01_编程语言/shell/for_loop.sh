# https://www.cyberciti.biz/faq/bash-for-loop/
# 查看文件的for循环
for i in `cat ip.txt` ; do   ping -c 1 $i  ; done
