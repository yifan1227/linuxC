#include <stdio.h>

int main()
{
	int a[] = { 1,2,3,4,5 };
	int *p = *(&a+1);
	printf("0x%08X, 0x%08X\n", &a, &a+1);
	printf("%d", *(p - 1));
	getchar();
	return 1;
}