#ifndef SPEED_SENSOR_FUNCTIONS_H
#define SPEED_SENSOR_FUNCTIONS_H

#include <stdint.h>

float Speed_CalculateRotations(uint32_t pulses);
float Speed_CalculateDistance(float rotations);
float Speed_CalculateVelocity(float distance_m, float time_s);
uint32_t Speed_CalculatePulses(uint32_t current_count, uint32_t last_count);
int Speed_ValidateTimeDelta(float dt);
float Speed_TimeTicksToSeconds(uint32_t ticks, uint32_t ticks_per_second);
float Speed_CalculateSpeedMPS(uint32_t pulses, float time_s);

#endif
