# Q-Lite API Documentation

**REST API for Q-Lite HTTP Gateway**

---

## 📡 Base URL

```
http://localhost:8080
```

Default port: 8080 (configurable with `--port`)

---

## 🔌 Endpoints

### 1. GET /

**Purpose**: Server status and available endpoints

**Request**:
```bash
curl http://localhost:8080/
```

**Response** (200 OK):
```json
{
  "status": "ok",
  "message": "Q-Lite v0.1.0-alpha",
  "endpoints": {
    "GET /",
    "POST /api/generate",
    "POST /api/chat"
  }
}
```

---

### 2. POST /api/generate

**Purpose**: Generate text completion (Ollama `/api/generate`)

**Request**:
```bash
curl -X POST http://localhost:8080/api/generate \
  -H "Content-Type: application/json" \
  -d '{
    "model": "qwen2.5:7b",
    "prompt": "Hello!"
  }'
```

**Request Body**:
```json
{
  "model": "string",  // Required: Model name (e.g., "qwen2.5:7b")
  "prompt": "string"  // Required: Input prompt
}
```

**Response** (200 OK):
```json
{
  "response": "Hello! How can I help you today?"
}
```

**Error Response** (400 Bad Request):
```json
{
  "error": "Missing model or prompt field"
}
```

**Error Response** (500 Internal Server Error):
```json
{
  "error": "Failed to connect to Ollama"
}
```

---

### 3. POST /api/chat

**Purpose**: Chat completion (Ollama `/api/chat`)

**Request**:
```bash
curl -X POST http://localhost:8080/api/chat \
  -H "Content-Type: application/json" \
  -d '{
    "model": "qwen2.5:7b",
    "message": "What is Q-Lite?"
  }'
```

**Request Body**:
```json
{
  "model": "string",   // Required: Model name
  "message": "string"  // Required: User message
}
```

**Response** (200 OK):
```json
{
  "response": "Q-Lite is an ultra-lightweight LLM gateway..."
}
```

**Error Response** (400 Bad Request):
```json
{
  "error": "Missing model or message field"
}
```

**Error Response** (500 Internal Server Error):
```json
{
  "error": "Failed to connect to Ollama"
}
```

---

## 📝 Common Use Cases

### Text Generation

```bash
# Generate story
curl -X POST http://localhost:8080/api/generate \
  -H "Content-Type: application/json" \
  -d '{
    "model": "qwen2.5:7b",
    "prompt": "Once upon a time..."
  }'
```

### Q&A

```bash
# Ask question
curl -X POST http://localhost:8080/api/chat \
  -H "Content-Type: application/json" \
  -d '{
    "model": "qwen2.5:7b",
    "message": "What is the capital of France?"
  }'
```

### Code Generation

```bash
# Generate Python code
curl -X POST http://localhost:8080/api/generate \
  -H "Content-Type: application/json" \
  -d '{
    "model": "qwen2.5:7b",
    "prompt": "Write a Python function to sort a list"
  }'
```

---

## ⚙️ Configuration

### Command Line Options

```bash
./q-lite --port 8080 --ollama http://localhost:11434
```

| Option | Default | Description |
|--------|---------|-------------|
| `--port` | 8080 | HTTP server port |
| `--ollama` | http://localhost:11434 | Ollama server URL |
| `--help` | - | Show help message |

### Ollama Integration

Q-Lite automatically forwards requests to Ollama:

```
Client → Q-Lite (port 8080) → Ollama (port 11434)
```

---

## 🔒 Security Notes

### Current Status (v0.1.0-alpha)

**No Authentication**:
- Q-Lite does NOT implement authentication
- All endpoints are publicly accessible
- **Do NOT expose to the internet**

**Recommendations**:
1. Run on localhost only (127.0.0.1)
2. Use reverse proxy (nginx, Apache) for authentication
3. Use firewall rules to restrict access

### Example: nginx Reverse Proxy

```nginx
server {
    listen 80;
    server_name your-domain.com;

    location / {
        auth_basic "Restricted";
        auth_basic_user_file /etc/nginx/.htpasswd;

        proxy_pass http://localhost:8080;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }
}
```

---

## 🚀 Performance

### Benchmarks

| Metric | Value |
|--------|-------|
| Binary Size | 51KB |
| RAM Usage | <1MB (estimated) |
| HTTP Overhead | <1ms |
| Throughput | ~100 req/s (single core) |

### Load Testing

```bash
# Apache Bench
ab -n 1000 -c 10 http://localhost:8080/

# Wrk
wrk -t4 -c100 -d30s http://localhost:8080/
```

---

## 🐛 Error Codes

| HTTP Status | Meaning |
|-------------|---------|
| 200 | Success |
| 400 | Bad Request (missing fields) |
| 405 | Method Not Allowed |
| 500 | Internal Server Error (Ollama connection failed) |

---

## 📚 Examples

### Python

```python
import requests

# Generate text
response = requests.post(
    "http://localhost:8080/api/generate",
    json={
        "model": "qwen2.5:7b",
        "prompt": "Hello!"
    }
)
print(response.json()["response"])
```

### JavaScript (Node.js)

```javascript
const fetch = require('node-fetch');

fetch('http://localhost:8080/api/chat', {
  method: 'POST',
  headers: { 'Content-Type': 'application/json' },
  body: JSON.stringify({
    model: 'qwen2.5:7b',
    message: 'Hello!'
  })
})
.then(res => res.json())
.then(data => console.log(data.response));
```

### Bash

```bash
# Simple script
#!/bin/bash
MODEL="qwen2.5:7b"
PROMPT="Write a haiku about AI"

curl -s -X POST http://localhost:8080/api/generate \
  -H "Content-Type: application/json" \
  -d "{\"model\":\"$MODEL\",\"prompt\":\"$PROMPT\"}" \
  | jq -r '.response'
```

---

## 🔄 Version History

### v0.1.0-alpha (Current)
- ✅ GET / (status)
- ✅ POST /api/generate
- ✅ POST /api/chat
- ❌ No authentication
- ❌ No streaming

### v0.2.0 (Planned)
- [ ] Streaming support
- [ ] Better error handling
- [ ] Request validation

---

**Need Help?** [Open an Issue](https://github.com/RalphBigBear/q-lite/issues)
