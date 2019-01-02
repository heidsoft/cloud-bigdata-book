　　功 能: 将s所指向的某一块内存中的每个字节的内容全部设置为ch指定的ASCII值,

　　块的大小由第三个参数指定,这个函数通常为新申请的内存做初始化工作

　　用 法: void *memset(void *s, char ch, unsigned n);

　　程序例:

　　#include <string.h>

　　#include <stdio.h>

　　#include <mem.h>

　　int main(void)

　　{

　　char buffer[] = "Hello world\n";

　　printf("Buffer before memset: %s\n", buffer);

　　memset(buffer, '*', strlen(buffer) );

　　printf("Buffer after memset: %s\n", buffer);

　　return 0;

　　} 

　　输出结果：

　　Buffer before memset: Hello world

　　Buffer after memset: ***********

　　编译平台：

　　Microsoft Visual C++ 6.0

　　也不一定就是把内容全部设置为ch指定的ASCII值，而且该处的ch可为int或者其他类型，并不一定要是char类型。例如下面这样：

　　int array[5] = {1,4,3,5,2};

　　for(int i = 0; i < 5; i++)

　　cout<<array<<" ";

　　cout<<endl;

　　memset(array,0,5*sizeof(int));

　　for(int k = 0; k < 5; k++)

　　cout<<array[k]<<" ";

　　cout<<endl;

　　输出的结果就是：1 4 3 5 2

　　0 0 0 0 0

　　后面的表大小的参数是以字节为单位，所以，对于int或其他的就并不是都乘默认的1（字符型）了。而且不同的机器上int的大小也可能不同，所以最好用sizeof（）。 

　　要注意的是，memset是对字节进行操作，所以上述程序如果改为

　　int array[5] = {1,4,3,5,2};

　　for(int i = 0; i < 5; i++)

　　cout<<array<<" ";

　　cout<<endl;

　　memset(array,1,5*sizeof(int));// 注意 这里与上面的程序不同

　　for(int k = 0; k < 5; k++) 

　　cout<<array[k]<<" ";

　　cout<<endl;

　　输出的结果就是：1 4 3 5 2

　　16843009 16843009 16843009 16843009 16843009

　　为什么呢？

　　因为memset是以字节为单位就是对array指向的内存的5个字节进行赋 值，每个都用ASCII为1的字符去填充，转为二进制后，1就是00000001,占一个字节。一个INT元素是4字节，合一起就是 1000000010000000100000001，就等于16843009，就完成了对一个INT元素的赋值了。

　　所以用memset对非字符型数组赋初值是不可取的！

　　楼上说的很对，只是程序执行结果是0 0 0 0；程序不同的地方不在那里。程序如下：

　　int array[5] = {1,4,3,5,2};

　　for(int i = 0; i < 5; i++)

　　cout<<array<<" ";

　　cout<<endl;

　　memset(array,1,5*sizeof(int)); //这里才是不同的地方

　　for(int k = 0; k < 5; k++) //不同不在这里，k=1只是少循环了一次而已

　　cout<<array[k]<<" ";

　　cout<<endl;

　　例如有一个结构体Some x，可以这样清零：

　　memset( &x, 0, sizeof(Some) );

　　如果是一个结构体的数组Some x[10]，可以这样：

　　menset( x, 0, sizeof(Some)*10 );


memset函数详细说明

　　1。void *memset(void *s,int c,size_t n)

　　总的作用：将已开辟内存空间 s 的首 n 个字节的值设为值 c。

　　2。例子

　　main(){

　　char *s="Golden Global View";

　　clrscr();

　　memset(s,'G',6);//貌似这里有点问题//

　　printf("%s",s);

　　getchar();

　　return 0;

　　}　

　　【这个问题相当大，程序根本就运行不下去了，你这里的S志向的是一段只读的内存，而你memset又试图修改它，所以运行时要出错，修改办法char *s修改为char s[]】

　　3。memset() 函数常用于内存空间初始化。如：

　　char str[100];

　　memset(str,0,100);

　　4。memset()的深刻内涵：用来对一段内存空间全部设置为某个字符，一般用在对定义的字符串进行初始化为‘memset(a, '\0', sizeof(a));

　　memcpy用来做内存拷贝，你可以拿它拷贝任何数据类型的对象，可以指定拷贝的数据长度；例：char a[100],b[50]; memcpy(b, a, sizeof(b));注意如用sizeof(a)，会造成b的内存地址溢出。

