// Q-Lite - Sensor API Implementation

#include "sensor.h"
#include "platform.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_SENSORS 16
#define MAX_LINE 256

static sensor_config_t g_sensors[MAX_SENSORS];
static int g_sensor_count = 0;

// Parse sensor type from string
static sensor_type_t parse_sensor_type(const char *type_str) {
    if (strcmp(type_str, "temperature") == 0) return SENSOR_TYPE_TEMPERATURE;
    if (strcmp(type_str, "humidity") == 0) return SENSOR_TYPE_HUMIDITY;
    if (strcmp(type_str, "light") == 0) return SENSOR_TYPE_LIGHT;
    if (strcmp(type_str, "distance") == 0) return SENSOR_TYPE_DISTANCE;
    if (strcmp(type_str, "sound") == 0) return SENSOR_TYPE_SOUND;
    if (strcmp(type_str, "gas") == 0) return SENSOR_TYPE_GAS;
    if (strcmp(type_str, "motion") == 0) return SENSOR_TYPE_MOTION;
    if (strcmp(type_str, "pressure") == 0) return SENSOR_TYPE_PRESSURE;
    return SENSOR_TYPE_UNKNOWN;
}

// Parse sensor driver from string
static sensor_driver_t parse_sensor_driver(const char *driver_str) {
    if (strcmp(driver_str, "dht22") == 0) return SENSOR_DRIVER_DHT22;
    if (strcmp(driver_str, "bh1750") == 0) return SENSOR_DRIVER_BH1750;
    if (strcmp(driver_str, "hc-sr04") == 0) return SENSOR_DRIVER_HC_SR04;
    if (strcmp(driver_str, "mpu6050") == 0) return SENSOR_DRIVER_MPU6050;
    if (strcmp(driver_str, "bme280") == 0) return SENSOR_DRIVER_BME280;
    if (strcmp(driver_str, "mq135") == 0) return SENSOR_DRIVER_MQ135;
    if (strcmp(driver_str, "pir") == 0) return SENSOR_DRIVER_PIR;
    return SENSOR_DRIVER_UNKNOWN;
}

// Initialize sensor system
int sensor_system_init(const char *config_file) {
    memset(g_sensors, 0, sizeof(g_sensors));
    g_sensor_count = 0;

    // Load configuration from file (JSON format)
    FILE *fp = fopen(config_file, "r");
    if (!fp) {
        return -1;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp) && g_sensor_count < MAX_SENSORS) {
        // Parse JSON line (simplified parsing)
        // In production, use a proper JSON parser
        if (strstr(line, "\"id\":")) {
            char *id_start = strstr(line, "\"id\":\"");
            if (id_start) {
                id_start += 6;
                char *id_end = strstr(id_start, "\"");
                if (id_end) {
                    size_t id_len = id_end - id_start;
                    memcpy(g_sensors[g_sensor_count].id, id_start, id_len);
                    g_sensors[g_sensor_count].id[id_len] = '\0';
                }
            }
        }
        // ... (parse other fields)
        g_sensors[g_sensor_count].enabled = 1;
        g_sensors[g_sensor_count].value = 0.0f;
        g_sensors[g_sensor_count].last_update_ms = 0;
        g_sensor_count++;
    }

    fclose(fp);
    return g_sensor_count;
}

// Cleanup sensor system
void sensor_system_cleanup(void) {
    memset(g_sensors, 0, sizeof(g_sensors));
    g_sensor_count = 0;
}

// List all sensors
int sensor_list(char *buffer, size_t buffer_size) {
    int pos = 0;
    pos += snprintf(buffer + pos, buffer_size - pos, "{\"sensors\":[");

    for (int i = 0; i < g_sensor_count; i++) {
        if (i > 0) {
            pos += snprintf(buffer + pos, buffer_size - pos, ",");
        }

        pos += snprintf(buffer + pos, buffer_size - pos,
            "{"
            "\"id\":\"%s\","
            "\"name\":\"%s\","
            "\"type\":%d,"
            "\"driver\":%d,"
            "\"unit\":\"%s\","
            "\"value\":%.2f,"
            "\"last_update\":%u"
            "}",
            g_sensors[i].id,
            g_sensors[i].name,
            (int)g_sensors[i].type,
            (int)g_sensors[i].driver,
            g_sensors[i].unit,
            g_sensors[i].value,
            g_sensors[i].last_update_ms
        );
    }

    pos += snprintf(buffer + pos, buffer_size - pos, "]}");
    return pos;
}

// Read specific sensor
int sensor_read(const char *sensor_id, char *buffer, size_t buffer_size) {
    sensor_config_t *sensor = sensor_get_config(sensor_id);
    if (!sensor) {
        return -1;
    }

    // Read sensor value (platform-specific)
    // This is a simplified implementation
    sensor->value = platform_read_sensor(sensor->driver, sensor->driver_params);
    sensor->last_update_ms = platform_get_time_ms();

    snprintf(buffer, buffer_size,
        "{"
        "\"id\":\"%s\","
        "\"name\":\"%s\","
        "\"type\":%d,"
        "\"value\":%.2f,"
        "\"unit\":\"%s\","
        "\"timestamp\":%u"
        "}",
        sensor->id,
        sensor->name,
        (int)sensor->type,
        sensor->value,
        sensor->unit,
        sensor->last_update_ms
    );

    return 0;
}

// Read multiple sensors
int sensor_read_batch(const char **sensor_ids, int count, char *buffer, size_t buffer_size) {
    int pos = 0;
    pos += snprintf(buffer + pos, buffer_size - pos, "{\"sensors\":[");

    for (int i = 0; i < count; i++) {
        if (i > 0) {
            pos += snprintf(buffer + pos, buffer_size - pos, ",");
        }

        sensor_config_t *sensor = sensor_get_config(sensor_ids[i]);
        if (sensor) {
            sensor->value = platform_read_sensor(sensor->driver, sensor->driver_params);
            sensor->last_update_ms = platform_get_time_ms();

            pos += snprintf(buffer + pos, buffer_size - pos,
                "{"
                "\"id\":\"%s\","
                "\"value\":%.2f,"
                "\"timestamp\":%u"
                "}",
                sensor->id,
                sensor->value,
                sensor->last_update_ms
            );
        }
    }

    pos += snprintf(buffer + pos, buffer_size - pos, "]}");
    return pos;
}

// Update all sensors (called periodically)
int sensor_update_all(void) {
    uint32_t now = platform_get_time_ms();

    for (int i = 0; i < g_sensor_count; i++) {
        if (!g_sensors[i].enabled) {
            continue;
        }

        if (now - g_sensors[i].last_update_ms >= g_sensors[i].interval_ms) {
            g_sensors[i].value = platform_read_sensor(g_sensors[i].driver, g_sensors[i].driver_params);
            g_sensors[i].last_update_ms = now;
        }
    }

    return 0;
}

// Get sensor config
sensor_config_t *sensor_get_config(const char *sensor_id) {
    for (int i = 0; i < g_sensor_count; i++) {
        if (strcmp(g_sensors[i].id, sensor_id) == 0) {
            return &g_sensors[i];
        }
    }
    return NULL;
}

// Set sensor enabled state
int sensor_set_enabled(const char *sensor_id, int enabled) {
    sensor_config_t *sensor = sensor_get_config(sensor_id);
    if (!sensor) {
        return -1;
    }

    sensor->enabled = enabled ? 1 : 0;
    return 0;
}
