/**
 * @file test_helpers.h
 * @brief Common Test Utilities and Helpers
 * @note Reusable functions for ISO 26262 compliant testing
 */

#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include "unity.h"
#include "automotive_types.h"
#include <math.h>

/* ========================================================================
 * FLOATING POINT COMPARISON
 * ======================================================================== */

/**
 * @brief Assert two doubles are equal within tolerance
 */
#define TEST_ASSERT_DOUBLE_WITHIN_TOLERANCE(expected, actual, tolerance) \
    TEST_ASSERT_DOUBLE_WITHIN(tolerance, expected, actual)

/**
 * @brief Assert PWM value is within valid range [0, 4095]
 */
#define TEST_ASSERT_PWM_VALID(pwm) \
    TEST_ASSERT_TRUE_MESSAGE((pwm) >= 0 && (pwm) <= 4095, "PWM out of range")

/* ========================================================================
 * MOCK HELPERS
 * ======================================================================== */

/**
 * @brief Helper to create expectation for PWM setting
 * @note Reduces boilerplate in tests
 */
#define EXPECT_PCA9685_SET_PWM(hi2c, addr, channel, on, off) \
    PCA9685_SetPWM_ExpectAndReturn(hi2c, addr, channel, on, off, HAL_OK)

/**
 * @brief Helper for I2C transmission expectation
 */
#define EXPECT_I2C_TRANSMIT(hi2c, addr, data, size, timeout) \
    HAL_I2C_Master_Transmit_ExpectAndReturn(hi2c, addr, data, size, timeout, HAL_OK)

/* ========================================================================
 * TEST FIXTURE HELPERS
 * ======================================================================== */

/**
 * @brief Initialize I2C handle for testing
 */
static inline void TestHelper_InitI2C(I2C_HandleTypeDef *hi2c) {
    hi2c->Instance = (void*)0x40005400;  /* Simulated I2C3 */
    hi2c->Timing = 0x00000000;
    hi2c->OwnAddress1 = 0;
    hi2c->ErrorCode = 0;
    hi2c->State = 0;
}

/**
 * @brief Initialize ThreadX thread for testing
 */
static inline void TestHelper_InitThread(TX_THREAD *thread, const char *name) {
    thread->tx_thread_id = 0x12345678;
    thread->tx_thread_name = (char*)name;
    thread->tx_thread_state = 0;
    thread->tx_thread_priority = 10;
}

/**
 * @brief Initialize ThreadX queue for testing
 */
static inline void TestHelper_InitQueue(TX_QUEUE *queue, const char *name) {
    queue->tx_queue_id = 0x87654321;
    queue->tx_queue_name = (char*)name;
    queue->tx_queue_message_size = 4;
    queue->tx_queue_capacity = 16;
}

/* ========================================================================
 * CONVERSION HELPERS
 * ======================================================================== */

/**
 * @brief Convert speed [0.0, 1.0] to PWM [0, 4095]
 */
static inline uint16_t TestHelper_SpeedToPWM(double speed) {
    if (speed < 0.0) speed = 0.0;
    if (speed > 1.0) speed = 1.0;
    return (uint16_t)(speed * 4095.0);
}

/**
 * @brief Convert angle [0, 180] to servo pulse [200, 500]
 */
static inline uint16_t TestHelper_AngleToPulse(uint8_t angle) {
    if (angle > 180) angle = 180;
    return 200 + ((uint32_t)angle * 300 / 180);
}

#endif /* TEST_HELPERS_H */
