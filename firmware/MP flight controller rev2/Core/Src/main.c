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
#include "dma.h"
#include "i2c.h"
#include "memorymap.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "IBUS.h"
#include "CRSF.h"
#include "SBUS.h"
#include "MYFUNCTIONS.h"
#include "DSHOT.h"
//#include "EEPROM.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

int _write(int file, char* p, int len)
{
	for(int i=0;i<len;i++)
	{
		while(!LL_USART_IsActiveFlag_TXE(USART1));
		LL_USART_TransmitData8(USART1, *(p+i));
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
extern uint8_t usart1_rx_flag;					// DEBUG
extern uint8_t usart1_rx_data;					// DEBUG

extern uint8_t ibus_rx_buffer[32];				// FLYSKY IBUS BUFFER
extern uint8_t ibus_rx_complete_flag;			// FLYSKY IBUS COMPLETE FLAG
extern uint8_t crsf_rx_buffer[26];				// CRSF BUFFER
extern uint8_t crsf_rx_complete_flag;			// CRSF COMPLETE FLAG
extern uint8_t sbus_rx_buffer[25];				// SBUS BUFFER
extern uint8_t sbus_rx_complete_flag;			// SBUS COMPLETE FLAG

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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_SPI3_Init();
  MX_SPI6_Init();
  MX_TIM1_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_TIM8_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_USB_Init();
  /* USER CODE BEGIN 2 */
	  /* BUZZER TIMER ------------------------------------------------------------------------------*/
  LL_TIM_EnableCounter(TIM12);


  /* ENABLE INTERRUPT --------------------------------------------------------------------------*/
  /* THIS ENABLE INTERRUPT IRQ_HANDLER WHEN DATA IS RECEIVED */
  LL_USART_EnableIT_RXNE(USART1);		// DEBUG
  LL_USART_EnableIT_RXNE(USART2);		// EXTERNAL UART
  LL_USART_EnableIT_RXNE(USART3);		// MP-CP

  /* INITIALIZATION ----------------------------------------------------------------------------*/
  //	IBUS_USART6_Initialization();
  //	CRSF_USART6_Initialization();
  //	SBUS_USART6_Initialization();
  Dshot_Init(TIM8 ,DSHOT150, timer8);
//  EEPROM_Initialization();

  /* CHECK DATA RECEIVED -----------------------------------------------------------------------*/
  while(Is_Data_Received() == 0)
  {buzzer_Data_Not_Received();}

  /* CALIBRATION -------------------------------------------------------------------------------*/
  /* Only for analog protocol ; Dshot does not need calibration --------------------------------*/
  if(iBus.switch_A == 2000)
  {
	  buzzer_Calibration_Started();
	  Calibrate_ESC(TIM1,ONESHOT125);
//		  Calibrate_ESC(TIM8,ONESHOT125);// dshot is set to tim8
	  Calibrate_ESC(TIM5,MULTISHOT);
	  Calibrate_ESC(TIM4,ONESHOT42);
	  while(iBus.switch_A != 1000)
	  {
		  Is_Data_Received();
		  buzzer_Calibration_Done();
	  }
  }

  /* SAFETY ------------------------------------------------------------------------------------*/
  while(Is_Throttle_Min() == 0)
  {buzzer_Throttel_Safety();}

  buzzer_All_Set();


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		/*--------------------------------------------------------------------------------------------*/
		/* DEBUG -------------------------------------------------------------------------------------*/
		/*--------------------------------------------------------------------------------------------*/
//		LL_GPIO_TogglePin();
//		LL_GPIO_IsInputPinSet(GPIOx, PinMask);
//		HAL_Delay(1000);
//		printf("%d %f\n",count++,f+=0.001 );

		/*--------------------------------------------------------------------------------------------*/
		/* IBUS PROTOCOL -----------------------------------------------------------------------------*/
		/*--------------------------------------------------------------------------------------------*/
		if(ibus_rx_complete_flag == 1)
		{
			ibus_rx_complete_flag = 0;
			if(IBUS_Check_CHKSUM(&ibus_rx_buffer[0], IBUS_PACKET_SIZE) == 1)
			{
				IBUS_Parsing(&ibus_rx_buffer[0], &iBus);
				if(IBUS_isActiveFailsafe(&iBus)==1)
				{			// FAILSAFE CODE HERE

				}
				else
				{

				}
#if DEBUG_NO == 0//9
				printf("%d\t%d\t%d\t%d\t%d\t%d\n",
						iBus.channel_1, iBus.channel_2,
						iBus.channel_3, iBus.channel_4,
						iBus.switch_A , iBus.switch_B);
				HAL_Delay(30);
#endif
			}
		}

		/*--------------------------------------------------------------------------------------------*/
		/* CRSF PROTOCOL -----------------------------------------------------------------------------*/
		/*--------------------------------------------------------------------------------------------*/
		if(crsf_rx_complete_flag == 1)
		{
			crsf_rx_complete_flag = 0;
			if(CRSF_Check_crc8(&crsf_rx_buffer[2], CRSF_FRAME_LENGTH - 1) == 1)
			{
				CRSF_Parsing(&crsf_rx_buffer[0], &CRSF);
				if(CRSF_isActiveFailsafe(&CRSF)==1)
				{			// FAILSAFE CODE HERE

				}
				else
				{

				}

#if DEBUG_NO == 0//10
				printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t"
						"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
						CRSF.channel[0] ,CRSF.channel[1] ,CRSF.channel[2] ,CRSF.channel[3],
						CRSF.channel[4] ,CRSF.channel[5] ,CRSF.channel[6] ,CRSF.channel[7],
						CRSF.channel[8] ,CRSF.channel[9] ,CRSF.channel[10],CRSF.channel[11],
						CRSF.channel[12],CRSF.channel[13],CRSF.channel[14],CRSF.channel[15]);
				HAL_Delay(30);
#endif
			}
		}

		/*--------------------------------------------------------------------------------------------*/
		/* SBUS PROTOCOL -----------------------------------------------------------------------------*/
		/*--------------------------------------------------------------------------------------------*/
		if(sbus_rx_complete_flag == 1)
		{
			sbus_rx_complete_flag = 0;
			SBUS_Parsing(&sbus_rx_buffer[0], &SBUS);
			if(SBUS_isActiveFailsafe(&SBUS)==1)
			{			// FAILSAFE CODE HERE

			}
			else
			{

			}

#if DEBUG_NO == 0//11
			printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t"
					"%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t"
					"%d\t%d\t%d\t%d\t%d\n",
					SBUS.channel[0] ,SBUS.channel[1] ,SBUS.channel[2] ,SBUS.channel[3],
					SBUS.channel[4] ,SBUS.channel[5] ,SBUS.channel[6] ,SBUS.channel[7],
					SBUS.channel[8] ,SBUS.channel[9] ,SBUS.channel[10],SBUS.channel[11],
					SBUS.channel[12],SBUS.channel[13],SBUS.channel[14],SBUS.channel[15],
					SBUS.channel_17,SBUS.channel_18,SBUS.frame_lost,SBUS.failsafe_activated,
					SBUS.footer);
			HAL_Delay(30);
#endif
		}


		/*--------------------------------------------------------------------------------------------*/
		/* PWM  PROTOCOL -----------------------------------------------------------------------------*/
		/*--------------------------------------------------------------------------------------------*/
