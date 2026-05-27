#include "alloc.h"
#include <unistd.h>

#define BLOCK_SIZE sizeof(Block)

static Block *head = NULL;

static Block *find_free_block(size_t size){
    Block *current = head;
    while(current != NULL){
        if(current->free && current->size >= size){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

static Block *request_space(size_t size){
    Block *block = sbrk(0);
    if(sbrk(BLOCK_SIZE + size) == (void *) -1){
        return NULL;
    }
    block->size = size;
    block->free = 0;
    block->next = NULL;

    return block;
}

void *my_malloc(size_t size){
    if(size == 0) return NULL;

    Block *block = find_free_block(size);
    if(block != NULL){
        block->free = 0;
        return (void *)(block + 1);
    }

    block = request_space(size);
    if(block == NULL) return NULL;

    if(head == NULL){
        head = block;
    }
    else{
        Block *current = head;
        while(current->next != NULL){
            current = current->next;
        }
        current->next = block;
    }
    return (void*)(block + 1);
}

void my_free(void *ptr){
    if(ptr == NULL) return;
    Block *block = (Block *)ptr-1;
    block->free = 1;
}