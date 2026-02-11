#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "ollama.h"

// Task 3: Request Queue (extern from main.c)
extern volatile int active_requests;
#define MAX_CONCURRENT_REQUESTS 10

// External Ollama client
extern char* ollama_generate(const char *model, const char *prompt);
extern char* ollama_chat(const char *model, const char *message);
extern int ollama_generate_stream(const char *model, const char *prompt, int client_fd);

// 创建 HTTP 响应
static void create_response(HttpContext *ctx, int status_code, const char *content_type, const char *body) {
    char header[512];
    int header_len = snprintf(header, sizeof(header),
        "HTTP/1.1 %d OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        status_code, content_type, strlen(body)
    );

    memcpy(ctx->response, header, header_len);
    memcpy(ctx->response + header_len, body, strlen(body));
    ctx->response_len = header_len + strlen(body);
}

// 简化 JSON 解析（手动提取字段）
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

// FSM: 等待连接
void http_handle_idle(HttpContext *ctx) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    // 非阻塞 accept
    ctx->client_fd = accept(ctx->server_fd, (struct sockaddr*)&client_addr, &client_len);

    if (ctx->client_fd >= 0) {
        // 新连接
        ctx->state = HTTP_STATE_READING;
        ctx->request_len = 0;
        printf("[HTTP] New connection from %s\n", inet_ntoa(client_addr.sin_addr));
    } else {
        // 无连接，保持 IDLE
        ctx->state = HTTP_STATE_IDLE;
    }
}

// FSM: 读取请求
void http_handle_reading(HttpContext *ctx) {
    ssize_t bytes_read = read(ctx->client_fd, ctx->request + ctx->request_len,
                               HTTP_MAX_REQUEST - ctx->request_len - 1);

    if (bytes_read <= 0) {
        // 连接关闭或错误
        ctx->state = HTTP_STATE_CLOSING;
        return;
    }

    ctx->request_len += bytes_read;
    ctx->request[ctx->request_len] = '\0';

    // 检查是否读取完整请求（查找 \r\n\r\n）
    if (strstr(ctx->request, "\r\n\r\n") != NULL) {
        ctx->state = HTTP_STATE_PROCESSING;
    }
}

// FSM: 处理请求
void http_handle_processing(HttpContext *ctx) {
    // Task 3: 检查并发队列
    if (active_requests >= MAX_CONCURRENT_REQUESTS) {
        http_send_error(ctx->client_fd, 503, "Service Unavailable (too many requests)");
        ctx->state = HTTP_STATE_CLOSING;
        return;
    }

    // 增加请求计数
    __sync_fetch_and_add(&active_requests, 1);

    // 解析 HTTP 方法
    if (strncmp(ctx->request, "GET ", 4) == 0) {
        // 简单的 GET 响应
        const char *body = "{\"status\":\"ok\",\"message\":\"Q-Lite v0.1.0-alpha\",\"endpoints\":{\"GET /\",\"POST /api/generate\",\"POST /api/chat\"}";
        create_response(ctx, 200, "application/json", body);
    } else if (strncmp(ctx->request, "POST ", 5) == 0) {
        // 查找 JSON body（在 \r\n\r\n 之后）
        char *json_start = strstr(ctx->request, "\r\n\r\n");
        if (json_start != NULL) {
            json_start += 4;  // 跳过 \r\n\r\n

            // 解析请求: 提取 model 和 prompt/message
            char *model = extract_json_field(json_start, "model");
            char *prompt_or_message = extract_json_field(json_start,
                (strstr(json_start, "\"prompt\":") != NULL) ? "prompt" : "message");

            if (model && prompt_or_message) {
                // 调用 Ollama
                char *ollama_response;
                if (strstr(json_start, "\"prompt\":") != NULL) {
                    // /api/generate
                    ollama_response = ollama_generate(model, prompt_or_message);
                } else {
                    // /api/chat
                    ollama_response = ollama_chat(model, prompt_or_message);
                }

                // 创建 HTTP 响应
                create_response(ctx, 200, "application/json", ollama_response);

                // 清理
                free(model);
                free(prompt_or_message);
                free(ollama_response);
            } else {
                const char *body = "{\"error\":\"Missing model or prompt/message field\"}";
                create_response(ctx, 400, "application/json", body);
            }
        } else {
            const char *body = "{\"error\":\"No JSON body\"}";
            create_response(ctx, 400, "application/json", body);
        }
    } else {
        const char *body = "{\"error\":\"Method not allowed\"}";
        create_response(ctx, 405, "application/json", body);
    }

    // 减少请求计数
    __sync_fetch_and_sub(&active_requests, 1);

    ctx->state = HTTP_STATE_RESPONDING;
}

