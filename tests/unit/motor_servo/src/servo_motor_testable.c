/* Testable Servo Motor functions from ThreadX */
#include "servo_motor_testable.h"

static inline uint16_t clamp_u16(int32_t v) {
    return (uint16_t)v;
}

int Servo_SetAngle_Testable(I2C_HandleTypeDef *hi2c,
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
