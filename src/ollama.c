#include "ollama.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

// HTTP POST 客户端 (Raw Socket - 零依赖)
int http_post(const char *host, int port,
              const char *path, const char *body,
              char *response, size_t response_size) {
    int sock;
    struct sockaddr_in server;
    char request[OLLAMA_MAX_RESPONSE * 2];
    char header[512];

    // 创建 socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket failed");
        return -1;
    }

    // 连接服务器
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(host);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("connect failed");
        close(sock);
        return -1;
    }

    // 构建 HTTP 请求
    int header_len = snprintf(header, sizeof(header),
        "POST %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        path, host, strlen(body)
    );

    memcpy(request, header, header_len);
    memcpy(request + header_len, body, strlen(body));

    // 发送请求
    if (send(sock, request, header_len + strlen(body), 0) < 0) {
        perror("send failed");
        close(sock);
        return -1;
    }

    // 接收响应
    int total_received = 0;
    int bytes_received;
    while ((bytes_received = recv(sock, response + total_received,
                                   response_size - total_received - 1, 0)) > 0) {
        total_received += bytes_received;
    }

    response[total_received] = '\0';
    close(sock);

    // 跳过 HTTP header (查找 \r\n\r\n)
    char *body_start = strstr(response, "\r\n\r\n");
    if (body_start != NULL) {
        body_start += 4;  // 跳过 \r\n\r\n
        memmove(response, body_start, strlen(body_start) + 1);
    }

    return total_received;
}

// 生成 JSON body
static char* create_generate_json(const char *model, const char *prompt) {
    char *json = malloc(OLLAMA_MAX_RESPONSE);
    if (!json) return NULL;

    snprintf(json, OLLAMA_MAX_RESPONSE,
        "{\"model\":\"%s\",\"prompt\":\"%s\",\"stream\":false}",
        model, prompt
    );

    return json;
}

// 生成对话 JSON body
static char* create_chat_json(const char *model, const char *message) {
    char *json = malloc(OLLAMA_MAX_RESPONSE);
    if (!json) return NULL;

    snprintf(json, OLLAMA_MAX_RESPONSE,
        "{\"model\":\"%s\",\"messages\":[{\"role\":\"user\",\"content\":\"%s\"}],\"stream\":false}",
        model, message
    );

    return json;
}

// 提取 JSON 字段 (简单解析)
static char* extract_json_field(const char *json, const char *field) {
    char search[128];
    snprintf(search, sizeof(search), "\"%s\":\"", field);

    char *start = strstr(json, search);
    if (!start) return NULL;

    start += strlen(search);
    char *end = strchr(start, '"');
    if (!end) return NULL;

    *end = '\0';
    char *result = strdup(start);
    *end = '"';

    return result;
}

// Ollama Generate API
char* ollama_generate(const char *model, const char *prompt) {
    char *json_body = create_generate_json(model, prompt);
    if (!json_body) return NULL;

    char response[OLLAMA_MAX_RESPONSE];
    int result = http_post(OLLAMA_DEFAULT_HOST, OLLAMA_DEFAULT_PORT,
                          OLLAMA_API_GENERATE, json_body,
                          response, sizeof(response));

    free(json_body);

    if (result < 0) {
        return strdup("{\"error\":\"Failed to connect to Ollama\"}");
    }

    // 提取 response 字段
    char *reply = extract_json_field(response, "response");
    if (!reply) {
        return strdup("{\"error\":\"Invalid Ollama response\"}");
    }

    return reply;
}

// Ollama Chat API
char* ollama_chat(const char *model, const char *message) {
    char *json_body = create_chat_json(model, message);
    if (!json_body) return NULL;

    char response[OLLAMA_MAX_RESPONSE];
    int result = http_post(OLLAMA_DEFAULT_HOST, OLLAMA_DEFAULT_PORT,
                          OLLAMA_API_CHAT, json_body,
                          response, sizeof(response));

    free(json_body);

    if (result < 0) {
        return strdup("{\"error\":\"Failed to connect to Ollama\"}");
    }

    // 提取 message.content 字段 (简化处理)
    char *content_start = strstr(response, "\"content\":\"");
    if (!content_start) {
        return strdup("{\"error\":\"Invalid Ollama response\"}");
    }

    content_start += strlen("\"content\":\"");
    char *content_end = strchr(content_start, '"');
    if (!content_end) {
        return strdup("{\"error\":\"Invalid Ollama response\"}");
    }

    *content_end = '\0';
    char *reply = strdup(content_start);
    *content_end = '"';

    return reply;
}

// 解析完整响应 (保留)
int ollama_parse_response(const char *json, OllamaResponse *resp) {
    // TODO: 实现完整解析
    strncpy(resp->model, "unknown", sizeof(resp->model) - 1);
    strncpy(resp->response, json, sizeof(resp->response) - 1);
    resp->done = 1;
    return 0;
}
