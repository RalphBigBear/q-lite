#ifndef BACKEND_H
#define BACKEND_H

#include <stddef.h>
#include <stdint.h>

// Backend types
#define BACKEND_OLLAMA 0
#define BACKEND_OPENAI_COMPAT 1

// Task 3: KV Cache for streaming (inspired by nanochat's engine.py)
// Session context for caching KV pairs across tokens
typedef struct {
    char *session_id;
    void *kv_cache;
    uint32_t last_token_pos;
    uint32_t cache_size;
} SessionContext;

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
    char* (*generate_with_cache)(Backend *backend, const char *model, const char *prompt, SessionContext *ctx);
    char* (*chat)(Backend *backend, const char *model, const char *message);
    void (*destroy)(Backend *backend);
    void (*session_free)(SessionContext *ctx);
} BackendOps;

// Backend functions
Backend* backend_ollama_create(const char *host, int port);
Backend* backend_openai_create(const char *host, int port);
void backend_destroy(Backend *backend);

// API functions
char* backend_generate(Backend *backend, const char *model, const char *prompt);
char* backend_generate_cached(Backend *backend, const char *model, const char *prompt, SessionContext *ctx);
char* backend_chat(Backend *backend, const char *model, const char *message);

// Session management (Task 3)
SessionContext* backend_session_create(const char *session_id);
void backend_session_free(SessionContext *ctx);

// Auto-detect backend
Backend* backend_autodetect(void);

#endif
