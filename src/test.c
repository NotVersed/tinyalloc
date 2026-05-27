#include <stdio.h>
#include "alloc.h"

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

    return 0;
}