#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "config.h"
#include "http.h"
#include "mem-profile.h"
#include "backend.h"

// 全局上下文
static volatile int running = 1;

// Task 3: Request Queue
volatile int active_requests = 0;
#define MAX_CONCURRENT_REQUESTS 10

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
    printf("  --backend TYPE      Backend type: ollama, openai, auto (default: auto)\n");
    printf("  --backend-host HOST Backend host (default: localhost)\n");
    printf("  --backend-port PORT Backend port (default: auto-detect)\n");
    printf("  --memory-stats      Enable memory profiling\n");
    printf("  --help              Show this help\n");
    printf("\nBackends:\n");
    printf("  ollama   - Ollama API (port 11434)\n");
    printf("  openai   - OpenAI-compatible API (vLLM, LM Studio)\n");
    printf("  auto     - Auto-detect available backend\n");
}

int main(int argc, char **argv) {
    Config config = {
        .port = DEFAULT_PORT
    };

    int show_memory_stats = 0;
    char backend_type[32] = "auto";
    char backend_host[256] = "localhost";
    int backend_port = 0;

    // 解析参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--port") == 0 && i + 1 < argc) {
            config.port = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--backend") == 0 && i + 1 < argc) {
            strncpy(backend_type, argv[++i], sizeof(backend_type) - 1);
        } else if (strcmp(argv[i], "--backend-host") == 0 && i + 1 < argc) {
            strncpy(backend_host, argv[++i], sizeof(backend_host) - 1);
        } else if (strcmp(argv[i], "--backend-port") == 0 && i + 1 < argc) {
            backend_port = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--memory-stats") == 0) {
            show_memory_stats = 1;
        } else if (strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
    }

    // 初始化内存统计
    MemStats mem_stats;
    mem_profile_init(&mem_stats);

    // 初始化 backend
    Backend *backend = NULL;
    if (strcmp(backend_type, "auto") == 0) {
        backend = backend_autodetect();
    } else if (strcmp(backend_type, "ollama") == 0) {
        int port = (backend_port > 0) ? backend_port : 11434;
        backend = backend_ollama_create(backend_host, port);
    } else if (strcmp(backend_type, "openai") == 0) {
        int port = (backend_port > 0) ? backend_port : 8000;
        backend = backend_openai_create(backend_host, port);
    } else {
        fprintf(stderr, "Unknown backend type: %s\n", backend_type);
        return 1;
    }

    if (!backend) {
        fprintf(stderr, "Failed to initialize backend\n");
        return 1;
    }

    printf("╔══════════════════════════════════════════╗\n");
    printf("║     Q-Lite v%s - HTTP Gateway          ║\n", Q_LITE_VERSION);
    printf("╠══════════════════════════════════════════╣\n");
    printf("║  Port:    %-4d                         ║\n", config.port);
    printf("║  Backend: %-30s ║\n", backend->name);
    printf("║  Host:    %-30s ║\n", backend->host);
    printf("║  Port:    %-4d                         ║\n", backend->port);
    printf("║  Memory:  %-30s ║\n", show_memory_stats ? "Enabled" : "Disabled");
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

    // 显示初始内存状态
    if (show_memory_stats) {
        mem_profile_detailed();
    }

    // 主事件循环
    while (running) {
        http_server_run(&ctx);

        // 每隔一段时间显示内存统计（如果启用）
        if (show_memory_stats) {
            static int counter = 0;
            if (++counter % 1000 == 0) {  // 每 1000 次循环
                mem_profile_detailed();
            }
        }
    }

    // 清理
    close(server_fd);
    backend_destroy(backend);

    // 显示最终内存统计
    if (show_memory_stats) {
        printf("\n");
        mem_profile_print(&mem_stats);
        mem_profile_detailed();
    }

    printf("[Q-Lite] Shutdown complete\n");

    return 0;
}
