#include "unity.h"
#include "speed_sensor_functions.h"
#include <math.h>

void setUp(void) {
}

void tearDown(void) {
}

// Testes para Speed_CalculateRotations
void test_Speed_CalculateRotations_ShouldReturnOneForThirtyPulses(void) {
    float result = Speed_CalculateRotations(30);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.0f, result);
}

void test_Speed_CalculateRotations_ShouldReturnZeroForZeroPulses(void) {
    float result = Speed_CalculateRotations(0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0f, result);
}

void test_Speed_CalculateRotations_ShouldReturnTwoFor60Pulses(void) {
    float result = Speed_CalculateRotations(60);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 2.0f, result);
}

void test_Speed_CalculateRotations_ShouldReturnHalfFor15Pulses(void) {
    float result = Speed_CalculateRotations(15);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.5f, result);
}

// Testes para Speed_CalculateDistance
void test_Speed_CalculateDistance_ShouldReturnPerimeterForOneRotation(void) {
    float result = Speed_CalculateDistance(1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.212f, result);
}

void test_Speed_CalculateDistance_ShouldReturnZeroForZeroRotations(void) {
    float result = Speed_CalculateDistance(0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.0f, result);
}

void test_Speed_CalculateDistance_ShouldReturnDoubleForTwoRotations(void) {
    float result = Speed_CalculateDistance(2.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001, 0.424f, result);
}

void test_Speed_CalculateDistance_ShouldHandleTenRotations(void) {
    float result = Speed_CalculateDistance(10.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 2.12f, result);
}

// Testes para Speed_CalculateVelocity
void test_Speed_CalculateVelocity_ShouldCalculateCorrectSpeed(void) {
    float result = Speed_CalculateVelocity(10.0f, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 5.0f, result);
}

void test_Speed_CalculateVelocity_ShouldReturnZeroForZeroTime(void) {
    float result = Speed_CalculateVelocity(10.0f, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0f, result);
}

void test_Speed_CalculateVelocity_ShouldReturnZeroForSmallTime(void) {
    float result = Speed_CalculateVelocity(10.0f, 0.0001f);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0f, result);
}

void test_Speed_CalculateVelocity_ShouldHandleZeroDistance(void) {
    float result = Speed_CalculateVelocity(0.0f, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0f, result);
}

void test_Speed_CalculateVelocity_ShouldHandleLargeValues(void) {
    float result = Speed_CalculateVelocity(100.0f, 10.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 10.0f, result);
}

// Testes para Speed_CalculatePulses
void test_Speed_CalculatePulses_ShouldReturnDifferenceWhenNoOverflow(void) {
    uint32_t result = Speed_CalculatePulses(1000, 500);
    TEST_ASSERT_EQUAL_UINT32(500, result);
}

void test_Speed_CalculatePulses_ShouldReturnZeroWhenEqual(void) {
    uint32_t result = Speed_CalculatePulses(1000, 1000);
    TEST_ASSERT_EQUAL_UINT32(0, result);
}

void test_Speed_CalculatePulses_ShouldHandleOverflow(void) {
    uint32_t result = Speed_CalculatePulses(100, 65500);
    TEST_ASSERT_EQUAL_UINT32(136, result);
}

void test_Speed_CalculatePulses_ShouldHandleMaxOverflow(void) {
    uint32_t result = Speed_CalculatePulses(0, 65535);
    TEST_ASSERT_EQUAL_UINT32(1, result);
}

// Testes para Speed_ValidateTimeDelta
void test_Speed_ValidateTimeDelta_ShouldAcceptValidTime(void) {
    int result = Speed_ValidateTimeDelta(1.0f);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_Speed_ValidateTimeDelta_ShouldRejectZeroTime(void) {
    int result = Speed_ValidateTimeDelta(0.0f);
    TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_Speed_ValidateTimeDelta_ShouldRejectSmallTime(void) {
    int result = Speed_ValidateTimeDelta(0.0001f);
    TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_Speed_ValidateTimeDelta_ShouldRejectLargeTime(void) {
    int result = Speed_ValidateTimeDelta(20.0f);
    TEST_ASSERT_EQUAL_INT(-1, result);
}

void test_Speed_ValidateTimeDelta_ShouldAcceptBoundary(void) {
    int result = Speed_ValidateTimeDelta(0.002f);
    TEST_ASSERT_EQUAL_INT(0, result);
}

// Testes para Speed_TimeTicksToSeconds
void test_Speed_TimeTicksToSeconds_ShouldConvert1000Ticks(void) {
    float result = Speed_TimeTicksToSeconds(1000, 1000);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 1.0f, result);
}

void test_Speed_TimeTicksToSeconds_ShouldConvert500Ticks(void) {
    float result = Speed_TimeTicksToSeconds(500, 1000);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.5f, result);
}

void test_Speed_TimeTicksToSeconds_ShouldReturnZeroForZeroTicks(void) {
    float result = Speed_TimeTicksToSeconds(0, 1000);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0f, result);
}

void test_Speed_TimeTicksToSeconds_ShouldHandle2000Ticks(void) {
    float result = Speed_TimeTicksToSeconds(2000, 1000);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 2.0f, result);
}

// Testes para Speed_CalculateSpeedMPS
void test_Speed_CalculateSpeedMPS_ShouldCalculateCorrectSpeed(void) {
    float result = Speed_CalculateSpeedMPS(30, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.212f, result);
}

void test_Speed_CalculateSpeedMPS_ShouldReturnZeroForZeroPulses(void) {
    float result = Speed_CalculateSpeedMPS(0, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0f, result);
}

void test_Speed_CalculateSpeedMPS_ShouldReturnZeroForZeroTime(void) {
    float result = Speed_CalculateSpeedMPS(30, 0.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0f, result);
}

void test_Speed_CalculateSpeedMPS_ShouldHandleHighSpeed(void) {
    float result = Speed_CalculateSpeedMPS(300, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.1, 2.12f, result);
}

void test_Speed_CalculateSpeedMPS_ShouldHandleLowSpeed(void) {
    float result = Speed_CalculateSpeedMPS(15, 2.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.053f, result);
}
