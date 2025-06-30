/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		DRONE_CONFIG.c
 * @brief   	:		This file includes the LL driver functions for DShOT Protocol
 * ______________________________________________________________________________________________________
 */

#include "DRONE_CONFIG.h"


#if QUADCOPTER_X8
/*QUAD X8 motor mixing (X-configuration)
															Front
															  ↑
												CW		M1↑       M2↑	CCW
												CCW		M5↓       M6↓	CW

												CCW		M4↑       M3↑	CW
												CW		M8↓       M7↓	CCW
															  ↓
															 Back
 */

//	                        		M1       M2     M3      M4      M5      M6      M7      M8
const float roll_factor[8]  = { 	+1,     -1,     -1,     +1,		+1,     -1,     -1,     +1 };
const float pitch_factor[8] = {		-1,		-1,		+1,     +1,		-1,		-1,		+1,     +1 };
const float yaw_factor[8]   = { 	-1,     +1,     -1,     +1,		+1,     -1,     +1,     -1 };

#define NUM_MOTOR  8

/**
 * @brief  Configure timer for pwm
 * @param  protocol		if oneshot125 psc = 2-1 else 1-1
 * @param  arr			protocol arr value
 * @retval No return value
 */
void Pwm_Init(uint32_t protocol, uint32_t arr)
{

	if(protocol == 125){ // oneshot125
		TIM1->PSC = 1;
		TIM8->PSC = 1;

		TIM1->ARR = arr;
		TIM8->ARR = arr;
	}
	else{
		TIM1->PSC = 0;
		TIM8->PSC = 0;

		TIM1->ARR = arr;
		TIM8->ARR = arr;
	}
	Enable_Pwm_Channels(TIM1);
	Enable_Pwm_Channels(TIM8);
}
/**
 * @brief  Configure timer for pwm
 * @param  dshot_type
 * @retval No return value
 */
void Enable_Dshot_Channels(enum_dshot_type dshot_type)
{
	Dshot_Init(TIM1 ,dshot_type, timer1);
	Dshot_Init(TIM8 ,dshot_type, timer8);
}
/**
 * @brief  Set motor values to timer
 * @param  motor		Pointer to motor Values
 * @param  max_ccr		max value for Capture/Compare Register
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Pwm_Set(uint32_t * motor, uint32_t max_ccr, uint32_t min_ccr, uint32_t motor_offset){
	TIM1->CCR1 = motor[0]  > max_ccr ? max_ccr : motor[0]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[0];
	TIM1->CCR2 = motor[1]  > max_ccr ? max_ccr : motor[1]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[1];
	TIM1->CCR3 = motor[2]  > max_ccr ? max_ccr : motor[2]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[2];
	TIM1->CCR4 = motor[3]  > max_ccr ? max_ccr : motor[3]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[3];
	TIM8->CCR1 = motor[4]  > max_ccr ? max_ccr : motor[4]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[4];
	TIM8->CCR2 = motor[5]  > max_ccr ? max_ccr : motor[5]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[5];
	TIM8->CCR3 = motor[6]  > max_ccr ? max_ccr : motor[6]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[6];
	TIM8->CCR4 = motor[7]  > max_ccr ? max_ccr : motor[7]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[7];
}
/**
 * @brief  ReSet motor values of timer
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Pwm_Reset(uint32_t min_ccr, uint32_t motor_offset){
	TIM1->CCR1 = min_ccr+motor_offset;
	TIM1->CCR2 = min_ccr+motor_offset;
	TIM1->CCR3 = min_ccr+motor_offset;
	TIM1->CCR4 = min_ccr+motor_offset;
	TIM8->CCR1 = min_ccr+motor_offset;
	TIM8->CCR2 = min_ccr+motor_offset;
	TIM8->CCR3 = min_ccr+motor_offset;
	TIM8->CCR4 = min_ccr+motor_offset;
}
/**
 * @brief  Dshot Set motor values to timer
 * @param  motor		Pointer to motor Values
 * @param  max_ccr		max value for Capture/Compare Register
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Dshot_Set(uint32_t * motor, uint32_t max_ccr, uint32_t min_ccr, uint32_t motor_offset){
	uint16_t motor_val[16] = {0};
	for(int i=0; i<NUM_MOTOR;i++){
		motor_val[i] = motor[i]  > max_ccr ? max_ccr : motor[i]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[i];
	}
	Dshot_Write(&motor_val[0], timer1);
	Dshot_Write(&motor_val[4], timer8);
}
/**
 * @brief  Dshot ReSet motor values of timer
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Dshot_Reset( uint32_t min_ccr, uint32_t motor_offset){
	uint16_t motor_val[16] = {0};
	for(int i=0; i<NUM_MOTOR;i++){
		motor_val[i] = min_ccr+motor_offset;
	}
	Dshot_Write(&motor_val[0], timer1);
	Dshot_Write(&motor_val[4], timer8);
}

#elif HEXACOPTER_X6
/*HEX X6 motor mixing (X-configuration)
															Front
															  ↑

													M1 (CW)       M2 (CCW)

											M6 (CCW)     	  . 			M3 (CW)

													M5 (CW)       M4 (CCW)
															  ↓
															 Back

 */
