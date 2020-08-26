# This Makefile is generated by omake: https://github.com/ouonline/omake.git

CC := gcc

ifeq ($(debug), y)
	CFLAGS += -g
else
	CFLAGS += -O2 -DNDEBUG
endif

AR := ar

TARGET := libutils_shared.so libutils_static.a

.PHONY: all clean distclean

all: $(TARGET)

omake_dep_0.hash.c.o: hash.c
	$(CC) $(CFLAGS) -Wall -Werror -Wextra -fPIC -c $< -o $@

omake_dep_0.qbuf.c.o: qbuf.c
	$(CC) $(CFLAGS) -Wall -Werror -Wextra -fPIC -c $< -o $@

omake_dep_0.rbtree.c.o: rbtree.c
	$(CC) $(CFLAGS) -Wall -Werror -Wextra -fPIC -c $< -o $@

omake_dep_0.str_utils.c.o: str_utils.c
	$(CC) $(CFLAGS) -Wall -Werror -Wextra -fPIC -c $< -o $@

omake_dep_0.time_utils.c.o: time_utils.c
	$(CC) $(CFLAGS) -Wall -Werror -Wextra -fPIC -c $< -o $@

utils_shared_OBJS := omake_dep_0.time_utils.c.o omake_dep_0.str_utils.c.o omake_dep_0.rbtree.c.o omake_dep_0.qbuf.c.o omake_dep_0.hash.c.o

libutils_shared.so: $(utils_shared_OBJS)
	$(CC) $(CFLAGS) -Wall -Werror -Wextra -fPIC -shared -o $@ $^

utils_static_OBJS := omake_dep_0.time_utils.c.o omake_dep_0.str_utils.c.o omake_dep_0.rbtree.c.o omake_dep_0.qbuf.c.o omake_dep_0.hash.c.o

libutils_static.a: $(utils_static_OBJS)
	$(AR) rc $@ $^

clean:
	rm -f $(TARGET) $(utils_shared_OBJS) $(utils_static_OBJS)

distclean:
	$(MAKE) clean
