#ifndef BACKEND_H
#define BACKEND_H

#include <stddef.h>

// Backend types
#define BACKEND_OLLAMA 0
#define BACKEND_OPENAI_COMPAT 1

// Backend structure
typedef struct {
    char name[32];
    char host[256];
    int port;
    int api_type;
    void *priv;
} Backend;

// Backend operations
typedef struct {
    char* (*generate)(Backend *backend, const char *model, const char *prompt);
    char* (*chat)(Backend *backend, const char *model, const char *message);
    void (*destroy)(Backend *backend);
} BackendOps;

// Backend functions
Backend* backend_ollama_create(const char *host, int port);
Backend* backend_openai_create(const char *host, int port);
void backend_destroy(Backend *backend);

// API functions
char* backend_generate(Backend *backend, const char *model, const char *prompt);
char* backend_chat(Backend *backend, const char *model, const char *message);

// Auto-detect backend
Backend* backend_autodetect(void);

#endif
