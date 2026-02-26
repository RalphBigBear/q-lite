# Q-Lite for ESP32

Ultra-lightweight LLM gateway for ESP32/ESP32-S3 microcontrollers.

---

## 🎯 Supported Targets

- **ESP32-S3** (recommended) - Dual-core, 520KB RAM, WiFi
- **ESP32** - Single/dual-core, 520KB RAM, WiFi
- **ESP32-C3** - Single-core RISC-V, 400KB RAM, WiFi

---

## 📋 Prerequisites

1. **ESP-IDF v5.x** (tested on v5.5.3)
   ```bash
   # Install ESP-IDF
   git clone --recursive https://github.com/espressif/esp-idf.git
   cd esp-idf
   git checkout v5.5.3
   ./install.sh esp32s3
   ```

2. **Hardware Requirements**
   - ESP32-S3-DevKitC-1 (4MB Flash, 520KB RAM)
   - USB-C cable for programming
   - WiFi access

---

## 🚀 Quick Start

### Step 1: Set Target Chip

```bash
cd platforms/esp32

# Set target to ESP32-S3
idf.py set-target esp32s3

# Alternative: ESP32 or ESP32-C3
# idf.py set-target esp32
# idf.py set-target esp32c3
```

### Step 2: Configure WiFi

Edit `main/cmake/include/sdkconfig.h` or set at runtime:

```c
// Hardcode WiFi credentials (not recommended for production)
#define EXAMPLE_ESP_WIFI_SSID "YourWiFiSSID"
#define EXAMPLE_ESP_WIFI_PASS "YourWiFiPassword"
```

Or use NVS (recommended):

```bash
# Burn WiFi credentials to NVS
idf.py menuconfig
# Navigate to: Component config -> Example Configuration
# Set WiFi SSID and password
```

### Step 3: Build

```bash
# Build the project
idf.py build

# Expected output:
# Project build complete. To flash, run this command:
# idf.py -p (PORT) flash
```

### Step 4: Flash

```bash
# Find USB port
idf.py -p list

# Flash to device
idf.py -p /dev/ttyUSB0 flash

# Monitor serial output
idf.py -p /dev/ttyUSB0 monitor

# Or do both in one command
idf.py -p /dev/ttyUSB0 flash monitor
```

### Step 5: Verify

```bash
# Check device IP address (from serial monitor)
# Expected: I (12345) q-lite: Q-Lite ready, IP: 192.168.1.100

# Test HTTP endpoint
curl http://192.168.1.100/
# Expected: {"status":"ok","message":"Q-Lite HTTP Server"}
```

---

## 🔧 Build Issues

### Issue #5: Build Fails on ESP-IDF v5.5.3

**Symptoms**:
```
FAILED: q-lite-esp32.elf
ld: cannot find -lxtensa
```

**Solution**:

1. **Clean and Rebuild**
   ```bash
   idf.py fullclean
   rm -rf build/
   idf.py set-target esp32s3
   idf.py build
   ```

2. **Verify Target**
   ```bash
   idf.py show-target
   # Expected: esp32s3
   ```

3. **Check SDKConfig**
   ```bash
   grep CONFIG_IDF_TARGET sdkconfig
   # Expected: CONFIG_IDF_TARGET_ESP32S3=y
   ```

4. **Use sdkconfig.defaults**
   - The `sdkconfig.defaults` file in this directory sets the correct target
   - Delete `sdkconfig` if it exists, then rebuild
   ```bash
   rm sdkconfig
   idf.py build
   ```

---

## 📊 Memory Usage

| Component | RAM Usage | Flash Usage |
|-----------|-----------|-------------|
| Q-Lite Core | ~50KB | ~80KB |
| ESP-IDF Stack | ~100KB | ~200KB |
| WiFi Stack | ~150KB | ~150KB |
| HTTP Server | ~20KB | ~30KB |
| **Total** | **~320KB** | **~460KB** |

**Note**: ESP32-S3 has 520KB RAM, leaving ~200KB free for application data.

---

## 🔌 API Endpoints

### Health Check
```bash
curl http://DEVICE_IP/
# {"status":"ok","message":"Q-Lite HTTP Server"}
```

### List Models
```bash
curl http://DEVICE_IP/models
# {"models":["llama2-7b","mistral-7b"]}
```

### Generate Text
```bash
curl -X POST http://DEVICE_IP/api/generate \
  -H "Content-Type: application/json" \
  -d '{"model":"llama2-7b","prompt":"Hello, world!","max_tokens":50}'
```

---

## 🐛 Debugging

### Serial Monitor
```bash
idf.py monitor
# Use Ctrl+] to exit
```

### GDB Debugging
```bash
idf.py gdb
```

### Core Dump Analysis
```bash
idf.py coredump-info
```

---

## 📚 Further Reading

- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/)
- [ESP32-S3 Technical Reference](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- [Q-Lite Main README](../../../README.md)

---

## 🤝 Contributing

When submitting bug reports or improvements, please include:

1. ESP-IDF version (`idf.py --version`)
2. Target chip (`idf.py show-target`)
3. SDKConfig dump (`idf.py menuconfig` → Save as text)
4. Full error log (`idf.py build 2>&1 | tee build.log`)

---

## 📄 License

MIT License - See [LICENSE](../../../LICENSE) for details.

---

**Made with Q-Lite** 🌟
