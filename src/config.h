#ifndef CONFIG_H
#define CONFIG_H

#define Q_LITE_VERSION "0.1.0"
#define DEFAULT_PORT 8080
#define DEFAULT_OLLAMA "http://localhost:11434"
#define MAX_REQUEST_SIZE 4096

typedef struct {
    int port;
    char ollama_url[256];
} Config;

#endif
