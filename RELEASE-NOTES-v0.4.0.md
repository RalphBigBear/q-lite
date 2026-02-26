# Q-Lite v0.4.0 - 语义感知网关

**Release Date**: February 26, 2026
**Version**: v0.4.0
**Status**: 🎉 **语义感知网关发布！**

---

## 🎉 重大更新

### 新功能：语义感知网关 ✨

Q-Lite 现在是一个**语义感知网关**，提供：
- ✅ 云端 API 支持 (OpenAI + Anthropic)
- ✅ 传感器 API (温度、湿度、光照、距离等)
- ✅ 执行器 API (LED、舵机、继电器、蜂鸣器等)
- ✅ 规则引擎 (语义编排 + 自动执行)

**核心理念**：
> "提供底层能力支持，由用户通过语义编排"

用户可以通过自然语言描述规则，LLM 自动解析并生成可执行规则，无需编写代码。

---

## 🎯 使用场景

### 场景 1: 智能温控系统

**用户输入**：
```
创建一个智能温控系统：
当温度超过30度时，开启风扇，LED变红
当温度低于20度时，关闭风扇，LED变蓝
```

**LLM 自动生成规则**：
```json
{
  "rules": [
    {
      "id": "rule-001",
      "name": "高温警报",
      "condition": {"type": "threshold", "sensor": "temp1", "operator": ">", "value": 30.0},
      "actions": [
        {"type": "actuator", "target_id": "fan1", "command": "on"},
        {"type": "actuator", "target_id": "led1", "command": "set", "value": {"r": 255, "g": 0, "b": 0}}
      ]
    },
    {
      "id": "rule-002",
      "name": "低温模式",
      "condition": {"type": "threshold", "sensor": "temp1", "operator": "<", "value": 20.0},
      "actions": [
        {"type": "actuator", "target_id": "fan1", "command": "off"},
        {"type": "actuator", "target_id": "led1", "command": "set", "value": {"r": 0, "g": 0, "b": 255}}
      ]
    }
  ]
}
```

**自动执行**：
1. 传感器实时读取温度数据
2. 规则引擎每秒评估所有规则
3. 当温度超过30度时，自动开启风扇 + LED变红
4. 当温度低于20度时，自动关闭风扇 + LED变蓝

---

### 场景 2: 智能门禁

**用户输入**：
```
创建一个智能门禁系统：
当距离传感器检测到有人靠近（< 10cm）时，
蜂鸣器响铃，舵机开门，等待5秒后关闭
```

**自动执行**：
1. 距离传感器实时检测
2. 当有人靠近（< 10cm）时：
   - 蜂鸣器响铃（500ms）
   - 舵机开门（90度）
   - 等待 5 秒
   - 舵机关门（0度）

---

## 🚀 快速开始

### 1. 配置云端 API

编辑 `config.json`：
```json
{
  "provider": "openai",
  "model": "gpt-4o-mini",
  "base_url": "https://api.openai.com/v1",
  "api_key": "sk-...",
  "timeout": 30000
}
```

或使用 Anthropic：
```json
{
  "provider": "anthropic",
  "model": "claude-3-5-sonnet-20241022",
  "base_url": "https://api.anthropic.com/v1",
  "api_key": "sk-ant-...",
  "timeout": 30000
}
```

### 2. 配置传感器

编辑 `sensors.json`：
```json
{
  "sensors": [
    {
      "id": "temp1",
      "name": "室内温度",
      "type": "temperature",
      "driver": "dht22",
      "driver_params": "pin=4",
      "interval_ms": 1000,
      "unit": "C"
    }
  ]
}
```

### 3. 配置执行器

编辑 `actuators.json`：
```json
{
  "actuators": [
    {
      "id": "fan1",
      "name": "风扇",
      "type": "relay",
      "driver": "gpio",
      "driver_params": "pin=17,active_low=false"
    }
  ]
}
```

### 4. 创建规则（通过 LLM）

```bash
curl -X POST http://device-ip/chat/completions \
  -H "Content-Type: application/json" \
  -d '{
    "mode": "rule_generation",
    "prompt": "创建一个规则：当温度超过30度时，开启风扇",
    "context": {
      "sensors": ["temp1"],
      "actuators": ["fan1", "led1"]
    }
  }'
```

### 5. 启动 Q-Lite

```bash
./q-lite \
  --config config.json \
  --sensors sensors.json \
  --actuators actuators.json \
  --rules rules.json \
  --port 8080
```

---

## 📚 API 文档

### 传感器 API

