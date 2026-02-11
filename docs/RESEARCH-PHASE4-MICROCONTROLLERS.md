# Q-Lite Phase 4: Microcontroller Ports

**Date**: 2026-02-11
**Focus**: ESP32, Raspberry Pi Pico, STM32

---

## 🎯 目标

将 Q-Lite HTTP 网关移植到微控制器平台，实现**边缘 LLM 网关**。

**关键区分**：
- Q-Lite 是**网关**，不是 LLM 推理引擎
- 连接到后端 LLM (Ollama, vLLM)
- 处理 HTTP 请求，转发到后端

---

## 📊 参考项目分析

### 1. ESP32-LLM (DaveBben)

**项目**: https://github.com/DaveBben/esp32-llm

**特点**:
- 基于 llama2.c (Karpathy)
- ESP32-S3 (8MB PSRAM, 520KB SRAM)
- 260K 参数 TinyLLaMA
- **已验证可行性** ✅

**技术栈**:
- ESP-IDF (官方 SDK)
- FreeRTOS
- LWIP (轻量级 TCP/IP)

**熵值**: 低 (ESP-IDF 成熟)

### 2. Raspberry Pi Pico (RP2040)

**硬件**:
- RP2040: Dual-core ARM Cortex-M0+ @ 133MHz
- 264KB SRAM
- 2MB Flash (通常)
- 无内置 WiFi (需要外接模块)

**技术栈**:
- Pico C/C++ SDK
- FreeRTOS (可选)
- LWIP (通过 WiFi 模块)

**熵值**: 中等 (需要外接 WiFi)

**参考**: picoLLM (商业库)

### 3. STM32 (ARM Cortex-M)

**硬件**:
- STM32F4/F7/H7 系列
- 512KB - 2MB SRAM
- 以太网 MAC (内置)
- 外接 PHY 芯片

**技术栈**:
- STM32CubeIDE
- STM32Cube.AI (代码生成)
- LwIP or NetXDuo
- FreeRTOS

**熵值**: 低 (STM32 生态成熟)

**参考**: STM32Cube.AI, TensorFlow Lite Micro

---

## 🔧 移植策略

### 架构设计

```
┌─────────────────────────────────────┐
│   Microcontroller (Q-Lite Gateway)  │
│   - HTTP Server (Q-Lite)            │
│   - WiFi/Ethernet (LwIP)            │
│   - JSON Parsing (minimal)           │
└─────────────────────────────────────┘
              ↓ Network
┌─────────────────────────────────────┐
│   Backend LLM Server (Ollama)       │
│   - Running on powerful machine     │
│   - vLLM, Ollama                    │
└─────────────────────────────────────┘
```

### 核心挑战

**1. 内存限制**
- Q-Lite 核心代码: ~50KB Flash
- 运行时内存: ~20KB RAM
- 缓冲区: HTTP 请求/响应

**2. 网络栈**
- ESP32: 内置 WiFi + LwIP ✅
- Pico: 外接 WiFi (ESP8266/ESP32) + LwIP
- STM32: 以太网 PHY + LwIP

**3. 平台差异**
- UART 调试输出
- 定时器/时钟
- 多线程 (FreeRTOS)

---

## 📝 实现计划

### Task 1: ESP32 Port (优先)

**原因**: 内置 WiFi，成熟生态

**步骤**:
1. 创建 ESP-IDF 项目
2. 移植 Q-Lite HTTP 服务器
3. 集成 LwIP TCP/IP
4. 测试 Ollama 连接

**预期**:
- Flash: ~100KB (Q-Lite + ESP-IDF 组件)
- RAM: ~50KB (运行时)
- WiFi: Station 模式

### Task 2: STM32 Port

**原因**: 以太网稳定，工业级

**步骤**:
1. 创建 STM32CubeIDE 项目
2. 配置以太网 PHY (LAN8720)
3. 移植 Q-Lite + LwIP
4. FreeRTOS 集成

**预期**:
- Flash: ~80KB
- RAM: ~40KB
- Ethernet: 100Mbps

### Task 3: Raspberry Pi Pico Port

**原因**: 低成本 ($4)

**步骤**:
1. 创建 Pico C SDK 项目
2. 外接 WiFi 模块 (ESP8266 AT 命令)
3. 移植 Q-Lite
4. UART WiFi 通信

**预期**:
- Flash: ~60KB
- RAM: ~30KB
- WiFi: UART → ESP8266

---

## 🎨 代码结构

### 平台抽象层

```c
// platform.h
typedef struct {
    void (*init)(void);
    int (*network_start)(const char *ssid, const char *pass);
    void (*debug_print)(const char *msg);
    unsigned long (*get_time_ms)(void);
} PlatformOps;

// ESP32 实现
PlatformOps* platform_esp32(void);

// STM32 实现
PlatformOps* platform_stm32(void);

// Pico 实现
PlatformOps* platform_pico(void);
```

### 条件编译

```c
// config.h
#if defined(ESP32_PLATFORM)
    #include "esp32_platform.h"
#elif defined(STM32_PLATFORM)
    #include "stm32_platform.h"
#elif defined(PICO_PLATFORM)
    #include "pico_platform.h"
#endif
```

---

## 📊 熵值对比

| 平台 | Flash | RAM | 网络 | 复杂度 | 熵值 |
|------|-------|-----|------|--------|------|
| ESP32 | 100KB | 50KB | 内置WiFi | 低 | **低** ✅ |
| STM32 | 80KB | 40KB | 以太网 | 中 | **低** ✅ |
| Pico | 60KB | 30KB | 外接WiFi | 中高 | 中 |

---

## ✅ Expected Outcome

**Task 1 - ESP32**:
- ✅ ESP-IDF 组件
- ✅ WiFi 连接 + DHCP
- ✅ HTTP 服务器运行
- ✅ 连接到 Ollama 后端

**Task 2 - STM32**:
- ✅ STM32CubeIDE 项目
- ✅ 以太网初始化
- ✅ LwIP 集成
- ✅ FreeRTOS 任务

**Task 3 - Pico**:
- ✅ Pico C SDK 项目
- ✅ UART WiFi (ESP8266)
- ✅ AT 命令解析
- ✅ HTTP 服务器

**总计**:
- 3 个平台移植
- 平台抽象层
- 完整文档
- 预计代码: +500 LOC (平台相关)

---

## 🚧 Limitations

1. **无 LLM 推理**: Q-Lite 只做网关，不做推理
2. **网络依赖**: 需要后端 LLM 服务器
3. **内存限制**: 缓冲区大小受限
4. **并发限制**: 最多 1-2 个并发连接

---

**Next Step**: IMPLEMENT - 开始 ESP32 移植
