/* Mock implementation for STM32 HAL for unit testing */
#include "../test/mocks/stm32_hal_i2c.h"
#include <string.h>

/* Minimal typedefs for other HAL handles referenced */
typedef struct { int dummy; } UART_HandleTypeDef;
typedef struct { int dummy; } TIM_HandleTypeDef;

/* Mock state */
static struct {
    HAL_StatusTypeDef i2c_write_return;
    HAL_StatusTypeDef i2c_read_return;
    uint32_t i2c_write_count;
    uint32_t i2c_read_count;
    uint8_t last_i2c_write_data[256];
    uint16_t last_i2c_write_size;
    uint16_t last_i2c_dev_address;
    uint16_t last_i2c_mem_address;
    uint8_t last_i2c_read_data[256];
    uint16_t last_i2c_read_size;
} mock_state = {
    .i2c_write_return = HAL_OK,
    .i2c_read_return = HAL_OK,
    .i2c_write_count = 0,
    .i2c_read_count = 0,
    .last_i2c_write_size = 0,
    .last_i2c_dev_address = 0,
    .last_i2c_mem_address = 0,
    .last_i2c_read_size = 0
};

void HAL_Mock_Reset(void) {
    mock_state.i2c_write_return = HAL_OK;
    mock_state.i2c_read_return = HAL_OK;
    mock_state.i2c_write_count = 0;
    mock_state.i2c_read_count = 0;
    mock_state.last_i2c_write_size = 0;
    mock_state.last_i2c_dev_address = 0;
    mock_state.last_i2c_mem_address = 0;
    mock_state.last_i2c_read_size = 0;
    memset(mock_state.last_i2c_write_data, 0, sizeof(mock_state.last_i2c_write_data));
    memset(mock_state.last_i2c_read_data, 0, sizeof(mock_state.last_i2c_read_data));
}

void HAL_Mock_SetI2CWriteReturn(HAL_StatusTypeDef status) {
    mock_state.i2c_write_return = status;
}

void HAL_Mock_SetI2CReadReturn(HAL_StatusTypeDef status) {
    mock_state.i2c_read_return = status;
}

uint32_t HAL_Mock_GetI2CWriteCallCount(void) {
    return mock_state.i2c_write_count;
}

uint32_t HAL_Mock_GetI2CReadCallCount(void) {
    return mock_state.i2c_read_count;
}

uint8_t* HAL_Mock_GetLastI2CWriteData(void) {
    return mock_state.last_i2c_write_data;
}

uint16_t HAL_Mock_GetLastI2CWriteSize(void) {
    return mock_state.last_i2c_write_size;
}

uint16_t HAL_Mock_GetLastI2CDevAddress(void) {
    return mock_state.last_i2c_dev_address;
}

HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
                                     uint16_t MemAddress, uint16_t MemAddSize,
                                     uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    mock_state.i2c_write_count++;
    mock_state.last_i2c_dev_address = DevAddress;
    mock_state.last_i2c_mem_address = MemAddress;
    mock_state.last_i2c_write_size = Size;
    
    if (Size <= sizeof(mock_state.last_i2c_write_data)) {
        memcpy(mock_state.last_i2c_write_data, pData, Size);
    }
    
    return mock_state.i2c_write_return;
}

HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
                                    uint16_t MemAddress, uint16_t MemAddSize,
                                    uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    mock_state.i2c_read_count++;
    mock_state.last_i2c_dev_address = DevAddress;
    mock_state.last_i2c_mem_address = MemAddress;
    mock_state.last_i2c_read_size = Size;
    
    if (Size <= sizeof(mock_state.last_i2c_read_data)) {
        memcpy(pData, mock_state.last_i2c_read_data, Size);
    }
    
    return mock_state.i2c_read_return;
}

HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData,
                                     uint16_t Size, uint32_t Timeout) {
    /* Mock: just return OK */
    return HAL_OK;
}

HAL_StatusTypeDef HAL_TIM_Base_Start(TIM_HandleTypeDef *htim) {
    return HAL_OK;
}

HAL_StatusTypeDef HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim) {
    return HAL_OK;
}

void HAL_Delay(uint32_t Delay) {
    /* Mock: do nothing */
}
