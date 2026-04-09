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
#include "DSHOT.h"
//#include "EEPROM.h"
#include "DRONE_CONFIG.h"

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

// SELECT ANY ONE PARAMETER
#define ONESHOT125 1
#define ONESHOT42  0
#define MULTISHOT  0
#define DSHOT      0

#define DSHOT_TYPE 150 // or DSHOT150

// TAKES 1 MINUTE FOR CABILBRATION // can be done only once
#define CALIBRATE_ESC      0

#define timer_freq 200000000

// CCR Values for MAX and MIN Duty Cycle
#define oneshot125_max_ccr  ((timer_freq * 0.000250f)/2)	// 250 µs Pulse Width // 25000
#define oneshot125_min_ccr  ((timer_freq * 0.000125f)/2)	// 125 µs Pulse Width // 12500
#define oneshot42_max_ccr   (timer_freq * 0.000084f)		// 84  µs Pulse Width // 16800
#define oneshot42_min_ccr   (timer_freq * 0.000042f)		// 42  µs Pulse Width // 8400
#define multishot_max_ccr   (timer_freq * 0.000025f)		// 25  µs Pulse Width // 5000
#define multishot_min_ccr   (timer_freq * 0.000005f)		// 5   µs Pulse Width // 1000

// ARR Values for MAX and MIN Update-Rate
#define oneshot125_max_updateRate_arr  (timer_freq / (4000*2))-1	// 4  KHz Update Rate // 25000
#define oneshot125_min_updateRate_arr  (timer_freq / (2000*2))-1	// 2  KHz Update Rate // 50000
#define oneshot42_max_updateRate_arr   (timer_freq / (20000))-1		// 20 KHz Update Rate // 10000
#define oneshot42_min_updateRate_arr   (timer_freq / (8000))-1		// 8  KHz Update Rate // 25000
#define multishot_max_updateRate_arr   (timer_freq / (50000))-1		// 50 KHz Update Rate // 4000
#define multishot_min_updateRate_arr   (timer_freq / (25000))-1		// 25 KHz Update Rate // 8000

// SELECT UPDATE RATE
#define oneshot125_updateRate_arr   	oneshot125_min_updateRate_arr
#define oneshot42_updateRate_arr   		oneshot42_min_updateRate_arr
#define multishot_updateRate_arr   		multishot_min_updateRate_arr

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

extern uint8_t usart1_rx_flag;					// DEBUG
extern uint8_t usart1_rx_data;					// DEBUG

extern uint8_t cp_2_mp_rx_buffer[32];			// CP-MP TX BUFFER
extern uint8_t cp_2_mp_rx_complete_flag;		// CP-MP

extern uint8_t tim7_1000Hz_flag;
extern uint8_t tim7_2000Hz_flag;
extern uint8_t tim7_4000Hz_flag;

typedef struct _RX_DATA
{
	unsigned short setpoint_roll;
	unsigned short setpoint_pitch;
	unsigned short setpoint_throttle;
	unsigned short setpoint_yaw;
	float roll_pid;
	float pitch_pid;
	float yaw_rate_pid;
	float yaw_heading_pid;
	unsigned char failsafe_flag;
	unsigned char motor_arming_flag;
	unsigned char a;
	unsigned char b;
}Struct_RXDATA;

Struct_RXDATA rx;

typedef enum
{
	oneshot125 = 125,			// Values are bluff; has no meaning
	oneshot42  = 42,
	multishot  = 5
} Analog_ESC_Protocol;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

