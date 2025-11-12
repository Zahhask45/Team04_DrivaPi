#ifndef MAIN_H
#define MAIN_H


#include "tx_api.h"
#include "can_driver.h"
#include "motor_driver.h"
#include "sensors.h"

// ==============================
// CONFIGURATION
// ==============================
#define THREAD_STACK_SIZE     1024
#define QUEUE_SIZE            10

// CAN Command IDs
#define CMD_SPEED             0x101
#define CMD_STEERING          0x102

// Event flag bits
#define FLAG_CAN_SPEED_CMD    (1 << 0)
#define FLAG_CAN_STEER_CMD    (1 << 1)
#define FLAG_SENSOR_UPDATE    (1 << 2)

// ==============================
// GLOBAL OBJECTS
// ==============================
TX_THREAD               thread_can_rx;
TX_THREAD               thread_can_tx;
TX_THREAD               thread_speed;
TX_THREAD               thread_steering;
TX_THREAD               thread_sensor;

TX_QUEUE                queue_speed_cmd;
TX_QUEUE                queue_steer_cmd;
TX_EVENT_FLAGS_GROUP    event_flags;
TX_MUTEX                speed_data_mutex;

// Shared data
float g_vehicle_speed = 0.0f;

// Thread stacks
ULONG stack_can_rx[THREAD_STACK_SIZE / sizeof(ULONG)];
ULONG stack_can_tx[THREAD_STACK_SIZE / sizeof(ULONG)];
ULONG stack_speed[THREAD_STACK_SIZE / sizeof(ULONG)];
ULONG stack_steering[THREAD_STACK_SIZE / sizeof(ULONG)];
ULONG stack_sensor[THREAD_STACK_SIZE / sizeof(ULONG)];

// CAN message structure
typedef struct {
    uint32_t id;
    uint8_t  data[8];
    uint8_t  len;
} CAN_MESSAGE;

// ==============================
// THREAD DECLARATIONS
// ==============================
void thread_can_rx_entry(ULONG thread_input);
void thread_can_tx_entry(ULONG thread_input);
void thread_speed_entry(ULONG thread_input);
void thread_steering_entry(ULONG thread_input);
void thread_sensor_entry(ULONG thread_input);

#endif