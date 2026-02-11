#ifndef OLLAMA_H
#define OLLAMA_H

#include <stddef.h>

// Ollama API 配置
#define OLLAMA_DEFAULT_HOST "localhost"
#define OLLAMA_DEFAULT_PORT 11434
#define OLLAMA_MAX_RESPONSE 8192

// API 端点
#define OLLAMA_API_GENERATE "/api/generate"
#define OLLAMA_API_CHAT     "/api/chat"

// 生成响应
typedef struct {
    char model[64];
    char response[OLLAMA_MAX_RESPONSE];
    int done;
} OllamaResponse;

// HTTP 客户端 (内部)
int http_post(const char *host, int port,
              const char *path, const char *body,
              char *response, size_t response_size);

// Ollama API 函数
char* ollama_generate(const char *model, const char *prompt);
char* ollama_chat(const char *model, const char *message);

// 流式生成 (Task 2: Streaming)
int ollama_generate_stream(const char *model, const char *prompt, int client_fd);

// 辅助函数
int ollama_parse_response(const char *json, OllamaResponse *resp);

#endif