//                        		  M1       M2       M3      M4      M5      M6
const float roll_factor[6]  = { +0.5,	 -0.5,   -1.0,     -0.5  , +0.5  ,  +1.0 };
const float pitch_factor[6] = { -0.866,  -0.866,  0.0,     +0.866, +0.866,   0.0 };
const float yaw_factor[6]   = { -1,      +1,     -1,       +1,     -1,      +1   };

#define NUM_MOTOR  6

/**
 * @brief  Configure timer for pwm
 * @param  protocol		if oneshot125 psc = 2-1 else 1-1
 * @param  arr			protocol arr value
 * @retval No return value
 */
void Pwm_Init(uint32_t protocol, uint32_t arr)
{
	if(protocol == 125){ // oneshot125
		TIM1->PSC = 1;
		TIM8->PSC = 1;

		TIM1->ARR = arr;
		TIM8->ARR = arr;
	}
	else{
		TIM1->PSC = 0;
		TIM8->PSC = 0;

		TIM1->ARR = arr;
		TIM8->ARR = arr;
	}
	Enable_Pwm_Channels(TIM1);
	Enable_Pwm_Channels(TIM8);
}
/**
 * @brief  Configure timer for pwm
 * @param  dshot_type
 * @retval No return value
 */
void Enable_Dshot_Channels(enum_dshot_type dshot_type)
{
	Dshot_Init(TIM1 ,dshot_type, timer1);
	Dshot_Init(TIM8 ,dshot_type, timer8);
}
/**
 * @brief  Set motor values to timer
 * @param  motor		Pointer to motor Values
 * @param  max_ccr		max value for Capture/Compare Register
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Pwm_Set(uint32_t * motor, uint32_t max_ccr, uint32_t min_ccr, uint32_t motor_offset){
	TIM1->CCR1 = motor[0]  > max_ccr ? max_ccr : motor[0]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[0];
	TIM1->CCR2 = motor[1]  > max_ccr ? max_ccr : motor[1]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[1];
	TIM1->CCR3 = motor[2]  > max_ccr ? max_ccr : motor[2]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[2];
	TIM1->CCR4 = motor[3]  > max_ccr ? max_ccr : motor[3]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[3];
	TIM8->CCR1 = motor[4]  > max_ccr ? max_ccr : motor[4]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[4];
	TIM8->CCR2 = motor[5]  > max_ccr ? max_ccr : motor[5]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[5];
}
/**
 * @brief  ReSet motor values of timer
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Pwm_Reset(uint32_t min_ccr, uint32_t motor_offset){
	TIM1->CCR1 = min_ccr+motor_offset;
	TIM1->CCR2 = min_ccr+motor_offset;
	TIM1->CCR3 = min_ccr+motor_offset;
	TIM1->CCR4 = min_ccr+motor_offset;
	TIM8->CCR1 = min_ccr+motor_offset;
	TIM8->CCR2 = min_ccr+motor_offset;
}
/**
 * @brief  Dshot Set motor values to timer
 * @param  motor		Pointer to motor Values
 * @param  max_ccr		max value for Capture/Compare Register
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Dshot_Set(uint32_t * motor, uint32_t max_ccr, uint32_t min_ccr, uint32_t motor_offset){
	uint16_t motor_val[16] = {0};
	for(int i=0; i<NUM_MOTOR;i++){
		motor_val[i] = motor[i]  > max_ccr ? max_ccr : motor[i]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[i];
	}
	Dshot_Write(&motor_val[0], timer1);
	Dshot_Write(&motor_val[4], timer8);
}
/**
 * @brief  Dshot ReSet motor values of timer
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Dshot_Reset( uint32_t min_ccr, uint32_t motor_offset){
	uint16_t motor_val[16] = {0};
	for(int i=0; i<NUM_MOTOR;i++){
		motor_val[i] = min_ccr+motor_offset;
	}
	Dshot_Write(&motor_val[0], timer1);
	Dshot_Write(&motor_val[4], timer8);
}

#elif HEXACOPTER_X12
/*HEX X12 motor mixing (X-configuration)
															 Front
															   ↑

													M1 (CW)↑      M2 (CCW)↑
													M7 (CCW)↓     M8 (CW)↓

											M6 (CCW)↑     	   . 			M3 (CW)↑
											M12(CW)↓     	   . 			M9 (CCW)↓

													M5 (CW)↑      M4 (CCW)↑
													M11(CCW)↑     M10(CW)↓
															   ↓
															  Back

 */
