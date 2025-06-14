/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		bmi088.h
 * @brief   	:		This file contains all the constants parameters for BMI088 IMU
 * ______________________________________________________________________________________________________
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BMI088_H__
#define __BMI088_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Definitions ---------------------------------------------------------------*/
#define BMI088_SPI_CHANNEL				SPI4

#define BMI088_SPI_SCLK_PIN				LL_GPIO_PIN_2
#define BMI088_SPI_SCLK_PORT			GPIOE
#define BMI088_SPI_SCLK_CLK				LL_AHB4_GRP1_PERIPH_GPIOE

#define BMI088_SPI_MISO_PIN				LL_GPIO_PIN_5
#define BMI088_SPI_MISO_PORT			GPIOE
#define BMI088_SPI_MISO_CLK				LL_AHB4_GRP1_PERIPH_GPIOE

#define BMI088_SPI_MOSI_PIN				LL_GPIO_PIN_6
#define BMI088_SPI_MOSI_PORT			GPIOE
#define BMI088_SPI_MOSI_CLK				LL_AHB4_GRP1_PERIPH_GPIOE

#define BMI088_SPI_nCS_ACCEL_PIN		LL_GPIO_PIN_10
#define BMI088_SPI_nCS_ACCEL_PORT		GPIOE
#define BMI088_SPI_nCS_ACCEL_CLK		LL_AHB4_GRP1_PERIPH_GPIOE
#define BMI088_SPI_nCS_GYRO_PIN			LL_GPIO_PIN_11
#define BMI088_SPI_nCS_GYRO_PORT		GPIOE
#define BMI088_SPI_nCS_GYRO_CLK			LL_AHB4_GRP1_PERIPH_GPIOE

#define BMI088_INT_ACCEL1_PIN			LL_GPIO_PIN_4
#define BMI088_INT_ACCEL1_PORT			GPIOG
#define BMI088_INT_ACCEL1_CLK			LL_AHB4_GRP1_PERIPH_GPIOG
#define BMI088_INT_ACCEL2_PIN			LL_GPIO_PIN_5
#define BMI088_INT_ACCEL2_PORT			GPIOG
#define BMI088_INT_ACCEL2_CLK			LL_AHB4_GRP1_PERIPH_GPIOG

#define BMI088_INT_GYRO1_PIN			LL_GPIO_PIN_6
#define BMI088_INT_GYRO1_PORT			GPIOG
#define BMI088_INT_GYRO1_CLK			LL_AHB4_GRP1_PERIPH_GPIOG
#define BMI088_INT_GYRO2_PIN			LL_GPIO_PIN_7
#define BMI088_INT_GYRO2_PORT			GPIOG
#define BMI088_INT_GYRO2_CLK			LL_AHB4_GRP1_PERIPH_GPIOG

#define BMI088_CHIP_SELECT_ACCEL(BMI088)		LL_GPIO_ResetOutputPin(BMI088_SPI_nCS_ACCEL_PORT, BMI088_SPI_nCS_ACCEL_PIN)
#define BMI088_CHIP_DESELECT_ACCEL(BMI088)		LL_GPIO_SetOutputPin(  BMI088_SPI_nCS_ACCEL_PORT, BMI088_SPI_nCS_ACCEL_PIN)
#define BMI088_CHIP_SELECT_GYRO(BMI088)			LL_GPIO_ResetOutputPin(BMI088_SPI_nCS_GYRO_PORT,  BMI088_SPI_nCS_GYRO_PIN)
#define BMI088_CHIP_DESELECT_GYRO(BMI088)		LL_GPIO_SetOutputPin(  BMI088_SPI_nCS_GYRO_PORT,  BMI088_SPI_nCS_GYRO_PIN)

