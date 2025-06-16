/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		icm45686.c
 * @brief   	:		This file includes the LL driver functions for ICM45686 IMU
 * ______________________________________________________________________________________________________
 */

#include "ICM45686.h"

Struct_ICM45686 ICM45686;

/**
 * @brief  Initialize GPIO pins for SPI communication
 * @retval No return value
 */
void ICM45686_SPI_GPIO_Initialization(void)
{

	/*ICM45686 GPIO Control Configuration
	 * PE10  ------> nCS_ICM45686_Pin (output)
	 * PE09  ------> INT_ICM45686_PIN (input)
	 */
	LL_GPIO_ResetOutputPin(nCS_ICM45686_GPIO_Port, nCS_ICM45686_Pin);

	GPIO_InitStruct.Pin = nCS_ICM45686_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(nCS_ICM45686_GPIO_Port, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = INT_ICM45686_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(INT_ICM45686_GPIO_Port, &GPIO_InitStruct);

	LL_SPI_Enable(ICM45686_SPI_CHANNEL);

	ICM45686_CHIP_DESELECT(ICM45686);
}

/**
 * @brief  Send and receive the data via SPI LL Drivers
 * @retval Received data
 * @param  data			Data to send
 */
static unsigned char ICM45686_Transmit_Receive(unsigned char data)
{
	LL_SPI_TransmitData8(ICM45686_SPI_CHANNEL, data);
	while(LL_SPI_IsActiveFlag_TXC(ICM45686_SPI_CHANNEL)==RESET);

	while(LL_SPI_IsActiveFlag_RXWNE(ICM45686_SPI_CHANNEL)==RESET);
	return LL_SPI_ReceiveData8(ICM45686_SPI_CHANNEL);
}

/**
 * @brief  Read 1 byte of given register
 * @retval Received data
 * @param  reg_addr		Register address to write
 */
uint8_t ICM45686_Readbyte(uint8_t reg_addr)
{
	uint8_t val;

	ICM45686_CHIP_SELECT(ICM45686);
	ICM45686_Transmit_Receive(reg_addr | 0x80); 			//Register. MSB 1 is read instruction.
	val = ICM45686_Transmit_Receive(0x00); 				//Send DUMMY to read data
	ICM45686_CHIP_DESELECT(ICM45686);

	return val;
}

/**
 * @brief  Read n bytes starting from given register address
 * @retval No return value
 * @param  reg_addr		Register address to start read from
 * @param  len			Number of byte to read
 * @param  data			Pointer to data
 */
void ICM45686_Read_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data)
{
	unsigned int i = 0;

	ICM45686_CHIP_SELECT(ICM45686);
	ICM45686_Transmit_Receive(reg_addr | 0x80); 			//Register. MSB 1 is read instruction.
	while(i < len)
	{
		data[i++] = ICM45686_Transmit_Receive(0x00); 		//Send DUMMY to read data
	}
	ICM45686_CHIP_DESELECT(ICM45686);
}

/**
 * @brief  Write 1 bytes in given register address
 * @retval No return value
 * @param  reg_addr		Register address to write
 * @param  val				Value to be written
 */
void ICM45686_Writebyte(uint8_t reg_addr, uint8_t val)
{
	ICM45686_CHIP_SELECT(ICM45686);
	ICM45686_Transmit_Receive(reg_addr & 0x7F); 			//Register. MSB 0 is write instruction.
	ICM45686_Transmit_Receive(val); 						//Send Data to write
	ICM45686_CHIP_DESELECT(ICM45686);
}

/**
 * @brief  Write n bytes starting from given register address
 * @retval No return value
 * @param  reg_addr		Register address to write
 * @param  len				Number of byte to read
 * @param  data			Pointer of data to be written
 */
void ICM45686_Write_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data)
{
	unsigned int i = 0;
	ICM45686_CHIP_SELECT(ICM45686);
	ICM45686_Transmit_Receive(reg_addr & 0x7F); 			//Register. MSB 0 is write instruction.
	while(i < len)
	{
		ICM45686_Transmit_Receive(data[i++]); 				//Send Data to write
	}
	ICM45686_CHIP_DESELECT(ICM45686);
}

/**
 * @brief  Initialize the device in reset form
 * @retval No return value
 */
int ICM45686_Initialization(void)
{


	ICM45686_SPI_GPIO_Initialization();
	/*
	uint8_t who_am_i = 0;
	printf("Checking ICM45686...");
	who_am_i = ICM45686_Readbyte(ICM45686_REG_WHO_AM_I);
	if(who_am_i == ICM45686_ID)
	{
		printf("\nICM45686 who_am_i = 0x%02x...OK\n\n", who_am_i);
	}
	else if(who_am_i != ICM45686_ID)
	{
		who_am_i = ICM45686_Readbyte(ICM45686_REG_WHO_AM_I);
		if (who_am_i != ICM45686_ID){
			printf( "ICM45686 Not OK: 0x%02x Should be 0x%02x\n", who_am_i, ICM45686_ID);
			return 1;
		}
	}
	 */
	// GYRO_MODE | ACCEL MODE
	ICM45686_Writebyte(ICM45686_REG_PWR_MGMT0, ICM45686_GYRO_LOW_NOISE|ICM45686_ACCEL_LOW_NOISE);
	HAL_Delay(1);
	// 2000DPS 1.6KHz
	ICM45686_Writebyte(ICM45686_REG_GYRO_CONFIG0, ICM45686_GYRO_2000_DPS|ICM45686_GYRO_ODR_1_6KHz);
	HAL_Delay(1);
	// 16G 1.6KHz
	ICM45686_Writebyte(ICM45686_REG_ACCEL_CONFIG0, ICM45686_ACCEL_16G|ICM45686_ACCEL_ODR_1_6KHz);
	HAL_Delay(1);
	// Enable DRDY Interrupt
	ICM45686_Writebyte(ICM45686_REG_INT1_CONFIG0, 0x04);
	HAL_Delay(1);
	// Enable DRDY Interrupt
	ICM45686_Writebyte(ICM45686_REG_INT1_CONFIG2, 0x02);
	HAL_Delay(1);

	return 0;
}