//                         		 M1       M2       M3      M4      M5      M6      M7      M8      M9      M10     M11     M12
const float roll_factor[12]  = { +0.5,	-0.5,  	-1.0,	-0.5  , +0.5  , +1.0 ,	+0.5,	-0.5,  	-1.0,	-0.5  , +0.5,   +1.0 };
const float pitch_factor[12] = { -0.866,  -0.866,  0.0,   +0.866, +0.866,  0.0 ,	-0.866, -0.866,  0.0,   +0.866, +0.866,  0.0 };
const float yaw_factor[12]   = { -1,      +1,     -1,     +1,     -1,     +1,     -1,     +1,     -1,     +1,     -1,     +1   };

#define NUM_MOTOR  12

/**
 * @brief  Configure timer for pwm
 * @param  protocol		if oneshot125 psc = 2-1 else 1-1
 * @param  arr			protocol arr value
 * @retval No return value
 */
void Pwm_Init(uint32_t protocol, uint32_t arr)
{
	if(protocol == 125){ // oneshot125
		TIM1->PSC = 1;
		TIM8->PSC = 1;
		TIM5->PSC = 1;

		TIM1->ARR = arr;
		TIM8->ARR = arr;
		TIM5->ARR = arr;
	}
	else{
		TIM1->PSC = 0;
		TIM8->PSC = 0;
		TIM5->PSC = 0;

		TIM1->ARR = arr;
		TIM8->ARR = arr;
		TIM5->ARR = arr;
	}
	Enable_Pwm_Channels(TIM1);
	Enable_Pwm_Channels(TIM8);
	Enable_Pwm_Channels(TIM5);
}
/**
 * @brief  Configure timer for pwm
 * @param  dshot_type
 * @retval No return value
 */
