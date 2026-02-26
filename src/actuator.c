// Q-Lite - Actuator API Implementation

#include "actuator.h"
#include "platform.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_ACTUATORS 16
#define MAX_LINE 256

static actuator_config_t g_actuators[MAX_ACTUATORS];
static int g_actuator_count = 0;

// Parse actuator type from string
static actuator_type_t parse_actuator_type(const char *type_str) {
    if (strcmp(type_str, "led") == 0) return ACTUATOR_TYPE_LED;
    if (strcmp(type_str, "rgb_led") == 0) return ACTUATOR_TYPE_RGB_LED;
    if (strcmp(type_str, "servo") == 0) return ACTUATOR_TYPE_SERVO;
    if (strcmp(type_str, "relay") == 0) return ACTUATOR_TYPE_RELAY;
    if (strcmp(type_str, "buzzer") == 0) return ACTUATOR_TYPE_BUZZER;
    if (strcmp(type_str, "motor") == 0) return ACTUATOR_TYPE_MOTOR;
    return ACTUATOR_TYPE_UNKNOWN;
}

// Parse actuator driver from string
static actuator_driver_t parse_actuator_driver(const char *driver_str) {
    if (strcmp(driver_str, "gpio") == 0) return ACTUATOR_DRIVER_GPIO;
    if (strcmp(driver_str, "pwm") == 0) return ACTUATOR_DRIVER_PWM;
    if (strcmp(driver_str, "i2c") == 0) return ACTUATOR_DRIVER_I2C;
    if (strcmp(driver_str, "ws2812b") == 0) return ACTUATOR_DRIVER_WS2812B;
    return ACTUATOR_DRIVER_UNKNOWN;
}

// Initialize actuator system
int actuator_system_init(const char *config_file) {
    memset(g_actuators, 0, sizeof(g_actuators));
    g_actuator_count = 0;

    // Load configuration from file (JSON format)
    FILE *fp = fopen(config_file, "r");
    if (!fp) {
        return -1;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp) && g_actuator_count < MAX_ACTUATORS) {
        // Parse JSON line (simplified parsing)
        // In production, use a proper JSON parser
        if (strstr(line, "\"id\":")) {
            char *id_start = strstr(line, "\"id\":\"");
            if (id_start) {
                id_start += 6;
                char *id_end = strstr(id_start, "\"");
                if (id_end) {
                    size_t id_len = id_end - id_start;
                    memcpy(g_actuators[g_actuator_count].id, id_start, id_len);
                    g_actuators[g_actuator_count].id[id_len] = '\0';
                }
            }
        }
        // ... (parse other fields)
        g_actuators[g_actuator_count].enabled = 1;
        g_actuators[g_actuator_count].state = 0;
        g_actuators[g_actuator_count].value = 0;
        g_actuator_count++;
    }

    fclose(fp);
    return g_actuator_count;
}

// Cleanup actuator system
void actuator_system_cleanup(void) {
    memset(g_actuators, 0, sizeof(g_actuators));
    g_actuator_count = 0;
}

// List all actuators
int actuator_list(char *buffer, size_t buffer_size) {
    int pos = 0;
    pos += snprintf(buffer + pos, buffer_size - pos, "{\"actuators\":[");

    for (int i = 0; i < g_actuator_count; i++) {
        if (i > 0) {
            pos += snprintf(buffer + pos, buffer_size - pos, ",");
        }

        pos += snprintf(buffer + pos, buffer_size - pos,
            "{"
            "\"id\":\"%s\","
            "\"name\":\"%s\","
            "\"type\":%d,"
            "\"driver\":%d,"
            "\"state\":%d,"
            "\"value\":%u"
            "}",
            g_actuators[i].id,
            g_actuators[i].name,
            (int)g_actuators[i].type,
            (int)g_actuators[i].driver,
            g_actuators[i].state,
            g_actuators[i].value
        );
    }

    pos += snprintf(buffer + pos, buffer_size - pos, "]}");
    return pos;
}

// Turn on actuator
int actuator_on(const char *actuator_id) {
    actuator_config_t *actuator = actuator_get_config(actuator_id);
    if (!actuator || !actuator->enabled) {
        return -1;
    }

    // Turn on actuator (platform-specific)
    platform_actuator_write(actuator->driver, actuator->driver_params, 1);
    actuator->state = 1;
    return 0;
}

// Turn off actuator
int actuator_off(const char *actuator_id) {
    actuator_config_t *actuator = actuator_get_config(actuator_id);
    if (!actuator || !actuator->enabled) {
        return -1;
    }

    // Turn off actuator (platform-specific)
    platform_actuator_write(actuator->driver, actuator->driver_params, 0);
    actuator->state = 0;
    return 0;
}

// Set actuator value (PWM, servo angle, etc.)
int actuator_set(const char *actuator_id, uint32_t value) {
    actuator_config_t *actuator = actuator_get_config(actuator_id);
    if (!actuator || !actuator->enabled) {
        return -1;
    }

    // Set actuator value (platform-specific)
    platform_actuator_write(actuator->driver, actuator->driver_params, value);
    actuator->value = value;
    actuator->state = (value > 0) ? 1 : 0;
    return 0;
}

// Set RGB LED color
int actuator_set_rgb(const char *actuator_id, uint8_t r, uint8_t g, uint8_t b) {
    actuator_config_t *actuator = actuator_get_config(actuator_id);
    if (!actuator || !actuator->enabled) {
        return -1;
    }

    if (actuator->type != ACTUATOR_TYPE_RGB_LED) {
        return -1;
    }

    // Set RGB LED color (platform-specific)
    platform_rgb_led_write(actuator->driver_params, r, g, b);
    return 0;
}

// Set servo angle
int actuator_set_angle(const char *actuator_id, uint16_t angle) {
    actuator_config_t *actuator = actuator_get_config(actuator_id);
    if (!actuator || !actuator->enabled) {
        return -1;
    }

    if (actuator->type != ACTUATOR_TYPE_SERVO) {
        return -1;
    }

    // Set servo angle (platform-specific)
    platform_servo_write(actuator->driver_params, angle);
    actuator->value = angle;
    return 0;
}

// Beep buzzer
int actuator_beep(const char *actuator_id, uint32_t duration_ms) {
    actuator_config_t *actuator = actuator_get_config(actuator_id);
    if (!actuator || !actuator->enabled) {
        return -1;
    }

    if (actuator->type != ACTUATOR_TYPE_BUZZER) {
        return -1;
    }

    // Beep buzzer (platform-specific)
    platform_buzzer_beep(actuator->driver_params, duration_ms);
    return 0;
}

// Get actuator config
actuator_config_t *actuator_get_config(const char *actuator_id) {
    for (int i = 0; i < g_actuator_count; i++) {
        if (strcmp(g_actuators[i].id, actuator_id) == 0) {
            return &g_actuators[i];
        }
    }
    return NULL;
}

// Set actuator enabled state
int actuator_set_enabled(const char *actuator_id, int enabled) {
    actuator_config_t *actuator = actuator_get_config(actuator_id);
    if (!actuator) {
        return -1;
    }

    actuator->enabled = enabled ? 1 : 0;
    return 0;
}
