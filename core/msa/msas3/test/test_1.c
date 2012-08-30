#include "stdio.h"
#include "math.h"

int main(int argc, char *argv[])
{
	//1
//	int x,y;
//	scanf("%d",&x);
//	if(x>=0)
//		if(x>0) y=1;
//		else y=0;
//	else y=-1;
//	printf("x=%d,y=%d\n",x,y);
	//判断是否为小写字母是的话直接输出 不是的话转成小写然后输出
//	char ch;
//	scanf("%c",&ch);
//	ch=(ch>='A'&&ch<='Z')?(ch+32):ch;
//	printf("%c",ch);
	//判断是否为闰年
//	int year,leap;
//	scanf("%d",&year);
//	if(year%4==0)
//	{
//		if(year%100==0)
//		{
//			if(year%400==0)
//				leap=1;
//			else leap=0;
//		}
//		else
//			leap=1;
//	}
//	else
//		leap=0;
//	if(leap)
//		printf("%d is ",year);
//	else
//		printf("%d is not ",year);
//	printf("a leap year\n");

	//解一元二次方程
	/*
	*变量le如何定义
	*/
//	float a,b,c,disc,x1,x2,realpart,imagpart;
//	scanf("%f,%f,%f",&a,&b,&c);
//	printf("The equation");
//	if(fabs(a)<=le-6)
//		printf("The eqadratic");
//	else 
//		disc=b*b-4*a*c;
//	if(fabs(disc)<=le-6)
//		printf("has two equal roots:%8.4f\n",-b/(2*a));
//	else if(disc>le-6)
//	{
//		x1=(-b+sqrt(disc))/(2*a);
//		x2=(-b-sqrt(disc))/(2*a);
//		printf("has distinct real roots:%8.4f and %8.4f\n",x1,x2);
//	}
//	else
//	{
//		realpart=-b/(2*a);
//		imagpart=sqrt(-disc)/(2*a);
//		printf("has complex roots:\n")
//		printf("%8.4f+%8.4fi\n",realpart,imagpart);
//		printf("%8.4f-%8.4fi\n",realpart,imagpart);
//	}
	////求最低运费
//	int c,s;
//	float p,w,d,f;
//	scanf("%f,%f,%d",&p,&w,&s);
//	if(s>3000) c=12;
//	else c=s/250;
//	switch(c)
//	{
//		case 0:d=0;break;
//		case 1:d=2;break;
//		case 2:
//		case 3:d=5;break;
//		case 4:
//		case 5:
//		case 6:
//		case 7:d=8;break;
//		case 8:
//		case 9:
//		case 10:
//		case 11:d=10;break;
//		case 12:d=15;break;
//	}
//	f=p*w*s*(1-d/100.0);
//	printf("freight=%15.4f",f);

	//从1加到100   goto语句
//	int i,sum=0;
//	i=1;
//	loop:if(i<=100)
//	{
//		sum=sum+i;
//		i++;
//		goto loop;
//	}
//		printf("%d",sum);

	//从1加到100     while语句
//	int i,sum=0;
//	i=1;
//	while(i<=100)
//	{
//		sum=sum+i;
//		i++;
//	}
//		printf("%d",sum);

	//从1加到100 do——while语句
//	int i,sum=0;
//	i=1;
//	do{
//		sum=sum+i;
//		i++;
//	}
//	while(i<=100);
//		printf("%d",sum);

	//从1加到100 do——while语句
//	int i,sum=0;
//	for(i=1;i++;i<=100)
//	{
//		sum=sum+i;
//		i++;
//	}
	//输出100-200之间不能被三整除的数
//	int n;
//	for(n=100;n<=200;n++)
//	{
//		if(n%3==0)
//			continue;
//		printf("%d\n",n);
//	}
	
 	//一维数组
//	int i,a[10];
//	for(i=0;i<=9;i++)
//		a[i]=i;
//	for(i=9;i>=0;i--)
//		printf("%d",a[i]);

	//fibonacci数组
//	int i;
//	static int f[20]={1,1};
//	for(i=2;i<20;i++)
//	{
//		f[i]=f[i-2]+f[i-1];
//	}
//	for(i=0;i<20;i++)
//	{
//		if(i%5==0)printf("\n");
//		printf("%12d",f[i]);
//	}
	
	//输入十个数 并且按照从小到大排序
//	int a[11];
//	int i,j,t;
//	printf("input 10 numbers:\n");
//	for(i=1;i<11;i++)
//		scanf("%d",&a[i]);
//	printf("\n");
//	for(j=1;j<=9;j++)
//		for(i=1;i<=10-j;i++)
//		if(a[i]>a[i+1])
//		{
//			t=a[i];
//			a[i]=a[i+1];
//			a[i+1]=t;
//		}
//		printf("the sorted numbers:\n");
//		for(i=1;i<11;i++)
//			printf("%d",a[i]);

	//二维数组   将二维数组的行与列位置调换
//	static int a[2][3]={{1,2,3},{4,5,6}};
//	static int b[3][2],i,j;
//	printf("array a:\n");
//	for(i=0;i<=1;i++)
//	{
//		for(j=0;j<=2;j++)
//		{
//			printf("%5d",a[i][j]);
//			b[j][i]=a[i][j];
//		}
//		printf("\n");
//	}
//	printf("array b:\n");
//	for(i=0;i<=2;i++)
//	{
//		for(j=0;j<=1;j++)
//			printf("%5d",b[i][j]);
//		printf("\n");
//	}

	//有个3行四列的矩阵  求出最大元素的值以及其所在的行号和列号
//	int i,j,row,colum,max;
//	static int a[3][4]={{1,-2,3,4},{-3,-5,9,2},{5,6,-7,8}};
//	max=a[0][0];
//	for(i=0;i<=2;i++)
//		for(j=0;j<=3;j++)
//		{
//		  if(a[i][j]>max)
//			{
//				max=a[i][j];
//				row=i;
//				colum=j;
//			}
//			
//		}
//	printf("max=%d,row=%d,colum=%d\n",max,row,colum);

	//输出一个字符串
//	static char c[10]={'I',' ','a','m',' ','a',' ','b','o','y'};
//	int i;
//	for(i=0;i<10;i++)
//		printf("%c",c[i]);
//	printf("\n");

	//输入一个字符，判断有多少单词    get函数未定义 
//	char string[81];
//	int i,num=0,word=0;
//	char c;
//	gets(string);
//	for(i=0;(c=string[i])!='\0';i++)
//		if(c==' ')word=0;
//		else if(word==0)
//		{
//			word=1;
//			num++;
//		}
//		printf("There are %d words in the line\n",num);

	//函数调用
//	printstar();
//	print_message();
//	printstar();
	
	//调用函数输出最大值
	int a,b,c;
	scanf("%d,%d",&a,&b);
	c=max(a,b);
	printf("Max is %d\n",c);
	return 0;
}
max(x,y)
{
	int z;
	z=x>y?x:y;
	return (z);
}
printstar()
{
	printf("********************************\n");
}
print_message()
{
	printf("How do you do ! \n");
}
