/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		DRONE_CONFIG.h
 * @brief   	:		This file includes the LL driver functions for DShOT Protocol
 * ______________________________________________________________________________________________________
 */

#ifndef __DRONE_CONFIG_H
#define __DRONE_CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dshot.h"
#include "tim.h"

/* Define Macros -------------------------------------------------------------*/
// DEFAULT QUADCOPTER_X4
#define QUADCOPTER_X4	1
#define QUADCOPTER_X8	0
#define HEXACOPTER_X6	0
#define HEXACOPTER_X12	0
#define OCTOCOPTER_X8	0
#define OCTOCOPTER_X16	0


/* External Configuration Function -------------------------------------------*/
void Calculate_Motor_Values(uint32_t * motor,
		unsigned short setpoint_throttle,
		unsigned short roll_pid,
		unsigned short pitch_pid,
		unsigned short yaw_rate_pid,
		uint32_t min_ccr, uint32_t motor_offset, uint32_t throttle_multiplier);
void Enable_Pwm_Channels(TIM_TypeDef * TIMx);
void Enable_Dshot_Channels(enum_dshot_type dshot_type);
void Pwm_Init(uint32_t protocol, uint32_t arr);
void Pwm_Set(uint32_t * motor, uint32_t max_ccr, uint32_t min_ccr, uint32_t motor_offset);
void Pwm_Reset(uint32_t min_ccr, uint32_t motor_offset);
void Dshot_Set(uint32_t * motor, uint32_t max_ccr, uint32_t min_ccr, uint32_t motor_offset);
void Dshot_Reset( uint32_t min_ccr, uint32_t motor_offset);

#ifdef __cplusplus
}
#endif
#endif /*__DRONE_CONFIG_H */