void Enable_Dshot_Channels(enum_dshot_type dshot_type)
{
	Dshot_Init(TIM1 ,dshot_type, timer1);
	Dshot_Init(TIM8 ,dshot_type, timer8);
	Dshot_Init(TIM5 ,dshot_type, timer5);
}
/**
 * @brief  Set motor values to timer
 * @param  motor		Pointer to motor Values
 * @param  max_ccr		max value for Capture/Compare Register
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Pwm_Set(uint32_t * motor, uint32_t max_ccr, uint32_t min_ccr, uint32_t motor_offset){
	TIM1->CCR1 = motor[0]  > max_ccr ? max_ccr : motor[0]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[0];
	TIM1->CCR2 = motor[1]  > max_ccr ? max_ccr : motor[1]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[1];
	TIM1->CCR3 = motor[2]  > max_ccr ? max_ccr : motor[2]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[2];
	TIM1->CCR4 = motor[3]  > max_ccr ? max_ccr : motor[3]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[3];
	TIM8->CCR1 = motor[4]  > max_ccr ? max_ccr : motor[4]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[4];
	TIM8->CCR2 = motor[5]  > max_ccr ? max_ccr : motor[5]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[5];
	TIM8->CCR3 = motor[6]  > max_ccr ? max_ccr : motor[6]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[6];
	TIM8->CCR4 = motor[7]  > max_ccr ? max_ccr : motor[7]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[7];
	TIM5->CCR1 = motor[8]  > max_ccr ? max_ccr : motor[8]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[8];
	TIM5->CCR2 = motor[9]  > max_ccr ? max_ccr : motor[9]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[9];
	TIM5->CCR3 = motor[10] > max_ccr ? max_ccr : motor[10] < min_ccr+motor_offset ? min_ccr+motor_offset : motor[10];
	TIM5->CCR4 = motor[11] > max_ccr ? max_ccr : motor[11] < min_ccr+motor_offset ? min_ccr+motor_offset : motor[11];
}
/**
 * @brief  ReSet motor values of timer
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Pwm_Reset(uint32_t min_ccr, uint32_t motor_offset){
	TIM1->CCR1 = min_ccr+motor_offset;
	TIM1->CCR2 = min_ccr+motor_offset;
	TIM1->CCR3 = min_ccr+motor_offset;
	TIM1->CCR4 = min_ccr+motor_offset;
	TIM8->CCR1 = min_ccr+motor_offset;
	TIM8->CCR2 = min_ccr+motor_offset;
	TIM8->CCR3 = min_ccr+motor_offset;
	TIM8->CCR4 = min_ccr+motor_offset;
	TIM5->CCR1 = min_ccr+motor_offset;
	TIM5->CCR2 = min_ccr+motor_offset;
	TIM5->CCR3 = min_ccr+motor_offset;
	TIM5->CCR4 = min_ccr+motor_offset;
}
/**
 * @brief  Dshot Set motor values to timer
 * @param  motor		Pointer to motor Values
 * @param  max_ccr		max value for Capture/Compare Register
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Dshot_Set(uint32_t * motor, uint32_t max_ccr, uint32_t min_ccr, uint32_t motor_offset){
	uint16_t motor_val[16] = {0};
	for(int i=0; i<NUM_MOTOR;i++){
		motor_val[i] = motor[i]  > max_ccr ? max_ccr : motor[i]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[i];
	}
	Dshot_Write(&motor_val[0], timer1);
	Dshot_Write(&motor_val[4], timer8);
	Dshot_Write(&motor_val[8], timer5);
}
/**
 * @brief  Dshot ReSet motor values of timer
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Dshot_Reset( uint32_t min_ccr, uint32_t motor_offset){
	uint16_t motor_val[16] = {0};
	for(int i=0; i<NUM_MOTOR;i++){
		motor_val[i] = min_ccr+motor_offset;
	}
	Dshot_Write(&motor_val[0], timer1);
	Dshot_Write(&motor_val[4], timer8);
	Dshot_Write(&motor_val[8], timer5);
}

#elif OCTOCOPTER_X8
/*OCTO X8 motor mixing (X-configuration)
								Front
								   ↑
								(M1 CW)

						(M8 CCW)   			(M2 CCW)

  				(M7 CW)   						(M3 CW)

						(M6 CCW) 			(M4 CCW)

								(M5 CW)
								   ↓
								Back
 */
//                          M1       M2       M3      M4      M5      M6      M7      M8
const float roll_factor[8]  = {  0.0,     -0.707, -1.0,   -0.707,  0.0, +0.707,   +1.0,   +0.707 };
const float pitch_factor[8] = { -1.0, 	-0.707,	 0.0,  	+0.707,	+1.0, +0.707, 	 0.0,  	-0.707 };
const float yaw_factor[8]   = { -1,       +1,     -1,     +1,     -1,   +1,       -1,     +1,    };

#define NUM_MOTOR  8

/**
 * @brief  Configure timer for pwm
 * @param  protocol		if oneshot125 psc = 2-1 else 1-1
 * @param  arr			protocol arr value
 * @retval No return value
 */
