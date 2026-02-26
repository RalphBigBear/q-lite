// Q-Lite - Sensor API
// Provides REST API endpoints for reading sensor data

#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

// Sensor types
typedef enum {
    SENSOR_TYPE_TEMPERATURE,
    SENSOR_TYPE_HUMIDITY,
    SENSOR_TYPE_LIGHT,
    SENSOR_TYPE_DISTANCE,
    SENSOR_TYPE_SOUND,
    SENSOR_TYPE_GAS,
    SENSOR_TYPE_MOTION,
    SENSOR_TYPE_PRESSURE,
    SENSOR_TYPE_UNKNOWN
} sensor_type_t;

// Sensor driver types
typedef enum {
    SENSOR_DRIVER_DHT22,
    SENSOR_DRIVER_BH1750,
    SENSOR_DRIVER_HC_SR04,
    SENSOR_DRIVER_MPU6050,
    SENSOR_DRIVER_BME280,
    SENSOR_DRIVER_MQ135,
    SENSOR_DRIVER_PIR,
    SENSOR_DRIVER_UNKNOWN
} sensor_driver_t;

// Sensor configuration
typedef struct {
    char id[32];
    char name[64];
    sensor_type_t type;
    sensor_driver_t driver;
    char driver_params[256];  // Pin, I2C address, etc.
    uint32_t interval_ms;       // Sampling interval
    char unit[16];            // Unit (C, %, lux, cm, etc.)
    float value;               // Current value
    uint32_t last_update_ms;   // Last update time
    int enabled;               // 0 = disabled, 1 = enabled
} sensor_config_t;

// Initialize sensor system
int sensor_system_init(const char *config_file);

// Cleanup sensor system
void sensor_system_cleanup(void);

// List all sensors
int sensor_list(char *buffer, size_t buffer_size);

// Read specific sensor
int sensor_read(const char *sensor_id, char *buffer, size_t buffer_size);

// Read multiple sensors
int sensor_read_batch(const char **sensor_ids, int count, char *buffer, size_t buffer_size);

// Update all sensors (called periodically)
int sensor_update_all(void);

// Get sensor config
sensor_config_t *sensor_get_config(const char *sensor_id);

// Set sensor enabled state
int sensor_set_enabled(const char *sensor_id, int enabled);

#endif // SENSOR_H
