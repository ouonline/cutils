CC := gcc
CFLAGS := -O2 -Wall -Werror

OBJS := $(patsubst %.c, %.o, $(wildcard *.c))

TARGET := rbtree-test atomic-test

.PHONY: all clean

all: $(TARGET)

rbtree-test: test-rbtree.o rbtree.o
	$(CC) $(CFLAGS) -o $@ $^

atomic-test: test-atomic.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
