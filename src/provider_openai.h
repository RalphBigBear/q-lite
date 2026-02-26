// Q-Lite - OpenAI Provider
// Supports OpenAI-compatible APIs (OpenAI, DeepSeek, etc.)

#ifndef PROVIDER_OPENAI_H
#define PROVIDER_OPENAI_H

#include "backend.h"

// OpenAI Provider Configuration
typedef struct {
    char base_url[256];
    char api_key[256];
    char model[64];
    int timeout_ms;
} provider_openai_config_t;

// Initialize OpenAI provider
int provider_openai_init(const provider_openai_config_t *config);

// Create chat completion session
backend_session_t *provider_openai_create_session(const chat_request_t *request);

// Send message to OpenAI
int provider_openai_send_message(backend_session_t *session, const chat_message_t *message);

// Get response (streaming)
int provider_openai_get_response(backend_session_t *session, char *buffer, size_t buffer_size);

// Close session
void provider_openai_close_session(backend_session_t *session);

// Cleanup
void provider_openai_cleanup(void);

#endif // PROVIDER_OPENAI_H
