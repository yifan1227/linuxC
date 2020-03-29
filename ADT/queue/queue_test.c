#include "queue.h"
#include <stdio.h>

int main()
{
    insert(1);
    insert(2);
    insert(3);
    printf("First is %d\n", first());
    delete();
    printf("First after pop() is %d\n", first());
    getchar();
    return 0;
}
