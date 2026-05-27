CC = gcc
CFLAGS = -Wall -Wextra -g

all: tinyalloc

tinyalloc: src/alloc.c src/test.c
	$(CC) $(CFLAGS) src/alloc.c src/test.c -o tinyalloc

clean:
	rm -f tinyalloc