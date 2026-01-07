/* Servo Motor functions - imported from ThreadX production code */
#include "servo_motor.h"
#include "pca9685_testable.h"
#include <stdint.h>

static inline uint16_t clamp_u16(int32_t v) {
    if (v < 0) return 0;
    if (v > (int32_t)(PCA9685_COUNTS - 1u)) return (uint16_t)(PCA9685_COUNTS - 1u);
    return (uint16_t)v;
}

int Servo_SetAngle(I2C_HandleTypeDef *hi2c,
                   uint8_t addr7,
                   uint8_t channel,
                   uint16_t angle_deg,
                   uint16_t min_pulse_counts,
                   uint16_t max_pulse_counts)
{
    if (angle_deg > 180u) angle_deg = 180u;
    if (min_pulse_counts >= max_pulse_counts) {
        return -1;
    }

    uint32_t range = (uint32_t)max_pulse_counts - (uint32_t)min_pulse_counts;
    uint32_t pulse = (uint32_t)min_pulse_counts + (range * angle_deg) / 180u;

    HAL_StatusTypeDef st = PCA9685_SetPWM_Testable(hi2c, addr7, channel, 0, clamp_u16((int32_t)pulse));
    if (st != HAL_OK) {
        return -2;
    }
    return 0;
}

int Servo_Sweep(I2C_HandleTypeDef *hi2c,
                uint8_t addr7,
                uint8_t channel,
                uint16_t angle_start_deg,
                uint16_t angle_end_deg,
                uint16_t angle_step_deg,
                uint32_t delay_ms,
                uint16_t min_pulse_counts,
                uint16_t max_pulse_counts)
{
    if (angle_step_deg == 0) angle_step_deg = 1;

    if (angle_start_deg <= angle_end_deg) {
        for (uint16_t angle = angle_start_deg; angle <= angle_end_deg; angle += angle_step_deg) {
            Servo_SetAngle(hi2c, addr7, channel, angle, min_pulse_counts, max_pulse_counts);
            if (angle + angle_step_deg < angle) break;
        }
    } else {
        for (int angle = (int)angle_start_deg; angle >= (int)angle_end_deg; angle -= (int)angle_step_deg) {
            Servo_SetAngle(hi2c, addr7, channel, (uint16_t)angle, min_pulse_counts, max_pulse_counts);
            if (angle - (int)angle_step_deg > angle) break;
        }
    }

    return 0;
}
