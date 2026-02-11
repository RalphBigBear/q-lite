#include "backend.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Task 3: KV Cache implementation (inspired by nanochat)
// Simple malloc-based cache for desktop platforms

// Cache entry for each token position
typedef struct {
    void *key;
    void *value;
} KVPair;

// Create a new session context
SessionContext* backend_session_create(const char *session_id) {
    SessionContext *ctx = (SessionContext*)malloc(sizeof(SessionContext));
    if (!ctx) {
        fprintf(stderr, "[Backend] Failed to allocate session context\n");
        return NULL;
    }

    ctx->session_id = strdup(session_id);
    ctx->kv_cache = NULL;
    ctx->last_token_pos = 0;
    ctx->cache_size = 0;

    printf("[Backend] Created session: %s\n", session_id);

    return ctx;
}

// Free session context
void backend_session_free(SessionContext *ctx) {
    if (!ctx) return;

    printf("[Backend] Freeing session: %s\n", ctx->session_id);

    // Free cache entries
    if (ctx->kv_cache) {
        for (uint32_t i = 0; i < ctx->cache_size; i++) {
            KVPair *kv = &((KVPair*)ctx->kv_cache)[i];
            free(kv->key);
            free(kv->value);
        }
        free(ctx->kv_cache);
    }

    // Free session ID
    if (ctx->session_id) {
        free(ctx->session_id);
    }

    free(ctx);
}

// Resize cache (internal function)
static int session_cache_resize(SessionContext *ctx, uint32_t new_size) {
    KVPair *new_cache = (KVPair*)realloc(ctx->kv_cache, sizeof(KVPair) * new_size);
    if (!new_cache) {
        fprintf(stderr, "[Backend] Failed to resize cache to %u entries\n", new_size);
        return -1;
    }

    ctx->kv_cache = new_cache;
    ctx->cache_size = new_size;

    return 0;
}

// Store KV pair in cache
int backend_session_cache_kv(SessionContext *ctx, void *key, void *value) {
    if (!ctx || !key || !value) {
        return -1;
    }

    // Resize if needed (double size)
    if (ctx->last_token_pos >= ctx->cache_size) {
        uint32_t new_size = ctx->cache_size == 0 ? 64 : ctx->cache_size * 2;
        if (session_cache_resize(ctx, new_size) < 0) {
            return -1;
        }
    }

    // Store KV pair
    KVPair *kv = &((KVPair*)ctx->kv_cache)[ctx->last_token_pos];
    kv->key = key;  // Caller owns memory
    kv->value = value;

    ctx->last_token_pos++;

    return 0;
}

// Get KV pair from cache
void* backend_session_get_kv(SessionContext *ctx, uint32_t token_pos) {
    if (!ctx || !ctx->kv_cache) {
        return NULL;
    }

    if (token_pos >= ctx->last_token_pos) {
        return NULL;
    }

    KVPair *kv = &((KVPair*)ctx->kv_cache)[token_pos];
    return kv->value;
}

// Get cache statistics
void backend_session_stats(const SessionContext *ctx) {
    if (!ctx) {
        printf("[Backend] Session: NULL\n");
        return;
    }

    printf("[Backend] Session: %s\n", ctx->session_id);
    printf("[Backend]   Cache entries: %u\n", ctx->last_token_pos);
    printf("[Backend]   Cache capacity: %u\n", ctx->cache_size);
}
