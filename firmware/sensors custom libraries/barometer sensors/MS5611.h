/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		ms5611.h
 * @brief   	:		This file contains all the constants parameters for MS5611 BARO
 * ______________________________________________________________________________________________________
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MS5611_H__
#define __MS5611_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Definitions ---------------------------------------------------------------*/
#define MS5611_SPI_CHANNEL		SPI5

#define MS5611_SPI_SCLK_PIN		LL_GPIO_PIN_0
#define MS5611_SPI_SCLK_PORT	GPIOK
#define MS5611_SPI_SCLK_CLK		LL_AHB4_GRP1_PERIPH_GPIOK

#define MS5611_SPI_MISO_PIN		LL_GPIO_PIN_8
#define MS5611_SPI_MISO_PORT	GPIOF
#define MS5611_SPI_MISO_CLK		LL_AHB4_GRP1_PERIPH_GPIOF

#define MS5611_SPI_MOSI_PIN		LL_GPIO_PIN_10
#define MS5611_SPI_MOSI_PORT	GPIOJ
#define MS5611_SPI_MOSI_CLK		LL_AHB4_GRP1_PERIPH_GPIOJ

#define MS5611_SPI_nCS_PIN		LL_GPIO_PIN_13
#define MS5611_SPI_nCS_PORT		GPIOE
#define MS5611_SPI_nCS_CLK		LL_AHB4_GRP1_PERIPH_GPIOE

#define MS5611_CHIP_SELECT(MS5611)		LL_GPIO_ResetOutputPin(MS5611_SPI_nCS_PORT, MS5611_SPI_nCS_PIN)
#define MS5611_CHIP_DESELECT(MS5611)	LL_GPIO_SetOutputPin(  MS5611_SPI_nCS_PORT, MS5611_SPI_nCS_PIN)

/* Register Map --------------------------------------------------------------*/
#define MS5611_REG_RESET			UINT8_C(0x1E)
#define MS5611_REG_PRESURE_D1		UINT8_C(0x40)
#define MS5611_REG_TEMPERATURE_D2 	UINT8_C(0x50)
#define MS5611_REG_ADC_REG			UINT8_C(0x00)
#define MS5611_REG_PROM_REG			UINT8_C(0xA0)  //0xA0) to 0xAE

/* Define Macros -------------------------------------------------------------*/
#define OSR_256  					UINT8_C(0x00)  // define pressure and temperature conversion rates
#define OSR_512  					UINT8_C(0x02)
#define OSR_1024 					UINT8_C(0x04)
#define OSR_2048 					UINT8_C(0x06)
#define OSR_4096 					UINT8_C(0x08)
#define ADC_SIZE					3
#define PROM_SIZE					16
#define SEA_LEVEL_PRESSURE			101325
#define OSR_256_CONVERSION_TIME		1
#define OSR_512_CONVERSION_TIME		2
#define OSR_1024_CONVERSION_TIME	3
#define OSR_2048_CONVERSION_TIME	5
#define OSR_4096_CONVERSION_TIME	9

/* Structure Definition ------------------------------------------------------*/
typedef struct _MS5611
{
	float 			pressure;
	float 			temperature;
	double			altitude;
	double			filtered_altitude;
	uint32_t		raw_pressure;
	uint32_t		raw_temperature;
	// prom data start do not change syntax
	uint16_t		FACTORY_DATA;
	uint16_t		COEF1;
	uint16_t		COEF2;
	uint16_t		COEF3;
	uint16_t		COEF4;
	uint16_t		COEF5;
	uint16_t		COEF6;
	uint16_t		CRC_SERIAL_CODE;
	// prom data end do not change syntax
	int32_t			dT;
	int64_t			T2;
	int64_t			OFFSET;
	int64_t			OFFSET2;
	int64_t			SENS;
	int64_t			SENS2;
	uint8_t			ADC_DATA[ADC_SIZE];
}Struct_MS5611;

extern Struct_MS5611 MS5611;

/* Read and Write Function ---------------------------------------------------*/
uint8_t MS5611_Readbyte(uint8_t reg_addr);
void 	MS5611_Read_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data);
void 	MS5611_Writebyte(uint8_t reg_addr, uint8_t val);
void 	MS5611_Write_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data);

/* External Configuration Function -------------------------------------------*/
void MS5611_SPI_GPIO_Initialization(void);
int  MS5611_Initialization(void);
void MS5611_Read_Factory_Calibrated_Data(uint16_t* data);
void MS5611_Get_Raw_Pressure(uint32_t* pressure);
void MS5611_Get_Raw_Temperature(uint32_t* temperature);
void MS5611_Calculate_Temperature(Struct_MS5611* MS5611);
void MS5611_Calculate_Temperature_Compensated_Pressure(Struct_MS5611* MS5611);
void MS5611_Get_Altitude(Struct_MS5611 * MS5611);

#ifdef __cplusplus
}
#endif

#endif /* __MS5611_H__ */
