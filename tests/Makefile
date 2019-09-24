CC := gcc

ifeq ($(debug), y)
    CFLAGS := -g
else
    CFLAGS := -O2 -DNDEBUG
endif

CFLAGS := $(CFLAGS) -Wall -Werror
LIBS := ../libutils.a -lpthread
INCLUDE := -I..

OBJS := $(patsubst %.c, %.o, $(wildcard *.c))

TARGET := test_rbtree test_str_util test_hash

.PHONY: all clean pre-process post-clean

all: $(TARGET)

$(OBJS): | pre-process

pre-process:
	$(MAKE) -C ..

post-clean:
	$(MAKE) clean -C ..

test_rbtree: test_rbtree.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

test_str_util: test_str_util.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

test_hash: test_hash.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean: | post-clean
	rm -f $(TARGET) $(OBJS)
