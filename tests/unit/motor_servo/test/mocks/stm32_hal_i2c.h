/**
 * @file stm32_hal_i2c.h
 * @brief STM32 HAL I2C Mock Header for Unit Testing
 * @note Simplified HAL for testing without real STM32 hardware
 */

#ifndef STM32_HAL_I2C_H
#define STM32_HAL_I2C_H

#include <stdint.h>

/* HAL Status Definitions */
typedef enum {
    HAL_OK       = 0x00U,
    HAL_ERROR    = 0x01U,
    HAL_BUSY     = 0x02U,
    HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

/* I2C State Definitions (Simplified) */
#define HAL_I2C_STATE_READY 0x00000001U

/* I2C Handle Structure (Simplified) */
typedef struct {
    void*       Instance;
    uint32_t    Timing;
    uint32_t    OwnAddress1;
    uint32_t    AddressingMode;
    uint32_t    ErrorCode;
    uint32_t    State;
} I2C_HandleTypeDef;

/* HAL I2C Functions */
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c,
                                          uint16_t DevAddress,
                                          uint8_t *pData,
                                          uint16_t Size,
                                          uint32_t Timeout);

HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c,
                                         uint16_t DevAddress,
                                         uint8_t *pData,
                                         uint16_t Size,
                                         uint32_t Timeout);

HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c,
                                    uint16_t DevAddress,
                                    uint16_t MemAddress,
                                    uint16_t MemAddSize,
                                    uint8_t *pData,
                                    uint16_t Size,
                                    uint32_t Timeout);

HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c,
                                   uint16_t DevAddress,
                                   uint16_t MemAddress,
                                   uint16_t MemAddSize,
                                   uint8_t *pData,
                                   uint16_t Size,
                                   uint32_t Timeout);

#endif /* STM32_HAL_I2C_H */
