#ifndef PLATFORM_H
#define PLATFORM_H

#include <stddef.h>
#include <stdint.h>

// Platform abstraction for microcontroller ports

// Platform types
#define PLATFORM_ESP32 1
#define PLATFORM_STM32 2
#define PLATFORM_PICO 3
#define PLATFORM_DESKTOP 0

// Platform presets (inspired by nanochat's --depth philosophy)
typedef enum {
    TARGET_AUTO = 0,    // Auto-detect platform
    TARGET_ESP32,       // ESP32-S3 (WiFi, 4MB PSRAM)
    TARGET_STM32,       // STM32F4/F7 (Ethernet)
    TARGET_PICO,        // Raspberry Pi Pico (WiFi via ESP8266)
    TARGET_DESKTOP      // Desktop/Linux/macOS/Windows
} PlatformPreset;

// Platform configuration
typedef struct {
    uint32_t flash_size;
    uint32_t ram_size;
    const char *network_type;
    int max_connections;
    int buffer_size;         // HTTP buffer size
    int queue_depth;         // Request queue depth
    int timeout_ms;         // Request timeout
} PlatformConfig;

// Platform operations
typedef struct {
    // Initialize platform hardware
    void (*init)(void);

    // Start network (WiFi/Ethernet)
    int (*network_start)(const char *ssid, const char *password);

    // Get current time in milliseconds
    uint32_t (*get_time_ms)(void);

    // Debug print
    void (*debug_print)(const char *msg);

    // Delay
    void (*delay_ms)(uint32_t ms);

    // Get platform configuration
    PlatformConfig (*get_config)(void);
} PlatformOps;

// Platform detection
#if defined(ESP32)
    #define PLATFORM_CURRENT PLATFORM_ESP32
    PlatformOps* platform_esp32_get_ops(void);
#elif defined(STM32)
    #define PLATFORM_CURRENT PLATFORM_STM32
    PlatformOps* platform_stm32_get_ops(void);
#elif defined(PICO)
    #define PLATFORM_CURRENT PLATFORM_PICO
    PlatformOps* platform_pico_get_ops(void);
#else
    #define PLATFORM_CURRENT 0
    #warning "No platform defined"
#endif

// Global platform operations (set at runtime)
extern PlatformOps *platform_ops;

// Platform initialization
void platform_init(void);

// Get preset configuration (inspired by nanochat's single-dial philosophy)
PlatformConfig platform_get_preset(PlatformPreset preset);

// Convenience macros
#define PLATFORM_INIT()          platform_init()
#define PLATFORM_DEBUG(msg)     if(platform_ops) platform_ops->debug_print(msg)
#define PLATFORM_DELAY(ms)       if(platform_ops) platform_ops->delay_ms(ms)
#define PLATFORM_TIME_MS()       (platform_ops ? platform_ops->get_time_ms() : 0)

#endif
