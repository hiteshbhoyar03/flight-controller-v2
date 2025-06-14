/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		bmm350.h
 * @brief   	:		This file contains all the constants parameters for BMM350 mag
 * ______________________________________________________________________________________________________
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BMM350_H__
#define __BMM350_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Definitions ---------------------------------------------------------------*/
#define BMI088_I2C_CHANNEL		I2C1

#define BMM350_INT_PIN			LL_GPIO_PIN_4
#define BMM350_INT_PORT			GPIOI
#define BMM350_INT_CLK			LL_AHB4_GRP1_PERIPH_GPIOI

/* Register Map --------------------------------------------------------------*/
#define BMM350_REG_CHIP_ID                          UINT8_C(0x00)
// #define BMM350_REG_REV_ID                           UINT8_C(0x01)
#define BMM350_REG_ERR_REG                          UINT8_C(0x02)
#define BMM350_REG_PAD_CTRL                         UINT8_C(0x03)
#define BMM350_REG_PMU_CMD_AGGR_SET                 UINT8_C(0x04)
#define BMM350_REG_PMU_CMD_AXIS_EN                  UINT8_C(0x05)
#define BMM350_REG_PMU_CMD                          UINT8_C(0x06)
#define BMM350_REG_PMU_CMD_STATUS_0                 UINT8_C(0x07)
#define BMM350_REG_PMU_CMD_STATUS_1                 UINT8_C(0x08)
#define BMM350_REG_I3C_ERR                          UINT8_C(0x09)
#define BMM350_REG_I2C_WDT_SET                      UINT8_C(0x0A)
// #define BMM350_REG_TRSDCR_REV_ID                    UINT8_C(0x0D)
// #define BMM350_REG_TC_SYNC_TU                       UINT8_C(0x21)
// #define BMM350_REG_TC_SYNC_ODR                      UINT8_C(0x22)
// #define BMM350_REG_TC_SYNC_TPH_1                    UINT8_C(0x23)
// #define BMM350_REG_TC_SYNC_TPH_2                    UINT8_C(0x24)
// #define BMM350_REG_TC_SYNC_DT                       UINT8_C(0x25)
// #define BMM350_REG_TC_SYNC_ST_0                     UINT8_C(0x26)
// #define BMM350_REG_TC_SYNC_ST_1                     UINT8_C(0x27)
// #define BMM350_REG_TC_SYNC_ST_2                     UINT8_C(0x28)
// #define BMM350_REG_TC_SYNC_STATUS                   UINT8_C(0x29)
#define BMM350_REG_INT_CTRL                         UINT8_C(0x2E)
#define BMM350_REG_INT_CTRL_IBI                     UINT8_C(0x2F)
#define BMM350_REG_INT_STATUS                       UINT8_C(0x30)
#define BMM350_REG_MAG_X_XLSB                       UINT8_C(0x31)
#define BMM350_REG_MAG_X_LSB                        UINT8_C(0x32)
#define BMM350_REG_MAG_X_MSB                        UINT8_C(0x33)
#define BMM350_REG_MAG_Y_XLSB                       UINT8_C(0x34)
#define BMM350_REG_MAG_Y_LSB                        UINT8_C(0x35)
#define BMM350_REG_MAG_Y_MSB                        UINT8_C(0x36)
#define BMM350_REG_MAG_Z_XLSB                       UINT8_C(0x37)
#define BMM350_REG_MAG_Z_LSB                        UINT8_C(0x38)
#define BMM350_REG_MAG_Z_MSB                        UINT8_C(0x39)
#define BMM350_REG_TEMP_XLSB                        UINT8_C(0x3A)
#define BMM350_REG_TEMP_LSB                         UINT8_C(0x3B)
#define BMM350_REG_TEMP_MSB                         UINT8_C(0x3C)
#define BMM350_REG_SENSORTIME_XLSB                  UINT8_C(0x3D)
#define BMM350_REG_SENSORTIME_LSB                   UINT8_C(0x3E)
#define BMM350_REG_SENSORTIME_MSB                   UINT8_C(0x3F)
#define BMM350_REG_OTP_CMD_REG                      UINT8_C(0x50)
#define BMM350_REG_OTP_DATA_MSB_REG                 UINT8_C(0x52)
#define BMM350_REG_OTP_DATA_LSB_REG                 UINT8_C(0x53)
#define BMM350_REG_OTP_STATUS_REG                   UINT8_C(0x55)
#define BMM350_REG_TMR_SELFTEST_USER                UINT8_C(0x60)
#define BMM350_REG_CTRL_USER                        UINT8_C(0x61)
#define BMM350_REG_CMD                              UINT8_C(0x7E)


