/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		bmi088.c
 * @brief   	:		This file includes the LL driver functions for BMI088 IMU
 * ______________________________________________________________________________________________________
 */

#include "BMI088.h"

Struct_BMI088 BMI088;

/**
 * @brief  Initialize GPIO pins for SPI communication
 * @retval No return value
 */
void BMI088_SPI_GPIO_Initialization(void)
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

	/**BMI088 GPIO Control Configuration
	 * PE10 ------> BMI088_SPI_nCS_ACCEL_PIN (output)
	 * PE11 ------> BMI088_SPI_nCS_GYRO_PIN (output)
	 * PG4  ------> BMI088_INT_ACCEL1_PIN (input)
	 * PG5  ------> BMI088_INT_ACCEL2_PIN (input)
	 * PG6  ------> BMI088_INT_GYRO1_PIN (input)
	 * PG7  ------> BMI088_INT_GYRO2_PIN (input)
	 */
	LL_GPIO_ResetOutputPin(BMI088_SPI_nCS_ACCEL_PORT, BMI088_SPI_nCS_ACCEL_PIN);
	LL_GPIO_ResetOutputPin(BMI088_SPI_nCS_GYRO_PORT,  BMI088_SPI_nCS_GYRO_PIN);

	GPIO_InitStruct.Pin = BMI088_SPI_nCS_ACCEL_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(BMI088_SPI_nCS_ACCEL_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = BMI088_INT_ACCEL1_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(BMI088_INT_ACCEL1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = BMI088_INT_ACCEL2_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(BMI088_INT_ACCEL2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = BMI088_SPI_nCS_GYRO_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(BMI088_SPI_nCS_GYRO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = BMI088_INT_GYRO1_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(BMI088_INT_GYRO1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = BMI088_INT_GYRO2_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(BMI088_INT_GYRO2_PORT, &GPIO_InitStruct);

	LL_SPI_Enable(BMI088_SPI_CHANNEL);

	BMI088_CHIP_DESELECT_ACCEL(BMI088);
	BMI088_CHIP_DESELECT_GYRO(BMI088);
}

/**
 * @brief  Send and receive the data via SPI LL Drivers
 * @retval Received data
 * @param  data			Data to send
 */
static unsigned char BMI088_Transmit_Receive(unsigned char data)
{
	LL_SPI_TransmitData8(BMI088_SPI_CHANNEL, data);
	while(LL_SPI_IsActiveFlag_TXC(BMI088_SPI_CHANNEL)==RESET);

	while(LL_SPI_IsActiveFlag_RXWNE(BMI088_SPI_CHANNEL)==RESET);
	return LL_SPI_ReceiveData8(BMI088_SPI_CHANNEL);
}

/**
 * @brief  Read 1 byte of given register
 * @retval Received data
 * @param  reg_addr		Register address to read from
 */
uint8_t BMI088_Readbyte(uint8_t reg_addr)
{
	uint8_t val;

	BMI088_Transmit_Receive((reg_addr << 1) | 0x01); 		//Register lsb bit#0 1 is read instruction.
	val = BMI088_Transmit_Receive(0x00); 					//Send DUMMY to read data

	return val;
}

/**
 * @brief  Read n bytes starting from given register address
 * @retval No return value
 * @param  reg_addr		Register address to start read from
 * @param  len			Number of byte to read
 * @param  data			Pointer to data
 */
void BMI088_Read_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data)
{
	unsigned int i = 0;

	BMI088_Transmit_Receive((reg_addr << 1) | 0x01); 		//Register. lsb bit#0 1 is read instruction.
	while(i < len)
	{
		data[i++] = BMI088_Transmit_Receive(0x00);			//Send DUMMY to read data
	}
}

/**
 * @brief  Write 1 bytes in given register address
 * @retval No return value
 * @param  reg_addr		Register address to write
 * @param  val			Value to be written
 */
void BMI088_Writebyte(uint8_t reg_addr, uint8_t val)
{
	BMI088_Transmit_Receive((reg_addr << 1) & 0xFE); 		//Register. lsb bit#0 0 is write instruction..
	BMI088_Transmit_Receive(val); 							//Send Data to write
}

/**
 * @brief  Write n bytes starting from given register address
 * @retval No return value
 * @param  reg_addr		Register address to start write
 * @param  len			Number of byte to read
 * @param  data			Pointer of data to be written
 */
void BMI088_Write_Buffer(unsigned char reg_addr, unsigned char len, unsigned char* data)
{
	unsigned int i = 0;
	BMI088_Transmit_Receive((reg_addr << 1) & 0xFE); 		//Register. lsb bit#0 0 is write instruction.
	while(i < len)
	{
		BMI088_Transmit_Receive(data[i++]); 				//Send Data to write
	}
}

/**
 * @brief  Initialize the device in reset form
 * @retval 0
 */
int BMI088_Initialization(void)
{

	BMI088_SPI_GPIO_Initialization();
	/*
	uint8_t who_am_i = 0;
	printf("Checking BMI088...");
	who_am_i = BMI088_Readbyte(BMI088_ACC_REG_CHIP_ID); //////////SELECT DESELECT
	if(who_am_i == 0x1E)
	{
		printf("\nBMI088 who_am_i = 0x%02x...OK\n\n", who_am_i);
	}
	else if(who_am_i != 0x1E)
	{
		who_am_i = BMI088_Readbyte(BMI088_ACC_REG_CHIP_ID);
		if (who_am_i != 0x1E){
			printf( "BMI088 Not OK: 0x%02x Should be 0x%02x\n", who_am_i, 0x47);
			return 1;
		}
	}
	 */
	// SOFT RESET -> IT ALSO RESET ALL REGISTER  VALUES 0xB6
	BMI088_Writebyte(BMI088_ACC_REG_SOFT_RESET, BMI088_SOFT_RESET_VALUE);
	HAL_Delay(1);
	// NORMAL FILTER AND 1600 ODR
	BMI088_Writebyte(BMI088_ACC_REG_CONF, BMI088_ACCEL_NORMAL|BMI088_ACCEL_ODR_1600);
	HAL_Delay(1);
	// 24G   0x03
	BMI088_Writebyte(BMI088_ACC_REG_RANGE, BMI088_ACCEL_RANGE_24G);
	HAL_Delay(1);
	// ACTIVE MODE
	BMI088_Writebyte(BMI088_ACC_REG_PWR_CONF, BMI088_ACCEL_ACTIVE_MODE);
	HAL_Delay(1);
	// ACCELEROMETER ON
	BMI088_Writebyte(BMI088_ACC_REG_PWR_CTRL, BMI088_ACCEL_ON);
	HAL_Delay(1);

	// SOFT RESET -> IT ALSO RESET ALL REGISTER  VALUES  0XB6
	BMI088_Writebyte(BMI088_GYRO_REG_SOFT_RESET, BMI088_SOFT_RESET_VALUE);
	HAL_Delay(30);
	BMI088_Writebyte(BMI088_GYRO_REG_LPM1, BMI088_GYRO_NORMAL);
	HAL_Delay(1);
	// BANDWIDTH 532 HZ ODR 2000
	BMI088_Writebyte(BMI088_GYRO_REG_BANDWIDTH, BMI088_GYRO_ODR_2000_532BW);
	HAL_Delay(1);
	// 2000 DPS   0x00
	BMI088_Writebyte(BMI088_GYRO_REG_RANGE, BMI088_GYRO_RANGE_2000DPS);
	HAL_Delay(1);

	return 0;
}
/**
 * @brief  Get 6 axis accel gyro raw data
 * @retval No return value
 * @param  accel		Pointer to store accel data
 * @param  gyro			Pointer to store gyro data
 */
// void BMI088_Get6AxisRawData(short* accel, short* gyro)
// {
//	/*LSB FIRST BOTH*/
// 	BMI088_CHIP_SELECT_ACCEL(BMI088);
// 	BMI088_Read_Buffer(BMI088_ACC_REG_X_LSB, 6, (unsigned char*)accel);
// 	BMI088_CHIP_DESELECT_ACCEL(BMI088);
//
// 	BMI088_CHIP_SELECT_ACCEL(BMI088);
// 	BMI088_Read_Buffer(BMI088_GYRO_REG_RATE_X_LSB, 6, (unsigned char*)gyro);
// 	BMI088_CHIP_DESELECT_ACCEL(BMI088);
//
// 	/*OR*/
//
// 	unsigned char data_accel[6];
// 	BMI088_CHIP_SELECT_ACCEL(BMI088);
// 	BMI088_Read_Buffer(BMI088_ACC_REG_X_LSB, 6, data_accel);
// 	BMI088_CHIP_DESELECT_ACCEL(BMI088);
//
// 	accel[0] = (data_accel[1] << 8) | data_accel[0];
// 	accel[1] = (data_accel[3] << 8) | data_accel[2];
// 	accel[2] = (data_accel[5] << 8) | data_accel[4];
// 	accel[0] = (short)(((short)(data_accel[1] << 8))  | data_accel[0]);
// 	accel[1] = (short)(((short)(data_accel[3] << 8))  | data_accel[2]);
// 	accel[2] = (short)(((short)(data_accel[5] << 8))  | data_accel[4]);
//
// 	unsigned char data_gyro[6];
// 	BMI088_CHIP_SELECT_GYRO(BMI088);
// 	BMI088_Read_Buffer(BMI088_GYRO_REG_RATE_X_LSB, 6, data_gyro);
// 	BMI088_CHIP_DESELECT_GYRO(BMI088);
//
// 	gyro[0] = (data_gyro[1] << 8) | data_gyro[0];
// 	gyro[1] = (data_gyro[3] << 8) | data_gyro[2];
// 	gyro[2] = (data_gyro[5] << 8) | data_gyro[4];
// 	gyro[0] = (short)(((short)(data_gyro[1] << 8))  | data_gyro[0]);
// 	gyro[1] = (short)(((short)(data_gyro[3] << 8))  | data_gyro[2]);
// 	gyro[2] = (short)(((short)(data_gyro[5] << 8))  | data_gyro[4]);
// }

/**
 * @brief  Get 3 axis accel raw data
 * @retval No return value
 * @param  accel			Pointer to store data
 */
void BMI088_Get3AxisAccelRawData(short* accel)
{
	/*LSB FIRST*/
	// 	BMI088_CHIP_SELECT_ACCEL(BMI088);
	// 	BMI088_Read_Buffer(BMI088_ACC_REG_X_LSB, 6, (unsigned char*)accel);
	// 	BMI088_CHIP_DESELECT_ACCEL(BMI088);

	/*OR*/

	unsigned char data_accel[6];
	BMI088_CHIP_SELECT_ACCEL(BMI088);
	BMI088_Read_Buffer(BMI088_ACC_REG_X_LSB, 6, data_accel);
	BMI088_CHIP_DESELECT_ACCEL(BMI088);

	accel[0] = (data_accel[1] << 8) | data_accel[0];
	accel[1] = (data_accel[3] << 8) | data_accel[2];
	accel[2] = (data_accel[5] << 8) | data_accel[4];
	// 	accel[0] = (short)(((short)(data_accel[1] << 8))  | data_accel[0]);
	// 	accel[1] = (short)(((short)(data_accel[3] << 8))  | data_accel[2]);
	// 	accel[2] = (short)(((short)(data_accel[5] << 8))  | data_accel[4]);
}

/**
 * @brief  Get 3 axis gyroscope raw data
 * @retval No return value
 * @param  gyro			Pointer to store data
 */
void BMI088_Get3AxisGyroRawData(short* gyro)
{
	/*LSB FIRST*/
	// 	BMI088_CHIP_SELECT_ACCEL(BMI088);
	// 	BMI088_Read_Buffer(BMI088_GYRO_REG_RATE_X_LSB, 6, (unsigned char*)gyro);
	// 	BMI088_CHIP_DESELECT_ACCEL(BMI088);

	/*OR*/

	unsigned char data_gyro[6];
	BMI088_CHIP_SELECT_GYRO(BMI088);
	BMI088_Read_Buffer(BMI088_GYRO_REG_RATE_X_LSB, 6, data_gyro);
	BMI088_CHIP_DESELECT_GYRO(BMI088);

	gyro[0] = (data_gyro[1] << 8) | data_gyro[0];
	gyro[1] = (data_gyro[3] << 8) | data_gyro[2];
	gyro[2] = (data_gyro[5] << 8) | data_gyro[4];
	// 	gyro[0] = (short)(((short)(data_gyro[1] << 8))  | data_gyro[0]);
	// 	gyro[1] = (short)(((short)(data_gyro[3] << 8))  | data_gyro[2]);
	// 	gyro[2] = (short)(((short)(data_gyro[5] << 8))  | data_gyro[4]);
}

/**
 * @brief  Get sensortime raw data
 * @retval No return value
 * @param  sensortime			Pointer to store data
 */
void BMI088_GetSensortime(uint32_t* sensortime)
{
	/*LSB FIRST*/
	// 	BMI088_CHIP_SELECT_ACCEL(BMI088);
	// 	BMI088_Read_Buffer(BMI088_ACC_REG_SENSORTIME_0, 3, (unsigned char*)sensortime);
	// 	BMI088_CHIP_DESELECT_ACCEL(BMI088);

	/*OR*/

	unsigned char data[3];
	BMI088_CHIP_SELECT_ACCEL(BMI088);
	BMI088_Read_Buffer(BMI088_ACC_REG_SENSORTIME_0, 3, data);
	BMI088_CHIP_DESELECT_ACCEL(BMI088);

	sensortime[0]  = (data[2] << 16) | (data[1] << 8) | data[0];
	// 	sensortime[0]  = (uint32_t)(((uint32_t)(data[2] << 16)) | ((uint32_t)(data[1] << 8)) | data[0]);
}

/**
 * @brief  Get temperature raw data
 * @retval No return value
 * @param  temperature			Pointer to store data
 */
void BMI088_TempRawData(short* temperature)
{
	/*MSB FIRST*/
	unsigned char data[2];
	BMI088_CHIP_SELECT_ACCEL(BMI088);
	BMI088_Read_Buffer(BMI088_ACC_REG_TEMP_MSB, 2, data);
	BMI088_CHIP_DESELECT_ACCEL(BMI088);
	temperature[0] = ((data[0] << 8) | data[1]);

	/*OR*/

	//	temperature[0] = (short)(((short)(data[0] << 8))  | data[1]);
}

/**
 * @brief  Get accel and temperature raw data and convert to g and degree celsius
 * @retval No return value
 * @param  BMI088			Pointer to Structure data
 * @param  range			Range of accelerometer
 */
void BMI088_Calculate_Accel_Temp(Struct_BMI088* BMI088, float range)
{
	BMI088_Get3AxisAccelRawData(&BMI088->accel_x_raw);
	BMI088_TempRawData(&BMI088->temperature_raw);

	BMI088->accel_x = BMI088->accel_x_raw * range / 32768.f;
	BMI088->accel_y = BMI088->accel_y_raw * range / 32768.f;
	BMI088->accel_z = BMI088->accel_z_raw * range / 32768.f;
	BMI088->temperature = (BMI088->temperature_raw / 132.48f)+25.f;
}

/**
 * @brief  Get gyroscope raw data and convert dps
 * @retval No return value
 * @param  BMI088			Pointer to Structure data
 * @param  dps				Range of gyroscope
 */
void BMI088_Calculate_Gyro(Struct_BMI088* BMI088, float dps)
{
	BMI088_Get3AxisGyroRawData(&BMI088->gyro_x_raw);
	BMI088->gyro_x  = BMI088->gyro_x_raw  * dps / 32768.f;
	BMI088->gyro_y  = BMI088->gyro_y_raw  * dps / 32768.f;
	BMI088->gyro_z  = BMI088->gyro_z_raw  * dps / 32768.f;
}

/**
 * @brief  Check if accelerometer chip data is ready
 * @retval Ready = 1, Not ready = 0
 */
int BMI088_DataReady_ACCEL(void)
{
	return LL_GPIO_IsInputPinSet(BMI088_INT_ACCEL1_PORT, BMI088_INT_ACCEL1_PIN);
}
/**
 * @brief  Check if gyroscope chip data is ready
 * @retval Ready = 1, Not ready = 0
 */
int BMI088_DataReady_GYRO(void)
{
	return LL_GPIO_IsInputPinSet(BMI088_INT_GYRO1_PORT, BMI088_INT_GYRO1_PIN);
}
