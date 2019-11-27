#include <stdio.h>

int main()
{
	char const *ptr[] = {
		"hello",
		"world",
		"who",
		"are"};
	printf("sizeof ptr[]: %d, sizeof ptr[0]: %d\n", sizeof(ptr), sizeof(ptr[0]));
	for(int i = 0; i < 4; i++)
	{
		printf("ptr[%d] is %p, pointing to %s\n", i, ptr[i], ptr[i]);
	}
}

