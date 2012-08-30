#include <stdio.h> 

int main()
{
	  int a,b,c;
	  a=b=c=10;
	  a=b==c;
	  printf("a=%d,b=%d,c=%d\n",a,b,c);
	  a=b>c>=100;
	  printf("a=%d,b=%d,c=%d\n",a,b,c);
    return 0; 
}
