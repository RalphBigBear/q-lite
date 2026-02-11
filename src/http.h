#ifndef HTTP_H
#define HTTP_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define HTTP_MAX_REQUEST 4096
#define HTTP_MAX_RESPONSE 8192

typedef enum {
    HTTP_STATE_IDLE,        // 等待连接
    HTTP_STATE_READING,     // 读取请求
    HTTP_STATE_PROCESSING,  // 处理请求
    HTTP_STATE_RESPONDING,  // 发送响应
    HTTP_STATE_CLOSING      // 关闭连接
} HttpState;

typedef struct {
    int server_fd;          // 服务器 socket
    int client_fd;          // 客户端 socket
    HttpState state;        // 当前状态
    char request[HTTP_MAX_REQUEST];
    char response[HTTP_MAX_RESPONSE];
    int request_len;
    int response_len;
} HttpContext;

// FSM 状态处理函数
void http_handle_idle(HttpContext *ctx);
void http_handle_reading(HttpContext *ctx);
void http_handle_processing(HttpContext *ctx);
void http_handle_responding(HttpContext *ctx);
void http_handle_closing(HttpContext *ctx);

// 主事件循环
void http_server_run(HttpContext *ctx);

// 启动 HTTP 服务器
int http_server_start(int port);

// Chunked streaming support
int http_send_chunk(int client_fd, const char *data, size_t len);
int http_send_chunk_end(int client_fd);
int http_send_chunked_header(int client_fd, const char *content_type);

// Error responses
int http_send_error(int client_fd, int code, const char *message);

#endif
