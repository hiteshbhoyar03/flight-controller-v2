/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "memorymap.h"
#include "spi.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "BMI088.h"
#include "ICM42688P.h"
#include "ICM45686.h"
#include "MS5611.h"
#include "ICP20100.h"
#include "M9N.h"
//#include "EEPROM.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

int _write(int file, char* p, int len)
{
	for(int i=0;i<len;i++)
	{
		while(!LL_USART_IsActiveFlag_TXE(USART2));
		LL_USART_TransmitData8(USART2, *(p+i));
	}
	return len;
}

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEBUG_NO 0

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint8_t usart2_rx_flag;					// DEBUG
extern uint8_t usart2_rx_data;					// DEBUG
//extern uint8_t usart3_rx_flag;					// Uart CP-MP
//extern uint8_t usart3_rx_data;					// Uart CP-MP
extern uint8_t gps_ubx_rx_buffer[36];			// GPS BUFFER
extern uint8_t gps_ubx_rx_complete_flag;		// GPS COMPLETE FLAG

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C3_Init();
  MX_I2C4_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_SPI4_Init();
  MX_SPI6_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_USB_OTG_FS_USB_Init();
  /* USER CODE BEGIN 2 */

	/* ENABLE INTERRUPT --------------------------------------------------------------------------*/
	/* THIS ENABLE INTERRUPT IRQ_HANDLER WHEN DATA IS RECEIVED */
	LL_USART_EnableIT_RXNE(USART2);		// DEBUG
	LL_USART_EnableIT_RXNE(USART3);		// Uart CP-MP
	LL_USART_EnableIT_RXNE(USART6);		// GPS1

	/* INITIALIZATION ----------------------------------------------------------------------------*/
	ICM45686_Initialization();
	ICM42688P_Initialization();
	BMI088_Initialization();
	ICP20100_Initialization();
	MS5611_Initialization();
	M9N_Initialization();
	//  EEPROM_Initialization();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{

		/*--------------------------------------------------------------------------------------------*/
		/* DEBUG -------------------------------------------------------------------------------------*/
		/*--------------------------------------------------------------------------------------------*/