int Is_CP_MP_Data_Received(void);
int Is_Throttle_Min(void);
unsigned char CP_MP_Check_CHKSUM(unsigned char* data, unsigned char length);
void CP_MP_Parsing(unsigned char* data, Struct_RXDATA* rx);
unsigned char CP_MP_isActiveFailsafe(Struct_RXDATA* rx);
void Calibrate_ESC(TIM_TypeDef * timer, Analog_ESC_Protocol protocol);

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
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */

	/* ENABLE INTERRUPT --------------------------------------------------------------------------*/
	/* THIS ENABLE INTERRUPT IRQ_HANDLER WHEN DATA IS RECEIVED */
	LL_USART_EnableIT_RXNE(USART1);		// DEBUG
	LL_USART_EnableIT_RXNE(USART2);		// EXTERNAL UART
	LL_USART_EnableIT_RXNE(USART3);		// MP-CP

	/* INITIALIZATION ----------------------------------------------------------------------------*/
	//  EEPROM_Initialization();

	/* CHECK DATA RECEIVED -----------------------------------------------------------------------*/
	while(Is_CP_MP_Data_Received() == 0);
	/* SAFETY ------------------------------------------------------------------------------------*/
	while(Is_Throttle_Min() == 0);

	LL_TIM_EnableCounter(TIM7); //10Hz, 50Hz, 1kHz loop
	LL_TIM_EnableIT_UPDATE(TIM7);

	uint32_t motor[16]={0};

	uint32_t max_ccr;
	uint32_t min_ccr;
	uint32_t motor_offset;
	uint32_t throttle_multiplier;

#if DSHOT
	Enable_Dshot_Channels(DSHOT_TYPE);
	max_ccr = 2047;
	min_ccr = 0;
	motor_offset = (max_ccr - min_ccr)/20;
	throttle_multiplier = (max_ccr - min_ccr)/1000;

#elif MULTISHOT
	max_ccr = multishot_max_ccr;
	min_ccr = multishot_min_ccr;
	motor_offset = (max_ccr - min_ccr)/20;
	throttle_multiplier = (max_ccr - min_ccr)/100;
	Pwm_Init(multishot, multishot_updateRate_arr);
#if(CALIBRATE_ESC)
	Calibrate_ESC(TIM1, multishot);
	Calibrate_ESC(TIM8, multishot);
	Calibrate_ESC(TIM5, multishot);
	Calibrate_ESC(TIM4, multishot);
#endif

#elif ONESHOT42
	max_ccr = oneshot42_max_ccr;
	min_ccr = oneshot42_min_ccr;
	motor_offset = (max_ccr - min_ccr)/20;
	throttle_multiplier = (max_ccr - min_ccr)/100;
	Pwm_Init(oneshot42, oneshot42_updateRate_arr);
#if(CALIBRATE_ESC)
	Calibrate_ESC(TIM1, oneshot42);
	Calibrate_ESC(TIM8, oneshot42);
	Calibrate_ESC(TIM5, oneshot42);
	Calibrate_ESC(TIM4, oneshot42);
#endif

#else  // ONESHOT125
	max_ccr = oneshot125_max_ccr;
	min_ccr = oneshot125_min_ccr;
	motor_offset = (max_ccr - min_ccr)/20;
	throttle_multiplier = (max_ccr - min_ccr)/100;
	Pwm_Init(oneshot125, oneshot125_updateRate_arr);
#if(CALIBRATE_ESC)
	Calibrate_ESC(TIM1, oneshot125);
	Calibrate_ESC(TIM8, oneshot125);
	Calibrate_ESC(TIM5, oneshot125);
	Calibrate_ESC(TIM4, oneshot125);
#endif
#endif


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
		/* cp-mp UART PROTOCOL -----------------------------------------------------------------------------*/
		if(cp_2_mp_rx_complete_flag == 1)
		{
			cp_2_mp_rx_complete_flag = 0;
			if(CP_MP_Check_CHKSUM(&cp_2_mp_rx_buffer[0], sizeof(cp_2_mp_rx_buffer)-2) == 1)
			{
				CP_MP_Parsing(&cp_2_mp_rx_buffer[0], &rx);


				if(CP_MP_isActiveFailsafe(&rx)==1)
				{			// FAILSAFE CODE HERE
				}
				else
				{

				}
			}
		}

		if(tim7_1000Hz_flag){
			tim7_1000Hz_flag =0;
			if(rx.setpoint_yaw < 1485 || rx.setpoint_yaw > 1515)
			{

				Calculate_Motor_Values(&motor[0],
						rx.setpoint_throttle,
						rx.roll_pid,
						rx.pitch_pid,
						rx.yaw_rate_pid,
						min_ccr, motor_offset, throttle_multiplier);
			}
			else
			{
				Calculate_Motor_Values(&motor[0],
						rx.setpoint_throttle,
						rx.roll_pid,
						rx.pitch_pid,
						rx.yaw_heading_pid,
						min_ccr, motor_offset, throttle_multiplier);
			}
		}

