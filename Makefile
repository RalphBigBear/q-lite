CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
LDFLAGS =

TARGET = q-lite
SRCS = src/main.c src/http.c src/ollama.c src/mem-profile.c
OBJS = $(SRCS:.c=.o)

# Detect architecture
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

# ARM64 optimization (Apple Silicon, modern ARM)
ifeq ($(UNAME_S),aarch64)
    CFLAGS += -march=armv8-a -mtune=cortex-a72
    $(info ARM64 detected, enabling ARMv8 optimizations)
    TARGET_ARCH = arm64
endif

# ARM32 optimization (Raspberry Pi)
ifeq ($(UNAME_M),armv7l)
    CFLAGS += -march=armv7-a -mtune=cortex-a53 -mfpu=neon-vfpv4
    $(info ARM32 detected, enabling ARMv7 + NEON optimizations)
    TARGET_ARCH = arm32
endif

# x86_64 (fallback)
ifeq ($(UNAME_M),x86_64)
    CFLAGS += -march=x86-64 -mtune=generic
    $(info x86_64 detected, using generic optimization)
    TARGET_ARCH = x64
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete: $(TARGET) ($(TARGET_ARCH))"
	@echo "Binary size:"
	@ls -lh $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ARM optimized build
arm-opt: CFLAGS += -O3 -march=native
arm-opt: clean $(TARGET)

# RISC-V 64-bit cross-compilation
riscv64: CC = riscv64-unknown-elf-gcc
riscv64: CFLAGS = -Wall -Wextra -O2 -march=rv64imafdc -mabi=lp64d -std=c99
riscv64: $(TARGET)
	@echo "Built for RISC-V 64-bit (RV64IMAFDC)"

# RISC-V 32-bit cross-compilation
riscv32: CC = riscv32-unknown-elf-gcc
riscv32: CFLAGS = -Wall -Wextra -O2 -march=rv32imac -mabi=ilp32 -std=c99
riscv32: $(TARGET)
	@echo "Built for RISC-V 32-bit (RV32IMAC)"

# Test RISC-V binary with QEMU (if installed)
test-riscv64: qemu-riscv64 $(TARGET) --port 8080 || echo "QEMU not installed"
test-riscv32: qemu-riscv32 $(TARGET) --port 8080 || echo "QEMU not installed"

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

.PHONY: all clean run test memory-test arm-opt riscv64 riscv32 test-riscv64 test-riscv32
