#include <stdio.h>

int main()
{
	int a = 0x12345678;
	char *p = (char*) &a;		//pointer forced conversion
	printf("&a = 0x%08X\n", &a);
	for (int i = 0; i < 4; i++)
	{
		printf("p + %d = 0x%08X, *(p + %d) = 0x%2X\n", i, p + i, i, *(p + i));
	}
	getchar();
	return 1;
}