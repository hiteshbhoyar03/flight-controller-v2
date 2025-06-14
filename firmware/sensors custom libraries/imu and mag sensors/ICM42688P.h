/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		icm42688p.h
 * @brief   	:		This file contains all the constants parameters for ICM42688P IMU
 * ______________________________________________________________________________________________________
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ICM42688P_H__
#define __ICM42688P_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Definitions ---------------------------------------------------------------*/
#define ICM42688P_SPI_CHANNEL		SPI2

#define ICM42688P_SPI_SCLK_PIN		LL_GPIO_PIN_1
#define ICM42688P_SPI_SCLK_PORT		GPIOI
#define ICM42688P_SPI_SCLK_CLK		LL_AHB4_GRP1_PERIPH_GPIOI

#define ICM42688P_SPI_MISO_PIN		LL_GPIO_PIN_2
#define ICM42688P_SPI_MISO_PORT		GPIOC
#define ICM42688P_SPI_MISO_CLK		LL_AHB4_GRP1_PERIPH_GPIOC

#define ICM42688P_SPI_MOSI_PIN		LL_GPIO_PIN_3
#define ICM42688P_SPI_MOSI_PORT		GPIOI
#define ICM42688P_SPI_MOSI_CLK		LL_AHB4_GRP1_PERIPH_GPIOI

#define ICM42688P_SPI_nCS_PIN		LL_GPIO_PIN_8
#define ICM42688P_SPI_nCS_PORT		GPIOE
#define ICM42688P_SPI_nCS_CLK		LL_AHB4_GRP1_PERIPH_GPIOE

#define ICM42688P_INT_PIN			LL_GPIO_PIN_3
#define ICM42688P_INT_PORT			GPIOG
#define ICM42688P_INT_CLK			LL_AHB4_GRP1_PERIPH_GPIOG

#define ICM42688P_CHIP_SELECT(ICM42688P)		LL_GPIO_ResetOutputPin(ICM42688P_SPI_nCS_PORT, ICM42688P_SPI_nCS_PIN)
#define ICM42688P_CHIP_DESELECT(ICM42688P)	    LL_GPIO_SetOutputPin(  ICM42688P_SPI_nCS_PORT, ICM42688P_SPI_nCS_PIN)

