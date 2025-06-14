/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		icm42686.h
 * @brief   	:		This file contains all the constants parameters for ICM45686 IMU
 * ______________________________________________________________________________________________________
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ICM45686_H__
#define __ICM45686_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Definitions ---------------------------------------------------------------*/
#define ICM45686_SPI_CHANNEL		SPI1

#define ICM45686_SPI_SCLK_PIN		LL_GPIO_PIN_3
#define ICM45686_SPI_SCLK_PORT		GPIOB
#define ICM45686_SPI_SCLK_CLK		LL_AHB4_GRP1_PERIPH_GPIOB

#define ICM45686_SPI_MISO_PIN		LL_GPIO_PIN_4
#define ICM45686_SPI_MISO_PORT		GPIOB
#define ICM45686_SPI_MISO_CLK		LL_AHB4_GRP1_PERIPH_GPIOB

#define ICM45686_SPI_MOSI_PIN		LL_GPIO_PIN_7
#define ICM45686_SPI_MOSI_PORT		GPIOD
#define ICM45686_SPI_MOSI_CLK		LL_AHB4_GRP1_PERIPH_GPIOD

#define ICM45686_SPI_nCS_PIN		LL_GPIO_PIN_7
#define ICM45686_SPI_nCS_PORT		GPIOE
#define ICM45686_SPI_nCS_CLK		LL_AHB4_GRP1_PERIPH_GPIOE

#define ICM45686_INT_PIN			LL_GPIO_PIN_13
#define ICM45686_INT_PORT			GPIOF
#define ICM45686_INT_CLK			LL_AHB4_GRP1_PERIPH_GPIOF

#define ICM45686_CHIP_SELECT(ICM45686)		LL_GPIO_ResetOutputPin(ICM45686_SPI_nCS_PORT, ICM45686_SPI_nCS_PIN)
#define ICM45686_CHIP_DESELECT(ICM45686)	LL_GPIO_SetOutputPin(  ICM45686_SPI_nCS_PORT, ICM45686_SPI_nCS_PIN)

