#ifndef STM32U5xx_HAL_H
#define STM32U5xx_HAL_H

#include <stdint.h>

// 1. Basic Types
typedef uint32_t uint32_t;
typedef uint8_t  uint8_t;

// 2. Hardware Register Structs
typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CNT;
} TIM_TypeDef;

typedef struct
{
    volatile uint32_t DR;
} USART_TypeDef;

// 3. Handle Structs
typedef struct
{
    TIM_TypeDef *Instance;
} TIM_HandleTypeDef;

typedef struct
{
    USART_TypeDef *Instance;
} UART_HandleTypeDef;

// 4. Function Prototypes
void HAL_TIM_Base_Start(TIM_HandleTypeDef *htim);
void HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim);
void HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout);

// 5. Macros
#define __HAL_RCC_TIM1_CLK_ENABLE()  ((void)0)

#endif
