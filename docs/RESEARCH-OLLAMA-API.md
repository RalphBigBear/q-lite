# Q-Lite Ollama API Research

**Date**: 2026-02-11
**Source**: Ollama Official Documentation

---

## 🔌 Ollama API Endpoints

### POST /api/generate

**Request**:
```json
{
  "model": "qwen2.5:7b",
  "prompt": "Hello!",
  "stream": false
}
```

**Response**:
```json
{
  "model": "qwen2.5:7b",
  "response": "Hello! How can I help you today?",
  "done": true
}
```

### POST /api/chat

**Request**:
```json
{
  "model": "qwen2.5:7b",
  "messages": [
    {
      "role": "user",
      "content": "Hello!"
    }
  ],
  "stream": false
}
```

**Response**:
```json
{
  "model": "qwen2.5:7b",
  "message": {
    "role": "assistant",
    "content": "Hello! How can I help you today?"
  },
  "done": true
}
```

### GET /api/tags

**Response**:
```json
{
  "models": [
    {
      "name": "qwen2.5:7b",
      "modified_at": "2025-01-15T00:00:00Z",
      "size": 4500000000
    }
  ]
}
```

---

## 🎯 Q-Lite 需要实现的端点

### MVP (v0.2.0)
- ✅ POST /api/generate - 简单文本生成
- ✅ POST /api/chat - 对话模式
- ❌ Stream 支持 (v0.3.0)

### 后续版本
- GET /api/tags - 列出可用模型
- POST /api/embeddings - 向量嵌入

---

## 🔧 HTTP 客户端选项

### 选项 1: libcurl
**优点**:
- 成熟稳定
- 支持 HTTPS
- 易于使用

**缺点**:
- ❌ 外部依赖 (违反 Q-Lite 零依赖目标)
- ❌ 增加二进制大小

### 选项 2: Raw Socket
**优点**:
- ✅ 零依赖
- ✅ 完全控制
- ✅ 极小二进制

**缺点**:
- 需要手动实现 HTTP 协议
- 需要手动处理 TLS (如需 HTTPS)

### 选项 3: 简化 libcurl (动态链接)
**优点**:
- 系统已有 libcurl
- 不增加二进制大小

**缺点**:
- 依赖系统库
- 不够"纯粹"

---

## 💡 决策: Raw Socket (Q-Lite 原则)

**Q-Lite 核心原则**: 零依赖、极低熵

**选择**: Raw Socket + 手动 HTTP
- ✅ 零外部依赖
- ✅ 完全控制
- ✅ 极简实现 (<200 LOC)
- ✅ Ollama 通常在本地 (localhost:11434)，无需 HTTPS

---

## 📝 实现计划

### Phase 1: HTTP Client 基础
```c
// 发送 HTTP POST 请求
int http_post(const char *host, int port,
              const char *path, const char *body,
              char *response, size_t response_size);
```

### Phase 2: JSON 解析
```c
// 使用 cJSON (嵌入式库)
cJSON *json = cJSON_Parse(body);
const char *model = cJSON_GetObjectItem(json, "model")->valuestring;
```

### Phase 3: Ollama 集成
```c
// ollama.c
char* ollama_generate(const char *model, const char *prompt);
char* ollama_chat(const char *model, const char *message);
```

---

## 🎨 低熵实现参考

### HTTP POST 请求格式
```http
POST /api/generate HTTP/1.1
Host: localhost:11434
Content-Type: application/json
Content-Length: 45

{"model":"qwen2.5:7b","prompt":"Hello!"}
```

### Raw Socket 发送
```c
int sock = socket(AF_INET, SOCK_STREAM, 0);
connect(sock, &address, sizeof(address));
send(sock, request, strlen(request), 0);
recv(sock, response, response_size, 0);
close(sock);
```

---

## ✅ Research 总结

**API 端点**: /api/generate, /api/chat
**HTTP 方法**: POST + JSON body
**实现方式**: Raw Socket (零依赖)
**JSON 库**: cJSON (嵌入式)

**下一步**: DESIGN - 详细设计 ollama.c

---

**Inspired by**: Ollama API Documentation
**Date**: 2026-02-11 15:07 GMT+8
