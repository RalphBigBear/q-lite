# Q-Lite Phase 2: Edge Optimization Research

**Date**: 2026-02-11
**Focus**: Memory profiling, ARM optimization, RISC-V support

---

## 🎯 Goals

1. **Memory Profiling** - 测量并减少 RAM 使用
2. **ARM Optimization** - ARM64/ARM32 性能优化
3. **RISC-V Support** - RISC-V 架构移植

---

## 📊 Memory Profiling Techniques

### 1. Static Analysis (编译时)

**工具**: `size` 命令
```bash
$ size q-lite
text    data     bss     dec     hex filename
14321    1024    512    15857   3df1 q-lite
```

**含义**:
- `text`: 代码段
- `data`: 初始化数据
- `bss`: 未初始化数据
- `dec`: 总计

### 2. Dynamic Analysis (运行时)

**工具**: Valgrind Massif
```bash
valgrind --tool=massif ./q-lite --port 8080

# 分析结果
ms_print massif.out.xxxx
```

**输出示例**:
```
KB
20.8^                                                                     #
    |                                                                  #
    |                                                                  @#:|
    |                                                                @@:::@
    |                                      @@#@@@@@@@@@            :::@  :.
    0 +----------------------------------------------------------------------->Ki
    0                                                                   120.0
```

### 3. Heap Tracking (自定义)

**在代码中添加**:
```c
#include <malloc.h>

void print_memory_usage() {
    struct mallinfo mi = mallinfo();
    printf("Heap: %d KB\n", mi.uordblks / 1024);
}
```

---

## 🔧 ARM Optimization Techniques

### 1. Compiler Flags

**ARM64 (Apple Silicon)**:
```makefile
CFLAGS = -Wall -Wextra -O3 -march=armv8-a -mtune=cortex-a72 -std=c99
```

**ARM32 (Raspberry Pi)**:
```makefile
CFLAGS = -Wall -Wextra -O3 -march=armv7-a -mtune=cortex-a53 -std=c99
```

### 2. Inline Assembly (关键路径)

**字符串操作优化**:
```c
// ARM64 optimized memcpy (example)
static inline void* fast_memcpy(void* dest, const void* src, size_t n) {
    __asm__ volatile(
        "memcpy %0, %1, %2"
        : "=r"(dest)
        : "r"(src), "r"(n)
        : "memory"
    );
    return dest;
}
```

### 3. Cache Alignment

**数据结构对齐**:
```c
typedef struct {
    char data[64] __attribute__((aligned(64)));  // Cache line alignment
} CacheLine;
```

---

## 🚀 RISC-V Support

### 1. RISC-V Toolchain

**安装**:
```bash
# macOS
brew install riscv-isac-llvm

# Linux
sudo apt-get install gcc-riscv64-unknown-elf
```

### 2. Cross-Compilation

**Makefile**:
```makefile
# RISC-V 64-bit
riscv64: CC = riscv64-unknown-elf-gcc
riscv64: CFLAGS = -Wall -Wextra -O2 -march=rv64imafdc -mabi=lp64d -std=c99
riscv64: $(TARGET)

# RISC-V 32-bit
riscv32: CC = riscv32-unknown-elf-gcc
riscv32: CFLAGS = -Wall -Wextra -O2 -march=rv32imac -mabi=ilp32 -std=c99
riscv32: $(TARGET)
```

### 3. RISC-V Specific Optimizations

**使用 RISC-V 扩展**:
- `M`: Integer multiplication/division
- `A`: Atomic instructions
- `F`: Single-precision floating-point
- `D`: Double-precision floating-point
- `C`: Compressed instructions

---

## 📈 Memory Optimization Strategies

### 1. Reduce Stack Usage

**Before** (大栈占用):
```c
void process_request() {
    char buffer[8192];  // 8KB on stack!
    // ...
}
```

**After** (小栈占用):
```c
void process_request() {
    static char buffer[8192];  // In BSS, not stack
    // ...
}
```

### 2. Reuse Buffers

**全局缓冲池**:
```c
#define BUFFER_POOL_SIZE 4
static struct {
    char data[4096];
    bool in_use;
} buffer_pool[BUFFER_POOL_SIZE];
```

### 3. String Interning

**避免重复字符串**:
```c
// Before: 每次都分配
char *msg = strdup("OK");

// After: 使用静态指针
static const char *MSG_OK = "OK";
```

---

## 🎯 Phase 2 Implementation Plan

### Task 1: Memory Profiling (1-2 hours)
- [ ] 添加 `--memory-stats` 命令行选项
- [ ] 实现 `print_memory_usage()` 函数
- [ ] 使用 Valgrind Massif 分析
- [ ] 目标: 验证 <1MB RAM

### Task 2: ARM Optimization (2-3 hours)
- [ ] 添加 ARM 特定编译标志
- [ ] 优化关键路径 (HTTP parsing)
- [ ] Benchmark 对比 (优化前 vs 后)
- [ ] 目标: 10-20% 性能提升

### Task 3: RISC-V Support (3-4 hours)
- [ ] 添加 RISC-V 编译目标
- [ ] 测试 QEMU RISC-V 模拟器
- [ ] 目标: 成功编译并运行

---

## 📚 References

1. **ARM Optimization**:
   - "ARM Assembly Language" - William Hohl
   - "Embedded Real-Time Systems" - CPEN 432

2. **Memory Profiling**:
   - Valgrind Massif Documentation
   - "Memory as a Programming Concept" - Franta/Kalay

3. **RISC-V**:
   - "The RISC-V Reader" - Patterson & Waterman
   - RISC-V International Specifications

---

## ✅ Research Summary

**Memory Profiling**:
- Valgrind Massif (运行时)
- `size` 命令 (静态)
- 自定义 `mallinfo()` 跟踪

**ARM Optimization**:
- `-march=` + `-mtune=` flags
- Cache line alignment
- 内联汇编 (可选)

**RISC-V Support**:
- riscv64-unknown-elf-gcc
- `-march=rv64imafdc`
- QEMU 测试

**Next Step**: DESIGN - 详细设计 memory profiling 模块

---

**Inspired by**: Embedded systems optimization literature
**Date**: 2026-02-11 16:25 GMT+8
