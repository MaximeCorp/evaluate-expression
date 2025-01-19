CC=gcc
CFLAGS=-Wall -Werror -Wextra -pedantic -std=c99 -Wvla

SRCS=$(wildcard src/*c)
OBJS=$(patsubst %.c,%.o, $(SRCS))

all: $(OBJS)
	$(CC) -o evalexpr $^

check:
	./tests/test.sh

clean:
	rm -f evalexpr $(OBJS)

test:
	$(CC) $(CFLAGS) -fsanitize=address --coverage -fPIC -lgcov  $(SRCS) -o evalexpr

.PHONY: all clean check
