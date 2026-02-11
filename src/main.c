#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "config.h"
#include "http.h"

// 全局上下文
static volatile int running = 1;

// 信号处理
void sigint_handler(int sig __attribute__((unused))) {
    printf("\n[Q-Lite] Shutting down...\n");
    running = 0;
}

void print_usage(const char *prog) {
    printf("Q-Lite v%s - Ultra-lightweight LLM gateway\n", Q_LITE_VERSION);
    printf("Inspired by llama2.c (Karpathy), ESP32-LLM (Davebben), PicoClaw (Sipeed)\n\n");
    printf("Usage: %s [options]\n", prog);
    printf("Options:\n");
    printf("  --port PORT         HTTP server port (default: %d)\n", DEFAULT_PORT);
    printf("  --ollama URL        Ollama server URL (default: %s)\n", DEFAULT_OLLAMA);
    printf("  --help              Show this help\n");
}

int main(int argc, char **argv) {
    Config config = {
        .port = DEFAULT_PORT,
        .ollama_url = DEFAULT_OLLAMA
    };

    // 解析参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--port") == 0 && i + 1 < argc) {
            config.port = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--ollama") == 0 && i + 1 < argc) {
            strncpy(config.ollama_url, argv[++i], sizeof(config.ollama_url) - 1);
        } else if (strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
    }

    printf("╔══════════════════════════════════════════╗\n");
    printf("║     Q-Lite v%s - HTTP Gateway          ║\n", Q_LITE_VERSION);
    printf("╠══════════════════════════════════════════╣\n");
    printf("║  Port:    %-4d                         ║\n", config.port);
    printf("║  Ollama:  %-30s ║\n", config.ollama_url);
    printf("╚══════════════════════════════════════════╝\n");

    // 设置信号处理
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigint_handler);

    // 启动 HTTP 服务器
    int server_fd = http_server_start(config.port);
    if (server_fd < 0) {
        fprintf(stderr, "Failed to start HTTP server\n");
        return 1;
    }

    // 初始化 HTTP 上下文
    HttpContext ctx = {
        .server_fd = server_fd,
        .client_fd = -1,
        .state = HTTP_STATE_IDLE
    };

    printf("[Q-Lite] Ready to accept connections\n");
    printf("[Q-Lite] Press Ctrl+C to stop\n\n");

    // 主事件循环
    while (running) {
        http_server_run(&ctx);
    }

    // 清理
    close(server_fd);
    printf("[Q-Lite] Shutdown complete\n");

    return 0;
}
