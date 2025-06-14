/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		ms5611.c
 * @brief   	:		This file includes the LL driver functions for MS5611 IMU
 * ______________________________________________________________________________________________________
 */

#include "MS5611.h"
#include <math.h>

Struct_MS5611 MS5611;

/**
 * @brief  Initialize GPIO pins for SPI communication
 * @retval No return value
 */
void MS5611_SPI_GPIO_Initialization(void)
{
	LL_SPI_InitTypeDef SPI_InitStruct = {0};
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Peripheral clock enable */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI5);

	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOK);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOF);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOJ);
	/**SPI5 GPIO Configuration
	PK0   ------> SPI5_SCK
	PF8   ------> SPI5_MISO
	PJ10   ------> SPI5_MOSI
	 */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_0;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
	LL_GPIO_Init(GPIOK, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
	LL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
	LL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

	SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
	SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
	SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
	SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
	SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
	SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
	SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV4;
	SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
	SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
	SPI_InitStruct.CRCPoly = 0x0;
	LL_SPI_Init(SPI5, &SPI_InitStruct);
	LL_SPI_SetStandard(SPI5, LL_SPI_PROTOCOL_MOTOROLA);
	LL_SPI_SetFIFOThreshold(SPI5, LL_SPI_FIFO_TH_01DATA);
	LL_SPI_EnableNSSPulseMgt(SPI5);
	/**MS5611 GPIO Control Configuration
	 * PE13  ------> MS5611_SPI_nCS_PIN (output)
	 */
	LL_GPIO_ResetOutputPin(MS5611_SPI_nCS_PORT, MS5611_SPI_nCS_PIN);

	GPIO_InitStruct.Pin = MS5611_SPI_nCS_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(MS5611_SPI_nCS_PORT, &GPIO_InitStruct);

	LL_SPI_Enable(MS5611_SPI_CHANNEL);

	MS5611_CHIP_DESELECT(MS5611);
}

/**
 * @brief  Send and receive the data via SPI LL Drivers
 * @retval Received data
 * @param  data			Data to send
 */
static unsigned char MS5611_Transmit_Receive(unsigned char data)
{
	LL_SPI_TransmitData8(MS5611_SPI_CHANNEL, data);
	while(LL_SPI_IsActiveFlag_TXC(MS5611_SPI_CHANNEL)==RESET);

	while(LL_SPI_IsActiveFlag_RXWNE(MS5611_SPI_CHANNEL)==RESET);
	return LL_SPI_ReceiveData8(MS5611_SPI_CHANNEL);
}

/**
 * @brief  Read 1 byte of given register
 * @retval Received data
 * @param  reg_addr		Register address to read from
 */
uint8_t MS5611_Readbyte(uint8_t reg_addr)
{
	uint8_t val;

	MS5611_CHIP_SELECT(MS5611);
	MS5611_Transmit_Receive(reg_addr | 0x80);
	val = MS5611_Transmit_Receive(0x00); 					//Send DUMMY to read data
	MS5611_CHIP_DESELECT(MS5611);

	return val;
}

/**
 * @brief  Read n bytes starting from given register address
 * @retval No return value
 * @param  reg_addr		Register address to start read from
 * @param  len			Number of byte to read
 * @param  data			Pointer to data
 */
void MS5611_Read_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data)
{
	unsigned int i = 0;

	MS5611_CHIP_SELECT(MS5611);
	MS5611_Transmit_Receive(reg_addr | 0x80);
	while(i < len)
	{
		data[i++] = MS5611_Transmit_Receive(0x00);			//Send DUMMY to read data
	}
	MS5611_CHIP_DESELECT(MS5611);
}

/**
 * @brief  Write 1 bytes in given register address
 * @retval No return value
 * @param  reg_addr		Register address to write
 * @param  val			Value to be written
 */
void MS5611_Writebyte(uint8_t reg_addr, uint8_t val)
{
	MS5611_CHIP_SELECT(MS5611);
	MS5611_Transmit_Receive(reg_addr & 0x7F);
	MS5611_Transmit_Receive(val);	 						//Send Data to write
	MS5611_CHIP_DESELECT(MS5611);
}

