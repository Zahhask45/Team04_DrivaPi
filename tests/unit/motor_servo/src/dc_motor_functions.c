#include "dc_motor_functions.h"
#include <stdint.h>

static inline uint16_t clamp_u16(int32_t v) {
    if (v < 0) return 0;
    if (v >= (int32_t)PCA9685_COUNTS) return (uint16_t)(PCA9685_COUNTS - 1u);
    return (uint16_t)v;
}

static inline uint16_t sub_sat_u16(uint16_t a, uint16_t b) {
    if (b >= a) return 0;
    return (uint16_t)(a - b);
}

uint16_t DC_Motor_Clamp(int32_t value) {
    return clamp_u16(value);
}

uint16_t DC_Motor_SubSaturate(uint16_t a, uint16_t b) {
    return sub_sat_u16(a, b);
}

int DC_Motor_ValidatePWM(int32_t left_counts, int32_t right_counts) {
    if (left_counts < -((int32_t)PCA9685_COUNTS - 1)) return -1;
    if (left_counts > ((int32_t)PCA9685_COUNTS - 1)) return -1;
    if (right_counts < -((int32_t)PCA9685_COUNTS - 1)) return -1;
    if (right_counts > ((int32_t)PCA9685_COUNTS - 1)) return -1;
    return 0;
}

uint16_t DC_Motor_CalculatePWM(double speed) {
    const uint16_t max = (uint16_t)(PCA9685_COUNTS - 1u);
    if (speed < 0.0) speed = 0.0;
    if (speed > 1.0) speed = 1.0;
    return (uint16_t)(speed * max);
}

motor_direction_t DC_Motor_GetDirection(double value) {
    if (value > 0.001) return MOTOR_FORWARD;
    if (value < -0.001) return MOTOR_BACKWARD;
    return MOTOR_STOP;
}

double DC_Motor_AbsoluteSpeed(double value) {
    if (value < 0.0) return -value;
    return value;
}
