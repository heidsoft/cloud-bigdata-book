// 字符串拷贝函数的实现
char *strcpy(char *dest, const char *src)
{
	if ( !dest || !src )
	{
		return NULL ;
	}

	if ( dest == src)
	{
		return dest ;
	}

	char *temp = dest ;

	while( (*src) != '/0')
	{
		*dest = *src;
		src++;
		dest++;
	}

	dest = temp;
	return dest;
}