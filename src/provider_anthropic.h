// Q-Lite - Anthropic Provider
// Supports Claude API (Claude 3.5 Sonnet, Claude 3 Opus, etc.)

#ifndef PROVIDER_ANTHROPIC_H
#define PROVIDER_ANTHROPIC_H

#include "backend.h"

// Anthropic Provider Configuration
typedef struct {
    char base_url[256];
    char api_key[256];
    char model[64];
    int timeout_ms;
} provider_anthropic_config_t;

// Initialize Anthropic provider
int provider_anthropic_init(const provider_anthropic_config_t *config);

// Create chat completion session
backend_session_t *provider_anthropic_create_session(const chat_request_t *request);

// Send message to Anthropic
int provider_anthropic_send_message(backend_session_t *session, const chat_message_t *message);

// Get response (streaming)
int provider_anthropic_get_response(backend_session_t *session, char *buffer, size_t buffer_size);

// Close session
void provider_anthropic_close_session(backend_session_t *session);

// Cleanup
void provider_anthropic_cleanup(void);

#endif // PROVIDER_ANTHROPIC_H
