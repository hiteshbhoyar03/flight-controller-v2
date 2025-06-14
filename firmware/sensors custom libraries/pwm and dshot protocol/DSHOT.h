/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		DSHOT.h
 * @brief   	:		This file contains all the constants parameters for DSHOT Protocol
 * ______________________________________________________________________________________________________
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __DSHOT_H__
#define __DSHOT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include <math.h>		// lrintf

/* Definitions ---------------------------------------------------------------*/
#define TIMER_CLOCK				240000000	/* Timer Clock */
#define motor_count             4

#define MOTOR_1_TIM_CHANNEL     LL_TIM_CHANNEL_CH1
#define MOTOR_2_TIM_CHANNEL     LL_TIM_CHANNEL_CH2
#define MOTOR_3_TIM_CHANNEL     LL_TIM_CHANNEL_CH3
#define MOTOR_4_TIM_CHANNEL     LL_TIM_CHANNEL_CH4

/* Define Macros -------------------------------------------------------------*/
#define MHZ_TO_HZ(x) 			((x) * 1000000)

#define DSHOT2400_HZ     		MHZ_TO_HZ(48)
#define DSHOT1200_HZ     		MHZ_TO_HZ(24)
#define DSHOT600_HZ     		MHZ_TO_HZ(12)
#define DSHOT300_HZ     		MHZ_TO_HZ(6)
#define DSHOT150_HZ     		MHZ_TO_HZ(3)

#define MOTOR_BIT_0            	7
#define MOTOR_BIT_1            	14
#define MOTOR_BITLENGTH        	20

#define DSHOT_FRAME_SIZE       	16
#define DSHOT_DMA_BUFFER_SIZE   18          /* resolution + frame reset (2us) */

#define DSHOT_MIN_THROTTLE      48
#define DSHOT_MAX_THROTTLE     	2047
#define DSHOT_RANGE 			(DSHOT_MAX_THROTTLE - DSHOT_MIN_THROTTLE)

/* Enumeration Definition ----------------------------------------------------*/
typedef enum
{
	DSHOT150  = 150,
	DSHOT300  = 300,
	DSHOT600  = 600,
	DSHOT1200 = 1200,
	DSHOT2400 = 2400
} enum_dshot_type;
typedef enum
{
	timer1 = 0,
	timer8 = 1,
	timer5 = 2,
	timer4 = 3
} enum_timer_number;

/* External Configuration Function -------------------------------------------*/
void Dshot_Init(TIM_TypeDef * TIMx ,enum_dshot_type dshot_type, enum_timer_number timer);
void Dshot_Write(uint16_t* motor_value,enum_timer_number timer);

#ifdef __cplusplus
}
#endif

#endif /* __DSHOT_H__ */
