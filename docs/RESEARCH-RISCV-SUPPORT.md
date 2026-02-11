# Q-Lite RISC-V Support Research

**Date**: 2026-02-11
**Focus**: RISC-V 32/64-bit 架构移植

---

## 🎯 RISC-V Overview

### RISC-V 扩展

**Base ISA**:
- **RV32I**: 32-bit integer
- **RV64I**: 64-bit integer

**Standard Extensions**:
- **M**: Integer multiply/divide
- **A**: Atomic operations
- **F**: Single-precision floating-point
- **D**: Double-precision floating-point
- **C**: Compressed instructions (16-bit)

**Common Combinations**:
- `RV32IMAC` - 32-bit with M+A+C
- `RV64IMAFDC` - 64-bit full featured

---

## 🔧 Toolchain Setup

### Installation

**macOS**:
```bash
# Use Homebrew
brew install riscv-gnu-toolchain

# Or download prebuilt
wget https://github.com/riscv/riscv-gnu-toolchain/releases
```

**Linux**:
```bash
# Ubuntu/Debian
sudo apt-get install gcc-riscv64-unknown-elf

# Fedora
sudo dnf install riscv64-linux-gnu-gcc
```

### Cross-Compiler

**RISC-V 64-bit**:
```bash
riscv64-unknown-elf-gcc --version
```

**RISC-V 32-bit**:
```bash
riscv32-unknown-elf-gcc --version
```

---

## 📝 Makefile Integration

### New Targets

```makefile
# RISC-V 64-bit
riscv64: CC = riscv64-unknown-elf-gcc
riscv64: CFLAGS = -Wall -Wextra -O2 \
    -march=rv64imafdc \
    -mabi=lp64d \
    -std=c99
riscv64: $(TARGET)
	@echo "Built for RISC-V 64-bit"

# RISC-V 32-bit
riscv32: CC = riscv32-unknown-elf-gcc
riscv32: CFLAGS = -Wall -Wextra -O2 \
    -march=rv32imac \
    -mabi=ilp32 \
    -std=c99
riscv32: $(TARGET)
	@echo "Built for RISC-V 32-bit"
```

---

## 🧪 Testing with QEMU

### QEMU RISC-V

**Install QEMU**:
```bash
# macOS
brew install qemu

# Linux
sudo apt-get install qemu-system-riscv64
```

**Run RISC-V Binary**:
```bash
# 64-bit
qemu-riscv64 ./q-lite

# 32-bit
qemu-riscv32 ./q-lite
```

---

## 🔧 RISC-V Specific Optimizations

### 1. Compressed Instructions (-C extension)

**Benefit**:
- 10-30% smaller code size
- Better cache utilization

**Compiler Flag**:
```makefile
CFLAGS += -march=rv64imafdc  # 'c' enables compressed
```

### 2. Branch Alignment

**RISC-V branch prediction**:
```c
// Align frequently executed branches
if (LIKELY(condition)) {
    // Hot path
}
```

### 3. Register Usage

**RISC-V has 32 registers**:
- More registers than x86
- Better for register-intensive code

**Optimization**:
- Reduce stack usage
- Increase register allocation

---

## 📊 RISC-V vs ARM vs x86

| Feature | RISC-V | ARM64 | x86_64 |
|---------|--------|-------|--------|
| Registers | 32 | 31 | 16 |
| ISA Type | RISC | RISC | CISC |
| Code Density | Medium | High | High |
| Binary Size | +5-10% | Baseline | +10-15% |

---

## 🎯 Implementation Plan

### Step 1: Toolchain Verification
- [ ] 检查 RISC-V 工具链是否安装
- [ ] 测试编译简单程序
- [ ] 验证 QEMU 可用

### Step 2: Makefile Update
- [ ] 添加 `riscv64`, `riscv32` targets
- [ ] 设置正确的 `-march` 和 `-mabi`
- [ ] 测试跨编译

### Step 3: QEMU Testing
- [ ] 编译 RISC-V 二进制
- [ ] 用 QEMU 运行
- [ ] 验证功能

### Step 4: CI/CD (可选)
- [ ] GitHub Actions 跨编译
- [ ] 自动化测试

---

## ⚠️ Known Issues

### 1. Toolchain Availability
- macOS: 需要手动安装
- Linux: 包管理器可用

### 2. QEMU Performance
- 比原生慢 10-100x
- 仅用于功能测试，非性能测试

### 3. Binary Size
- RISC-V 可能比 ARM 大 5-10%
- 压缩指令可缓解

---

## ✅ Expected Outcome

**Binary Size**:
- RISC-V 64-bit: ~55KB (+3KB from ARM)
- RISC-V 32-bit: ~53KB (+1KB)

**Performance**:
- QEMU 模拟慢（仅测试）
- 真实硬件待测（SiFive, etc.）

**Compatibility**:
- 功能等同于 ARM/x86
- API 兼容

---

## 📚 References

1. **RISC-V Specifications**:
   - riscv.org/specifications

2. **QEMU Documentation**:
   - qemu.org/docs

3. **GCC RISC-V Options**:
   - gcc.gnu.org/onlinedocs/gcc/RISC-V-Options.html

---

**Next Step**: IMPLEMENT - 添加 RISC-V targets 到 Makefile
