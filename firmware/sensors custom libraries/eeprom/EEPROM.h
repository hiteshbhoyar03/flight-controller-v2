/**
 * ______________________________________________________________________________________________________
 * @author		:		HITESH BHOYAR
 * @file    	:		eeprom.h
 * @brief   	:		EEPROM M95P32
 * ______________________________________________________________________________________________________
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __EEPROM_H__
#define __EEPROM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Definitions ---------------------------------------------------------------*/
#define EEPROM_SPI_CHANNEL		SPI5

#define EEPROM_SPI_SCLK_PIN		LL_GPIO_PIN_0
#define EEPROM_SPI_SCLK_PORT	GPIOK
#define EEPROM_SPI_SCLK_CLK		LL_AHB4_GRP1_PERIPH_GPIOK

#define EEPROM_SPI_MISO_PIN		LL_GPIO_PIN_8
#define EEPROM_SPI_MISO_PORT	GPIOF
#define EEPROM_SPI_MISO_CLK		LL_AHB4_GRP1_PERIPH_GPIOF

#define EEPROM_SPI_MOSI_PIN		LL_GPIO_PIN_10
#define EEPROM_SPI_MOSI_PORT	GPIOJ
#define EEPROM_SPI_MOSI_CLK		LL_AHB4_GRP1_PERIPH_GPIOJ

#define EEPROM_SPI_nCS_PIN		LL_GPIO_PIN_10
#define EEPROM_SPI_nCS_PORT		GPIOG
#define EEPROM_SPI_nCS_CLK		LL_AHB4_GRP1_PERIPH_GPIOG

#define EEPROM_CHIP_SELECT(EEPROM)		LL_GPIO_ResetOutputPin(EEPROM_SPI_nCS_PORT, EEPROM_SPI_nCS_PIN)
#define EEPROM_CHIP_DESELECT(EEPROM)	LL_GPIO_SetOutputPin(  EEPROM_SPI_nCS_PORT, EEPROM_SPI_nCS_PIN)

/* Define Instructions -------------------------------------------------------*/
#define EEPROM_CMD_PGWR         UINT8_C(0x02)   // Page write (erase and program)
#define EEPROM_CMD_READ		    UINT8_C(0x03)   // Read data single output
#define EEPROM_CMD_FREAD		UINT8_C(0x0B)   // Fast read single output with one dummy byte
#define EEPROM_CMD_WREN		    UINT8_C(0x06)   // Write enable
#define EEPROM_CMD_WRDI		    UINT8_C(0x04)   // Write disable
#define EEPROM_CMD_PGER		    UINT8_C(0xDB)   // Page erase (512 bytes)
#define EEPROM_CMD_SCER		    UINT8_C(0x20)   // Sector erase (4 Kbytes)
#define EEPROM_CMD_BKER		    UINT8_C(0xD8)   // Block erase (64 Kbytes)
#define EEPROM_CMD_CHER		    UINT8_C(0xC7)   // Chip erase
#define EEPROM_CMD_RSTEN		UINT8_C(0x66)   // Enable reset
#define EEPROM_CMD_RESET	    UINT8_C(0x99)   // Software reset

/* Define Macros -------------------------------------------------------------*/
#define EEPROM_PAGE_SIZE		512   		// 0x0200
#define EEPROM_TOTAL_SIZE		4194304 	// 0x400000

/* Define Union --------------------------------------------------------------*/
typedef union _Parser
{
	unsigned char byte[4];
	float f;
}Parser;

/* Read and Write Function ---------------------------------------------------*/
uint8_t EEPROM_ReadByte(uint32_t address);
void EEPROM_Read_Buffer(uint32_t address, uint8_t *buffer, uint16_t length);
void EEPROM_WriteByte(uint32_t address, uint8_t data);
void EEPROM_Write_Buffer(uint32_t address, uint8_t *buffer, uint16_t length);

/* External Configuration Function -------------------------------------------*/
void EEPROM_WriteEnable(void);
void EEPROM_SPI_GPIO_Initialization(void);
void EEPROM_Initialization(void);
void EEPROM_EraseAll(void);
void EEPROM_Erase_Chip(void);
void EEPROM_Erase_Block(uint32_t address);
void EEPROM_Erase_Sector(uint32_t address);
void EEPROM_Erase_PAGE(uint32_t address);
void EEPROM_PID_Gain_Write(unsigned char id, float P_Gain, float I_Gain, float D_Gain);
unsigned char EEPROM_PID_Gain_Read(unsigned char id, float* P_Gain, float* I_Gain, float* D_Gain);

#ifdef __cplusplus
}
#endif

#endif /* __EEPROM_H__ */
