/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		m9n.h
 * @brief   	:		This file contains all the constants parameters for UBX Protocol
 * ______________________________________________________________________________________________________
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __M9N_H__
#define __M9N_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Structure Definition ------------------------------------------------------*/
/* Packet is <sync1><sync2><class><id><length_2bytes><payload....><chk_a><chk_b>*/
typedef struct _M9N_UBX_NAV_POSLLH
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
}Struct_M9N_UBX_NAV_POSLLH;

extern Struct_M9N_UBX_NAV_POSLLH gps1_posllh;

/* External Configuration Function -------------------------------------------*/
unsigned char UBX_Check_CHKSUM(unsigned char* data, unsigned char len);
void UBX_NAV_POSLLH_Parsing(unsigned char* data, Struct_M9N_UBX_NAV_POSLLH* posllh);
void M9N_TransmitData_USART6(unsigned char* data, unsigned char len);
void M9N_USART6_Initialization(void);
void M9N_Initialization(void);

#ifdef __cplusplus
}
#endif
#endif /*__M9N_H__ */