void Pwm_Init(uint32_t protocol, uint32_t arr)
{
	if(protocol == 125){ // oneshot125
		TIM1->PSC = 1;
		TIM8->PSC = 1;

		TIM1->ARR = arr;
		TIM8->ARR = arr;
	}
	else{
		TIM1->PSC = 0;
		TIM8->PSC = 0;

		TIM1->ARR = arr;
		TIM8->ARR = arr;
	}
	Enable_Pwm_Channels(TIM1);
	Enable_Pwm_Channels(TIM8);
}
/**
 * @brief  Configure timer for pwm
 * @param  dshot_type
 * @retval No return value
 */
void Enable_Dshot_Channels(enum_dshot_type dshot_type)
{
	Dshot_Init(TIM1 ,dshot_type, timer1);
	Dshot_Init(TIM8 ,dshot_type, timer8);
}
/**
 * @brief  Set motor values to timer
 * @param  motor		Pointer to motor Values
 * @param  max_ccr		max value for Capture/Compare Register
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Pwm_Set(uint32_t * motor, uint32_t max_ccr, uint32_t min_ccr, uint32_t motor_offset){
	TIM1->CCR1 = motor[0]  > max_ccr ? max_ccr : motor[0]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[0];
	TIM1->CCR2 = motor[1]  > max_ccr ? max_ccr : motor[1]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[1];
	TIM1->CCR3 = motor[2]  > max_ccr ? max_ccr : motor[2]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[2];
	TIM1->CCR4 = motor[3]  > max_ccr ? max_ccr : motor[3]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[3];
	TIM8->CCR1 = motor[4]  > max_ccr ? max_ccr : motor[4]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[4];
	TIM8->CCR2 = motor[5]  > max_ccr ? max_ccr : motor[5]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[5];
	TIM8->CCR3 = motor[6]  > max_ccr ? max_ccr : motor[6]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[6];
	TIM8->CCR4 = motor[7]  > max_ccr ? max_ccr : motor[7]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[7];
}
/**
 * @brief  ReSet motor values of timer
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Pwm_Reset(uint32_t min_ccr, uint32_t motor_offset){
	TIM1->CCR1 = min_ccr+motor_offset;
	TIM1->CCR2 = min_ccr+motor_offset;
	TIM1->CCR3 = min_ccr+motor_offset;
	TIM1->CCR4 = min_ccr+motor_offset;
	TIM8->CCR1 = min_ccr+motor_offset;
	TIM8->CCR2 = min_ccr+motor_offset;
	TIM8->CCR3 = min_ccr+motor_offset;
	TIM8->CCR4 = min_ccr+motor_offset;
}
/**
 * @brief  Dshot Set motor values to timer
 * @param  motor		Pointer to motor Values
 * @param  max_ccr		max value for Capture/Compare Register
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Dshot_Set(uint32_t * motor, uint32_t max_ccr, uint32_t min_ccr, uint32_t motor_offset){
	uint16_t motor_val[16] = {0};
	for(int i=0; i<NUM_MOTOR;i++){
		motor_val[i] = motor[i]  > max_ccr ? max_ccr : motor[i]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[i];
	}
	Dshot_Write(&motor_val[0], timer1);
	Dshot_Write(&motor_val[4], timer8);
}
/**
 * @brief  Dshot ReSet motor values of timer
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Dshot_Reset( uint32_t min_ccr, uint32_t motor_offset){
	uint16_t motor_val[16] = {0};
	for(int i=0; i<NUM_MOTOR;i++){
		motor_val[i] = min_ccr+motor_offset;
	}
	Dshot_Write(&motor_val[0], timer1);
	Dshot_Write(&motor_val[4], timer8);
}

#elif OCTOCOPTER_X16
/*QUAD X16 motor mixing (X-configuration)
								Front
								   ↑
								(M1 CW)
								(M9 CCW)

						(M8 CCW)   			(M2 CCW)
						(M16 CW)   			(M10 CW)

  				(M7 CW)   						(M3 CW)
  				(M15 CCW)   						(M11 CCW)

						(M6 CCW) 			(M4 CCW)
						(M14 CW) 			(M12 CW)

								(M5 CW)
								(M13 CCW)
								   ↓
								Back
 */
