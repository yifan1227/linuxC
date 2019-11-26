#include <stdio.h>

int main()
{
	int arr[][3]={{1,2,3},{4,5,6}};
	int **p = arr;
	int (*ptr)[3];
	printf("%p\n", p);
	printf("p + 1 is :%p\n", ++p);
	printf("**(p+1) is:%d\n", *p);
	ptr = arr;
	printf("%p\n", ptr);
	printf("%p\n", *ptr + 2);
	printf("%d\n", *(*ptr + 2));
	printf("p + 1 is :%p\n", ++ptr);
	printf("**(p+1) is:%d\n", *(*ptr)+1);
	return 1;
}
