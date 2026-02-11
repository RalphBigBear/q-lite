CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LDFLAGS =

TARGET = q-lite
SRCS = src/main.c src/http.c src/ollama.c src/mem-profile.c src/backend.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"
	@echo "Binary size:"
	@ls -lh $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Cleaned build artifacts"

.PHONY: all clean
