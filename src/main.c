#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

void print_usage(const char *prog) {
    printf("Q-Lite v%s - Ultra-lightweight LLM gateway\n", Q_LITE_VERSION);
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

    // Parse arguments
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

    printf("Starting Q-Lite on port %d...\n", config.port);
    printf("Ollama backend: %s\n", config.ollama_url);
    printf("\nInspired by:\n");
    printf("  - llama2.c (Andrej Karpathy)\n");
    printf("  - ESP32-LLM (Davebben)\n");
    printf("  - PicoClaw (Sipeed)\n");

    // TODO: Start HTTP server

    return 0;
}
