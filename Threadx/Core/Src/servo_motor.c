#include "app_threadx.h"
#include "servo_motor.h"
#include "pca9685.h"

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

    HAL_StatusTypeDef st = PCA9685_SetPWM(hi2c, addr7, channel, 0, clamp_u16((int32_t)pulse));
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

    double freq = (double)SERVO_DEFAULT_FREQ_HZ;
    if (PCA9685_SetFrequency(hi2c, addr7, freq) != HAL_OK) {
        const char *msg = "Servo_Sweep: Failed to set frequency, continuing anyway\r\n";
        HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    }

    if (angle_step_deg == 0) angle_step_deg = 1;

    if (angle_start_deg <= angle_end_deg) {
        for (uint16_t angle = angle_start_deg; angle <= angle_end_deg; angle += angle_step_deg) {
            Servo_SetAngle(hi2c, addr7, channel, angle, min_pulse_counts, max_pulse_counts);
            HAL_Delay(delay_ms);
            if (angle > UINT16_MAX - angle_step_deg)
                break;
        }
    } else {
        for (int angle = (int)angle_start_deg; angle >= (int)angle_end_deg; angle -= (int)angle_step_deg) {
            Servo_SetAngle(hi2c, addr7, channel, (uint16_t)angle, min_pulse_counts, max_pulse_counts);
            HAL_Delay(delay_ms);
            if (angle - (int)angle_step_deg > angle) break;
        }
    }

    const char *msg = "Servo_Sweep: complete\r\n";
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    return 0;
}

VOID servo_motor(ULONG initial_input)
{
	t_can_message	msg;
    ULONG 			actual_flags;

    while (1)
    {
        tx_event_flags_get(&event_flags, FLAG_CAN_STEER_CMD,
        TX_OR_CLEAR, &actual_flags, TX_NO_WAIT);

        while (tx_queue_receive(&queue_steer_cmd, &msg, TX_NO_WAIT) == TX_SUCCESS)
        {
            float angle_f = *((float*)msg.data);
            uint16_t angle = (uint16_t)angle_f;
            (void)Servo_SetAngle(&hi2c3, PCA9685_ADDR_SERVO, SERVO_CH,
                                 angle, SERVO_DEFAULT_MIN_PULSE_COUNTS, SERVO_DEFAULT_MAX_PULSE_COUNTS);
        }
        tx_thread_sleep(50);
   }
}
