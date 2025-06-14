/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		dshot.c
 * @brief   	:		This file includes the LL driver functions for DShOT Protocol
 * ______________________________________________________________________________________________________
 */

#include "dshot.h"

/* Variables -----------------------------------------------------------------*/
static uint32_t all_motor_dmabuffer[4][motor_count][DSHOT_DMA_BUFFER_SIZE];
// static uint32_t motor_dmabuffer[DSHOT_DMA_BUFFER_SIZE];

/* Static functions ----------------------------------------------------------*/
/* dshot init */
static uint32_t dshot_choose_type(enum_dshot_type dshot_type);
static void dshot_set_timer(TIM_TypeDef * TIMx , enum_dshot_type dshot_type);
static void TIM1_DMA_Configuration(void);
static void TIM8_DMA_Configuration(void);
static void TIM5_DMA_Configuration(void);
// static void TIM4_DMA_Configuration(void);
static void dshot_start_pwm(TIM_TypeDef * TIMx);

/* dshot write */
static uint16_t dshot_prepare_packet(uint16_t value);
static void dshot_prepare_dmabuffer_all(uint16_t* motor_value_buff,enum_timer_number timer);

/**
 * @brief  Initializes DSHOT Protocol
 * @param  dshot_type	Dshot type
 * @retval No return value
 */
void Dshot_Init(TIM_TypeDef * TIMx ,enum_dshot_type dshot_type, enum_timer_number timer)
{
	dshot_set_timer(TIMx,dshot_type);

	switch (timer)
	{
	//	case(timer4):		TIM4_DMA_Configuration();		break;
	case(timer5):		TIM5_DMA_Configuration();		break;
	case(timer8):		TIM8_DMA_Configuration();		break;
	default:	
	case(timer1):		TIM1_DMA_Configuration();		break;
	}

	dshot_start_pwm(TIMx);
}

/**
 * @brief  dshot send data to esc
 * @param  motor_value_buff	Pointer to throttle value buffer
 * @param  timer			Timer enum for buffer
 * @retval No return value
 */
void Dshot_Write(uint16_t* motor_value_buff, enum_timer_number timer)
{
	switch (timer)
	{
	//	case timer4:
	//		LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_4);	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_5);
	//		LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_6);	/*LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_7);*/
	//		break;
	case timer5:
		LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_0);	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_1);
		LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_2);	LL_DMA_DisableStream(DMA2, LL_DMA_STREAM_3);
		break;
	case timer8:
		LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_4);	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_5);
		LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_6);	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_7);
		break;
	default:	
	case timer1:
		LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_0);	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_1);
		LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_2);	LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_3);
		break;
	}

	dshot_prepare_dmabuffer_all(motor_value_buff,timer);
	//	TIM1_DMA_Configuration(); // NOT REQUIRED PROBABBLY
	//	TIM8_DMA_Configuration();
	//	TIM5_DMA_Configuration();
	/* TIM4_DMA_Configuration(); */
	switch (timer)
	{
	//	case timer4:
	//		LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_4);	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_5);
	//		LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_6);	/*LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_7);*/
	//		break;
	case timer5:
		LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_1);
		LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_3);
		break;
	case timer8:
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_4);	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_5);
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_6);	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_7);
		break;
	default:	
	case timer1:
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);
		LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_2);	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_3);
		break;
	}

	//	LL_DMA_ClearFlag_DME5(DMAx);//all
	//	LL_DMA_ClearFlag_FE5(DMAx);
	//	LL_DMA_ClearFlag_HT5(DMAx);
	//	LL_DMA_ClearFlag_TC5(DMAx);
	//	LL_DMA_ClearFlag_TE5(DMAx);

}

/* Static functions ----------------------------------------------------------*/
/**
 * @brief  choose dshot protocol
 * @param  dshot_type		Dshot protocol type enum
 * @retval dshot frequency
 */
static uint32_t dshot_choose_type(enum_dshot_type dshot_type)
{
	switch (dshot_type)
	{
	case(DSHOT2400):		return DSHOT2400_HZ;
	case(DSHOT1200):		return DSHOT1200_HZ;
	case(DSHOT600):			return DSHOT600_HZ;
	case(DSHOT300):			return DSHOT300_HZ;
	default:case(DSHOT150):	return DSHOT150_HZ;
	}
}

/**
 * @brief  Configure timer for dshot 
 * @param  TIMx			Timer number
 * @param  dshot_type	Dshot protocol type enum
 * @retval No return value
 */
static void dshot_set_timer(TIM_TypeDef * TIMx , enum_dshot_type dshot_type)
{
	uint16_t dshot_prescaler;
	uint32_t timer_clock = TIMER_CLOCK;
	dshot_prescaler = lrintf((float) timer_clock / dshot_choose_type(dshot_type) + 0.01f) - 1;	// Calculate prescaler by dshot type
	TIMx->PSC = dshot_prescaler;
	TIMx->ARR = MOTOR_BITLENGTH;
}

/**
 * @brief  Configure DMA parameter 
 * @retval No return value
 */
static void TIM1_DMA_Configuration(void)
{
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_0, (uint32_t)all_motor_dmabuffer[0][0], (uint32_t)&TIM1->CCR1, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_1, (uint32_t)all_motor_dmabuffer[0][1], (uint32_t)&TIM1->CCR2, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_2, (uint32_t)all_motor_dmabuffer[0][2], (uint32_t)&TIM1->CCR3, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_3, (uint32_t)all_motor_dmabuffer[0][3], (uint32_t)&TIM1->CCR4, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, DSHOT_DMA_BUFFER_SIZE);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_1, DSHOT_DMA_BUFFER_SIZE);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_2, DSHOT_DMA_BUFFER_SIZE);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_3, DSHOT_DMA_BUFFER_SIZE);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_2);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_3);
}

