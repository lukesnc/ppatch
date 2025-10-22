CFLAGS = -Wall -Wextra
LIBS = -Llibpatch -lpatch

.PHONY: all
all: ppatch

ppatch: main.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
