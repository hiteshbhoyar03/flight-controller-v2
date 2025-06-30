/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		ibus.c
 * @brief   	:		This file includes the functions for IBUS Protocol
 * ______________________________________________________________________________________________________
 */

#include <IBUS.h>

Struct_IBUS iBus;

unsigned char IBUS_Check_CHKSUM(unsigned char* data, unsigned char len)
{
	unsigned short crc = 0xFFFF;

	for(int i=0;i<len-2;i++)
	{
		crc = crc - data[i];
	}

	return ((crc&0x00FF) == data[30]) && ((crc >> 8) == data[31]);
}

void IBUS_Parsing(unsigned char* data, Struct_IBUS* iBus)
{
	iBus->roll      = (unsigned short)( data[3]  << 8 | data[2]  ) & 0x0FFF;
	iBus->pitch     = (unsigned short)( data[5]  << 8 | data[4]  ) & 0x0FFF;
	iBus->throttle  = (unsigned short)( data[7]  << 8 | data[6]  ) & 0x0FFF;
	iBus->yaw       = (unsigned short)( data[9]  << 8 | data[8]  ) & 0x0FFF;
	iBus->switch_A  = (unsigned short)( data[11] << 8 | data[10] ) & 0x0FFF;
	iBus->switch_B  = (unsigned short)( data[13] << 8 | data[12] ) & 0x0FFF;
//	iBus->switch_C  = (unsigned short)( data[15] << 8 | data[14] ) & 0x0FFF;	// USE IN CASE OF FS-i6X
//	iBus->switch_D  = (unsigned short)( data[17] << 8 | data[16] ) & 0x0FFF;
//	iBus->VrA       = (unsigned short)( data[19] << 8 | data[18] ) & 0x0FFF;
//	iBus->VrB       = (unsigned short)( data[21] << 8 | data[20] ) & 0x0FFF;

	iBus->failsafe  = (unsigned char)( data[13] >> 4 );
//	iBus->failsafe  = (unsigned char)( iBus->switch_D == 1500 );				// USE IN CASE OF FS-i6X
}

unsigned char IBUS_isActiveFailsafe(Struct_IBUS* iBus)
{
	return iBus->failsafe != 0;
}

void IBUS_UART_Initialization(void)
{

}
