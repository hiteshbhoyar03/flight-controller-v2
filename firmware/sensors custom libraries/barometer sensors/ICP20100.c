/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		icp20100.c
 * @brief   	:		This file includes the LL driver functions for ICP20100 BARO
 * ______________________________________________________________________________________________________
 */

#include "ICP20100.h"
#include <math.h>

Struct_ICP20100 ICP20100;

/**
 * @brief  Initialize GPIO pins for SPI communication
 * @retval No return value
 */
void ICP20100_SPI_GPIO_Initialization(void)
{
	LL_SPI_InitTypeDef SPI_InitStruct = {0};

	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Peripheral clock enable */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI4);

	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOE);
	/**SPI4 GPIO Configuration
	 PE2   ------> SPI4_SCK
	PE5   ------> SPI4_MISO
	PE6   ------> SPI4_MOSI
	 */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_2|LL_GPIO_PIN_5|LL_GPIO_PIN_6;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
	LL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
	SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
	SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
	SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;
	SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
	SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
	SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV8;
	SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
	SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
	SPI_InitStruct.CRCPoly = 0x0;
	LL_SPI_Init(SPI4, &SPI_InitStruct);
	LL_SPI_SetStandard(SPI4, LL_SPI_PROTOCOL_MOTOROLA);
	LL_SPI_SetFIFOThreshold(SPI4, LL_SPI_FIFO_TH_01DATA);
	LL_SPI_EnableNSSPulseMgt(SPI4);
	/**ICP20100 GPIO Control Configuration
	 * PE2  ------> ICP20100_SPI_nCS_PIN (output)
	 * PG8  ------> ICP20100_INT_PIN (input)
	 */
	LL_GPIO_ResetOutputPin(ICP20100_SPI_nCS_PORT, ICP20100_SPI_nCS_PIN);

	GPIO_InitStruct.Pin = ICP20100_SPI_nCS_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(ICP20100_SPI_nCS_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ICP20100_INT_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(ICP20100_INT_PORT, &GPIO_InitStruct);

	LL_SPI_Enable(ICP20100_SPI_CHANNEL);

	ICP20100_CHIP_DESELECT(ICP20100);
}

/**
 * @brief  Send and receive the data via SPI LL Drivers
 * @retval Received data
 * @param  data			Data to send
 */
static unsigned char IPC20100_Transmit_Receive(unsigned char data)
{
	LL_SPI_TransmitData8(ICP20100_SPI_CHANNEL, data);
	while(LL_SPI_IsActiveFlag_TXC(ICP20100_SPI_CHANNEL)==RESET);

	while(LL_SPI_IsActiveFlag_RXWNE(ICP20100_SPI_CHANNEL)==RESET);
	return LL_SPI_ReceiveData8(ICP20100_SPI_CHANNEL);
}

/**
 * @brief  Read 1 byte of given register
 * @retval Received data
 * @param  reg_addr		Register address to start read from
 */
uint8_t ICP20100_Readbyte(uint8_t reg_addr)
{
	uint8_t val;

	ICP20100_CHIP_SELECT(ICP20100);
	IPC20100_Transmit_Receive(reg_addr | 0x80); 				//Register. MSB 1 is read instruction.
	val = IPC20100_Transmit_Receive(0x00); 					//Send DUMMY to read data
	ICP20100_CHIP_DESELECT(ICP20100);

	return val;
}

/**
 * @brief  Read n bytes starting from given register address
 * @retval No return value
 * @param  reg_addr		Register address to start read from
 * @param  len			Number of byte to read
 * @param  data			Pointer to data
 */
void ICP20100_Read_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data)
{
	unsigned int i = 0;

	ICP20100_CHIP_SELECT(ICP20100);
	IPC20100_Transmit_Receive(reg_addr | 0x80);				//Register. MSB 1 is read instruction.
	while(i < len)
	{
		data[i++] = IPC20100_Transmit_Receive(0x00);			//Send DUMMY to read data
	}
	ICP20100_CHIP_DESELECT(ICP20100);
}

/**
 * @brief  Write 1 bytes in given register address
 * @retval No return value
 * @param  reg_addr		Register address to write
 * @param  val			Value to be written
 */
void ICP20100_Writebyte(uint8_t reg_addr, uint8_t val)
{
	ICP20100_CHIP_SELECT(ICP20100);
	IPC20100_Transmit_Receive(reg_addr & 0x7F); 				//Register. MSB 0 is write instruction.
	IPC20100_Transmit_Receive(val); 							//Send Data to write
	ICP20100_CHIP_DESELECT(ICP20100);
}

/**
 * @brief  Write n bytes starting from given register address
 * @retval No return value
 * @param  reg_addr		Register address to start write
 * @param  len			Number of byte to read
 * @param  data			Pointer of data to be written
 */
void ICP20100_Write_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data)
{
	unsigned int i = 0;
	ICP20100_CHIP_SELECT(ICP20100);
	IPC20100_Transmit_Receive(reg_addr & 0x7F); 				//Register. MSB 0 is write instruction.
	while(i < len)
	{
		IPC20100_Transmit_Receive(data[i++]); 					//Send Data to write
	}
	ICP20100_CHIP_DESELECT(ICP20100);
}

/**
 * @brief  Initialize the device in reset form
 * @retval No return value
 */
