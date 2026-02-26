// Q-Lite - Rule Engine
// Provides semantic rule orchestration for sensor-actuator automation

#ifndef RULE_H
#define RULE_H

#include <stdint.h>

// Condition operators
typedef enum {
    RULE_OP_GT,         // Greater than
    RULE_OP_LT,         // Less than
    RULE_OP_EQ,         // Equal to
    RULE_OP_NE,         // Not equal to
    RULE_OP_GTE,        // Greater than or equal to
    RULE_OP_LTE,        // Less than or equal to
    RULE_OP_UNKNOWN
} rule_operator_t;

// Condition types
typedef enum {
    RULE_CONDITION_THRESHOLD,      // Sensor threshold condition
    RULE_CONDITION_RANGE,          // Sensor range condition
    RULE_CONDITION_TIME,          // Time-based condition
    RULE_CONDITION_AND,          // Logical AND
    RULE_CONDITION_OR,           // Logical OR
    RULE_CONDITION_UNKNOWN
} rule_condition_type_t;

// Action types
typedef enum {
    RULE_ACTION_ACTUATOR,       // Control actuator
    RULE_ACTION_DELAY,           // Delay execution
    RULE_ACTION_LOG,            // Log message
    RULE_ACTION_NOTIFICATION,    // Send notification
    RULE_ACTION_UNKNOWN
} rule_action_type_t;

// Rule condition
typedef struct {
    rule_condition_type_t type;
    char sensor_id[32];
    rule_operator_t operator;
    float value;
    float value2;              // For range conditions
    struct rule_condition *and_condition;
    struct rule_condition *or_condition;
} rule_condition_t;

// Rule action
typedef struct {
    rule_action_type_t type;
    char target_id[32];         // Actuator ID or message
    char command[64];           // on/off/set/beep
    union {
        float value;             // For set commands
        uint32_t duration_ms;   // For delay/beep
        struct {
            uint8_t r, g, b;   // For RGB LED
        } rgb;
    };
} rule_action_t;

// Rule definition
typedef struct {
    char id[32];
    char name[64];
    int enabled;
    rule_condition_t condition;
    rule_action_t actions[16];
    int action_count;
    uint32_t created_at;
    uint32_t triggered_count;
    uint32_t last_triggered_ms;
} rule_t;

// Initialize rule system
int rule_system_init(const char *config_file);

// Cleanup rule system
void rule_system_cleanup(void);

// List all rules
int rule_list(char *buffer, size_t buffer_size);

// Add rule (from JSON or LLM-generated)
int rule_add(const char *rule_json);

// Remove rule
int rule_remove(const char *rule_id);

// Enable/disable rule
int rule_set_enabled(const char *rule_id, int enabled);

// Evaluate rule (called periodically)
int rule_evaluate(rule_t *rule);

// Evaluate all rules
int rule_evaluate_all(void);

// Get rule by ID
rule_t *rule_get(const char *rule_id);

#endif // RULE_H
