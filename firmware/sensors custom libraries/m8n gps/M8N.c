/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		m8n.c
 * @brief   	:		This file includes the functions for UBX Protocol
 * ______________________________________________________________________________________________________
 */

#include "M8N.h"

#define NMEA 0
#define UBX  1
#define GPS1_PROTOCOL UBX			// SELECT PROTOCOL NMEA IS DEFAULT
#define GPS2_PROTOCOL UBX			// SELECT PROTOCOL NMEA IS DEFAULT

Struct_M8N_UBX_NAV_POSLLH gps1_posllh;
Struct_M8N_UBX_NAV_POSLLH gps2_posllh;

const unsigned char UBX_CFG_PRT[] = {
	0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00,
	0xD0, 0x08, 0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x01, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9A, 0x79
};

const unsigned char UBX_CFG_MSG[] = {
	0xB5, 0x62, 0x06, 0x01, 0x08, 0x00, 0x01, 0x02, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x13, 0xBE
};

const unsigned char UBX_CFG_RATE[] = {
	0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xC8, 0x00, 0x01, 0x00,
	0x01, 0x00, 0xDE, 0x6A
};

const unsigned char UBX_CFG_CFG[] = {
	0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x31,
	0xBF
};

void M8N_TransmitData_USART1(unsigned char* data, unsigned char len)
{
	for(int i=0;i<len;i++)
	{
		while(!LL_USART_IsActiveFlag_TXE(USART1));
		LL_USART_TransmitData8(USART1, *(data+i));
	}
}

void M8N_TransmitData_UART8(unsigned char* data, unsigned char len)
{
	for(int i=0;i<len;i++)
	{
		while(!LL_USART_IsActiveFlag_TXE(UART8));
		LL_USART_TransmitData8(UART8, *(data+i));
	}
}

void UBX_NAV_POSLLH_Parsing(unsigned char* data, Struct_M8N_UBX_NAV_POSLLH* posllh)
{
	posllh->CLASS  = data[2];
	posllh->ID     = data[3];
	posllh->LENGTH = (data[4] | data[5]<<8);

	posllh->iTOW   = (data[9] <<24 | data[8] <<16 | data[7] <<8 | data[6] );
	posllh->lon    = (data[13]<<24 | data[12]<<16 | data[11]<<8 | data[10]);
	posllh->lat    = (data[17]<<24 | data[16]<<16 | data[15]<<8 | data[14]);
	posllh->height = (data[21]<<24 | data[20]<<16 | data[19]<<8 | data[18]);
	posllh->hMSL   = (data[25]<<24 | data[24]<<16 | data[23]<<8 | data[22]);
	posllh->hAcc   = (data[29]<<24 | data[28]<<16 | data[27]<<8 | data[26]);
	posllh->vAcc   = (data[33]<<24 | data[32]<<16 | data[31]<<8 | data[30]);

//	posllh->lon_f64 = posllh->lon / 10000000.;
//	posllh->lat_f64 = posllh->lat / 10000000.;
}

unsigned char UBX_Check_CHKSUM(unsigned char* data, unsigned char len)
{
	unsigned char CK_A = 0, CK_B = 0;

	for(int i=2;i<len-2;i++)
	{
		CK_A = CK_A + data[i];
		CK_B = CK_B + CK_A;
	}

	return ((CK_A == data[len-2]) && (CK_B == data[len-1]));
}

void M8N_Initialization(void)
{
	M8N_USART1_Initialization();
	M8N_UART8_Initialization();
	
#if GPS1_PROTOCOL == UBX
	// UNCOMMENT IF USING NMEA PROTOCOL
	M8N_TransmitData_USART1(&UBX_CFG_PRT[0] , sizeof(UBX_CFG_PRT));
	HAL_Delay(100);
	M8N_TransmitData_USART1(&UBX_CFG_MSG[0] , sizeof(UBX_CFG_MSG));
	HAL_Delay(100);
	M8N_TransmitData_USART1(&UBX_CFG_RATE[0], sizeof(UBX_CFG_RATE));
	HAL_Delay(100);
	M8N_TransmitData_USART1(&UBX_CFG_CFG[0] , sizeof(UBX_CFG_CFG));
	HAL_Delay(100);
#endif


#if GPS1_PROTOCOL == UBX
	// UNCOMMENT IF USING NMEA PROTOCOL
	M8N_TransmitData_UART8(&UBX_CFG_PRT[0] , sizeof(UBX_CFG_PRT));
	HAL_Delay(100);
	M8N_TransmitData_UART8(&UBX_CFG_MSG[0] , sizeof(UBX_CFG_MSG));
	HAL_Delay(100);
	M8N_TransmitData_UART8(&UBX_CFG_RATE[0], sizeof(UBX_CFG_RATE));
	HAL_Delay(100);
	M8N_TransmitData_UART8(&UBX_CFG_CFG[0] , sizeof(UBX_CFG_CFG));
#endif

}

void M8N_USART1_Initialization(void)
{
	LL_USART_InitTypeDef USART_InitStruct = {0};

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

	/** Initializes the peripherals clock
	 */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
	PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
	Error_Handler();
	}

	/* Peripheral clock enable */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOA);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOB);
	/**USART1 GPIO Configuration
	 PA10   ------> USART1_RX
	PB14   ------> USART1_TX
	*/
	GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_7;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_14;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USART1 interrupt Init */
	NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	NVIC_EnableIRQ(USART1_IRQn);

	USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
	USART_InitStruct.BaudRate = 9600;
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	LL_USART_Init(USART1, &USART_InitStruct);
	LL_USART_SetTXFIFOThreshold(USART1, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_SetRXFIFOThreshold(USART1, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_DisableFIFO(USART1);
	LL_USART_ConfigAsyncMode(USART1);

	LL_USART_Enable(USART1);

}

void M8N_UART8_Initialization(void)
{

	LL_USART_InitTypeDef UART_InitStruct = {0};

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

	/** Initializes the peripherals clock
	 */
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART8;
	PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
	Error_Handler();
	}

	/* Peripheral clock enable */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_UART8);

	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOE);
	/**UART8 GPIO Configuration
	 PE1   ------> UART8_TX
	PE0   ------> UART8_RX
	*/
	GPIO_InitStruct.Pin = LL_GPIO_PIN_1|LL_GPIO_PIN_0;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_8;
	LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/* UART8 interrupt Init */
	NVIC_SetPriority(UART8_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
	NVIC_EnableIRQ(UART8_IRQn);

	UART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
	UART_InitStruct.BaudRate = 9600;
	UART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	UART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	UART_InitStruct.Parity = LL_USART_PARITY_NONE;
	UART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	UART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	UART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	LL_USART_Init(UART8, &UART_InitStruct);
	LL_USART_DisableFIFO(UART8);
	LL_USART_SetTXFIFOThreshold(UART8, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_SetRXFIFOThreshold(UART8, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_ConfigAsyncMode(UART8);

	LL_USART_Enable(UART8);

}
