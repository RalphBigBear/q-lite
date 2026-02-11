#include "backend.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// Ollama backend implementation (wrapper around ollama.c)
Backend* backend_ollama_create(const char *host, int port) {
    Backend *backend = malloc(sizeof(Backend));
    if (!backend) return NULL;

    strncpy(backend->name, "ollama", sizeof(backend->name) - 1);
    strncpy(backend->host, host, sizeof(backend->host) - 1);
    backend->port = port;
    backend->api_type = BACKEND_OLLAMA;
    backend->priv = NULL;

    return backend;
}

// OpenAI-compatible backend implementation
Backend* backend_openai_create(const char *host, int port) {
    Backend *backend = malloc(sizeof(Backend));
    if (!backend) return NULL;

    strncpy(backend->name, "openai", sizeof(backend->name) - 1);
    strncpy(backend->host, host, sizeof(backend->host) - 1);
    backend->port = port;
    backend->api_type = BACKEND_OPENAI_COMPAT;
    backend->priv = NULL;

    return backend;
}

// Destroy backend
void backend_destroy(Backend *backend) {
    if (backend) {
        free(backend);
    }
}

// OpenAI-compatible HTTP POST (reuse from ollama.c)
extern int http_post(const char *host, int port,
                     const char *path, const char *body,
                     char *response, size_t response_size);

// OpenAI Generate API
static char* openai_generate(Backend *backend, const char *model, const char *prompt) {
    char body[4096];
    snprintf(body, sizeof(body),
        "{\"model\":\"%s\",\"prompt\":\"%s\",\"max_tokens\":512}",
        model, prompt
    );

    char response[8192];
    int result = http_post(backend->host, backend->port,
                          "/v1/completions", body,
                          response, sizeof(response));

    if (result < 0) {
        return strdup("{\"error\":\"Failed to connect to OpenAI-compatible backend\"}");
    }

    // Extract text from OpenAI response: {"choices":[{"text":"..."}]}
    char *text_start = strstr(response, "\"text\":\"");
    if (!text_start) {
        return strdup("{\"error\":\"Invalid OpenAI response\"}");
    }

    text_start += strlen("\"text\":\"");
    char *text_end = strchr(text_start, '"');
    if (!text_end) {
        return strdup("{\"error\":\"Invalid OpenAI response\"}");
    }

    *text_end = '\0';
    char *reply = strdup(text_start);
    *text_end = '"';

    return reply;
}

// OpenAI Chat API
static char* openai_chat(Backend *backend, const char *model, const char *message) {
    char body[4096];
    snprintf(body, sizeof(body),
        "{\"model\":\"%s\",\"messages\":[{\"role\":\"user\",\"content\":\"%s\"}],\"max_tokens\":512}",
        model, message
    );

    char response[8192];
    int result = http_post(backend->host, backend->port,
                          "/v1/chat/completions", body,
                          response, sizeof(response));

    if (result < 0) {
        return strdup("{\"error\":\"Failed to connect to OpenAI-compatible backend\"}");
    }

    // Extract content from OpenAI response: {"choices":[{"message":{"content":"..."}}]}
    char *content_start = strstr(response, "\"content\":\"");
    if (!content_start) {
        return strdup("{\"error\":\"Invalid OpenAI response\"}");
    }

    content_start += strlen("\"content\":\"");
    char *content_end = strchr(content_start, '"');
    if (!content_end) {
        return strdup("{\"error\":\"Invalid OpenAI response\"}");
    }

    *content_end = '\0';
    char *reply = strdup(content_start);
    *content_end = '"';

    return reply;
}

// Generate text (dispatches to appropriate backend)
char* backend_generate(Backend *backend, const char *model, const char *prompt) {
    if (!backend) return strdup("{\"error\":\"Backend not initialized\"}");

    if (backend->api_type == BACKEND_OLLAMA) {
        // Use existing ollama_generate
        extern char* ollama_generate(const char *model, const char *prompt);
        return ollama_generate(model, prompt);
    } else if (backend->api_type == BACKEND_OPENAI_COMPAT) {
        return openai_generate(backend, model, prompt);
    }

    return strdup("{\"error\":\"Unknown backend type\"}");
}

// Chat (dispatches to appropriate backend)
char* backend_chat(Backend *backend, const char *model, const char *message) {
    if (!backend) return strdup("{\"error\":\"Backend not initialized\"}");

    if (backend->api_type == BACKEND_OLLAMA) {
        // Use existing ollama_chat
        extern char* ollama_chat(const char *model, const char *message);
        return ollama_chat(model, message);
    } else if (backend->api_type == BACKEND_OPENAI_COMPAT) {
        return openai_chat(backend, model, message);
    }

    return strdup("{\"error\":\"Unknown backend type\"}");
}

// Simple port check (non-blocking)
static int check_port_available(const char *host, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return 0;

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(host);

    // Set timeout (1 second)
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));

    int result = connect(sock, (struct sockaddr*)&server, sizeof(server));
    close(sock);

    return (result == 0) ? 1 : 0;
}

// Auto-detect backend (try Ollama, vLLM, LM Studio)
Backend* backend_autodetect(void) {
    // Priority: Ollama > vLLM > LM Studio
    const char *backends[][3] = {
        {"ollama", "localhost", "11434"},
        {"vllm", "localhost", "8000"},
        {"lmstudio", "localhost", "1234"},
        {NULL, NULL, NULL}
    };

    for (int i = 0; backends[i][0] != NULL; i++) {
        const char *name = backends[i][0];
        const char *host = backends[i][1];
        int port = atoi(backends[i][2]);

        if (check_port_available(host, port)) {
            if (strcmp(name, "ollama") == 0) {
                return backend_ollama_create(host, port);
            } else {
                return backend_openai_create(host, port);
            }
        }
    }

    // Fallback to Ollama (default)
    return backend_ollama_create("localhost", 11434);
}
