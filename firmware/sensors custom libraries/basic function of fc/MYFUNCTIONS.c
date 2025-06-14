/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		myfunctions.c
 * @brief   	:		This file includes the Necessary functions for System Initialization
 * ______________________________________________________________________________________________________
 */

#include "MYFUNCTIONS.h"

//Struct_abc ABC;

extern uint8_t ibus_rx_buffer[32];				// FLYSKY IBUS BUFFER
extern uint8_t ibus_rx_complete_flag;			// FLYSKY IBUS COMPLETE FLAG
extern uint8_t crsf_rx_buffer[26];				// CRSF BUFFER
extern uint8_t crsf_rx_complete_flag;			// CRSF COMPLETE FLAG
extern uint8_t sbus_rx_buffer[25];				// SBUS BUFFER
extern uint8_t sbus_rx_complete_flag;			// SBUS COMPLETE FLAG

/**
  * @brief  Check if flight controller has received any data
  * @retval Received = 1, Not received = 0
  */
int Is_Data_Received(void)
{
#if RECEIVER_PROTOCOL == IBUS_PROTOCOL
	if(ibus_rx_complete_flag == 1)
	{
		ibus_rx_complete_flag = 0;
		if(IBUS_Check_CHKSUM(&ibus_rx_buffer[0], IBUS_PACKET_SIZE) == 1)
		{
			IBUS_Parsing(&ibus_rx_buffer[0], &iBus);
			return 1;
		}
	}
#endif
#if RECEIVER_PROTOCOL == CRSF_PROTOCOL
	if(crsf_rx_complete_flag == 1)
	{
		crsf_rx_complete_flag = 0;
		if(CRSF_Check_crc8(&crsf_rx_buffer[2], CRSF_FRAME_LENGTH - 1) == 1)
		{
			CRSF_Parsing(&crsf_rx_buffer[0], &CRSF);
			return 1;
		}
	}
#endif
#if RECEIVER_PROTOCOL == SBUS_PROTOCOL
	if(sbus_rx_complete_flag == 1)
	{
		sbus_rx_complete_flag = 0;
		SBUS_Parsing(&sbus_rx_buffer[0], &SBUS);
		return 1;
	}
#endif
	return 0;
}

/**
  * @brief  Check if throttle is at minimum position
  * @retval minimum = 1, else = 0
  */
int Is_Throttle_Min(void)
{
#if RECEIVER_PROTOCOL == IBUS_PROTOCOL
	if(ibus_rx_complete_flag == 1)
	{
		ibus_rx_complete_flag = 0;
		if(IBUS_Check_CHKSUM(&ibus_rx_buffer[0], IBUS_PACKET_SIZE) == 1)
		{
			IBUS_Parsing(&ibus_rx_buffer[0], &iBus);
			if(iBus.channel_1 < IBUS_CHANNEL_MIN+10) return 1;
		}
	}
#endif

#if RECEIVER_PROTOCOL == CRSF_PROTOCOL
	if(crsf_rx_complete_flag == 1)
	{
		crsf_rx_complete_flag = 0;
		if(CRSF_Check_crc8(&crsf_rx_buffer[2], CRSF_FRAME_LENGTH - 1) == 1)
		{
			CRSF_Parsing(&crsf_rx_buffer[0], &CRSF);
			if(CRSF.channel[0] < CRSF_CHANNEL_MIN+10) return 1;
		}
	}
#endif

#if RECEIVER_PROTOCOL == SBUS_PROTOCOL
	if(sbus_rx_complete_flag == 1)
	{
		sbus_rx_complete_flag = 0;
		SBUS_Parsing(&sbus_rx_buffer[0], &SBUS);
		if(SBUS.channel[0] < SBUS_CHANNEL_MIN+10) return 1;
	}
#endif

	return 0;
}

