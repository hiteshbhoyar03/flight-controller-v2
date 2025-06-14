/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		bmm350.c
 * @brief   	:		This file includes the LL driver functions for BMM350 IMU
 * ______________________________________________________________________________________________________
 */

#include "bmm350.h"
#include <stdlib.h>
#include <string.h>

Struct_BMM350 BMM350;

static void BMM350_I2C_Transmit_Data(uint8_t id, uint8_t* buffer, uint16_t len);
static void BMM350_I2C_Receive_Data(uint8_t id, uint8_t* data_addr, uint8_t len);
static void BMM350_Read_Data(uint8_t id, uint8_t reg_addr, uint8_t *data_addr, uint16_t len);
static void BMM350_Write_Data(uint8_t id, uint8_t reg_addr, uint8_t *data_addr, uint16_t len);

/**
 * @brief  Initialize the device in reset form
 * @retval 0
 */
int  BMM350_Initialization(void)
{
	BMM350_I2C_GPIO_Initialization();

	uint8_t avg_odr = (BMM350_AVG_2 | BMM350_ODR_100HZ);                               // format avg | odr
	BMM350_Write_Data(BMM350_CHIP_ID_VAL, BMM350_REG_PMU_CMD_AGGR_SET, &avg_odr, 1);   // By default avg -> 2 and odr -> 100Hz.
	HAL_Delay(1);

	uint8_t en_axis = (BMM350_EN_AXES_X | BMM350_EN_AXES_Y | BMM350_EN_AXES_Z);        // format x | y |z
	BMM350_Write_Data(BMM350_CHIP_ID_VAL, BMM350_REG_PMU_CMD_AXIS_EN, &en_axis, 1);    // By default all are enable, this is for timepass
	HAL_Delay(1);

	uint8_t cmd_config = (BMM350_PMU_CMD_NORMAL_MODE );                                // By default suspend mode
	BMM350_Write_Data(BMM350_CHIP_ID_VAL, BMM350_REG_PMU_CMD, &cmd_config, 1);
	HAL_Delay(1);
	return 0;
}

/**
 * @brief  Initialize GPIO pins for SPI communication
 * @retval No return value
 */
void BMM350_I2C_GPIO_Initialization(void)
{

}

/**
 * @brief  Send the data via I2C LL Drivers
 * @retval No return value
 * @param  id			Devidce id
 * @param  buffer		Pointer of data to SEND
 * @param  len			Length of data to SEND
 */
static void BMM350_I2C_Transmit_Data(uint8_t id, uint8_t* buffer, uint16_t len)
{
	LL_I2C_GenerateStartCondition(I2C1);                  // 1. start condition
	//	  while(!LL_I2C_IsActiveFlag_SB(I2C1));                 // 2. check start bit flag
	LL_I2C_TransmitData8(I2C1, (id << 1) | 0x00);         // 3. write device address (WRITE)
	while(!LL_I2C_IsActiveFlag_ADDR(I2C1));               // 4. wait address sent
	LL_I2C_ClearFlag_ADDR(I2C1);                          // 5. clear ADDR flag
	for(int i=0; i < len; i++)                            // 6. check TXE flag & write data
	{
		while(!LL_I2C_IsActiveFlag_TXE(I2C1));
		LL_I2C_TransmitData8(I2C1, buffer[i]);
	}
	//  while(!LL_I2C_IsActiveFlag_BTF(I2C1));                // 7. wait BTF flag (TXE flag set & empty DR condition)
	LL_I2C_GenerateStopCondition(I2C1);                   // 8. stop condition
}

/**
 * @brief  Receive the data via I2C LL Drivers
 * @retval No return value
 * @param  id			Devidce id
 * @param  data_addr	Pointer of data to SEND
 * @param  len			Length of data to RECEIVE
 */
static void BMM350_I2C_Receive_Data(uint8_t id, uint8_t* data_addr, uint8_t len)
{
	if(len == 1)                                          // 1. fast NACK configuration when receiving single byte.
	{LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);}
	else{LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);}
	LL_I2C_GenerateStartCondition(I2C1);                  // 2. start condition
	//  while(!LL_I2C_IsActiveFlag_SB(I2C1));                 // 3. check start bit flag
	LL_I2C_TransmitData8(I2C1, (id << 1) | 0x01);         // 4. write device address (READ)
	while(!LL_I2C_IsActiveFlag_ADDR(I2C1));               // 5. wait address sent
	LL_I2C_ClearFlag_ADDR(I2C1);                          // 6. clear ADDR flag
	for(int i = 0; i < len; i++)                          // 7. check RXNE flag & read data
	{
		if(i == len - 1)
		{LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);}    // 8. NACK at last byte
		while(!LL_I2C_IsActiveFlag_RXNE(I2C1));
		data_addr[i] = LL_I2C_ReceiveData8(I2C1);
	}
	LL_I2C_GenerateStopCondition(I2C1);                   // 9. stop condition
}

