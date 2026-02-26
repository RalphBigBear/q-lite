// Q-Lite - Actuator API
// Provides REST API endpoints for controlling actuators (LEDs, servos, relays, etc.)

#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <stdint.h>

// Actuator types
typedef enum {
    ACTUATOR_TYPE_LED,
    ACTUATOR_TYPE_RGB_LED,
    ACTUATOR_TYPE_SERVO,
    ACTUATOR_TYPE_RELAY,
    ACTUATOR_TYPE_BUZZER,
    ACTUATOR_TYPE_MOTOR,
    ACTUATOR_TYPE_UNKNOWN
} actuator_type_t;

// Actuator driver types
typedef enum {
    ACTUATOR_DRIVER_GPIO,
    ACTUATOR_DRIVER_PWM,
    ACTUATOR_DRIVER_I2C,
    ACTUATOR_DRIVER_WS2812B,
    ACTUATOR_DRIVER_UNKNOWN
} actuator_driver_t;

// Actuator configuration
typedef struct {
    char id[32];
    char name[64];
    actuator_type_t type;
    actuator_driver_t driver;
    char driver_params[256];  // Pin, I2C address, etc.
    int state;                // 0 = off, 1 = on
    uint32_t value;          // PWM value, servo angle, etc.
    int enabled;              // 0 = disabled, 1 = enabled
} actuator_config_t;

// Initialize actuator system
int actuator_system_init(const char *config_file);

// Cleanup actuator system
void actuator_system_cleanup(void);

// List all actuators
int actuator_list(char *buffer, size_t buffer_size);

// Turn on actuator
int actuator_on(const char *actuator_id);

// Turn off actuator
int actuator_off(const char *actuator_id);

// Set actuator value (PWM, servo angle, etc.)
int actuator_set(const char *actuator_id, uint32_t value);

// Set RGB LED color
int actuator_set_rgb(const char *actuator_id, uint8_t r, uint8_t g, uint8_t b);

// Set servo angle
int actuator_set_angle(const char *actuator_id, uint16_t angle);

// Beep buzzer
int actuator_beep(const char *actuator_id, uint32_t duration_ms);

// Get actuator config
actuator_config_t *actuator_get_config(const char *actuator_id);

// Set actuator enabled state
int actuator_set_enabled(const char *actuator_id, int enabled);

#endif // ACTUATOR_H
