/*
 * depth_sdram.c
 *
 *  Created on: Apr 2, 2017
 *      Author: dizcza
 */

#include "main.h"
#include "depth_sdram.h"
#include "stm32f4xx_hal.h"

#define DMA_STREAM                 DMA2_Stream1
#define DMA_CHANNEL                DMA_CHANNEL_1
#define DMA_STREAM_IRQ             DMA2_Stream1_IRQn
#define DMA_STREAM_IRQHANDLER      DMA2_Stream1_IRQHandler

#define DEPTH_DMA_FLOAT_BUFFER_MAX_SIZE  0x3FFF  // (0xFFFF / 4)
#define DEPTH_DMA_WAIT_READY_MS          10

/* Private function prototypes -----------------------------------------------*/
static void DMA_Config(void);
static void TransferComplete(DMA_HandleTypeDef *DmaHandle);
static void TransferError(DMA_HandleTypeDef *DmaHandle);
static void TransferAbort(DMA_HandleTypeDef *DmaHandle);
static uint32_t inline Depth_SDRAM_GetPixelAddress(uint16_t x, uint16_t y);
static HAL_DMA_StateTypeDef WaitReadyDma(const DMA_HandleTypeDef *DmaHandle, uint32_t timeout_ms);
static void Depth_SDRAM_TestReadWrite();
static void Depth_SDRAM_TestDmaWrite();

static SDRAM_HandleTypeDef* m_SdramHandle;
static DMA_HandleTypeDef m_DmaHandle;

static uint16_t m_LcdWidth;
static uint32_t m_LcdArea;
static uint32_t m_DeviceAddress;
static uint32_t m_DmaAlreadyClearedPixels = 0;
static uint32_t m_DmaLastPixChunkSize = 0;

static __I const float m_depth_initial = 1.0f;

void Depth_SDRAM_Init(SDRAM_HandleTypeDef* sdramHandle, uint32_t startAddress,
		uint16_t lcd_width, uint16_t lcd_height) {
	m_SdramHandle = sdramHandle;
	m_DeviceAddress = startAddress;
	m_LcdWidth = lcd_width;
	m_LcdArea = lcd_width * lcd_height;
	DMA_Config();
#ifdef USE_ASSERT_EXPR
	Depth_SDRAM_TestReadWrite();
	Depth_SDRAM_TestDmaWrite();
#endif /* USE_ASSERT_EXPR */
}

static uint32_t inline Depth_SDRAM_GetPixelAddress(uint16_t x, uint16_t y) {
	return m_DeviceAddress + 4 * (y * m_LcdWidth + x);
}

void DMA2_Stream1_IRQHandler() {
	HAL_DMA_IRQHandler(&m_DmaHandle);
}

static HAL_DMA_StateTypeDef WaitReadyDma(const DMA_HandleTypeDef *DmaHandle, uint32_t timeout_ms) {
	const uint32_t start = HAL_GetTick();
	while (DmaHandle->State != HAL_DMA_STATE_READY && (HAL_GetTick() - start < timeout_ms)) {
		// just wait
	}
	return DmaHandle->State;
}

static void DMA_Config(void)
{
  /*## -1- Enable DMA2 clock #################################################*/
  __HAL_RCC_DMA2_CLK_ENABLE();

  /*##-2- Select the DMA functional Parameters ###############################*/
  m_DmaHandle.Init.Channel = DMA_CHANNEL;                     /* DMA_CHANNEL_1                    */
  m_DmaHandle.Init.Direction = DMA_MEMORY_TO_MEMORY;          /* M2M transfer mode                */
  m_DmaHandle.Init.PeriphInc = DMA_PINC_DISABLE;              /* Peripheral increment mode Disable */
  m_DmaHandle.Init.MemInc = DMA_MINC_ENABLE;                  /* Memory increment mode Enable     */
  m_DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD; /* Peripheral data alignment : Word */
  m_DmaHandle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;    /* memory data alignment : Word     */
  m_DmaHandle.Init.Mode = DMA_NORMAL;                         /* Normal DMA mode                  */
  m_DmaHandle.Init.Priority = DMA_PRIORITY_HIGH;              /* priority level : high            */
  m_DmaHandle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;           /* FIFO mode disabled               */
  m_DmaHandle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
  m_DmaHandle.Init.MemBurst = DMA_MBURST_SINGLE;              /* Memory burst                     */
  m_DmaHandle.Init.PeriphBurst = DMA_PBURST_SINGLE;           /* Peripheral burst                 */

  /*##-3- Select the DMA instance to be used for the transfer : DMA2_Stream1 #*/
  m_DmaHandle.Instance = DMA_STREAM;

  /*##-4- Initialize the DMA stream ##########################################*/
  HAL_StatusTypeDef dmaInitStatus = HAL_DMA_Init(&m_DmaHandle);
  assert_expr(dmaInitStatus == HAL_OK);

  /*##-5- Select Callbacks functions called after Transfer complete and Transfer error */
  HAL_DMA_RegisterCallback(&m_DmaHandle, HAL_DMA_XFER_CPLT_CB_ID, TransferComplete);
  HAL_DMA_RegisterCallback(&m_DmaHandle, HAL_DMA_XFER_ERROR_CB_ID, TransferError);
  HAL_DMA_RegisterCallback(&m_DmaHandle, HAL_DMA_XFER_ABORT_CB_ID, TransferAbort);

  /*##-6- Configure NVIC for DMA transfer complete/error interrupts ##########*/
  HAL_NVIC_SetPriority(DMA_STREAM_IRQ, 0, 0);
  HAL_NVIC_EnableIRQ(DMA_STREAM_IRQ);
}

