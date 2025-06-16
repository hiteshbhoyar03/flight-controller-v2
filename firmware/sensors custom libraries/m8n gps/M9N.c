/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		m9n.c
 * @brief   	:		This file includes the functions for UBX Protocol
 * ______________________________________________________________________________________________________
 */

#include "M9N.h"

#define NMEA 0
#define UBX  1
#define GPS1_PROTOCOL UBX			// SELECT PROTOCOL NMEA IS DEFAULT

Struct_M9N_UBX_NAV_POSLLH gps_posllh;

const unsigned char UBX_CFG_PRT[] = {
	0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00,
	0xD0, 0x08, 0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x01, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9A, 0x79
};

const unsigned char UBX_CFG_MSG[] = {
	0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0x01, 0x02, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x13, 0xBE
};

const unsigned char UBX_CFG_RATE[] = {
	0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xC8, 0x00, 0x01, 0x00,
	0x01, 0x00, 0xDE, 0x6A
};

const unsigned char UBX_CFG_CFG[] = {
	0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x31,
	0xBF
};

void M9N_TransmitData_USART6(unsigned char* data, unsigned char len)
{
	for(int i=0;i<len;i++)
	{
		while(!LL_USART_IsActiveFlag_TXE(USART1));
		LL_USART_TransmitData8(USART1, *(data+i));
	}
}

void UBX_NAV_POSLLH_Parsing(unsigned char* data, Struct_M9N_UBX_NAV_POSLLH* posllh)
{
	posllh->CLASS  = data[2];
	posllh->ID     = data[3];
	posllh->LENGTH = (data[4] | data[5]<<8);

	posllh->iTOW   = (data[9] <<24 | data[8] <<16 | data[7] <<8 | data[6] );
	posllh->lon    = (data[13]<<24 | data[12]<<16 | data[11]<<8 | data[10]);
	posllh->lat    = (data[17]<<24 | data[16]<<16 | data[15]<<8 | data[14]);
	posllh->height = (data[21]<<24 | data[20]<<16 | data[19]<<8 | data[18]);
	posllh->hMSL   = (data[25]<<24 | data[24]<<16 | data[23]<<8 | data[22]);
	posllh->hAcc   = (data[29]<<24 | data[28]<<16 | data[27]<<8 | data[26]);
	posllh->vAcc   = (data[33]<<24 | data[32]<<16 | data[31]<<8 | data[30]);

//	posllh->lon_f64 = posllh->lon / 10000000.;
//	posllh->lat_f64 = posllh->lat / 10000000.;
}

unsigned char UBX_Check_CHKSUM(unsigned char* data, unsigned char len)
{
	unsigned char CK_A = 0, CK_B = 0;

	for(int i=2;i<len-2;i++)
	{
		CK_A = CK_A + data[i];
		CK_B = CK_B + CK_A;
	}

	return ((CK_A == data[len-2]) && (CK_B == data[len-1]));
}

void M9N_Initialization(void)
{
	M9N_USART1_Initialization();
	
#if GPS1_PROTOCOL == UBX
	// UNCOMMENT IF USING NMEA PROTOCOL
	M9N_TransmitData_USART6(&UBX_CFG_PRT[0] , sizeof(UBX_CFG_PRT));
	HAL_Delay(100);
	M9N_TransmitData_USART6(&UBX_CFG_MSG[0] , sizeof(UBX_CFG_MSG));
	HAL_Delay(100);
	M9N_TransmitData_USART6(&UBX_CFG_RATE[0], sizeof(UBX_CFG_RATE));
	HAL_Delay(100);
	M9N_TransmitData_USART6(&UBX_CFG_CFG[0] , sizeof(UBX_CFG_CFG));
	HAL_Delay(100);
#endif



}

void M9N_USART6_Initialization(void)
{

}