#if DEBUG_NO == 0//12

#define TIMx 			TIM1				// TIM1 , TIM2 , TIM3 , TIM4
#define ccr_max 		oneshot125_max_ccr 	// oneshot125_max_ccr, oneshot42_max_ccr, multishot_max_ccr
#define ccr_min 		oneshot125_min_ccr 	// oneshot125_min_ccr, oneshot42_min_ccr, multishot_min_ccr
#define channel_max 	IBUS_CHANNEL_MAX	// IBUS_CHANNEL_MAX  , CRSF_CHANNEL_MAX , SBUS_CHANNEL_MAX
#define channel_min 	IBUS_CHANNEL_MIN	// IBUS_CHANNEL_MIN  , CRSF_CHANNEL_MIN , SBUS_CHANNEL_MIN
#define TIMx_channel_1 	iBus.channel_1		// iBus.channel_1    , CRSF.channel[0] 	, SBUS.channel[0]
#define TIMx_channel_2 	iBus.channel_2		// iBus.channel_2    , CRSF.channel[1]  , SBUS.channel[1]
#define TIMx_channel_3 	iBus.channel_3		// iBus.channel_3    , CRSF.channel[2]  , SBUS.channel[2]
#define TIMx_channel_4 	iBus.channel_4		// iBus.channel_4    , CRSF.channel[3]  , SBUS.channel[3]

		TIMx->CCR1 = ccr_min + (ccr_max - ccr_min)*(TIMx_channel_1 - channel_min)/(channel_max-channel_min);
		TIMx->CCR2 = ccr_min + (ccr_max - ccr_min)*(TIMx_channel_2 - channel_min)/(channel_max-channel_min);
		TIMx->CCR3 = ccr_min + (ccr_max - ccr_min)*(TIMx_channel_3 - channel_min)/(channel_max-channel_min);
		TIMx->CCR4 = ccr_min + (ccr_max - ccr_min)*(TIMx_channel_4 - channel_min)/(channel_max-channel_min);

#endif


		/*--------------------------------------------------------------------------------------------*/
		/* DSHOT PROTOCOL ----------------------------------------------------------------------------*/
		/*--------------------------------------------------------------------------------------------*/
		/* FOR Timer 8 -------------------------------------------------------------------------------*/
#if DEBUG_NO == 0//13

#define TIMx_channel_1 	iBus.channel_1		// iBus.channel_1    , CRSF.channel[0] 	, SBUS.channel[0]
#define TIMx_channel_2 	iBus.channel_2		// iBus.channel_2    , CRSF.channel[1]  , SBUS.channel[1]
#define TIMx_channel_3 	iBus.channel_3		// iBus.channel_3    , CRSF.channel[2]  , SBUS.channel[2]
#define TIMx_channel_4 	iBus.channel_4		// iBus.channel_4    , CRSF.channel[3]  , SBUS.channel[3]
		uint16_t motor_value[motor_count]={iBus.channel_1,iBus.channel_2,
				iBus.channel_3,iBus.channel_4};
		Dshot_Write(motor_value, timer8);
#endif











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