/* Accelerometer Register Map ------------------------------------------------*/
#define BMI088_ACC_REG_CHIP_ID 				UINT8_C(0x00)
#define BMI088_ACC_REG_ERR_REG 				UINT8_C(0x02)
#define BMI088_ACC_REG_STATUS 				UINT8_C(0x03)
#define BMI088_ACC_REG_X_LSB 				UINT8_C(0x12)
#define BMI088_ACC_REG_X_MSB 				UINT8_C(0x13)
#define BMI088_ACC_REG_Y_LSB 				UINT8_C(0x14)
#define BMI088_ACC_REG_Y_MSB 				UINT8_C(0x15)
#define BMI088_ACC_REG_Z_LSB 				UINT8_C(0x16)
#define BMI088_ACC_REG_Z_MSB 				UINT8_C(0x17)
#define BMI088_ACC_REG_SENSORTIME_0 		UINT8_C(0x18)
#define BMI088_ACC_REG_SENSORTIME_1 		UINT8_C(0x19)
#define BMI088_ACC_REG_SENSORTIME_2 		UINT8_C(0x1A)
#define BMI088_ACC_REG_INT_STATUS_1 		UINT8_C(0x1D)
#define BMI088_ACC_REG_TEMP_MSB 			UINT8_C(0x22)
#define BMI088_ACC_REG_TEMP_LSB 			UINT8_C(0x23)
#define BMI088_ACC_REG_FIFO_LENGTH_0 		UINT8_C(0x24)
#define BMI088_ACC_REG_FIFO_LENGTH_1 		UINT8_C(0x25)
#define BMI088_ACC_REG_FIFO_DATA 			UINT8_C(0x26)
#define BMI088_ACC_REG_CONF 				UINT8_C(0x40)
#define BMI088_ACC_REG_RANGE 				UINT8_C(0x41)
#define BMI088_ACC_REG_FIFO_DOWNS 			UINT8_C(0x45)
#define BMI088_ACC_REG_FIFO_WTM_0 			UINT8_C(0x46)
#define BMI088_ACC_REG_FIFO_WTM_1 			UINT8_C(0x47)
#define BMI088_ACC_REG_FIFO_CONFIG_0 		UINT8_C(0x48)
#define BMI088_ACC_REG_FIFO_CONFIG_1 		UINT8_C(0x49)
#define BMI088_ACC_REG_INT1_IO_CTRL 		UINT8_C(0x53)
#define BMI088_ACC_REG_INT2_IO_CTRL 		UINT8_C(0x54)
#define BMI088_ACC_REG_INT_MAP_DATA 		UINT8_C(0x58)
#define BMI088_ACC_REG_SELF_TEST 			UINT8_C(0x6D)
#define BMI088_ACC_REG_PWR_CONF 			UINT8_C(0x7C)
#define BMI088_ACC_REG_PWR_CTRL 			UINT8_C(0x7D)
#define BMI088_ACC_REG_SOFT_RESET 			UINT8_C(0x7E)
/* Accelerometer Register Map Ends -------------------------------------------*/

/* Gyroscope Register Map ----------------------------------------------------*/
#define BMI088_GYRO_REG_CHIP_ID 			UINT8_C(0x00)
#define BMI088_GYRO_REG_RATE_X_LSB 			UINT8_C(0x02)
#define BMI088_GYRO_REG_RATE_X_MSB 			UINT8_C(0x03)
#define BMI088_GYRO_REG_RATE_Y_LSB 			UINT8_C(0x04)
#define BMI088_GYRO_REG_RATE_Y_MSB 			UINT8_C(0x05)
#define BMI088_GYRO_REG_RATE_Z_LSB 			UINT8_C(0x06)
#define BMI088_GYRO_REG_RATE_Z_MSB 			UINT8_C(0x07)
#define BMI088_GYRO_REG_INT_STATUS_1 		UINT8_C(0x0A)
#define BMI088_GYRO_REG_FIFO_STATUS 		UINT8_C(0x0E)
#define BMI088_GYRO_REG_RANGE 				UINT8_C(0x0F)
#define BMI088_GYRO_REG_BANDWIDTH 			UINT8_C(0x10)
#define BMI088_GYRO_REG_LPM1 				UINT8_C(0x11)
#define BMI088_GYRO_REG_SOFT_RESET 			UINT8_C(0x14)
#define BMI088_GYRO_REG_INT_CTRL 			UINT8_C(0x15)
#define BMI088_GYRO_REG_INT3_INT4_IO_CONF 	UINT8_C(0x16)
#define BMI088_GYRO_REG_INT3_INT4_IO_MAP 	UINT8_C(0x18)
#define BMI088_GYRO_REG_FIFO_WM_EN 			UINT8_C(0x1E)
#define BMI088_GYRO_REG_FIFO_EXT_INT_S 		UINT8_C(0x34)
#define BMI088_GYRO_REG_SELF_TEST 			UINT8_C(0x3C)
#define BMI088_GYRO_REG_FIFO_CONFIG_0		UINT8_C(0x3D)
#define BMI088_GYRO_REG_FIFO_CONFIG_1		UINT8_C(0x3E)
#define BMI088_GYRO_REG_FIFO_DATA 			UINT8_C(0x3F)
/* Gyroscope Register Map Ends -----------------------------------------------*/

/* Define Accelerometer Macros -----------------------------------------------*/
#define BMI088_SOFT_RESET_VALUE         UINT8_C(0xB6)

#define BMI088_ACCEL_ON       			UINT8_C(0x04)
#define BMI088_ACCEL_SUSPEND_MODE       UINT8_C(0x03)
#define BMI088_ACCEL_ACTIVE_MODE       	UINT8_C(0x00)

#define BMI088_ACCEL_RANGE_3G       	UINT8_C(0x00)
#define BMI088_ACCEL_RANGE_6G       	UINT8_C(0x01)
#define BMI088_ACCEL_RANGE_12G      	UINT8_C(0x02)
#define BMI088_ACCEL_RANGE_24G      	UINT8_C(0x03)

