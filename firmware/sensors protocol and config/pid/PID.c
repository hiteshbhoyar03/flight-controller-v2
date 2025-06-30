/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		PID.c
 * @brief   	:		This file includes the LL driver functions for BMI088 IMU
 * ______________________________________________________________________________________________________
 */

#include "PID.h"

PIDDouble roll;
PIDDouble pitch;
PIDSingle yaw_rate;
PIDSingle yaw_heading;

#define SAMPLE_TIME 0.001f
#define OUTER_DERIV_FILT_ENABLE 1
#define INNER_DERIV_FILT_ENABLE 1
#define OUT_ERR_SUM_MAX 500
#define OUT_I_ERR_MIN -OUT_ERR_SUM_MAX
#define IN_ERR_SUM_MAX 500
#define IN_I_ERR_MIN -IN_ERR_SUM_MAX

void PIDController_Init(PIDSingle* axis)
{
	/* Clear controller variables */
	axis->integral = 0.0f;
	axis->error_sum  = 0.0f;

	axis->derivative  = 0.0f;
	axis->prev_measurement = 0.0f;

	axis->result = 0.0f;
}
void PIDController_Init_All(void)
{
	PIDController_Init(&roll.pid_in);
	PIDController_Init(&roll.pid_out);
	PIDController_Init(&pitch.pid_in);
	PIDController_Init(&pitch.pid_out);
	PIDController_Init(&yaw_heading);
	PIDController_Init(&yaw_rate);
}

void PIDController_Init_Constants(void)
{
	roll.pid_out.Kp  = 0.0f;
	roll.pid_out.Ki  = 0.0f;
	roll.pid_out.Kd  = 0.0f;

	roll.pid_in.Kp   = 0.0f;
	roll.pid_in.Ki   = 0.0f;
	roll.pid_in.Kp   = 0.0f;

	pitch.pid_out.Kp = 0.0f;
	pitch.pid_out.Ki = 0.0f;
	pitch.pid_out.Kd = 0.0f;

	pitch.pid_in.Kp  = 0.0f;
	pitch.pid_in.Ki  = 0.0f;
	pitch.pid_in.Kd  = 0.0f;

	yaw_heading.Kp   = 0.0f;
	yaw_heading.Ki   = 0.0f;
	yaw_heading.Kd   = 0.0f;

	yaw_rate.Kp      = 0.0f;
	yaw_rate.Ki      = 0.0f;
	yaw_rate.Kd      = 0.0f;
}
/*Rotation Angle*/
/*Angular Rate*/
void Double_Loop_PID_Calculation(PIDDouble* axis, float setpoint, float angle, float rate)
{
	/*********** Double PID Outer Begin (Roll and Pitch Angular Position Control) *************/
	axis->pid_out.reference = setpoint;				// Setpoint of outer PID control
	axis->pid_out.measured_value = angle;			// Rotation Angle

	/* Outer loop Error signal */
	axis->pid_out.error = axis->pid_out.reference - axis->pid_out.measured_value;

	/* Proportional outer loop */
	axis->pid_out.proportional = axis->pid_out.error * axis->pid_out.Kp;

	/* Summation of error for outer loop */
	axis->pid_out.error_sum = axis->pid_out.error_sum + axis->pid_out.error * SAMPLE_TIME;

	/* Anti-wind-up via integral clamping */
	if(axis->pid_out.error_sum > OUT_ERR_SUM_MAX) axis->pid_out.error_sum = OUT_ERR_SUM_MAX;
	else if(axis->pid_out.error_sum < OUT_I_ERR_MIN) axis->pid_out.error_sum = OUT_I_ERR_MIN;

	/* Integral outer loop */
	axis->pid_out.integral = axis->pid_out.error_sum * axis->pid_out.Ki;

	/* Derivative outer loop */
	axis->pid_out.error_deriv = -rate;

#if !OUTER_DERIV_FILT_ENABLE
	axis->pid_out.derivative = axis->pid_out.error_deriv * axis->pid_out.Kd;
#else
	axis->pid_out.error_deriv_filter = axis->pid_out.error_deriv_filter * 0.4f + axis->pid_out.error_deriv * 0.6f;
	axis->pid_out.derivative = axis->pid_out.error_deriv_filter * axis->pid_out.Kd;
#endif

	/* Compute PID result for outer loop */
	axis->pid_out.result = axis->pid_out.proportional + axis->pid_out.integral + axis->pid_out.derivative;

	/************ Double PID Inner Begin (Roll and Pitch Angular Rate Control) **************/
	axis->pid_in.reference = axis->pid_out.result;	//Set point of inner PID control is the PID result of outer loop (for double PID control)
	axis->pid_in.measured_value = rate;					//ICM-20602 angular rate

	/* Inner loop Error signal */
	axis->pid_in.error = axis->pid_in.reference - axis->pid_in.measured_value;

	/* Proportional inner loop */
	axis->pid_in.proportional = axis->pid_in.error * axis->pid_in.Kp;

	/* Summation of error for inner loop */
	axis->pid_in.error_sum = axis->pid_in.error_sum + axis->pid_in.error * SAMPLE_TIME;

	/* Anti-wind-up via integral clamping */
	if(axis->pid_out.error_sum > IN_ERR_SUM_MAX) axis->pid_out.error_sum = IN_ERR_SUM_MAX;
	else if(axis->pid_out.error_sum < IN_I_ERR_MIN) axis->pid_out.error_sum = IN_I_ERR_MIN;

	/* Integral inner loop */
	axis->pid_in.integral = axis->pid_in.error_sum * axis->pid_in.Ki;

	/* Derivative inner loop */
	axis->pid_in.error_deriv = -(axis->pid_in.measured_value - axis->pid_in.prev_measurement) / SAMPLE_TIME;
	axis->pid_in.prev_measurement = axis->pid_in.measured_value;

#if !INNER_DERIV_FILT_ENABLE
	axis->pid_in.derivative = axis->pid_in.error_deriv * axis->pid_in.Kd;
#else
	axis->pid_in.error_deriv_filter = axis->pid_in.error_deriv_filter * 0.5f + axis->pid_in.error_deriv * 0.5f;
	axis->pid_in.derivative = axis->pid_in.error_deriv_filter * axis->pid_in.Kd;
#endif

	/* Compute PID result for inner loop */
	axis->pid_in.result = axis->pid_in.proportional + axis->pid_in.integral + axis->pid_in.derivative; //Calculate PID result of inner loop
}

