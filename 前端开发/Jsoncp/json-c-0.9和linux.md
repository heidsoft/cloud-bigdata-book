tar zxvf json-c-0.9.tar.gz
cd json-c-0.9/
./configure --prefix=/usr/local/json-c-0.9(没有prefix选项，默认的是/usr/local/lib/)
make && make install
echo "/usr/local/json-c-0.9/lib/" > /etc/ld.so.conf.d/json-c-0.9.conf
echo "/usr/local/tokyocabinet-1.4.33/lib/" > /etc/ld.so.conf.d/tokyocabinet-1.4.33.conf

/sbin/ldconfig


编辑配置文件/etc/ld.so.conf,在文件中加入lib库的路径（默认的是/usr/local/lib）
执行ldconfig或/sbin/ldconfig。然后编译，例如：
 gcc -ljson -I/usr/local/include/json/ -L/usr/local/lib/ test2.c

也可以更改环境变量(LD_LIBRARY_PATH)来加入json库，使用export命令如：
export LD_LIBRARY_PATH = 库路径,可以是多个库，多个库间以：隔开，export
设置后对所有的命令都有效；
用另一种设置环境变量的方法，该环境变量只对该命令有效，
当该命令执行完成后，该环境变量就无效了。


#LD_LIBRARY_PATH=/usr/local/lib/
./a.out（执行文件）

