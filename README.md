# Q-Lite

**Ultra-lightweight LLM gateway for edge devices.**

- **<1MB RAM** - Can run on ESP32, Raspberry Pi Zero, or any minimal hardware
- **Pure C** - Zero dependencies, single binary
- **Multi-Platform** - Linux, macOS, Windows, ESP32, STM32, RP2040
- **Ollama Compatible** - Works with local Ollama server out of the box

---

## 🎯 What is Q-Lite?

Q-Lite is a minimal LLM gateway designed for edge devices with limited resources. It provides HTTP API endpoints for interacting with local LLM servers (Ollama, vLLM, LM Studio) with minimal overhead.

### Key Features

- ✅ Ultra-lightweight (<1MB RAM)
- ✅ Pure C implementation
- ✅ HTTP server with REST API
- ✅ Ollama backend integration
- ✅ Edge device friendly (ARM, RISC-V, x86)
- ✅ Multi-backend support (Ollama, vLLM, LM Studio)
- ✅ HTTP Chunked Streaming (real-time responses)
- ✅ Request Queue (concurrent limiting)

---

## 📊 Platform Support

### Desktop/Server (Linux, macOS, Windows)

| Platform | Status | Binary Size | RAM Usage |
|----------|--------|-------------|-----------|
| Linux/x86_64 | ✅ Full | 53KB | <500KB |
| macOS/x86_64 | ✅ Full | 60KB | <600KB |
| Windows/x86_64 | ✅ Full | 58KB | <700KB |

### Edge Devices

| Platform | Status | Flash | RAM | Binary Size |
|----------|--------|-------|-----|-------------|
| ESP32-S3 | ✅ Port | 4MB | 520KB | ~100KB |
| ESP32 | ✅ Port | 4MB | 520KB | ~100KB |
| STM32F4/F7 | ✅ Port | 512KB | 128KB | ~80KB |
| RP2040 | ✅ Port | 2MB | 264KB | ~60KB |

---

## 🚀 Quick Start

### Option 1: Download Release (Recommended)

```bash
# Download latest release from GitHub
wget https://github.com/RalphBigBear/q-lite/releases/download/v0.3.0/q-lite
chmod +x q-lite

# Run with auto-detect backend
./q-lite --port 8080
```

### Option 2: Build from Source (Linux/macOS)

```bash
# Clone
git clone https://github.com/RalphBigBear/q-lite.git
cd q-lite

# Build
make

# Run with auto-detect (detects Ollama, vLLM, LM Studio)
./q-lite --port 8080

# Or specify backend explicitly
./q-lite --backend ollama --port 8080
./q-lite --backend openai --backend-port 8000 --port 8080
```

### Option 3: ESP32 Microcontroller

```bash
cd platforms/esp32
idf.py set-target esp32s3
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

**See [platforms/esp32/README.md](platforms/esp32/README.md) for detailed ESP32 instructions.**

### Option 4: STM32 Microcontroller

```bash
cd platforms/stm32
# Open in STM32CubeIDE
# Build & Flash
```

**See [platforms/stm32/README.md](platforms/stm32/README.md) for detailed STM32 instructions.**

### Option 5: Raspberry Pi Pico

```bash
cd platforms/pico
mkdir build && cd build
cmake ..
make
# Copy q-lite-pico.uf2 to Pico (hold BOOTSEL)
```

**See [platforms/pico/README.md](platforms/pico/README.md) for detailed Pico instructions.**

---

## 📖 Usage

### Start Server

```bash
# Auto-detect backend (recommended)
./q-lite --port 8080

# Specify backend
./q-lite --backend ollama --port 8080

# Specify host and port
./q-lite --backend openai --backend-host 192.168.1.100 --backend-port 8000 --port 8080
```

### Test Endpoints

```bash
# Health check
curl http://localhost:8080/
# {"status":"ok","message":"Q-Lite HTTP Server"}

# List models
curl http://localhost:8080/models
# {"models":["llama2-7b","mistral-7b","gemma-7b"]}

# Generate text (streaming)
curl -X POST http://localhost:8080/api/generate \
  -H "Content-Type: application/json" \
  -d '{"model":"llama2-7b","prompt":"Hello, world!","max_tokens":50,"stream":true}'

# Generate text (non-streaming)
curl -X POST http://localhost:8080/api/generate \
  -H "Content-Type: application/json" \
  -d '{"model":"llama2-7b","prompt":"Hello, world!","max_tokens":50}'
