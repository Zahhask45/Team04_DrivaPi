/**
 * @file main.h
 * @brief Mock header for main.h (unit testing)
 * @note Simplified version without STM32 HAL dependencies
 */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include test-compatible HAL */
#include "stm32_hal_i2c.h"

/* Exported types (from original main.h) */
typedef struct {
    void* Instance;
} UART_HandleTypeDef;

/* Exported constants */
extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c3;

/* Exported functions prototypes */
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
