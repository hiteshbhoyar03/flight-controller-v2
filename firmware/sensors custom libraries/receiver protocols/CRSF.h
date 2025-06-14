/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		crsf.h
 * @brief   	:		This file contains all the constants parameters for CRSF Protocol
 * ______________________________________________________________________________________________________
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRSF_H__
#define __CRSF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Define Macros -------------------------------------------------------------*/
#define CRSF_MAX_CHANNEL 	16		// Total channel
#define CRSF_PACKET_SIZE 	26		// Packet is <addr><length><type><payload....><crc>
#define CRSF_PACKET_LENGTH 	22		// Payload 22 bytes -> 11 bits each channel
#define CRSF_FRAME_LENGTH 	24		// Length of ( type + payload + crc )
#define CRSF_CHANNEL_MIN 	172
#define CRSF_CHANNEL_MID 	992
#define CRSF_CHANNEL_MAX 	1810

/* Structure Definition ------------------------------------------------------*/
typedef struct _CRSF
{
//	uint16_t channel[CRSF_MAX_CHANNEL]; 		// if used check crsf.c file
	unsigned short channel[CRSF_MAX_CHANNEL];
	unsigned char failsafe;

}Struct_CRSF;

extern Struct_CRSF CRSF;

/* External Configuration Function -------------------------------------------*/
unsigned char CRSF_Check_crc8(unsigned char* data, unsigned char len);
void CRSF_Parsing(unsigned char* crsfData, Struct_CRSF* CRSF);
void CRSF_USART6_Initialization(void);
unsigned char CRSF_isActiveFailsafe(Struct_CRSF* CRSF);

#ifdef __cplusplus
}
#endif
#endif /*__CRSF_H__ */

/*
 // Basic setup
#define CRSF_MAX_CHANNEL 16
#define CRSF_FRAME_SIZE_MAX 64
#define CRSF_CONNECTION_TIMEOUT 1000

// Device address & type, The address means the destination of the data packet, so for decoder, the destination is the FC.
#define CRSF_TYPE_SETTINGS_WRITE 0x2D
#define CRSF_ADDRESS_MODULE 0xEE            // Crossfire transmitter
#define CRSF_ADDRESS_RADIO 0xEA             // Radio Transmitter
#define CRSF_ADDRESS_FLIGHT_CONTROLLER 0xC8 // Flight Controler

// Define channel input limite
#define CRSF_CHANNEL_MIN 172
#define CRSF_CHANNEL_MID 992
#define CRSF_CHANNEL_MAX 1810

// internal crsf variables
#define CRSF_TIME_NEEDED_PER_FRAME_US 1100 // 700 ms + 400 ms for potential ad-hoc request
#define CRSF_TIME_BETWEEN_FRAMES_US 6667   // At fastest, frames are sent by the transmitter every 6.667 milliseconds, 150 Hz
#define CRSF_PACKET_TIMEOUT_US 100000
#define CRSF_PAYLOAD_OFFSET offsetof(crsfFrameDef_t, type)
#define CRSF_MAX_CHANNEL 16
#define CRSF_FRAME_SIZE_MAX 64
#define CRSF_MSP_RX_BUF_SIZE 128
#define CRSF_MSP_TX_BUF_SIZE 128
#define CRSF_PAYLOAD_SIZE_MAX 60
#define CRSF_PACKET_LENGTH 22
#define CRSF_PACKET_SIZE 26
#define CRSF_FRAME_LENGTH 24 // length of type + payload + crc

 */
