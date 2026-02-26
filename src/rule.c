// Q-Lite - Rule Engine Implementation

#include "rule.h"
#include "sensor.h"
#include "actuator.h"
#include "platform.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_RULES 16
#define MAX_LINE 2048

static rule_t g_rules[MAX_RULES];
static int g_rule_count = 0;

// Initialize rule system
int rule_system_init(const char *config_file) {
    memset(g_rules, 0, sizeof(g_rules));
    g_rule_count = 0;

    // Load configuration from file (JSON format)
    FILE *fp = fopen(config_file, "r");
    if (!fp) {
        return -1;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp) && g_rule_count < MAX_RULES) {
        // Parse JSON line (simplified parsing)
        // In production, use a proper JSON parser
        if (strstr(line, "\"id\":")) {
            char *id_start = strstr(line, "\"id\":\"");
            if (id_start) {
                id_start += 6;
                char *id_end = strstr(id_start, "\"");
                if (id_end) {
                    size_t id_len = id_end - id_start;
                    memcpy(g_rules[g_rule_count].id, id_start, id_len);
                    g_rules[g_rule_count].id[id_len] = '\0';
                }
            }
        }
        if (strstr(line, "\"name\":")) {
            char *name_start = strstr(line, "\"name\":\"");
            if (name_start) {
                name_start += 8;
                char *name_end = strstr(name_start, "\"");
                if (name_end) {
                    size_t name_len = name_end - name_start;
                    memcpy(g_rules[g_rule_count].name, name_start, name_len);
                    g_rules[g_rule_count].name[name_len] = '\0';
                }
            }
        }
        // ... (parse other fields: condition, actions)
        g_rules[g_rule_count].enabled = 1;
        g_rules[g_rule_count].triggered_count = 0;
        g_rules[g_rule_count].last_triggered_ms = 0;
        g_rules[g_rule_count].created_at = platform_get_time_ms();
        g_rule_count++;
    }

    fclose(fp);
    return g_rule_count;
}

// Cleanup rule system
void rule_system_cleanup(void) {
    memset(g_rules, 0, sizeof(g_rules));
    g_rule_count = 0;
}

// List all rules
int rule_list(char *buffer, size_t buffer_size) {
    int pos = 0;
    pos += snprintf(buffer + pos, buffer_size - pos, "{\"rules\":[");

    for (int i = 0; i < g_rule_count; i++) {
        if (i > 0) {
            pos += snprintf(buffer + pos, buffer_size - pos, ",");
        }

        pos += snprintf(buffer + pos, buffer_size - pos,
            "{"
            "\"id\":\"%s\","
            "\"name\":\"%s\","
            "\"enabled\":%d,"
            "\"condition\":{\"type\":%d,\"sensor\":\"%s\",\"operator\":%d,\"value\":%.2f},"
            "\"triggered_count\":%u,"
            "\"last_triggered\":%u"
            "}",
            g_rules[i].id,
            g_rules[i].name,
            g_rules[i].enabled,
            (int)g_rules[i].condition.type,
            g_rules[i].condition.sensor_id,
            (int)g_rules[i].condition.operator,
            g_rules[i].condition.value,
            g_rules[i].triggered_count,
            g_rules[i].last_triggered_ms
        );
    }

    pos += snprintf(buffer + pos, buffer_size - pos, "]}");
    return pos;
}

