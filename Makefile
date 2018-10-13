TARGET=server
CC=gcc
SRCS=$(wildcard *.c)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) -o $@ $^

.PHONY: clean
clean:
	rm -f $(TARGET)
