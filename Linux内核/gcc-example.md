gcc main.c

gcc main.c -o main

开启所有警告
gcc -Wall main.c -o main

生成预处理文件
gcc -E main.c > main.i

生成汇编代码
gcc -S main.c > main.s

生成汇编后的目标文件
gcc -C main.c

gcc -save-temps main.c

gcc  -Wall main.c -o main -lCPPfile

gcc -c -Wall -Werror -fPIC Cfile.c

gcc -shared -o libCfile.so Cfile.o

gcc -Wall -v main.c -o main

gcc -Wall -ansi main.c -o main

gcc -Wall -funsigned-char main.c -o main

gcc -Wall -fsigned-char main.c -o main

gcc -Wall -DMY_MACRO main.c -o main