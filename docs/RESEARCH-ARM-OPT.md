# Q-Lite ARM Optimization Research

**Date**: 2026-02-11
**Focus**: ARM64/ARM32 性能优化

---

## 🎯 Optimization Targets

### 1. Compiler Optimization Flags

**ARM64 (Apple Silicon, modern ARM)**:
```makefile
CFLAGS_ARM64 = -Wall -Wextra -O3 \
    -march=armv8-a \
    -mtune=cortex-a72 \
    -mcpu=cortex-a72 \
    -std=c99
```

**ARM32 (Raspberry Pi 3/4)**:
```makefile
CFLAGS_ARM32 = -Wall -Wextra -O3 \
    -march=armv7-a \
    -mtune=cortex-a53 \
    -mfpu=neon-vfpv4 \
    -std=c99
```

### 2. NEON SIMD Intrinsics

**向量化字符串操作**:
```c
#include <arm_neon.h>

// NEON 加速的 memcpy (16-byte aligned)
void neon_memcpy(void* dst, const void* src, size_t n) {
    uint8_t* d = (uint8_t*)dst;
    const uint8_t* s = (const uint8_t*)src;

    // 16-byte 块拷贝
    while (n >= 16) {
        uint8x16_t data = vld1q_u8(s);
        vst1q_u8(d, data);
        d += 16;
        s += 16;
        n -= 16;
    }

    // 剩余字节
    while (n--) {
        *d++ = *s++;
    }
}
```

### 3. Cache Alignment

**64-byte cache line alignment**:
```c
typedef struct {
    char data[64];
} __attribute__((aligned(64))) CacheLine;

// HTTP buffer alignment
typedef struct {
    char request[HTTP_MAX_REQUEST] __attribute__((aligned(64)));
    char response[HTTP_MAX_RESPONSE] __attribute__((aligned(64)));
} AlignedBuffers;
```

### 4. Branch Prediction Hint

**Likely/Unlikely macros**:
```c
#define LIKELY(x) __builtin_expect(!!(x), 1)
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

// Usage
if (UNLIKELY(bytes_read <= 0)) {
    // Error path (cold)
}

if (LIKELY(bytes_read > 0)) {
    // Hot path
}
```

### 5. Loop Unrolling

**手动展开循环**:
```c
// Before
for (int i = 0; i < 4; i++) {
    process(data[i]);
}

// After (unrolled)
process(data[0]);
process(data[1]);
process(data[2]);
process(data[3]);
```

---

## 🔧 Makefile Integration

### Platform Detection

```makefile
# Detect architecture
UNAME_S := $(shell uname -s)
UNAME_M := $(shell uname -m)

# ARM64 flags
ifeq ($(UNAME_M),aarch64)
    CFLAGS += -march=armv8-a -mtune=cortex-a72
    TARGET_ARCH = arm64
endif

# ARM32 flags
ifeq ($(UNAME_M),armv7l)
    CFLAGS += -march=armv7-a -mtune=cortex-a53 -mfpu=neon-vfpv4
    TARGET_ARCH = arm32
endif

# x86_64 flags (fallback)
ifeq ($(UNAME_M),x86_64)
    CFLAGS += -march=x86-64 -mtune=generic
    TARGET_ARCH = x64
endif
```

### Optimization Targets

```makefile
# Standard build
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# ARM optimized build
arm-opt: CFLAGS += -O3 -march=native -flto
arm-opt: $(TARGET)

# Profile-guided optimization (PGO)
pgo: CFLAGS += -fprofile-generate
pgo: $(TARGET)
	./$(TARGET) --port 8080 &
	sleep 5
	killall $(TARGET)
	$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS) -fprofile-use"
```

---

## 📊 Benchmark Plan

### Test Scenarios

1. **HTTP Parsing Speed**
   - 1000 requests
   - Measure: requests/second

2. **Memory Bandwidth**
   - memcpy performance
   - buffer copying

3. **Cache Efficiency**
   - L1/L2 cache hits
   - Cache line alignment impact

### Tools

```bash
# perf (Linux)
perf stat ./q-lite --port 8080

# Instruments (macOS)
instruments -t Time\ Profiler ./q-lite --port 8080

# Google Benchmark
# (集成到测试代码)
```

---

## ✅ Implementation Plan

### Step 1: Compiler Flags (立即)
- [ ] 添加 ARM 检测到 Makefile
- [ ] 测试不同优化级别 (-O2 vs -O3)
- [ ] 编译并对比二进制大小

### Step 2: Hot Path Optimization (可选)
- [ ] 识别热点 (profiling)
- [ ] 应用 NEON intrinsics (HTTP parsing)
- [ ] Cache alignment

### Step 3: Benchmark (验证)
- [ ] 基准测试 (优化前)
- [ ] 应用优化
- [ ] 基准测试 (优化后)
- [ ] 对比结果

---

## 🎯 Expected Improvements

- **性能**: 10-20% 提升 (HTTP 处理)
- **二进制**: 可能增加 2-5KB (优化标志)
- **兼容性**: 保持跨平台

---

**Inspired by**: ARM optimization guides
**Next Step**: IMPLEMENT - 添加 ARM 编译标志到 Makefile
