/* Simplified PCA9685 for unit testing */
#include "pca9685_testable.h"
#include <string.h>

HAL_StatusTypeDef PCA9685_SetPWM_Testable(I2C_HandleTypeDef *hi2c, uint16_t addr, 
                                           uint8_t channel, uint16_t on, uint16_t off) {
    uint8_t data[4];
    uint8_t reg = LED0_ON_L + 4 * channel;
    uint16_t devaddr = (uint16_t)(addr << 1);
    
    data[0] = on & 0xFF;
    data[1] = (on >> 8) & 0xFF;
    data[2] = off & 0xFF;
    data[3] = (off >> 8) & 0xFF;

    return HAL_I2C_Mem_Write(hi2c, devaddr, reg, 1, data, 4, 500);
}
