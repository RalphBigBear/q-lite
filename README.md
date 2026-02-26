# Q-Lite

**Semantic Awareness Gateway for Edge Devices** 🌟

- **<1MB RAM** - Can run on ESP32, Raspberry Pi Zero, or any minimal hardware
- **Pure C** - Zero dependencies, single binary
- **Multi-Platform** - Linux, macOS, Windows, ESP32, STM32, RP2040
- **Ollama Compatible** - Works with local Ollama server out of box
- **✨ Semantic Orchestration** - Natural language to automation rules via LLM

---

## 🎯 What is Q-Lite?

Q-Lite is a **semantic awareness gateway** designed for edge devices with limited resources. It provides:

### Core Features (v0.4.0)

- ✅ **Cloud API Support** - OpenAI, Anthropic (Claude), OpenAI-compatible APIs
- ✅ **Sensor API** - Temperature, humidity, light, distance, sound, gas, motion, pressure
- ✅ **Actuator API** - LED, RGB LED, servo, relay, buzzer, motor control
- ✅ **Rule Engine** - Semantic orchestration + automatic execution
- ✅ **Ultra-lightweight** - <1MB RAM, ~100KB binary
- ✅ **Pure C** - Zero dependencies
- ✅ **HTTP Server** - REST API endpoints
- ✅ **Edge Device Friendly** - ARM, RISC-V, x86

### Core Philosophy

> **"Provide low-level capabilities, users orchestrate via semantics"**

Users can describe automation rules in natural language:
> *"Create a rule: When temperature exceeds 30°C, turn on fan and set LED to red"*

The LLM automatically parses and generates executable rules - **no coding required**.

---

## 🏗️ Architecture

```
┌─────────────────────────────────────────┐
│ User (Natural Language)                │
│ "When temperature exceeds 30°C,        │
│  turn on fan and set LED to red"       │
└──────────────┬────────────────────────┘
               ↓ HTTP /chat/completions
┌─────────────────────────────────────────┐
│ Q-Lite Gateway (Semantic Awareness)    │
│                                         │
│  ┌─────────────────────────────────┐    │
│  │ LLM Client (Cloud API) ✨    │    │
│  │  - OpenAI / Anthropic          │    │
│  │  - Semantic understanding      │    │
│  │  - Rule generation             │    │
│  └─────────────────────────────────┘    │
│                 ↓ POST /rules              │
│  ┌─────────────────────────────────┐    │
│  │ Rule Engine ✨                │    │
│  │  - Load rules                 │    │
│  │  - Monitor sensors            │    │
│  │  - Trigger actuators         │    │
│  └─────────────────────────────────┘    │
│         ↓              ↓                │
│  ┌────────────┐  ┌────────────┐  │
│  │ Sensor API │  │Actuator API│  │
│  └────────────┘  └────────────┘  │
└──────────────┬────────────────────────┘
               ↓
        ┌──────┴──────┐
        ↓             ↓
┌───────────┐  ┌──────────┐
│ Sensors   │  │Actuators│
│ Temp/Humid│  │ LED/     │
│ Light/    │  │ Servo/   │
│ Distance/  │  │ Relay/   │
│ Sound/     │  │ Buzzer/  │
│ Gas/Motion │  │ Motor    │
└───────────┘  └──────────┘
```

### Components

- **LLM Client** - Cloud API integration (OpenAI, Anthropic)
- **Rule Engine** - Semantic rule orchestration + automatic execution
- **Sensor API** - Unified sensor interface (8+ sensor types)
- **Actuator API** - Unified actuator interface (6+ actuator types)
- **HTTP Server** - REST API endpoints
- **Request Queue** - Concurrent limiting (max 10 requests)

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

## 🎮 Usage Examples

### Example 1: Smart Temperature Control System

**User Input (Natural Language)**:
```
"Create a smart temperature control system:
When temperature exceeds 30°C, turn on fan and set LED to red
When temperature drops below 20°C, turn off fan and set LED to blue"
```

