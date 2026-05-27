#include "alloc.h"
#include <stdio.h>
#include <unistd.h>

#define BLOCK_SIZE sizeof(Block)

static Block *head = NULL;

static Strategy current_strategy = FIRST_FIT;

void set_strategy(Strategy s) { current_strategy = s; }

static Block *find_free_block(size_t size) {
  Block *current = head;
  Block *best = NULL;

  while (current != NULL) {
    if (current->free && current->size >= size) {
      if (current_strategy == FIRST_FIT) {
        return current;
      }
      if (best == NULL || current->size < best->size) {
        best = current;
      }
    }
    current = current->next;
  }
  return best;
}

static Block *request_space(size_t size) {
  Block *block = sbrk(0);
  if (sbrk(BLOCK_SIZE + size) == (void *)-1) {
    return NULL;
  }
  block->size = size;
  block->free = 0;
  block->next = NULL;

  return block;
}

void *my_malloc(size_t size) {
  if (size == 0)
    return NULL;

  Block *block = find_free_block(size);
  if (block != NULL) {
    block->free = 0;
    return (void *)(block + 1);
  }

  block = request_space(size);
  if (block == NULL)
    return NULL;

  if (head == NULL) {
    head = block;
  } else {
    Block *current = head;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = block;
  }
  return (void *)(block + 1);
}

void my_free(void *ptr) {
  if (ptr == NULL)
    return;
  Block *block = (Block *)ptr - 1;
  block->free = 1;

  // coalesce adjacent free blocks
  Block *current = head;
  while (current != NULL && current->next != NULL) {
    if (current->free && current->next->free) {
      current->size = current->size + BLOCK_SIZE + current->next->size;
      current->next = current->next->next;
    } else {
      current = current->next;
    }
  }
}

void my_dump() {
  printf("\n--- Heap Dump ---\n");
  Block *current = head;
  int i = 0;
  while (current != NULL) {
    printf("Block %d: header=%p data=%p size=%zu free=%d\n", i, (void *)current,
           (void *)(current + 1), current->size, current->free);
    current = current->next;
    i++;
  }
  printf("--- End Dump ---\n\n");
}