/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    stm32h7xx_it.c
 * @brief   Interrupt Service Routines.
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
#include "stm32h7xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define NMEA_PROTO  0
#define UBX_PROTO   1

#define IBUS_PROTO 	1
#define CRSF_PROTO  0
#define SBUS_PROTO  0

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define UBX_SYNC_CHAR_1 				0xB5	// Every Message starts with 2 Bytes: 0xB5 0x62
#define UBX_SYNC_CHAR_2 				0x62

#define IBUS_PROTOCOL_LENGTH 			0x20 	// Length of packet
#define IBUS_PROTOCOL_COMMAND40			0x40	// Command to set servo or motor speed is always 0x40
#define CRSF_ADDRESS_FLIGHT_CONTROLLER 	0xC8
#define CRSF_FRAME_LENGTH 				24 		// length of type + payload + crc
#define SBUS_HEADER 					0x0F

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t usart2_rx_flag = 0;					// DEBUG
uint8_t usart2_rx_data = 0;					// DEBUG

//uint8_t usart3_rx_flag;						// Uart CP-MP
//uint8_t usart3_rx_data;						// Uart CP-MP

uint8_t uart4_rx_flag = 0;					// RECEIVER
uint8_t uart4_rx_data = 0;					// RECEIVER

uint8_t usart6_rx_flag = 0;					// GPS1
uint8_t usart6_rx_data = 0;					// GPS1

uint8_t gps_ubx_rx_buffer[36];				// GPS1 BUFFER
uint8_t gps_ubx_rx_complete_flag = 0;		// GPS1 COMPLETE FLAG

uint8_t ibus_rx_buffer[32];					// FLYSKY IBUS BUFFER
uint8_t ibus_rx_complete_flag = 0;			// FLYSKY IBUS COMPLETE FLAG
uint8_t crsf_rx_buffer[26];					// CRSF BUFFER
uint8_t crsf_rx_complete_flag = 0;			// CRSF COMPLETE FLAG
uint8_t sbus_rx_buffer[25];					// SBUS BUFFER
uint8_t sbus_rx_complete_flag = 0;			// SBUS COMPLETE FLAG

