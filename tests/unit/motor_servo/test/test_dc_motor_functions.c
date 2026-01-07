#include "unity.h"
#include "dc_motor_functions.h"

void setUp(void) {
}

void tearDown(void) {
}

// Tests for DC_Motor_Clamp
void test_DC_Motor_Clamp_ShouldReturnZeroForNegative(void) {
    uint16_t result = DC_Motor_Clamp(-100);
    TEST_ASSERT_EQUAL_UINT16(0, result);
}

void test_DC_Motor_Clamp_ShouldReturnMaxForOverflow(void) {
    uint16_t result = DC_Motor_Clamp(5000);
    TEST_ASSERT_EQUAL_UINT16(4095, result);
}

void test_DC_Motor_Clamp_ShouldReturnValueInRange(void) {
    uint16_t result = DC_Motor_Clamp(2000);
    TEST_ASSERT_EQUAL_UINT16(2000, result);
}

void test_DC_Motor_Clamp_ShouldHandleZero(void) {
    uint16_t result = DC_Motor_Clamp(0);
    TEST_ASSERT_EQUAL_UINT16(0, result);
}

void test_DC_Motor_Clamp_ShouldHandleMaxValue(void) {
    uint16_t result = DC_Motor_Clamp(4095);
    TEST_ASSERT_EQUAL_UINT16(4095, result);
}

// Tests for DC_Motor_SubSaturate
void test_DC_Motor_SubSaturate_ShouldReturnZeroWhenBGreaterThanA(void) {
    uint16_t result = DC_Motor_SubSaturate(100, 200);
    TEST_ASSERT_EQUAL_UINT16(0, result);
}

void test_DC_Motor_SubSaturate_ShouldReturnDifference(void) {
    uint16_t result = DC_Motor_SubSaturate(500, 200);
    TEST_ASSERT_EQUAL_UINT16(300, result);
}

void test_DC_Motor_SubSaturate_ShouldReturnZeroWhenEqual(void) {
    uint16_t result = DC_Motor_SubSaturate(100, 100);
    TEST_ASSERT_EQUAL_UINT16(0, result);
}

// Tests for DC_Motor_ValidatePWM
void test_DC_Motor_ValidatePWM_ShouldAcceptValidRange(void) {
    int result = DC_Motor_ValidatePWM(2000, 3000);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_DC_Motor_ValidatePWM_ShouldAcceptNegativeValues(void) {
    int result = DC_Motor_ValidatePWM(-2000, -1000);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_DC_Motor_ValidatePWM_ShouldRejectOverflow(void) {
    int result = DC_Motor_ValidatePWM(5000, 2000);
    TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_DC_Motor_ValidatePWM_ShouldRejectUnderflow(void) {
    int result = DC_Motor_ValidatePWM(-5000, 2000);
    TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_DC_Motor_ValidatePWM_ShouldAcceptZero(void) {
    int result = DC_Motor_ValidatePWM(0, 0);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_DC_Motor_ValidatePWM_ShouldRejectRightCountsOverflow(void) {
    // Test right_counts overflow (line 27)
    int result = DC_Motor_ValidatePWM(2000, 5000);
    TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_DC_Motor_ValidatePWM_ShouldRejectRightCountsUnderflow(void) {
    // Test right_counts underflow (line 26)
    int result = DC_Motor_ValidatePWM(2000, -5000);
    TEST_ASSERT_EQUAL_INT(-1, result);
}

// Tests for DC_Motor_CalculatePWM
void test_DC_Motor_CalculatePWM_ShouldReturnZeroForZeroSpeed(void) {
    uint16_t result = DC_Motor_CalculatePWM(0.0);
    TEST_ASSERT_EQUAL_UINT16(0, result);
}

void test_DC_Motor_CalculatePWM_ShouldReturnMaxForFullSpeed(void) {
    uint16_t result = DC_Motor_CalculatePWM(1.0);
    TEST_ASSERT_EQUAL_UINT16(4095, result);
}

void test_DC_Motor_CalculatePWM_ShouldReturnHalfForHalfSpeed(void) {
    uint16_t result = DC_Motor_CalculatePWM(0.5);
    TEST_ASSERT_UINT16_WITHIN(10, 2047, result);
}

void test_DC_Motor_CalculatePWM_ShouldClampNegativeToZero(void) {
    uint16_t result = DC_Motor_CalculatePWM(-0.5);
    TEST_ASSERT_EQUAL_UINT16(0, result);
}

void test_DC_Motor_CalculatePWM_ShouldClampAboveOneToMax(void) {
    uint16_t result = DC_Motor_CalculatePWM(1.5);
    TEST_ASSERT_EQUAL_UINT16(4095, result);
}

// Tests for DC_Motor_GetDirection
void test_DC_Motor_GetDirection_ShouldReturnForward(void) {
    motor_direction_t dir = DC_Motor_GetDirection(0.5);
    TEST_ASSERT_EQUAL(MOTOR_FORWARD, dir);
}

void test_DC_Motor_GetDirection_ShouldReturnBackward(void) {
    motor_direction_t dir = DC_Motor_GetDirection(-0.5);
    TEST_ASSERT_EQUAL(MOTOR_BACKWARD, dir);
}

void test_DC_Motor_GetDirection_ShouldReturnStopForZero(void) {
    motor_direction_t dir = DC_Motor_GetDirection(0.0);
    TEST_ASSERT_EQUAL(MOTOR_STOP, dir);
}

void test_DC_Motor_GetDirection_ShouldReturnStopForSmallPositive(void) {
    motor_direction_t dir = DC_Motor_GetDirection(0.0001);
    TEST_ASSERT_EQUAL(MOTOR_STOP, dir);
}

void test_DC_Motor_GetDirection_ShouldReturnStopForSmallNegative(void) {
    motor_direction_t dir = DC_Motor_GetDirection(-0.0001);
    TEST_ASSERT_EQUAL(MOTOR_STOP, dir);
}

// Tests for DC_Motor_AbsoluteSpeed
void test_DC_Motor_AbsoluteSpeed_ShouldReturnPositiveForPositive(void) {
    double result = DC_Motor_AbsoluteSpeed(5.5);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 5.5, result);
}

void test_DC_Motor_AbsoluteSpeed_ShouldReturnPositiveForNegative(void) {
    double result = DC_Motor_AbsoluteSpeed(-5.5);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 5.5, result);
}

void test_DC_Motor_AbsoluteSpeed_ShouldReturnZeroForZero(void) {
    double result = DC_Motor_AbsoluteSpeed(0.0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, result);
}
