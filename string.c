#include <stdio.h>
#include <string.h>

int main() {
	const char *a;
	char const *p;
	a = "yifan";
	char e[10] = "yifan";
	p = a;
	char b[10] = "yifan";
	char *c = b;
	printf("%s\n", a);
	printf("%s\n", b);
	//strlen and sizeof
	int l = strlen(e);
	int n = sizeof(e);
	a = "lianxin";
	*(b + 1) = 'd';
	printf("123 %s 456\n", a);
	printf("%s\n", b);
	getchar();
}
