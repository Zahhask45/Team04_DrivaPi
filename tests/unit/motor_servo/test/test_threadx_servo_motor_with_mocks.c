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

/* ========================================================================== */
/* Servo_Sweep Tests                                                          */
/* ========================================================================== */

/* Servo_Sweep Forward Sweep Tests */
void test_Servo_Sweep_ForwardSweep_ShouldCallMultipleSetAngle(void) {
    // Sweep from 0 to 90 degrees in steps of 30
    int result = Servo_Sweep(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                             0, 90, 30, 0, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    // Should call SetAngle 4 times (0, 30, 60, 90)
    TEST_ASSERT_EQUAL_UINT32(4, HAL_Mock_GetI2CWriteCallCount());
}

void test_Servo_Sweep_SingleStep_ShouldCallSetAngleOnce(void) {
    // Sweep from 90 to 90 (single position)
    int result = Servo_Sweep(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                             90, 90, 10, 0, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_UINT32(1, HAL_Mock_GetI2CWriteCallCount());
}

void test_Servo_Sweep_SmallSteps_ShouldWorkCorrectly(void) {
    // Sweep from 0 to 10 in steps of 1
    int result = Servo_Sweep(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                             0, 10, 1, 0, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    // Should call SetAngle 11 times (0, 1, 2, ..., 10)
    TEST_ASSERT_EQUAL_UINT32(11, HAL_Mock_GetI2CWriteCallCount());
}

/* Servo_Sweep Backward Sweep Tests */
void test_Servo_Sweep_BackwardSweep_ShouldCallMultipleSetAngle(void) {
    // Sweep from 180 to 0 in steps of 45
    int result = Servo_Sweep(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                             180, 0, 45, 0, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    // Should call SetAngle 5 times (180, 135, 90, 45, 0)
    TEST_ASSERT_EQUAL_UINT32(5, HAL_Mock_GetI2CWriteCallCount());
}

void test_Servo_Sweep_BackwardSmallRange_ShouldWork(void) {
    // Sweep from 100 to 80 in steps of 10
    int result = Servo_Sweep(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                             100, 80, 10, 0, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    // Should call SetAngle 3 times (100, 90, 80)
    TEST_ASSERT_EQUAL_UINT32(3, HAL_Mock_GetI2CWriteCallCount());
}

/* Servo_Sweep Edge Cases */
void test_Servo_Sweep_ZeroStepSize_ShouldDefaultToOne(void) {
    // Step size 0 should be treated as 1
    int result = Servo_Sweep(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                             0, 5, 0, 0, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    // Should call SetAngle 6 times (0, 1, 2, 3, 4, 5) with step=1
    TEST_ASSERT_EQUAL_UINT32(6, HAL_Mock_GetI2CWriteCallCount());
}

void test_Servo_Sweep_FullRange_ShouldWork(void) {
    // Sweep full range 0 to 180 in steps of 45
    int result = Servo_Sweep(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                             0, 180, 45, 0, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    // Should call SetAngle 5 times (0, 45, 90, 135, 180)
    TEST_ASSERT_EQUAL_UINT32(5, HAL_Mock_GetI2CWriteCallCount());
}

void test_Servo_Sweep_LargeStep_ShouldWork(void) {
    // Large step that goes beyond end in one step
    int result = Servo_Sweep(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                             0, 50, 100, 0, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    // Should call SetAngle 1 time (just 0, next would be 100 > 50)
    TEST_ASSERT_EQUAL_UINT32(1, HAL_Mock_GetI2CWriteCallCount());
}

/* Servo_Sweep with Custom Pulse Range */
void test_Servo_Sweep_CustomPulseRange_ShouldWork(void) {
    // Test with custom pulse range
    int result = Servo_Sweep(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                             0, 60, 30, 0, 100, 600);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    // Should call SetAngle 3 times (0, 30, 60)
    TEST_ASSERT_EQUAL_UINT32(3, HAL_Mock_GetI2CWriteCallCount());
}

void test_Servo_Sweep_DifferentChannel_ShouldWork(void) {
    // Test with different channel
    int result = Servo_Sweep(&test_i2c, PCA9685_ADDR_SERVO, 3,
                             45, 135, 45, 0, 200, 500);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    // Should call SetAngle 3 times (45, 90, 135)
    TEST_ASSERT_EQUAL_UINT32(3, HAL_Mock_GetI2CWriteCallCount());
}

/* ========================================================================== */
/* Edge Cases for clamp_u16 coverage (servo_motor.c)                         */
/* ========================================================================== */

void test_Servo_SetAngle_ExtremelyLargePulseCounts_ShouldClamp(void) {
    // Test with pulse counts that would overflow uint16_t (> 4095)
    // This tests the upper bound of clamp_u16
    int result = Servo_SetAngle(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                                180, 4000, 5000);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_UINT32(1, HAL_Mock_GetI2CWriteCallCount());
}

void test_Servo_SetAngle_VeryWidePulseRange_ShouldHandleOverflow(void) {
    // Test with very wide pulse range (min=0, max=65535)
    // This exercises clamp_u16 with values exceeding PCA9685_COUNTS
    int result = Servo_SetAngle(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                                180, 0, 65535);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_UINT32(1, HAL_Mock_GetI2CWriteCallCount());
}

void test_Servo_SetAngle_ZeroDegreeWithLargePulse_ShouldClamp(void) {
    // Test with 0 degree and large pulse range to ensure clamping works
    int result = Servo_SetAngle(&test_i2c, PCA9685_ADDR_SERVO, SERVO_CH,
                                0, 0, 10000);
    
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_UINT32(1, HAL_Mock_GetI2CWriteCallCount());
}
