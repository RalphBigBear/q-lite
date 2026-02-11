# Q-Lite HTTP Server Research Report

**Date**: 2026-02-11
**Phase**: RESEARCH (Q-Core v2.0)

---

## 🔍 参考项目分析

### 1. Hackaday micro HTTP Server (HTTaP)
**URL**: https://hackaday.io/project/20042-micro-http-server-in-c
**License**: AGPLv3
**Size**: ~50KB (source tarball)

**关键特性**:
- ✅ **单线程** - 符合 Q-Lite 需求
- ✅ **Blocking + Polled 模式** - 极其灵活
- ✅ **HTTP 1.1** - 持久连接
- ✅ **FSM 设计** - 清晰的状态机
- ✅ **嵌入式友好** - 设计用于 IoT
- ✅ **超时管理** - 避免挂起

**代码特点**:
```c
// 主循环示例
while (1) {
    HTTaP_server(timeout);  // FSM 实现
}
```

**FSM 状态**:
- ETAT2_attente_donnee_entrante (等待数据)
- 处理 GET/POST
- 文件服务
- 超时处理

**内存占用**:
- 极小（~50KB 包含所有功能）
- 适合嵌入式

---

### 2. libmicrohttpd
**URL**: https://www.gnu.org/software/libmicrohttpd/
**License**: LGPL
**性质**: GNU 官方库

**优点**:
- 成熟稳定
- API 简单
- HTTPS 支持

**缺点**:
- ❌ 依赖外部库（不符合 Q-Lite 零依赖目标）
- ❌ 二进制较大

---

### 3. StackOverflow: nweb (200 lines)
**特点**:
- 超级简洁（200 行 C）
- 教育性代码
- 基础 HTTP/1.0

**缺点**:
- 功能有限
- 无持久连接

---

## 📊 熵值对比

| 项目 | LOC | 依赖 | 内存 | 总熵 |
|------|-----|------|------|------|
| **HTTaP** | ~500 | 0 | <50KB | 🟢 极低 |
| **libmicrohttpd** | ~5000 | 1 | ~200KB | 🟡 中 |
| **nweb** | ~200 | 0 | <20KB | 🟢 极低 |
| **Q-Lite 目标** | <300 | 0 | <100KB | 🟢 极低 |

---

## 🎯 HTTaP 的核心设计模式

### 1. FSM (Finite State Machine)

```c
// 状态定义
enum State {
    STATE_IDLE,
    STATE_RECEIVING,
    STATE_PROCESSING,
    STATE_SENDING,
    STATE_CLOSING
};

// 主循环
void http_server_run(int timeout) {
    enum State state = STATE_IDLE;

    while (!abort_flag) {
        switch (state) {
            case STATE_IDLE:
                // 等待连接
                break;
            case STATE_RECEIVING:
                // 接收数据
                break;
            // ... 其他状态
        }
    }
}
```

### 2. Blocking vs Polled 模式

**Blocking 模式** (空闲时):
```c
HTTaP_mode_block();  // 阻塞等待请求，节省 CPU
```

**Polled 模式** (工作时):
```c
HTTaP_mode_poll();   // 非阻塞，允许其他任务运行
```

**这对 Q-Lite 很重要**:
- 可以同时运行 HTTP 服务器 + LLM 推理
- 单线程即可处理多个任务

### 3. 超时管理

```c
if (timeout_counter) {
    // 超时处理
    close(socket);
}
```

---

## 🏆 低熵启示

### HTTaP 的哲学

> "remain as compact as possible (so keep the feature set minimal)"
> 保持尽可能紧凑（功能集最小化）

**与 Q-Lite 一致**:
- ✅ 功能精简
- ✅ 单文件实现
- ✅ 零依赖
- ✅ 嵌入式优先

### HTTaP 的限制

**设计用于**:
- 单用户（单个连接）
- 局域网
- IoT 控制

**Q-Lite 需要**:
- 单用户（足够）
- 本地 Ollama（局域网）
- Edge AI（嵌入式）

**完美匹配！**

---

## 🎨 Q-Lite 设计决策

### 借鉴 HTTaP

**核心架构**:
- ✅ FSM 状态机
- ✅ 单线程事件循环
- ✅ Blocking/Polled 模式
- ✅ HTTP/1.1 持久连接

### 简化 HTTaP

**移除**:
- ❌ 文件服务（不需要）
- ❌ HTTaP 协议（太复杂）
- ❌ CORS（本地用不到）

**保留**:
- ✅ HTTP/1.1 GET/POST
- ✅ JSON 请求解析
- ✅ 超时管理

### 新增 Q-Lite 特性

**集成**:
- 🆕 Ollama API 转发
- 🆕 cJSON JSON 解析
- 🆕 错误处理

---

## 📝 实现路线图

### Phase 1: 核心 HTTP (src/http.c)

**基于 HTTaP 的 FSM**:
```c
// 3 个核心状态
enum {
    HTTP_STATE_IDLE,        // 等待连接
    HTTP_STATE_READING,     // 读取请求
    HTTP_STATE_RESPONDING   // 发送响应
};
```

**函数签名**:
```c
int http_server_start(int port);
int http_server_poll(int timeout_ms);  // 非阻塞轮询
```

### Phase 2: JSON 集成

**使用 cJSON**:
```c
// 解析请求
cJSON *req = cJSON_Parse(request_body);
const char *model = cJSON_GetObjectItem(req, "model")->valuestring;
const char *message = cJSON_GetObjectItem(req, "message")->valuestring;

// 调用 Ollama
char *response = ollama_generate(model, message);

// 发送响应
char *json_response = cJSON_Print(response);
write(client_fd, json_response, strlen(json_response));
```

---

## ✅ Research 总结

**找到的最佳参考**: HTTaP (Hackaday micro HTTP Server)

**原因**:
1. ✅ 单线程 FSM 设计（极低熵）
2. ✅ Blocking/Polled 双模式（灵活）
3. ✅ HTTP/1.1 持久连接（现代）
4. ✅ 嵌入式优先（与 Q-Lite 一致）
5. ✅ 源代码可读（教育性）

**Q-Lite 将**:
- 借鉴 FSM 架构
- 移除文件服务（不需要）
- 增加 Ollama 集成
- 保持 <100KB 内存目标

**下一步**: DESIGN 阶段 - 详细设计 HTTP.c

---

**Inspired by**: HTTaP (Yann Guidon / YGDES)
**Date**: 2026-02-11 14:54 GMT+8
