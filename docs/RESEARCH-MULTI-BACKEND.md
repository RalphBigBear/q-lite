# Q-Lite Multi-Backend Support Research

**Date**: 2026-02-11
**Focus**: 支持 Ollama, vLLM, LM Studio

---

## 🎯 支持的后端

### 1. Ollama (当前)
**API**: HTTP/JSON
**端口**: 11434
**状态**: ✅ 已实现

### 2. vLLM
**API**: OpenAI-compatible HTTP/JSON
**端口**: 8000
**特性**:
- OpenAI API 兼容
- 高性能推理
- Batch 请求

### 3. LM Studio
**API**: OpenAI-compatible HTTP/JSON
**端口**: 1234
**特性**:
- 本地 GUI 服务器
- OpenAI 兼容
- 易于使用

### 4. OpenAI (可选)
**API**: HTTPS OpenAI API
**用途**: 云端推理
**成本**: 需要 API key

---

## 🔧 架构设计

### Backend Abstraction

```c
// Backend interface
typedef struct {
    char name[32];
    char base_url[256];
    int port;
    int api_type;  // 0=ollama, 1=openai-compatible
} Backend;

// Backend operations
typedef struct {
    char* (*generate)(Backend *backend, const char *model, const char *prompt);
    char* (*chat)(Backend *backend, const char *model, const char *message);
} BackendOps;
```

### Configuration

```c
// q-lite.conf (或命令行)
Backend ollama = {
    .name = "ollama",
    .base_url = "http://localhost",
    .port = 11434,
    .api_type = 0
};

Backend vllm = {
    .name = "vllm",
    .base_url = "http://localhost",
    .port = 8000,
    .api_type = 1  // OpenAI-compatible
};
```

---

## 📝 API 差异

### Ollama API

**Generate**:
```json
POST /api/generate
{
  "model": "qwen2.5:7b",
  "prompt": "Hello!",
  "stream": false
}
```

**Chat**:
```json
POST /api/chat
{
  "model": "qwen2.5:7b",
  "messages": [
    {"role": "user", "content": "Hello!"}
  ]
}
```

### OpenAI-Compatible API (vLLM, LM Studio)

**Chat Completion**:
```json
POST /v1/chat/completions
{
  "model": "qwen2.5:7b",
  "messages": [
    {"role": "user", "content": "Hello!"}
  ]
}
```

**Completion**:
```json
POST /v1/completions
{
  "model": "qwen2.5:7b",
  "prompt": "Hello!"
}
```

---

## 🎨 Implementation Plan

### Step 1: Backend Abstraction Layer
- [ ] 定义 `Backend` 结构体
- [ ] 定义 `BackendOps` 接口
- [ ] 实现 Ollama backend
- [ ] 实现 OpenAI-compatible backend

### Step 2: Configuration
- [ ] 命令行选项 `--backend`
- [ ] 配置文件支持 (可选)
- [ ] 自动检测后端

### Step 3: Testing
- [ ] 测试 Ollama
- [ ] 测试 vLLM (如果可用)
- [ ] 测试 LM Studio (如果可用)

---

## 💡 Design Decisions

### 1. API 转换

**问题**: Ollama API vs OpenAI API 不同

**解决方案**:
```c
// Ollama → OpenAI 格式转换
char* ollama_to_openai(const char *ollama_json) {
    // Parse Ollama response
    // Convert to OpenAI format
    return openai_json;
}
```

### 2. 统一接口

**Q-Lite API**:
- `POST /api/generate` → Ollama 格式
- `POST /v1/chat/completions` → OpenAI 格式
- **自动检测后端，转换请求**

### 3. Backend 选择

**优先级**:
1. 命令行 `--backend`
2. 环境变量 `Q_LITE_BACKEND`
3. 自动检测 (端口扫描)

---

## 📊 代码结构

```c
// backend.h
typedef struct Backend Backend;

// ollama_backend.c
Backend* ollama_backend_create(const char *host, int port);

// openai_backend.c
Backend* openai_backend_create(const char *host, int port);

// main.c
Backend *backend = detect_backend();
backend->ops->generate(backend, model, prompt);
```

---

## ✅ Expected Outcome

**新增功能**:
- ✅ 支持 vLLM
- ✅ 支持 LM Studio
- ✅ 统一 API 接口
- ✅ 自动后端检测

**二进制大小**:
- 预计 +3-5KB (后端抽象)

**向后兼容**:
- ✅ 默认仍为 Ollama
- ✅ API 不变

---

## 🚧 Limitations

1. **vLLM/LM Studio 未安装**
   - 优雅降级到 Ollama
   - 错误提示

2. **API 差异**
   - 某些高级功能可能不支持
   - 基础功能兼容

3. **测试覆盖**
   - 需要实际安装 vLLM/LM Studio
   - 可能无法完全测试

---

**Next Step**: IMPLEMENT - Backend abstraction layer
