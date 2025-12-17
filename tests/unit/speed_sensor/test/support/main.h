#ifndef MAIN_H
#define MAIN_H

// If your code uses global flags/mutexes from main.h, declare them extern here
#include "app_threadx.h"

// Example of globals usually found in main
extern TX_MUTEX speed_data_mutex;
extern TX_EVENT_FLAGS_GROUP event_flags;
extern float g_vehicle_speed;

// Defines
#define FLAG_SENSOR_UPDATE  0x01

#endif