/* Register Map --------------------------------------------------------------*/
#define ICM45686_REG_ACCEL_DATA_X1_UI 		    UINT8_C(0x00)
#define ICM45686_REG_ACCEL_DATA_X0_UI 			UINT8_C(0x01)
#define ICM45686_REG_ACCEL_DATA_Y1_UI 			UINT8_C(0x02)
#define ICM45686_REG_ACCEL_DATA_Y0_UI 			UINT8_C(0x03)
#define ICM45686_REG_ACCEL_DATA_Z1_UI 			UINT8_C(0x04)
#define ICM45686_REG_ACCEL_DATA_Z0_UI 			UINT8_C(0x05)
#define ICM45686_REG_GYRO_DATA_X1_UI            UINT8_C(0x06)
#define ICM45686_REG_GYRO_DATA_X0_UI 		    UINT8_C(0x07)
#define ICM45686_REG_GYRO_DATA_Y1_UI 		    UINT8_C(0x08)
#define ICM45686_REG_GYRO_DATA_Y0_UI 		    UINT8_C(0x09)
#define ICM45686_REG_GYRO_DATA_Z1_UI 		    UINT8_C(0x0A)
#define ICM45686_REG_GYRO_DATA_Z0_UI 		    UINT8_C(0x0B)
#define ICM45686_REG_TEMP_DATA1_UI 			    UINT8_C(0x0C)
#define ICM45686_REG_TEMP_DATA0_UI 			    UINT8_C(0x0D)
#define ICM45686_REG_TMST_FSYNCH 			    UINT8_C(0x0E)
#define ICM45686_REG_TMST_FSYNCL 		    	UINT8_C(0x0F)
#define ICM45686_REG_PWR_MGMT0 			        UINT8_C(0x10)
#define ICM45686_REG_FIFO_COUNT_0 			    UINT8_C(0x12)
#define ICM45686_REG_FIFO_COUNT_1 		    	UINT8_C(0x13)
#define ICM45686_REG_FIFO_DATA 		        	UINT8_C(0x14)
#define ICM45686_REG_INT1_CONFIG0 	    		UINT8_C(0x16)
#define ICM45686_REG_INT1_CONFIG1 			    UINT8_C(0x17)
#define ICM45686_REG_INT1_CONFIG2 			    UINT8_C(0x18)
#define ICM45686_REG_INT1_STATUS0 			    UINT8_C(0x19)
#define ICM45686_REG_INT1_STATUS1 		    	UINT8_C(0x1A)
#define ICM45686_REG_ACCEL_CONFIG0 			    UINT8_C(0x1B)
#define ICM45686_REG_GYRO_CONFIG0 			    UINT8_C(0x1C)
#define ICM45686_REG_FIFO_CONFIG0 			    UINT8_C(0x1D)
#define ICM45686_REG_FIFO_CONFIG1_0 			UINT8_C(0x1E)
#define ICM45686_REG_FIFO_CONFIG1_1 			UINT8_C(0x1F)
#define ICM45686_REG_FIFO_CONFIG2 			    UINT8_C(0x20)
#define ICM45686_REG_FIFO_CONFIG3 			    UINT8_C(0x21)
#define ICM45686_REG_FIFO_CONFIG4	        	UINT8_C(0x22)
#define ICM45686_REG_TMST_WOM_CONFIG 			UINT8_C(0x23)
#define ICM45686_REG_FSYNC_CONFIG0 			    UINT8_C(0x24)
#define ICM45686_REG_FSYNC_CONFIG1 			    UINT8_C(0x25)
#define ICM45686_REG_RTC_CONFIG 			    UINT8_C(0x26)
#define ICM45686_REG_DMP_EXT_SEN_ODR_CFG 		UINT8_C(0x27)
#define ICM45686_REG_ODR_DECIMATE_CONFIG 		UINT8_C(0x28)
#define ICM45686_REG_EDMP_APEX_EN0 	            UINT8_C(0x29)
#define ICM45686_REG_EDMP_APEX_EN1 		        UINT8_C(0x2A)
#define ICM45686_REG_APEX_BUFFER_MGMT 			UINT8_C(0x2B)
#define ICM45686_REG_INTF_CONFIG0 			    UINT8_C(0x2C)
#define ICM45686_REG_INTF_CONFIG1_OVRD 			UINT8_C(0x2D)
#define ICM45686_REG_INTF_AUX_CONFIG 			UINT8_C(0x2E)
#define ICM45686_REG_IOC_PAD_SCENARIO 			UINT8_C(0x2F)
#define ICM45686_REG_IOC_PAD_SCENARIO_AUX_OVRD 	UINT8_C(0x30)
#define ICM45686_REG_DRIVE_CONFIG0 			    UINT8_C(0x32)
#define ICM45686_REG_DRIVE_CONFIG1 			    UINT8_C(0x33)
#define ICM45686_REG_DRIVE_CONFIG2 			    UINT8_C(0x34)
#define ICM45686_REG_INT_APEX_CONFIG0 			UINT8_C(0x39)
#define ICM45686_REG_INT_APEX_CONFIG1 			UINT8_C(0x3A)
#define ICM45686_REG_INT_APEX_STATUS0 			UINT8_C(0x3B)
#define ICM45686_REG_INT_APEX_STATUS1 			UINT8_C(0x3C)
#define ICM45686_REG_ACCEL_DATA_X1_AUX1 		UINT8_C(0x44)
#define ICM45686_REG_ACCEL_DATA_X0_AUX1 		UINT8_C(0x45)
#define ICM45686_REG_ACCEL_DATA_Y1_AUX1 		UINT8_C(0x46)
#define ICM45686_REG_ACCEL_DATA_Y0_AUX1 		UINT8_C(0x47)
#define ICM45686_REG_ACCEL_DATA_Z1_AUX1 		UINT8_C(0x48)
#define ICM45686_REG_ACCEL_DATA_Z0_AUX1 		UINT8_C(0x49)
#define ICM45686_REG_GYRO_DATA_X1_AUX1 			UINT8_C(0x4A)
#define ICM45686_REG_GYRO_DATA_X0_AUX1	 		UINT8_C(0x4B)
#define ICM45686_REG_GYRO_DATA_Y1_AUX1 			UINT8_C(0x4C)
#define ICM45686_REG_GYRO_DATA_Y0_AUX1 			UINT8_C(0x4D)
#define ICM45686_REG_GYRO_DATA_Z1_AUX1 			UINT8_C(0x4E)
#define ICM45686_REG_GYRO_DATA_Z0_AUX1 			UINT8_C(0x4F)
#define ICM45686_REG_TEMP_DATA1_AUX1 			UINT8_C(0x50)
#define ICM45686_REG_TEMP_DATA0_AUX1 			UINT8_C(0x51)
#define ICM45686_REG_TMST_FSYNCH_AUX1 			UINT8_C(0x52)
#define ICM45686_REG_TMST_FSYNCL_AUX1 			UINT8_C(0x53)
#define ICM45686_REG_PWR_MGMT_AUX1 			    UINT8_C(0x54)
#define ICM45686_REG_FS_SEL_AUX1 			    UINT8_C(0x55)
#define ICM45686_REG_INT2_CONFIG0 			    UINT8_C(0x56)
#define ICM45686_REG_INT2_CONFIG1 			    UINT8_C(0x57)
#define ICM45686_REG_INT2_CONFIG2 			    UINT8_C(0x58)
#define ICM45686_REG_INT2_STATUS0 			    UINT8_C(0x59)
#define ICM45686_REG_INT2_STATUS1 			    UINT8_C(0x5A)
#define ICM45686_REG_WHO_AM_I 			        UINT8_C(0x72)
#define ICM45686_REG_REG_HOST_MSG 			    UINT8_C(0x73)
#define ICM45686_REG_IREG_ADDR_15_8 			UINT8_C(0x7C)
#define ICM45686_REG_IREG_ADDR_7_0 			    UINT8_C(0x7D)
#define ICM45686_REG_IREG_DATA 			        UINT8_C(0x7E)
#define ICM45686_REG_REG_MISC2 			        UINT8_C(0x7F)
/* Register Map Ends ---------------------------------------------------------*/

