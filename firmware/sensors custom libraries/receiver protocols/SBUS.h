/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		sbus.h
 * @brief   	:		This file contains all the constants parameters for SBUS Protocol
 * ______________________________________________________________________________________________________
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SBUS_H__
#define __SBUS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define SBUS_PACKET_SIZE 	25		// Packet is <header><payload....><ch17_18_framelost_failsafe><footer>
#define SBUS_CHANNEL_MIN 	172
#define SBUS_CHANNEL_MID 	992
#define SBUS_CHANNEL_MAX 	1811

/* Structure Definition ------------------------------------------------------*/
typedef struct _SBUS
{
//	uint16_t channel[16];			// if used check sbus.c file
//	uint8_t channel_17;
	unsigned short channel[16];
	unsigned char channel_17;
	unsigned char channel_18;
	unsigned char frame_lost;
	unsigned char failsafe_activated;
	unsigned char footer;
}Struct_SBUS;

extern Struct_SBUS SBUS;

/* External Configuration Function -------------------------------------------*/
// unsigned char SBUS_Check_crc8(unsigned char* data, unsigned char len);
void SBUS_Parsing(unsigned char* sbusData, Struct_SBUS* SBUS);
void SBUS_USART6_Initialization(void);
unsigned char SBUS_isActiveFailsafe(Struct_SBUS* SBUS);

#ifdef __cplusplus
}
#endif
#endif /*__SBUS_H__ */

/*
SBUS is a bus protocol for receivers to send commands to servos. Unlike PWM,
SBUS uses a bus architecture where a single serial line can be connected with up to 16 servos with each receiving a unique command.

The SBUS protocol uses an inverted serial logic with a baud rate of 100000, 8 data bits, even parity, and 2 stop bits.
The SBUS packet is 25 bytes long consisting of:

Byte[0]: SBUS header, 0x0F
Byte[1 -22]: 16 servo channels, 11 bits each
Byte[23]
Bit 0: channel 17 (0x01)
Bit 1: channel 18 (0x02)
Bit 2: frame lost (0x04)
Bit 3: failsafe activated (0x08)
Byte[24]: SBUS footer
Note that lost frame is indicated when a frame is lost between the transmitter and receiver.
Failsafe activation typically requires that many frames are lost in a row and indicates that the receiver has moved into failsafe mode.
Packets are sent approximately every 10 ms or 20 ms, depending on the system configuration.

A variation on SBUS called "Fast SBUS" has started to be used. This uses a baudrate of 200000 and a quicker update rate.

Note on CH17 and CH18: Channel 17 and channel 18 are digital on/off channels. These are not universally available on all SBUS receivers and servos.

FrSky receivers will output a range of 172 - 1811 with channels set to a range of -100% to +100%. 
Using extended limits of -150% to +150% outputs a range of 0 to 2047, which is the maximum range acheivable with 11 bits of data.

Because SBUS is a digital bus format, it is an excellent means of receiving pilot commands from a transmitter and an SBUS capable receiver. 
If SBUS servos are used in the aircraft, SBUS is also an excellent means of sending actuator commands - 
servo commands can often be sent with lower latency and, by only using a single pin to command up to 16 servos, 
additional microcontroller pins are freed for other uses.

*/
