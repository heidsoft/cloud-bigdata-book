#!/bin/bash
#根据输入的进程名称关闭匹配进程
str=""
echo "你输入的所有参数是$@"

for i in "$@";
do
    echo "处理的参数是$i"
    str="$str$i " ;
done

echo "你关闭的进程是$str"

if [ -z "$str" ]; then
  echo "请输入你需要关闭的进程名称."
  echo ""
  echo "Usage: bash stop.sh process_name"
  exit
fi

str=${str%?}

###################################################
#sed ^ 表示一行的开头。如：/^#/ 以#开头的匹配。
#$ 表示一行的结尾。如：/}$/ 以}结尾的匹配。
#\< 表示词首。 如 \<abc 表示以 abc 为首的詞。
#\> 表示词尾。 如 abc\> 表示以 abc 結尾的詞。
#. 表示任何单个字符。
#* 表示某个字符出现了0次或多次。
#\+ 之前的字符出现一次或以上(加号需转义)
#sed "s/^[^ ]\+ \+//g" 表示将行首匹配元素替换为空格
#sed "s/ .*//g" / 表示将空格 .* 替换为空格
#echo "aaabbb    " | sed 's/ //g' 删除所有空格
#echo "abccc b ab a ef f"|sed "s/^[^ ]\+ \+//g"
###################################################
ps aux | grep "$str" | grep -v "grep " | sed "s/^[^ ]\+ \+//g" | sed "s/ .*//g" | xargs kill -9 2>/dev/null

echo "杀死所有配置'$str'的进程"