/* Define Accelerometer Macros -----------------------------------------------*/

#define ICM45686_ID 			        	UINT8_C(0xE9)


#define ICM45686_ACCEL_POWER_OFF			UINT8_C(0x00)
#define ICM45686_ACCEL_STANDBY				UINT8_C(0x01)
#define ICM45686_ACCEL_LOW_POWER 			UINT8_C(0x02)
#define ICM45686_ACCEL_LOW_NOISE			UINT8_C(0x03)

#define ICM45686_ACCEL_32G 					UINT8_C(0x00)
#define ICM45686_ACCEL_16G 					UINT8_C(0x10)
#define ICM45686_ACCEL_8G 					UINT8_C(0x20)
#define ICM45686_ACCEL_4G 					UINT8_C(0x30)
#define ICM45686_ACCEL_2G 					UINT8_C(0x40)

#define ICM45686_ACCEL_ODR_6_4KHz			UINT8_C(0x03)
#define ICM45686_ACCEL_ODR_3_2KHz			UINT8_C(0x04)
#define ICM45686_ACCEL_ODR_1_6KHz			UINT8_C(0x05)
#define ICM45686_ACCEL_ODR_800Hz			UINT8_C(0x06)
#define ICM45686_ACCEL_ODR_400Hz			UINT8_C(0x07)
#define ICM45686_ACCEL_ODR_200Hz			UINT8_C(0x08)
#define ICM45686_ACCEL_ODR_100Hz			UINT8_C(0x09)
#define ICM45686_ACCEL_ODR_50Hz				UINT8_C(0x0A)
#define ICM45686_ACCEL_ODR_25Hz				UINT8_C(0x0B)
#define ICM45686_ACCEL_ODR_12_5Hz			UINT8_C(0x0C)
#define ICM45686_ACCEL_ODR_6_25Hz			UINT8_C(0x0D)
#define ICM45686_ACCEL_ODR_3_125Hz			UINT8_C(0x0E)
#define ICM45686_ACCEL_ODR_1_5625Hz			UINT8_C(0x0F)