/**
 * @brief  Write n bytes starting from given register address
 * @retval No return value
 * @param  reg_addr		Register address to start write
 * @param  len			Number of byte to read
 * @param  data			Pointer of data to be written
 */
void MS5611_Write_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data)
{
	unsigned int i = 0;
	MS5611_CHIP_SELECT(MS5611);
	MS5611_Transmit_Receive(reg_addr & 0x7F);
	while(i < len)
	{
		MS5611_Transmit_Receive(data[i++]); 				//Send Data to write
	}
	MS5611_CHIP_DESELECT(MS5611);
}

/**
 * @brief  Read factory data and coefficients
 * @retval No return value
 */
void MS5611_Read_Factory_Calibrated_Data(uint16_t* data)
{
	uint8_t	prom_data[PROM_SIZE];
	MS5611_Read_Buffer( MS5611_REG_PROM_REG , 16 , prom_data);
	data[0] = (uint16_t)(((uint16_t)(prom_data[0]  << 8)) | prom_data[1]);
	data[1] = (uint16_t)(((uint16_t)(prom_data[2]  << 8)) | prom_data[3]);
	data[2] = (uint16_t)(((uint16_t)(prom_data[4]  << 8)) | prom_data[5]);
	data[3] = (uint16_t)(((uint16_t)(prom_data[6]  << 8)) | prom_data[7]);
	data[4] = (uint16_t)(((uint16_t)(prom_data[8]  << 8)) | prom_data[9]);
	data[5] = (uint16_t)(((uint16_t)(prom_data[10] << 8)) | prom_data[11]);
	data[6] = (uint16_t)(((uint16_t)(prom_data[12] << 8)) | prom_data[13]);
	data[7] = (uint16_t)(((uint16_t)(prom_data[14] << 8)) | prom_data[15]);
}

/**
 * @brief  Initialize the device in reset form
 * @retval No return value
 */
int MS5611_Initialization(void)
{
	MS5611_SPI_GPIO_Initialization();
	MS5611_Read_Factory_Calibrated_Data(&MS5611.FACTORY_DATA) ;
	// MS5611_Writebyte( MS5611_PRESURE_D1 | OSR_1024,  0x01);
	return 0;
}

/**
 * @brief  Get raw pressure data
 * @retval No return value
 * @param  pressure			Pointer to store pressure data
 */
void MS5611_Get_Raw_Pressure(uint32_t* pressure)
{
	MS5611_Writebyte( MS5611_REG_PRESURE_D1 | OSR_1024, 0x01 );  // OSR_1024
	HAL_Delay(OSR_1024_CONVERSION_TIME);  	// 256->1 512->2 1024->3 2048->5 4096->9

	unsigned char data[ADC_SIZE];
	MS5611_Read_Buffer(MS5611_REG_ADC_REG, ADC_SIZE, data);
	pressure[0] = (uint32_t)(((uint32_t)(data[0] << 16)) | ((uint32_t)(data[1] << 8)) | data[2]);
}

/**
 * @brief  Get raw pressure temperature data
 * @retval No return value
 * @param  temperature		Pointer to store temperature data
 */
void MS5611_Get_Raw_Temperature(uint32_t* temperature)
{
	MS5611_Writebyte( MS5611_REG_TEMPERATURE_D2 | OSR_1024, 0x01 ); //OSR_1024
	HAL_Delay(OSR_1024_CONVERSION_TIME);  	// 256->1 512->2 1024->3 2048->5 4096->9

	unsigned char data[ADC_SIZE];
	MS5611_Read_Buffer(MS5611_REG_ADC_REG, ADC_SIZE, data);
	temperature[0]  = (uint32_t)(((uint32_t)(data[0] << 16)) | ((uint32_t)(data[1] << 8)) | data[2]);
}

/**
 * @brief  Get raw pressure temperature data
 * @retval No return value
 * @param  MS5611			Pointer to Structure
 */
