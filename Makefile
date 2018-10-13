TARGET=server
CC=gcc

CFLAGS=-Wall -Werror -Wextra
LDFLAGS=

SRCS=$(wildcard *.c)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(TARGET)
