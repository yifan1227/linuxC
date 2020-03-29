#include "stack.h"
#include <stdio.h>

int main()
{
    push(1);
    push(2);
    push(3);
    printf("Top is %d\n", top());
    getchar();
    return 0;
}
