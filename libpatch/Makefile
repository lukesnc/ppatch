SRC = $(wildcard ./src/*.c)
SRC += $(wildcard ./src/bps/*.c)
SRC += $(wildcard ./src/ips/*.c)
SRC += $(wildcard ./src/ups/*.c)
SRC += $(wildcard ./src/common/*.c)

OBJ = $(SRC:.c=.o)

CFLAGS = -std=c99 -c

libpatch.a: $(OBJ)
	ar rcs $@ $^

%.o: %.c
	$(CC) -std=c99 -c $< -o $@
