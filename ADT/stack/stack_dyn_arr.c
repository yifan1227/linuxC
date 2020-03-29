#include "stack.h"
#include <assert.h>
#include <stdlib.h>

#ifdef DYN_ARR

static STACK_TYPE *stack;
static int top_element = -1;
static int stack_size;

void create_stack(int size)
{
    assert(stack_size == 0);
    stack_size = size;
    stack = (STACK_TYPE *)malloc(sizeof(size));
    assert(stack != NULL);
}

void destroy_stack(void)
{
    assert(stack_size > 0);
    stack_size = 0;
    free(stack);
    stack = NULL;
}

void push(STACK_TYPE value)
{
    assert(!is_full());
    top_element += 1;
    stack[top_element] = value;
}

void pop(void)
{
    assert(!is_empty());
    top_element -= 1;
}

STACK_TYPE top(void)
{
    assert(!is_empty());
    return stack[top_element];
}

/* tradional pop */
STACK_TYPE old_pop(void)
{
    assert(!is_empty());
    return stack[top_element--];
}

int is_empty(void)
{
    return top_element == -1;
}

int is_full(void)
{
    return top_element == stack_size -1;
}

#endif