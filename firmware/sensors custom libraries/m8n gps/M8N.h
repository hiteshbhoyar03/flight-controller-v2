/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		m8n.h
 * @brief   	:		This file contains all the constants parameters for UBX Protocol
 * ______________________________________________________________________________________________________
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __M8N_H__
#define __M8N_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Structure Definition ------------------------------------------------------*/
/* Packet is <sync1><sync2><class><id><length_2bytes><payload....><chk_a><chk_b>*/
typedef struct _M8N_UBX_NAV_POSLLH
{
	unsigned char 	CLASS;
	unsigned char 	ID;
	unsigned short 	LENGTH;

	unsigned int 	iTOW;
	signed int   	lon;
	signed int   	lat;
	signed int   	height;
	signed int   	hMSL;
	unsigned int 	hAcc;
	unsigned int 	vAcc;

	double 	lon_f64;
	double 	lat_f64;
}Struct_M8N_UBX_NAV_POSLLH;

extern Struct_M8N_UBX_NAV_POSLLH gps1_posllh;
extern Struct_M8N_UBX_NAV_POSLLH gps2_posllh;

/* External Configuration Function -------------------------------------------*/
unsigned char UBX_Check_CHKSUM(unsigned char* data, unsigned char len);
void UBX_NAV_POSLLH_Parsing(unsigned char* data, Struct_M8N_UBX_NAV_POSLLH* posllh);
void M8N_TransmitData_USART1(unsigned char* data, unsigned char len);
void M8N_TransmitData_UART8(unsigned char* data, unsigned char len);
void M8N_USART1_Initialization(void);
void M8N_UART8_Initialization(void);
void M8N_Initialization(void);

#ifdef __cplusplus
}
#endif
#endif /*__M8N_H__ */