```

---

## 🏗️ Architecture

```
┌─────────────────────────────────────────┐
│ Edge Device (Q-Lite, <1MB RAM)          │
│                                         │
│  HTTP Server (REST API)                 │
│  ↓                                      │
│  Backend Client (Ollama, vLLM, LM Studio)│
│  ↓                                      │
│  Local LLM Server (Ollama, 128GB RAM)   │
│                                         │
└─────────────────────────────────────────┘
```

### Components

- **HTTP Server** - Lightweight HTTP server with REST API
- **Backend Client** - Multi-backend support (Ollama, vLLM, LM Studio, OpenAI-compatible)
- **Request Queue** - Concurrent limiting (max 10 requests)
- **Platform Abstraction** - Unified interface for different platforms

---

## 🎓 Inspired By

This project stands on the shoulders of giants, drawing inspiration from:

### 🦀 nanochat - Andrej Karpathy
[github.com/karpathy/nanochat](https://github.com/karpathy/nanochat)

**Single-dial philosophy**: Inspired by nanochat's `--depth` parameter, Q-Lite adopts a simplified `--target` preset system (e.g., `--target esp32`, `--target desktop`). One parameter auto-configures all platform-specific settings.

**End-to-end mindset**: Like nanochat's `speedrun.sh`, Q-Lite provides `quickstart.sh` for instant demos.

### 🦌 llama2.c - Andrej Karpathy
[github.com/karpathy/llama2.c](https://github.com/karpathy/llama2.c)

**Single-file architecture**: Q-Lite applies the same philosophy to LLM gateways - minimal code, maximum clarity.

### 📡 ESP32-LLM - Davebben
[github.com/DaveBben/esp32-llm](https://github.com/DaveBben/esp32-llm)

**Edge AI breakthrough**: Proved that LLMs can run on 1MB RAM. Q-Lite builds on this for gateway deployment.

---

## 📚 Documentation

- **[BUILD.md](docs/BUILD.md)** - Build instructions & troubleshooting
- **[API.md](docs/API.md)** - REST API documentation & examples
- **[platforms/esp32/README.md](platforms/esp32/README.md)** - ESP32 build guide
- **[platforms/stm32/README.md](platforms/stm32/README.md)** - STM32 build guide
- **[platforms/pico/README.md](platforms/pico/README.md)** - Raspberry Pi Pico build guide

---

## 🔧 Technology Stack

- **Language**: C99 (pure C, no C++ dependencies)
- **HTTP**: Custom HTTP server
- **Backend**: Ollama API compatible
- **Platforms**: Linux, macOS, Windows, ESP32, STM32, RP2040

---

## 📈 Benchmarks

Coming soon. Target metrics:

- **RAM**: <1MB (base), <500KB (minimal)
- **Binary**: <100KB (stripped)
- **Latency**: <10ms overhead
- **Throughput**: >100 req/s (single core)

---

## 🛣️ Roadmap

### Phase 1: Core MVP ✅ Complete
- [x] Project structure
- [x] Basic HTTP server
- [x] Ollama API integration
- [x] Single binary deployment

### Phase 2: Edge Optimization ✅ Complete
- [x] Memory profiling (<1MB target)
- [x] ARM optimization
- [x] RISC-V support

### Phase 3: Advanced Features ✅ Complete
- [x] Multi-backend support (Ollama, vLLM, LM Studio)
- [x] HTTP Chunked Streaming (real-time responses)
- [x] Request Queue (concurrent limiting)

### Phase 4: Microcontroller Ports ✅ Complete
- [x] ESP32 port (WiFi, ESP-IDF)
- [x] STM32 port (Ethernet, STM32CubeIDE)
- [x] Raspberry Pi Pico port (WiFi via ESP8266)

### Phase 5: Future Enhancements
- [ ] TLS/SSL support
- [ ] WebSocket support
- [ ] Authentication
- [ ] Metrics & monitoring

---

## 🤝 Contributing

Contributions welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

Focus areas:
- Memory optimization
- New platform ports
- Bug fixes
- Documentation improvements

---

## ❓ FAQ

**Q: What's the minimum hardware?**
A: Target: ESP32-S3 (520KB RAM). Recommended: Raspberry Pi Zero (512MB RAM).

**Q: Can I use this with OpenAI API?**
A: Yes, Q-Lite supports OpenAI-compatible APIs (vLLM, LM Studio, etc.).

**Q: Is this production-ready?**
A: Phase 4 MVP. Use with caution.

**Q: How do I configure WiFi on ESP32?**
A: See [platforms/esp32/README.md](platforms/esp32/README.md) for detailed instructions.

---

## 📄 License

MIT License - See [LICENSE](LICENSE) for details.

---

## 🔗 Links

- **GitHub**: [https://github.com/RalphBigBear/q-lite](https://github.com/RalphBigBear/q-lite)
- **Issues**: [https://github.com/RalphBigBear/q-lite/issues](https://github.com/RalphBigBear/q-lite/issues)
- **Discussions**: [https://github.com/RalphBigBear/q-lite/discussions](https://github.com/RalphBigBear/q-lite/discussions)

---

**Made with inspiration from Karpathy, Davebben, and Sipeed.**

*"Simplicity is the ultimate sophistication."* - Leonardo da Vinci
