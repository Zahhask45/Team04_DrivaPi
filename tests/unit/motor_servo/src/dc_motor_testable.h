/* Testable DC Motor functions from ThreadX */
#ifndef DC_MOTOR_TESTABLE_H
#define DC_MOTOR_TESTABLE_H

#include "../test/mocks/stm32_hal_i2c.h"
#include "pca9685_testable.h"
#include <stdint.h>
#include <math.h>

#define PCA9685_ADDR_MOTOR 0x60
#define MOTOR_L_PWM 7
#define MOTOR_L_A 5
#define MOTOR_L_B 6
#define MOTOR_R_PWM 0
#define MOTOR_R_A 1
#define MOTOR_R_B 2

void Motor_Stop_Testable(I2C_HandleTypeDef *hi2c);
void Motor_Forward_Testable(I2C_HandleTypeDef *hi2c, double speed);
void Motor_Backward_Testable(I2C_HandleTypeDef *hi2c, double speed);
void Motor_Left_Testable(I2C_HandleTypeDef *hi2c, double speed);
void Motor_Right_Testable(I2C_HandleTypeDef *hi2c, double speed);
void Motor_SetMotor_Testable(I2C_HandleTypeDef *hi2c, double value);
void Motor_SetPWM_Testable(I2C_HandleTypeDef *hi2c, int32_t left_counts, int32_t right_counts);

#endif
