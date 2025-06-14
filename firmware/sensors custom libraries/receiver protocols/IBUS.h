/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		ibus.h
 * @brief   	:		This file contains all the constants parameters for IBUS Protocol
 * ______________________________________________________________________________________________________
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IBUS_H__
#define __IBUS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Define Macros -------------------------------------------------------------*/
#define IBUS_PACKET_SIZE 	32		// Packet is <len><cmd><data....><chkl><chkh>
#define IBUS_CHANNEL_MIN 	1000
#define IBUS_CHANNEL_MID 	1500
#define IBUS_CHANNEL_MAX 	2000

/* Structure Definition ------------------------------------------------------*/
typedef struct _FS_iBus
{
	unsigned short channel_1;
	unsigned short channel_2;
	unsigned short channel_3;
	unsigned short channel_4;
	unsigned short switch_A;
	unsigned short switch_B;
	unsigned short switch_C;
	unsigned short switch_D;
	unsigned short VrA;
	unsigned short VrB;

	unsigned char failsafe;
}Struct_IBUS;

extern Struct_IBUS iBus;

/* External Configuration Function -------------------------------------------*/
unsigned char IBUS_Check_CHKSUM(unsigned char* data, unsigned char len);
void IBUS_Parsing(unsigned char* data, Struct_IBUS* iBus);
void IBUS_USART6_Initialization(void);
unsigned char IBUS_isActiveFailsafe(Struct_IBUS* iBus);

#ifdef __cplusplus
}
#endif
#endif /*__IBUS_H__ */
