#include <stdio.h>

int main()
{
	int a[2][3] = { 1,2,3,4,5,6 };
	int(*p)[3] = a;
	printf("p = 0x%08X\n", p);
	printf("p+1 =  0x%08X\n", p + 1);
	printf("*p =  0x%08X\n", *p);				//现在表示整个数组的地址
	printf("*p+1 =  0x%08X\n", *p + 1); 		//取一次*表示首元素地址（进入该数组），取第二次*得到元素值
	printf("*(p+1)=  0x%08X\n", *(p + 1));
	printf("**p = %d\n", **p);
	printf("**(p+1)= %d\n", **(p + 1));
	getchar();
	return 1;
}