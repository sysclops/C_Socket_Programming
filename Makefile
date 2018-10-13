TARGET=server
CC=gcc

CFLAGS=-Wall -Werror -Wextra -std=c99
LDFLAGS=

SRCS=$(wildcard *.c)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(TARGET)
