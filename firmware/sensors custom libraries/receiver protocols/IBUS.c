/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		ibus.c
 * @brief   	:		This file includes the functions for IBUS Protocol
 * ______________________________________________________________________________________________________
 */

#include <IBUS.h>

Struct_IBUS iBus;

unsigned char IBUS_Check_CHKSUM(unsigned char* data, unsigned char len)
{
	unsigned short crc = 0xFFFF;

	for(int i=0;i<len-2;i++)
	{
		crc = crc - data[i];
	}

	return ((crc&0x00FF) == data[30]) && ((crc >> 8) == data[31]);
}

void IBUS_Parsing(unsigned char* data, Struct_IBUS* iBus)
{
	iBus->channel_1 = (unsigned short)( data[3]  << 8 | data[2]  ) & 0x0FFF;
	iBus->channel_2 = (unsigned short)( data[5]  << 8 | data[4]  ) & 0x0FFF;
	iBus->channel_3 = (unsigned short)( data[7]  << 8 | data[6]  ) & 0x0FFF;
	iBus->channel_4 = (unsigned short)( data[9]  << 8 | data[8]  ) & 0x0FFF;
	iBus->switch_A  = (unsigned short)( data[11] << 8 | data[10] ) & 0x0FFF;
	iBus->switch_B  = (unsigned short)( data[13] << 8 | data[12] ) & 0x0FFF;
//	iBus->switch_C  = (unsigned short)( data[15] << 8 | data[14] ) & 0x0FFF;	// USE IN CASE OF FS-i6X
//	iBus->switch_D  = (unsigned short)( data[17] << 8 | data[16] ) & 0x0FFF;
//	iBus->VrA       = (unsigned short)( data[19] << 8 | data[18] ) & 0x0FFF;
//	iBus->VrB       = (unsigned short)( data[21] << 8 | data[20] ) & 0x0FFF;

	iBus->failsafe  = (unsigned char)( data[13] >> 4 );
//	iBus->failsafe  = (unsigned char)( iBus->switch_D == 1500 );				// USE IN CASE OF FS-i6X
}

unsigned char IBUS_isActiveFailsafe(Struct_IBUS* iBus)
{
	return iBus->failsafe != 0;
}

void IBUS_USART6_Initialization(void)
{
	LL_USART_InitTypeDef USART_InitStruct = {0};

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

	/** Initializes the peripherals clock
	 */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART6;
	PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/* Peripheral clock enable */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART6);

	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOG);
	/**USART6 GPIO Configuration
	  PG9   ------> USART6_RX
	  PG14   ------> USART6_TX
	 */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_9|LL_GPIO_PIN_14;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
	LL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/* USART6 interrupt Init */
	NVIC_SetPriority(USART6_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	NVIC_EnableIRQ(USART6_IRQn);

	USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
	USART_InitStruct.BaudRate = 115200;
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_RX;
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	LL_USART_Init(USART6, &USART_InitStruct);
	LL_USART_SetTXFIFOThreshold(USART6, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_SetRXFIFOThreshold(USART6, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_DisableFIFO(USART6);
	LL_USART_ConfigAsyncMode(USART6);

	LL_USART_Enable(USART6);
}
