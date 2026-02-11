// ESP32 Platform Implementation

#if defined(ESP32_PLATFORM)

#include "platform.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <sys/time.h>

static const char *TAG = "Q-Lite-ESP32";

// ESP32: Get time in milliseconds
static uint32_t esp32_get_time_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000UL + tv.tv_usec / 1000UL);
}

// ESP32: Debug print
static void esp32_debug_print(const char *msg) {
    ESP_LOGI(TAG, "%s", msg);
}

// ESP32: Delay
static void esp32_delay_ms(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

// ESP32: Network start (WiFi)
static int esp32_network_start(const char *ssid, const char *password) {
    ESP_LOGI(TAG, "Connecting to WiFi: %s", ssid);

    wifi_config_t wifi_config = {
        .sta = {
            .threshold = 0,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };

    strcpy((char*)wifi_config.sta.ssid, ssid);
    strcpy((char*)wifi_config.sta.password, password);

    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

    // Wait for connection (TODO: add event handler)
    vTaskDelay(pdMS_TO_TICKS(5000));

    return 0;
}

// ESP32: Get configuration
static PlatformConfig esp32_get_config(void) {
    PlatformConfig config = {
        .flash_size = 4 * 1024 * 1024,  // 4MB
        .ram_size = 520 * 1024,             // 520KB
        .network_type = "WiFi (built-in)",
        .max_connections = 5
    };
    return config;
}

// ESP32 platform operations
PlatformOps* platform_esp32_get_ops(void) {
    static PlatformOps ops = {
        .init = NULL,  // ESP-IDF handles init
        .network_start = esp32_network_start,
        .debug_print = esp32_debug_print,
        .delay_ms = esp32_delay_ms,
        .get_time_ms = esp32_get_time_ms,
        .get_config = esp32_get_config
    };
    return &ops;
}

#endif // ESP32_PLATFORM
