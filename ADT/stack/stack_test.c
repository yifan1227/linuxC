#include "stack.h"
#include <stdio.h>

int main()
{
    #ifdef DYN_ARR
    printf("Using dynamic array\n");
    create_stack(100);
    #endif
    #ifdef LINK
    printf("using linked list\n");
    #endif
    push(1);
    push(2);
    push(3);
    printf("Top is %d\n", top());
    pop();
    printf("Top after pop() is %d\n", top());
    getchar();
    return 0;
}