/* Register Map -----------------------------------------------------------------*/
#define ICM42688P_REG_DEVICE_CONFIG 		UINT8_C(0x11)
#define ICM42688P_REG_DRIVE_CONFIG 			UINT8_C(0x13)
#define ICM42688P_REG_INT_CONFIG 			UINT8_C(0x14)
#define ICM42688P_REG_FIFO_CONFIG 			UINT8_C(0x16)
#define ICM42688P_REG_TEMP_DATA1 			UINT8_C(0x1D)
#define ICM42688P_REG_TEMP_DATA0 			UINT8_C(0x1E)
#define ICM42688P_REG_ACCEL_DATA_X1 		UINT8_C(0x1F)
#define ICM42688P_REG_ACCEL_DATA_X0 		UINT8_C(0x20)
#define ICM42688P_REG_ACCEL_DATA_Y1 		UINT8_C(0x21)
#define ICM42688P_REG_ACCEL_DATA_Y0 		UINT8_C(0x22)
#define ICM42688P_REG_ACCEL_DATA_Z1 		UINT8_C(0x23)
#define ICM42688P_REG_ACCEL_DATA_Z0 		UINT8_C(0x24)
#define ICM42688P_REG_GYRO_DATA_X1 			UINT8_C(0x25)
#define ICM42688P_REG_GYRO_DATA_X0 			UINT8_C(0x26)
#define ICM42688P_REG_GYRO_DATA_Y1 			UINT8_C(0x27)
#define ICM42688P_REG_GYRO_DATA_Y0 			UINT8_C(0x28)
#define ICM42688P_REG_GYRO_DATA_Z1 			UINT8_C(0x29)
#define ICM42688P_REG_GYRO_DATA_Z0 			UINT8_C(0x2A)
#define ICM42688P_REG_TMST_FSYNCH 			UINT8_C(0x2B)
#define ICM42688P_REG_TMST_FSYNCL 			UINT8_C(0x2C)
#define ICM42688P_REG_INT_STATUS 			UINT8_C(0x2D)
#define ICM42688P_REG_FIFO_COUNTH 			UINT8_C(0x2E)
#define ICM42688P_REG_FIFO_COUNTL 			UINT8_C(0x2F)
#define ICM42688P_REG_FIFO_DATA 			UINT8_C(0x30)
#define ICM42688P_REG_APEX_DATA0 			UINT8_C(0x31)
#define ICM42688P_REG_APEX_DATA1 			UINT8_C(0x32)
#define ICM42688P_REG_APEX_DATA2 			UINT8_C(0x33)
#define ICM42688P_REG_APEX_DATA3 			UINT8_C(0x34)
#define ICM42688P_REG_APEX_DATA4 			UINT8_C(0x35)
#define ICM42688P_REG_APEX_DATA5 			UINT8_C(0x36)
#define ICM42688P_REG_INT_STATUS2 			UINT8_C(0x37)
#define ICM42688P_REG_INT_STATUS3 			UINT8_C(0x38)
#define ICM42688P_REG_SIGNAL_PATH_RESET		UINT8_C(0x4B)
#define ICM42688P_REG_INTF_CONFIG0 			UINT8_C(0x4C)
#define ICM42688P_REG_INTF_CONFIG1 			UINT8_C(0x4D)
#define ICM42688P_REG_PWR_MGMT0 			UINT8_C(0x4E)
#define ICM42688P_REG_GYRO_CONFIG0 			UINT8_C(0x4F)
#define ICM42688P_REG_ACCEL_CONFIG0 		UINT8_C(0x50)
#define ICM42688P_REG_GYRO_CONFIG1 			UINT8_C(0x51)
#define ICM42688P_REG_GYRO_ACCEL_CONFIG0 	UINT8_C(0x52)
#define ICM42688P_REG_ACCEL_CONFIG1 		UINT8_C(0x53)
#define ICM42688P_REG_TMST_CONFIG 			UINT8_C(0x54)
#define ICM42688P_REG_APEX_CONFIG0 			UINT8_C(0x55)
#define ICM42688P_REG_SMD_CONFIG 			UINT8_C(0x56)
#define ICM42688P_REG_FIFO_CONFIG1 			UINT8_C(0x5F)
#define ICM42688P_REG_FIFO_CONFIG2 			UINT8_C(0x60)
#define ICM42688P_REG_FIFO_CONFIG3 			UINT8_C(0x61)
#define ICM42688P_REG_FSYNC_CONFIG 			UINT8_C(0x62)
#define ICM42688P_REG_INT_CONFIG0 			UINT8_C(0x63)
#define ICM42688P_REG_INT_CONFIG1 			UINT8_C(0x64)
#define ICM42688P_REG_INT_SOURCE0 			UINT8_C(0x65)
#define ICM42688P_REG_INT_SOURCE1 			UINT8_C(0x66)
#define ICM42688P_REG_INT_SOURCE3 			UINT8_C(0x68)
#define ICM42688P_REG_INT_SOURCE4 			UINT8_C(0x69)
#define ICM42688P_REG_FIFO_LOST_PKT0 		UINT8_C(0x6C)
#define ICM42688P_REG_FIFO_LOST_PKT1 		UINT8_C(0x6D)
#define ICM42688P_REG_SELF_TEST_CONFIG 		UINT8_C(0x70)
#define ICM42688P_REG_WHO_AM_I 				UINT8_C(0x75)
#define ICM42688P_REG_REG_BANK_SEL 			UINT8_C(0x76)
/* Register Map Ends ---------------------------------------------------------*/

/* Define Accelerometer Macros -----------------------------------------------*/
#define ICM42688P_ID 						UINT8_C(0x47)
#define ICM42688P_SOFT_RESET				UINT8_C(0x01)

#define ICM42688P_ACCEL_POWER_OFF			UINT8_C(0x00)
#define ICM42688P_ACCEL_STANDBY				UINT8_C(0x01)
#define ICM42688P_ACCEL_LOW_POWER 			UINT8_C(0x02)
#define ICM42688P_ACCEL_LOW_NOISE			UINT8_C(0x03)

#define ICM42688P_ACCEL_16G 				UINT8_C(0x00)
#define ICM42688P_ACCEL_8G 					UINT8_C(0x20)
#define ICM42688P_ACCEL_4G 					UINT8_C(0x40)
#define ICM42688P_ACCEL_2G 					UINT8_C(0x60)

