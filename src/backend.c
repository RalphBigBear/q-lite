#include "backend.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Generate text (dispatches to appropriate backend)
char* backend_generate(Backend *backend, const char *model, const char *prompt) {
    if (!backend) return strdup("{\"error\":\"Backend not initialized\"}");

    if (backend->api_type == BACKEND_OLLAMA) {
        // Use existing ollama_generate
        extern char* ollama_generate(const char *model, const char *prompt);
        return ollama_generate(model, prompt);
    } else if (backend->api_type == BACKEND_OPENAI_COMPAT) {
        // TODO: Implement OpenAI-compatible generate
        return strdup("{\"error\":\"OpenAI backend not yet implemented\"}");
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
        // TODO: Implement OpenAI-compatible chat
        return strdup("{\"error\":\"OpenAI backend not yet implemented\"}");
    }

    return strdup("{\"error\":\"Unknown backend type\"}");
}

// Auto-detect backend (try Ollama first, then OpenAI)
Backend* backend_autodetect(void) {
    // Try Ollama (localhost:11434)
    Backend *ollama = backend_ollama_create("localhost", 11434);
    // TODO: Actually check if backend is available
    return ollama;  // Default to Ollama for now
}