#define BMI088_ACCEL_OSR4       		UINT8_C(0x80)
#define BMI088_ACCEL_OSR2      			UINT8_C(0x90)
#define BMI088_ACCEL_NORMAL       		UINT8_C(0xA0)

#define BMI088_ACCEL_ODR_12_5       	UINT8_C(0x05)
#define BMI088_ACCEL_ODR_25       		UINT8_C(0x06)
#define BMI088_ACCEL_ODR_50       		UINT8_C(0x07)
#define BMI088_ACCEL_ODR_100       		UINT8_C(0x08)
#define BMI088_ACCEL_ODR_200       		UINT8_C(0x09)
#define BMI088_ACCEL_ODR_400       		UINT8_C(0x0A)
#define BMI088_ACCEL_ODR_800       		UINT8_C(0x0B)
#define BMI088_ACCEL_ODR_1600       	UINT8_C(0x0C)

/* Define Gyroscope Macros ---------------------------------------------------*/
#define BMI088_GYRO_NORMAL   			UINT8_C(0x00)
#define BMI088_GYRO_SUSPEND_MODE   		UINT8_C(0x80)
#define BMI088_GYRO_ACTIVE_MODE   		UINT8_C(0x20)

#define BMI088_GYRO_RANGE_2000DPS   	UINT8_C(0x00)
#define BMI088_GYRO_RANGE_1000DPS   	UINT8_C(0x01)
#define BMI088_GYRO_RANGE_500DPS    	UINT8_C(0x02)
#define BMI088_GYRO_RANGE_250DPS    	UINT8_C(0x03)
#define BMI088_GYRO_RANGE_125DPS    	UINT8_C(0x04)

#define BMI088_GYRO_ODR_2000_532BW      UINT8_C(0x00)	// BANDWIDTH 532 HZ
#define BMI088_GYRO_ODR_2000_230BW      UINT8_C(0x01)	// BANDWIDTH 230 HZ
#define BMI088_GYRO_ODR_1000            UINT8_C(0x02)	// BANDWIDTH 116 HZ
#define BMI088_GYRO_ODR_400             UINT8_C(0x03)	// BANDWIDTH 47 HZ
#define BMI088_GYRO_ODR_200_23BW        UINT8_C(0x04)	// BANDWIDTH 23 HZ
#define BMI088_GYRO_ODR_100_12BW        UINT8_C(0x05)	// BANDWIDTH 12 HZ
#define BMI088_GYRO_ODR_200_64BW        UINT8_C(0x06)	// BANDWIDTH 64 HZ
#define BMI088_GYRO_ODR_100_32BW        UINT8_C(0x07)	// BANDWIDTH 32 HZ

//#define BMI088_ACTIVE_HIGH			UINT8_C(0x01)
//#define BMI088_ACTIVE_LOW				UINT8_C(0x00)
//#define BMI088_PUSH_PULL				UINT8_C(0x00)
//#define BMI088_OPEN_DRAIN				UINT8_C(0x01)

/* Structure Definition ------------------------------------------------------*/
typedef struct _BMI088{
	short accel_x_raw;
	short accel_y_raw;
	short accel_z_raw;
	short temperature_raw;
	short gyro_x_raw;
	short gyro_y_raw;
	short gyro_z_raw;

	float accel_x;
	float accel_y;
	float accel_z;
	float temperature;
	float gyro_x;
	float gyro_y;
	float gyro_z;

	uint32_t sensortime;
}Struct_BMI088;

extern Struct_BMI088 BMI088;

/* Read and Write Function ---------------------------------------------------*/
uint8_t BMI088_Readbyte(uint8_t reg_addr);
void 	BMI088_Read_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data);
void 	BMI088_Writebyte(uint8_t reg_addr, uint8_t val);
void 	BMI088_Write_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data);

/* External Configuration Function -------------------------------------------*/
void BMI088_SPI_GPIO_Initialization(void);
int  BMI088_Initialization(void);
//void BMI088_Get6AxisRawData(short* accel, short* gyro);
void BMI088_Get3AxisAccelRawData(short* accel);
void BMI088_Get3AxisGyroRawData(short* gyro);
void BMI088_GetSensortime(uint32_t* sensortime);
void BMI088_TempRawData(short* temperature);
void BMI088_Calculate_Accel_Temp(Struct_BMI088* BMI088, float range);	// ALREADY HAS FORMULA AND RAW DATA
void BMI088_Calculate_Gyro(Struct_BMI088* BMI088, float dps);			// ALREADY HAS FORMULA AND RAW DATA
int  BMI088_DataReady_ACCEL(void);
int  BMI088_DataReady_GYRO(void);

#ifdef __cplusplus
}
#endif

#endif /* __BMI088_H__ */
