/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		icm42688p.c
 * @brief   	:		This file includes the LL driver functions for ICM42688P IMU
 * ______________________________________________________________________________________________________
 */

#include "ICM42688P.h"

Struct_ICM42688P ICM42688P;

/**
 * @brief  Initialize GPIO pins for SPI communication
 * @retval No return value
 */
void ICM42688P_SPI_GPIO_Initialization(void)
{
	LL_SPI_InitTypeDef SPI_InitStruct = {0};
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Peripheral clock enable */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_SPI2);

	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOI);
	LL_AHB4_GRP1_EnableClock(LL_AHB4_GRP1_PERIPH_GPIOC);
	/**SPI2 GPIO Configuration
	PI1   ------> SPI2_SCK
	PI3   ------> SPI2_MOSI
	PC2   ------> SPI2_MISO
	 */
	GPIO_InitStruct.Pin = LL_GPIO_PIN_1|LL_GPIO_PIN_3;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
	LL_GPIO_Init(GPIOI, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
	LL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
	SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
	SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
	SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;
	SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
	SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
	SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV4;
	SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
	SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
	SPI_InitStruct.CRCPoly = 0x0;
	LL_SPI_Init(SPI2, &SPI_InitStruct);
	LL_SPI_SetStandard(SPI2, LL_SPI_PROTOCOL_MOTOROLA);
	LL_SPI_SetFIFOThreshold(SPI2, LL_SPI_FIFO_TH_01DATA);
	LL_SPI_EnableNSSPulseMgt(SPI2);

	/**ICM42688P GPIO Control Configuration
	 * PE8  ------> ICM42688P_SPI_nCS_PIN (output)
	 * PG3  ------> ICM42688P_INT_PIN (input)
	 */
	LL_GPIO_ResetOutputPin(ICM42688P_SPI_nCS_PORT, ICM42688P_SPI_nCS_PIN);

	GPIO_InitStruct.Pin = ICM42688P_SPI_nCS_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(ICM42688P_SPI_nCS_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = ICM42688P_INT_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(ICM42688P_INT_PORT, &GPIO_InitStruct);

	LL_SPI_Enable(ICM42688P_SPI_CHANNEL);

	ICM42688P_CHIP_DESELECT(ICM42688P);
}

/**
 * @brief  Send and receive the data via SPI LL Drivers
 * @retval Received data
 * @param  data			Data to send
 */
static unsigned char ICM42688P_Transmit_Receive(unsigned char data)
{
	LL_SPI_TransmitData8(ICM42688P_SPI_CHANNEL, data);
	while(LL_SPI_IsActiveFlag_TXC(ICM42688P_SPI_CHANNEL)==RESET);

	while(LL_SPI_IsActiveFlag_RXWNE(ICM42688P_SPI_CHANNEL)==RESET);
	return LL_SPI_ReceiveData8(ICM42688P_SPI_CHANNEL);
}

/**
 * @brief  Read 1 byte of given register
 * @retval Received data
 * @param  reg_addr		Register address to read
 */
uint8_t ICM42688P_Readbyte(uint8_t reg_addr)
{
	uint8_t val;

	ICM42688P_CHIP_SELECT(ICM42688P);
	ICM42688P_Transmit_Receive(reg_addr | 0x80); 				//Register. MSB 1 is read instruction.
	val = ICM42688P_Transmit_Receive(0x00); 					//Send DUMMY to read data
	ICM42688P_CHIP_DESELECT(ICM42688P);

	return val;
}

/**
 * @brief  Read n bytes starting from given register address
 * @retval No return value
 * @param  reg_addr		Register address to start read
 * @param  len			Number of byte to read
 * @param  data			Pointer to data
 */
void ICM42688P_Read_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data)
{
	unsigned int i = 0;

	ICM42688P_CHIP_SELECT(ICM42688P);
	ICM42688P_Transmit_Receive(reg_addr | 0x80); 				//Register. MSB 1 is read instruction.
	while(i < len)
	{
		data[i++] = ICM42688P_Transmit_Receive(0x00);			//Send DUMMY to read data
	}
	ICM42688P_CHIP_DESELECT(ICM42688P);
}

/**
 * @brief  Write 1 bytes in given register address
 * @retval No return value
 * @param  reg_addr		Register address to write
 * @param  val				Value to be written
 */
