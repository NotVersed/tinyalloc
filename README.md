# tinyalloc

A custom heap memory allocator written in C from scratch, without using `malloc` or `free`.
Implements a free list allocator with first-fit and best-fit strategies, block coalescing, and heap introspection.

## How it works

### The heap layout

Every allocation consists of a small **block header** followed by the usable memory the caller receives:

low address                                         high address
┌─────────────────┬──────────────┬─────────────────┬──────────────┐
│   Block header  │  user data   │   Block header  │  user data   │
│   (24 bytes)    │  (n bytes)   │   (24 bytes)    │  (n bytes)   │
└─────────────────┴──────────────┴─────────────────┴──────────────┘
^                                ^
head                             head->next

The header stores three fields:

| Field  | Type         | Description                        |
|--------|--------------|------------------------------------|
| size   | size_t       | bytes of user data following header |
| free   | int          | 1 if available, 0 if in use         |
| next   | Block *      | pointer to next block in list       |

### Allocation

`my_malloc(size)` first searches the free list for a reusable block. If none is found it calls `sbrk()` to extend the heap and claim new memory from the OS.

### Freeing

`my_free(ptr)` marks the block as free by flipping `free = 1`. It then coalesces any adjacent free blocks into one larger block to prevent fragmentation.

### Strategies

| Strategy   | Behavior                                      | Tradeoff                        |
|------------|-----------------------------------------------|---------------------------------|
| FIRST_FIT  | Returns the first block that is big enough    | Fast, but wastes more memory    |
| BEST_FIT   | Returns the smallest block that fits exactly  | Less waste, but slower to find  |

Switch strategies at runtime:
```c
set_strategy(BEST_FIT);
void *ptr = my_malloc(8);
```

## Building

```bash
make
./tinyalloc
```

## Example output
Block 0: header=0x... data=0x... size=4  free=0
Block 1: header=0x... data=0x... size=32 free=1
Block 2: header=0x... data=0x... size=8  free=1