/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		eeprom.c
 * @brief   	:		This file includes the LL driver functions for BMI088 IMU
 * ______________________________________________________________________________________________________
 */

#include "EEPROM.h"

static uint8_t SPI_EEPROM_Transmit_Receive(uint8_t data);
static uint8_t SPI_EEPROM_Transmit_Receive_32(uint32_t data);

/**
 * @brief  Initialize GPIO pins for SPI communication
 * @retval No return value
 */
void EEPROM_SPI_GPIO_Initialization(void)
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
	/**EEPROM GPIO Control Configuration
	 * PE  ------> EEPROM_SPI_nCS_PIN (output)
	 */
	LL_GPIO_ResetOutputPin(EEPROM_SPI_nCS_PORT, EEPROM_SPI_nCS_PIN);

	GPIO_InitStruct.Pin = EEPROM_SPI_nCS_PIN;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(EEPROM_SPI_nCS_PORT, &GPIO_InitStruct);

	LL_SPI_Enable(EEPROM_SPI_CHANNEL);

	EEPROM_CHIP_DESELECT(EEPROM);
}

/**
 * @brief  Initialize the device and spi communication
 * @retval No return value
 */
void EEPROM_Initialization(void)
{
	EEPROM_SPI_GPIO_Initialization();
}

/**
 * @brief  Send and receive the data via SPI LL Drivers
 * @retval Received data fro spi 
 * @param  data			Data to send
 */
static uint8_t SPI_EEPROM_Transmit_Receive(uint8_t data)
{
	LL_SPI_TransmitData8(EEPROM_SPI_CHANNEL, data);
	while(!LL_SPI_IsActiveFlag_TXC(EEPROM_SPI_CHANNEL));

	while(!LL_SPI_IsActiveFlag_RXWNE(EEPROM_SPI_CHANNEL));
	return LL_SPI_ReceiveData8(EEPROM_SPI_CHANNEL);
}
static uint8_t SPI_EEPROM_Transmit_Receive_32(uint32_t data)
{
	LL_SPI_TransmitData32(EEPROM_SPI_CHANNEL, data);
	while(!LL_SPI_IsActiveFlag_TXC(EEPROM_SPI_CHANNEL));

	while(!LL_SPI_IsActiveFlag_RXWNE(EEPROM_SPI_CHANNEL));
	return LL_SPI_ReceiveData8(EEPROM_SPI_CHANNEL);
}

/**
 * @brief  Enable Write operation
 * @retval No return value
 */
void EEPROM_WriteEnable(void)
{
	EEPROM_CHIP_SELECT(EEPROM);
	SPI_EEPROM_Transmit_Receive(EEPROM_CMD_WREN);
	EEPROM_CHIP_DESELECT(EEPROM);
}

/**
 * @brief  Read 1 byte of given Memory
 * @retval Received data
 * @param  address		Memory address to read from
 */
uint8_t EEPROM_ReadByte(uint32_t address)
{
	uint8_t ret_val;

	EEPROM_CHIP_SELECT(EEPROM);
	SPI_EEPROM_Transmit_Receive_32((EEPROM_CMD_READ << 24) | address);
	// SPI_EEPROM_Transmit_Receive(EEPROM_CMD_READ);
	// SPI_EEPROM_Transmit_Receive((uint8_t)address>>16);
	// SPI_EEPROM_Transmit_Receive((uint8_t)address>>8);
	// SPI_EEPROM_Transmit_Receive((uint8_t)address);
	ret_val = SPI_EEPROM_Transmit_Receive(0x00);		// send garbage byte to receive
	EEPROM_CHIP_DESELECT(EEPROM);
	return ret_val;
}

/**
 * @brief  Read length byte of given Memory
 * @retval No return value
 * @param  address		Memory address to start read from
 * @param  buffer		buffer pointer to store data
 * @param  length		length of data to be read
 */
void EEPROM_Read_Buffer(uint32_t address, uint8_t *buffer, uint16_t length)
{
	SPI_EEPROM_Transmit_Receive_32((EEPROM_CMD_READ << 24) | address);
	for(uint16_t i=0; i < length ; i++)	// read multiple bytes
	{
		buffer[i] = SPI_EEPROM_Transmit_Receive(0x00);
	}
}

