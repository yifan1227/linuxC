#include <stdio.h>

int main()
{
	int a[2] = {1, 2};
	int *b = &a;
	printf("%d \n", *b++);
	printf("%d \n", *++b);
	return 1;
}

