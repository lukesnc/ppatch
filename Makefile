CFLAGS = -Wall -Wextra -L./libpatch/ -lpatch

.PHONY: all
all: ppatch

ppatch: main.c
	$(CC) $(CFLAGS) -o $@ $^
