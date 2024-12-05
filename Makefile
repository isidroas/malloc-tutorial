CC = clang
FLAGS = -O0 -W -Wall -Wextra -g

tests_src=$(wildcard test/test-*.c)
tests_exe=$(patsubst %.c,%,$(tests_src))
all: malloc.so $(tests_exe) wrapper

malloc.so: malloc.c
	$(CC) $^ $(FLAGS) -o $@ -shared -fPIC

test-%: test-%.c
	$(CC) $^ $(FLAGS) -o $@

wrapper: wrapper.c
	$(CC) $^ $(FLAGS) -o $@
