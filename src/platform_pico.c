// Raspberry Pi Pico Platform Implementation

#if defined(PICO_PLATFORM)

#include "platform.h"
#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"

#define UART_ID uart0
#define BAUD_RATE 115200
#define ESP8266_UART uart1

// Pico: Get time in milliseconds
static uint32_t pico_get_time_ms(void) {
    return to_ms_since_boot(get_absolute_time());
}

// Pico: Debug print (UART)
static void pico_debug_print(const char *msg) {
    uart_puts(UART_ID, msg);
    uart_puts(UART_ID, "\n");
}

// Pico: Delay
static void pico_delay_ms(uint32_t ms) {
    sleep_ms(ms);
}

// Pico: Network start (via ESP8266 AT commands)
static int pico_network_start(const char *ssid, const char *password) {
    // Send AT commands to ESP8266 via UART
    char cmd[128];

    // Reset ESP8266
    uart_puts(ESP8266_UART, "AT+RST\r\n");
    sleep_ms(2000);

    // Set WiFi mode
    uart_puts(ESP8266_UART, "AT+CWMODE=1\r\n");
    sleep_ms(500);

    // Connect to WiFi
    snprintf(cmd, sizeof(cmd), "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, password);
    uart_puts(ESP8266_UART, cmd);
    sleep_ms(10000);  // Wait for connection

    // Check connection status
    uart_puts(ESP8266_UART, "AT+CWJAP?\r\n");
    sleep_ms(500);

    return 0;
}

// Pico: Get configuration
static PlatformConfig pico_get_config(void) {
    PlatformConfig config = {
        .flash_size = 2 * 1024 * 1024,  // 2MB
        .ram_size = 264 * 1024,             // 264KB
        .network_type = "WiFi (ESP8266 via UART)",
        .max_connections = 2
    };
    return config;
}

// Pico platform operations
PlatformOps* platform_pico_get_ops(void) {
    static PlatformOps ops = {
        .init = NULL,  // Pico SDK handles init
        .network_start = pico_network_start,
        .debug_print = pico_debug_print,
        .delay_ms = pico_delay_ms,
        .get_time_ms = pico_get_time_ms,
        .get_config = pico_get_config
    };
    return &ops;
}

#endif // PICO_PLATFORM
