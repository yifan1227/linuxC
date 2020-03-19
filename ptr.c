#include <stdio.h>

void My_swap(int *a, int *b);
void My_swap2(int, int);
void My_swap3(int &a, int &b);
void ptr_to_ptr(int** ptr);

int main()
{
	int a = 1;
	int b = 3;
	int *c = &a;
	int *d = &b;
	My_swap(c, d);	 //swaped
	My_swap2(a, b);  //not swaped
	My_swap3(a, b);	 //swaped
	int **ptr = &c;
	ptr_to_ptr(ptr); //**ptr
	int array[5] = { 0,1,2,3,4 };	//It shows that pointer to 2d array is a special secondary pointer where p = *p, *p means enter the specific line. p+1=p+columns*sizeof(type),*p+1=p+sizeof(type).
	printf("array = 0x%08X\n", array);	//It's special here. The array name is a pointer but its own address equals its value. 数组名的两层意义：1.数组名，&a表示整个数组的地址 2.首元素的地址
	printf("&array = 0x%08X\n", &array);
	//printf("*(&array) = 0x%08X\n", *(&array));
	printf("*array = %d", *array);
	getchar();
	return 0;
}

void My_swap(int *a, int *b)
{
	int t;
	t = *a;
	*a = *b;
	*b = t;
	return;
}

void My_swap2(int a, int b)
{
	int t;
	t = a;
	a = b;
	b = t;
	return;
}

void My_swap3(int &a, int &b)
{
	int t;
	t = a;
	a = b;
	b = t;
	return;
}

void ptr_to_ptr(int** ptr)
{
	printf("ptr = 0x%08X\n", ptr);
	printf("*ptr = 0x%08X\n", *ptr);
	printf("**ptr = %d\n", **ptr);
	return;
}