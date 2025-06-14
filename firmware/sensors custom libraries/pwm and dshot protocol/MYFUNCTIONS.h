/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		myfunctions.h
 * @brief   	:		This file contains all the constants parameters System Initialization
 * ______________________________________________________________________________________________________
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MYFUNCTIONS_H__
#define __MYFUNCTIONS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "ibus.h"
#include "crsf.h"
#include "sbus.h"

/* Define Macros -------------------------------------------------------------*/
#define NMEA_PROTOCOL 		0
#define UBX_PROTOCOL 		1
#define GPS1_PROTOCOL 		UBX_PROTOCOL		// SELECT GPS PROTOCOL
#define GPS2_PROTOCOL 		UBX_PROTOCOL		// SELECT GPS PROTOCOL

#define IBUS_PROTOCOL 		0x20				// Values are header of packet; just given for bluff
#define CRSF_PROTOCOL 		0xC8
#define SBUS_PROTOCOL 		0x0F
#define RECEIVER_PROTOCOL 	IBUS_PROTOCOL		// SELECT RX PROTOCOL

// CCR Values for MAX and MIN Duty Cycle
#define oneshot125_max_ccr  29999
#define oneshot125_min_ccr  15000
#define oneshot42_max_ccr   20159
#define oneshot42_min_ccr   10080
#define multishot_max_ccr    5999
#define multishot_min_ccr    1200

// ARR Values for MAX and MIN Update-Rate
#define oneshot125_max_updateRate_arr  29999	// 4 KHz Update Rate
#define oneshot125_min_updateRate_arr  59999	// 2 KHz Update Rate
#define oneshot42_max_updateRate_arr   20159	// almost 12 KHz Update Rate
#define oneshot42_min_updateRate_arr   40319	// almost 6 KHz Update Rate
#define multishot_max_updateRate_arr    5999	// 40 KHz Update Rate
#define multishot_min_updateRate_arr   11999	// 12 KHz Update Rate

/* Structure Definition ------------------------------------------------------*/
//typedef struct _abc
//{
//	unsigned char max_ccr;
//	unsigned char min_ccr;
//	unsigned char ch_max;
//	unsigned char ch_min;
//
//}Struct_abc;
//extern Struct_abc ABC;

/* Enumeration ---------------------------------------------------------------*/
typedef enum
{
	ONESHOT125 = 125,			// Values are bluff; has no meaning
	ONESHOT42  = 42,
	MULTISHOT  = 5
} Analog_ESC_Protocol;

//typedef enum
//{
//	DSHOT150 = 150,
//	DSHOT300 = 300,
//	DSHOT600 = 600,
//	DSHOT1200= 1200,
//	DSHOT2400= 2400
//} dshot_type;
//// extern dshot_type b;


/* External Configuration Function -------------------------------------------*/
int Is_Data_Received(void);
int Is_Throttle_Min(void);
void Calibrate_ESC(TIM_TypeDef * timer, Analog_ESC_Protocol protocol);



void buzzer_Data_Not_Received(void);
void buzzer_Calibration_Started(void);
void buzzer_Calibration_Done(void);
void buzzer_Throttel_Safety(void);
void buzzer_All_Set(void);

#ifdef __cplusplus
}
#endif

#endif /* __MYFUNCTIONS_H__ */