void Single_Loop_Heading_PID_Calculation(PIDSingle* axis, float setpoint, float angle, float rate)
{
	axis->reference = setpoint;							// Setpoint of outer PID control
	axis->measured_value = angle;						// Rotation Angle

	axis->error = axis->reference - axis->measured_value;					// Error signal

	if(axis->error > 180.f) axis->error -= 360.f;
	else if(axis->error < -180.f) axis->error += 360.f;

	axis->proportional = axis->error * axis->Kp;					// Proportional 
	axis->error_sum = axis->error_sum + axis->error * SAMPLE_TIME;	// Summation of error
	axis->integral = axis->error_sum * axis->Ki;					// Integral

	axis->error_deriv = -rate;
	axis->derivative = axis->error_deriv * axis->Kd;

	axis->result = axis->proportional + axis->integral + axis->derivative; //Calculate PID result of yaw measured_value control
}

void Single_Loop_Rate_PID_Calculation(PIDSingle* axis, float setpoint, float rate)
{
	axis->reference = setpoint;
	axis->measured_value = rate;

	axis->error = axis->reference - axis->measured_value;
	axis->proportional = axis->error * axis->Kp;

	axis->error_sum = axis->error_sum + axis->error * SAMPLE_TIME;
	axis->integral = axis->error_sum * axis->Ki;

	axis->error_deriv = -(axis->measured_value - axis->prev_measurement) / SAMPLE_TIME;
	axis->prev_measurement = axis->measured_value;
	axis->derivative = axis->error_deriv * axis->Kd;

	axis->result = axis->proportional + axis->integral + axis->derivative;
}

void Reset_PID_Integral(PIDSingle* axis)
{
	axis->error_sum = 0;
}

void Reset_All_PID_Integral(void)
{
	Reset_PID_Integral(&roll.pid_in);
	Reset_PID_Integral(&roll.pid_out);
	Reset_PID_Integral(&pitch.pid_in);
	Reset_PID_Integral(&pitch.pid_out);
	Reset_PID_Integral(&yaw_heading);
	Reset_PID_Integral(&yaw_rate);
}