//                               M1       M2       M3      M4      M5      M6      M7      M8        M9      M10    M11      M12    M13     M14      M15      M16
const float roll_factor[16]  = {  0.0,   -0.707, -1.0,   -0.707,  0.0, +0.707,   +1.0,   +0.707,	 0.0,   -0.707, -1.0,   -0.707,  0.0, +0.707,   +1.0,   +0.707 };
const float pitch_factor[16] = { -1.0,   -0.707,  0.0,	 +0.707, +1.0, +0.707,    0.0,   -0.707,	-1.0, 	-0.707,	 0.0,  	+0.707,	+1.0, +0.707, 	 0.0,  	-0.707 };
const float yaw_factor[16]   = { -1,	 +1,     -1,     +1,     -1,   +1,       -1,     +1,		+1,     -1,     +1,     -1,   	+1,   -1,       +1,     -1     };

#define NUM_MOTOR  16

/**
 * @brief  Configure timer for pwm
 * @param  protocol		if oneshot125 psc = 2-1 else 1-1
 * @param  arr			protocol arr value
 * @retval No return value
 */
void Pwm_Init(uint32_t protocol, uint32_t arr)
{
	if(protocol == 125){ // oneshot125
		TIM1->PSC = 1;
		TIM8->PSC = 1;
		TIM5->PSC = 1;
		TIM4->PSC = 1;

		TIM1->ARR = arr;
		TIM8->ARR = arr;
		TIM5->ARR = arr;
		TIM4->ARR = arr;
	}
	else{
		TIM1->PSC = 0;
		TIM8->PSC = 0;
		TIM5->PSC = 0;
		TIM4->PSC = 0;

		TIM1->ARR = arr;
		TIM8->ARR = arr;
		TIM5->ARR = arr;
		TIM4->ARR = arr;
	}
	Enable_Pwm_Channels(TIM1);
	Enable_Pwm_Channels(TIM8);
	Enable_Pwm_Channels(TIM5);
	Enable_Pwm_Channels(TIM4);
}
/**
 * @brief  Configure timer for pwm
 * @param  dshot_type
 * @retval No return value
 */