```bash
# 列出所有传感器
GET /sensors

# 读取特定传感器
GET /sensors/temp1

# 批量读取
POST /sensors/read
{"ids": ["temp1", "humid1", "light1"]}

# 实时数据流
WS /sensors/stream
```

### 执行器 API

```bash
# 列出所有执行器
GET /actuators

# 开启执行器
POST /actuators/fan1/on

# 关闭执行器
POST /actuators/fan1/off

# 设置值（PWM、角度等）
POST /actuators/servo1/angle
{"angle": 90}

# 设置 RGB LED
POST /actuators/led1/set
{"r": 255, "g": 0, "b": 0}
```

### 规则 API

```bash
# 列出所有规则
GET /rules

# 添加规则
POST /rules
{
  "name": "高温风扇控制",
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

# 删除规则
DELETE /rules/rule-001

# 启用/禁用规则
PUT /rules/rule-001
{"enabled": true}
```

---

## 📋 支持的设备

### 传感器
- 温度传感器：DHT22, BME280
- 湿度传感器：DHT22, BME280
- 光照传感器：BH1750
- 距离传感器：HC-SR04
- 声音传感器：模拟麦克风
- 气体传感器：MQ135
- 运动传感器：PIR
- 压力传感器：BMP280

### 执行器
- LED：GPIO, PWM
- RGB LED：WS2812B
- 舵机：PWM
- 继电器：GPIO
- 蜂鸣器：PWM
- 电机：PWM, H-bridge

---

## 🏗️ 架构设计

```
┌─────────────────────────────────────────┐
│ 用户 (自然语言)                       │
│ "当温度超过30度时，开启风扇"          │
└──────────────┬────────────────────────┘
               ↓ HTTP
┌─────────────────────────────────────────┐
│ Q-Lite Gateway (语义感知网关)         │
│                                         │
│  ┌─────────────────────────────────┐    │
│  │ LLM Client (云端 API) ✨     │    │
│  │  - OpenAI / Anthropic          │    │
│  │  - 语义理解 + 规则生成        │    │
│  └─────────────────────────────────┘    │
│                 ↓                       │
│  ┌─────────────────────────────────┐    │
│  │ Rule Engine (规则引擎) ✨    │    │
│  │  - 条件评估                  │    │
│  │  - 动作执行                  │    │
│  └─────────────────────────────────┘    │
│                 ↓                       │
│  ┌─────────────────────────────────┐    │
│  │ Sensor API (传感器)           │    │
│  │  - 实时读取                  │    │
│  │  - 数据流                    │    │
│  └─────────────────────────────────┘    │
│                 ↓                       │
│  ┌─────────────────────────────────┐    │
│  │ Actuator API (执行器) ✨     │    │
│  │  - 控制命令                  │    │
│  │  - 状态查询                  │    │
│  └─────────────────────────────────┘    │
└──────────────┬────────────────────────┘
               ↓
        ┌──────┴──────┐
        ↓             ↓
┌───────────┐  ┌──────────┐
│  传感器    │  │ 执行器    │
│ 温度/湿度/  │  │ LED/舵机/ │
│ 光照/距离   │  │ 继电器/   │
└───────────┘  │ 蜂鸣器    │
               └──────────┘
```

---

## 🔄 从 v0.3.1 升级

### 新增功能
- ✅ 云端 API 支持 (OpenAI + Anthropic)
- ✅ 传感器 API (8+ 传感器类型)
- ✅ 执行器 API (6+ 执行器类型)
- ✅ 规则引擎 (语义编排 + 自动执行)
- ✅ 配置文件示例 (config, sensors, actuators, rules)

### API 变更
- 新增 `/sensors/*` 端点
- 新增 `/actuators/*` 端点
- 新增 `/rules/*` 端点
- 新增 `/chat/completions` 端点支持 `mode=rule_generation`

---

## 📚 文档

- **[README.md](README.md)** - 主文档
- **[PLATFORMS.md](docs/PLATFORMS.md)** - 平台支持文档
- **[API.md](docs/API.md)** - REST API 文档
- **[SENSORS.md](docs/SENSORS.md)** - 传感器驱动文档
- **[ACTUATORS.md](docs/ACTUATORS.md)** - 执行器驱动文档
- **[RULES.md](docs/RULES.md)** - 规则引擎文档

---

## 🙏 致谢

**灵感来源**：
- Andrej Karpathy (nanochat, llama2.c)
- mattdef (miniclaw) - 语义编排理念

** contributors**：
- Ralph (主开发)

---

## 📄 License

MIT License - See [LICENSE](LICENSE) for details.

---

**Thank you for using Q-Lite!** 🚀

**语义感知网关 - 让 LLM 驱动物理世界**