/**
  * @brief  DMA conversion complete callback
  * @note   This function is executed when the transfer complete interrupt
  *         is generated
  * @retval None
  */
static void TransferComplete(DMA_HandleTypeDef *DmaHandle)
{
	m_DmaAlreadyClearedPixels += m_DmaLastPixChunkSize;
	if (m_DmaAlreadyClearedPixels < m_LcdArea) {
		Depth_SDRAM_ClearDepth();
	} else {
		m_DmaAlreadyClearedPixels = 0;
	}
}

/**
  * @brief  DMA conversion error callback
  * @note   This function is executed when the transfer error interrupt
  *         is generated during DMA transfer
  * @retval None
  */
static void TransferError(DMA_HandleTypeDef *DmaHandle)
{
  assert_expr(0);
}

static void TransferAbort(DMA_HandleTypeDef *DmaHandle)
{
  assert_expr(0);
}

void Depth_SDRAM_WriteDepth(uint16_t x, uint16_t y, float depth) {
	*(__IO float *)(Depth_SDRAM_GetPixelAddress(x, y)) = depth;
}

float Depth_SDRAM_ReadDepth(uint16_t x, uint16_t y) {
	return *(__IO float *)(Depth_SDRAM_GetPixelAddress(x, y));
}

void Depth_SDRAM_ClearDepth() {
	HAL_DMA_StateTypeDef dmaStatus = WaitReadyDma(&m_DmaHandle, DEPTH_DMA_WAIT_READY_MS);
	assert_expr(dmaStatus == HAL_DMA_STATE_READY);
	m_DmaLastPixChunkSize = m_LcdArea - m_DmaAlreadyClearedPixels;
	if (m_DmaLastPixChunkSize > DEPTH_DMA_FLOAT_BUFFER_MAX_SIZE) {
		m_DmaLastPixChunkSize = DEPTH_DMA_FLOAT_BUFFER_MAX_SIZE;
	}
	HAL_StatusTypeDef status = HAL_DMA_Start_IT(&m_DmaHandle,
								  (uint32_t)&(m_depth_initial),
								  (uint32_t)(m_DeviceAddress + 4 * m_DmaAlreadyClearedPixels),
								  m_DmaLastPixChunkSize);
	assert_expr(status == HAL_OK);
}

static void Depth_SDRAM_TestReadWrite() {
	uint16_t x, y;
	uint32_t m_lcd_height = m_LcdArea / m_LcdWidth;
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

static void Depth_SDRAM_TestDmaWrite() {
	uint16_t x, y;
	uint32_t m_lcd_height = m_LcdArea / m_LcdWidth;
	for (y = 0; y < m_lcd_height; ++y) {
		for (x = 0; x < m_LcdWidth; ++x) {
			Depth_SDRAM_WriteDepth(x, y, -2.f);
		}
	}
	Depth_SDRAM_ClearDepth();
	for (y = 0; y < m_lcd_height; ++y) {
		for (x = 0; x < m_LcdWidth; ++x) {
			float depth = Depth_SDRAM_ReadDepth(x, y);
			float delta = depth - m_depth_initial;
			delta = delta > 0 ? delta : -delta;
			assert_expr(delta < 1e-5);
		}
	}
}
