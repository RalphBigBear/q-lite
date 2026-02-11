# Q-Lite Streaming & Queue Research

**Date**: 2026-02-11
**Focus**: HTTP Chunked Encoding + Request Queue

---

## 🎯 Task 2: Streaming Support

### 研究发现

**❌ WebSocket (太复杂)**:
- RFC 6455 握手协议
- 需要 SHA-1, Base64 编码
- 多线程处理
- 代码量: ~2000 LOC
- **熵值**: 高

**✅ HTTP Chunked Encoding (低熵)**:
- RFC 9112 标准
- 零额外依赖
- 单线程处理
- 代码量: ~100 LOC
- **熵值**: 极低 ✅

### Chunked Encoding 格式

```
HTTP/1.1 200 OK
Content-Type: application/json
Transfer-Encoding: chunked

5\r\n
hello\r\n
6\r\n
 world\r\n
0\r\n
\r\n
```

**规则**:
- `{size_hex}\r\n{data}\r\n`
- `0\r\n\r\n` 表示结束
- 无需知道总长度

### Ollama 流式响应

**请求**:
```json
POST /api/generate
{
  "model": "qwen2.5:7b",
  "prompt": "Hello!",
  "stream": true
}
```

**响应** (多行 JSON):
```json
{"model":"qwen2.5:7b","response":"Hello","done":false}
{"model":"qwen2.5:7b","response":" there","done":false}
{"model":"qwen2.5:7b","response":"!","done":true}
```

### 实现方案

```c
// 流式响应函数
int http_send_chunk(int client_fd, const char *data, size_t len) {
    char chunk_header[32];
    snprintf(chunk_header, sizeof(chunk_header), "%zx\r\n", len);

    // 发送 chunk header
    send(client_fd, chunk_header, strlen(chunk_header), 0);

    // 发送 chunk data
    send(client_fd, data, len, 0);

    // 发送 \r\n
    send(client_fd, "\r\n", 2, 0);

    return 0;
}

// 结束 chunked 响应
int http_send_chunk_end(int client_fd) {
    return send(client_fd, "0\r\n\r\n", 5, 0);
}
```

---

## 🎯 Task 3: Request Queue

### 研究发现

**❌ 复杂队列系统**:
- 多队列优先级
- 持久化
- 代码量: ~500 LOC
- **熵值**: 高

**✅ 简单 FIFO 队列**:
- 最大并发: 10
- 拒绝策略: 503 Service Unavailable
- 代码量: ~50 LOC
- **熵值**: 极低 ✅

### 实现方案

```c
// 全局请求计数器
static volatile int active_requests = 0;
#define MAX_CONCURRENT_REQUESTS 10

// 请求处理
int handle_request(int client_fd) {
    // 检查队列
    if (active_requests >= MAX_CONCURRENT_REQUESTS) {
        http_send_error(client_fd, 503, "Service Unavailable");
        return -1;
    }

    // 增加计数
    __sync_fetch_and_add(&active_requests, 1);

    // 处理请求
    process_request(client_fd);

    // 减少计数
    __sync_fetch_and_sub(&active_requests, 1);

    return 0;
}
```

---

## 📊 熵值对比

| 方案 | 代码行数 | 依赖 | 复杂度 | 熵值 |
|------|---------|------|--------|------|
| WebSocket | ~2000 | 多 | 高 | 高 |
| Chunked Encoding | ~100 | 0 | 低 | **极低** ✅ |
| 复杂队列 | ~500 | 多 | 高 | 高 |
| 简单 FIFO | ~50 | 0 | 低 | **极低** ✅ |

---

## ✅ Expected Outcome

**Task 2 - Streaming**:
- ✅ HTTP Chunked Encoding 支持
- ✅ 流式转发 Ollama 响应
- ✅ 零额外依赖
- 预计代码: ~100 LOC

**Task 3 - Queue**:
- ✅ 最大 10 并发请求
- ✅ 超出返回 503
- ✅ 线程安全计数器
- 预计代码: ~50 LOC

**Binary 大小**:
- 预计: +2-3KB (从 52KB → ~55KB)
- 目标: <100KB ✅

---

**Next Step**: IMPLEMENT
