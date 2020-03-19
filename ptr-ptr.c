#include <stdio.h>

void swap_F(int, int);
void swap_ptr_F(int *, int *);
void swap_ptr_T(int **, int **);

int main()
{
	int a = 1;
	int b = 3;
	printf("*****************************\n");
	printf("&a = 0x%08X\n", &a);
	printf("&b = 0x%08X\n", &b);
	swap_F(a, b);
	int *p = &a;
	int *q = &b;
	printf("*****************************\n");
	printf("&p = 0x%08X\n", &p);
	printf("&q = 0x%08X\n", &q);
	swap_ptr_F(p, q);
	printf("*****************************\n");
	printf("p = 0x%08X\n", p);
	printf("q = 0x%08X\n", q);
	swap_ptr_T(&p, &q);
	getchar();
	return 0;

}

void swap_F(int a, int b)
{
	int t;
	printf("&a = 0x%08X\n", &a);
	printf("&b = 0x%08X\n", &b);
	t = a;
	a = b;
	b = t;
	return;
}

void swap_ptr_F(int *a, int *b)
{
	int *t;
	printf("&p = 0x%08X\n", &a);
	printf("&q = 0x%08X\n", &b);
	t = a;
	a = b;
	b = t;
	return;
}

void swap_ptr_T(int **a, int **b)
{
	int *t;
	t = *a;
	*a = *b;
	*b = t;
	printf("p = 0x%08X\n", *a);
	printf("q = 0x%08X\n", *b);
	return;
}