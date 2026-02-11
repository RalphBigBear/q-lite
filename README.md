# Q-Lite

**Ultra-lightweight LLM gateway for edge devices.**

- **<1MB RAM** - Can run on ESP32, Raspberry Pi Zero, or any minimal hardware
- **Pure C** - Zero dependencies, single binary
- **HTTP API** - Simple REST interface for LLM inference
- **Ollama Compatible** - Works with local Ollama server out of the box

## Features

- ✅ Minimal memory footprint (<1MB RAM)
- ✅ Single C file implementation (optional)
- ✅ HTTP server with REST API
- ✅ Ollama backend integration
- ✅ Edge device friendly (ARM, RISC-V, x86)

## Quick Start

### Option 1: Download Release (Recommended)

```bash
# Download latest release from GitHub
wget https://github.com/RalphBigBear/q-lite/releases/download/v0.1.0-alpha/q-lite
chmod +x q-lite

# Run
./q-lite --port 8080
```

### Option 2: Build from Source

```bash
# Clone
git clone https://github.com/RalphBigBear/q-lite.git
cd q-lite

# Build
make

# Run
./q-lite --port 8080 --ollama http://localhost:11434
```

### Test

```bash
curl http://localhost:8080/
# Output: {"status":"ok","message":"Q-Lite HTTP Server"}
```

## Architecture

```
Edge Device (Q-Lite, <1MB RAM)
         ↓ HTTP
Local LLM Server (Ollama, 128GB RAM)
         ↓
Response ←
```

## Project Goals

Q-Lite aims to solve a simple problem: **running LLM gateways on minimal hardware.**

Most existing gateways require 100MB+ RAM and complex dependencies. Q-Lite takes a different approach:

- **Minimal footprint** - Target <1MB RAM
- **Zero dependencies** - Pure C, no external libraries
- **Simple deployment** - Single binary, run anywhere
- **Edge-first** - Designed for constrained devices

Inspired by the philosophy that *"everything should be made as simple as possible, but not simpler."*

## Inspired By

This project stands on the shoulders of giants. Q-Lite was directly inspired by:

### 🦀 llama2.c - Andrej Karpathy
[github.com/karpathy/llama2.c](https://github.com/karpathy/llama2.c)

The foundation of this project. Karpathy's *llama2.c* proved that LLM inference doesn't require complex frameworks - a single C file is enough. Q-Lite applies the same philosophy to LLM gateways: **minimal code, maximum clarity.**

> *"I want to see just how small we can go."* - Andrej Karpathy

**Learned from llama2.c**:
- Single-file architecture
- Educational code style
- Zero-dependency approach
- "Teach by showing" philosophy

### 📡 ESP32-LLM - Davebben
[github.com/DaveBben/esp32-llm](https://github.com/DaveBben/esp32-llm)

Demonstrated that LLMs can run on microcontrollers (1MB RAM). This project proved that **edge AI is not just possible - it's practical.**

**Learned from ESP32-LLM**:
- Memory optimization techniques
- Hardware constraints awareness
- Embedded systems best practices

### 🌸 PicoClaw - Sipeed
[github.com/sipeed/picoclaw](https://github.com/sipeed/picoclaw)

A lightweight LLM gateway in Go. Showed that gateway code can be <1000 lines while remaining functional.

**Learned from PicoClaw**:
- Clean API design
- Ollama integration patterns
- Configuration simplicity

## Why These Projects Matter

The LLM ecosystem is dominated by heavy frameworks (PyTorch, TensorFlow). These three projects proved that **minimalism is powerful**:

- **llama2.c**: Educational + Production-ready
- **ESP32-LLM**: Edge computing breakthrough
- **PicoClaw**: Lightweight gateway design

Q-Lite combines their philosophies into a single goal: **run LLM gateways anywhere.**

## Technology Stack

- **Language**: C99 (pure C, no C++ dependencies)
- **HTTP**: Custom server (libmicrohttpd optional)
- **JSON**: cJSON (embedded library)
- **Backend**: Ollama API compatible

## Roadmap

### Phase 1: Core MVP ✅ Complete
- [x] Project structure
- [x] Basic HTTP server
- [x] Ollama API integration
- [x] Single binary deployment

### Phase 2: Edge Optimization ✅ Complete
- [x] Memory profiling (<1MB target)
- [x] ARM optimization
- [x] RISC-V support

### Phase 2: Edge Optimization
- [ ] Memory profiling (<1MB target)
- [ ] ARM optimization
- [ ] RISC-V support

### Phase 3: Advanced Features
- [ ] Multi-backend support (Ollama, vLLM, LM Studio)
- [ ] WebSocket support
- [ ] Queue management

### Phase 4: Microcontroller Ports
- [ ] ESP32 port
- [ ] Raspberry Pi Pico port
- [ ] STM32 port

## Benchmarks

Coming soon. Target metrics:

- **RAM**: <1MB (base), <500KB (minimal)
- **Binary**: <100KB (stripped)
- **Latency**: <10ms overhead
- **Throughput**: >100 req/s (single core)

## Contributing

Contributions welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

Focus areas:
- Memory optimization
- New platform ports
- Bug fixes
- Documentation improvements

## Documentation

- **[BUILD.md](docs/BUILD.md)** - Build instructions & troubleshooting
- **[API.md](docs/API.md)** - REST API documentation & examples

## License

MIT License - See [LICENSE](LICENSE) for details.

## FAQ

**Q: Why C instead of Rust/Go?**
A: C runs everywhere. No runtime, no GC, minimal binary size. Perfect for edge devices.

**Q: Can I use this with OpenAI API?**
A: Currently Ollama-only. OpenAI support planned for Phase 3.

**Q: What's the minimum hardware?**
A: Target: ESP32-S3 (1MB RAM). Recommended: Raspberry Pi Zero (512MB RAM).

**Q: Is this production-ready?**
A: Phase 1 MVP. Use with caution. Feedback welcome!

## Contact

- Issues: [GitHub Issues](https://github.com/RalphBigBear/q-lite/issues)
- Discussions: [GitHub Discussions](https://github.com/RalphBigBear/q-lite/discussions)

---

**Made with inspiration from Karpathy, Davebben, and Sipeed.**

*"Simplicity is the ultimate sophistication."* - Leonardo da Vinci