void MS5611_Calculate_Temperature(Struct_MS5611* MS5611)
{
	int32_t TEMP;
	MS5611_Get_Raw_Temperature(&MS5611->raw_temperature);
	// Difference between actual and reference temperature
	// dT = D2 - T_REF = D2 - C5 * 2^8
	MS5611->dT=(int32_t)MS5611->raw_temperature - ((int32_t)MS5611->COEF5<<8);
	// Actual temperature (-40…85°C with 0.01°C resolution)
	// TEMP = 20°C + dT * TEMPSENS = 2000 + dT * C6 / 2^23
	TEMP = 2000 + ((int64_t)MS5611->dT * (int64_t)MS5611->COEF6>>23) ;
	// Second order temperature compensation
	if (TEMP<2000)
	{
		// T2 = dT2 / 2^31
		MS5611->T2=((int64_t)MS5611->dT*(int64_t)MS5611->dT)>>31;
		// OFFSET2 = 5 * (TEMP – 2000)^2 / 2^1
		MS5611->OFFSET2 = (5 * ((int64_t)TEMP - 2000) * ((int64_t)TEMP - 2000)) >> 1 ;
		// SENS2   = 5 * (TEMP – 2000)^2 / 2^2
		MS5611->SENS2 = (5 * ((int64_t)TEMP - 2000) * ((int64_t)TEMP - 2000)) >> 2 ;

		// Low temperature
		if( TEMP < -1500 )
		{
			// OFFSET2 = OFFSET2 + 7 * (TEMP + 1500)^2
			MS5611->OFFSET2 += 7 * ((int64_t)TEMP + 1500) * ((int64_t)TEMP + 1500) ;
			// SENS2 = SENS2 + 11 * (TEMP + 1500)^2/ 2^1
			MS5611->SENS2 += 11 * ((((int64_t)TEMP + 1500) * ((int64_t)TEMP + 1500)) >> 1) ;
		}
	}
	else
	{
		// High temperature
		MS5611->T2 = 0 ;
		MS5611->OFFSET2 = 0 ;
		MS5611->SENS2 = 0 ;
	}
	MS5611->temperature = ( (float)TEMP - MS5611->T2 ) / 100;
}

/**
 * @brief  Get raw pressure temperature data
 * @retval No return value
 * @param  MS5611			Pointer to Structure
 */
void MS5611_Calculate_Temperature_Compensated_Pressure(Struct_MS5611* MS5611)
{
	int64_t P;

	MS5611_Get_Raw_Temperature(&MS5611->raw_temperature);
	MS5611_Get_Raw_Pressure(&MS5611->raw_pressure);

	// OFFSET = OFF_T1 + TCO * dT = C2 * 2^16 + (C4 * dT ) / 2^7
	MS5611->OFFSET = ( (int64_t)(MS5611->COEF2) << 16 ) + ( ( (int64_t)(MS5611->COEF4) * MS5611->dT ) >> 7 ) ;

	// Sensitivity at actual temperature 
	// SENS = SENS_T1 + TCS * dT = C1 * 2^15 + (C3 * dT ) / 2^8
	MS5611->SENS = ( (int64_t)MS5611->COEF1 << 15 ) + ( ((int64_t)MS5611->COEF3 * MS5611->dT) >> 8 ) ;

	MS5611->OFFSET -= MS5611->OFFSET2 ;
	MS5611->SENS -= MS5611->SENS2 ;

	// Temperature compensated pressure (10…1200mbar with 0.01mbar resolution)
	// P = D1 * SENS - OFFSET = (D1 * SENS / 2^21 - OFFSET) / 2^15
	P = ( ( (MS5611->raw_pressure * MS5611->SENS) >> 21 ) - MS5611->OFFSET ) >> 15 ;
	MS5611->pressure = (float) P / 100;		// mbar
}

//#define Y 0.90f

/**
 * @brief  Get raw pressure temperature data
 * @retval No return value
 * @param  MS5611			Pointer to Structure
 */
void MS5611_Get_Altitude(Struct_MS5611 * MS5611)
{
	MS5611->altitude = ((1 - pow((MS5611->pressure*100) / SEA_LEVEL_PRESSURE, 1/5.257)) / 0.0000225577);
	//	MS5611->filtered_altitude = MS5611->filtered_altitude * Y + MS5611->altitude * (1.0f - Y);
}
