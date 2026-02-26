# Q-Lite v0.4.0 - Semantic Awareness Gateway

**Release Date**: February 26, 2026
**Version**: v0.4.0
**Status**: 🎉 **Semantic Awareness Gateway Released!**

---

## 🎉 Major Updates

### New Feature: Semantic Awareness Gateway ✨

Q-Lite is now a **semantic awareness gateway** that provides:
- ✅ Cloud API Support (OpenAI + Anthropic)
- ✅ Sensor API (Temperature, Humidity, Light, Distance, etc.)
- ✅ Actuator API (LED, Servo, Relay, Buzzer, etc.)
- ✅ Rule Engine (Semantic Orchestration + Automatic Execution)

**Core Philosophy**:
> **"Provide low-level capabilities, users orchestrate via semantics"**

Users can describe rules in natural language, and the LLM automatically parses and generates executable rules - **no coding required**.

---

## 🎯 Use Cases

### Use Case 1: Smart Temperature Control System

**User Input** (Natural Language):
```
Create a smart temperature control system:
When temperature exceeds 30°C, turn on fan and set LED to red
When temperature drops below 20°C, turn off fan and set LED to blue
```

**LLM Automatically Generates Rules**:
```json
{
  "rules": [
    {
      "id": "rule-001",
      "name": "High Temperature Alert",
      "condition": {"type": "threshold", "sensor": "temp1", "operator": ">", "value": 30.0},
      "actions": [
        {"type": "actuator", "target_id": "fan1", "command": "on"},
        {"type": "actuator", "target_id": "led1", "command": "set", "value": {"r": 255, "g": 0, "b": 0}}
      ]
    },
    {
      "id": "rule-002",
      "name": "Low Temperature Mode",
      "condition": {"type": "threshold", "sensor": "temp1", "operator": "<", "value": 20.0},
      "actions": [
        {"type": "actuator", "target_id": "fan1", "command": "off"},
        {"type": "actuator", "target_id": "led1", "command": "set", "value": {"r": 0, "g": 0, "b": 255}}
      ]
    }
  ]
}
```

**Automatic Execution**:
1. Sensors read temperature data in real-time
2. Rule engine evaluates all rules every second
3. When temperature exceeds 30°C, automatically turn on fan + LED red
4. When temperature drops below 20°C, automatically turn off fan + LED blue

---

### Use Case 2: Smart Door Access System

**User Input** (Natural Language):
```
Create a smart door access system:
When distance sensor detects someone approaching (< 10cm),
beep buzzer, open servo door, wait 5 seconds, then close
```

**Automatic Execution**:
1. Distance sensor detects in real-time
2. When someone approaches (< 10cm):
   - Buzzer beeps (500ms)
   - Servo opens door (90°)
   - Wait 5 seconds
   - Servo closes door (0°)

---

## 🚀 Quick Start

### Step 1: Configure Cloud API

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

### Step 2: Configure Sensors

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
    }
  ]
}
```

### Step 3: Configure Actuators

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
    }
  ]
}
```

### Step 4: Create Rules (via LLM)

```bash
curl -X POST http://device-ip/chat/completions \
  -H "Content-Type: application/json" \
  -d '{
    "mode": "rule_generation",
    "prompt": "Create a rule: When temperature exceeds 30°C, turn on fan",
    "context": {
      "sensors": ["temp1"],
      "actuators": ["fan1", "led1"]
    }
  }'
```

### Step 5: Start Q-Lite

```bash
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

### Actuator API

```bash
# List all actuators
GET /actuators

# Turn on actuator
POST /actuators/fan1/on

# Turn off actuator
POST /actuators/fan1/off

# Set value (PWM, angle, etc.)
POST /actuators/servo1/angle
{"angle": 90}

# Set RGB LED
POST /actuators/led1/set
{"r": 255, "g": 0, "b": 0}
```

### Rule API

```bash
# List all rules
GET /rules

# Add rule
POST /rules
{
  "name": "High Temperature Fan Control",
  "condition": {
    "type": "threshold",
    "sensor": "temp1",
    "operator": ">",
    "value": 30.0
  },
  "actions": [
    {"type": "actuator", "target_id": "fan1", "command": "on"}
  ]
}

# Delete rule
DELETE /rules/rule-001

# Enable/disable rule
PUT /rules/rule-001
{"enabled": true}
```

---

## 📋 Supported Devices

### Sensors
- Temperature Sensors: DHT22, BME280
- Humidity Sensors: DHT22, BME280
- Light Sensors: BH1750
- Distance Sensors: HC-SR04
- Sound Sensors: Analog microphone
- Gas Sensors: MQ135
- Motion Sensors: PIR
- Pressure Sensors: BMP280

### Actuators
- LED: GPIO, PWM
- RGB LED: WS2812B
- Servo: PWM
- Relay: GPIO
- Buzzer: PWM
- Motor: PWM, H-bridge

---

## 🏗️ Architecture Design

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
│                 ↓                       │
│  ┌─────────────────────────────────┐    │
│  │ Sensor API (Sensors)           │    │
│  │  - Real-time read              │    │
│  │  - Data stream                │    │
│  └─────────────────────────────────┘    │
│                 ↓                       │
│  ┌─────────────────────────────────┐    │
│  │ Actuator API (Actuators) ✨     │    │
│  │  - Control commands            │    │
│  │  - Status query                │    │
│  └─────────────────────────────────┘    │
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

---

## 🔄 Upgrading from v0.3.1

### New Features
- ✅ Cloud API Support (OpenAI + Anthropic)
- ✅ Sensor API (8+ sensor types)
- ✅ Actuator API (6+ actuator types)
- ✅ Rule Engine (semantic orchestration + automatic execution)
- ✅ Configuration file examples (config, sensors, actuators, rules)

### API Changes
- New `/sensors/*` endpoints
- New `/actuators/*` endpoints
- New `/rules/*` endpoints
- New `/chat/completions` endpoint supports `mode=rule_generation`

---

## 📚 Documentation

- **[README.md](README.md)** - Main documentation
- **[PLATFORMS.md](docs/PLATFORMS.md)** - Platform support documentation
- **[API.md](docs/API.md)** - REST API documentation
- **[SENSORS.md](docs/SENSORS.md)** - Sensor driver documentation
- **[ACTUATORS.md](docs/ACTUATORS.md)** - Actuator driver documentation
- **[RULES.md](docs/RULES.md)** - Rule engine documentation

---

## 🙏 Acknowledgments

**Inspiration**:
- Andrej Karpathy (nanochat, llama2.c)
- mattdef (miniclaw) - Semantic orchestration philosophy

**Contributors**:
- Ralph (Primary development)

---

## 📄 License

MIT License - See [LICENSE](LICENSE) for details.

---

**Thank you for using Q-Lite!** 🚀

**Semantic Awareness Gateway - Let LLM drive the physical world**
