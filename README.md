# Q-Lite

**Ultra-lightweight LLM gateway with dual-version strategy.**

- **<1MB RAM (C version)** - Can run on ESP32, Raspberry Pi Zero, or any minimal hardware
- **Dual-Version Strategy** - Single-board C (Edge) + Node.js (Desktop)
- **Pure C** - Zero dependencies, single binary
- **Full-Featured Gateway** - Node.js version with routing, auth, proxy, and logging
- **Ollama Compatible** - Works with local Ollama server out of the box

---

## 🎯 Dual-Version Strategy

### Version 1: q-lite (C Language) - For Edge Devices

**Target**: Edge devices with limited resources (<1MB RAM)

**Features**:
- ✅ Ultra-lightweight (<1MB RAM)
- ✅ Pure C implementation
- ✅ HTTP server with REST API
- ✅ Ollama backend integration
- ✅ Edge device friendly (ARM, RISC-V, x86)

**Platforms**:
- Linux/x86: ✅ Full
- ESP32-S3: ✅ Port
- STM32F4/F7: ✅ Port
- RP2040: ✅ Port

**Binary Size**:
- Linux/x86: 53KB
- ESP32: ~100KB (estimated)
- STM32: ~80KB (estimated)
- Pico: ~60KB (estimated)

### Version 2: q-lite-gateway (Node.js) - For Desktop/Server

**Target**: Desktop/Server with sufficient resources

**Features**:
- ✅ Routing system (provider/model → endpoint)
- ✅ Authentication management (API Keys via environment variables)
- ✅ Proxy configuration (HTTP/SOCKS5)
- ✅ Logging system (JSONL format + log rotation)
- ✅ Health check (/health)
- ✅ Model listing (/models)

**Supported Providers**:
- OpenClaw (antigravity/gemini-3-pro-high)
- OpenAI (gpt-4-turbo, gpt-3.5-turbo)
- Anthropic (claude-3-opus, claude-3-5-sonnet)
- Zai (glm-4, glm-4.7)
- Local (llama3-70b, qwen-72b)

**Repository**: https://github.com/RalphBigBear/q-lite-gateway

---

## Inspired By

This project stands on the shoulders of giants, drawing inspiration from:

### 🦀 nanochat - Andrej Karpathy
[github.com/karpathy/nanochat](https://github.com/karpathy/nanochat)

**Single-dial philosophy**: Inspired by nanochat's `--depth` parameter, Q-Lite adopts a simplified `--target` preset system (e.g., `--target esp32`, `--target desktop`). One parameter auto-configures all platform-specific settings.

**End-to-end mindset**: Like nanochat's `speedrun.sh`, Q-Lite provides `quickstart.sh` for instant demos.

**Thank you @karpathy for proving that minimalism and accessibility beat feature bloat.**

### 🦌 llama2.c - Andrej Karpathy
[github.com/karpathy/llama2.c](https://github.com/karpathy/llama2.c)

**Single-file architecture**: Q-Lite applies the same philosophy to LLM gateways - minimal code, maximum clarity.

### 📡 ESP32-LLM - Davebben
[github.com/DaveBben/esp32-llm](https://github.com/DaveBben/esp32-llm)

**Edge AI breakthrough**: Proved that LLMs can run on 1MB RAM. Q-Lite builds on this for gateway deployment.

### 🌸 PicoClaw - Sipeed
[github.com/sipeed/picoclaw](https://github.com/sipeed/picoclaw)

**Lightweight gateway design**: Showed that gateway code can be <1000 lines while remaining functional.

---

## Features

- ✅ **Dual-Version Strategy**: C (edge) + Node.js (desktop)
- ✅ Minimal memory footprint (<1MB RAM for C version)
- ✅ Single C file implementation (optional)
- ✅ Full-featured Gateway (Node.js version)
- ✅ HTTP server with REST API
- ✅ Ollama backend integration
- ✅ Edge device friendly (ARM, RISC-V, x86)

---

## Quick Start

### Option 1: Download Release (C Version - Recommended for Edge)

```bash
# Download latest release from GitHub
wget https://github.com/RalphBigBear/q-lite/releases/download/v0.3.0/q-lite
chmod +x q-lite

# Run with auto-detect backend
./q-lite --port 8080
```

### Option 2: Build from Source (C Version - Desktop/Linux)

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

### Option 3: Use Node.js Gateway (Desktop/Server)

```bash
# Clone q-lite-gateway
git clone https://github.com/RalphBigBear/q-lite-gateway.git
cd q-lite-gateway

# Install dependencies
npm install

# Configure environment variables
export ANTIGRAVITY_API_KEY=sk-xxx
export OPENAI_API_KEY=sk-yyy
export ANTHROPIC_API_KEY=sk-ant-zzz

# Start gateway
npm start
```

### Option 4: Microcontroller Port (C Version)

**ESP32**:
```bash
cd platforms/esp32
idf.py build
idf.py flash
```

**STM32**:
```bash
cd platforms/stm32
# Open in STM32CubeIDE
# Build & Flash
```

**Raspberry Pi Pico**:
```bash
cd platforms/pico
mkdir build && cd build
cmake ..
make
# Copy q-lite-pico.uf2 to Pico (hold BOOTSEL)
```

### Test

```bash
# C version
curl http://localhost:8080/
# Output: {"status":"ok","message":"Q-Lite HTTP Server"}

# Node.js gateway
curl http://localhost:7333/health
# Output: {"status":"ok","timestamp":"..."}
```

---

## Architecture

```
┌─────────────────────────────────────────┐
│ Edge Device (Q-Lite, <1MB RAM)      │
│         ↓ HTTP                         │
│ Local LLM Server (Ollama, 128GB RAM)   │
│         ↓                               │
│ Response ←                              │
└─────────────────────────────────────────┘

┌─────────────────────────────────────────┐
│ Desktop/Server (Q-Lite Gateway)       │
│         ↓ HTTP                         │
│   Gateway (routing, auth, proxy)       │
│         ↓                               │
│   Model Provider (OpenAI, Anthropic)    │
│         ↓                               │
│ Response ←                              │
└─────────────────────────────────────────┘
```

---

## Project Goals

Q-Lite aims to solve a simple problem: **running LLM gateways with a dual-version strategy.**

### Edge Version (C)
- **Minimal footprint** - Target <1MB RAM
- **Zero dependencies** - Pure C, no external libraries
- **Simple deployment** - Single binary, run anywhere
- **Edge-first** - Designed for constrained devices

### Desktop Version (Node.js)
- **Full-featured** - Routing, auth, proxy, logging
- **Easy to use** - Configuration files, environment variables
- **Production-ready** - JSONL logging, log rotation
- **Multi-provider** - OpenClaw, OpenAI, Anthropic, Zai, Local

Inspired by the philosophy that *"everything should be made as simple as possible, but not simpler."*

---

## Technology Stack

### Edge Version (C)
- **Language**: C99 (pure C, no C++ dependencies)
- **HTTP**: Custom server
- **Backend**: Ollama API compatible

### Desktop Version (Node.js)
- **Language**: JavaScript (Node.js)
- **Framework**: Express.js
- **Proxy**: https-proxy-agent, socks-proxy-agent

---

## Roadmap

### Phase 1: Core MVP (C Version) ✅ Complete
- [x] Project structure
- [x] Basic HTTP server
- [x] Ollama API integration
- [x] Single binary deployment

### Phase 2: Edge Optimization (C Version) ✅ Complete
- [x] Memory profiling (<1MB target)
- [x] ARM optimization
- [x] RISC-V support

### Phase 3: Advanced Features (C Version) ✅ Complete
- [x] Multi-backend support (Ollama, vLLM, LM Studio)
- [x] HTTP Chunked Streaming (real-time responses)
- [x] Request Queue (concurrent limiting)

### Phase 4: Microcontroller Ports (C Version) ✅ Complete
- [x] ESP32 port (WiFi, ESP-IDF)
- [x] STM32 port (Ethernet, STM32CubeIDE)
- [x] Raspberry Pi Pico port (WiFi via ESP8266)

### Phase 5: Dual-Version Strategy ✅ Complete (v0.3.0)
- [x] q-lite-gateway (Node.js version)
- [x] Routing system
- [x] Authentication management
- [x] Proxy configuration
- [x] Logging system (JSONL)

### Phase 6: Integration (Planned)
- [ ] Unified configuration
- [ ] Cross-platform testing
- [ ] Documentation unification
- [ ] Release management automation

---

## Platform Support Matrix

### Edge Version (C)

| Platform | Status | Flash | RAM | Network | Binary Size |
|----------|--------|-------|-----|----------|-------------|
| Linux/x86 | ✅ Full | N/A | N/A | Ethernet/WiFi | 53KB |
| ESP32-S3 | ✅ Port | 4MB | 520KB | WiFi (built-in) | ~100KB |
| STM32F4/F7 | ✅ Port | 512KB | 128KB | Ethernet | ~80KB |
| RP2040 | ✅ Port | 2MB | 264KB | WiFi (ESP8266) | ~60KB |

### Desktop Version (Node.js)

| Platform | Status | Node.js |
|----------|--------|---------|
| Linux/x64 | ✅ Full | >=18.0.0 |
| macOS/x64 | ✅ Full | >=18.0.0 |
| Windows/x64 | ✅ Full | >=18.0.0 |

---

## Benchmarks

Coming soon. Target metrics:

### Edge Version (C)
- **RAM**: <1MB (base), <500KB (minimal)
- **Binary**: <100KB (stripped)
- **Latency**: <10ms overhead
- **Throughput**: >100 req/s (single core)

### Desktop Version (Node.js)
- **Memory**: <100MB (typical)
- **Latency**: <100ms (including proxy)
- **Throughput**: >50 req/s (single core)
- **Logging**: JSONL format, daily rotation

---

## Contributing

Contributions welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

Focus areas:
- Memory optimization
- New platform ports
- Bug fixes
- Documentation improvements
- Gateway features (Node.js version)

---

## Documentation

- **[BUILD.md](docs/BUILD.md)** - Build instructions & troubleshooting
- **[API.md](docs/API.md)** - REST API documentation & examples
- **[GATEWAY-GUIDE.md](docs/GATEWAY-GUIDE.md)** - Node.js gateway guide (q-lite-gateway)

---

## FAQ

**Q: Why dual-version strategy?**
A: Edge devices need ultra-lightweight C implementation (<1MB RAM), while desktop/servers need full-featured Node.js gateway with routing, auth, and logging.

**Q: Which version should I use?**
A: Use C version for edge devices (ESP32, Raspberry Pi Zero). Use Node.js version for desktop/servers (Linux, macOS, Windows).

**Q: Can I use this with OpenAI API?**
A: C version: Currently Ollama-only. Node.js version: Yes, via q-lite-gateway (supports OpenAI, Anthropic, Zai, etc.)

**Q: What's the minimum hardware?**
A: C version: Target: ESP32-S3 (1MB RAM). Recommended: Raspberry Pi Zero (512MB RAM). Node.js version: Desktop/server with Node.js >=18.0.0.

**Q: Is this production-ready?**
A: C version: Phase 4 MVP. Use with caution. Node.js version: Production-ready with full features.

**Q: Where is q-lite-gateway?**
A: https://github.com/RalphBigBear/q-lite-gateway

---

## Contact

- Issues: [GitHub Issues](https://github.com/RalphBigBear/q-lite/issues)
- Discussions: [GitHub Discussions](https://github.com/RalphBigBear/q-lite/discussions)

---

## License

MIT License - See [LICENSE](LICENSE) for details.

---

**Made with inspiration from Karpathy, Davebben, and Sipeed.**

*"Simplicity is the ultimate sophistication."* - Leonardo da Vinci

---

**Dual-Version Strategy**: C (Edge) + Node.js (Desktop) = **Full Coverage**
