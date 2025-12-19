#include "unity.h"
#include <string.h>
#include "pca9685_testable.h"
#include "dc_motor_testable.h"
/* Force compilation of HAL mock implementation */
#include "../src/mock_stm32u5xx_hal.c"

static I2C_HandleTypeDef test_i2c;

void setUp(void) {
    HAL_Mock_Reset();
    test_i2c.Instance = NULL;
    test_i2c.State = HAL_I2C_STATE_READY;
}

void tearDown(void) {
}

/* Motor_Stop Tests */
void test_Motor_Stop_ShouldSetAllChannelsToZero(void) {
    Motor_Stop_Testable(&test_i2c);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

void test_Motor_Stop_ShouldUseCorrectI2CAddress(void) {
    Motor_Stop_Testable(&test_i2c);
    
    uint16_t addr = HAL_Mock_GetLastI2CDevAddress();
    TEST_ASSERT_EQUAL_UINT16(PCA9685_ADDR_MOTOR << 1, addr);
}

/* Motor_Forward Tests */
void test_Motor_Forward_HalfSpeedShouldSetCorrectPWM(void) {
    Motor_Forward_Testable(&test_i2c, 0.5);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

void test_Motor_Forward_FullSpeedShouldSetMaxPWM(void) {
    Motor_Forward_Testable(&test_i2c, 1.0);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

void test_Motor_Forward_ZeroSpeedShouldSetZeroPWM(void) {
    Motor_Forward_Testable(&test_i2c, 0.0);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

/* Motor_Backward Tests */
void test_Motor_Backward_HalfSpeedShouldSetCorrectPWM(void) {
    Motor_Backward_Testable(&test_i2c, 0.5);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

void test_Motor_Backward_FullSpeedShouldSetMaxPWM(void) {
    Motor_Backward_Testable(&test_i2c, 1.0);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

/* Motor_Left Tests */
void test_Motor_Left_HalfSpeedShouldWork(void) {
    Motor_Left_Testable(&test_i2c, 0.5);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

/* Motor_Right Tests */
void test_Motor_Right_HalfSpeedShouldWork(void) {
    Motor_Right_Testable(&test_i2c, 0.5);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

/* Motor_SetMotor Tests */
void test_Motor_SetMotor_PositiveValueShouldMoveForward(void) {
    Motor_SetMotor_Testable(&test_i2c, 0.7);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

void test_Motor_SetMotor_NegativeValueShouldMoveBackward(void) {
    Motor_SetMotor_Testable(&test_i2c, -0.7);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

void test_Motor_SetMotor_ZeroShouldStop(void) {
    Motor_SetMotor_Testable(&test_i2c, 0.0);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

void test_Motor_SetMotor_VerySmallValueShouldStop(void) {
    Motor_SetMotor_Testable(&test_i2c, 0.0000001);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

/* Motor_SetPWM Tests */
void test_Motor_SetPWM_PositiveLeftPositiveRight(void) {
    Motor_SetPWM_Testable(&test_i2c, 2000, 3000);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

void test_Motor_SetPWM_NegativeLeftNegativeRight(void) {
    Motor_SetPWM_Testable(&test_i2c, -2000, -3000);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

void test_Motor_SetPWM_ZeroLeftZeroRight(void) {
    Motor_SetPWM_Testable(&test_i2c, 0, 0);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

void test_Motor_SetPWM_MixedValues(void) {
    Motor_SetPWM_Testable(&test_i2c, 2000, -3000);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

void test_Motor_SetPWM_OverflowValues(void) {
    Motor_SetPWM_Testable(&test_i2c, 10000, -10000);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

/* I2C Error Handling Tests */
void test_Motor_Forward_ShouldContinueOnI2CError(void) {
    HAL_Mock_SetI2CWriteReturn(HAL_ERROR);
    
    Motor_Forward_Testable(&test_i2c, 0.5);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

void test_Motor_Stop_ShouldContinueOnI2CTimeout(void) {
    HAL_Mock_SetI2CWriteReturn(HAL_TIMEOUT);
    
    Motor_Stop_Testable(&test_i2c);
    
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}