void ICM42688P_Writebyte(uint8_t reg_addr, uint8_t val)
{
	ICM42688P_CHIP_SELECT(ICM42688P);
	ICM42688P_Transmit_Receive(reg_addr & 0x7F); 				//Register. MSB 0 is write instruction.
	ICM42688P_Transmit_Receive(val); 							//Send Data to write
	ICM42688P_CHIP_DESELECT(ICM42688P);
}

/**
 * @brief  Write n bytes starting from given register address
 * @retval No return value
 * @param  reg_addr		Register address to start write
 * @param  len			Number of byte to read
 * @param  data			Pointer of data to be written
 */
void ICM42688P_Write_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data)
{
	unsigned int i = 0;
	ICM42688P_CHIP_SELECT(ICM42688P);
	ICM42688P_Transmit_Receive(reg_addr & 0x7F); 				//Register. MSB 0 is write instruction.
	while(i < len)
	{
		ICM42688P_Transmit_Receive(data[i++]); 				//Send Data to write
	}
	ICM42688P_CHIP_DESELECT(ICM42688P);
}

/**
 * @brief  Initialize the device in reset form
 * @retval No return value
 */
int ICM42688P_Initialization(void)
{


	ICM42688P_SPI_GPIO_Initialization();
	/*
	uint8_t who_am_i = 0;
	printf("Checking ICM42688P...");
	who_am_i = ICM42688P_Readbyte(ICM42688P_REG_WHO_AM_I);
	if(who_am_i == ICM42688P_ID)
	{
		printf("\nICM42688P who_am_i = 0x%02x...OK\n\n", who_am_i);
	}
	else if(who_am_i != ICM42688P_ID)
	{
		who_am_i = ICM42688P_Readbyte(ICM42688P_REG_WHO_AM_I);
		if (who_am_i != ICM42688P_ID){
			printf( "ICM42688P Not OK: 0x%02x Should be 0x%02x\n", who_am_i, ICM42688P_ID);
			return 1;
		}
	}
	 */
	// SOFT RESET
	ICM42688P_Writebyte(ICM42688P_REG_DEVICE_CONFIG, ICM42688P_SOFT_RESET);
	HAL_Delay(1);
	// TEMP ON BY DEFAULT & GYRO-ACCEL LOW NOISE MODE
	ICM42688P_Writebyte(ICM42688P_REG_PWR_MGMT0, ICM42688P_GYRO_LOW_NOISE|ICM42688P_ACCEL_LOW_NOISE);
	HAL_Delay(1);
	// 16G | 1KHz
	ICM42688P_Writebyte(ICM42688P_REG_ACCEL_CONFIG0, ICM42688P_ACCEL_16G|ICM42688P_ACCEL_ODR_1KHz);
	HAL_Delay(1);
	// 2000DPS | 1KHz
	ICM42688P_Writebyte(ICM42688P_REG_GYRO_CONFIG0, ICM42688P_GYRO_2000_DPS|ICM42688P_GYRO_ODR_1KHz);
	HAL_Delay(1);
	// Enable DRDY Interrupt INT2 LATCHED INT1 LATCHED
	ICM42688P_Writebyte(ICM42688P_REG_INT_CONFIG, 0x36);
	HAL_Delay(1);

	return 0;
}

/**
 * @brief  Get 6 axis accel gyro raw data
 * @retval No return value
 * @param  accel		Pointer to store accel data
 * @param  gyro			Pointer to store gyro data
 */
void ICM42688P_Get6AxisRawData(short* accel, short* gyro)
{
	/*MSB FIRST*/
	unsigned char data[12];
	ICM42688P_Read_Buffer(ICM42688P_REG_ACCEL_DATA_X1, 12, data);

	accel[0] = (data[0]  << 8) | data[1];
	accel[1] = (data[2]  << 8) | data[3];
	accel[2] = (data[4]  << 8) | data[5];

	gyro[0] = ((data[6]  << 8) | data[7]);
	gyro[1] = ((data[8]  << 8) | data[9]);
	gyro[2] = ((data[10] << 8) | data[11]);

	/*OR*/

	//	accel[0] = (short)(((short)(data[0] << 8))  | data[1]);
	//	accel[1] = (short)(((short)(data[2] << 8))  | data[3]);
	//	accel[2] = (short)(((short)(data[4] << 8))  | data[5]);
	//	gyro[0]  = (short)(((short)(data[6] << 8))  | data[7]);
	//	gyro[1]  = (short)(((short)(data[8] << 8))  | data[9]);
	//	gyro[2]  = (short)(((short)(data[10] << 8)) | data[11]);
}