　　strcpy就只能拷贝字符串了，它遇到'\0'就结束拷贝；例：char a[100],b[50];strcpy(a,b);如用strcpy(b,a)，要注意a中的字符串长度（第一个‘\0’之前）是否超过50位，如超过，则会造成b的内存地址溢出。

　　5.补充：某人的一点心得

　　memset可以方便的清空一个结构类型的变量或数组。

　　如：

　　struct sample_struct

　　{

　　char csName[16];

　　int iSeq;

　　int iType;

　　};

　　对于变量

　　struct sample_strcut stTest;

　　一般情况下，清空stTest的方法：

　　stTest.csName[0]='\0';

　　stTest.iSeq=0;

　　stTest.iType=0;

　　用memset就非常方便：

　　memset(&stTest,0,sizeof(struct sample_struct));

　　如果是数组：

　　struct sample_struct TEST[10];

　　则

　　memset(TEST,0,sizeof(struct sample_struct)*10);

　　6。strcpy

　　原型：extern char *strcpy(char *dest,char *src);

　　用法：＃i nclude

　　功能：把src所指由NULL结束的字符串复制到dest所指的数组中。

　　说明：src和dest所指内存区域不可以重叠且dest必须有足够的空间来容纳src的字符串。

　　返回指向dest的指针。

　　memcpy

　　原型：extern void *memcpy(void *dest, void *src, unsigned int count);

　　用法：＃i nclude

　　功能：由src所指内存区域复制count个字节到dest所指内存区域。

　　说明：src和dest所指内存区域不能重叠，函数返回指向dest的指针。

　　memset

　　原型：extern void *memset(void *buffer, int c, int count);

　　用法：＃i nclude

　　功能：把buffer所指内存区域的前count个字节设置成字符c。

　　说明：返回指向buffer的指针。



原文链接：http://baike.baidu.com/view/982208.htm
<====================================================================================================>

1。void *memset(void *s,int c,size_t n)
总的作用：将已开辟内存空间 s 的首 n 个字节的值设为值 c。

2。例子

main(){
char *s="Golden Global View";

clrscr();

memset(s,'G',6);//貌似这里有点问题//
printf("%s",s);

getchar();
return 0;
}　
3。memset() 函数常用于内存空间初始化。如：
char str[100];
memset(str,0,100);

4。memset()的深刻内涵：用来对一段内存空间全部设置为某个字符，一般用在对定义的字符串进行初始化为‘memset(a, '\0', sizeof(a));

memcpy用来做内存拷贝，你可以拿它拷贝任何数据类型的对象，可以指定拷贝的数据长度；例：char a[100],b[50]; memcpy(b, a, sizeof(b));注意如用sizeof(a)，会造成b的内存地址溢出。

strcpy就只能拷贝字符串了，它遇到'\0'就结束拷贝；例：char a[100],b[50];strcpy(a,b);如用strcpy(b,a)，要注意a中的字符串长度（第一个‘\0’之前）是否超过50位，如超过，则会造成b的内存地址溢出。

5.补充：某人的一点心得
memset可以方便的清空一个结构类型的变量或数组。

如：
struct sample_struct
{
char csName[16];
int iSeq;
int iType;
};

对于变量
struct sample_strcut stTest;

一般情况下，清空stTest的方法：
stTest.csName[0]='\0';
stTest.iSeq=0;
stTest.iType=0;

用memset就非常方便：
memset(&stTest,0,sizeof(struct sample_struct));

如果是数组：
struct sample_struct TEST[10];
则
memset(TEST,0,sizeof(struct sample_struct)*10);

6。strcpy
原型：extern char *strcpy(char *dest,char *src);
用法：＃i nclude
功能：把src所指由NULL结束的字符串复制到dest所指的数组中。
说明：src和dest所指内存区域不可以重叠且dest必须有足够的空间来容纳src的字符串。
返回指向dest的指针。


memcpy
原型：extern void *memcpy(void *dest, void *src, unsigned int count);
用法：＃i nclude
功能：由src所指内存区域复制count个字节到dest所指内存区域。
说明：src和dest所指内存区域不能重叠，函数返回指向dest的指针。



memset
原型：extern void *memset(void *buffer, int c, int count);
用法：＃i nclude
功能：把buffer所指内存区域的前count个字节设置成字符c。
说明：返回指向buffer的指针。

原文链接：http://www.azure.net.ru/read.php/137.htm 
