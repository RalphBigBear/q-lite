CC = gcc
CFLAGS = -Wall -O2 -std=c99
LDFLAGS = -lcurl

TARGET = q-lite
SRCS = src/main.c src/http.c src/ollama.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

test: $(TARGET)
	./tests/test_basic.sh

.PHONY: all clean test
