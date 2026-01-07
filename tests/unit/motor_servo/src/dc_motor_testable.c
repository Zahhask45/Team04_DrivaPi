/* DC Motor functions - imported from ThreadX production code */
#include "dc_motor_testable.h"

static inline uint16_t clamp_u16(int32_t v) {
    if (v < 0) return 0;
    if (v >= (int32_t)PCA9685_COUNTS) return (uint16_t)(PCA9685_COUNTS - 1u);
    return (uint16_t)v;
}

void Motor_Stop_Testable(I2C_HandleTypeDef *hi2c) {
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_A, 0, 0);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_B, 0, 0);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_PWM, 0, 0);

    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_A, 0, 0);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_B, 0, 0);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_PWM, 0, 0);
}

void Motor_Forward_Testable(I2C_HandleTypeDef *hi2c, double speed) {
    uint16_t max = 4095;
    uint16_t pwm_val = (uint16_t)(speed * max);

    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_A, 0, max);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_B, 0, 0);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_PWM, 0, pwm_val);

    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_A, 0, 0);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_B, 0, max);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_PWM, 0, pwm_val);
}

void Motor_Backward_Testable(I2C_HandleTypeDef *hi2c, double speed) {
    uint16_t max = 4095;
    uint16_t pwm_val = (uint16_t)(speed * max);

    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_A, 0, 0);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_B, 0, max);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_PWM, 0, pwm_val);

    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_A, 0, max);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_B, 0, 0);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_PWM, 0, pwm_val);
}

void Motor_Left_Testable(I2C_HandleTypeDef *hi2c, double speed) {
    const uint16_t max = (uint16_t)(PCA9685_COUNTS - 1u);
    uint16_t pwm_val = clamp_u16((int32_t)(speed * (double)max + 0.5));

    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_A, 0, max);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_B, 0, 0);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_PWM, 0, pwm_val);

    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_A, 0, 0);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_B, 0, max);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_PWM, 0, pwm_val);
}

void Motor_Right_Testable(I2C_HandleTypeDef *hi2c, double speed) {
    const uint16_t max = (uint16_t)(PCA9685_COUNTS - 1u);
    uint16_t pwm_val = clamp_u16((int32_t)(speed * (double)max + 0.5));

    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_A, 0, 0);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_B, 0, max);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_PWM, 0, pwm_val);

    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_A, 0, max);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_B, 0, 0);
    PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_PWM, 0, pwm_val);
}

void Motor_SetMotor_Testable(I2C_HandleTypeDef *hi2c, double value) {
    const uint16_t max = (uint16_t)(PCA9685_COUNTS - 1u);
    uint16_t pwm_val = clamp_u16((int32_t)(fabs(value) * (double)max + 0.5));

    if (fabs(value) < 1e-6) {
        Motor_Stop_Testable(hi2c);
        return;
    }

    if (value > 0.0) {
        // Forward
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_A, 0, max);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_B, 0, 0);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_PWM, 0, pwm_val);

        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_A, 0, 0);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_B, 0, max);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_PWM, 0, pwm_val);
    } else {
        // Backward
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_A, 0, 0);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_B, 0, max);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_PWM, 0, pwm_val);

        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_A, 0, max);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_B, 0, 0);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_PWM, 0, pwm_val);
    }
}

void Motor_SetPWM_Testable(I2C_HandleTypeDef *hi2c, int32_t left_counts, int32_t right_counts) {
    const uint16_t max = (uint16_t)(PCA9685_COUNTS - 1u);

    /* Left motor */
    if (left_counts > 0) {
        uint16_t pwm = clamp_u16(left_counts);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_A, 0, max);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_B, 0, 0);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_PWM, 0, pwm);
    } else if (left_counts < 0) {
        uint16_t pwm = clamp_u16(-left_counts);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_A, 0, 0);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_B, 0, max);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_PWM, 0, pwm);
    } else {
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_A, 0, 0);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_B, 0, 0);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_L_PWM, 0, 0);
    }

    /* Right motor */
    if (right_counts > 0) {
        uint16_t pwm = clamp_u16(right_counts);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_A, 0, 0);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_B, 0, max);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_PWM, 0, pwm);
    } else if (right_counts < 0) {
        uint16_t pwm = clamp_u16(-right_counts);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_A, 0, max);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_B, 0, 0);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_PWM, 0, pwm);
    } else {
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_A, 0, 0);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_B, 0, 0);
        PCA9685_SetPWM_Testable(hi2c, PCA9685_ADDR_MOTOR, MOTOR_R_PWM, 0, 0);
    }
}
