#include "stack.h"
#include <assert.h>
#include <stdlib.h>

#ifdef LINK

typedef struct STACK_NODE{
    STACK_TYPE value;
    struct STACK_NODE *next;
}stack_node;

static stack_node *stack;

void push(STACK_TYPE value)
{
    stack_node *new_node;

    new_node = malloc(sizeof(stack_node));
    assert(new_node != NULL);
    new_node->next = stack;
    new_node->value = value;
    stack = new_node;
}

void pop(void)
{
    stack_node *first_node;

    assert(!is_empty());
    first_node = stack;
    stack = first_node->next;
    free(first_node);
}

STACK_TYPE top(void)
{
    assert(!is_empty());
    return stack->value;
}

int is_empty(void)
{
    return stack == NULL;
}

#endif