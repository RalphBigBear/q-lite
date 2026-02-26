// Q-Lite - OpenAI Provider Implementation

#include "provider_openai.h"
#include "http.h"
#include <string.h>
#include <stdlib.h>

static provider_openai_config_t g_config = {0};
static int g_initialized = 0;

// Initialize OpenAI provider
int provider_openai_init(const provider_openai_config_t *config) {
    if (!config || !config->api_key[0]) {
        return -1;
    }

    memcpy(&g_config, config, sizeof(provider_openai_config_t));
    g_initialized = 1;
    return 0;
}

// Create chat completion session
backend_session_t *provider_openai_create_session(const chat_request_t *request) {
    if (!g_initialized) {
        return NULL;
    }

    backend_session_t *session = (backend_session_t *)malloc(sizeof(backend_session_t));
    if (!session) {
        return NULL;
    }

    memset(session, 0, sizeof(backend_session_t));
    session->provider_type = BACKEND_PROVIDER_OPENAI;

    // Build request body (OpenAI format)
    char request_body[2048];
    snprintf(request_body, sizeof(request_body),
        "{"
        "\"model\":\"%s\","
        "\"messages\":["
            "{\"role\":\"system\",\"content\":\"%s\"},"
            "{\"role\":\"user\",\"content\":\"%s\"}"
        "],"
        "\"stream\":%s"
        "}",
        g_config.model,
        request->system_prompt ? request->system_prompt : "You are a helpful assistant.",
        request->messages[0].content,
        request->stream ? "true" : "false"
    );

    // Initialize HTTP connection
    session->http = http_client_init(g_config.base_url, "/v1/chat/completions");
    if (!session->http) {
        free(session);
        return NULL;
    }

    // Set headers
    char auth_header[512];
    snprintf(auth_header, sizeof(auth_header), "Authorization: Bearer %s", g_config.api_key);
    http_client_set_header(session->http, "Content-Type", "application/json");
    http_client_set_header(session->http, auth_header, NULL);

    // Send request
    http_client_post(session->http, request_body);

    return session;
}

// Send message to OpenAI
int provider_openai_send_message(backend_session_t *session, const chat_message_t *message) {
    if (!session || !message) {
        return -1;
    }

    // OpenAI API requires full message history in each request
    // This is a simplified implementation
    return 0;
}

// Get response (streaming)
int provider_openai_get_response(backend_session_t *session, char *buffer, size_t buffer_size) {
    if (!session || !buffer) {
        return -1;
    }

    // Read HTTP response
    int bytes_read = http_client_read(session->http, buffer, buffer_size);

    // Parse streaming response (OpenAI format: "data: {...}")
    if (bytes_read > 0 && strncmp(buffer, "data: ", 6) == 0) {
        char *content_start = strstr(buffer + 6, "\"content\":\"");
        if (content_start) {
            content_start += 11; // Skip "\"content\":\""
            char *content_end = strstr(content_start, "\"");
            if (content_end) {
                size_t content_len = content_end - content_start;
                if (content_len < buffer_size) {
                    memcpy(buffer, content_start, content_len);
                    buffer[content_len] = '\0';
                    return (int)content_len;
                }
            }
        }
    }

    return bytes_read;
}

// Close session
void provider_openai_close_session(backend_session_t *session) {
    if (session) {
        if (session->http) {
            http_client_cleanup(session->http);
        }
        free(session);
    }
}

// Cleanup
void provider_openai_cleanup(void) {
    g_initialized = 0;
    memset(&g_config, 0, sizeof(provider_openai_config_t));
}