#if DSHOT
		if(rx.motor_arming_flag == 1)
		{
			if(rx.failsafe_flag == 0)
			{
				if(rx.setpoint_throttle > 1030)
				{
					Dshot_Set(motor, max_ccr, min_ccr, motor_offset);
				}
				else
				{
					Dshot_Reset(min_ccr, motor_offset);
				}
			}
			else
			{
				Dshot_Reset(min_ccr, 0);
			}
		}
		else
		{
			Dshot_Reset(min_ccr, 0);
		}
#else
		if(rx.motor_arming_flag == 1)
		{
			if(rx.failsafe_flag == 0)
			{
				if(rx.setpoint_throttle > 1030)
				{
					Pwm_Set(motor, max_ccr, min_ccr, motor_offset);
				}
				else
				{
					Pwm_Reset(min_ccr, motor_offset);
				}
			}
			else
			{
				Pwm_Reset(min_ccr, 0);
			}
		}
		else
		{
			Pwm_Reset(min_ccr, 0);
		}
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

/**
 * @brief Check if Data is received from CP
 * @retval 1 if received
 */
int Is_CP_MP_Data_Received(void)
{
	if(cp_2_mp_rx_complete_flag == 1)
	{
		cp_2_mp_rx_complete_flag = 0;
		if(CP_MP_Check_CHKSUM(&cp_2_mp_rx_buffer[0], sizeof(cp_2_mp_rx_buffer)) == 1)
		{
			CP_MP_Parsing(&cp_2_mp_rx_buffer[0], &rx);
			return 1;
		}
	}
	return 0;
}
/**
 * @brief Check if Throttle is minimun in Data received from CP
 * @retval 1	if throttle is min
 */
int Is_Throttle_Min(void)
{
	if(cp_2_mp_rx_complete_flag == 1)
	{
		cp_2_mp_rx_complete_flag = 0;
		if(CP_MP_Check_CHKSUM(&cp_2_mp_rx_buffer[0], sizeof(cp_2_mp_rx_buffer)) == 1)
		{
			CP_MP_Parsing(&cp_2_mp_rx_buffer[0], &rx);
			if(rx.setpoint_throttle < 1010) return 1;
		}
	}
	return 0;
}
/**
 * @brief Check if Data received from CP is correct
 * @param  data		Pointer to data received
 * @param  length	Length of data received minus 2 i.e. (sizeof(cp_2_mp_rx_buffer)-2)
 * @retval 1 		if Checksum Matches
 */
unsigned char CP_MP_Check_CHKSUM(unsigned char* data, unsigned char length)
{
	unsigned short crc = 0xFFFF;			// Initial value
	unsigned short polynomial = 0x1021;

	for (uint32_t i = 0; i < length; i++) {
		crc ^= (uint16_t)data[i] << 8;
		for (uint8_t j = 0; j < 8; j++) {
			if (crc & 0x8000)
				crc = (crc << 1) ^ polynomial;
			else
				crc <<= 1;
		}
	}
	return ((crc&0x00FF) == data[length-2]) && ((crc >> 8) == data[length-1]);
}
/**
 * @brief Parse the Data received from CP
 * @param  data		Pointer to data received
 * @param  rx		Pointer to store received data
 * @retval None
 */