/**
 * @brief  Write 1 byte of given Memory
 * @retval No return value
 * @param  address		Memory address to start read from
 * @param  data			data to write
 */
void EEPROM_WriteByte(uint32_t address, uint8_t data)
{
	EEPROM_WriteEnable();
	EEPROM_CHIP_SELECT(EEPROM);
	SPI_EEPROM_Transmit_Receive_32((EEPROM_CMD_PGWR << 24) | address);
	// SPI_EEPROM_Transmit_Receive(EEPROM_CMD_PGWR);
	// SPI_EEPROM_Transmit_Receive((uint8_t)address>>16);
	// SPI_EEPROM_Transmit_Receive((uint8_t)address>>8);
	// SPI_EEPROM_Transmit_Receive((uint8_t)address);
	SPI_EEPROM_Transmit_Receive(data);
	EEPROM_CHIP_DESELECT(EEPROM);
}

/**
 * @brief  Write n byte of given Memory
 * @retval No return value
 * @param  address		Memory address to start read from
 * @param  buffer		Pointer of buffer to start write from 
 * @param  length		Length of buffer
 */
void EEPROM_Write_Buffer(uint32_t address, uint8_t *buffer, uint16_t length)
{
	EEPROM_WriteEnable();

	EEPROM_CHIP_SELECT(EEPROM);
	SPI_EEPROM_Transmit_Receive_32(EEPROM_CMD_PGWR << 24 | address);
	// SPI_EEPROM_Transmit_Receive(EEPROM_CMD_PGWR);
	// SPI_EEPROM_Transmit_Receive(address);

	for(uint16_t i = 0; i < length; i++)
	{
		SPI_EEPROM_Transmit_Receive(buffer[i]);    				// Check if this is before or after if statement
		if( (address+i) % EEPROM_PAGE_SIZE == 0 && i != 0 )		// when page is changed while writing, you need to send command & address again.
		{
			EEPROM_CHIP_DESELECT(EEPROM);
			EEPROM_WriteEnable();			// write enable
			EEPROM_CHIP_SELECT(EEPROM);
			SPI_EEPROM_Transmit_Receive_32((EEPROM_CMD_PGWR << 24) | (address+i));
			//SPI_EEPROM_Transmit_Receive(EEPROM_CMD_PGWR);
			//SPI_EEPROM_Transmit_Receive(address+i);
		}
		// SPI_EEPROM_Transmit_Receive(buffer[i]);
	}
	EEPROM_CHIP_DESELECT(EEPROM);
}

/**
 * @brief  Erase all data
 * @retval No return value
 */
void EEPROM_EraseAll(void)
{
	uint32_t address = 0x00;
	EEPROM_WriteEnable();
	while(address < EEPROM_TOTAL_SIZE)
	{
		EEPROM_CHIP_SELECT(EEPROM);
		SPI_EEPROM_Transmit_Receive_32((EEPROM_CMD_PGWR<<24) | address);
		//SPI_EEPROM_Transmit_Receive(EEPROM_CMD_PGWR);
		//SPI_EEPROM_Transmit_Receive(address);
		for(uint8_t i=0; i < EEPROM_PAGE_SIZE; i++)		// send data(0x0) EEPROM_PAGE_SIZE times
		{
			SPI_EEPROM_Transmit_Receive(0x0);
		}
		EEPROM_CHIP_DESELECT(EEPROM);
		address += EEPROM_PAGE_SIZE;
	}
}

/**
 * @brief  Erase whole chip 4 Mbytes
 * @retval No return value
 */
void EEPROM_Erase_Chip(void)
{
	EEPROM_CHIP_SELECT(EEPROM);
	EEPROM_WriteEnable();
	SPI_EEPROM_Transmit_Receive(EEPROM_CMD_CHER);
	EEPROM_CHIP_DESELECT(EEPROM);
}

/**
 * @brief  Erase block 64 Kbytes
 * @param  address		Memory address to erase block
 * @retval No return value
 */
void EEPROM_Erase_Block(uint32_t address)
{
	EEPROM_CHIP_SELECT(EEPROM);
	EEPROM_WriteEnable();
	SPI_EEPROM_Transmit_Receive_32((EEPROM_CMD_BKER<<24) | address);
	EEPROM_CHIP_DESELECT(EEPROM);
}