/* Define  Macros ------------------------------------------------------------*/
#define BMM350_CHIP_ID_VAL 0x33

/* BMM350_REG_PMU_CMD_AGGR_SET -----------------------------------------------*/
// ODR
#define BMM350_ODR_400HZ                            UINT8_C(0x02)
#define BMM350_ODR_200HZ                            UINT8_C(0x03)
#define BMM350_ODR_100HZ                            UINT8_C(0x04)
#define BMM350_ODR_50HZ                             UINT8_C(0x05)
#define BMM350_ODR_25HZ                             UINT8_C(0x06)
#define BMM350_ODR_12_5HZ                           UINT8_C(0x07)
#define BMM350_ODR_6_25HZ                           UINT8_C(0x08)
#define BMM350_ODR_3_125HZ                          UINT8_C(0x09)
#define BMM350_ODR_1_5625HZ                         UINT8_C(0x0A)
// AVG
#define BMM350_AVG_NO_AVG                           UINT8_C(0x00)
#define BMM350_AVG_2                                UINT8_C(0x10)
#define BMM350_AVG_4                                UINT8_C(0x20)
#define BMM350_AVG_8                                UINT8_C(0x30)

/* BMM350_REG_PMU_CMD_AXIS_EN ------------------------------------------------*/
#define BMM350_EN_AXES_X                            UINT8_C(0x01)
#define BMM350_EN_AXES_Y                            UINT8_C(0x02)
#define BMM350_EN_AXES_Z                            UINT8_C(0x04)

/* BMM350_REG_PMU_CMD --------------------------------------------------------*/
#define BMM350_PMU_CMD_SUSPEND                      UINT8_C(0x00)
#define BMM350_PMU_CMD_NORMAL_MODE                  UINT8_C(0x01)
#define BMM350_PMU_CMD_UPDATE_ODR_AVG               UINT8_C(0x02)
#define BMM350_PMU_CMD_FORCED_MODE                  UINT8_C(0x03)
#define BMM350_PMU_CMD_FORCED_MODE_FAST             UINT8_C(0x04)
#define BMM350_PMU_CMD_FLUX_GUIDE_RESET             UINT8_C(0x05)
#define BMM350_PMU_CMD_FLUX_GUIDE_RESET_FAST        UINT8_C(0x06)
#define BMM350_PMU_CMD_BIT_RESET                    UINT8_C(0x07)
#define BMM350_PMU_CMD_BIT_RESET_FAST               UINT8_C(0x08)

/* BMM350_REG_INT_CTRL -------------------------------------------------------*/
#define BMM350_INT_MODE_PULSE                       UINT8_C(0x01)
#define BMM350_INT_MODE_LATCHED                     UINT8_C(0x00)
#define BMM350_INT_POL_ACTIVE_HIGH                  UINT8_C(0x02)
#define BMM350_INT_POL_ACTIVE_LOW                   UINT8_C(0x00)
#define BMM350_INT_OD_PUSHPULL                      UINT8_C(0x04)
#define BMM350_INT_OD_OPENDRAIN                     UINT8_C(0x00)
#define BMM350_INT_OUTPUT_EN_OFF                    UINT8_C(0x00)
#define BMM350_INT_OUTPUT_EN_ON                     UINT8_C(0x08)
#define BMM350_INT_DRDY_EN                          UINT8_C(0x80)
#define BMM350_INT_DRDY_DIS                         UINT8_C(0x00)




#define BMM350_DISABLE                              UINT8_C(0x0)
#define BMM350_ENABLE                               UINT8_C(0x1)

#define BMM350_CMD_NOP                              UINT8_C(0x0)
#define BMM350_CMD_SOFTRESET                        UINT8_C(0xB6)

/* Structure Definition ------------------------------------------------------*/
typedef struct _BMM350{
	uint32_t mag_x_raw;
	uint32_t mag_y_raw;
	uint32_t mag_z_raw;
	uint32_t temperature_raw;
	uint32_t sensortime;

	float mag_x;
	float mag_y;
	float mag_z;
	float temperature;

}Struct_BMM350;

extern Struct_BMM350 BMM350;

/* External Configuration Function -------------------------------------------*/
void BMM350_I2C_GPIO_Initialization(void);
int  BMM350_Initialization(void);
void BMM350_GetAllRawData(uint32_t* mag , uint32_t* temperature , uint32_t* sensortime);
void BMM350_Get3AxisMagRawData(uint32_t* mag);
void BMM350_GetTemperatureRawData(uint32_t* temperature);
void BMM350_GetSensortimeRawData(uint32_t* sensortime);
int  BMM350_DataReady(void);

#ifdef __cplusplus
}
#endif

#endif /* __BMM350_H__ */