void CP_MP_Parsing(unsigned char* data, Struct_RXDATA* rx)
{
	rx->setpoint_roll     = ((((uint16_t)data[ 3]) << 8) | data[2]);
	rx->setpoint_pitch    = ((((uint16_t)data[ 5]) << 8) | data[4]);
	rx->setpoint_throttle = ((((uint16_t)data[ 7]) << 8) | data[6]);
	rx->setpoint_yaw      = ((((uint16_t)data[ 9]) << 8) | data[8]);
	rx->roll_pid          = (float)((((uint32_t)data[13]) << 24) | (((uint32_t)data[12]) << 16) | (((uint32_t)data[11]) << 8) | data[10]);
	rx->pitch_pid         = (float)((((uint32_t)data[17]) << 24) | (((uint32_t)data[16]) << 16) | (((uint32_t)data[15]) << 8) | data[14]);
	rx->yaw_rate_pid      = (float)((((uint32_t)data[21]) << 24) | (((uint32_t)data[20]) << 16) | (((uint32_t)data[19]) << 8) | data[18]);
	rx->yaw_heading_pid   = (float)((((uint32_t)data[25]) << 24) | (((uint32_t)data[24]) << 16) | (((uint32_t)data[23]) << 8) | data[22]);
	rx->failsafe_flag     = data[26];
	rx->motor_arming_flag = data[27];
	rx->a     = data[28];
	rx->b     = data[29];
}
/**
 * @brief Check if failsafe is active in Data received from CP
 * @param  rx		Pointer to stored data
 * @retval value of rx->failsafe_flag
 */
unsigned char CP_MP_isActiveFailsafe(Struct_RXDATA* rx)
{
	return rx->failsafe_flag != 0;
}
/**
 * @brief Calibrate ESC
 * @param  timer		Pointer to timer
 * @param  protocol		Analog_ESC_Protocol
 * @retval None
 */
void Calibrate_ESC(TIM_TypeDef * timer, Analog_ESC_Protocol protocol)
{
	LL_TIM_EnableCounter(timer);
	LL_TIM_CC_EnableChannel(timer , LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(timer , LL_TIM_CHANNEL_CH2);
	LL_TIM_CC_EnableChannel(timer , LL_TIM_CHANNEL_CH3);
	LL_TIM_CC_EnableChannel(timer , LL_TIM_CHANNEL_CH4);

	switch (protocol)
	{
	case multishot :
		timer->PSC = 0;
		timer->ARR = multishot_updateRate_arr;

		timer->CCR1 = multishot_max_ccr;		timer->CCR2 = multishot_max_ccr;
		timer->CCR3 = multishot_max_ccr;		timer->CCR4 = multishot_max_ccr;
		HAL_Delay(7000);
		timer->CCR1 = multishot_min_ccr;		timer->CCR2 = multishot_min_ccr;
		timer->CCR3 = multishot_min_ccr;		timer->CCR4 = multishot_min_ccr;
		HAL_Delay(8000);
		break;
	case oneshot42 :
		timer->PSC = 0;
		timer->ARR = oneshot42_updateRate_arr;

		timer->CCR1 = oneshot42_max_ccr;		timer->CCR2 = oneshot42_max_ccr;
		timer->CCR3 = oneshot42_max_ccr;		timer->CCR4 = oneshot42_max_ccr;
		HAL_Delay(7000);
		timer->CCR1 = oneshot42_min_ccr;		timer->CCR2 = oneshot42_min_ccr;
		timer->CCR3 = oneshot42_min_ccr;		timer->CCR4 = oneshot42_min_ccr;
		HAL_Delay(8000);
		break;
	default:
	case oneshot125 :
		timer->PSC = 1;
		timer->ARR = oneshot125_updateRate_arr;

		timer->CCR1 = oneshot125_max_ccr;		timer->CCR2 = oneshot125_max_ccr;
		timer->CCR3 = oneshot125_max_ccr;		timer->CCR4 = oneshot125_max_ccr;
		HAL_Delay(7000);
		timer->CCR1 = oneshot125_min_ccr;		timer->CCR2 = oneshot125_min_ccr;
		timer->CCR3 = oneshot125_min_ccr;		timer->CCR4 = oneshot125_min_ccr;
		HAL_Delay(8000);
		break;
	}
}

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
