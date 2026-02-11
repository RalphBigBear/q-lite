CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LDFLAGS =

TARGET = q-lite
SRCS = src/main.c src/http.c src/ollama.c src/mem-profile.c
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
	@./$(TARGET) --port 8080 &
	@sleep 1
	@curl -s http://localhost:8080/
	@echo ""
	@pkill -f $(TARGET) || true

memory-test: $(TARGET)
	@echo "Testing with memory profiling..."
	@./$(TARGET) --port 8080 --memory-stats &
	@sleep 2
	@curl -s http://localhost:8080/
	@echo ""
	@sleep 1
	@pkill -f $(TARGET) || true

.PHONY: all clean run test memory-test
