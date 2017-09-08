

关于sprintf和snprintf的正确使用。

考虑以下有缺陷的例子：
void f(const char *p)
{
    char buf[11]={0};
    sprintf(buf,"%10s",p); // very dangerous
    printf("%sn",buf);
}

不要让格式标记“%10s”误导你。如果p的长度大于10个字符，那么sprintf() 的写操作就会越过buf的边界，从而产生一个缓冲区溢出。
检测这类缺陷并不容易，因为它们只在 p 的长度大于10个字符的时候才会发生。黑客通常利用这类脆弱的代码来入侵看上去安全的系统。

要修正这一缺陷，可以使用函数snprintf()代替函数sprintf()。

函数原型：int snprintf(char *dest, size_t n, const char *fmt, ...);
函数说明: 最多从源串中拷贝n－1个字符到目标串中，然后再在后面加一个0。所以如果目标串的大小为n的话，将不会溢出。
函数返回值: 若成功则返回存入数组的字符数，若编码出错则返回负值。

推荐的用法：
void f(const char *p)
{
    char buf[11]={0};
    snprintf(buf, sizeof(buf), "%10s", p); // 注意：这里第2个参数应当用sizeof(str)，而不要使用硬编码11，也不应当使用sizeof(str)-1或10
    printf("%sn",buf);
} 
