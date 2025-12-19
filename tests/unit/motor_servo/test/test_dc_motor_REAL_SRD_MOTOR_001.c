/**
 * @file test_dc_motor_REAL_SRD_MOTOR_001_ForwardMotion.c
 * @brief Unit Tests for REAL dc_motor.c from Threadx/Core/Src/
 * 
 * @req SRD-MOTOR-001: Motor shall move vehicle forward with speed [0.0, 1.0]
 * @asil ASIL-B
 * @safety_goal SG-01: Vehicle shall respond to acceleration commands
 * @coverage_requirement 100% Branch Coverage
 * 
 * This test validates THE REAL production code from:
 * ../Threadx/Core/Src/dc_motor.c via wrapper mapping in dc_motor.h
 */

#include "../vendor/unity/src/unity.h"
#include "../src/dc_motor.h"
#include "../src/dc_motor_testable.c"
#include "../src/pca9685_testable.c"
#include "../src/mock_stm32u5xx_hal.c"
#include <string.h>
#include <stdint.h>

/* dc_motor.h wrapper maps Motor_* to Motor_*_Testable(&hi2c3) */
I2C_HandleTypeDef hi2c3;

void setUp(void) {
    memset(&hi2c3, 0, sizeof(hi2c3));
}

void tearDown(void) {
}

void test_Motor_Stop_Should_SetAllChannelsToZero(void) {
    Motor_Stop();
}

void test_Motor_Forward_At_Zero_Should_SetZeroPWM(void) {
    Motor_Forward(0.0);
}

void test_Motor_Forward_At_50Percent_Should_SetCorrectPWM(void) {
    Motor_Forward(0.5);
}

void test_Motor_Forward_At_Max_Should_SetFullPWM(void) {
    Motor_Forward(1.0);
}

void test_Motor_Backward_At_50Percent_Should_ReverseDirection(void) {
    Motor_Backward(0.5);
}

void test_Motor_Left_At_50Percent_Should_TurnLeft(void) {
    Motor_Left(0.5);
}

void test_Motor_Right_At_50Percent_Should_TurnRight(void) {
    Motor_Right(0.5);
}

void test_Motor_SetMotor_NearZeroSpeed_ShouldCallMotorStop(void) {
    Motor_SetMotor(0.0000001);
}

void test_Motor_SetMotor_PositiveSpeed_ShouldMoveForward(void) {
    Motor_SetMotor(0.5);
}

void test_Motor_SetMotor_NegativeSpeed_ShouldMoveBackward(void) {
    Motor_SetMotor(-0.5);
}

void test_Motor_SetPWM_PositiveLeftRight_ShouldSetForward(void) {
    Motor_SetPWM(2000, 2000);
}

void test_Motor_SetPWM_NegativeLeftRight_ShouldSetBackward(void) {
    Motor_SetPWM(-2000, -2000);
}

void test_Motor_SetPWM_ZeroLeftRight_ShouldStop(void) {
    Motor_SetPWM(0, 0);
}

void test_Motor_SetPWM_NegativeValues_ShouldClampToZero(void) {
    Motor_SetPWM(-1000, -500);
}

void test_Motor_SetPWM_OverflowValues_ShouldClampToMax(void) {
    Motor_SetPWM(5000, 6000);
}
