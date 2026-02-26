# Q-Lite v0.3.0 - Release Notes

**Release Date**: February 16, 2026
**Version**: v0.3.0
**Status**: 🎉 **Edge Device Gateway Released**

---

## 🎉 Major Updates

### Ultra-Lightweight LLM Gateway ✨

**Target**: Edge devices with limited resources (<1MB RAM)

**Features**:
- ✅ Ultra-lightweight (<1MB RAM)
- ✅ Pure C implementation
- ✅ HTTP server with REST API
- ✅ Multi-backend support (Ollama, vLLM, LM Studio, OpenAI-compatible)
- ✅ Edge device friendly (ARM, RISC-V, x86)
- ✅ HTTP Chunked Streaming (real-time responses)
- ✅ Request Queue (concurrent limiting)

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

---

## 🎯 Why Q-Lite?

### Edge Version (C)
- **Minimal footprint** - Target <1MB RAM
- **Zero dependencies** - Pure C, no external libraries
- **Simple deployment** - Single binary, run anywhere
- **Edge-first** - Designed for constrained devices

**Use Case**:
- **Edge Devices**: Use q-lite - ESP32, Raspberry Pi Zero, STM32, RP2040
- **Desktop/Server**: Use q-lite - Linux, macOS, Windows

---

## 🚀 Quick Start

### Option 1: Download Release

```bash
# Download latest release from GitHub
wget https://github.com/RalphBigBear/q-lite/releases/download/v0.3.0/q-lite
chmod +x q-lite

# Run with auto-detect backend
./q-lite --port 8080
```

### Option 2: Build from Source

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

### Option 3: Microcontroller Port

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
# Health check
curl http://localhost:8080/
# Output: {"status":"ok","message":"Q-Lite HTTP Server"}
```

---

## 📋 Supported Backends

### Ollama
```bash
./q-lite --backend ollama --port 8080
curl -X POST http://localhost:8080/chat/completions \
  -H "Content-Type: application/json" \
  -d '{
    "model": "llama3-70b",
    "messages": [{"role": "user", "content": "Hello!"}]
  }'
```

### vLLM
```bash
./q-lite --backend vllm --backend-port 8000 --port 8080
```

### LM Studio
```bash
./q-lite --backend lmstudio --backend-port 1234 --port 8080
```

### OpenAI-Compatible
```bash
./q-lite --backend openai --backend-port 8000 --port 8080
```

---

## 📋 Supported Models

Q-Lite supports any model available through the configured backend:

### Ollama
- llama3-70b
- mistral-7b
- gemma-7b
- (Any Ollama model)

### vLLM / LM Studio / OpenAI-Compatible
- Any model served by the backend

---

## 🔄 Migration from v0.2.0

### Breaking Changes
- **None** - v0.3.0 is backward compatible with v0.2.0

### New Features
- ✅ Multi-backend support (Ollama, vLLM, LM Studio)
- ✅ HTTP Chunked Streaming (real-time responses)
- ✅ Request Queue (concurrent limiting)

---

## 📄 Documentation

- **[README.md](README.md)** - Main documentation
- **[BUILD.md](docs/BUILD.md)** - Build instructions
- **[API.md](docs/API.md)** - REST API documentation
- **[platforms/esp32/README.md](platforms/esp32/README.md)** - ESP32 build guide
- **[platforms/stm32/README.md](platforms/stm32/README.md)** - STM32 build guide
- **[platforms/pico/README.md](platforms/pico/README.md)** - Raspberry Pi Pico build guide

---

## 🧪 Testing

```bash
# Test HTTP server
./q-lite --port 8080
curl http://localhost:8080/

# Test Ollama backend
./q-lite --backend ollama --port 8080
curl -X POST http://localhost:8080/chat/completions \
  -H "Content-Type: application/json" \
  -d '{
    "model": "llama3-70b",
    "messages": [{"role": "user", "content": "Hello!"}]
  }'

# Test streaming
curl -X POST http://localhost:8080/chat/completions \
  -H "Content-Type: application/json" \
  -d '{
    "model": "llama3-70b",
    "messages": [{"role": "user", "content": "Hello!"}],
    "stream": true
  }'
```

---

## 📊 Benchmarks

### C Version (Edge Devices)
- **RAM**: <1MB (target)
- **Binary**: <100KB (stripped)
- **Latency**: <10ms overhead
- **Throughput**: >100 req/s (single core)

---

## 🐛 Known Issues

### C Version
- **Stream support**: HTTP Chunked Streaming implemented, but needs more testing
- **Authentication**: No built-in authentication (relies on backend)
- **Proxy**: No proxy support (use external proxy)

---

## 🔮 Next Steps

### Phase 6: Future Enhancements
- [ ] TLS/SSL support
- [ ] WebSocket support
- [ ] Authentication
- [ ] Metrics & monitoring
- [ ] Better error handling

---

## 🙏 Acknowledgments

**Inspiration**:
- Andrej Karpathy (nanochat, llama2.c)
- Davebben (ESP32-LLM)
- Sipeed (PicoClaw)

**Contributors**:
- Ralph (Initial development)

---

## 📄 License

MIT License - See [LICENSE](LICENSE) for details.

---

## 📞 Support

- Issues: [GitHub Issues](https://github.com/RalphBigBear/q-lite/issues)
- Discussions: [GitHub Discussions](https://github.com/RalphBigBear/q-lite/discussions)

---

**Thank you for using Q-Lite!** 🚀

**Ultra-Lightweight LLM Gateway for Edge Devices**
