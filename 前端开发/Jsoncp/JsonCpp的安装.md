JsonCpp的安装 4、JsonCpp的安装 1、下载工具

JsonCpp

http://sourceforge.net/projects/jsoncpp/files/

scons

http://sourceforge.net/projects/scons/files/scons/2.1.0/scons-2.1.0.tar.gz

说明：

① 安装JsonCpp需要下载JsonCpp和Scon还需要机器上有python

② scons又是一个牛叉的工具，功能和GNU make一样，又比make简单多了。scons是python工具，需要先安装好python。

2、先安装scons

① 解压缩

tar -zxvf scons-2.0.1.tar.gz

② 设置环境变量

export MYSCONS=解压的路径

export SCONS_LIB_DIR=$MYSCONS/engine

③ 切换到jsoncpp的解压目录，执行命令：

python $MYSCONS/script/scons platform=linux-gcc

④ 生成静态和动态库文件

在jsoncpp-src-0.5.0/libs/linux-gcc-4.1.2目录下

libjson_linux-gcc-4.4.4_libmt.a

libjson_linux-gcc-4.4.4_libmt.so

mkdir /usr/include/jsoncpp

⑤ 把两个库文件拷贝到jsoncpp目录下

3、示例

将静态库重命名为libjson.a

示例示例

View Code 
#include "json/json.h"
#include <string>
#include <iostream>
using namespace std;

int main()
{
    string test ="{\"id\":1,\"name\":\"kurama\"}";
    Json::Reader reader;
    Json::Value value;
    if(reader.parse(test,value))
    {
        if(!value["id"].isNull())
        {
            cout << value["id"].asInt() << endl;
            cout << value["name"].asString()<< endl;
        }
    }
    return 0;
}
按 Ctrl+C 复制代码编译：g++ -o json_t json_t2.cpp /usr/include/json/libjson.a

参考

【1】 scons官网

http://www.scons.org/

【2】 安装参考

http://blog.csdn.net/zxkurama/article/details/6543552

http://hi.baidu.com/%B4%AB%CB%B5%D6%D0%B5%C4%C8%CC%D5%DF%C3%A8/blog/item/a6eb970c98a644d67acbe15a.html