/**
 * @brief  Receive the data via I2C LL Drivers
 * @retval No return value
 * @param  id			Devidce id
 * @param  reg_addr		Register value
 * @param  data_addr	Pointer of data
 * @param  len			Length of data to RECEIVE
 */
static void BMM350_Read_Data(uint8_t id, uint8_t reg_addr, uint8_t *data_addr, uint16_t len)
{
	BMM350_I2C_Transmit_Data(id, &reg_addr, 1);           // send register
	BMM350_I2C_Receive_Data(id, data_addr, len);         // read data
}

/**
 * @brief  Receive the data via I2C LL Drivers
 * @retval No return value
 * @param  id			Devidce id
 * @param  reg_addr		Register value
 * @param  data_addr	Pointer of data
 * @param  len			Length of data to SEND
 */
static void BMM350_Write_Data(uint8_t id, uint8_t reg_addr, uint8_t *data_addr, uint16_t len)
{
	int8_t *buffer;
	buffer = malloc(len +1);
	buffer[0] = reg_addr;
	memcpy(buffer + 1, data_addr, len);
	BMM350_I2C_Transmit_Data(id, (uint8_t*)buffer, len + 1);  //send register + data
	free(buffer);
}

void BMM350_GetAllRawData(uint32_t* mag , uint32_t* temperature , uint32_t* sensortime)
{
	unsigned char data[15];
	BMM350_Read_Data(BMM350_CHIP_ID_VAL, BMM350_REG_MAG_X_XLSB, data, 15);

	mag[0] = (data[2]  << 16) | (data[1]  << 8) | data[0];
	mag[1] = (data[5]  << 16) | (data[4]  << 8) | data[3];
	mag[2] = (data[8]  << 16) | (data[7]  << 8) | data[6];
	temperature[0] = (data[11] << 16) | (data[10] << 8) | data[9];
	sensortime[0]  = (data[14] << 16) | (data[13] << 8) | data[12];

	// mag[0] = (uint32_t)((uint32_t)(data[2] ) << 16 | (uint32_t)(data[1] ) << 8 | data[0]);
	// mag[1] = (uint32_t)((uint32_t)(data[5] ) << 16 | (uint32_t)(data[4] ) << 8 | data[3]);
	// mag[2] = (uint32_t)((uint32_t)(data[8] ) << 16 | (uint32_t)(data[7] ) << 8 | data[6]);
	// mag[3] = (uint32_t)((uint32_t)(data[11]) << 16 | (uint32_t)(data[10]) << 8 | data[9]);
	// mag[4] = (uint32_t)((uint32_t)(data[14]) << 16 | (uint32_t)(data[13]) << 8 | data[12]);
}

void BMM350_Get3AxisMagRawData(uint32_t* mag)
{
	unsigned char data_mag[9];
	BMM350_Read_Data(BMM350_CHIP_ID_VAL, BMM350_REG_MAG_X_XLSB, data_mag, 9);

	mag[0] = (data_mag[2]  << 16) | (data_mag[1]  << 8) | data_mag[0];
	mag[1] = (data_mag[5]  << 16) | (data_mag[4]  << 8) | data_mag[3];
	mag[2] = (data_mag[8]  << 16) | (data_mag[7]  << 8) | data_mag[6];
}

void BMM350_GetTemperatureRawData(uint32_t* temperature)
{
	unsigned char data_temp[3];
	BMM350_Read_Data(BMM350_CHIP_ID_VAL, BMM350_REG_TEMP_XLSB, data_temp, 3);

	temperature[0] = (data_temp[2]  << 16) | (data_temp[1]  << 8) | data_temp[0];
}

void BMM350_GetSensortimeRawData(uint32_t* sensortime)
{
	unsigned char data_sensortime[3];
	BMM350_Read_Data(BMM350_CHIP_ID_VAL, BMM350_REG_SENSORTIME_XLSB, data_sensortime, 3);

	sensortime[0] = (data_sensortime[2]  << 16) | (data_sensortime[1]  << 8) | data_sensortime[0];
}

int BMM350_DataReady(void)
{
	return LL_GPIO_IsInputPinSet(BMM350_INT_PORT, BMM350_INT_PIN);
}

