#include "servo_functions.h"

static inline uint16_t clamp_u16(int32_t v) {
    return (uint16_t)v;
}

int Servo_ValidateAngle(uint16_t angle_deg) {
    if (angle_deg > 180u) return -1;
    return 0;
}

int Servo_ValidatePulseRange(uint16_t min_pulse, uint16_t max_pulse) {
    if (min_pulse >= max_pulse) return -1;
    if (max_pulse >= PCA9685_COUNTS) return -1;
    return 0;
}

uint16_t Servo_CalculatePulse(uint16_t angle_deg, uint16_t min_pulse, uint16_t max_pulse) {
    if (angle_deg > 180u) angle_deg = 180u;
    
    uint32_t range = (uint32_t)max_pulse - (uint32_t)min_pulse;
    uint32_t pulse = (uint32_t)min_pulse + (range * angle_deg) / 180u;
    
    return clamp_u16((int32_t)pulse);
}

uint16_t Servo_ClampAngle(uint16_t angle_deg) {
    if (angle_deg > 180u) return 180u;
    return angle_deg;
}

uint16_t Servo_MapAngleToPulse(uint8_t angle_percent) {
    if (angle_percent > 100u) angle_percent = 100u;
    
    uint32_t range = (uint32_t)SERVO_MAX_PULSE - (uint32_t)SERVO_MIN_PULSE;
    uint32_t pulse = (uint32_t)SERVO_MIN_PULSE + (range * angle_percent) / 100u;
    
    return (uint16_t)pulse;
}
