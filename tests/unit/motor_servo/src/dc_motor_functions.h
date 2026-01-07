#ifndef DC_MOTOR_FUNCTIONS_H
#define DC_MOTOR_FUNCTIONS_H

#include <stdint.h>

#define PCA9685_COUNTS 4096u

typedef enum {
    MOTOR_STOP = 0,
    MOTOR_FORWARD,
    MOTOR_BACKWARD
} motor_direction_t;

uint16_t DC_Motor_Clamp(int32_t value);
uint16_t DC_Motor_SubSaturate(uint16_t a, uint16_t b);
int DC_Motor_ValidatePWM(int32_t left_counts, int32_t right_counts);
uint16_t DC_Motor_CalculatePWM(double speed);
motor_direction_t DC_Motor_GetDirection(double value);
double DC_Motor_AbsoluteSpeed(double value);

#endif
