/* Testable Servo Motor functions from ThreadX */
#ifndef SERVO_MOTOR_TESTABLE_H
#define SERVO_MOTOR_TESTABLE_H

#include "../test/mocks/stm32_hal_i2c.h"
#include "pca9685_testable.h"
#include <stdint.h>

#define PCA9685_ADDR_SERVO 0x40u
#define SERVO_CH 0u
#define SERVO_DEFAULT_MIN_PULSE_COUNTS 200u
#define SERVO_DEFAULT_MAX_PULSE_COUNTS 500u

int Servo_SetAngle_Testable(I2C_HandleTypeDef *hi2c,
                             uint8_t addr7,
                             uint8_t channel,
                             uint16_t angle_deg,
                             uint16_t min_pulse_counts,
                             uint16_t max_pulse_counts);

#endif