/**
 * @brief  Get 3 axis accel raw data
 * @retval No return value
 * @param  accel			Pointer to store data
 */
void ICM42688P_Get3AxisAccRawData(short* accel)
{
	/*MSB FIRST*/
	unsigned char data[6];
	ICM42688P_Read_Buffer(ICM42688P_REG_ACCEL_DATA_X1, 6, data);

	accel[0] = ((data[0] << 8) | data[1]);
	accel[1] = ((data[2] << 8) | data[3]);
	accel[2] = ((data[4] << 8) | data[5]);

	/*OR*/

	//	accel[0] = (short)(((short)(data[0] << 8))  | data[1]);
	//	accel[1] = (short)(((short)(data[2] << 8))  | data[3]);
	//	accel[2] = (short)(((short)(data[4] << 8))  | data[5]);
}

/**
 * @brief  Get 3 axis gyroscope raw data
 * @retval No return value
 * @param  gyro			Pointer to store data
 */
void ICM42688P_Get3AxisGyroRawData(short* gyro)
{
	/*MSB FIRST*/
	unsigned char data[6];
	ICM42688P_Read_Buffer(ICM42688P_REG_GYRO_DATA_X1, 6, data);

	gyro[0] = ((data[0] << 8) | data[1]);
	gyro[1] = ((data[2] << 8) | data[3]);
	gyro[2] = ((data[4] << 8) | data[5]);

	/*OR*/

	//	gyro[0] = (short)(((short)(data[0] << 8))  | data[1]);
	//	gyro[1] = (short)(((short)(data[2] << 8))  | data[3]);
	//	gyro[2] = (short)(((short)(data[4] << 8))  | data[5]);
}

/**
 * @brief  Get temperature raw data
 * @retval No return value
 * @param  temperature			Pointer to store data
 */
void ICM42688P_TempRawData(short* temperature)
{
	unsigned char data[2];
	ICM42688P_Read_Buffer(ICM42688P_REG_TEMP_DATA1, 2, data);

	temperature[0] = ((data[0] << 8) | data[1]);

	/*OR*/

	//	temperature[0] = (short)(((short)(data[0] << 8))  | data[1]);
}

/**
 * @brief  Get accel and temperature raw data and convert to g and degree celsius
 * @retval No return value
 * @param  ICM42688P		Pointer to Structure data
 * @param  range			Range of accelerometer
 * @param  dps				Range of gyroscope
 */
void ICM42688P_Calculate_Accel_Gyro_Temp(Struct_ICM42688P* ICM42688P, float range, float dps)
{
	ICM42688P_Get6AxisRawData(&ICM42688P->accel_x_raw , &ICM42688P->gyro_x_raw);
	ICM42688P_TempRawData(&ICM42688P->temperature_raw);

	ICM42688P->accel_x = ICM42688P->accel_x_raw * range / 32768.f;
	ICM42688P->accel_y = ICM42688P->accel_y_raw * range / 32768.f;
	ICM42688P->accel_z = ICM42688P->accel_z_raw * range / 32768.f;
	ICM42688P->gyro_x  = ICM42688P->gyro_x_raw  * dps / 32768.f;
	ICM42688P->gyro_y  = ICM42688P->gyro_y_raw  * dps / 32768.f;
	ICM42688P->gyro_z  = ICM42688P->gyro_z_raw  * dps / 32768.f;
	ICM42688P->temperature = (ICM42688P->temperature_raw / 132.48f)+25.f;
}

/**
 * @brief  Get timestamp raw data
 * @retval No return value
 * @param  timestamp			Pointer to store data
 */
// void ICM42688P_TimeStampRawData(short* timestamp)
// {
// 	unsigned char data[2];
// 	ICM42688P_Read_Buffer(ICM42688P_REG_TMST_FSYNCH, 2, data);
// 	timestamp[0] = ((data[0] << 8) | data[1]);
//	/*OR*/
// 	timestamp[0] = (short)(((short)(data[0] << 8))  | data[1]);
// }

/**
 * @brief  Check if chip data is ready
 * @retval Ready = 1, Not ready = 0
 */
int ICM42688P_DataReady(void)
{
	return LL_GPIO_IsInputPinSet(ICM42688P_INT_PORT, ICM42688P_INT_PIN);
}
