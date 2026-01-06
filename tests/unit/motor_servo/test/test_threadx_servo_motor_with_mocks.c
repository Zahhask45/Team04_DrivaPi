#include "unity.h"
#include "servo_motor.h"
/* Force compilation of HAL mock implementation */
#include "../src/mock_stm32u5xx_hal.c"
/* Force compilation of PCA9685 testable implementation */
#include "../src/pca9685_testable.c"

static I2C_HandleTypeDef test_i2c;

void setUp(void) {
    HAL_Mock_Reset();
    test_i2c.Instance = NULL;
    test_i2c.State = HAL_I2C_STATE_READY;
}

void tearDown(void) {
}

/* Servo_SetAngle Success Tests */
void test_Servo_SetAngle_MinAngleShouldSucceed(void) {
    int result = Servo_SetAngle(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                                         0, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_UINT32(1, HAL_Mock_GetI2CWriteCallCount());
}

void test_Servo_SetAngle_MaxAngleShouldSucceed(void) {
    int result = Servo_SetAngle(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                                         180, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_UINT32(1, HAL_Mock_GetI2CWriteCallCount());
}

void test_Servo_SetAngle_MidAngleShouldSucceed(void) {
    int result = Servo_SetAngle(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                                         90, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_UINT32(1, HAL_Mock_GetI2CWriteCallCount());
}

void test_Servo_SetAngle_45DegreeShouldSucceed(void) {
    int result = Servo_SetAngle(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                                         45, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_UINT32(1, HAL_Mock_GetI2CWriteCallCount());
}

void test_Servo_SetAngle_135DegreeShouldSucceed(void) {
    int result = Servo_SetAngle(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                                         135, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_UINT32(1, HAL_Mock_GetI2CWriteCallCount());
}

/* Servo_SetAngle Edge Cases */
void test_Servo_SetAngle_AngleOver180ShouldClampTo180(void) {
    int result = Servo_SetAngle(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                                         200, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_UINT32(1, HAL_Mock_GetI2CWriteCallCount());
}

void test_Servo_SetAngle_InvalidRangeShouldReturnError(void) {
    int result = Servo_SetAngle(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                                         90, 500, 200);
    
    TEST_ASSERT_EQUAL_INT(-1, result);
    TEST_ASSERT_EQUAL_UINT32(0, HAL_Mock_GetI2CWriteCallCount());
}

void test_Servo_SetAngle_EqualMinMaxShouldReturnError(void) {
    int result = Servo_SetAngle(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                                         90, 300, 300);
    
    TEST_ASSERT_EQUAL_INT(-1, result);
    TEST_ASSERT_EQUAL_UINT32(0, HAL_Mock_GetI2CWriteCallCount());
}

/* I2C Error Handling */
void test_Servo_SetAngle_I2CErrorShouldReturnNegative2(void) {
    HAL_Mock_SetI2CWriteReturn(HAL_ERROR);
    
    int result = Servo_SetAngle(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                                         90, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(-2, result);
    TEST_ASSERT_EQUAL_UINT32(1, HAL_Mock_GetI2CWriteCallCount());
}

void test_Servo_SetAngle_I2CTimeoutShouldReturnNegative2(void) {
    HAL_Mock_SetI2CWriteReturn(HAL_TIMEOUT);
    
    int result = Servo_SetAngle(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                                         90, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(-2, result);
    TEST_ASSERT_EQUAL_UINT32(1, HAL_Mock_GetI2CWriteCallCount());
}

/* Different Pulse Ranges */
void test_Servo_SetAngle_CustomPulseRange(void) {
    int result = Servo_SetAngle(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                                         90, 100, 600);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_UINT32(1, HAL_Mock_GetI2CWriteCallCount());
}

void test_Servo_SetAngle_WidePulseRange(void) {
    int result = Servo_SetAngle(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                                         90, 150, 650);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_UINT32(1, HAL_Mock_GetI2CWriteCallCount());
}

/* Different Channels */
void test_Servo_SetAngle_DifferentChannelShouldWork(void) {
    int result = Servo_SetAngle(&test_i2c, PCA9685_ADDR_SERVO, 5,
                                         90, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_UINT32(1, HAL_Mock_GetI2CWriteCallCount());
}

/* Default Parameters Test */
void test_Servo_SetAngle_WithDefaultParametersShouldWork(void) {
    int result = Servo_SetAngle(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                                         90, 
                                         SERVO_DEFAULT_MIN_PULSE_COUNTS,
                                         SERVO_DEFAULT_MAX_PULSE_COUNTS);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_UINT32(1, HAL_Mock_GetI2CWriteCallCount());
}