/**
 * @brief  Get 6 axis accel gyro raw data
 * @retval No return value
 * @param  accel			Pointer to store accel data
 * @param  gyro			Pointer to store gyro data
 */
void ICM45686_Get6AxisRawData(short* accel, short* gyro)
{
	/*MSB FIRST*/
	unsigned char data[12];
	ICM45686_Read_Buffer(ICM45686_REG_ACCEL_DATA_X1_UI, 12, data);

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
 * @brief  Get 3 axis gyroscope raw data
 * @retval No return value
 * @param  gyro			Pointer to store data
 */
void ICM45686_Get3AxisGyroRawData(short* gyro)
{
	/*MSB FIRST*/
	unsigned char data[6];
	ICM45686_Read_Buffer(ICM45686_REG_GYRO_DATA_X1_UI, 6, data);

	gyro[0] = ((data[0] << 8) | data[1]);
	gyro[1] = ((data[2] << 8) | data[3]);
	gyro[2] = ((data[4] << 8) | data[5]);

	/*OR*/

	//	gyro[0] = (short)(((short)(data[0] << 8))  | data[1]);
	//	gyro[1] = (short)(((short)(data[2] << 8))  | data[3]);
	//	gyro[2] = (short)(((short)(data[4] << 8))  | data[5]);
}

/**
 * @brief  Get 3 axis accel raw data
 * @retval No return value
 * @param  accel			Pointer to store data
 */
void ICM45686_Get3AxisAccRawData(short* accel)
{
	unsigned char data[6];
	ICM45686_Read_Buffer(ICM45686_REG_ACCEL_DATA_X1_UI, 6, data);

	accel[0] = ((data[0] << 8) | data[1]);
	accel[1] = ((data[2] << 8) | data[3]);
	accel[2] = ((data[4] << 8) | data[5]);

	/*OR*/

	//	accel[0] = (short)(((short)(data[0] << 8))  | data[1]);
	//	accel[1] = (short)(((short)(data[2] << 8))  | data[3]);
	//	accel[2] = (short)(((short)(data[4] << 8))  | data[5]);
}

/**
 * @brief  Get temperature raw data
 * @retval No return value
 * @param  temperature			Pointer to store data
 */
void ICM45686_TempRawData(short* temperature)
{
	/*MSB FIRST*/
	unsigned char data[2];
	ICM45686_Read_Buffer(ICM45686_REG_TEMP_DATA1_UI, 2, data);

	temperature[0] = ((data[0] << 8) | data[1]);
	/*OR*/
	//	temperature[0] = (short)(((short)(data[0] << 8))  | data[1]);
}

/**
 * @brief  Get accel and temperature raw data and convert to g and degree celsius
 * @retval No return value
 * @param  ICM45686		Pointer to Structure data
 * @param  range			Range of accelerometer
 * @param  dps				Range of gyroscope
 */
void ICM45686_Calculate_Accel_Gyro_Temp(Struct_ICM45686* ICM45686, float range, float dps)
{
	ICM45686_Get6AxisRawData(&ICM45686->accel_x_raw , &ICM45686->gyro_x_raw);
	ICM45686_TempRawData(&ICM45686->temperature_raw);

	ICM45686->accel_x = ICM45686->accel_x_raw * range / 32768.f;
	ICM45686->accel_y = ICM45686->accel_y_raw * range / 32768.f;
	ICM45686->accel_z = ICM45686->accel_z_raw * range / 32768.f;
	ICM45686->gyro_x  = ICM45686->gyro_x_raw  * dps / 32768.f;
	ICM45686->gyro_y  = ICM45686->gyro_y_raw  * dps / 32768.f;
	ICM45686->gyro_z  = ICM45686->gyro_z_raw  * dps / 32768.f;
	ICM45686->temperature = (ICM45686->temperature_raw / 132.48f)+25.f;
}

/**
 * @brief  Get timestamp raw data
 * @retval No return value
 * @param  timestamp			Pointer to store data
 */
// void ICM45686_TimeStampRawData(short* timestamp)
// {
//	/*MSB FIRST*/
// 	unsigned char data[2];
// 	ICM45686_Read_Buffer(ICM45686_REG_TMST_FSYNCH, 2, data);
// 	timestamp[0] = ((data[0] << 8) | data[1]);
//	/*OR*/
// 	timestamp[0] = (short)(((short)(data[0] << 8))  | data[1]);
// }

/**
 * @brief  Check if chip data is ready
 * @retval Ready = 1, Not ready = 0
 */
int ICM45686_DataReady(void)
{
	return LL_GPIO_IsInputPinSet(INT_ICM45686_GPIO_Port, INT_ICM45686_PIN);
}
