　　I/O函数包
 
　　FastCGI软件开发套件，开源的WebServer 2.0服务器包含I/O函数包用与简化把已存在的CGI程序转成FastCGI程序或者编写FastCGI程序，在开发套件中有两个函数包：fcgi_stdio 和 fcgiapp，在你的程序中必须包含这些包中的一个：
 
　　fcgi_stdio.h fcgiapp.h fcgi_stdio包，是fcgiapp包的顶层包，在转化CGI程序或者是写新的FastCGI程序是，我们强烈推荐你用它，fcgi_stdio包有以下几个优点：
 
　　简单：只要有三个性的API需要学。
 
　　易懂：如果你正包CGI程序转化为FastCGI程序，你会发现CGI程序与FastCGI程序之间只有很少的区别。在我们设计函数库的时候我们尽可能的 把FastCGI应用程序变得容易理解，以至于我们在建立新FastCGI程序的时候我们使用相同的环境变量，相同的解析查询字符串的技术，以及相同的 I/O程序等。
 
　　方便：这个库函数提供了CGI和FastCGI二进制的文件的完美兼容。因此不管是CGI还是FastCGI，都同样运行。
 
　　代码结构
 
　　FastCGI的代码构成，把你的代码分成两个独立部分：
 
　　1．初始化部分：只执行一次
 
　　2．应答循环部分：FastCGI脚本每被调用一次，这部分九被执行一次
 
　　
 
　　一个应答循环的典型格式如下：
 
　　while (FCGI_Accept() >= 0) {//循环条件
 
　　
 
　　# 应答循环体
 
　　
 
　　}
 
　　
 
　　知道一个客户端请求来的时候FCGI_Accept块才执行，并返回0。如果有一个系统故障，或是系统管理员终止进程，Accept将返回-1。
 
　　如果应用程序作为一个CGI程序被调用，那么第一次调用Accept时，返回0，第二次总是返回-1，产生CGI行为。（请详见20页的"FCGI_Accept (3)" ）
 
　　注意，在CGI中鼓励用小脚本，然而在FastCGI中则鼓励使用组合式的脚本。你可以在从新构想你的程序的全局结构，来获得FastCGI的高性能。
 
　　例1: TinyFastCGI
 
　　这是一个用C语言写的一个简单FastCGI应答程序例子：
 
　　#include "fcgi_stdio.h"
 
　　
 
　　#include <stdlib.h>
 
　　
 
　　int count;
 
　　
 
　　void initialize(void)
 
　　{
 
　　count=0;
 
　　}
 
　　
 
　　void main(void)
 
　　{
 
　　
 
　　initialize();
 
　　
 
　　
 
　　while (FCGI_Accept() >= 0) {
 
　　printf("Content-type: text/html\r\n"
 
　　"\r\n"
 
　　"<title>FastCGI Hello! (C, fcgi_stdio library)</title>"
 
　　"<h1>FastCGI Hello! (C, fcgi_stdio library)</h1>"
 
　　"Request number %d running on host <i>%s</i>\n",
 
　　++count, getenv("SERVER_HOSTNAME"));
 
　　}
 
　　}
 
　　例2：原始数据产生器
 
　　思考一下，一个应答应用程序产生第N次原始数据。
 
　　一个CGI应用程序将没有有效的方法来解决这个问题。例如，如果用户访问第50000次的原始数据，那么CGI应用程序就不许从第一条原始数据开始计算，知道第50000条的，要是应用程序终止，伴随着她辛苦积累的数据也会随之消失。
 
　　如果一个客户端想访问第4900条原始数据，那么服务器必须重新开始积累。
 
　　由于我们能维持这个状态，FastCGI应用程序对与这样的问题就更有效。一个FastCGI应用程序在初始化阶段能够计算一个扩展的源数据的表，并保持表的不同范围。当客户端请求一个特别原始数据的时候，循环应答需要从表里查询。
 
　　这里有一个原始数据代码事例：
 
　　#include "fcgi_stdio.h"
 
　　#include <stdlib.h>
 
　　#include <string.h>
 
　　
 
　　#define POTENTIALLY_PRIME 0
 
　　#define COMPOSITE 1
 
　　#define VALS_IN_SIEVE_TABLE 1000000
 
　　#define MAX_NUMBER_OF_PRIME_NUMBERS 78600
 
　　
 
　　
 
　　long int sieve_table[VALS_IN_SIEVE_TABLE];
 
　　long int prime_table[MAX_NUMBER_OF_PRIME_NUMBERS];
 
　　
 
　　void
 
　　initialize_prime_table(void)
 
　　{
 
　　long int prime_counter=1;
 
　　long int current_prime=2, c, d;
 
　　
 
　　prime_table[prime_counter]=current_prime;
 
　　
 
　　while (current_prime < VALS_IN_SIEVE_TABLE) {
 
　　
 
　　for (c = current_prime; c <= VALS_IN_SIEVE_TABLE;
 
　　c += current_prime) {
 
　　sieve_table[c] = COMPOSITE;
 
　　}
 
　　
 
　　
 
　　for (d=current_prime+1; sieve_table[d] == COMPOSITE; d++);
 
　　
 
　　prime_table[++prime_counter]=d;
 
　　current_prime=d;
 
　　}
 
　　}
 
　　void main(void)
 
　　{
 
　　char *query_string;
 
　　long int n;
 
　　
 
　　initialize_prime_table();
 
　　
 
　　while(FCGI_Accept() >= 0) {
 
　　
 
　　printf("Content-type: text/html\r\n"
 
　　"\r\n");
 
　　
 
　　printf("<title>Prime FastCGI</title>\n"
 
　　"<h1>Prime FastCGI</h1>\n");
 
　　query_string = getenv("QUERY_STRING");
 
　　if(query_string == NULL) {
 
　　printf("Usage: Specify a positive number in the query string.\n");
 
　　} else {
 
　　query_string = strchr(query_string, `=') + 1;
 
　　n = strtol(query_string);
 
　　if(n < 1) {
 
　　printf("The query string `%s' is not a positive number.\n",
 
　　query_string);
 
　　} else if(n > MAX_NUMBER_OF_PRIME_NUMBERS) {
 
　　printf("The number %d is too large for this program.\n", n);
 
　　} else{
 
　　printf("The %ldth prime number is %ld.\n", n, prime_table[n]);
 
　　}
 
　　}
 
　　}
 
　　}
 
　　这个应用程序在初始化时有一个显而意见的开销，但是后来的访问是快速的。
