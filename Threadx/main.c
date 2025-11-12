#include "main.h"

//==============================
// APPLICATION DEFINE
// ==============================
void tx_application_define(void *first_unused_memory)
{
    CHAR *memory_ptr = (CHAR*)first_unused_memory;

    // Create queues
    tx_queue_create(&queue_speed_cmd, "Speed Queue", sizeof(CAN_MESSAGE)/sizeof(ULONG),
    memory_ptr, QUEUE_SIZE * sizeof(CAN_MESSAGE));
    memory_ptr += QUEUE_SIZE * sizeof(CAN_MESSAGE);

    tx_queue_create(&queue_steer_cmd, "Steering Queue", sizeof(CAN_MESSAGE)/sizeof(ULONG),
    memory_ptr, QUEUE_SIZE * sizeof(CAN_MESSAGE));
    memory_ptr += QUEUE_SIZE * sizeof(CAN_MESSAGE);

    // Create event flags
    tx_event_flags_create(&event_flags, "System Events");

    // Create mutex
    tx_mutex_create(&speed_data_mutex, "Speed Mutex", TX_NO_INHERIT);

    // Create threads
    tx_thread_create(&thread_can_rx, "CAN RX Thread", thread_can_rx_entry, 0,
    stack_can_rx, THREAD_STACK_SIZE, 2, 2, TX_NO_TIME_SLICE, TX_AUTO_START);

    tx_thread_create(&thread_speed, "Speed Control Thread", thread_speed_entry, 0,
    stack_speed, THREAD_STACK_SIZE, 4, 4, TX_NO_TIME_SLICE, TX_AUTO_START);

    tx_thread_create(&thread_steering, "Steering Control Thread", thread_steering_entry, 0,
    stack_steering, THREAD_STACK_SIZE, 5, 5, TX_NO_TIME_SLICE, TX_AUTO_START);

    tx_thread_create(&thread_sensor, "Sensor Thread", thread_sensor_entry, 0,
    stack_sensor, THREAD_STACK_SIZE, 6, 6, TX_NO_TIME_SLICE, TX_AUTO_START);

    tx_thread_create(&thread_can_tx, "CAN TX Thread", thread_can_tx_entry, 0,
    stack_can_tx, THREAD_STACK_SIZE, 7, 7, TX_NO_TIME_SLICE, TX_AUTO_START);
}

// ==============================
// THREAD IMPLEMENTATIONS
// ==============================

// -------- CAN RECEIVE THREAD --------
void thread_can_rx_entry(ULONG thread_input)
{
    CAN_MESSAGE msg;

    while (1)
    {
        // Blocking receive (driver-specific)
        if (can_receive(&msg))
        {
            if (msg.id == CMD_SPEED)
            {
                tx_queue_send(&queue_speed_cmd, &msg, TX_NO_WAIT);
                tx_event_flags_set(&event_flags, FLAG_CAN_SPEED_CMD, TX_OR);
            }
            else if (msg.id == CMD_STEERING)
            {
                tx_queue_send(&queue_steer_cmd, &msg, TX_NO_WAIT);
                tx_event_flags_set(&event_flags, FLAG_CAN_STEER_CMD, TX_OR);
            }
        }
    }
}

// -------- SPEED CONTROL THREAD --------
void thread_speed_entry(ULONG thread_input)
{
    CAN_MESSAGE msg;
    ULONG actual_flags;

    while (1)
    {
        // Wait until a speed command event is set
        tx_event_flags_get(&event_flags, FLAG_CAN_SPEED_CMD,
        TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);

        // Process queued messages
        while (tx_queue_receive(&queue_speed_cmd, &msg, TX_NO_WAIT) == TX_SUCCESS)
        {
            float target_speed = parse_speed_cmd(&msg);
            motor_set_speed(target_speed);
        }
    }
}

// -------- STEERING CONTROL THREAD --------
void thread_steering_entry(ULONG thread_input)
{
    CAN_MESSAGE msg;
    ULONG actual_flags;

    while (1)
    {
        // Wait until a steering command event is set
        tx_event_flags_get(&event_flags, FLAG_CAN_STEER_CMD,
        TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);

        while (tx_queue_receive(&queue_steer_cmd, &msg, TX_NO_WAIT) == TX_SUCCESS)
        {
            float target_angle = parse_steer_cmd(&msg);
            motor_set_steering(target_angle);
        }
    }
}

// -------- SENSOR THREAD --------
// Assume this thread is triggered by an interrupt (e.g. timer)
void thread_sensor_entry(ULONG thread_input)
{
    while (1)
    {
        // Wait for an external signal from ISR
        ULONG actual_flags;
        tx_event_flags_get(&event_flags, FLAG_SENSOR_UPDATE,
        TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);

        float current_speed = read_speed_sensor();

        tx_mutex_get(&speed_data_mutex, TX_WAIT_FOREVER);
        g_vehicle_speed = current_speed;
        tx_mutex_put(&speed_data_mutex);

        // Notify CAN TX thread that new data is ready
        tx_event_flags_set(&event_flags, FLAG_SENSOR_UPDATE, TX_OR);
    }
}

// -------- CAN TRANSMIT THREAD --------
void thread_can_tx_entry(ULONG thread_input)
{
    CAN_MESSAGE msg;
    ULONG actual_flags;

    while (1)
    {
        // Wait until sensor thread signals new data
        tx_event_flags_get(&event_flags, FLAG_SENSOR_UPDATE,
        TX_OR_CLEAR, &actual_flags, TX_WAIT_FOREVER);

        tx_mutex_get(&speed_data_mutex, TX_WAIT_FOREVER);
        float speed = g_vehicle_speed;
        tx_mutex_put(&speed_data_mutex);

        make_speed_status_msg(&msg, speed);
        can_send(&msg);
    }
}

// ==============================
// ISR HOOKS OR CALLBACKS
// ==============================

// Example: Timer ISR triggers sensor update
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    // This ISR signals the sensor thread
    tx_event_flags_set(&event_flags, FLAG_SENSOR_UPDATE, TX_OR);
}
