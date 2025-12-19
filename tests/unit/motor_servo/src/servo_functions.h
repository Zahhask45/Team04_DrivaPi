#ifndef SERVO_FUNCTIONS_H
#define SERVO_FUNCTIONS_H

#include <stdint.h>

#define PCA9685_COUNTS 4096u
#define SERVO_MIN_PULSE 200u
#define SERVO_MAX_PULSE 500u

int Servo_ValidateAngle(uint16_t angle_deg);
int Servo_ValidatePulseRange(uint16_t min_pulse, uint16_t max_pulse);
uint16_t Servo_CalculatePulse(uint16_t angle_deg, uint16_t min_pulse, uint16_t max_pulse);
uint16_t Servo_ClampAngle(uint16_t angle_deg);
uint16_t Servo_MapAngleToPulse(uint8_t angle_percent);

#endif
