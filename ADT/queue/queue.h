#ifndef QUEUE_H
#define QUEUE_H

#define QUEUE_TYPE int

void insert(QUEUE_TYPE value);

void delete(void);

QUEUE_TYPE first(void);

int is_empty(void);

int is_full(void);

#endif