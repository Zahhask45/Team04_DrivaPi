/* Mock header for STM32 HAL for unit testing */
#ifndef MOCK_STM32U5XX_HAL_H
#define MOCK_STM32U5XX_HAL_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* HAL Status typedef */
typedef enum {
    HAL_OK       = 0x00U,
    HAL_ERROR    = 0x01U,
    HAL_BUSY     = 0x02U,
    HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

/* I2C State typedef */
typedef enum {
    HAL_I2C_STATE_RESET             = 0x00U,
    HAL_I2C_STATE_READY             = 0x20U,
    HAL_I2C_STATE_BUSY              = 0x24U,
    HAL_I2C_STATE_BUSY_TX           = 0x21U,
    HAL_I2C_STATE_BUSY_RX           = 0x22U,
    HAL_I2C_STATE_LISTEN            = 0x28U,
    HAL_I2C_STATE_BUSY_TX_LISTEN    = 0x29U,
    HAL_I2C_STATE_BUSY_RX_LISTEN    = 0x2AU,
    HAL_I2C_STATE_ABORT             = 0x60U,
    HAL_I2C_STATE_TIMEOUT           = 0x50U,
    HAL_I2C_STATE_ERROR             = 0x40U
} HAL_I2C_StateTypeDef;

/* I2C Handle Structure */
typedef struct {
    void *Instance;
    uint32_t Init;
    uint8_t *pBuffPtr;
    uint16_t XferSize;
    volatile uint16_t XferCount;
    volatile uint32_t XferOptions;
    volatile uint32_t PreviousState;
    void *hdmatx;
    void *hdmarx;
    void *Lock;
    volatile HAL_I2C_StateTypeDef State;
    volatile uint32_t Mode;
    volatile uint32_t ErrorCode;
    uint32_t AddrEventCount;
} I2C_HandleTypeDef;

/* UART Handle Structure */
typedef struct {
    void *Instance;
    uint32_t Init;
    uint8_t *pTxBuffPtr;
    uint16_t TxXferSize;
    volatile uint16_t TxXferCount;
    uint8_t *pRxBuffPtr;
    uint16_t RxXferSize;
    volatile uint16_t RxXferCount;
    void *hdmatx;
    void *hdmarx;
    void *Lock;
    volatile uint32_t State;
    volatile uint32_t ErrorCode;
} UART_HandleTypeDef;

/* TIM Handle Structure */
typedef struct {
    void *Instance;
    uint32_t Init;
    uint32_t Channel;
    void *hdma[7];
    void *Lock;
    volatile uint32_t State;
} TIM_HandleTypeDef;

/* HAL defines */
#define HAL_MAX_DELAY 0xFFFFFFFFU

/* Mock function declarations */
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, 
                                     uint16_t MemAddress, uint16_t MemAddSize,
                                     uint8_t *pData, uint16_t Size, uint32_t Timeout);

HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
                                    uint16_t MemAddress, uint16_t MemAddSize,
                                    uint8_t *pData, uint16_t Size, uint32_t Timeout);

HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData,
                                     uint16_t Size, uint32_t Timeout);

HAL_StatusTypeDef HAL_TIM_Base_Start(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim);

void HAL_Delay(uint32_t Delay);

/* Mock control functions */
void HAL_Mock_Reset(void);
void HAL_Mock_SetI2CWriteReturn(HAL_StatusTypeDef status);
void HAL_Mock_SetI2CReadReturn(HAL_StatusTypeDef status);
uint32_t HAL_Mock_GetI2CWriteCallCount(void);
uint32_t HAL_Mock_GetI2CReadCallCount(void);
uint8_t* HAL_Mock_GetLastI2CWriteData(void);
uint16_t HAL_Mock_GetLastI2CWriteSize(void);
uint16_t HAL_Mock_GetLastI2CDevAddress(void);

#ifdef __cplusplus
}
#endif

#endif /* MOCK_STM32U5XX_HAL_H */