#define ICM42688P_ACCEL_ODR_32KHz			UINT8_C(0x01)
#define ICM42688P_ACCEL_ODR_16KHz			UINT8_C(0x02)
#define ICM42688P_ACCEL_ODR_8KHz			UINT8_C(0x03)
#define ICM42688P_ACCEL_ODR_4KHz			UINT8_C(0x04)
#define ICM42688P_ACCEL_ODR_2KHz			UINT8_C(0x05)
#define ICM42688P_ACCEL_ODR_1KHz			UINT8_C(0x06)
#define ICM42688P_ACCEL_ODR_200Hz			UINT8_C(0x07)
#define ICM42688P_ACCEL_ODR_100Hz			UINT8_C(0x08)
#define ICM42688P_ACCEL_ODR_50Hz			UINT8_C(0x09)
#define ICM42688P_ACCEL_ODR_25Hz			UINT8_C(0x0A)
#define ICM42688P_ACCEL_ODR_12_5Hz			UINT8_C(0x0B)
#define ICM42688P_ACCEL_ODR_6_25Hz			UINT8_C(0x0C)
#define ICM42688P_ACCEL_ODR_3_125Hz			UINT8_C(0x0D)
#define ICM42688P_ACCEL_ODR_1_5625Hz		UINT8_C(0x0E)
#define ICM42688P_ACCEL_ODR_500Hz			UINT8_C(0x0F)

/* Define Gyroscope Macros ---------------------------------------------------*/
#define ICM42688P_GYRO_POWER_OFF			UINT8_C(0x00)
#define ICM42688P_GYRO_STANDBY			    UINT8_C(0x04)
#define ICM42688P_GYRO_LOW_NOISE 			UINT8_C(0x0C)

#define ICM42688P_GYRO_2000_DPS 			UINT8_C(0x00)
#define ICM42688P_GYRO_1000_DPS 			UINT8_C(0x20)
#define ICM42688P_GYRO_500_DPS 				UINT8_C(0x40)
#define ICM42688P_GYRO_250_DPS 				UINT8_C(0x60)
#define ICM42688P_GYRO_125_DPS 				UINT8_C(0x80)
#define ICM42688P_GYRO_62_5_DPS 			UINT8_C(0xA0)
#define ICM42688P_GYRO_31_25_DPS 			UINT8_C(0xC0)
#define ICM42688P_GYRO_15_625_DPS 			UINT8_C(0xE0)

#define ICM42688P_GYRO_ODR_32KHz 			UINT8_C(0x01)
#define ICM42688P_GYRO_ODR_16KHz 			UINT8_C(0x02)
#define ICM42688P_GYRO_ODR_8KHz 			UINT8_C(0x03)
#define ICM42688P_GYRO_ODR_4KHz 			UINT8_C(0x04)
#define ICM42688P_GYRO_ODR_2KHz 			UINT8_C(0x05)
#define ICM42688P_GYRO_ODR_1KHz 			UINT8_C(0x06)
#define ICM42688P_GYRO_ODR_200Hz 			UINT8_C(0x07)
#define ICM42688P_GYRO_ODR_100Hz 			UINT8_C(0x08)
#define ICM42688P_GYRO_ODR_50Hz 			UINT8_C(0x09)
#define ICM42688P_GYRO_ODR_25Hz 			UINT8_C(0x0A)
#define ICM42688P_GYRO_ODR_12_5Hz 			UINT8_C(0x0B)
#define ICM42688P_GYRO_ODR_500Hz 			UINT8_C(0x0F)

/* Structure Definition ------------------------------------------------------*/
typedef struct _ICM42688P{
	short temperature_raw;
	short accel_x_raw;
	short accel_y_raw;
	short accel_z_raw;
	short gyro_x_raw;
	short gyro_y_raw;
	short gyro_z_raw;

	//	short timestamp;

	float temperature;
	float accel_x;
	float accel_y;
	float accel_z;
	float gyro_x;
	float gyro_y;
	float gyro_z;
}Struct_ICM42688P;

extern Struct_ICM42688P ICM42688P;

/* Read and Write Function ---------------------------------------------------*/
uint8_t ICM42688P_Readbyte(uint8_t reg_addr);
void 	ICM42688P_Read_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data);
void 	ICM42688P_Writebyte(uint8_t reg_addr, uint8_t val);
void 	ICM42688P_Write_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data);

/* External Configuration Function -------------------------------------------*/
void ICM42688P_SPI_GPIO_Initialization(void);
int  ICM42688P_Initialization(void);
void ICM42688P_Get6AxisRawData(short* accel, short* gyro);
void ICM42688P_Get3AxisAccRawData(short* accel);
void ICM42688P_Get3AxisGyroRawData(short* gyro);
void ICM42688P_TempRawData(short* temperature);
void ICM42688P_Calculate_Accel_Gyro_Temp(Struct_ICM42688P* ICM42688P, float range, float dps);	// ALREADY HAS FORMULA AND RAW DATA
//void ICM42688P_TimeStampRawData(short* timestamp);
int  ICM42688P_DataReady(void);

#ifdef __cplusplus
}
#endif

#endif /* __ICM42688P_H__ */
