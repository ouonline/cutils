CC := gcc
CFLAGS := -Wall -g

TARGET := rbtree-test atomic-test

.PHONY: all clean

all: $(TARGET)

rbtree-test: test-rbtree.o rbtree.o
	$(CC) $(CFLAGS) -o $@ $^
rbtree.o: rbtree.c
	$(CC) $(CFLAGS) -c $<

atomic-test: test-atomic.o
	$(CC) $(CFLAGS) -o $@ $^ -lpthread

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(TARGET) *.o
