#include <stdio.h>
#include "m_print.h"

void local()
{
	int a = 1;
	printf("%p\n",&a);
	m_print(&a);
}

int main()
{
	local();
	return 1;
}