/**
  * @brief  Calibrate ESC
  * @retval No return value
  * @param  timer			Pointer to TIM_TypeDef
  * @param  protocol		Analog ESC protocol
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
	case MULTISHOT :
		timer->PSC  = 0;
		timer->ARR=multishot_min_updateRate_arr;

		timer->CCR1 = multishot_max_ccr;		timer->CCR2 = multishot_max_ccr;
		timer->CCR3 = multishot_max_ccr;		timer->CCR4 = multishot_max_ccr;
		HAL_Delay(7000);
		timer->CCR1 = multishot_min_ccr;		timer->CCR2 = multishot_min_ccr;
		timer->CCR3 = multishot_min_ccr;		timer->CCR4 = multishot_min_ccr;
		HAL_Delay(8000);
		break;
	case ONESHOT42 :
		timer->PSC  = 0;
		timer->ARR=oneshot42_min_updateRate_arr;

		timer->CCR1 = oneshot42_max_ccr;		timer->CCR2 = oneshot42_max_ccr;
		timer->CCR3 = oneshot42_max_ccr;		timer->CCR4 = oneshot42_max_ccr;
		HAL_Delay(7000);
		timer->CCR1 = oneshot42_min_ccr;		timer->CCR2 = oneshot42_min_ccr;
		timer->CCR3 = oneshot42_min_ccr;		timer->CCR4 = oneshot42_min_ccr;
		HAL_Delay(8000);
		break;
	default:
	case ONESHOT125 :
		timer->PSC  = 1;
		timer->ARR=oneshot125_min_updateRate_arr;

		timer->CCR1 = oneshot125_max_ccr;		timer->CCR2 = oneshot125_max_ccr;
		timer->CCR3 = oneshot125_max_ccr;		timer->CCR4 = oneshot125_max_ccr;
		HAL_Delay(7000);
		timer->CCR1 = oneshot125_min_ccr;		timer->CCR2 = oneshot125_min_ccr;
		timer->CCR3 = oneshot125_min_ccr;		timer->CCR4 = oneshot125_min_ccr;
		HAL_Delay(8000);
		break;
	}
}

/**
  * @brief  Buzzer codes below
  * @retval No return value
  */
void buzzer_Data_Not_Received(void)
{
	LL_TIM_CC_EnableChannel(TIM12, LL_TIM_CHANNEL_CH1);
	TIM12->PSC = 3000;
	HAL_Delay(200);
	LL_TIM_CC_DisableChannel(TIM12, LL_TIM_CHANNEL_CH1);
	HAL_Delay(200);
}
void buzzer_Calibration_Started(void)
{
	LL_TIM_CC_EnableChannel(TIM12, LL_TIM_CHANNEL_CH1);
	TIM12->PSC = 1500;
	HAL_Delay(200);
	TIM12->PSC = 2000;
	HAL_Delay(200);
	TIM12->PSC = 1500;
	HAL_Delay(200);
	TIM12->PSC = 2000;
	HAL_Delay(200);
	LL_TIM_CC_DisableChannel(TIM12, LL_TIM_CHANNEL_CH1);
}
void buzzer_Calibration_Done(void)
{
	LL_TIM_CC_EnableChannel(TIM12, LL_TIM_CHANNEL_CH1);
	TIM12->PSC = 1500;
	HAL_Delay(200);
	TIM12->PSC = 2000;
	HAL_Delay(200);
	LL_TIM_CC_DisableChannel(TIM12, LL_TIM_CHANNEL_CH1);
}
void buzzer_Throttel_Safety(void)
{
	LL_TIM_CC_EnableChannel(TIM12, LL_TIM_CHANNEL_CH1);
	TIM12->PSC = 1000;
	HAL_Delay(70);
	LL_TIM_CC_DisableChannel(TIM12, LL_TIM_CHANNEL_CH1);
	HAL_Delay(70);
}
void buzzer_All_Set(void)
{
	LL_TIM_CC_EnableChannel(TIM12 , LL_TIM_CHANNEL_CH1);
	TIM12->PSC=2000;
	HAL_Delay(100);
	TIM12->PSC=1500;
	HAL_Delay(100);
	TIM12->PSC=1000;
	HAL_Delay(100);
	LL_TIM_CC_DisableChannel(TIM12 , LL_TIM_CHANNEL_CH1);
}
//void ADC_Init(){
//	LL_DMA_ConfigAddresses(DMA2,LL_DMA_STREAM_4,
//		 						LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
//								(uint32_t)adcVal,
//								LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
//	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_4, 3);
//	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_4);
//
//	LL_ADC_Enable(ADC1);
//
//	LL_ADC_REG_StartConversionSWStart(ADC1);
//
//}