/* Define Gyroscope Macros ---------------------------------------------------*/
#define ICM45686_GYRO_POWER_OFF			    UINT8_C(0x00)
#define ICM45686_GYRO_STANDBY			    UINT8_C(0x04)
#define ICM45686_GYRO_LOW_POWER 			UINT8_C(0x08)
#define ICM45686_GYRO_LOW_NOISE 			UINT8_C(0x0C)

#define ICM45686_GYRO_4000_DPS 				UINT8_C(0x00)
#define ICM45686_GYRO_2000_DPS 				UINT8_C(0x10)
#define ICM45686_GYRO_1000_DPS 				UINT8_C(0x20)
#define ICM45686_GYRO_500_DPS 				UINT8_C(0x30)
#define ICM45686_GYRO_250_DPS 				UINT8_C(0x40)
#define ICM45686_GYRO_125_DPS 				UINT8_C(0x50)
#define ICM45686_GYRO_62_5_DPS 				UINT8_C(0x60)
#define ICM45686_GYRO_31_25_DPS 			UINT8_C(0x70)
#define ICM45686_GYRO_15_625_DPS 			UINT8_C(0x80)

#define ICM45686_GYRO_ODR_6_4KHz 			UINT8_C(0x03)
#define ICM45686_GYRO_ODR_3_2KHz			UINT8_C(0x04)
#define ICM45686_GYRO_ODR_1_6KHz			UINT8_C(0x05)
#define ICM45686_GYRO_ODR_800Hz				UINT8_C(0x06)
#define ICM45686_GYRO_ODR_400Hz				UINT8_C(0x07)
#define ICM45686_GYRO_ODR_200Hz 			UINT8_C(0x08)
#define ICM45686_GYRO_ODR_100Hz 			UINT8_C(0x09)
#define ICM45686_GYRO_ODR_50Hz 				UINT8_C(0x0A)
#define ICM45686_GYRO_ODR_25Hz 				UINT8_C(0x0B)
#define ICM45686_GYRO_ODR_12_5Hz 			UINT8_C(0x0C)
#define ICM45686_GYRO_ODR_6_25Hz			UINT8_C(0x0D)
#define ICM45686_GYRO_ODR_3_125Hz			UINT8_C(0x0E)
#define ICM45686_GYRO_ODR_1_5625Hz			UINT8_C(0x0F)

/* Structure Definition ------------------------------------------------------*/
typedef struct _ICM45686{
	short accel_x_raw;
	short accel_y_raw;
	short accel_z_raw;
	short gyro_x_raw;
	short gyro_y_raw;
	short gyro_z_raw;
	short temperature_raw;

	//	short timestamp;

	float accel_x;
	float accel_y;
	float accel_z;
	float gyro_x;
	float gyro_y;
	float gyro_z;
	float temperature;
}Struct_ICM45686;

extern Struct_ICM45686 ICM45686;

/* Read and Write Function ---------------------------------------------------*/
uint8_t ICM45686_Readbyte(uint8_t reg_addr);
void 	ICM45686_Read_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data);
void 	ICM45686_Writebyte(uint8_t reg_addr, uint8_t val);
void 	ICM45686_Write_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data);

/* External Configuration Function -------------------------------------------*/
void ICM45686_SPI_GPIO_Initialization(void);
int  ICM45686_Initialization(void);
void ICM45686_Get6AxisRawData(short* accel, short* gyro);
void ICM45686_Get3AxisGyroRawData(short* gyro);
void ICM45686_Get3AxisAccRawData(short* accel);
void ICM45686_TempRawData(short* temperature);
void ICM45686_Calculate_Accel_Gyro_Temp(Struct_ICM45686* ICM45686, float range, float dps);	// ALREADY HAS FORMULA AND RAW DATA
//void ICM45686_TimeStampRawData(short* timestamp);
int  ICM45686_DataReady(void);

#ifdef __cplusplus
}
#endif

#endif /* __ICM45686_H__ */