**LLM Generates Rules (JSON)**:
```json
{
  "rules": [
    {
      "id": "rule-001",
      "name": "High Temperature Alert",
      "condition": {
        "type": "threshold",
        "sensor": "temp1",
        "operator": ">",
        "value": 30.0
      },
      "actions": [
        {
          "type": "actuator",
          "target_id": "fan1",
          "command": "on"
        },
        {
          "type": "actuator",
          "target_id": "led1",
          "command": "set",
          "value": {"r": 255, "g": 0, "b": 0}
        }
      ]
    },
    {
      "id": "rule-002",
      "name": "Low Temperature Mode",
      "condition": {
        "type": "threshold",
        "sensor": "temp1",
        "operator": "<",
        "value": 20.0
      },
      "actions": [
        {
          "type": "actuator",
          "target_id": "fan1",
          "command": "off"
        },
        {
          "type": "actuator",
          "target_id": "led1",
          "command": "set",
          "value": {"r": 0, "g": 0, "b": 255}
        }
      ]
    }
  ]
}
```

**Automatic Execution**:
1. Sensor reads temperature data every 1000ms
2. Rule engine evaluates all rules every 1000ms
3. When `temp1 > 30.0`: Turn on fan + LED red
4. When `temp1 < 20.0`: Turn off fan + LED blue

---

### Example 2: Smart Door Access System

**User Input (Natural Language)**:
```
"Create a smart door access system:
When distance sensor detects someone approaching (< 10cm),
beep buzzer, open servo door, wait 5 seconds, then close"
```

**Automatic Execution**:
1. Distance sensor reads every 100ms
2. When someone approaches (`distance1 < 10.0`):
   - Buzzer beeps (500ms)
   - Servo opens door (90°)
   - Wait 5 seconds
   - Servo closes door (0°)

---

## 🚀 Quick Start

### Step 1: Download Release

```bash
# Download latest release from GitHub
wget https://github.com/RalphBigBear/q-lite/releases/download/v0.4.0/q-lite
chmod +x q-lite

# Or build from source
git clone https://github.com/RalphBigBear/q-lite.git
cd q-lite
make
```

### Step 2: Configure Cloud API (Optional)

Edit `config.json`:

**OpenAI**:
```json
{
  "provider": "openai",
  "model": "gpt-4o-mini",
  "base_url": "https://api.openai.com/v1",
  "api_key": "sk-...",
  "timeout": 30000
}
```

**Anthropic**:
```json
{
  "provider": "anthropic",
  "model": "claude-3-5-sonnet-20241022",
  "base_url": "https://api.anthropic.com/v1",
  "api_key": "sk-ant-...",
  "timeout": 30000
}
```

### Step 3: Configure Sensors (Optional)

Edit `sensors.json`:

```json
{
  "sensors": [
    {
      "id": "temp1",
      "name": "Indoor Temperature",
      "type": "temperature",
      "driver": "dht22",
      "driver_params": "pin=4",
      "interval_ms": 1000,
      "unit": "C"
    },
    {
      "id": "distance1",
      "name": "Distance Sensor",
      "type": "distance",
      "driver": "hc-sr04",
      "driver_params": "trigger_pin=5,echo_pin=18",
      "interval_ms": 100,
      "unit": "cm"
    }
  ]
}
```

### Step 4: Configure Actuators (Optional)

Edit `actuators.json`:

```json
{
  "actuators": [
    {
      "id": "fan1",
      "name": "Fan",
      "type": "relay",
      "driver": "gpio",
      "driver_params": "pin=17,active_low=false"
    },
    {
      "id": "led1",
      "name": "Status LED",
      "type": "rgb_led",
      "driver": "ws2812b",
      "driver_params": "pin=15,num_leds=12"
    }
  ]
}
```

### Step 5: Start Q-Lite

```bash
# Auto-detect backend (Ollama, vLLM, LM Studio)
./q-lite --port 8080

# Specify backend
./q-lite --backend openai --port 8080

# With sensors and actuators
./q-lite \
  --config config.json \
  --sensors sensors.json \
  --actuators actuators.json \
  --rules rules.json \
  --port 8080
```