uint8_t tim7_1ms_flag = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
	while (1)
	{
	}
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
	/*--------------------------------------------------------------------------------------------*/
	/* DEBUG -------------------------------------------------------------------------------------*/
	/*--------------------------------------------------------------------------------------------*/
	if(LL_USART_IsActiveFlag_RXNE(USART2))
	{
		usart2_rx_data = LL_USART_ReceiveData8(USART2);
		usart2_rx_flag = 1;

		while(!LL_USART_IsActiveFlag_TXE(USART2));
		LL_USART_TransmitData8(USART6, usart2_rx_data);		// Transmit TO GPS

	}

  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */

  /* USER CODE END USART3_IRQn 0 */
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
  /* USER CODE BEGIN UART4_IRQn 0 */
	/*--------------------------------------------------------------------------------------------*/
	/* RECEIVER ----------------------------------------------------------------------------------*/
	/*--------------------------------------------------------------------------------------------*/
	static unsigned char cnt = 0;

	if(LL_USART_IsActiveFlag_RXNE(UART4))
	{
		//		LL_USART_ClearFlag_RXNE(USART6);					// FUNCTION NOT AVAILABLE

		uart4_rx_data = LL_USART_ReceiveData8(UART4);
		uart4_rx_flag = 1;

		//		while(!LL_USART_IsActiveFlag_TXE(USART3));
		//		LL_USART_TransmitData8(USART3, uart4_rx_data);		// Transmit TO PC

#if IBUS_PROTO
		switch(cnt)
		{
		case 0 :	if(uart4_rx_data == IBUS_PROTOCOL_LENGTH)
		{ibus_rx_buffer[cnt++] = uart4_rx_data;}			break;

		case 1 :	if(uart4_rx_data == IBUS_PROTOCOL_COMMAND40)
		{ibus_rx_buffer[cnt++] = uart4_rx_data;}
		else {cnt = 0;}										break;

		case 31:	ibus_rx_buffer[cnt]    = uart4_rx_data;
		cnt=0;
		ibus_rx_complete_flag  = 1;							break;

		default:	ibus_rx_buffer[cnt++]  = uart4_rx_data;			break;
		}
#endif

#if CRSF_PROTO
		switch(cnt)
		{
		case 0 :	if(uart4_rx_data == CRSF_ADDRESS_FLIGHT_CONTROLLER)
		{crsf_rx_buffer[cnt++] = uart4_rx_data;}					break;

		case 1 :	if(uart4_rx_data == CRSF_FRAME_LENGTH)
		{crsf_rx_buffer[cnt++] = uart4_rx_data;}
		else {cnt = 0;}												break;

		case 25:	crsf_rx_buffer[cnt]    = uart4_rx_data;
		cnt=0;
		crsf_rx_complete_flag  = 1;									break;

		default:	crsf_rx_buffer[cnt++]  = uart4_rx_data;			break;
		}
#endif

#if SBUS_PROTO
		switch(cnt)
		{
		case 0 :	if(uart4_rx_data == SBUS_HEADER)
		{sbus_rx_buffer[cnt++] = uart4_rx_data;}					break;

		case 24:	sbus_rx_buffer[cnt]    = uart4_rx_data;
		cnt=0;
		sbus_rx_complete_flag  = 1;									break;

		default:	sbus_rx_buffer[cnt++]  = uart4_rx_data;			break;
		}
#endif

	}

  /* USER CODE END UART4_IRQn 0 */
  /* USER CODE BEGIN UART4_IRQn 1 */

  /* USER CODE END UART4_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */
	static unsigned char tim7_1ms_count = 0;
	if(LL_TIM_IsActiveFlag_UPDATE(TIM7))
	{
		LL_TIM_ClearFlag_UPDATE(TIM7);

		tim7_1ms_count++;
		if(tim7_1ms_count == 1)
		{
			tim7_1ms_count = 0;
			tim7_1ms_flag = 1;
		}
	}

  /* USER CODE END TIM7_IRQn 0 */
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

/**
  * @brief This function handles USART6 global interrupt.
  */
void USART6_IRQHandler(void)
{
  /* USER CODE BEGIN USART6_IRQn 0 */

	/*--------------------------------------------------------------------------------------------*/
	/* GPS ---------------------------------------------------------------------------------------*/
	/*--------------------------------------------------------------------------------------------*/
	static unsigned char cnt = 0;

	if(LL_USART_IsActiveFlag_RXNE(USART6))
	{
		usart6_rx_data = LL_USART_ReceiveData8(USART6);
		usart6_rx_flag = 1;

		//		while(!LL_USART_IsActiveFlag_TXE(USART2));
		//		LL_USART_TransmitData8(USART2, usart6_rx_data);		// Transmit TO DEBUG USART

#if UBX_PROTO
		switch(cnt)
		{
		case 0 :	if(usart6_rx_data == UBX_SYNC_CHAR_1)
		{gps_ubx_rx_buffer[cnt++] = usart6_rx_data;}		break;

		case 1 :	if(usart6_rx_data == UBX_SYNC_CHAR_2)
		{gps_ubx_rx_buffer[cnt++] = usart6_rx_data;}
		else {cnt = 0;}										break;

		case 35:	gps_ubx_rx_buffer[cnt]    = usart6_rx_data;
		cnt=0;
		gps_ubx_rx_complete_flag  = 1;						break;

		default:	gps_ubx_rx_buffer[cnt++]  = usart6_rx_data;			break;
		}
#endif

	}

  /* USER CODE END USART6_IRQn 0 */
  /* USER CODE BEGIN USART6_IRQn 1 */

  /* USER CODE END USART6_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