/**
 * @brief  Configure DMA parameter 
 * @retval No return value
 */
static void TIM8_DMA_Configuration(void)
{
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_4, (uint32_t)all_motor_dmabuffer[1][0], (uint32_t)&TIM8->CCR1, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_5, (uint32_t)all_motor_dmabuffer[1][1], (uint32_t)&TIM8->CCR2, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_6, (uint32_t)all_motor_dmabuffer[1][2], (uint32_t)&TIM8->CCR3, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_7, (uint32_t)all_motor_dmabuffer[1][3], (uint32_t)&TIM8->CCR4, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_4, DSHOT_DMA_BUFFER_SIZE);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_5, DSHOT_DMA_BUFFER_SIZE);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_6, DSHOT_DMA_BUFFER_SIZE);
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_7, DSHOT_DMA_BUFFER_SIZE);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_4);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_5);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_6);
	LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_7);
}

/**
 * @brief  Configure DMA parameter 
 * @retval No return value
 */
static void TIM5_DMA_Configuration(void)
{
	LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_0, (uint32_t)all_motor_dmabuffer[2][0], (uint32_t)&TIM5->CCR1, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_1, (uint32_t)all_motor_dmabuffer[2][1], (uint32_t)&TIM5->CCR2, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_2, (uint32_t)all_motor_dmabuffer[2][2], (uint32_t)&TIM5->CCR3, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_3, (uint32_t)all_motor_dmabuffer[2][3], (uint32_t)&TIM5->CCR4, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_0, DSHOT_DMA_BUFFER_SIZE);
	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_1, DSHOT_DMA_BUFFER_SIZE);
	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_2, DSHOT_DMA_BUFFER_SIZE);
	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_3, DSHOT_DMA_BUFFER_SIZE);
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_1);
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_2);
	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_3);
}

/**
 * @brief  Configure DMA parameter 
 * @retval No return value
 */
//static void TIM4_DMA_Configuration(void)
//{
//	LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_4, (uint32_t)all_motor_dmabuffer[3][0], (uint32_t)&TIM4->CCR1, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
//	LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_5, (uint32_t)all_motor_dmabuffer[3][1], (uint32_t)&TIM4->CCR2, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
//	LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_6, (uint32_t)all_motor_dmabuffer[3][2], (uint32_t)&TIM4->CCR3, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
//	/* LL_DMA_ConfigAddresses(DMA2, LL_DMA_STREAM_7, (uint32_t)all_motor_dmabuffer[3][3], (uint32_t)&TIM4->CCR4, LL_DMA_DIRECTION_MEMORY_TO_PERIPH); */
//	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_4, DSHOT_DMA_BUFFER_SIZE);
//	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_5, DSHOT_DMA_BUFFER_SIZE);
//	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_6, DSHOT_DMA_BUFFER_SIZE);
//	/* LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_7, DSHOT_DMA_BUFFER_SIZE); */
//	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_4);
//	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_5);
//	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_6);
//	/* LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_7); */
//}

/**
 * @brief  Start Timer pwm 
 * @param  TIMx			Timer number
 * @retval No return value
 */
static void dshot_start_pwm(TIM_TypeDef * TIMx)
{
	//	Enabling/disabling DMA request can restart a new cycle without PWM start/stop.
	LL_TIM_CC_EnableChannel(TIMx , LL_TIM_CHANNEL_CH1);
	LL_TIM_CC_EnableChannel(TIMx , LL_TIM_CHANNEL_CH2);
	LL_TIM_CC_EnableChannel(TIMx , LL_TIM_CHANNEL_CH3);
	LL_TIM_CC_EnableChannel(TIMx , LL_TIM_CHANNEL_CH4);
}

/**
 * @brief  Make packet using throttlr value
 * @param  value		Throttle valeu
 * @retval No return value
 */
static uint16_t dshot_prepare_packet(uint16_t value)
{
	uint16_t packet;
	uint8_t dshot_telemetry = 0;

	packet = (value << 1) | (dshot_telemetry ? 1 : 0);

	unsigned checksum = 0;
	unsigned chksum_data = packet;

	for(int i = 0; i < 3; i++)
	{
		checksum ^=  chksum_data; // xor data by nibbles
		chksum_data >>= 4;
	}

	checksum &= 0xf;
	packet = (packet << 4) | checksum;

	return packet;
}

// Convert 16 bits packet to 16 pwm signal
/**
 * @brief  Prepare dma buffer
 * @param  motor_value_buff	Pointer to throttle value buffer
 * @param  timer			Timer enum for buffer
 * @retval No return value
 */
static void dshot_prepare_dmabuffer_all(uint16_t* motor_value_buff,enum_timer_number timer)
{
	for (int channel = 0; channel < motor_count; channel++)
	{
		uint16_t packet;
		packet = dshot_prepare_packet(motor_value_buff[channel]);

		for(int bit_index = 0; bit_index < 16; bit_index++)
		{
			all_motor_dmabuffer[timer][channel][bit_index] = (packet & 0x8000) ? MOTOR_BIT_1 : MOTOR_BIT_0;
			packet <<= 1;
		}

		all_motor_dmabuffer[timer][channel][16] = 0;
		all_motor_dmabuffer[timer][channel][17] = 0;
	}
}
