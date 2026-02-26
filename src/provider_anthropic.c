// Q-Lite - Anthropic Provider Implementation

#include "provider_anthropic.h"
#include "http.h"
#include <string.h>
#include <stdlib.h>

static provider_anthropic_config_t g_config = {0};
static int g_initialized = 0;

// Initialize Anthropic provider
int provider_anthropic_init(const provider_anthropic_config_t *config) {
    if (!config || !config->api_key[0]) {
        return -1;
    }

    memcpy(&g_config, config, sizeof(provider_anthropic_config_t));
    g_initialized = 1;
    return 0;
}

// Create chat completion session
backend_session_t *provider_anthropic_create_session(const chat_request_t *request) {
    if (!g_initialized) {
        return NULL;
    }

    backend_session_t *session = (backend_session_t *)malloc(sizeof(backend_session_t));
    if (!session) {
        return NULL;
    }

    memset(session, 0, sizeof(backend_session_t));
    session->provider_type = BACKEND_PROVIDER_ANTHROPIC;

    // Build request body (Anthropic format)
    char request_body[4096];
    snprintf(request_body, sizeof(request_body),
        "{"
        "\"model\":\"%s\","
        "\"max_tokens\":%d,"
        "\"messages\":["
            "{\"role\":\"user\",\"content\":\"%s\"}"
        "],"
        "\"stream\":%s"
        "}",
        g_config.model,
        request->max_tokens > 0 ? request->max_tokens : 4096,
        request->messages[0].content,
        request->stream ? "true" : "false"
    );

    // Initialize HTTP connection
    session->http = http_client_init(g_config.base_url, "/v1/messages");
    if (!session->http) {
        free(session);
        return NULL;
    }

    // Set headers (Anthropic uses x-api-key, not Authorization)
    char auth_header[512];
    snprintf(auth_header, sizeof(auth_header), "x-api-key: %s", g_config.api_key);
    http_client_set_header(session->http, "Content-Type", "application/json");
    http_client_set_header(session->http, "anthropic-version", "2023-06-01");
    http_client_set_header(session->http, auth_header, NULL);

    // Send request
    http_client_post(session->http, request_body);

    return session;
}

// Send message to Anthropic
int provider_anthropic_send_message(backend_session_t *session, const chat_message_t *message) {
    if (!session || !message) {
        return -1;
    }

    // Anthropic API requires full message history in each request
    // This is a simplified implementation
    return 0;
}

// Get response (streaming)
int provider_anthropic_get_response(backend_session_t *session, char *buffer, size_t buffer_size) {
    if (!session || !buffer) {
        return -1;
    }

    // Read HTTP response
    int bytes_read = http_client_read(session->http, buffer, buffer_size);

    // Parse streaming response (Anthropic format: "data: {...}")
    if (bytes_read > 0 && strncmp(buffer, "data: ", 6) == 0) {
        char *delta_start = strstr(buffer + 6, "\"delta\":{\"text\":\"");
        if (delta_start) {
            delta_start += 17; // Skip "\"delta\":{\"text\":\""
            char *delta_end = strstr(delta_start, "\"");
            if (delta_end) {
                size_t delta_len = delta_end - delta_start;
                if (delta_len < buffer_size) {
                    memcpy(buffer, delta_start, delta_len);
                    buffer[delta_len] = '\0';
                    return (int)delta_len;
                }
            }
        }
    }

    return bytes_read;
}

// Close session
void provider_anthropic_close_session(backend_session_t *session) {
    if (session) {
        if (session->http) {
            http_client_cleanup(session->http);
        }
        free(session);
    }
}

// Cleanup
void provider_anthropic_cleanup(void) {
    g_initialized = 0;
    memset(&g_config, 0, sizeof(provider_anthropic_config_t));
}
