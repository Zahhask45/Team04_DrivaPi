/**
 * @file dc_motor.h
 * @brief Mock for dc_motor.h (unit testing)  
 */

#ifndef __DC_MOTOR_H
#define __DC_MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32_hal_i2c.h"

/* PCA9685 Constants */
#define PCA9685_ADDR_MOTOR 0x60
#define MOTOR_L_PWM 7
#define MOTOR_L_A 5
#define MOTOR_L_B 6
#define MOTOR_R_PWM 0
#define MOTOR_R_A 1
#define MOTOR_R_B 2
#define PCA9685_COUNTS 4096u
#define MOTOR_I2C hi2c3

extern I2C_HandleTypeDef hi2c3;

/* Function prototypes */
void Motor_SetMotor(double value);
void Motor_SetPWM(int32_t left_counts, int32_t right_counts);
void Motor_Stop(void);
void Motor_Forward(double speed);
void Motor_Backward(double speed);
void Motor_Left(double speed);
void Motor_Right(double speed);

#ifdef __cplusplus
}
#endif

#endif /* __DC_MOTOR_H */
