/**
 * @file automotive_types.h
 * @brief Platform-Independent Automotive Types for Unit Testing
 * @note Enables testing on Linux x64 while targeting ARM/Automotive SoC
 * 
 * Strategy:
 * - In tests: Use standard C types compatible with x64
 * - In target: Include real hardware headers
 * 
 * This resolves portability between:
 * - Test Environment: Linux x86_64, GCC
 * - Target Environment: ARM Cortex-M33, ARM Compiler
 */

#ifndef AUTOMOTIVE_TYPES_H
#define AUTOMOTIVE_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Build Detection */
#if defined(UNIT_TEST_BUILD) || defined(TEST)
    #define TESTING_ENVIRONMENT 1
#else
    #define TESTING_ENVIRONMENT 0
#endif

/* ========================================================================
 * AUTOSAR-COMPLIANT INTEGER TYPES
 * ======================================================================== */

#if TESTING_ENVIRONMENT
    /* Test Environment: Standard C types */
    typedef uint8_t     uint8;
    typedef uint16_t    uint16;
    typedef uint32_t    uint32;
    typedef uint64_t    uint64;
    typedef int8_t      sint8;
    typedef int16_t     sint16;
    typedef int32_t     sint32;
    typedef int64_t     sint64;
    typedef float       float32;
    typedef double      float64;
    typedef uint8_t     boolean;
    
    /* AUTOSAR Constants */
    #define TRUE        ((boolean)1)
    #define FALSE       ((boolean)0)
    #define STD_ON      1U
    #define STD_OFF     0U
    #define STD_HIGH    1U
    #define STD_LOW     0U
    
#else
    /* Target Environment: AUTOSAR types */
    #include "Std_Types.h"
#endif

/* ========================================================================
 * THREADX TYPES (Platform-Independent)
 * ======================================================================== */

#if TESTING_ENVIRONMENT
    /* Already defined in mocks/tx_api.h */
    #include "tx_api.h"
#else
    /* Target: Real ThreadX */
    #include "tx_api.h"
#endif

/* ========================================================================
 * STM32 HAL TYPES (Platform-Independent)
 * ======================================================================== */

#if TESTING_ENVIRONMENT
    /* Already defined in mocks/stm32_hal_i2c.h */
    #include "stm32_hal_i2c.h"
#else
    /* Target: Real STM32 HAL */
    #include "stm32u5xx_hal.h"
#endif

/* ========================================================================
 * APPLICATION-SPECIFIC CONSTANTS
 * ======================================================================== */

/* Motor Channel Definitions */
#define MOTOR_L_A       0
#define MOTOR_L_B       1
#define MOTOR_L_PWM     2
#define MOTOR_R_A       3
#define MOTOR_R_B       4
#define MOTOR_R_PWM     5

/* Servo Channel */
#define SERVO_CHANNEL   0

/* ========================================================================
 * SAFETY-CRITICAL MACROS (ISO 26262)
 * ======================================================================== */

/**
 * @brief Compile-time assertion
 * @note Prevents errors at compile time
 */
#define STATIC_ASSERT(condition, message) \
    _Static_assert(condition, message)

/**
 * @brief Runtime assertion (enabled in test only)
 */
#if TESTING_ENVIRONMENT
    #include <assert.h>
    #define RUNTIME_ASSERT(condition) assert(condition)
#else
    #define RUNTIME_ASSERT(condition) \
        do { if (!(condition)) { /* Log error */ } } while(0)
#endif

/**
 * @brief NULL pointer check
 */
#define CHECK_NULL_PTR(ptr) \
    do { \
        if ((ptr) == NULL) { \
            RUNTIME_ASSERT(false); \
            return HAL_ERROR; \
        } \
    } while(0)

/* ========================================================================
 * COMPILE-TIME TYPE VALIDATION (ISO 26262)
 * ======================================================================== */

/* Ensure types have expected sizes */
STATIC_ASSERT(sizeof(uint8) == 1,   "uint8 must be 1 byte");
STATIC_ASSERT(sizeof(uint16) == 2,  "uint16 must be 2 bytes");
STATIC_ASSERT(sizeof(uint32) == 4,  "uint32 must be 4 bytes");
STATIC_ASSERT(sizeof(sint8) == 1,   "sint8 must be 1 byte");
STATIC_ASSERT(sizeof(sint16) == 2,  "sint16 must be 2 bytes");
STATIC_ASSERT(sizeof(sint32) == 4,  "sint32 must be 4 bytes");
STATIC_ASSERT(sizeof(float32) == 4, "float32 must be 4 bytes");
STATIC_ASSERT(sizeof(float64) == 8, "float64 must be 8 bytes");
STATIC_ASSERT(sizeof(boolean) == 1, "boolean must be 1 byte");

/* ========================================================================
 * HELPER MACROS
 * ======================================================================== */

#define CLAMP(value, min, max) \
    (((value) < (min)) ? (min) : (((value) > (max)) ? (max) : (value)))

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#endif /* AUTOMOTIVE_TYPES_H */
