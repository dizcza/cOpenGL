/*
 * depth_sdram.c
 *
 *  Created on: Apr 2, 2017
 *      Author: dizcza
 */

#include "main.h"
#include "stdint.h"
#include "depth_sdram.h"
#include "stm32f4xx_hal.h"
#include "debug_printf.h"

#define DMA_BUFFER_MAX_SZ          0xFFFF
#define DEPTH_DMA_WAIT_READY_MS    10

/* Private function prototypes -----------------------------------------------*/
static void DMA_ConfigWrite(void);
static void WriteTransferComplete(DMA_HandleTypeDef *DmaHandle);
static void WriteTransferError(DMA_HandleTypeDef *DmaHandle);
static void WriteTransferAbort(DMA_HandleTypeDef *DmaHandle);
static uint32_t inline Depth_SDRAM_GetPixelAddress(uint16_t x, uint16_t y);
static uint16_t inline GetLcdHeight();
static HAL_DMA_StateTypeDef WaitReadyDma(const DMA_HandleTypeDef *DmaHandle, uint32_t timeout_ms);
static void Depth_SDRAM_TestReadWrite();
static void Depth_SDRAM_TestDmaWrite();
static void Depth_SDRAM_TestDmaFinished();

static SDRAM_HandleTypeDef* m_SdramHandle;
DMA_HandleTypeDef Depth_DmaHandleWrite;

static uint16_t m_LcdWidth;
static uint32_t m_LcdArea;
static uint32_t m_DeviceAddress;
static uint32_t m_DmaAlreadyClearedPixels = 0;
static uint32_t m_DmaLastPixChunkSize = 0;
static uint32_t m_DmaBufferWriteSz = DMA_BUFFER_MAX_SZ;

static __I const float m_depth_initial = 1.0f;

void Depth_SDRAM_Init(SDRAM_HandleTypeDef* sdramHandle, uint32_t startAddress,
		uint16_t lcd_width, uint16_t lcd_height) {
	m_SdramHandle = sdramHandle;
	m_DeviceAddress = startAddress;
	m_LcdWidth = lcd_width;
	m_LcdArea = lcd_width * lcd_height;
	if (m_LcdArea < m_DmaBufferWriteSz) {
		m_DmaBufferWriteSz = m_LcdArea;
	}
	DMA_ConfigWrite();
#ifdef USE_ASSERT_EXPR
	Depth_SDRAM_TestReadWrite();
	Depth_SDRAM_TestDmaWrite();
#endif /* USE_ASSERT_EXPR */
}

static uint32_t inline Depth_SDRAM_GetPixelAddress(uint16_t x, uint16_t y) {
	return m_DeviceAddress + 4 * (y * m_LcdWidth + x);
}

static uint16_t inline GetLcdHeight() {
	return m_LcdArea / m_LcdWidth;
}

void DMA2_Stream1_IRQHandler() {
	HAL_DMA_IRQHandler(&Depth_DmaHandleWrite);
}

static HAL_DMA_StateTypeDef WaitReadyDma(const DMA_HandleTypeDef *DmaHandle, uint32_t timeout_ms) {
	const uint32_t start = HAL_GetTick();
	while (DmaHandle->State != HAL_DMA_STATE_READY && (HAL_GetTick() - start < timeout_ms)) {
		// just wait
	}
	return DmaHandle->State;
}

static void DMA_ConfigWrite(void)
{
  /*## -1- Enable DMA2 clock #################################################*/
  __HAL_RCC_DMA2_CLK_ENABLE();

  /*##-2- Select the DMA functional Parameters ###############################*/
  Depth_DmaHandleWrite.Init.Channel = DMA_CHANNEL_0;
  Depth_DmaHandleWrite.Init.Direction = DMA_MEMORY_TO_MEMORY;
  Depth_DmaHandleWrite.Init.PeriphInc = DMA_PINC_DISABLE;
  Depth_DmaHandleWrite.Init.MemInc = DMA_MINC_ENABLE;
  Depth_DmaHandleWrite.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  Depth_DmaHandleWrite.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  Depth_DmaHandleWrite.Init.Mode = DMA_NORMAL;
  Depth_DmaHandleWrite.Init.Priority = DMA_PRIORITY_HIGH;
  Depth_DmaHandleWrite.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  Depth_DmaHandleWrite.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  Depth_DmaHandleWrite.Init.MemBurst = DMA_MBURST_SINGLE;
  Depth_DmaHandleWrite.Init.PeriphBurst = DMA_PBURST_SINGLE;

  /*##-3- Select the DMA instance to be used for the transfer */
  Depth_DmaHandleWrite.Instance = DMA2_Stream1;

  /*##-4- Initialize the DMA stream ##########################################*/
  HAL_StatusTypeDef dmaInitStatus = HAL_DMA_Init(&Depth_DmaHandleWrite);
  assert_expr(dmaInitStatus == HAL_OK);

  /*##-5- Select Callbacks functions called after Transfer complete and Transfer error */
  HAL_DMA_RegisterCallback(&Depth_DmaHandleWrite, HAL_DMA_XFER_CPLT_CB_ID, WriteTransferComplete);
  HAL_DMA_RegisterCallback(&Depth_DmaHandleWrite, HAL_DMA_XFER_ERROR_CB_ID, WriteTransferError);
  HAL_DMA_RegisterCallback(&Depth_DmaHandleWrite, HAL_DMA_XFER_ABORT_CB_ID, WriteTransferAbort);

  /*##-6- Configure NVIC for DMA transfer complete/error interrupts ##########*/
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
}


