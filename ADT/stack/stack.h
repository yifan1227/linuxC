#ifndef STACK_H
#define STACK_H

//#define ARR
//#define DYN_ARR
#define LINK

#define STACK_TYPE int

void push(STACK_TYPE value);

void pop(void);

STACK_TYPE top(void);

int is_empty(void);

int is_full(void);

/* for dynamic allocated stack */
void create_stack(int size);

void destroy_stack(void);

#endif