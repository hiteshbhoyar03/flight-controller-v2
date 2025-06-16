/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		sbus.c
 * @brief   	:		This file includes the functions for SBUS Protocol
 * ______________________________________________________________________________________________________
 */

#include "SBUS.h"

Struct_SBUS SBUS;

// unsigned char SBUS_Check_crc8(unsigned char* data, unsigned char len)
// {
//
// 	return (crc == data[24]);
// }

void SBUS_Parsing(unsigned char* sbusData, Struct_SBUS* SBUS)
{	// EACH CHANNEL COMPRISES OF 11 BITS
	SBUS->channel[0]  = (unsigned short)((sbusData[1]       | sbusData[2]  << 8) & 0x07FF);
	SBUS->channel[1]  = (unsigned short)((sbusData[2]  >> 3 | sbusData[3]  << 5) & 0x07FF);
	SBUS->channel[2]  = (unsigned short)((sbusData[3]  >> 6 | sbusData[4]  << 2  | sbusData[5] <<10)& 0x07FF);
	SBUS->channel[3]  = (unsigned short)((sbusData[5]  >> 1 | sbusData[6]  << 7) & 0x07FF);
	SBUS->channel[4]  = (unsigned short)((sbusData[6]  >> 4 | sbusData[7]  << 4) & 0x07FF);
	SBUS->channel[5]  = (unsigned short)((sbusData[7]  >> 7 | sbusData[8]  << 1  | sbusData[9] <<9) & 0x07FF);
	SBUS->channel[6]  = (unsigned short)((sbusData[9]  >> 2 | sbusData[10] << 6) & 0x07FF);
	SBUS->channel[7]  = (unsigned short)((sbusData[10] >> 5 | sbusData[11] << 3) & 0x07FF);
	SBUS->channel[8]  = (unsigned short)((sbusData[12]      | sbusData[13] << 8) & 0x07FF);
	SBUS->channel[9]  = (unsigned short)((sbusData[13] >> 3 | sbusData[14] << 5) & 0x07FF);
	SBUS->channel[10] = (unsigned short)((sbusData[14] >> 6 | sbusData[15] << 2  | sbusData[18]<<10)& 0x07FF);
	SBUS->channel[11] = (unsigned short)((sbusData[16] >> 1 | sbusData[17] << 7) & 0x07FF);
	SBUS->channel[12] = (unsigned short)((sbusData[17] >> 4 | sbusData[18] << 4) & 0x07FF);
	SBUS->channel[13] = (unsigned short)((sbusData[18] >> 7 | sbusData[19] << 1  | sbusData[20]<<9) & 0x07FF);
	SBUS->channel[14] = (unsigned short)((sbusData[20] >> 2 | sbusData[21] << 6) & 0x07FF);
	SBUS->channel[15] = (unsigned short)((sbusData[21] >> 5 | sbusData[22] << 3) & 0x07FF);

	SBUS->channel_17  = (unsigned char)(sbusData[23] & 0x01);
	SBUS->channel_18  = (unsigned char)(sbusData[23] & 0x02);
	SBUS->frame_lost  = (unsigned char)(sbusData[23] & 0x04);
	SBUS->failsafe_activated = (unsigned char)(sbusData[23] & 0x08);
	SBUS->footer      = (unsigned char)sbusData[24];

}

unsigned char SBUS_isActiveFailsafe(Struct_SBUS* SBUS)
{
	return SBUS->failsafe_activated != 0;
}

void SBUS_USART2_Initialization(void)
{

}
