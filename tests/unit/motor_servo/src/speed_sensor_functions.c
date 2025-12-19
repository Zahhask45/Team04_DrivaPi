#include "speed_sensor_functions.h"

#define WHEEL_PERIMETER_M   0.212f
#define PULSES_PER_REV      30.0f
#define TIMER_PERIOD        65535

float Speed_CalculateRotations(uint32_t pulses) {
    return (float)pulses / PULSES_PER_REV;
}

float Speed_CalculateDistance(float rotations) {
    return rotations * WHEEL_PERIMETER_M;
}

float Speed_CalculateVelocity(float distance_m, float time_s) {
    if (time_s <= 0.001f) return 0.0f;
    return distance_m / time_s;
}

uint32_t Speed_CalculatePulses(uint32_t current_count, uint32_t last_count) {
    if (current_count >= last_count) {
        return current_count - last_count;
    } else {
        return (TIMER_PERIOD - last_count) + current_count + 1;
    }
}

int Speed_ValidateTimeDelta(float dt) {
    if (dt <= 0.001f) return -1;
    if (dt > 10.0f) return -1;
    return 0;
}

float Speed_TimeTicksToSeconds(uint32_t ticks, uint32_t ticks_per_second) {
    return (float)ticks / (float)ticks_per_second;
}

float Speed_CalculateSpeedMPS(uint32_t pulses, float time_s) {
    if (time_s <= 0.001f) return 0.0f;
    
    float rotations = Speed_CalculateRotations(pulses);
    float distance = Speed_CalculateDistance(rotations);
    return Speed_CalculateVelocity(distance, time_s);
}
