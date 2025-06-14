/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		icp20100.h
 * @brief   	:		This file contains all the constants parameters for ICP20100 BARO
 * ______________________________________________________________________________________________________
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ICP20100_H__
#define __ICP20100_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Definitions ---------------------------------------------------------------*/
#define ICP20100_SPI_CHANNEL		SPI4

#define ICP20100_SPI_SCLK_PIN		LL_GPIO_PIN_2
#define ICP20100_SPI_SCLK_PORT		GPIOE
#define ICP20100_SPI_SCLK_CLK		LL_AHB4_GRP1_PERIPH_GPIOE

#define ICP20100_SPI_MISO_PIN		LL_GPIO_PIN_5
#define ICP20100_SPI_MISO_PORT		GPIOE
#define ICP20100_SPI_MISO_CLK		LL_AHB4_GRP1_PERIPH_GPIOE

#define ICP20100_SPI_MOSI_PIN		LL_GPIO_PIN_6
#define ICP20100_SPI_MOSI_PORT		GPIOE
#define ICP20100_SPI_MOSI_CLK		LL_AHB4_GRP1_PERIPH_GPIOE

#define ICP20100_SPI_nCS_PIN		LL_GPIO_PIN_12
#define ICP20100_SPI_nCS_PORT		GPIOE
#define ICP20100_SPI_nCS_CLK		LL_AHB4_GRP1_PERIPH_GPIOE

#define ICP20100_INT_PIN			LL_GPIO_PIN_8
#define ICP20100_INT_PORT			GPIOG
#define ICP20100_INT_CLK			LL_AHB4_GRP1_PERIPH_GPIOG

#define ICP20100_CHIP_SELECT(ICP20100)		LL_GPIO_ResetOutputPin(ICP20100_SPI_nCS_PORT, ICP20100_SPI_nCS_PIN)
#define ICP20100_CHIP_DESELECT(ICP20100)	LL_GPIO_SetOutputPin(ICP20100_SPI_nCS_PORT, ICP20100_SPI_nCS_PIN)

/* Register Map --------------------------------------------------------------*/
#define ICP20100_REG_TRIM1_MSB 				UINT8_C(0x05)
#define ICP20100_REG_TRIM2_LSB 				UINT8_C(0x06)
#define ICP20100_REG_TRIM2_MSB 				UINT8_C(0x07)
#define ICP20100_REG_DEVICE_ID 				UINT8_C(0x0C)
#define ICP20100_REG_IO_DRIVE_STRENGTH 		UINT8_C(0x0D)
#define ICP20100_REG_OTP_CONFIG1 			UINT8_C(0xAC)
#define ICP20100_REG_OTP_MR_LSB 			UINT8_C(0xAD)
#define ICP20100_REG_OTP_MR_MSB 			UINT8_C(0xAE)
#define ICP20100_REG_OTP_MRA_LSB 			UINT8_C(0xAF)
#define ICP20100_REG_OTP_MRA_MSB 			UINT8_C(0xB0)
#define ICP20100_REG_OTP_MRB_LSB 			UINT8_C(0xB1)
#define ICP20100_REG_OTP_MRB_MSB 			UINT8_C(0xB2)
#define ICP20100_REG_OTP_ADDRESS_REG 		UINT8_C(0xB5)
#define ICP20100_REG_OTP_COMMAND_REG 		UINT8_C(0xB6)
#define ICP20100_REG_OTP_RDATA 				UINT8_C(0xB8)
#define ICP20100_REG_OTP_STATUS 			UINT8_C(0xB9)
#define ICP20100_REG_OTP_DBG2 				UINT8_C(0xBC)
#define ICP20100_REG_MASTER_LOCK 			UINT8_C(0xBE)
#define ICP20100_REG_OTP_STATUS2 			UINT8_C(0xBF)
#define ICP20100_REG_MODE_SELECT 			UINT8_C(0xC0)
#define ICP20100_REG_INTERRUPT_STATUS 		UINT8_C(0xC1)
#define ICP20100_REG_INTERRUPT_MASK 		UINT8_C(0xC2)
#define ICP20100_REG_FIFO_CONFIG 			UINT8_C(0xC3)
#define ICP20100_REG_FIFO_FILL 				UINT8_C(0xC4)
#define ICP20100_REG_SPI_MODE 				UINT8_C(0xC5)
#define ICP20100_REG_PRESS_ABS_LSB 			UINT8_C(0xC7)
#define ICP20100_REG_PRESS_ABS_MSB 			UINT8_C(0xC8)
#define ICP20100_REG_PRESS_DELTA_LSB 		UINT8_C(0xC9)
#define ICP20100_REG_PRESS_DELTA_MSB 		UINT8_C(0xCA)
#define ICP20100_REG_DEVICE_STATUS 			UINT8_C(0xCD)
#define ICP20100_REG_I3C_INFO 				UINT8_C(0xCE)
#define ICP20100_REG_VERSION 				UINT8_C(0xD3)
#define ICP20100_REG_PRESS_DATA_0			UINT8_C(0xFA)
#define ICP20100_REG_PRESS_DATA_1 			UINT8_C(0xFB)
#define ICP20100_REG_PRESS_DATA_2 			UINT8_C(0xFC)
#define ICP20100_REG_TEMP_DATA_0 			UINT8_C(0xFD)
#define ICP20100_REG_TEMP_DATA_1 			UINT8_C(0xFE)
#define ICP20100_REG_TEMP_DATA_2 			UINT8_C(0xFF)
/* Register Map Ends ---------------------------------------------------------*/

/* Define Macros -------------------------------------------------------------*/

#define ICP20100_ID 					UINT8_C(0x63)

/* Structure Definition ------------------------------------------------------*/
typedef struct _ICP20100{
	int32_t 		raw_pressure;
	int32_t 		raw_temperature;
	float 			pressure;
	float 			temperature;
	double			altitude;
	double			filtered_altitude;
}Struct_ICP20100;

extern Struct_ICP20100 ICP20100;

/* Read and Write Function ---------------------------------------------------*/
uint8_t ICP20100_Readbyte(uint8_t reg_addr);
void 	ICP20100_Read_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data);
void 	ICP20100_Writebyte(uint8_t reg_addr, uint8_t val);
void 	ICP20100_Write_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data);

/* External Configuration Function -------------------------------------------*/
void ICP20100_SPI_GPIO_Initialization(void);
int  ICP20100_Initialization(void);
void ICP20100_Get_Raw_Pressure_And_Temp(int32_t* pressure, int32_t* temperature);
void ICP20100_Get_Raw_Pressure(int32_t* pressure);
void ICP20100_Get_Raw_Temperature(int32_t* temperature);
void ICP20100_Get_Altitude(Struct_ICP20100* ICP20100); //No temperature correction.
void ICP20100_Get_Altitude_Temp_Corrected(Struct_ICP20100* ICP20100); //Get Altitude with temperature correction.
int  ICP20100_DataReady(void);

#ifdef __cplusplus
}
#endif

#endif /* __ICP20100_H__ */
