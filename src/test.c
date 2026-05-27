#include "alloc.h"
#include <stdio.h>

int main() {
  printf("--- Test 1: Basic allocation ---\n");
  int *a = (int *)my_malloc(sizeof(int));
  *a = 42;
  printf("a = %d\n", *a);
  my_dump();

  printf("\n--- Test 2: Multiple allocations ---\n");
  int *b = (int *)my_malloc(sizeof(int));
  int *c = (int *)my_malloc(sizeof(int));
  *b = 100;
  *c = 200;
  printf("b = %d, c = %d\n", *b, *c);
  my_dump();

  printf("\n--- Test 3: Free and reallocate ---\n");
  my_free(b);
  int *d = (int *)my_malloc(sizeof(int));
  *d = 999;
  printf("d = %d (should have reused b's block)\n", *d);
  printf("b and d same address? %s\n", b == d ? "YES" : "NO");
  my_dump();

  printf("\n--- Test 4: String allocation ---\n");
  char *str = (char *)my_malloc(6);
  str[0] = 'h';
  str[1] = 'e';
  str[2] = 'l';
  str[3] = 'l';
  str[4] = 'o';
  str[5] = '\0';
  printf("str = %s\n", str);
  my_dump();

  printf("\n--- Test 5: Coalescing ---\n");
  int *e = (int *)my_malloc(4);
  int *f = (int *)my_malloc(4);
  printf("Before free:\n");
  my_dump();
  my_free(e);
  my_free(f);
  printf("After freeing both:\n");
  my_dump();
  int *g = (int *)my_malloc(8);
  *g = 12345;
  printf("Allocated 8 bytes after coalescing:\n");
  my_dump();

  printf("\n--- Test 6: Best-fit strategy ---\n");
  set_strategy(FIRST_FIT);

  // allocate 8 blocks
  int *p1 = (int *)my_malloc(4);
  int *sep1 = (int *)my_malloc(4);
  int *p2 = (int *)my_malloc(32);
  int *sep2 = (int *)my_malloc(4);
  int *p3 = (int *)my_malloc(8);
  int *sep3 = (int *)my_malloc(4);
  int *p4 = (int *)my_malloc(64);

  // use separators to prevent coalescing
  *sep1 = 1;
  *sep2 = 2;
  *sep3 = 3;

  // free alternating blocks, separators stay in use
  my_free(p1);
  my_free(p2);
  my_free(p3);
  my_free(p4);

  printf("Free blocks separated by in-use blocks:\n");
  my_dump();

  int *first = (int *)my_malloc(8);
  printf("First-fit chose block at: %p\n", (void *)first);
  my_free(first);

  set_strategy(BEST_FIT);
  int *best = (int *)my_malloc(8);
  printf("Best-fit chose block at:  %p\n", (void *)best);
  my_free(best);

  return 0;
}