---

## 📚 API Documentation

### Sensor API

```bash
# List all sensors
GET /sensors

# Read specific sensor
GET /sensors/temp1

# Batch read
POST /sensors/read
{"ids": ["temp1", "humid1", "light1"]}

# Real-time data stream
WS /sensors/stream
```

**Response Example**:
```json
{
  "sensors": [
    {
      "id": "temp1",
      "name": "Indoor Temperature",
      "type": 1,
      "driver": 0,
      "unit": "C",
      "value": 25.5,
      "last_update": 1708900000
    }
  ]
}
```

### Actuator API

```bash
# List all actuators
GET /actuators

# Turn on actuator
POST /actuators/fan1/on

# Turn off actuator
POST /actuators/fan1/off

# Set value (PWM, angle, etc.)
POST /actuators/led1/set
{"r": 255, "g": 0, "b": 0}

# Set servo angle
POST /actuators/servo1/angle
{"angle": 90}

# Beep buzzer
POST /actuators/buzzer1/beep
{"duration_ms": 1000}
```

### Rule API

```bash
# List all rules
GET /rules

# Add rule (from LLM or manual)
POST /rules
{
  "name": "High Temperature Alert",
  "condition": {
    "type": "threshold",
    "sensor": "temp1",
    "operator": ">",
    "value": 30.0
  },
  "actions": [
    {
      "type": "actuator",
      "target_id": "fan1",
      "command": "on"
    }
  ]
}

# Delete rule
DELETE /rules/rule-001

# Enable/disable rule
PUT /rules/rule-001
{"enabled": true}
```

### LLM Rule Generation

```bash
# Generate rule from natural language
POST /chat/completions
{
  "mode": "rule_generation",
  "prompt": "Create a rule: When temperature exceeds 30°C, turn on fan",
  "context": {
    "sensors": ["temp1", "humid1"],
    "actuators": ["fan1", "led1"]
  }
}

# LLM generates executable rule JSON
{
  "rule": {
    "id": "rule-001",
    "name": "High Temperature Alert",
    "condition": {
      "type": "threshold",
      "sensor": "temp1",
      "operator": ">",
      "value": 30.0
    },
    "actions": [
      {
        "type": "actuator",
        "target_id": "fan1",
        "command": "on"
      }
    ]
  }
}
```

---

## 🎓 Supported Devices

### Sensors (8+ Types)

| Type | Drivers | Examples |
|------|---------|----------|
| Temperature | DHT22, BME280 | Indoor temp |
| Humidity | DHT22, BME280 | Indoor humidity |
| Light | BH1750 | Ambient light |
| Distance | HC-SR04 | Proximity detection |
| Sound | Analog mic | Sound detection |
| Gas | MQ135 | Air quality |
| Motion | PIR | Motion detection |
| Pressure | BMP280 | Barometric pressure |

### Actuators (6+ Types)

| Type | Drivers | Examples |
|------|---------|----------|
| LED | GPIO, PWM | Status indicator |
| RGB LED | WS2812B | RGB status light |
| Servo | PWM | Door lock, camera pan |
| Relay | GPIO | Fan, light switch |
| Buzzer | PWM | Alarm, notification |
| Motor | PWM, H-bridge | Wheel, pump |

---

## 🎓 Inspired By

This project stands on shoulders of giants, drawing inspiration from:

### 🦀 nanochat - Andrej Karpathy
[github.com/karpathy/nanochat](https://github.com/karpathy/nanochat)

**Single-dial philosophy**: Inspired by nanochat's `--depth` parameter, Q-Lite adopts a simplified `--target` preset system (e.g., `--target esp32`, `--target desktop`). One parameter auto-configures all platform-specific settings.

### 🦌 llama2.c - Andrej Karpathy
[github.com/karpathy/llama2.c](https://github.com/karpathy/llama2.c)

**Single-file architecture**: Q-Lite applies the same philosophy to LLM gateways - minimal code, maximum clarity.

### 📡 ESP32-LLM - Davebben
[github.com/DaveBben/esp32-llm](https://github.com/DaveBben/esp32-llm)

**Edge AI breakthrough**: Proved that LLMs can run on 1MB RAM. Q-Lite builds on this for gateway deployment.

### 🎮 miniclaw - mattdef
[github.com/mattdef/miniclaw](https://github.com/mattdef/miniclaw)

**Semantic orchestration**: Inspiration for natural language rule generation and edge agent capabilities.

---

## 📚 Documentation

- **[BUILD.md](docs/BUILD.md)** - Build instructions & troubleshooting
- **[API.md](docs/API.md)** - REST API documentation & examples
- **[platforms/esp32/README.md](platforms/esp32/README.md)** - ESP32 build guide
- **[platforms/stm32/README.md](platforms/stm32/README.md)** - STM32 build guide
- **[platforms/pico/README.md](platforms/pico/README.md)** - Raspberry Pi Pico build guide
- **[RELEASE-NOTES.md](RELEASE-NOTES.md)** - Release notes and changelog

---

## 🔧 Technology Stack

- **Language**: C99 (pure C, no C++ dependencies)
- **HTTP**: Custom HTTP server
- **Backends**: Ollama, OpenAI, Anthropic
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

### Phase 5: Semantic Awareness ✅ Complete (v0.4.0)
- [x] Cloud API support (OpenAI, Anthropic)
- [x] Sensor API (8+ sensor types)
- [x] Actuator API (6+ actuator types)
- [x] Rule engine (semantic orchestration)
- [x] Natural language rule generation

### Phase 6: Future Enhancements
- [ ] WebSocket real-time data stream
- [ ] MQTT notification support
- [ ] Time-based scheduling (e.g., "every day at 08:00")
- [ ] Complex conditions (e.g., "temp > 30°C AND humidity < 50%")
- [ ] Platform-specific sensor drivers (ESP32, STM32, RP2040)

---

## 🤝 Contributing

Contributions welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

Focus areas:
- Memory optimization
- New sensor drivers
- New actuator drivers
- Bug fixes
- Documentation improvements

---

## ❓ FAQ

**Q: What's the minimum hardware?**
A: Target: ESP32-S3 (520KB RAM). Recommended: Raspberry Pi Zero (512MB RAM).

**Q: Can I use this with OpenAI API?**
A: Yes, Q-Lite supports OpenAI, Anthropic (Claude), and OpenAI-compatible APIs.

**Q: How does semantic orchestration work?**
A: You describe rules in natural language ("When temperature exceeds 30°C, turn on fan"), LLM parses and generates executable JSON rules automatically.

**Q: Is this production-ready?**
A: v0.4.0 is a semantic awareness gateway prototype. Use with caution in production.

**Q: How do I configure WiFi on ESP32?**
A: See [platforms/esp32/README.md](platforms/esp32/README.md) for detailed instructions.

**Q: Do I need an LLM for semantic orchestration?**
A: Optional. You can write rules manually in JSON format, or use LLM for automatic rule generation from natural language.

---

## 📄 License

MIT License - See [LICENSE](LICENSE) for details.

---

## 🔗 Links

- **GitHub**: [https://github.com/RalphBigBear/q-lite](https://github.com/RalphBigBear/q-lite)
- **Issues**: [https://github.com/RalphBigBear/q-lite/issues](https://github.com/RalphBigBear/q-lite/issues)
- **Discussions**: [https://github.com/RalphBigBear/q-lite/discussions](https://github.com/RalphBigBear/q-lite/discussions)
- **Latest Release**: [https://github.com/RalphBigBear/q-lite/releases/latest](https://github.com/RalphBigBear/q-lite/releases/latest)

---

**Made with inspiration from Karpathy, Davebben, and mattdef.**

*"Simplicity is the ultimate sophistication."* - Leonardo da Vinci

**"Provide low-level capabilities, users orchestrate via semantics" - Q-Lite v0.4.0** 🌟
