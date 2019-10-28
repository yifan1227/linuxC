#include <stdio.h>
#include "m_print.h"

void m_print(int *a)
{
        printf("%p\n", a);
        printf("%d\n", *a);
}

