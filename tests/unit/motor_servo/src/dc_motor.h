#ifndef UNIT_DC_MOTOR_H_WRAPPER
#define UNIT_DC_MOTOR_H_WRAPPER

/* Pull in unit test stubs before real header to satisfy types/macros */
#include "dc_motor_testable.h"

/* Map production function names to testable implementations */
#define Motor_Stop( )                  Motor_Stop_Testable(&hi2c3)
#define Motor_Forward(speed)           Motor_Forward_Testable(&hi2c3, (speed))
#define Motor_Backward(speed)          Motor_Backward_Testable(&hi2c3, (speed))
#define Motor_Left(speed)              Motor_Left_Testable(&hi2c3, (speed))
#define Motor_Right(speed)             Motor_Right_Testable(&hi2c3, (speed))
#define Motor_SetMotor(value)          Motor_SetMotor_Testable(&hi2c3, (value))
#define Motor_SetPWM(left,right)       Motor_SetPWM_Testable(&hi2c3, (left), (right))

#endif /* UNIT_DC_MOTOR_H_WRAPPER */
