# Building Q-Lite

**编译 Q-Lite HTTP Gateway**

---

## 📋 Prerequisites

### macOS
```bash
# Xcode Command Line Tools (required)
xcode-select --install

# Verify gcc is installed
gcc --version
```

### Linux
```bash
# Debian/Ubuntu
sudo apt-get install build-essential

# Fedora/RHEL
sudo dnf install gcc make

# Arch Linux
sudo pacman -S base-devel
```

---

## 🚀 Quick Build

```bash
# Clone repository
git clone https://github.com/RalphBigBear/q-lite.git
cd q-lite

# Build
make

# Run
./q-lite --port 8080
```

---

## 📊 Build Options

### Standard Build
```bash
make
```
Output: `q-lite` binary (~51KB)

### Clean Build
```bash
make clean
make
```

### Run with Default Settings
```bash
make run
```

---

## 🔧 Build Configuration

### Compiler Flags (Makefile)
```makefile
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
```

### Optimization Levels
- **O0**: No optimization (debug)
- **O1**: Basic optimization
- **O2**: Recommended (default)
- **O3**: Maximum optimization (larger binary)

To change optimization level, edit `Makefile`:
```makefile
CFLAGS = -Wall -Wextra -O3 -std=c99
```

---

## 🐛 Debug Build

For debugging with GDB:

```bash
# Build with debug symbols
gcc -Wall -Wextra -g -std=c99 -c src/main.c -o src/main.o
gcc -Wall -Wextra -g -std=c99 -c src/http.c -o src/http.o
gcc -Wall -Wextra -g -std=c99 -c src/ollama.c -o src/ollama.o
gcc src/main.o src/http.o src/ollama.o -o q-lite

# Debug with GDB
gdb ./q-lite
```

---

## 📦 Binary Size

### Current Size
```
$ ls -lh q-lite
-rwxr-xr-x  1 ralph  staff   51K Feb 11 15:12 q-lite
```

### Size Breakdown
- **Target**: <100KB
- **Current**: 51KB
- **Margin**: 49KB (49% under target)

### Strip Binary (Optional)
```bash
# Remove debug symbols (reduces size by ~10-20%)
strip q-lite
```

---

## 🧪 Testing

### Automated Test
```bash
make test
```

### Manual Test

**Start Server**:
```bash
./q-lite --port 8080
```

**Test GET**:
```bash
curl http://localhost:8080/
```

Expected output:
```json
{"status":"ok","message":"Q-Lite v0.1.0-alpha","endpoints":{"GET /","POST /api/generate","POST /api/chat"}}
```

**Test POST (需要 Ollama 运行)**:
```bash
# Start Ollama first
ollama serve

# Test generate
curl -X POST http://localhost:8080/api/generate \
  -H "Content-Type: application/json" \
  -d '{"model":"qwen2.5:7b","prompt":"Hello!"}'
```

---

## 🔍 Troubleshooting

### Build Error: "gcc not found"
```bash
# macOS
xcode-select --install

# Linux
sudo apt-get install build-essential
```

### Build Error: "undefined reference to `inet_ntoa'"
This is already fixed in the code (includes `<arpa/inet.h>`)

### Runtime Error: "bind failed: Address already in use"
```bash
# Find process using port 8080
lsof -i :8080

# Kill process
kill -9 <PID>

# Or use different port
./q-lite --port 9090
```

### Runtime Error: "connect failed: Address family not supported"
This means Ollama is not running. Start it first:
```bash
ollama serve
```

---

## 📱 Cross-Compilation

### ARM64 (Apple Silicon)
```bash
# Native compilation (no special flags needed)
make
```

### x86_64 (Intel)
```bash
# On Apple Silicon Mac
gcc -arch x86_64 -Wall -Wextra -O2 -std=c99 src/*.c -o q-lite-x64
```

### ARM (Raspberry Pi)
```bash
# On Linux x86_64
arm-linux-gnueabihf-gcc -Wall -Wextra -O2 -std=c99 src/*.c -o q-lite-arm
```

---

## 🚀 Performance Tips

### 1. Enable O3 Optimization
```makefile
CFLAGS = -Wall -Wextra -O3 -std=c99
```
Trade-off: Larger binary, faster execution

### 2. Static Linking (Not Recommended)
```bash
gcc -static src/*.c -o q-lite
```
Trade-off: Much larger binary (~500KB+)

### 3. Use `-march=native`
```makefile
CFLAGS = -Wall -Wextra -O3 -march=native -std=c99
```
Trade-off: Binary only works on same CPU architecture

---

## 📝 Next Steps

After building:
1. **Test**: Run `make test`
2. **Deploy**: Copy `q-lite` to target device
3. **Integrate**: Connect to Ollama server
4. **Monitor**: Check logs for errors

---

**Need Help?** [Open an Issue](https://github.com/RalphBigBear/q-lite/issues)