int ICP20100_Initialization(void)
{
	ICP20100_SPI_GPIO_Initialization();
	/*
	uint8_t id = 0;
	printf("Checking ICP20100...");
	id = ICP20100_Readbyte(ICP20100_REG_DEVICE_ID);
	if(id == ICP20100_ID)
	{
		printf("\nICP20100 who_am_i = 0x%02x...OK\n\n", id);
	}
	else if(id != ICP20100_ID)
	{
		who_am_i = ICP20100_Readbyte(ICP20100_REG_DEVICE_ID);
		if (id != ICP20100_ID){
			printf( "ICP20100 Not OK: 0x%02x Should be 0x%02x\n", id, ICP20100_ID);
			return 1;
		}
	}
	 */
	// ICP20100_Writebyte(	, 0x	);			//SOFT RESET
	// HAL_Delay(50);

	return 0;
}

/**
 * @brief  Get raw pressure temperature data
 * @retval No return value
 * @param  pressure			Pointer to store pressure data
 * @param  temperature		Pointer to store temperature data
 */
void ICP20100_Get_Raw_Pressure_And_Temp(int32_t* pressure, int32_t* temperature)
{
	/*LSB FIRST BOTH*/
	ICP20100_Read_Buffer(ICP20100_REG_PRESS_DATA_0, 3, (unsigned char*)pressure);
	ICP20100_Read_Buffer(ICP20100_REG_TEMP_DATA_0, 3, (unsigned char*)temperature);

	/*OR*/

	//	unsigned char data[6];
	//
	//	ICP20100_Read_Buffer(ICP20100_REG_PRESS_DATA_0, 6, data);
	//	pressure[0] = (data[2] << 16) | (data[1] << 8) | data[0];
	//	temperature[0]  = (data[5] << 16) | (data[4] << 8) | data[3];
	////	pressure[0] = (int32_t)(((int32_t)(data[2] << 16)) | ((int32_t)(data[1] << 8)) | data[0]);
	////	temperature[0]  = (int32_t)(((int32_t)(data[5] << 16)) | ((int32_t)(data[3] << 8)) | data[3]);

}

/**
 * @brief  Get raw pressure temperature data
 * @retval No return value
 * @param  pressure			Pointer to store pressure data
 */
void ICP20100_Get_Raw_Pressure(int32_t* pressure)
{
	/*LSB FIRST*/
	ICP20100_Read_Buffer(ICP20100_REG_PRESS_DATA_0, 3, (unsigned char*)pressure);

	/*OR*/

	//	 unsigned char data[3];
	//
	//	ICP20100_Read_Buffer(ICP20100_REG_PRESS_DATA_0, 3, data);
	//	pressure[0] = (data[2] << 16) | (data[1] << 8) | data[0];
	////	pressure[0] = (int32_t)(((int32_t)(data[2] << 16)) | ((int32_t)(data[1] << 8)) | data[0]);

}

/**
 * @brief  Get raw pressure temperature data
 * @retval No return value
 * @param  temperature		Pointer to store temperature data
 */
void ICP20100_Get_Raw_Temperature(int32_t* 	temperature)
{
	/*LSB FIRST*/
	ICP20100_Read_Buffer(ICP20100_REG_TEMP_DATA_0, 3, (unsigned char*)	temperature);

	/*OR*/

	//	unsigned char data[3];
	//
	//	ICP20100_Read_Buffer(ICP20100_REG_TEMP_DATA_0, 3, data);
	//	temperature[0]  = (data[2] << 16) | (data[1] << 8) | data[0];
	////	temperature[0] = (int32_t)(((int32_t)(data[2] << 16)) | ((int32_t)(data[1] << 8)) | data[0]);

}

#define SEA_LEVEL_PRESSURE				1013.25f
//#define X 0.90f

/**
 * @brief  Get raw pressure temperature data
 * @retval No return value
 * @param  ICP20100			Pointer to Structure
 */
void ICP20100_Get_Altitude(Struct_ICP20100* ICP20100) //No temperature correction.
{
	ICP20100_Get_Raw_Pressure(&ICP20100->raw_pressure);
	ICP20100->pressure    = (float)(  ICP20100->raw_pressure  >> 17 )*40.f+70.f;
	ICP20100->altitude    = (powf((SEA_LEVEL_PRESSURE / ICP20100->pressure), 0.1902226f) - 1.0) * 44307.69396f; //145366.45f * 0.3048f = 44307.69396f;
	//	ICP20100->filtered_altitude = (double)(ICP20100->filtered_altitude * X + ICP20100->altitude * (1.0f - X));
}

/**
 * @brief  Get raw pressure temperature data
 * @retval No return value
 * @param  ICP20100			Pointer to Structure
 */
void ICP20100_Get_Altitude_Temp_Corrected(Struct_ICP20100* ICP20100) //Get Altitude with temperature correction.
{
	ICP20100_Get_Raw_Pressure_And_Temp(&ICP20100->raw_pressure , &ICP20100->raw_temperature);
	//	ICP20100_Get_Raw_Pressure(&ICP20100->raw_pressure);
	//	ICP20100_Get_Raw_Temperature(&ICP20100->raw_temperature);
	ICP20100->pressure    = (float)(ICP20100->raw_pressure    >> 17 )*40.f+70.f;
	ICP20100->temperature = (float)(ICP20100->raw_temperature >> 18 )*65.f+25.f;
	ICP20100->altitude    = (double)((powf((SEA_LEVEL_PRESSURE / ICP20100->pressure), 0.1902226f) - 1.0f) * (ICP20100->temperature + 273.15f)) / 0.0065f;
	//	ICP20100->filtered_altitude = (double)(ICP20100->filtered_altitude * X + ICP20100->altitude * (1.0f - X));
}

/**
 * @brief  Check if chip data is ready
 * @retval Ready = 1, Not ready = 0
 */
int ICP20100_DataReady(void)
{
	return LL_GPIO_IsInputPinSet(ICP20100_INT_PORT, ICP20100_INT_PIN);
}