//		LL_GPIO_TogglePin(DEBUG_LED1_CP_GPIO_Port,DEBUG_LED1_CP_Pin);
//		LL_GPIO_IsInputPinSet(GPIOx, PinMask);
//		HAL_Delay(1000);
//		printf("%d %f\n",count++,f+=0.001 );

		/*--------------------------------------------------------------------------------------------*/
		/* ICM45686 IMU ------------------------------------------------------------------------------*/
		/*--------------------------------------------------------------------------------------------*/
		if(ICM45686_DataReady() == 1)
		{
			ICM45686_Get6AxisRawData(&ICM45686.accel_x_raw , &ICM45686.gyro_x_raw);
			//		  ICM45686_Get3AxisGyroRawData(&ICM45686.accel_x_raw);
			//		  ICM45686_Get3AxisAccRawData(&ICM45686.gyro_x_raw);
			ICM45686_TempRawData(&ICM45686.temperature_raw);
			ICM45686_Calculate_Accel_Gyro_Temp(&ICM45686, 16.f , 2000.f);	// ALREADY HAS FORMULA AND RAW DATA

			ICM45686.accel_x = ICM45686.accel_x_raw * 16.f / 32768.f;
			ICM45686.accel_y = ICM45686.accel_y_raw * 16.f / 32768.f;
			ICM45686.accel_z = ICM45686.accel_z_raw * 16.f / 32768.f;
			ICM45686.gyro_x  = ICM45686.gyro_x_raw  * 2000.f / 32768.f;
			ICM45686.gyro_y  = ICM45686.gyro_y_raw  * 2000.f / 32768.f;
			ICM45686.gyro_z  = ICM45686.gyro_z_raw  * 2000.f / 32768.f;
			ICM45686.temperature = (ICM45686.temperature_raw / 132.48f)+25.f;

#if DEBUG_NO == 0//1
			printf("%d\t%d\t%d\n", ICM45686.accel_x_raw, ICM45686.accel_y_raw, ICM45686.accel_z_raw);
			printf("%f\t%f\t%f\n", (float)(ICM45686.accel_x), (float)(ICM45686.accel_y), (float)(ICM45686.accel_z));
			printf("%d\t%d\t%d\n", ICM45686.gyro_x_raw, ICM45686.gyro_y_raw, ICM45686.gyro_z_raw);
			printf("%f\t%f\t%f\n", (float)(ICM45686.gyro_x), (float)(ICM45686.gyro_y), (float)(ICM45686.gyro_z));
#endif
		}

		/*--------------------------------------------------------------------------------------------*/
		/* ICM42688P IMU -----------------------------------------------------------------------------*/
		/*--------------------------------------------------------------------------------------------*/
		if(ICM42688P_DataReady() == 1)
		{
			ICM42688P_Get6AxisRawData(&ICM42688P.accel_x_raw , &ICM42688P.gyro_x_raw);
			//		  ICM42688P_Get3AxisGyroRawData(&ICM42688P.accel_x_raw);
			//		  ICM42688P_Get3AxisAccRawData(&ICM42688P.gyro_x_raw);
			ICM42688P_TempRawData(&ICM42688P.temperature_raw);
			ICM42688P_Calculate_Accel_Gyro_Temp(&ICM42688P, 16.f , 2000.f);  // ALREADY HAS FORMULA AND RAW DATA

			ICM42688P.temperature = (ICM42688P.temperature_raw / 132.48f)+25.f;
			ICM42688P.accel_x = ICM42688P.accel_x_raw * 16.f / 32768.f;
			ICM42688P.accel_y = ICM42688P.accel_y_raw * 16.f / 32768.f;
			ICM42688P.accel_z = ICM42688P.accel_z_raw * 16.f / 32768.f;
			ICM42688P.gyro_x  = ICM42688P.gyro_x_raw  * 2000.f / 32768.f;
			ICM42688P.gyro_y  = ICM42688P.gyro_y_raw  * 2000.f / 32768.f;
			ICM42688P.gyro_z  = ICM42688P.gyro_z_raw  * 2000.f / 32768.f;

#if DEBUG_NO == 0//2
			printf("%d\t%d\t%d\n", ICM42688P.accel_x_raw, ICM42688P.accel_y_raw, ICM42688P.accel_z_raw);
			printf("%f\t%f\t%f\n", (float)(ICM42688P.accel_x), (float)(ICM42688P.accel_y), (float)(ICM42688P.accel_z));
			printf("%d\t%d\t%d\n", ICM42688P.gyro_x_raw, ICM42688P.gyro_y_raw, ICM42688P.gyro_z_raw);
			printf("%f\t%f\t%f\n", (float)(ICM42688P.gyro_x), (float)(ICM42688P.gyro_y), (float)(ICM42688P.gyro_z));
#endif
		}

		/*--------------------------------------------------------------------------------------------*/
		/* BMI088 ACCEL ------------------------------------------------------------------------------*/
		/*--------------------------------------------------------------------------------------------*/
		if(BMI088_DataReady_ACCEL() == 1)
		{
			BMI088_Get3AxisAccelRawData(&BMI088.accel_x_raw);
			BMI088_TempRawData(&BMI088.temperature_raw);
			BMI088_Calculate_Accel_Temp(&BMI088, 24.f);	// ALREADY HAS FORMULA AND RAW DATA

			BMI088.accel_x = BMI088.accel_x_raw * 24.f / 32768.f;
			BMI088.accel_y = BMI088.accel_y_raw * 24.f / 32768.f;
			BMI088.accel_z = BMI088.accel_z_raw * 24.f / 32768.f;
			BMI088.temperature = (BMI088.temperature_raw / 132.48f)+25.f;

#if DEBUG_NO == 0//3
			printf("%d\t%d\t%d\n", BMI088.accel_x_raw, BMI088.accel_y_raw, BMI088.accel_z_raw);
			printf("%f\t%f\t%f\n", (float)(BMI088.accel_x), (float)(BMI088.accel_y), (float)(BMI088.accel_z));
#endif
		}

		/*--------------------------------------------------------------------------------------------*/
		/* BMI088 GYRO -------------------------------------------------------------------------------*/
		/*--------------------------------------------------------------------------------------------*/
		if(BMI088_DataReady_GYRO() == 1)
		{
			BMI088_Get3AxisGyroRawData(&BMI088.gyro_x_raw);
			BMI088_Calculate_Gyro(&BMI088, 2000.f);	// ALREADY HAS FORMULA AND RAW DATA

			BMI088.gyro_x = BMI088.gyro_x_raw * 2000.f / 32768.f;
			BMI088.gyro_y = BMI088.gyro_y_raw * 2000.f / 32768.f;
			BMI088.gyro_z = BMI088.gyro_z_raw * 2000.f / 32768.f;

#if DEBUG_NO == 0//4
			printf("%d\t%d\t%d\n", BMI088.gyro_x_raw, BMI088.gyro_y_raw, BMI088.gyro_z_raw);
			printf("%f\t%f\t%f\n", (float)(BMI088.gyro_x), (float)(BMI088.gyro_y), (float)(BMI088.gyro_z));
#endif
		}

		/*--------------------------------------------------------------------------------------------*/
		/* ICP20100 BARO -----------------------------------------------------------------------------*/
		/*--------------------------------------------------------------------------------------------*/
		if(ICP20100_DataReady() == 1)
		{
			ICP20100_Get_Raw_Pressure_And_Temp(&ICP20100.raw_pressure , &ICP20100.raw_temperature);
			//		  ICP20100_Get_Raw_Pressure(&ICP20100.raw_pressure);
			//		  ICP20100_Get_Raw_Temperature(&ICP20100.raw_temperature);
			//		  ICP20100_Get_Altitude(&ICP20100);					// ALREADY HAS FORMULA AND RAW DATA
			//		  ICP20100_Get_Altitude_Temp_Corrected(&ICP20100);	// ALREADY HAS FORMULA AND RAW DATA

			// FORMULA TO CONVERT RAW DATA TO MEANINGFUL DATA
			ICP20100.pressure    = (float)(ICP20100.raw_pressure    >> 17 )*40.f+70.f;
			ICP20100.temperature = (float)(ICP20100.raw_temperature >> 18 )*65.f+25.f;

#define BARO_FILTER1 0.90f // MOVE TO TOP
			ICP20100.filtered_altitude = (double)(ICP20100.filtered_altitude * BARO_FILTER1 + ICP20100.altitude * (1.0f - BARO_FILTER1));

#if DEBUG_NO == 0//5
			printf("%ld\t%ld\n", ICP20100.raw_pressure , ICP20100.raw_temperature);
			printf("%f\t%f\n", (float)(ICP20100.pressure) , (float)(ICP20100.temperature));
#endif
		}

		/*--------------------------------------------------------------------------------------------*/
		/* MS5611 BARO -------------------------------------------------------------------------------*/
		/*--------------------------------------------------------------------------------------------*/
		MS5611_Get_Raw_Pressure(&MS5611.raw_pressure);
		MS5611_Get_Raw_Temperature(&MS5611.raw_temperature);

		MS5611_Calculate_Temperature(&MS5611);			// ALREADY HAS RAW DATA FUNCTION
		MS5611_Calculate_Temperature_Compensated_Pressure(&MS5611);	// ALREADY HAS RAW DATA FUNCTION
		MS5611_Get_Altitude(&MS5611);					// NEED PRESSURE CALCULATION FIRST

