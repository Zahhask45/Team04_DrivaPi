/* Simplified PCA9685 header for unit testing */
#ifndef PCA9685_TESTABLE_H
#define PCA9685_TESTABLE_H

#include "../test/mocks/stm32_hal_i2c.h"
#include <stdint.h>

#define LED0_ON_L 0x06
#define PCA9685_COUNTS 4096u
#define I2C_MEMADD_SIZE_8BIT 1

HAL_StatusTypeDef PCA9685_SetPWM_Testable(I2C_HandleTypeDef *hi2c, uint16_t addr,
                                           uint8_t channel, uint16_t on, uint16_t off);

#endif