void Enable_Dshot_Channels(enum_dshot_type dshot_type)
{
	Dshot_Init(TIM1 ,dshot_type, timer1);
	Dshot_Init(TIM8 ,dshot_type, timer8);
	Dshot_Init(TIM5 ,dshot_type, timer5);
	Dshot_Init(TIM4 ,dshot_type, timer4);
}
/**
 * @brief  Set motor values to timer
 * @param  motor		Pointer to motor Values
 * @param  max_ccr		max value for Capture/Compare Register
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Pwm_Set(uint32_t * motor, uint32_t max_ccr, uint32_t min_ccr, uint32_t motor_offset){
	TIM1->CCR1 = motor[0]  > max_ccr ? max_ccr : motor[0]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[0];
	TIM1->CCR2 = motor[1]  > max_ccr ? max_ccr : motor[1]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[1];
	TIM1->CCR3 = motor[2]  > max_ccr ? max_ccr : motor[2]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[2];
	TIM1->CCR4 = motor[3]  > max_ccr ? max_ccr : motor[3]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[3];
	TIM8->CCR1 = motor[4]  > max_ccr ? max_ccr : motor[4]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[4];
	TIM8->CCR2 = motor[5]  > max_ccr ? max_ccr : motor[5]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[5];
	TIM8->CCR3 = motor[6]  > max_ccr ? max_ccr : motor[6]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[6];
	TIM8->CCR4 = motor[7]  > max_ccr ? max_ccr : motor[7]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[7];
	TIM5->CCR1 = motor[8]  > max_ccr ? max_ccr : motor[8]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[8];
	TIM5->CCR2 = motor[9]  > max_ccr ? max_ccr : motor[9]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[9];
	TIM5->CCR3 = motor[10] > max_ccr ? max_ccr : motor[10] < min_ccr+motor_offset ? min_ccr+motor_offset : motor[10];
	TIM5->CCR4 = motor[11] > max_ccr ? max_ccr : motor[11] < min_ccr+motor_offset ? min_ccr+motor_offset : motor[11];
	TIM4->CCR1 = motor[12] > max_ccr ? max_ccr : motor[12] < min_ccr+motor_offset ? min_ccr+motor_offset : motor[12];
	TIM4->CCR2 = motor[13] > max_ccr ? max_ccr : motor[13] < min_ccr+motor_offset ? min_ccr+motor_offset : motor[13];
	TIM4->CCR3 = motor[14] > max_ccr ? max_ccr : motor[14] < min_ccr+motor_offset ? min_ccr+motor_offset : motor[14];
	TIM4->CCR4 = motor[15] > max_ccr ? max_ccr : motor[15] < min_ccr+motor_offset ? min_ccr+motor_offset : motor[15];
}
/**
 * @brief  ReSet motor values of timer
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Pwm_Reset(uint32_t min_ccr, uint32_t motor_offset){
	TIM1->CCR1 = min_ccr+motor_offset;
	TIM1->CCR2 = min_ccr+motor_offset;
	TIM1->CCR3 = min_ccr+motor_offset;
	TIM1->CCR4 = min_ccr+motor_offset;
	TIM8->CCR1 = min_ccr+motor_offset;
	TIM8->CCR2 = min_ccr+motor_offset;
	TIM8->CCR3 = min_ccr+motor_offset;
	TIM8->CCR4 = min_ccr+motor_offset;
	TIM5->CCR1 = min_ccr+motor_offset;
	TIM5->CCR2 = min_ccr+motor_offset;
	TIM5->CCR3 = min_ccr+motor_offset;
	TIM5->CCR4 = min_ccr+motor_offset;
	TIM4->CCR1 = min_ccr+motor_offset;
	TIM4->CCR2 = min_ccr+motor_offset;
	TIM4->CCR3 = min_ccr+motor_offset;
	TIM4->CCR4 = min_ccr+motor_offset;
}
/**
 * @brief  Dshot Set motor values to timer
 * @param  motor		Pointer to motor Values
 * @param  max_ccr		max value for Capture/Compare Register
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Dshot_Set(uint32_t * motor, uint32_t max_ccr, uint32_t min_ccr, uint32_t motor_offset){
	uint16_t motor_val[16] = {0};
	for(int i=0; i<NUM_MOTOR;i++){
		motor_val[i] = motor[i]  > max_ccr ? max_ccr : motor[i]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[i];
	}
	Dshot_Write(&motor_val[0], timer1);
	Dshot_Write(&motor_val[4], timer8);
	Dshot_Write(&motor_val[8], timer5);
	Dshot_Write(&motor_val[12], timer4);
}
/**
 * @brief  Dshot ReSet motor values of timer
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Dshot_Reset( uint32_t min_ccr, uint32_t motor_offset){
	uint16_t motor_val[16] = {0};
	for(int i=0; i<NUM_MOTOR;i++){
		motor_val[i] = min_ccr+motor_offset;
	}
	Dshot_Write(&motor_val[0], timer1);
	Dshot_Write(&motor_val[4], timer8);
	Dshot_Write(&motor_val[8], timer5);
	Dshot_Write(&motor_val[12], timer4);
}

#else // QUADCOPTER_X4

/*QUAD X4 motor mixing (X-configuration)
														Front
														  ↑
											(CW)	M1        M2	(CCW)

											(CCW)	M4        M3	(CW)
														  ↓
														 Back
 */
//                          		M1      M2      M3      M4
const float roll_factor[4]  = { 	+1,     -1,     -1,     +1};
const float pitch_factor[4] = {		-1,		-1,		+1,     +1};
const float yaw_factor[4]   = { 	-1,     +1,     -1,     +1};

#define NUM_MOTOR  4

/**
 * @brief  Configure timer for pwm
 * @param  protocol		if oneshot125 psc = 2-1 else 1-1
 * @param  arr			protocol arr value
 * @retval No return value
 */
void Pwm_Init(uint32_t protocol, uint32_t arr)
{
	if(protocol == 125){ // oneshot125
		TIM1->PSC = 1;

		TIM1->ARR = arr;
	}
	else{
		TIM1->PSC = 0;

		TIM1->ARR = arr;
	}
	Enable_Pwm_Channels(TIM1);
}
/**
 * @brief  Configure timer for pwm
 * @param  dshot_type
 * @retval No return value
 */