#define BARO_FILTER2 0.90f // MOVE TO TOP
		MS5611.filtered_altitude = (double)(MS5611.filtered_altitude * BARO_FILTER2 + MS5611.altitude * (1.0f - BARO_FILTER2));


#if DEBUG_NO == 0//6
		printf("%lu\t%lu\n", MS5611.raw_pressure , MS5611.raw_temperature);
		printf("%f\t%f\t%f\n", (float)(MS5611.pressure), (float)(MS5611.temperature), (float)(MS5611.altitude));
#endif

		/*--------------------------------------------------------------------------------------------*/
		/* GPS UBX -----------------------------------------------------------------------------------*/
		/*--------------------------------------------------------------------------------------------*/
		if(gps_ubx_rx_complete_flag == 1)
		{
			gps_ubx_rx_complete_flag = 0;
			if(UBX_Check_CHKSUM(&gps_ubx_rx_buffer[0], 36) == 1)
			{
				UBX_NAV_POSLLH_Parsing(&gps_ubx_rx_buffer[0], &gps_posllh);
#if DEBUG_NO == 0//7
				printf("LAT: %d\tLON: %d\tHeight: %d\n",
						gps_posllh.lat, gps_posllh.lon, gps_posllh.height);
#endif
			}
		}



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_PWR_ConfigSupply(LL_PWR_LDO_SUPPLY);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  while (LL_PWR_IsActiveFlag_VOS() == 0)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_HSI48_Enable();

   /* Wait till HSI48 is ready */
  while(LL_RCC_HSI48_IsReady() != 1)
  {

  }
  LL_RCC_HSE_EnableCSS();
  LL_RCC_PLL_SetSource(LL_RCC_PLLSOURCE_HSE);
  LL_RCC_PLL1P_Enable();
  LL_RCC_PLL1R_Enable();
  LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_8_16);
  LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
  LL_RCC_PLL1_SetM(2);
  LL_RCC_PLL1_SetN(64);
  LL_RCC_PLL1_SetP(2);
  LL_RCC_PLL1_SetQ(2);
  LL_RCC_PLL1_SetR(2);
  LL_RCC_PLL1_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL1_IsReady() != 1)
  {
  }

   /* Intermediate AHB prescaler 2 when target frequency clock is higher than 80 MHz */
   LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
  {

  }
  LL_RCC_SetSysPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAHBPrescaler(LL_RCC_AHB_DIV_2);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_2);
  LL_RCC_SetAPB4Prescaler(LL_RCC_APB4_DIV_2);
  LL_SetSystemCoreClock(400000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
  LL_RCC_HSE_EnableCSS();
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  LL_RCC_PLL3P_Enable();
  LL_RCC_PLL3_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_8_16);
  LL_RCC_PLL3_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
  LL_RCC_PLL3_SetM(2);
  LL_RCC_PLL3_SetN(40);
  LL_RCC_PLL3_SetP(16);
  LL_RCC_PLL3_SetQ(2);
  LL_RCC_PLL3_SetR(2);
  LL_RCC_PLL3_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL3_IsReady() != 1)
  {
  }

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{

  /* Disables the MPU */
  LL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  LL_MPU_ConfigRegion(LL_MPU_REGION_NUMBER0, 0x87, 0x0, LL_MPU_REGION_SIZE_4GB|LL_MPU_TEX_LEVEL0|LL_MPU_REGION_NO_ACCESS|LL_MPU_INSTRUCTION_ACCESS_DISABLE|LL_MPU_ACCESS_SHAREABLE|LL_MPU_ACCESS_NOT_CACHEABLE|LL_MPU_ACCESS_NOT_BUFFERABLE);
  /* Enables the MPU */
  LL_MPU_Enable(LL_MPU_CTRL_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
