/* Servo Motor Control Header */
#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../test/mocks/stm32_hal_i2c.h"
#include <stdint.h>

/* PCA9685 PWM controller configuration */
#define PCA9685_ADDR_SERVO 0x40u
#define SERVO_CH 0u
#define SERVO_DEFAULT_MIN_PULSE_COUNTS 200u
#define SERVO_DEFAULT_MAX_PULSE_COUNTS 500u
#define SERVO_DEFAULT_FREQ_HZ 50u
#define PCA9685_COUNTS 4096u

/* Function prototypes */
int Servo_SetAngle(I2C_HandleTypeDef *hi2c,
                   uint8_t addr7,
                   uint8_t channel,
                   uint16_t angle_deg,
                   uint16_t min_pulse_counts,
                   uint16_t max_pulse_counts);

int Servo_Sweep(I2C_HandleTypeDef *hi2c,
                uint8_t addr7,
                uint8_t channel,
                uint16_t angle_start_deg,
                uint16_t angle_end_deg,
                uint16_t angle_step_deg,
                uint32_t delay_ms,
                uint16_t min_pulse_counts,
                uint16_t max_pulse_counts);

#ifdef __cplusplus
}
#endif

#endif /* SERVO_MOTOR_H */