// FSM: 发送响应
void http_handle_responding(HttpContext *ctx) {
    ssize_t bytes_sent = write(ctx->client_fd, ctx->response, ctx->response_len);

    if (bytes_sent < 0) {
        // 发送错误
        ctx->state = HTTP_STATE_CLOSING;
    } else if (bytes_sent == ctx->response_len) {
        // 发送完成
        ctx->state = HTTP_STATE_CLOSING;
    } else {
        // 部分发送，继续
        // TODO: 处理部分发送
        ctx->state = HTTP_STATE_CLOSING;
    }
}

// FSM: 关闭连接
void http_handle_closing(HttpContext *ctx) {
    close(ctx->client_fd);
    ctx->client_fd = -1;
    ctx->state = HTTP_STATE_IDLE;
    printf("[HTTP] Connection closed\n");
}

// 主事件循环
void http_server_run(HttpContext *ctx) {
    while (1) {
        switch (ctx->state) {
            case HTTP_STATE_IDLE:
                http_handle_idle(ctx);
                break;
            case HTTP_STATE_READING:
                http_handle_reading(ctx);
                break;
            case HTTP_STATE_PROCESSING:
                http_handle_processing(ctx);
                break;
            case HTTP_STATE_RESPONDING:
                http_handle_responding(ctx);
                break;
            case HTTP_STATE_CLOSING:
                http_handle_closing(ctx);
                break;
        }

        // 避免忙等待
        usleep(1000);  // 1ms
    }
}

// 启动 HTTP 服务器
int http_server_start(int port) {
    int server_fd;
    struct sockaddr_in address;

    // 创建 socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        return -1;
    }

    // 设置 socket 选项
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 绑定地址
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        return -1;
    }

    // 监听
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        return -1;
    }

    printf("[HTTP] Server started on port %d\n", port);
    return server_fd;
}

// 发送 chunked 响应头
int http_send_chunked_header(int client_fd, const char *content_type) {
    char header[256];
    int len = snprintf(header, sizeof(header),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Transfer-Encoding: chunked\r\n"
        "Connection: close\r\n"
        "\r\n",
        content_type
    );

    return send(client_fd, header, len, 0);
}

// 发送一个数据块
int http_send_chunk(int client_fd, const char *data, size_t len) {
    char chunk_header[32];

    // 格式: {size_hex}\r\n
    snprintf(chunk_header, sizeof(chunk_header), "%zx\r\n", len);

    // 发送 chunk header
    if (send(client_fd, chunk_header, strlen(chunk_header), 0) < 0) {
        return -1;
    }

    // 发送 chunk data
    if (len > 0 && send(client_fd, data, len, 0) < 0) {
        return -1;
    }

    // 发送 \r\n
    if (send(client_fd, "\r\n", 2, 0) < 0) {
        return -1;
    }

    return 0;
}

// 结束 chunked 响应
int http_send_chunk_end(int client_fd) {
    // 发送终止 chunk: 0\r\n\r\n
    return send(client_fd, "0\r\n\r\n", 5, 0);
}

// 发送错误响应
int http_send_error(int client_fd, int code, const char *message) {
    char header[512];
    char body[256];

    snprintf(body, sizeof(body), "{\"error\":%d,\"message\":\"%s\"}", code, message);

    int header_len = snprintf(header, sizeof(header),
        "HTTP/1.1 %d %s\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n",
        code, (code == 503) ? "Service Unavailable" : "Error", strlen(body)
    );

    send(client_fd, header, header_len, 0);
    send(client_fd, body, strlen(body), 0);

    return 0;
}
