/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		PID.h
 * @brief   	:		This file includes the LL driver functions for BMI088 IMU
 * ______________________________________________________________________________________________________
 */

#ifndef __PID_H
#define __PID_H
#ifdef __cplusplus
extern "C" {
#endif


typedef struct _PIDSingle
{
	/* Controller gains */
	float Kp;
	float Ki;
	float Kd;

	float reference;
	float measured_value;
	float prev_measurement;
	float error;
	float error_sum;
	float error_deriv;
	float error_deriv_filter;

	float proportional;
	float integral;
	float derivative;

	float result;
}PIDSingle;

typedef struct _PIDDouble
{
	PIDSingle pid_in;
	PIDSingle pid_out;
}PIDDouble;


extern PIDDouble roll;
extern PIDDouble pitch;
extern PIDSingle yaw_heading;
extern PIDSingle yaw_rate;

void PIDController_Init(PIDSingle* axis);
void PIDController_Init_All(void);
void PIDController_Init_Constants(void);
void Double_Loop_PID_Calculation(PIDDouble* axis, float setpoint, float angle, float rate);
void Single_Loop_Heading_PID_Calculation(PIDSingle* axis, float setpoint, float angle, float rate);
void Single_Loop_Rate_PID_Calculation(PIDSingle* axis, float set_point_rate, float rate);
void Reset_PID_Integral(PIDSingle* axis);
void Reset_All_PID_Integral(void);


#ifdef __cplusplus
}
#endif
#endif /*__PID_H */
