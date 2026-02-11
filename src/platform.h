#ifndef PLATFORM_H
#define PLATFORM_H

#include <stddef.h>
#include <stdint.h>

// Platform abstraction for microcontroller ports

// Platform types
#define PLATFORM_ESP32 1
#define PLATFORM_STM32 2
#define PLATFORM_PICO 3

// Platform configuration
typedef struct {
    uint32_t flash_size;
    uint32_t ram_size;
    const char *network_type;
    int max_connections;
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

// Convenience macros
#define PLATFORM_INIT()          platform_init()
#define PLATFORM_DEBUG(msg)     if(platform_ops) platform_ops->debug_print(msg)
#define PLATFORM_DELAY(ms)       if(platform_ops) platform_ops->delay_ms(ms)
#define PLATFORM_TIME_MS()       (platform_ops ? platform_ops->get_time_ms() : 0)

#endif