void Enable_Dshot_Channels(enum_dshot_type dshot_type)
{
	Dshot_Init(TIM1 ,dshot_type, timer1);
}
/**
 * @brief  Set motor values to timer
 * @param  motor		Pointer to motor Values
 * @param  max_ccr		max value for Capture/Compare Register
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Pwm_Set(uint32_t * motor, uint32_t max_ccr, uint32_t min_ccr, uint32_t motor_offset){
	TIM1->CCR1 = motor[0]  > max_ccr ? max_ccr : motor[0]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[0];
	TIM1->CCR2 = motor[1]  > max_ccr ? max_ccr : motor[1]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[1];
	TIM1->CCR3 = motor[2]  > max_ccr ? max_ccr : motor[2]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[2];
	TIM1->CCR4 = motor[3]  > max_ccr ? max_ccr : motor[3]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[3];
}
/**
 * @brief  ReSet motor values of timer
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Pwm_Reset(uint32_t min_ccr, uint32_t motor_offset){
	TIM1->CCR1 = min_ccr+motor_offset;
	TIM1->CCR2 = min_ccr+motor_offset;
	TIM1->CCR3 = min_ccr+motor_offset;
	TIM1->CCR4 = min_ccr+motor_offset;
}
/**
 * @brief  Dshot Set motor values to timer
 * @param  motor		Pointer to motor Values
 * @param  max_ccr		max value for Capture/Compare Register
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Dshot_Set(uint32_t * motor, uint32_t max_ccr, uint32_t min_ccr, uint32_t motor_offset){
	uint16_t motor_val[16] = {0};
	for(int i=0; i<NUM_MOTOR;i++){
		motor_val[i] = motor[i]  > max_ccr ? max_ccr : motor[i]  < min_ccr+motor_offset ? min_ccr+motor_offset : motor[i];
	}
	Dshot_Write(&motor_val[0], timer1);
}
/**
 * @brief  Dshot ReSet motor values of timer
 * @param  min_ccr		min value for Capture/Compare Register
 * @param  motor_offset	offset value for Capture/Compare Register
 * @retval No return value
 */
void Dshot_Reset( uint32_t min_ccr, uint32_t motor_offset){
	uint16_t motor_val[16] = {0};
	for(int i=0; i<NUM_MOTOR;i++){
		motor_val[i] = min_ccr+motor_offset;
	}
	Dshot_Write(&motor_val[0], timer1);
}

#endif

/**
 * @brief  Set motor values to timer
 * @param  TIMx				TIMER
 * @retval No return value
 */
void Enable_Pwm_Channels(TIM_TypeDef * TIMx)
{
	LL_TIM_EnableCounter(TIMx); //Motor PWM
	LL_TIM_CC_EnableChannel(TIMx, LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIMx, LL_TIM_CHANNEL_CH2);
	LL_TIM_CC_EnableChannel(TIMx, LL_TIM_CHANNEL_CH3);
	LL_TIM_CC_EnableChannel(TIMx, LL_TIM_CHANNEL_CH4);
}

/**
 * @brief  Set motor values to timer
 * @param  motor				Pointer to motor Values
 * @param  setpoint_throttle	Receiver throttle value
 * @param  roll_pid				Receiver roll value
 * @param  pitch_pid			Receiver pitch value
 * @param  yaw_rate_pid			Receiver yaw value
 * @param  min_ccr				min value for Capture/Compare Register
 * @param  motor_offset			offset value for Capture/Compare Register
 * @param  throttle_multiplier	multiplier to make value in range
 * @retval No return value
 */
void Calculate_Motor_Values(uint32_t * motor,
		unsigned short setpoint_throttle,
		unsigned short roll_pid,
		unsigned short pitch_pid,
		unsigned short yaw_rate_pid,
		uint32_t min_ccr, uint32_t motor_offset, uint32_t throttle_multiplier){
	for (int i = 0; i < NUM_MOTOR; i++) {
		motor[i] = min_ccr + motor_offset + (setpoint_throttle - 1000) * throttle_multiplier
				+ (roll_pid     * roll_factor[i])
				+ (pitch_pid    * pitch_factor[i])
				+ (yaw_rate_pid * yaw_factor[i]);
	}

}
