CFLAGS = -Wall -Wextra -L./libpatch/ -lpatch

ppatch: main.c
	$(CC) $(CFLAGS) -o $@ $^
