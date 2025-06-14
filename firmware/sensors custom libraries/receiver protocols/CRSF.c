/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		crsf.c
 * @brief   	:		This file includes the functions for CRSF Protocol
 * ______________________________________________________________________________________________________
 */

#include "CRSF.h"

Struct_CRSF CRSF;

/**
 * @brief  Parse the CRSF buffer
 * @param  data		Pointer to CRSF data buffer
 * @param  len		Length of buffer
 * @retval No return value
 */
unsigned char CRSF_Check_crc8(unsigned char* data, unsigned char len)
{	// Check crc for 23 bytes i.e bit[0] to bit[22]
	static const unsigned char crsf_crc8tab[256] = {
			0x00, 0xD5, 0x7F, 0xAA, 0xFE, 0x2B, 0x81, 0x54, 0x29, 0xFC, 0x56, 0x83, 0xD7, 0x02, 0xA8, 0x7D,
			0x52, 0x87, 0x2D, 0xF8, 0xAC, 0x79, 0xD3, 0x06, 0x7B, 0xAE, 0x04, 0xD1, 0x85, 0x50, 0xFA, 0x2F,
			0xA4, 0x71, 0xDB, 0x0E, 0x5A, 0x8F, 0x25, 0xF0, 0x8D, 0x58, 0xF2, 0x27, 0x73, 0xA6, 0x0C, 0xD9,
			0xF6, 0x23, 0x89, 0x5C, 0x08, 0xDD, 0x77, 0xA2, 0xDF, 0x0A, 0xA0, 0x75, 0x21, 0xF4, 0x5E, 0x8B,
			0x9D, 0x48, 0xE2, 0x37, 0x63, 0xB6, 0x1C, 0xC9, 0xB4, 0x61, 0xCB, 0x1E, 0x4A, 0x9F, 0x35, 0xE0,
			0xCF, 0x1A, 0xB0, 0x65, 0x31, 0xE4, 0x4E, 0x9B, 0xE6, 0x33, 0x99, 0x4C, 0x18, 0xCD, 0x67, 0xB2,
			0x39, 0xEC, 0x46, 0x93, 0xC7, 0x12, 0xB8, 0x6D, 0x10, 0xC5, 0x6F, 0xBA, 0xEE, 0x3B, 0x91, 0x44,
			0x6B, 0xBE, 0x14, 0xC1, 0x95, 0x40, 0xEA, 0x3F, 0x42, 0x97, 0x3D, 0xE8, 0xBC, 0x69, 0xC3, 0x16,
			0xEF, 0x3A, 0x90, 0x45, 0x11, 0xC4, 0x6E, 0xBB, 0xC6, 0x13, 0xB9, 0x6C, 0x38, 0xED, 0x47, 0x92,
			0xBD, 0x68, 0xC2, 0x17, 0x43, 0x96, 0x3C, 0xE9, 0x94, 0x41, 0xEB, 0x3E, 0x6A, 0xBF, 0x15, 0xC0,
			0x4B, 0x9E, 0x34, 0xE1, 0xB5, 0x60, 0xCA, 0x1F, 0x62, 0xB7, 0x1D, 0xC8, 0x9C, 0x49, 0xE3, 0x36,
			0x19, 0xCC, 0x66, 0xB3, 0xE7, 0x32, 0x98, 0x4D, 0x30, 0xE5, 0x4F, 0x9A, 0xCE, 0x1B, 0xB1, 0x64,
			0x72, 0xA7, 0x0D, 0xD8, 0x8C, 0x59, 0xF3, 0x26, 0x5B, 0x8E, 0x24, 0xF1, 0xA5, 0x70, 0xDA, 0x0F,
			0x20, 0xF5, 0x5F, 0x8A, 0xDE, 0x0B, 0xA1, 0x74, 0x09, 0xDC, 0x76, 0xA3, 0xF7, 0x22, 0x88, 0x5D,
			0xD6, 0x03, 0xA9, 0x7C, 0x28, 0xFD, 0x57, 0x82, 0xFF, 0x2A, 0x80, 0x55, 0x01, 0xD4, 0x7E, 0xAB,
			0x84, 0x51, 0xFB, 0x2E, 0x7A, 0xAF, 0x05, 0xD0, 0xAD, 0x78, 0xD2, 0x07, 0x53, 0x86, 0x2C, 0xF9};

	unsigned char crc = 0;
	for (uint8_t i = 0; i < len; i++)
	{
		crc = crsf_crc8tab[crc ^ data[i]];
	}
	return (crc == data[25]);
}

