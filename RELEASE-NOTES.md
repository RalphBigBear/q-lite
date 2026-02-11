# Q-Lite v0.2.0 Release Notes

**Release Date**: 2026-02-11
**Status**: Production Ready 🎉

---

## 🎯 Overview

Q-Lite v0.2.0 is a **major milestone** - complete multi-platform support for ultra-lightweight LLM gateway.

### Key Features

✅ **4 Development Phases Complete**
- Phase 1: Core MVP (HTTP Server, Ollama Client)
- Phase 2: Edge Optimization (Memory, ARM, RISC-V)
- Phase 3: Advanced Features (Multi-Backend, Streaming, Queue)
- Phase 4: Microcontroller Ports (ESP32, STM32, Pico)

✅ **Multi-Platform Support**
- Desktop (Linux, macOS, Windows)
- ESP32 (WiFi)
- STM32 (Ethernet)
- Raspberry Pi Pico (UART WiFi)

✅ **Low Entropy Design**
- Binary: 69KB (31% under target)
- Code: ~2500 LOC
- Zero external dependencies

---

## 📊 What's New

### Phase 3: Advanced Features

**Multi-Backend Support**
- Ollama (default)
- vLLM (OpenAI-compatible)
- LM Studio (OpenAI-compatible)
- Auto-detect backend (port scanning)

**HTTP Chunked Streaming**
- Real-time LLM responses
- Zero WebSocket dependency
- ~80 LOC added

**Request Queue**
- Max 10 concurrent requests
- 503 Service Unavailable on overload
- Thread-safe atomic counters

### Phase 4: Microcontroller Ports

**Platform Abstraction Layer**
- Unified interface for all platforms
- ~500 LOC (platform implementations)
- Easy to extend

**Supported Platforms**:

| Platform | Flash | RAM | Network | Binary |
|----------|-------|-----|----------|--------|
| Desktop | N/A | N/A | Ethernet/WiFi | 69KB |
| ESP32-S3 | 100KB | 50KB | WiFi (built-in) | ~100KB |
| STM32F4/F7 | 80KB | 40KB | Ethernet | ~80KB |
| RP2040 | 60KB | 30KB | WiFi (ESP8266) | ~60KB |

---

## 🚀 Installation

### Desktop (Linux/macOS/Windows)

```bash
# Download pre-built binary
wget https://github.com/RalphBigBear/q-lite/releases/download/v0.2.0/q-lite
chmod +x q-lite

# Run
./q-lite --backend auto --port 8080
```

### Build from Source

```bash
git clone https://github.com/RalphBigBear/q-lite.git
cd q-lite
make
./q-lite --backend auto
```

### ESP32

```bash
cd platforms/esp32
idf.py build
idf.py flash
```

### STM32

1. Open `platforms/stm32/` in STM32CubeIDE
2. Configure Ethernet (LAN8720)
3. Build & Flash

### Raspberry Pi Pico

```bash
cd platforms/pico
mkdir build && cd build
cmake ..
make
# Copy q-lite-pico.uf2 to Pico (hold BOOTSEL)
```

---

## 📚 Documentation

- **[BUILD.md](docs/BUILD.md)** - Build instructions for all platforms
- **[API.md](docs/API.md)** - REST API reference
- **[RESEARCH-*.md](docs/)** - Design research & analysis

---

## 🐛 Bug Fixes

- Fixed backend auto-detection (port scanning)
- Fixed HTTP chunked encoding
- Fixed request queue race conditions

---

## 🔮 Future Work

Potential enhancements for future releases:
- [ ] SSL/TLS support
- [ ] MQTT integration
- [ ] More microcontroller ports (nRF52, ESP8266)
- [ ] Power profiling

---

## 🙏 Acknowledgments

Inspired by:
- **llama2.c** (Andrej Karpathy) - Single-file LLM philosophy
- **ESP32-LLM** (Davebben) - Edge AI proof-of-concept
- **PicoClaw** (Sipeed) - Lightweight gateway design

---

## 📄 License

MIT License - See [LICENSE](LICENSE)

---

## 📥 Download

- **[q-lite](https://github.com/RalphBigBear/q-lite/releases/download/v0.2.0/q-lite)** - Desktop binary (Linux/macOS)
- Source code: https://github.com/RalphBigBear/q-lite/tree/v0.2.0

---

**Made with ❤️ for edge AI**
