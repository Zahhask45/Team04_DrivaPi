#include "unity.h"
#include <stdint.h>

#define PCA9685_COUNTS 4096u

static inline uint16_t clamp_u16_servo(int32_t v) {
    if (v < 0) return 0;
    if (v > (int32_t)(PCA9685_COUNTS - 1u)) return (uint16_t)(PCA9685_COUNTS - 1u);
    return (uint16_t)v;
}

static uint32_t calculate_servo_pulse(uint16_t angle_deg, 
                                      uint16_t min_pulse_counts, 
                                      uint16_t max_pulse_counts) {
    if (angle_deg > 180u) angle_deg = 180u;
    if (min_pulse_counts >= max_pulse_counts) {
        return 0;
    }
    
    uint32_t range = (uint32_t)max_pulse_counts - (uint32_t)min_pulse_counts;
    uint32_t pulse = (uint32_t)min_pulse_counts + (range * angle_deg) / 180u;
    return pulse;
}

void setUp(void) {
}

void tearDown(void) {
}

void test_clamp_u16_servo_NegativeValueShouldReturnZero(void) {
    TEST_ASSERT_EQUAL_UINT16(0, clamp_u16_servo(-100));
    TEST_ASSERT_EQUAL_UINT16(0, clamp_u16_servo(-1));
}

void test_clamp_u16_servo_OverMaxShouldReturnMax(void) {
    TEST_ASSERT_EQUAL_UINT16(4095, clamp_u16_servo(5000));
    TEST_ASSERT_EQUAL_UINT16(4095, clamp_u16_servo(10000));
}

void test_clamp_u16_servo_ValidRangeShouldReturnValue(void) {
    TEST_ASSERT_EQUAL_UINT16(0, clamp_u16_servo(0));
    TEST_ASSERT_EQUAL_UINT16(2000, clamp_u16_servo(2000));
    TEST_ASSERT_EQUAL_UINT16(4095, clamp_u16_servo(4095));
}

void test_calculate_servo_pulse_MinAngleShouldReturnMinPulse(void) {
    uint32_t pulse = calculate_servo_pulse(0, 200, 500);
    TEST_ASSERT_EQUAL_UINT32(200, pulse);
}

void test_calculate_servo_pulse_MaxAngleShouldReturnMaxPulse(void) {
    uint32_t pulse = calculate_servo_pulse(180, 200, 500);
    TEST_ASSERT_EQUAL_UINT32(500, pulse);
}

void test_calculate_servo_pulse_MidAngleShouldReturnMidPulse(void) {
    uint32_t pulse = calculate_servo_pulse(90, 200, 500);
    TEST_ASSERT_EQUAL_UINT32(350, pulse);
}

void test_calculate_servo_pulse_AngleOverMaxShouldClamp(void) {
    uint32_t pulse = calculate_servo_pulse(200, 200, 500);
    TEST_ASSERT_EQUAL_UINT32(500, pulse);
}

void test_calculate_servo_pulse_InvalidRangeShouldReturnZero(void) {
    uint32_t pulse = calculate_servo_pulse(90, 500, 200);
    TEST_ASSERT_EQUAL_UINT32(0, pulse);
    
    pulse = calculate_servo_pulse(90, 300, 300);
    TEST_ASSERT_EQUAL_UINT32(0, pulse);
}

void test_calculate_servo_pulse_QuarterAngle(void) {
    uint32_t pulse = calculate_servo_pulse(45, 200, 500);
    TEST_ASSERT_EQUAL_UINT32(275, pulse);
}

void test_calculate_servo_pulse_ThreeQuarterAngle(void) {
    uint32_t pulse = calculate_servo_pulse(135, 200, 500);
    TEST_ASSERT_EQUAL_UINT32(425, pulse);
}
