#include "unity.h"
#include <stdint.h>
#include <math.h>

#define PCA9685_COUNTS 4096u

static inline uint16_t clamp_u16(int32_t v) {
    if (v < 0) return 0;
    if (v >= (int32_t)PCA9685_COUNTS) return (uint16_t)(PCA9685_COUNTS - 1u);
    return (uint16_t)v;
}

static inline uint16_t sub_sat_u16(uint16_t a, uint16_t b) {
    if (b >= a) return 0;
    return (uint16_t)(a - b);
}

void setUp(void) {
}

void tearDown(void) {
}

void test_clamp_u16_NegativeValueShouldReturnZero(void) {
    TEST_ASSERT_EQUAL_UINT16(0, clamp_u16(-100));
    TEST_ASSERT_EQUAL_UINT16(0, clamp_u16(-1));
}

void test_clamp_u16_OverflowShouldReturnMax(void) {
    TEST_ASSERT_EQUAL_UINT16(4095, clamp_u16(5000));
    TEST_ASSERT_EQUAL_UINT16(4095, clamp_u16(10000));
    TEST_ASSERT_EQUAL_UINT16(4095, clamp_u16(4096));
}

void test_clamp_u16_ValidRangeShouldReturnValue(void) {
    TEST_ASSERT_EQUAL_UINT16(0, clamp_u16(0));
    TEST_ASSERT_EQUAL_UINT16(2000, clamp_u16(2000));
    TEST_ASSERT_EQUAL_UINT16(4095, clamp_u16(4095));
    TEST_ASSERT_EQUAL_UINT16(1, clamp_u16(1));
}

void test_sub_sat_u16_BGreaterThanAShouldReturnZero(void) {
    TEST_ASSERT_EQUAL_UINT16(0, sub_sat_u16(100, 200));
    TEST_ASSERT_EQUAL_UINT16(0, sub_sat_u16(0, 100));
}

void test_sub_sat_u16_EqualValuesShouldReturnZero(void) {
    TEST_ASSERT_EQUAL_UINT16(0, sub_sat_u16(100, 100));
    TEST_ASSERT_EQUAL_UINT16(0, sub_sat_u16(0, 0));
}

void test_sub_sat_u16_ValidSubtractionShouldReturnDifference(void) {
    TEST_ASSERT_EQUAL_UINT16(100, sub_sat_u16(200, 100));
    TEST_ASSERT_EQUAL_UINT16(300, sub_sat_u16(500, 200));
    TEST_ASSERT_EQUAL_UINT16(4095, sub_sat_u16(4095, 0));
}

void test_sub_sat_u16_EdgeCases(void) {
    TEST_ASSERT_EQUAL_UINT16(1, sub_sat_u16(65535, 65534));
    TEST_ASSERT_EQUAL_UINT16(0, sub_sat_u16(65535, 65535));
}
