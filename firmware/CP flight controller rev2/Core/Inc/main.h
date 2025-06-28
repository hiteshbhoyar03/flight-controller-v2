/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

#include "stm32h7xx_ll_rcc.h"
#include "stm32h7xx_ll_crs.h"
#include "stm32h7xx_ll_bus.h"
#include "stm32h7xx_ll_system.h"
#include "stm32h7xx_ll_exti.h"
#include "stm32h7xx_ll_cortex.h"
#include "stm32h7xx_ll_utils.h"
#include "stm32h7xx_ll_pwr.h"
#include "stm32h7xx_ll_dma.h"
#include "stm32h7xx_ll_spi.h"
#include "stm32h7xx_ll_tim.h"
#include "stm32h7xx_ll_usart.h"
#include "stm32h7xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPIO_01_CP_Pin LL_GPIO_PIN_2
#define GPIO_01_CP_GPIO_Port GPIOE
#define GPIO_02_CP_Pin LL_GPIO_PIN_3
#define GPIO_02_CP_GPIO_Port GPIOE
#define GPIO_03_CP_Pin LL_GPIO_PIN_4
#define GPIO_03_CP_GPIO_Port GPIOE
#define GPIO_04_CP_Pin LL_GPIO_PIN_5
#define GPIO_04_CP_GPIO_Port GPIOE
#define GPIO_05_CP_Pin LL_GPIO_PIN_6
#define GPIO_05_CP_GPIO_Port GPIOE
#define GPIO_06_CP_Pin LL_GPIO_PIN_13
#define GPIO_06_CP_GPIO_Port GPIOC
#define GPIO_07_CP_Pin LL_GPIO_PIN_14
#define GPIO_07_CP_GPIO_Port GPIOC
#define GPIO_08_CP_Pin LL_GPIO_PIN_15
#define GPIO_08_CP_GPIO_Port GPIOC
#define nCS_BMI088_GYRO_Pin LL_GPIO_PIN_4
#define nCS_BMI088_GYRO_GPIO_Port GPIOA
#define INT_BMI088_GYRO1_Pin LL_GPIO_PIN_4
#define INT_BMI088_GYRO1_GPIO_Port GPIOC
#define INT_BMI088_GYRO2_Pin LL_GPIO_PIN_5
#define INT_BMI088_GYRO2_GPIO_Port GPIOC
#define nCS_BMI088_ACCEL_Pin LL_GPIO_PIN_0
#define nCS_BMI088_ACCEL_GPIO_Port GPIOB
#define INT_BMI088_ACCEL1_Pin LL_GPIO_PIN_1
#define INT_BMI088_ACCEL1_GPIO_Port GPIOB
#define INT_BMI088_ACCEL2_Pin LL_GPIO_PIN_2
#define INT_BMI088_ACCEL2_GPIO_Port GPIOB
#define INT_ICM45686_Pin LL_GPIO_PIN_9
#define INT_ICM45686_GPIO_Port GPIOE
#define nCS_ICM45686_Pin LL_GPIO_PIN_10
#define nCS_ICM45686_GPIO_Port GPIOE
#define INT_ICM42688P_Pin LL_GPIO_PIN_11
#define INT_ICM42688P_GPIO_Port GPIOE
#define nCS_ICM42688P_Pin LL_GPIO_PIN_15
#define nCS_ICM42688P_GPIO_Port GPIOE
#define nCS_EEPROM_CP_Pin LL_GPIO_PIN_12
#define nCS_EEPROM_CP_GPIO_Port GPIOB
#define GPS_nRESET_Pin LL_GPIO_PIN_14
#define GPS_nRESET_GPIO_Port GPIOD
#define GPS_LNA_EN_Pin LL_GPIO_PIN_15
#define GPS_LNA_EN_GPIO_Port GPIOD
#define GPS_TIMEPULSE_Pin LL_GPIO_PIN_8
#define GPS_TIMEPULSE_GPIO_Port GPIOC
#define nCS_MS5611_Pin LL_GPIO_PIN_15
#define nCS_MS5611_GPIO_Port GPIOA
#define DEBUG_LED1_CP_Pin LL_GPIO_PIN_0
#define DEBUG_LED1_CP_GPIO_Port GPIOD
#define DEBUG_LED2_CP_Pin LL_GPIO_PIN_1
#define DEBUG_LED2_CP_GPIO_Port GPIOD
#define DEBUG_LED3_CP_Pin LL_GPIO_PIN_2
#define DEBUG_LED3_CP_GPIO_Port GPIOD
#define DEBUG_LED4_CP_Pin LL_GPIO_PIN_3
#define DEBUG_LED4_CP_GPIO_Port GPIOD
#define DEBUG_LED5_CP_Pin LL_GPIO_PIN_4
#define DEBUG_LED5_CP_GPIO_Port GPIOD
#define DEBUG_LED6_CP_Pin LL_GPIO_PIN_5
#define DEBUG_LED6_CP_GPIO_Port GPIOD
#define INT_MMC5983MA_Pin LL_GPIO_PIN_8
#define INT_MMC5983MA_GPIO_Port GPIOB
#define nCS_MMC5983MA_Pin LL_GPIO_PIN_9
#define nCS_MMC5983MA_GPIO_Port GPIOB
#define INT_ICP20100_Pin LL_GPIO_PIN_0
#define INT_ICP20100_GPIO_Port GPIOE
#define nCS_ICP20100_Pin LL_GPIO_PIN_1
#define nCS_ICP20100_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