static void WriteTransferComplete(DMA_HandleTypeDef *DmaHandle)
{
	m_DmaAlreadyClearedPixels += m_DmaLastPixChunkSize;
	if (m_DmaAlreadyClearedPixels < m_LcdArea) {
		Depth_SDRAM_ClearDepth();
	} else {
		m_DmaAlreadyClearedPixels = 0;
#ifdef USE_ASSERT_EXPR
		Depth_SDRAM_TestDmaFinished();
#endif /* USE_ASSERT_EXPR */
	}
}

static void WriteTransferError(DMA_HandleTypeDef *DmaHandle)
{
  assert_expr(0);
}

static void WriteTransferAbort(DMA_HandleTypeDef *DmaHandle)
{
  assert_expr(0);
}


void Depth_SDRAM_WriteDepth(uint16_t x, uint16_t y, float depth) {
	HAL_DMA_StateTypeDef dmaStatus = WaitReadyDma(&Depth_DmaHandleWrite, DEPTH_DMA_WAIT_READY_MS);
	assert_expr(dmaStatus == HAL_DMA_STATE_READY);
	*(__IO float *)(Depth_SDRAM_GetPixelAddress(x, y)) = depth;
}

float Depth_SDRAM_ReadDepth(uint16_t x, uint16_t y) {
	return *(__IO float *)(Depth_SDRAM_GetPixelAddress(x, y));
}

void Depth_SDRAM_ClearDepth() {
	HAL_DMA_StateTypeDef dmaStatus = WaitReadyDma(&Depth_DmaHandleWrite, DEPTH_DMA_WAIT_READY_MS);
	assert_expr(dmaStatus == HAL_DMA_STATE_READY);
	m_DmaLastPixChunkSize = m_LcdArea - m_DmaAlreadyClearedPixels;
	if (m_DmaLastPixChunkSize > m_DmaBufferWriteSz) {
		m_DmaLastPixChunkSize = m_DmaBufferWriteSz;
	}
	HAL_StatusTypeDef status = HAL_DMA_Start_IT(&Depth_DmaHandleWrite,
								  (uint32_t)&(m_depth_initial),
								  (uint32_t)(m_DeviceAddress + 4 * m_DmaAlreadyClearedPixels),
								  m_DmaLastPixChunkSize);
	assert_expr(status == HAL_OK);
}

static void Depth_SDRAM_TestReadWrite() {
	uint16_t x, y;
	uint16_t m_lcd_height = GetLcdHeight();
	for (y = 0; y < m_lcd_height; ++y) {
		for (x = 0; x < m_LcdWidth; ++x) {
			float depth_ref = -1.0f * ((float) (m_LcdWidth - x)) / m_LcdWidth
					+ 1.0f * ((float) x) / m_LcdWidth;
			Depth_SDRAM_WriteDepth(x, y, depth_ref);
			float depth_returned = Depth_SDRAM_ReadDepth(x, y);
			float delta = depth_ref - depth_returned;
			delta = delta > 0 ? delta : -delta;
			assert_expr(delta < 1e-5);
		}
	}
}

static void Depth_SDRAM_TestDmaFinished() {
	uint16_t x, y;
	uint16_t m_lcd_height = GetLcdHeight();
	for (y = 0; y < m_lcd_height; ++y) {
		for (x = 0; x < m_LcdWidth; ++x) {
			float depth = Depth_SDRAM_ReadDepth(x, y);
			float delta = depth - m_depth_initial;
			delta = delta > 0 ? delta : -delta;
			if (delta > 1e-5) {
				LCD_Printf("x=%d y=%d d=%f", x, y, depth);
				BSP_LCD_DrawPixel(x, y, 0xFF000000);
				BSP_LCD_DrawCircle(x, y, 10);
				while(1);
			}
			assert_expr(delta < 1e-5);
		}
	}
}

static void Depth_SDRAM_TestDmaWrite() {
	uint16_t x, y;
	uint16_t m_lcd_height = GetLcdHeight();
	for (y = 0; y < m_lcd_height; ++y) {
		for (x = 0; x < m_LcdWidth; ++x) {
			Depth_SDRAM_WriteDepth(x, y, -2.f);
		}
	}
	Depth_SDRAM_ClearDepth();
}
