#include "unity.h"
#include <stdint.h>

#define WHEEL_PERIMETER_M   0.212f
#define PULSES_PER_REV      30.0f
#define TIMER_PERIOD        65535
#define TX_TIMER_TICKS_PER_SECOND 1000

static float calculate_speed(uint32_t pulses, float dt) {
    if (dt <= 0.001f) return 0.0f;
    
    float rotations = (float)pulses / PULSES_PER_REV;
    float distance_m = rotations * WHEEL_PERIMETER_M;
    float speed_mps = distance_m / dt;
    
    return speed_mps;
}

static uint32_t calculate_pulses_with_overflow(uint32_t current_count, 
                                                uint32_t last_count) {
    uint32_t pulses = 0;
    if (current_count >= last_count) {
        pulses = current_count - last_count;
    } else {
        pulses = (TIMER_PERIOD - last_count) + current_count + 1;
    }
    return pulses;
}

void setUp(void) {
}

void tearDown(void) {
}

void test_calculate_speed_ZeroPulsesShouldReturnZeroSpeed(void) {
    float speed = calculate_speed(0, 1.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, speed);
}

void test_calculate_speed_SmallDtShouldReturnZero(void) {
    float speed = calculate_speed(100, 0.0001f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, speed);
    
    speed = calculate_speed(100, 0.0f);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, speed);
}

void test_calculate_speed_OneRevolutionInOneSecond(void) {
    float speed = calculate_speed(30, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.212f, speed);
}

void test_calculate_speed_TwoRevolutionsInOneSecond(void) {
    float speed = calculate_speed(60, 1.0f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.424f, speed);
}

void test_calculate_speed_HalfRevolutionInHalfSecond(void) {
    float speed = calculate_speed(15, 0.5f);
    TEST_ASSERT_FLOAT_WITHIN(0.001f, 0.212f, speed);
}

void test_calculate_speed_TypicalCase(void) {
    float speed = calculate_speed(100, 0.1f);
    float expected = (100.0f / 30.0f) * 0.212f / 0.1f;
    TEST_ASSERT_FLOAT_WITHIN(0.01f, expected, speed);
}

void test_calculate_pulses_NoOverflow(void) {
    uint32_t pulses = calculate_pulses_with_overflow(1000, 500);
    TEST_ASSERT_EQUAL_UINT32(500, pulses);
}

void test_calculate_pulses_NoChange(void) {
    uint32_t pulses = calculate_pulses_with_overflow(1000, 1000);
    TEST_ASSERT_EQUAL_UINT32(0, pulses);
}

void test_calculate_pulses_WithOverflow(void) {
    uint32_t pulses = calculate_pulses_with_overflow(100, 65500);
    uint32_t expected = (TIMER_PERIOD - 65500) + 100 + 1;
    TEST_ASSERT_EQUAL_UINT32(expected, pulses);
}

void test_calculate_pulses_MaxOverflow(void) {
    uint32_t pulses = calculate_pulses_with_overflow(0, 65535);
    TEST_ASSERT_EQUAL_UINT32(1, pulses);
}

void test_calculate_pulses_SmallOverflow(void) {
    uint32_t pulses = calculate_pulses_with_overflow(10, 65530);
    uint32_t expected = (TIMER_PERIOD - 65530) + 10 + 1;
    TEST_ASSERT_EQUAL_UINT32(expected, pulses);
}
