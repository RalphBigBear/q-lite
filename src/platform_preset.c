#include "platform.h"
#include <string.h>

// Platform preset configurations (inspired by nanochat's --depth)
// Each preset auto-configures all platform-specific settings

static const PlatformConfig presets[] = {
    [TARGET_AUTO] = {
        .flash_size = 0,
        .ram_size = 0,
        .network_type = "auto",
        .max_connections = 10,
        .buffer_size = 2048,
        .queue_depth = 10,
        .timeout_ms = 30000
    },
    [TARGET_ESP32] = {
        .flash_size = 4 * 1024 * 1024,     // 4MB
        .ram_size = 520 * 1024,            // 520KB (usable)
        .network_type = "WiFi",
        .max_connections = 5,                // Limited by ESP32
        .buffer_size = 1024,               // Smaller for ESP32
        .queue_depth = 3,                   // Very limited RAM
        .timeout_ms = 60000                 // Longer timeout for WiFi
    },
    [TARGET_STM32] = {
        .flash_size = 512 * 1024,           // 512KB
        .ram_size = 128 * 1024,            // 128KB
        .network_type = "Ethernet",
        .max_connections = 8,
        .buffer_size = 512,                  // Very limited RAM
        .queue_depth = 2,                   // Extremely limited
        .timeout_ms = 30000
    },
    [TARGET_PICO] = {
        .flash_size = 2 * 1024 * 1024,     // 2MB
        .ram_size = 264 * 1024,            // 264KB
        .network_type = "WiFi",
        .max_connections = 4,
        .buffer_size = 768,                  // Medium
        .queue_depth = 2,                   // Limited RAM
        .timeout_ms = 45000
    },
    [TARGET_DESKTOP] = {
        .flash_size = 0,                   // N/A
        .ram_size = 0,                     // N/A (plenty)
        .network_type = "Ethernet/WiFi",
        .max_connections = 100,              // Much higher
        .buffer_size = 8192,               // Large buffers
        .queue_depth = 20,                  // Deep queue
        .timeout_ms = 10000                 // Shorter timeout
    }
};

PlatformConfig platform_get_preset(PlatformPreset preset) {
    if (preset >= 0 && preset < sizeof(presets) / sizeof(presets[0])) {
        return presets[preset];
    }
    // Default to auto if invalid
    return presets[TARGET_AUTO];
}
