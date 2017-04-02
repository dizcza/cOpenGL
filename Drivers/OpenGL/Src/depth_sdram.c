/*
 * depth_sdram.c
 *
 *  Created on: Apr 2, 2017
 *      Author: dizcza
 */

#include "depth_sdram.h"

static SDRAM_HandleTypeDef* m_SdramHandle;
static uint16_t m_lcd_width, m_lcd_height;
static uint32_t m_DeviceAddress;

void Depth_SDRAM_Init(SDRAM_HandleTypeDef* sdramHandle, uint32_t startAddress,
		uint16_t lcd_width, uint16_t lcd_height) {
	m_SdramHandle = sdramHandle;
	m_DeviceAddress = startAddress;
	m_lcd_width = lcd_width;
	m_lcd_height = lcd_height;
}

static uint32_t inline Depth_SDRAM_GetStartAddress(uint16_t x, uint16_t y) {
	return m_DeviceAddress + 4 * (y * m_lcd_width + x);
}

/**
 * @brief  Reads an mount of data from the SDRAM memory in polling mode.
 * @param  uwStartAddress : Read start address
 * @param  pData : Pointer to data to be read
 * @param  uwDataSize: Size of read data from the memory
 */
static void Depth_SDRAM_ReadData(uint32_t uwStartAddress, uint32_t *pData,
		uint32_t uwDataSize) {
	HAL_SDRAM_Read_32b(m_SdramHandle, (uint32_t *) uwStartAddress, pData,
			uwDataSize);
}

/**
 * @brief  Reads an mount of data from the SDRAM memory in DMA mode.
 * @param  uwStartAddress : Read start address
 * @param  pData : Pointer to data to be read
 * @param  uwDataSize: Size of read data from the memory
 */
static void Depth_SDRAM_ReadData_DMA(uint32_t uwStartAddress, uint32_t *pData,
		uint32_t uwDataSize) {
	HAL_SDRAM_Read_DMA(m_SdramHandle, (uint32_t *) uwStartAddress, pData,
			uwDataSize);
}

/**
 * @brief  Writes an mount of data to the SDRAM memory in polling mode.
 * @param  uwStartAddress : Write start address
 * @param  pData : Pointer to data to be written
 * @param  uwDataSize: Size of written data from the memory
 */
static void Depth_SDRAM_WriteData(uint32_t uwStartAddress, uint32_t *pData,
		uint32_t uwDataSize) {
	/* Disable write protection */
	HAL_SDRAM_WriteProtection_Disable(m_SdramHandle);

	/*Write 32-bit data buffer to SDRAM memory*/
	HAL_SDRAM_Write_32b(m_SdramHandle, (uint32_t *) uwStartAddress, pData,
			uwDataSize);
}

/**
 * @brief  Writes an mount of data to the SDRAM memory in DMA mode.
 * @param  uwStartAddress : Write start address
 * @param  pData : Pointer to data to be written
 * @param  uwDataSize: Size of written data from the memory
 */
static void Depth_SDRAM_WriteData_DMA(uint32_t uwStartAddress, uint32_t *pData,
		uint32_t uwDataSize) {
	HAL_SDRAM_Write_DMA(m_SdramHandle, (uint32_t *) uwStartAddress, pData,
			uwDataSize);
}

void Depth_SDRAM_WriteDepth(uint16_t x, uint16_t y, float depth) {
	uint32_t uwStartAddress = Depth_SDRAM_GetStartAddress(x, y);
	Depth_SDRAM_WriteData(uwStartAddress, (uint32_t *) &depth, 1);
}

float Depth_SDRAM_ReadDepth(uint16_t x, uint16_t y) {
	uint32_t uwStartAddress = Depth_SDRAM_GetStartAddress(x, y);
	float depth;
	Depth_SDRAM_ReadData(uwStartAddress, (uint32_t *) &depth, 1);
	return depth;
}

void Depth_SDRAM_TestReadWrite() {
	uint16_t x, y;
	for (y = 0; y < m_lcd_height; ++y) {
		for (x = 0; x < m_lcd_width; ++x) {
			float depth = -1.0f * ((float) (m_lcd_width - x)) / m_lcd_width
					+ 1.0f * ((float) x) / m_lcd_width;
			Depth_SDRAM_WriteDepth(x, y, depth);
			float depth_returned = Depth_SDRAM_ReadDepth(x, y);
			float delta = depth - depth_returned;
			delta = delta > 0 ? delta : -delta;
			if (delta > 1e-5) {
				BSP_LED_On(LED4);
			}
		}
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
