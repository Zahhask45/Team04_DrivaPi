
#include "../vendor/unity/src/unity.h"
#include "servo_functions.h"

void setUp(void) {
}

void tearDown(void) {
}

// Testes para Servo_ValidateAngle
void test_Servo_ValidateAngle_ShouldAcceptZero(void) {
    int result = Servo_ValidateAngle(0);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_Servo_ValidateAngle_ShouldAccept90Degrees(void) {
    int result = Servo_ValidateAngle(90);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_Servo_ValidateAngle_ShouldAccept180Degrees(void) {
    int result = Servo_ValidateAngle(180);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_Servo_ValidateAngle_ShouldRejectAbove180(void) {
    int result = Servo_ValidateAngle(181);
    TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_Servo_ValidateAngle_ShouldRejectLargeValue(void) {
    int result = Servo_ValidateAngle(500);
    TEST_ASSERT_EQUAL_INT(-1, result);
}

// Testes para Servo_ValidatePulseRange
void test_Servo_ValidatePulseRange_ShouldAcceptValidRange(void) {
    int result = Servo_ValidatePulseRange(200, 500);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_Servo_ValidatePulseRange_ShouldRejectEqualValues(void) {
    int result = Servo_ValidatePulseRange(300, 300);
    TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_Servo_ValidatePulseRange_ShouldRejectMinGreaterThanMax(void) {
    int result = Servo_ValidatePulseRange(500, 200);
    TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_Servo_ValidatePulseRange_ShouldRejectMaxOverflow(void) {
    int result = Servo_ValidatePulseRange(200, 5000);
    TEST_ASSERT_EQUAL_INT(-1, result);
}

// Testes para Servo_CalculatePulse
void test_Servo_CalculatePulse_ShouldReturnMinForZeroDegrees(void) {
    uint16_t result = Servo_CalculatePulse(0, 200, 500);
    TEST_ASSERT_EQUAL_UINT16(200, result);
}

void test_Servo_CalculatePulse_ShouldReturnMaxFor180Degrees(void) {
    uint16_t result = Servo_CalculatePulse(180, 200, 500);
    TEST_ASSERT_EQUAL_UINT16(500, result);
}

void test_Servo_CalculatePulse_ShouldReturnMidFor90Degrees(void) {
    uint16_t result = Servo_CalculatePulse(90, 200, 500);
    TEST_ASSERT_UINT16_WITHIN(2, 350, result);
}

void test_Servo_CalculatePulse_ShouldClampAbove180(void) {
    uint16_t result = Servo_CalculatePulse(200, 200, 500);
    TEST_ASSERT_EQUAL_UINT16(500, result);
}

void test_Servo_CalculatePulse_ShouldHandle45Degrees(void) {
    uint16_t result = Servo_CalculatePulse(45, 200, 500);
    TEST_ASSERT_UINT16_WITHIN(2, 275, result);
}

void test_Servo_CalculatePulse_ShouldHandle135Degrees(void) {
    uint16_t result = Servo_CalculatePulse(135, 200, 500);
    TEST_ASSERT_UINT16_WITHIN(2, 425, result);
}

// Testes para Servo_ClampAngle
void test_Servo_ClampAngle_ShouldReturnValueInRange(void) {
    uint16_t result = Servo_ClampAngle(90);
    TEST_ASSERT_EQUAL_UINT16(90, result);
}

void test_Servo_ClampAngle_ShouldClampTo180(void) {
    uint16_t result = Servo_ClampAngle(250);
    TEST_ASSERT_EQUAL_UINT16(180, result);
}

void test_Servo_ClampAngle_ShouldReturnZero(void) {
    uint16_t result = Servo_ClampAngle(0);
    TEST_ASSERT_EQUAL_UINT16(0, result);
}

void test_Servo_ClampAngle_ShouldReturn180(void) {
    uint16_t result = Servo_ClampAngle(180);
    TEST_ASSERT_EQUAL_UINT16(180, result);
}

// Testes para Servo_MapAngleToPulse
void test_Servo_MapAngleToPulse_ShouldReturnMinFor0Percent(void) {
    uint16_t result = Servo_MapAngleToPulse(0);
    TEST_ASSERT_EQUAL_UINT16(200, result);
}

void test_Servo_MapAngleToPulse_ShouldReturnMaxFor100Percent(void) {
    uint16_t result = Servo_MapAngleToPulse(100);
    TEST_ASSERT_EQUAL_UINT16(500, result);
}

void test_Servo_MapAngleToPulse_ShouldReturnMidFor50Percent(void) {
    uint16_t result = Servo_MapAngleToPulse(50);
    TEST_ASSERT_UINT16_WITHIN(2, 350, result);
}

void test_Servo_MapAngleToPulse_ShouldClampAbove100(void) {
    uint16_t result = Servo_MapAngleToPulse(150);
    TEST_ASSERT_EQUAL_UINT16(500, result);
}

void test_Servo_MapAngleToPulse_ShouldHandle25Percent(void) {
    uint16_t result = Servo_MapAngleToPulse(25);
    TEST_ASSERT_UINT16_WITHIN(2, 275, result);
}

// Testes para Servo_ValidateAngle - cobrir branches 4,5
void test_Servo_ValidateAngle_Above180_ShouldReturnError(void) {
    int result = Servo_ValidateAngle(181);
    TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_Servo_ValidateAngle_Valid90_ShouldReturnSuccess(void) {
    int result = Servo_ValidateAngle(90);
    TEST_ASSERT_EQUAL_INT(0, result);
}
