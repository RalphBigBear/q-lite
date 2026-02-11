// Platform Initialization

#include "platform.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Global platform operations (set at runtime or compile-time)
PlatformOps *platform_ops = NULL;

// Platform initialization
void platform_init(void) {
#if defined(ESP32_PLATFORM)
    platform_ops = platform_esp32_get_ops();
    PLATFORM_DEBUG("Q-Lite: ESP32 Platform initialized");
#elif defined(STM32_PLATFORM)
    platform_ops = platform_stm32_get_ops();
    PLATFORM_DEBUG("Q-Lite: STM32 Platform initialized");
#elif defined(PICO_PLATFORM)
    platform_ops = platform_pico_get_ops();
    PLATFORM_DEBUG("Q-Lite: Pico Platform initialized");
#else
    // Desktop/Linux platform (no platform ops)
    platform_ops = NULL;
#endif

    // Call platform init if available
    if (platform_ops && platform_ops->init) {
        platform_ops->init();
    }
}

// Get platform configuration
PlatformConfig platform_get_config(void) {
    if (platform_ops && platform_ops->get_config) {
        return platform_ops->get_config();
    }

    // Default (desktop)
    PlatformConfig config = {
        .flash_size = 0,
        .ram_size = 0,
        .network_type = "Desktop",
        .max_connections = 10
    };
    return config;
}

// Print platform info
void platform_print_info(void) {
    if (!platform_ops) {
        printf("Platform: Desktop/Linux\n");
        return;
    }

    PlatformConfig config = platform_get_config();

    printf("Platform: Microcontroller\n");
    printf("  Flash: %u KB\n", config.flash_size / 1024);
    printf("  RAM:   %u KB\n", config.ram_size / 1024);
    printf("  Network: %s\n", config.network_type);
    printf("  Max Connections: %d\n", config.max_connections);
}
