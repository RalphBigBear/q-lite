// STM32 Platform Implementation

#if defined(STM32_PLATFORM)

#include "platform.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "lwip/init.h"
#include "lwip/tcp.h"
#include "FreeRTOS.h"
#include <sys/time.h>

static const char *TAG = "Q-Lite-STM32";

// STM32: Get time in milliseconds
static uint32_t stm32_get_time_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000UL + tv.tv_usec / 1000UL);
}

// STM32: Debug print (UART)
static void stm32_debug_print(const char *msg) {
    // TODO: Implement UART debug output
    // HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

// STM32: Delay
static void stm32_delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}

// STM32: Network start (Ethernet)
static int stm32_network_start(const char *ssid, const char *password) {
    // STM32 uses Ethernet, ignore WiFi credentials
    (void)ssid;
    (void)password;

    // Initialize LwIP
    lwip_init();

    // Initialize Ethernet MAC
    // TODO: Add HAL_ETH_Init()

    return 0;
}

// STM32: Get configuration
static PlatformConfig stm32_get_config(void) {
    PlatformConfig config = {
        .flash_size = 512 * 1024,  // 512KB
        .ram_size = 128 * 1024,        // 128KB
        .network_type = "Ethernet (10/100Mbps)",
        .max_connections = 3
    };
    return config;
}

// STM32 platform operations
PlatformOps* platform_stm32_get_ops(void) {
    static PlatformOps ops = {
        .init = NULL,  // STM32CubeIDE handles init
        .network_start = stm32_network_start,
        .debug_print = stm32_debug_print,
        .delay_ms = stm32_delay_ms,
        .get_time_ms = stm32_get_time_ms,
        .get_config = stm32_get_config
    };
    return &ops;
}

#endif // STM32_PLATFORM
