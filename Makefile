# Q-Lite Makefile - Platform Abstraction

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99 -Isrc
LDFLAGS =

TARGET = q-lite
SRCS = src/main.c src/http.c src/ollama.c src/mem-profile.c src/backend.c src/platform_init.c
OBJS = $(SRCS:.c=.o)

# Platform targets
PLATFORM_TARGETS = esp32 stm32 pico

.PHONY: all clean $(PLATFORM_TARGETS)

# Default target (desktop)
all: $(TARGET)

# Desktop target
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET)"
	@echo "Binary size:"
	@ls -lh $(TARGET)

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ESP32 platform
esp32:
	@echo "Building for ESP32..."
	@echo "Use: cd platforms/esp32 && idf.py build flash"

# STM32 platform
stm32:
	@echo "Building for STM32..."
	@echo "Use STM32CubeIDE to build"

# Pico platform
pico:
	@echo "Building for Raspberry Pi Pico..."
	@echo "Use: cd platforms/pico && mkdir -p build && cd build && cmake .. && make"

# Clean
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Cleaned build artifacts"