/**
 * @brief  Parse the CRSF buffer
 * @param  crsfData	Pointer to CRSF data buffer
 * @param  CRSF		Pointer to CRSF Structure
 * @retval No return value
 */
void CRSF_Parsing(unsigned char* crsfData, Struct_CRSF* CRSF)
{	// EACH CHANNEL COMPRISES OF 11 BITS
	if (crsfData[1] == CRSF_FRAME_LENGTH){	// 24
		CRSF->channel[0]  = (unsigned short)((crsfData[3]       | crsfData[4]  << 8) & 0x07FF);
		CRSF->channel[1]  = (unsigned short)((crsfData[4]  >> 3 | crsfData[5]  << 5) & 0x07FF);
		CRSF->channel[2]  = (unsigned short)((crsfData[5]  >> 6 | crsfData[6]  << 2  | crsfData[7] <<10)& 0x07FF);
		CRSF->channel[3]  = (unsigned short)((crsfData[7]  >> 1 | crsfData[8]  << 7) & 0x07FF);
		CRSF->channel[4]  = (unsigned short)((crsfData[8]  >> 4 | crsfData[9]  << 4) & 0x07FF);
		CRSF->channel[5]  = (unsigned short)((crsfData[9]  >> 7 | crsfData[10] << 1  | crsfData[11]<<9) & 0x07FF);
		CRSF->channel[6]  = (unsigned short)((crsfData[11] >> 2 | crsfData[12] << 6) & 0x07FF);
		CRSF->channel[7]  = (unsigned short)((crsfData[12] >> 5 | crsfData[13] << 3) & 0x07FF);
		CRSF->channel[8]  = (unsigned short)((crsfData[14]      | crsfData[15] << 8) & 0x07FF);
		CRSF->channel[9]  = (unsigned short)((crsfData[15] >> 3 | crsfData[16] << 5) & 0x07FF);
		CRSF->channel[10] = (unsigned short)((crsfData[16] >> 6 | crsfData[17] << 2  | crsfData[18]<<10)& 0x07FF);
		CRSF->channel[11] = (unsigned short)((crsfData[18] >> 1 | crsfData[19] << 7) & 0x07FF);
		CRSF->channel[12] = (unsigned short)((crsfData[19] >> 4 | crsfData[20] << 4) & 0x07FF);
		CRSF->channel[13] = (unsigned short)((crsfData[20] >> 7 | crsfData[21] << 1  | crsfData[22]<<9) & 0x07FF);
		CRSF->channel[14] = (unsigned short)((crsfData[22] >> 2 | crsfData[23] << 6) & 0x07FF);
		CRSF->channel[15] = (unsigned short)((crsfData[23] >> 5 | crsfData[24] << 3) & 0x07FF);

		//		CRSF->failsafe = 0;
		CRSF->failsafe = 1;
	}
}

/**
 * @brief  Is failsafe active
 * @param  CRSF		Pointer to CRSF Structure
 * @retval Failsafe Active 1 else 0
 */
unsigned char CRSF_isActiveFailsafe(Struct_CRSF* CRSF)
{
	return CRSF->failsafe != 0;
}

/**
 * @brief  Initialize GPIO pins for SPI communication
 * @retval No return value
 */
void CRSF_USART6_Initialization(void)
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
	USART_InitStruct.BaudRate = 420000;
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
