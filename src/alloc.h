#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>

typedef struct Block{
    size_t size; // size of block
    int free; // 1 if free, 0 if in use
    struct Block *next; // next block in list
} Block;

typedef enum {
    FIRST_FIT,
    BEST_FIT
} Strategy;

void set_strategy(Strategy s);

void *my_malloc(size_t size);
void my_free(void *ptr);
void my_dump(); // print the heap state

#endif