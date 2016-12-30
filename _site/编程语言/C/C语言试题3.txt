1、 int a=5;
 a=(a=3*5,a*4),a+5;
 a=？
 注：赋值运算符要优先于逗号运算符，故 a = 60;
 
2、交换两个变量（整型）的值，且不允许使用中间值
 int a,b;
 a^=b;
 b^=a;
 a^=b;
 注：a,b必须为整型变量
 
3、int a=3;
 a+=a-=a*a;
 a=?
 “=”为自右向左结合，故a=a-a*a,a=-6; a=a+a,a=-12;
 
4、int a;
 scanf(” %d\n”,&a);
 注：使用scanf函数时，建议不要包含’\n’。如果这样使用，则在输入变量a的值时，应该按两次<Enter>键，因为字符’\n’的作用相当于<Enter>键。
 
5、switch语句的一般形式为：
 switch(表达式）
 {
 case 常量表达式1：语句1
 case 常量表达式2：语句2
 ……
 case 常量表达式n：语句n
 default:语句n+1
 }
 注：switch表达式所计算的结果必须为整型，常量表达式也必须是整型数值，且不能为变量。
 
6、写出float类型的变量i与零值比较的语句。
 if ((x >=0.00001) &&(x<=-0.00001))
 
7、
 （1）用逻辑表达式、for循环语句求解逻辑题。5位运动员参加了10米台跳水比赛，有人让他们预测比赛结果
 A选手说：B第一，我第三。
 B选手说：我第二，E第四。
 C选手说：我第一，D第二。
 D选手说：C最后，我第三。
 E选手说：我第四，A第一。
 比赛结束后，每位选手都说对了一半，请编程确定比赛的名次。
 （2）日本某地发生了一件谋杀案，警察通过排查确定杀人凶手必为4个嫌疑犯的一个。以下为4个嫌疑犯的供词。
 A说：不是我。
 B说：是C。
 C说：是D。
 D说：C在胡说
 已知3个人说了真话，1个人说的是假话。现在请根据这些信息，写一个程序来确定到底谁是凶手。
 解：
 #include<stdio.h>
 int main()
 {
 int i,sum=0,flag=0;
 char killer;
 for(i=1;i<=4;i++)
 {
 killer =64 +i;
 sum = (killer !=’A')+(killer ==’C')+(killer == ‘D’) +(killer!=’D');
 if(sum ==3)
 {
 flag=1;
 printf(“%c is the killer.\n”,killer);
 break;
 }
 }
 if(flag ==0)
 printf(“Can not find\n”);
 return 0;
 }
 
8、
 模块化准则就是把一个大问题分解为许多小问题，每个小问题由一个函数来解决，每个函数都完成一个特定的功能，各个小问题应该尽量独立，即所谓高内聚，低耦合。函数内部应该是高内聚，完成一些紧密相关的任务，函数之间应该只有一些非常必要的联系，即低耦合。
 
9、
 extern 的使用（变量和函数）
 源文件file1.c:
 #include<stdio.h>
 
extern long power(int);
 int A=2;
 main()
 {
 int n =10,total;
 total = power(n);
 return 0;
 }
 源文件file2.c
 extern A;   //外部声明 一般只只针对的是全局变量
 long power(int n)
 {
 long total =1;
 int i;
 for(i=1;i<=n;i++)
 total = total * A;
 return total;
 }
 
10、
 数组初始化：
 在所有函数外定义的数组的所有元素将被自动赋予初值0，在函数内部定义的数组，系统不会为其进行初始化，在使用数组元素前必须对元素进行初始化。
 
11、
 int a[5]={1,2,3,4,5};
 int *p=a;
 p = a +10 ；//不合法的, 因为该数组只有5个元素
 p= a +5 ; //合法的，虽然该数组只有5个元素，从a[0]到a[4],但p可以指向数组存储空间的下一个位置。但不能对该变量执行*p运算，即不能获得此时指针p所指向的变量的值。
 a++;//不合法的，不能对数组名执行++、–操作。这是因为a是数组名，它是数组的首地址，它的值在程序的运行过程中式固定不变的，是常量。
 注：在指针上进行加减运算后所得到的指针，必须指向同一个数组或指向数组存储空间的下一个单元。
 
12、
 区别int (*p)[5]和int *p[5]。
 前者是一个指针，它指向一个含有5个元素的数组。后者是一个数组，它的长度为5，数组中每一个元素指向一个整型变量。
 区别int *f( int i, int j)和 int (*p)( int i ,int j)
 前者是返回指针的函数，它是一个函数的声明，后者是指向函数的指针，它定义了一个指针。
 
13、
 void change(int i, int *p)
 {
 i++;
 if(p != NULL)
 (*p)++;
 }
 对于指针型形参，实参也可以是NULL，因此change函数中必须检查p是否为NULL。如果实参为NULL，那么语句（*p)++将导致程序崩溃。
 注：如果一个函数的参数中有指针，那么出于程序健壮性的考虑，在该函数中须检查参数是否为NULL.
 
14、
 函数指针的使用示例：
 #include <stdio.h>
 #define GET_MAX 0
 #define GET_MIN 1
 
int get_max(int i, int j)
 {
 return i>j?i : j ;
 }
 int get_min(int i, int j)
 {
 return i<j?i:j ;
 }
 int compare(int i, int j,int flag)
 {
 int ret;
 int (*p)(int,int);
 if(flag == GET_MAX)
 p = get_max;
 else
 p = get_min;
 ret = p(i,j);
 return ret;
 }
 int main()
 {
 int i =5,j = 10,ret;
 ret = compare(i,j,GET_MAX);
 printf(“The MAX is %d\n”,ret);
 return 0;
 }
 注：（1）不能对指向函数的指针做任何运算，如p++、p–、p+n、p-n都是错误的。
 （2）指向函数的指针能通过同类型的函数（即参数相同、返回类型相同）名、函数指针或NULL来进行初始化或赋值。将函数指针初始化或赋值为NULL，表示该指针目前不指向任何函数。
 函数指针做形参：
 #include<stdio.h>
 int get_big(int i, int j)
 {
 return i>j?i:j ;
 }
 int get_max(int i, int j,int k,int (*p)(int,int))
 {
 int ret;
 ret = p(i,j);
 ret = p(ret,k);
 return ret;
 }
 int main()
 {
 int i =5 ,j =10, k =15,ret;
 ret =get_max(i,j,k,get_big);
 return 0;
 }
 函数指针作函数返回值：
 #include <stdio.h>
 int get_big(int i, int j)
 {
 return i>j?i:j;
 }
 int (*get_function(int a))(int ,int )
 {
 printf(“the number is %d\n”,a);
 return get_big;
 }
 int main()
 {
 int i=5,j=10,max;
 int (*p)(int,int);
 p = get_function(100);
 max = p(i,j);
 printf(“The MAX is %d\n”,max);
 return 0;
 }
 注：int (*get_function(int a0)(int ,int)是一个返回函数指针的函数。这个可能比较难以理解。我们首先抓住get_function,因为运算符（）的优先级比*高，他的返回值是int(*)(int,int),也就是它返回的是一个指向函数的指针。该指针所指向的函数有两个整型参数。get_function中，将get_big作为函数的返回值。get_big是一个函数名，也是函数get_big的入口地址，他是一个指针。
 15、
 char string[] = “Linux C”;
 char *p = “Linux C”;
 //”Linux C”是一个字符串常量。C语言对于字符串常量通常是这样处理的：在内存中开辟一个字符数组来存储该字符串常量，并把开辟出的字符数组的首地址赋给p.
 注：string[0] = ‘a’是可以的，而p[0] = ‘a’是非法的，因为p指向的是字符串常量，常量的内容不可改变。把p指向一个字符串常量或字符数组时合法的，例如：p = “Hello World!”; p= string;
 16、
 下面这段小程序的输出是什么？
 #include<stdio.h>
 void main()
 {
 int a[5] = {1,2,3,4,5};
 int *ptr = (int *)(&a+1);
 printf(“%d,%d\n”,*(a+1),*(ptr-));
 }
 注： &a+1不是首地址+1，系统会认为加一个a数组的偏移，是偏移了一个数组的大小（本例是5个int）。对于int *ptr=(int *)(&a+1)，ptr是&(a[5]0,也就是a+5。ptr与(&a+1)类型是不一样的，所以ptr-1只会减去sizeof(int *)。a，&a的地址是一样的，但含义不一样，a是数组首地址，也就是a[0]的地址，&a是对象（数组）首地址，a+1是数组下一元素的地址，即a[1]，&a+1是下一个对象的地址，即a[5]。*(a+1)就是a[1] ，*(ptr-1)是a[4]，执行结果是2，5。
 17、
 假设你只知道一个数组的数组名（a[]),如何确定这个数组的长度？
 int length = sizeof(a)/sizeof(a[0]);
 18、
 下面这个程序编译时会报错，请指出错误并改正。
 #include<stdio.h>
 int main(void)
 {
 int **p;
 int arr[100];
 p = &arr;
 return 0;
 }
 &arr是一个指向长度为100的数组的指针，而p是指向指针（该指针指向的是int型变量）的指针。主函数可以改为：
 int main(void)
 {
 int **p,*q;
 int arr[100];
 q=arr;
 p =&q;
 return 0;
 }
 用指针q进行过渡。
 注：两个指针只有在所指向的数据的类型一致时才可以相互赋值。
 19、
 写一个程序，以递归方式反序输出一个字符串。如给定字符串“abc”输出“cba ”。
 #include<stdio.h>
 void reverse(char *p)
 {
 if( *p == ‘\0′)
 return;
 reverse(p+1);
 printf(“%c”,*p);
 }
 int main()
 {
 reverse(“abc”);
 printf(“\n”);
 return 0;
 }
 20、写一个递归程序，判断数组a[n]是否是一个递增的数组。
 #include<stdio.h>
 int fun( int a[],int n)
 {
 if(n ==1)
 return 1;
 if( n == 2)
 return (a[n-1]>=a[n-2];
 return (fun(a,n-1)&&(a[n-1]>=a[n-2]));
 }
 int main()
 {
 int a[] = {1,2,3,4,5,6};
 if( fun(a,sizeof(a)/sizeof(a[0]) == 1)
 printf(“a:ok\n”);
 else
 printf(“a:no\n”);
 return 0;
 }
 21、
 写一个函数，它的原型是：
 int findnumstring (char *outputsr, char *intputstr)
 功能：在字符串中找出连续最长的数字串，把这个串的长度返回，并把这个最长数字串赋给其中一个函数参数outputstr所指内存。
 例如：“abcd12345eee125sss123456789 “的首地址传给 intputstr后，函数将返回9，outputstr所指的值为123456789。
 #include<stdio.h>
 #include<string.h>
 #include<stdlib.h>
 int findnumstring(char *outputstr,char *inputstr)
 {
 char *in = inputstr , *out = outputstr , *temp , *final;
 int count = 0,maxlen = 0;
 while( *in != ‘\0′)
 {
 if( *in > 47 && *in <58）
 {
 for( temp = in; *in >47 && *in <58; in++)
 count ++;
 }
 else
 in++;
 if( maxlen < count )
 {
 maxlen = count;
 final = temp;
 }
 count = 0 ;
 }
 for( int i=0;i <maxlen;i++)
 {
 *out = *final;
 out++;
 final++;
 }
 *out = ‘\0′;
 return maxlen;
 }
 void main()
 {
 char string[] =”abcd12345eee125ss123456789″;
 char *p = (char *)malloc( strlen(string) +1);
 int count = findnumstring(p,string);
 printf(“%s\n number string length =%d\n”,p ,count);
 }
 22、
 写一个实现字符串拷贝的函数。给定字符串拷贝函数strcpy的原型：
 char *strcpy(char *dest,const char *src);
 要求：（1）不调用任何库函数。（2）说明函数为什么返回char *.
 char *strcpy(char *dest,char *src)
 {
 if( (dest == NULL) || (src == NULL) )
 {
 return NULL;
 }
 char *ret_string = dest;
 while( *dest ++ = *src++)!=’\0′);
 return ret_string;
 }
 注：返回char * 指针的原因是为了实现链式表达式，如：
 int length = strlen( strcpy( dest, “hello world”) );
 23、
 练习题：
 （1）用递归的方法求一个有n个元素的int型数组的最大值。
 （2）利用数组可以实现高精度计算，方法是将大整数每位上的数字存储为数组的一个元素。对于：
 m= 88200807199688
 n= 345678912345678
 编写函数，实现大整数m、n的加、减、乘运算。
 （3）约瑟夫问题：
 古代某法官要判决n个犯人死刑，他有一条荒唐的逻辑，将犯人首尾的相接排成圆圈，然后从第s个人开始数起，每数到第m个犯人，就拉出来处决；然后又数m个，数到的犯人又拉出来处决，依次类推。剩下的最后一人可以豁免。
 编写程序，给出处决顺序，并给出哪一个人可以活下来。
 （4）编写一个函数，求两个字符串的长度最大的公共子串。
 24、
 宏的作用范围是从宏定义开始到本源程序文件结束为止。也可以使用#undef来提前终止作用范围。例如：
 #define MAX 256
 int main()
 {
 ………
 }
 #undef MAX
 int f()
 {
 …..
 }
 由于使用了#undef，使宏名MAX只在main函数中有效。
 25、编程小技巧
 示例：#ifdef DEBUG
 printf(“a=%d,b=%d”,a,b);
 #endif
 在调试程序时，可以在源程序头部加入如下语句：
 #define DEBUG
 这样在软件开发阶段，编译运行程序时会输出变量a,b的值。当程序调试完毕，在源程序文件头部删除这一行，则用户运行时不会输出a,b的值。这里打印出a,b值只是供调试使用。
 26、
 不能把共用体变量作为函数参数，也不能使函数返回共用体变量，但可以使用指向共用体变量的指针。
 27、
 typedif int NUMBER[10];
 NUMBER 声明为含有10个元素的数组类型。
 NUMBER n;
 n[0] =1;
 n为含有10个元素的数组。
 28、
 C语言允许在一个结构体重以位为单位来使用内存，这种以为为单位的成员称为位域或位段。
 struct bit_data
 {
 int a:6;
 int b:4;
 int c:4;
 int d;
 }
 注：0~5的6位存放a，6~9的4位存放b,10~13的4位存放c，14~31的18位空闲，32~63的32位存放d.
 若某一位段要从另外一个存储单元开始存放，结构体中的成员可以定义成如下形式：
 int a:6;
 int b:4;
 int :0;
 int c:4;
 int d;
 注：a使用0~5的6位，b使用6~9的4位。c从下一个存储单元开始存放，即存放在32~35位，10~31位空闲。
 struct bit_data
 {
 int a:6;
 int b:4;
 int :8;
 int c:4;
 int d;
 };
 注：0~5位存放a，6~9位存放b，10~17位的8位强制空闲，18~21位存放c,22~31位的10位也空闲。32~63的32位存放d。
 29、
 快速统计给定整数二进制形式中1的个数.
 int func( int x）
 {
 int count = 0;
 while (x)
 {
 count ++;
 x = x & (x-1);
 }
 return count;
 }
 如输入9999，该函数返回8。
 注：9999 = 9 x1024 +512 +256 +15
