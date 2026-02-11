CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LDFLAGS =

TARGET = q-lite
SRCS = src/main.c src/http.c
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

run: $(TARGET)
	./$(TARGET) --port 8080

test: $(TARGET)
	@echo "Testing HTTP server..."
	@timeout 2 ./$(TARGET) --port 8080 &
	@sleep 1
	@curl -s http://localhost:8080/ || echo "Test failed"
	@pkill -f $(TARGET) || true

.PHONY: all clean run test