// Add rule (from JSON or LLM-generated)
int rule_add(const char *rule_json) {
    if (g_rule_count >= MAX_RULES) {
        return -1;
    }

    // Parse JSON (simplified parsing)
    // In production, use a proper JSON parser
    rule_t *rule = &g_rules[g_rule_count];
    memset(rule, 0, sizeof(rule_t));

    // Parse ID
    char *id_start = strstr(rule_json, "\"id\":");
    if (id_start) {
        id_start = strstr(id_start + 5, "\"") + 1;
        char *id_end = strstr(id_start, "\"");
        if (id_end) {
            size_t id_len = id_end - id_start;
            memcpy(rule->id, id_start, id_len);
            rule->id[id_len] = '\0';
        }
    }

    // Parse name
    char *name_start = strstr(rule_json, "\"name\":");
    if (name_start) {
        name_start = strstr(name_start + 7, "\"") + 1;
        char *name_end = strstr(name_start, "\"");
        if (name_end) {
            size_t name_len = name_end - name_start;
            memcpy(rule->name, name_start, name_len);
            rule->name[name_len] = '\0';
        }
    }

    // Parse condition
    char *condition_start = strstr(rule_json, "\"condition\":");
    if (condition_start) {
        char *sensor_start = strstr(condition_start, "\"sensor\":\"");
        if (sensor_start) {
            sensor_start += 10;
            char *sensor_end = strstr(sensor_start, "\"");
            if (sensor_end) {
                size_t sensor_len = sensor_end - sensor_start;
                memcpy(rule->condition.sensor_id, sensor_start, sensor_len);
                rule->condition.sensor_id[sensor_len] = '\0';
            }
        }

        char *op_start = strstr(condition_start, "\"operator\":");
        if (op_start) {
            // Parse operator (simplified)
            if (strstr(op_start, ">")) rule->condition.operator = RULE_OP_GT;
            else if (strstr(op_start, "<")) rule->condition.operator = RULE_OP_LT;
            else if (strstr(op_start, "==")) rule->condition.operator = RULE_OP_EQ;
            else rule->condition.operator = RULE_OP_UNKNOWN;
        }

        char *value_start = strstr(condition_start, "\"value\":");
        if (value_start) {
            rule->condition.value = atof(value_start + 8);
        }
    }

    rule->enabled = 1;
    rule->triggered_count = 0;
    rule->last_triggered_ms = 0;
    rule->created_at = platform_get_time_ms();

    g_rule_count++;
    return 0;
}

// Remove rule
int rule_remove(const char *rule_id) {
    for (int i = 0; i < g_rule_count; i++) {
        if (strcmp(g_rules[i].id, rule_id) == 0) {
            // Remove rule by shifting remaining rules
            for (int j = i; j < g_rule_count - 1; j++) {
                memcpy(&g_rules[j], &g_rules[j + 1], sizeof(rule_t));
            }
            g_rule_count--;
            return 0;
        }
    }
    return -1;
}

// Enable/disable rule
int rule_set_enabled(const char *rule_id, int enabled) {
    rule_t *rule = rule_get(rule_id);
    if (!rule) {
        return -1;
    }

    rule->enabled = enabled ? 1 : 0;
    return 0;
}

// Evaluate condition
static int evaluate_condition(rule_condition_t *condition) {
    sensor_config_t *sensor = sensor_get_config(condition->sensor_id);
    if (!sensor) {
        return 0;
    }

    float sensor_value = sensor->value;

    switch (condition->operator) {
        case RULE_OP_GT:
            return sensor_value > condition->value;
        case RULE_OP_LT:
            return sensor_value < condition->value;
        case RULE_OP_EQ:
            return sensor_value == condition->value;
        case RULE_OP_NE:
            return sensor_value != condition->value;
        case RULE_OP_GTE:
            return sensor_value >= condition->value;
        case RULE_OP_LTE:
            return sensor_value <= condition->value;
        default:
            return 0;
    }
}

// Execute action
static void execute_action(rule_action_t *action) {
    switch (action->type) {
        case RULE_ACTION_ACTUATOR:
            if (strcmp(action->command, "on") == 0) {
                actuator_on(action->target_id);
            } else if (strcmp(action->command, "off") == 0) {
                actuator_off(action->target_id);
            } else if (strcmp(action->command, "set") == 0) {
                actuator_set(action->target_id, (uint32_t)action->value);
            }
            break;

        case RULE_ACTION_DELAY:
            platform_delay_ms(action->duration_ms);
            break;

        case RULE_ACTION_LOG:
            printf("[RULE] %s\n", action->target_id);
            break;

        case RULE_ACTION_NOTIFICATION:
            // TODO: Send notification (e.g., MQTT, HTTP)
            break;

        default:
            break;
    }
}

// Evaluate rule
int rule_evaluate(rule_t *rule) {
    if (!rule->enabled) {
        return 0;
    }

    if (!evaluate_condition(&rule->condition)) {
        return 0;
    }

    // Condition met, execute actions
    for (int i = 0; i < rule->action_count; i++) {
        execute_action(&rule->actions[i]);
    }

    rule->triggered_count++;
    rule->last_triggered_ms = platform_get_time_ms();
    return 1;
}

// Evaluate all rules
int rule_evaluate_all(void) {
    int triggered = 0;

    for (int i = 0; i < g_rule_count; i++) {
        if (rule_evaluate(&g_rules[i])) {
            triggered++;
        }
    }

    return triggered;
}

// Get rule by ID
rule_t *rule_get(const char *rule_id) {
    for (int i = 0; i < g_rule_count; i++) {
        if (strcmp(g_rules[i].id, rule_id) == 0) {
            return &g_rules[i];
        }
    }
    return NULL;
}