/**
 * @brief  Erase sector 4 Kbytes
 * @param  address		Memory address to erase sector
 * @retval No return value
 */
void EEPROM_Erase_Sector(uint32_t address)
{
	EEPROM_CHIP_SELECT(EEPROM);
	EEPROM_WriteEnable();
	SPI_EEPROM_Transmit_Receive_32((EEPROM_CMD_SCER<<24) | address);
	EEPROM_CHIP_DESELECT(EEPROM);
}

/**
 * @brief  Erase page 512 bytes
 * @param  address		Memory address to erase page
 * @retval No return value
 */
void EEPROM_Erase_Page(uint32_t address)
{
	EEPROM_CHIP_SELECT(EEPROM);
	EEPROM_WriteEnable();
	SPI_EEPROM_Transmit_Receive_32((EEPROM_CMD_PGER<<24) | address);
	EEPROM_CHIP_DESELECT(EEPROM);
}

void EEPROM_PID_Gain_Write(unsigned char id, float P_Gain, float I_Gain, float D_Gain)
{
	unsigned char buf_write[16];
	Parser data;

	buf_write[0] = 0x45;
	buf_write[1] = 0x50;
	buf_write[2] = id;
	data.f = P_Gain;
	buf_write[3] = data.byte[0];	buf_write[4] = data.byte[1];
	buf_write[5] = data.byte[2];	buf_write[6] = data.byte[3];

	data.f = I_Gain;
	buf_write[7] = data.byte[0];	buf_write[8] = data.byte[1];
	buf_write[9] = data.byte[2];	buf_write[10] = data.byte[3];

	data.f = D_Gain;
	buf_write[11] = data.byte[0];	buf_write[12] = data.byte[1];
	buf_write[13] = data.byte[2];	buf_write[14] = data.byte[3];

	unsigned char checksum = 0xff;
	for(int i=0;i<15;i++)
		checksum -= buf_write[i];

	buf_write[15] = checksum;

	switch(id)
	{
	case 0:	EEPROM_Write_Buffer(0x00, &buf_write[0], 16);	break;
	case 1:	EEPROM_Write_Buffer(0x01, &buf_write[0], 16);	break;
	case 2:	EEPROM_Write_Buffer(0x02, &buf_write[0], 16);	break;
	case 3:	EEPROM_Write_Buffer(0x03, &buf_write[0], 16);	break;
	case 4:	EEPROM_Write_Buffer(0x04, &buf_write[0], 16);	break;
	case 5:	EEPROM_Write_Buffer(0x05, &buf_write[0], 16);	break;
	}
}

unsigned char EEPROM_PID_Gain_Read(unsigned char id, float* P_Gain, float* I_Gain, float* D_Gain)
{
	unsigned char buf_read[16];
	Parser data;

	switch(id)
	{
	case 0:	EEPROM_Read_Buffer(0x00, &buf_read[0], 16);		break;
	case 1: EEPROM_Read_Buffer(0x01, &buf_read[0], 16);		break;
	case 2: EEPROM_Read_Buffer(0x02, &buf_read[0], 16);		break;
	case 3: EEPROM_Read_Buffer(0x03, &buf_read[0], 16);		break;
	case 4:	EEPROM_Read_Buffer(0x04, &buf_read[0], 16);		break;
	case 5:	EEPROM_Read_Buffer(0x05, &buf_read[0], 16);		break;
	}

	unsigned char checksum = 0xff;
	for(int i=0;i<15;i++)
		checksum -= buf_read[i];

	if(buf_read[15] == checksum && buf_read[0] == 0x45 && buf_read[1] == 0x50)
	{
		data.byte[0] = buf_read[3];		data.byte[1] = buf_read[4];
		data.byte[2] = buf_read[5];		data.byte[3] = buf_read[6];
		*P_Gain = data.f;

		data.byte[0] = buf_read[7];		data.byte[1] = buf_read[8];
		data.byte[2] = buf_read[9];		data.byte[3] = buf_read[10];
		*I_Gain = data.f;

		data.byte[0] = buf_read[11];	data.byte[1] = buf_read[12];
		data.byte[2] = buf_read[13];	data.byte[3] = buf_read[14];
		*D_Gain = data.f;

		return 0;
	}
	return 1;
}
