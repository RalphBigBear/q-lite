// Q-Lite for ESP32 - Microcontroller LLM Gateway

#include <stdio.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "platform.h"
#include "http.h"

static const char *TAG = "Q-Lite";

extern void http_server_main(void);

void app_main(void)
{
    // Initialize platform
    platform_init();

    // Print platform info
    ESP_LOGI(TAG, "Starting Q-Lite on ESP32...");
    platform_print_info();

    // Initialize NVS (Non-Volatile Storage)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize WiFi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Start WiFi (use NVS credentials)
    esp_wifi_start();

    // Start HTTP server
    http_server_main();

    ESP_LOGI(TAG, "Q-Lite ready");
}
